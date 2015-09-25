package com.elex.chatservice;

import android.content.Context;
import android.content.Intent;
import android.database.SQLException;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import com.elex.chatservice.exceptions.IdentityException;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSFormat;
import com.elex.chatservice.util.Meta;

import java.io.File;
import java.lang.reflect.Method;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSApiData
{
  public HSStorage storage;
  public HSApiClient client;
  private HSSectionDataSource sectionDataSource;
  private HSFaqDataSource faqDataSource;
  public static final String TAG = "HelpShiftDebug";
  private ArrayList<Faq> flatFaqList = null;

  private final int FILE_STORE_LIMIT = 10;
  private Context c;
  Iterator failedApiKeys = null;
  public static final int MARK_QUESTION = 0;
  public static final int PUSH_TOKEN = 1;
  public static final int ACTION_EVENTS = 2;
  public static final int MESSAGE_SEEN = 3;
  public static ArrayList<HSFaqSyncStatusEvents> observers = null;
  protected static boolean faqsSyncing = false;

  public HSApiData(Context c) { this.c = c;
    this.storage = new HSStorage(c);
    this.client = new HSApiClient(this.storage.getDomain(), this.storage.getAppId(), this.storage.getApiKey(), this.storage);

    this.sectionDataSource = new HSSectionDataSource(c);
    this.faqDataSource = new HSFaqDataSource(c);
  }

  protected void install(String apiKey, String domain, String appId)
  {
    this.storage.setApiKey(apiKey);
    this.storage.setDomain(domain);
    this.storage.setAppId(appId);

    this.client = new HSApiClient(domain, appId, apiKey, this.storage);
  }

  private void updateFlatList()
  {
    ArrayList sections = getSections();
    this.flatFaqList = new ArrayList();

    for (int i = 0; i < sections.size(); i++) {
      Section sectionItem = (Section)sections.get(i);
      ArrayList faqs = getFaqsDataForSection(sectionItem.getPublishId());
      for (int j = 0; j < faqs.size(); j++) {
        Faq faq = (Faq)faqs.get(j);
        this.flatFaqList.add(faq);
      }
    }
  }

  private void getAndStoreSections(final Handler callback, final Handler failure)
    throws SQLException
  {
    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        HashMap result = (HashMap)msg.obj;

        if (result != null) {
          JSONArray faqs = (JSONArray)result.get("response");

          sectionDataSource.clearSectionsData();
          sectionDataSource.storeSections(faqs);
          Message msgToPost = callback.obtainMessage();
          msgToPost.obj = sectionDataSource.getAllSections();
          callback.sendMessage(msgToPost);
          Thread indexThread = new Thread(new Runnable() {
            public void run() {
              Log.d("HelpShiftDebug", "Started indexing");
              updateIndex();

              HSApiData.signalSearchIndexesUpdated();
              Log.d("HelpShiftDebug", "finished indexing");
            }
          });
          indexThread.setDaemon(true);
          indexThread.start();
        }

        HSApiData.signalFaqsUpdated();
      }
    };
    Handler localFailure = new Handler()
    {
      public void handleMessage(Message msg) {
        HashMap result = (HashMap)msg.obj;
        HSApiData.faqsSyncing = false;
        Message msgToPost = failure.obtainMessage();
        msgToPost.obj = result;
        failure.sendMessage(msgToPost);
      }
    };
    faqsSyncing = true;
    this.client.fetchFaqs(localSuccess, localFailure);
  }

  private void getAndStoreConfig(final Handler callback, Handler failure)
    throws JSONException
  {
    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        HashMap result = (HashMap)msg.obj;

        if (result != null) {
          JSONObject config = (JSONObject)result.get("response");
          if (storage.getBreadCrumbsLimit().intValue() != config.optInt("bcl", 10))
            storage.updateBreadCrumbsLimit(Integer.valueOf(config.optInt("bcl", 10)));
          try
          {
            JSONObject storedConfig = storage.getConfig();
            JSONObject storedReviewConfig = storedConfig.optJSONObject("pr");
            JSONObject reviewConfig = config.optJSONObject("pr");
            if ((storedReviewConfig != null) && (!storedReviewConfig.getString("t").equals(reviewConfig.getString("t"))))
            {
              resetReviewCounter();
            }
          } catch (JSONException e) {
            Log.d("HelpShiftDebug", "Reseting counter", e);
          }
          storage.setConfig(config);
          Message msgToPost = callback.obtainMessage();
          msgToPost.obj = config;
          callback.sendMessage(msgToPost);
        }
      }
    };
    this.client.getConfig(localSuccess, failure);
  }

  protected void getSections(Handler callback, Handler failure)
  {
    ArrayList sections = null;
    try
    {
      sections = (ArrayList)this.sectionDataSource.getAllSections();
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting sections data ", s);
    }

    if ((sections != null) && (sections.size() > 0)) {
      Message result = callback.obtainMessage();
      result.obj = sections;
      callback.sendMessage(result);
      getAndStoreSections(callback, failure);
    } else {
      getAndStoreSections(callback, failure);
    }
  }

  protected ArrayList<Section> getSections() {
    ArrayList sections = null;
    try {
      sections = (ArrayList)this.sectionDataSource.getAllSections();
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting sections data ", s);
    }
    return sections;
  }

  protected ArrayList<Section> getPopulatedSections(ArrayList<Section> sections) {
    ArrayList sectionsList = new ArrayList();
    for (int i = 0; i < sections.size(); i++) {
      if (!isSectionEmpty((Section)sections.get(i))) {
        sectionsList.add(sections.get(i));
      }
    }
    return sectionsList;
  }

  protected ArrayList<Section> getPopulatedSections() {
    ArrayList sections = null;
    ArrayList sectionsList = new ArrayList();
    try {
      sections = (ArrayList)this.sectionDataSource.getAllSections();
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting sections data ", s);
    }
    sectionsList = getPopulatedSections(sections);
    return sectionsList;
  }

  protected boolean isSectionEmpty(Section section) {
    ArrayList faqs = getFaqsForSection(section.getPublishId());
    return faqs.isEmpty();
  }

  protected ArrayList getFaqsForSection(String publishId) {
    ArrayList faqs = new ArrayList();
    try {
      faqs = (ArrayList)this.faqDataSource.getFaqsForSection(publishId);
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting faqs for section", s);
    }
    return faqs;
  }

  protected ArrayList getFaqsDataForSection(String publishId) {
    ArrayList faqs = new ArrayList();
    try {
      faqs = (ArrayList)this.faqDataSource.getFaqsDataForSection(publishId);
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting faqs for section", s);
    }
    return faqs;
  }

  protected void getConfig(Handler callback, Handler failure) throws JSONException
  {
    JSONObject storedConfig = this.storage.getConfig();
    if (storedConfig.length() != 0) {
      Message msgToPost = callback.obtainMessage();
      msgToPost.obj = storedConfig;
      callback.sendMessage(msgToPost);
    }
    getAndStoreConfig(callback, failure);
  }

  private void getAndStoreIssues(Handler callback, Handler failure, String identity, String lastTs, String mc, Boolean onlyNew)
    throws JSONException
  {
    getAndStoreIssues(callback, failure, identity, lastTs, mc, onlyNew, null);
  }

  private void getAndStoreIssues(final Handler callback, final Handler failure, String identity, String lastTs, String mc, final Boolean onlyNew, String chatLaunchSource)
    throws JSONException
  {
    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        try {
          HashMap result = (HashMap)msg.obj;
          JSONObject issuesResult = (JSONObject)result.get("response");
          String dateStr = issuesResult.getString("timestamp");
          JSONArray issues = issuesResult.getJSONArray("issues");
          storage.setIssuesTs(dateStr);

          if (issues.length() > 0) {
            storage.storeIssues(issues);
          }

          Message msgToPost = callback.obtainMessage();
          if (onlyNew.booleanValue()) {
            msgToPost.obj = issues;
            if (issues.length() > 0) {
              callback.sendMessage(msgToPost);
            }
            else
              sendFailMessage(failure, -1);
          }
          else {
            msgToPost.obj = storage.getIssues();
            callback.sendMessage(msgToPost);
          }
        }
        catch (JSONException e) {
          Log.d("HelpShiftDebug", "JSON Exception!!!", e);
        }
      }
    };
    this.client.fetchMyIssues(localSuccess, failure, identity, lastTs, mc, chatLaunchSource);
  }

  private String generateMC() throws JSONException {
    JSONObject mc = new JSONObject();
    JSONArray issuesList = this.storage.getIssues();
    for (int i = 0; i < issuesList.length(); i++) {
      JSONObject issue = issuesList.getJSONObject(i);
      JSONArray messages = issue.getJSONArray("messages");
      for (int j = messages.length() - 1; j > 0; j--) {
        JSONObject message = messages.getJSONObject(j);
        if (!message.getString("id").startsWith("localRscMessage_")) {
          mc.put(issue.getString("id"), message.getString("created_at"));
          break;
        }
      }
    }
    return mc.toString();
  }

  private void sendFailMessage(Handler failure, int status) {
    Message result = failure.obtainMessage();
    HashMap messageResponse = new HashMap();
    messageResponse.put("status", Integer.valueOf(status));
    result.obj = messageResponse;
    failure.sendMessage(result);
  }

  public void getLatestIssues(Handler callback, Handler failure) throws JSONException {
    getLatestIssues(callback, failure, null);
  }

  protected void getLatestIssues(Handler callback, Handler failure, String chatLaunchSource) throws JSONException
  {
    HashMap ts = this.storage.getIssuesTs();
    String identity = this.storage.getIdentity();
    if (!TextUtils.isEmpty(identity)) {
      if (ts.containsKey("success")) {
        String dateStr = (String)ts.get("ts");
        getAndStoreIssues(callback, failure, identity, dateStr, generateMC(), Boolean.valueOf(true), chatLaunchSource);
      }
      else {
        getAndStoreIssues(callback, failure, identity, "", "", Boolean.valueOf(true), chatLaunchSource);
      }
    }
    else sendFailMessage(failure, 403);
  }

  protected void getAllIssues(Handler callback, Handler failure)
    throws JSONException
  {
    String identity = this.storage.getIdentity();
    if (!TextUtils.isEmpty(identity))
      getAndStoreIssues(callback, failure, identity, "", "", Boolean.valueOf(false));
    else
      sendFailMessage(failure, 403);
  }

  protected void getIssues(Handler callback, Handler failure)
    throws JSONException
  {
    HashMap ts = this.storage.getIssuesTs();
    String identity = this.storage.getIdentity();
    if (!TextUtils.isEmpty(identity)) {
      if (ts.containsKey("success")) {
        JSONArray issuesList = this.storage.getIssues();
        Message issuesMess = callback.obtainMessage();
        issuesMess.obj = issuesList;
        callback.sendMessage(issuesMess);
      } else {
        getAndStoreIssues(callback, failure, identity, "", "", Boolean.valueOf(false));
      }
    }
    else sendFailMessage(failure, 403); 
  }

  protected JSONObject getIssue(int position)
    throws JSONException
  {
    String issueId = this.storage.getIssuesIdList().getString(position);
    return this.storage.getIssue(issueId);
  }

  protected JSONObject getMetaInfo(Boolean isAddInfo) {
    if (isCustomIdentifier().booleanValue()) {
      return Meta.getMetaInfo(this.c, isAddInfo, getDeviceIdentifier());
    }

    return Meta.getMetaInfo(this.c, isAddInfo, null);
  }

  protected void createIssue(final Handler success, final Handler failure, final String messageText, HashMap userInfo)
    throws IdentityException
  {
    final String identity = this.storage.getIdentity();

    if (!identity.equals("")) {
      final JSONObject metaInfo = getMetaInfo(Boolean.valueOf(true));
      if (userInfo != null) {
        try {
          metaInfo.put("user_info", new JSONObject(userInfo));
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "userInfo JSONException", e);
        }
      }

      Handler localFailure = new Handler()
      {
        public void handleMessage(Message msg) {
          HashMap result = (HashMap)msg.obj;
          Integer status = (Integer)result.get("status");
          if (status.intValue() == 413) {
            metaInfo.remove("custom_meta");
            client.createIssue(success, failure, identity, messageText, metaInfo.toString());
          }
          else {
            Message fmsg = failure.obtainMessage();
            fmsg.obj = result;
            failure.sendMessage(fmsg);
          }
        }
      };
      this.client.createIssue(success, localFailure, identity, messageText, metaInfo.toString());
    }
    else {
      throw new IdentityException("Identity not found");
    }
  }

  protected void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers)
  {
    addMessage(success, failure, issueId, messageText, type, refers, -1);
  }

  protected void addMessage(Handler success, final Handler failure, final String issueId, final String messageText, final String type, final String refers, final int failedState)
  {
    String identity = this.storage.getIdentity();
    Handler localFailure = new Handler()
    {
      public void handleMessage(Message msg) {
        try {
          storage.storeFailedMessage(issueId, messageText, type, refers, failedState);
        }
        catch (JSONException e) {
          Log.d("HelpShiftDebug", "JSON Exception", e);
        }

        Message fmsg = failure.obtainMessage();
        fmsg.obj = ((HashMap)msg.obj);
        failure.sendMessage(fmsg);
      }
    };
    this.client.addMessage(success, localFailure, identity, issueId, messageText, type, refers);
  }

  protected void setInvisible(String messageId, String issueId) throws JSONException
  {
    JSONObject message = this.storage.getMessage(messageId, issueId);
    message.put("invisible", true);
    this.storage.updateMessage(message);
  }

  protected void setScreenshot(String messageId, String issueId, String screenshot) throws JSONException
  {
    JSONObject message = this.storage.getMessage(messageId, issueId);
    message.put("screenshot", screenshot);
    this.storage.updateMessage(message);
  }

  protected void setInProgress(String messageId, String issueId, Boolean inProgress) throws JSONException
  {
    JSONObject message = this.storage.getMessage(messageId, issueId);
    message.put("inProgress", inProgress);
    this.storage.updateMessage(message);
  }

  protected void markQuestion(final Handler success, final Handler failure, final String faqId, final Boolean helpful)
  {
    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        try {
          HashMap result = (HashMap)msg.obj;
          JSONObject markResult = (JSONObject)result.get("response");
          if (markResult.getString("status").equals("marked")) {
            faqDataSource.setIsHelpful(faqId, helpful);

            Message msgToPost = success.obtainMessage();
            msgToPost.obj = helpful;
            success.sendMessage(msgToPost);
          }
        } catch (JSONException e) {
          Message msgToPost = failure.obtainMessage();
          failure.sendMessage(msgToPost);
          Log.d("HelpShiftDebug", "JSON Exception", e);
        }
      }
    };
    Handler localFailure = new Handler()
    {
      public void handleMessage(Message msg) {
        faqDataSource.setIsHelpful(faqId, helpful);
        Message msgToPost = failure.obtainMessage();
        msgToPost.obj = msg.obj;
        failure.sendMessage(msgToPost);
      }
    };
    if (helpful.booleanValue())
      this.client.markHelpful(localSuccess, localFailure, faqId);
    else
      this.client.markUnhelpful(localSuccess, localFailure, faqId);
  }

  protected ArrayList localFaqSearch(String query)
  {
    if (this.flatFaqList == null) {
      updateFlatList();
    }

    ArrayList result = new ArrayList();

    String lcQuery = query.toLowerCase();

    if (this.storage.getDBFlag().booleanValue()) {
      ArrayList resultsAll = HSSearch.queryDocs(query, this.storage.readIndex());
      List results;
      if (resultsAll.size() >= 5)
        results = resultsAll.subList(0, 5);
      else {
        results = resultsAll;
      }

      for (Object r : results) {
        Faq faq = (Faq)this.flatFaqList.get(Integer.decode((String) r).intValue());
        result.add(faq);
      }
//      Faq faq;
//      for(Iterator i$ = results.iterator(); i$.hasNext(); result.add(faq))
//      {
//          String r = (String)i$.next();
//          faq = (Faq)flatFaqList.get(Integer.decode(r).intValue());
//      }
    } else {
      for (int i = 0; i < this.flatFaqList.size(); i++) {
        Faq faq = (Faq)this.flatFaqList.get(i);
        String title = faq.getTitle().toLowerCase();
        if (title.indexOf(lcQuery) != -1) {
          result.add(faq);
        }
      }
    }

    return result;
  }

  protected ArrayList getAllFaqs() {
    if (this.flatFaqList == null) {
      updateFlatList();
    }
    return this.flatFaqList;
  }

  protected void getNotificationCount(final Handler success, Handler failure) {
    try {
      Handler localSuccess = new Handler()
      {
        public void handleMessage(Message msg) {
          Integer activeCnt = storage.getActiveNotifCnt();
          Message msgToPost = success.obtainMessage();
          Bundle countData = new Bundle();
          countData.putInt("value", activeCnt.intValue());
          countData.putBoolean("cache", false);
          msgToPost.obj = countData;
          success.sendMessage(msgToPost);
        }
      };
      getLatestIssues(localSuccess, failure);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  protected void getNotificationData(Handler success, Handler failure)
  {
    try {
      getLatestIssues(success, failure);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  protected void reportAppStartEvent()
  {
    JSONArray actions = new JSONArray();
    JSONObject eventObj = new JSONObject();
    try {
      eventObj.put("ts", HSFormat.tsSecFormatter.format(System.currentTimeMillis() / 1000.0D));
      eventObj.put("t", "a");
      actions.put(eventObj);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
    reportActionEvents(actions);
  }

  protected void reportActionEvents()
  {
    JSONArray actions = HSFunnel.getActions();
    reportActionEvents(actions);
  }

  private void reportActionEvents(JSONArray actions) {
    String identifier = getHSId();
    String profileId = this.storage.getIdentity();
    String libraryVersion = "3.4.1";
    String sdkType = this.storage.getSdkType();
    String deviceId = getDeviceIdentifier();
    String uid = "";
    String deviceModel = Build.MODEL;
    String os = Build.VERSION.RELEASE;
    String appVersion = Meta.getApplicationVersion(this.storage.getContext());
    String rom = System.getProperty("os.version") + ":" + Build.FINGERPRINT;
    TelephonyManager tm = (TelephonyManager)this.c.getSystemService("phone");
    String cc = tm.getSimCountryIso();
    String ln = Locale.getDefault().getLanguage();

    if (!identifier.equals(deviceId)) {
      uid = deviceId;
    }

    JSONObject params = new JSONObject();
    try {
      params.put("id", identifier);
      params.put("uid", uid);
      params.put("pid", profileId);
      params.put("v", libraryVersion);
      params.put("e", actions);
      params.put("s", sdkType);
      params.put("dm", deviceModel);
      params.put("os", os);
      params.put("av", appVersion);
      params.put("rs", rom);
      if (!TextUtils.isEmpty(cc)) {
        params.put("cc", cc);
      }
      params.put("ln", ln);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
    String key = "action_event_" + Long.toString(System.currentTimeMillis());

    Handler apiFailHandler = getApiFailHandler(new Handler(), key, 2, params);

    reportActionEvents(new Handler(), apiFailHandler, actions, identifier, uid, profileId, libraryVersion, sdkType, deviceModel, os, appVersion, rom);
  }

  private void reportActionEvents(Handler success, Handler failure, JSONArray actions, String identifier, String uid, String profileId, String libraryVersion, String sdkType, String deviceModel, String os, String appVersion, String rom)
  {
    this.client.reportActionEvents(success, failure, actions, identifier, uid, profileId, libraryVersion, sdkType, deviceModel, os, appVersion, rom);
  }

  protected Boolean showReviewP()
  {
    if (this.storage.getReviewed() == 0) {
      JSONObject pr = (JSONObject)HSConfig.configData.get("pr");
      String rurl = (String)HSConfig.configData.get("rurl");
      if ((pr != null) && (pr.optBoolean("s", false)) && (!TextUtils.isEmpty(rurl))) {
        int reviewCount = this.storage.getReviewCounter();
        String counterType = pr.optString("t", "");
        int counterInterval = pr.optInt("i", 0);
        if (counterInterval > 0) {
          if ((counterType.equals("l")) && (reviewCount >= counterInterval))
          {
            return Boolean.valueOf(true);
          }if ((counterType.equals("s")) && (reviewCount != 0) && (new Date().getTime() / 1000L - reviewCount >= counterInterval))
          {
            return Boolean.valueOf(true);
          }
        }
      }
    }
    return Boolean.valueOf(false);
  }

  protected void loadConfig() {
    try {
      JSONObject pr = (JSONObject)HSConfig.configData.get("pr");
      JSONObject storedConfig = this.storage.getConfig();
      if ((pr == null) && (storedConfig.length() != 0))
        HSConfig.updateConfig(storedConfig);
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSON Exception:" + e.toString());
    }
  }

  protected void updateReviewCounter()
  {
    int reviewCounter = this.storage.getReviewCounter();
    int launchReviewCounter = this.storage.getLaunchReviewCounter();

    if (reviewCounter == 0) {
      launchReviewCounter = reviewCounter;
      reviewCounter = (int)(new Date().getTime() / 1000L);
    }

    launchReviewCounter++;
    this.storage.setLaunchReviewCounter(launchReviewCounter);

    loadConfig();
    JSONObject pr = (JSONObject)HSConfig.configData.get("pr");
    if (pr != null) {
      String counterType = pr.optString("t", "");
      if (counterType.equals("l")) {
        reviewCounter = this.storage.getLaunchReviewCounter();
      }
    }
    this.storage.setReviewCounter(reviewCounter);
  }

  protected void resetReviewCounter()
  {
    int reviewCounter = this.storage.getReviewCounter();
    try
    {
      JSONObject pr = this.storage.getConfig().getJSONObject("pr");
      if (pr != null) {
        String counterType = pr.optString("t", "");
        if (counterType.equals("s"))
          reviewCounter = (int)(new Date().getTime() / 1000L);
        else if (counterType.equals("l")) {
          reviewCounter = 0;
        }
        this.storage.setReviewCounter(reviewCounter);
        this.storage.setLaunchReviewCounter(0);
      }
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Reseting review counter", e);
    }
  }

  protected void enableReview() {
    this.storage.enableReview();
  }

  protected void disableReview() {
    this.storage.setReviewed();
  }

  protected void getAndStoreMessages(final String issueId, final Handler success, Handler failure, String chatLaunchSource) throws JSONException
  {
    try {
      String identity = this.storage.getIdentity();
      JSONObject issue = this.storage.getIssue(issueId);
      JSONArray messages = issue.getJSONArray("messages");
      JSONObject lastMessage;
      if (messages.length() == 0)
        lastMessage = messages.getJSONObject(0);
      else {
        lastMessage = messages.getJSONObject(messages.length() - 1);
      }

      Handler localSuccess = new Handler()
      {
        public void handleMessage(Message msg) {
          HashMap result = (HashMap)msg.obj;
          JSONArray messages = (JSONArray)result.get("response");
          try
          {
            storage.storeMessages(issueId, messages);
          } catch (JSONException e) {
            Log.d("HelpShiftDebug", e.toString(), e);
          }

          Message msgToPost = success.obtainMessage();
          msgToPost.obj = messages;
          success.sendMessage(msgToPost);
        }
      };
      this.client.fetchMessages(localSuccess, failure, identity, issueId, lastMessage.getString("created_at"), chatLaunchSource);
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  protected String getDeviceIdentifier() {
    String deviceId = this.storage.getDeviceIdentifier();
    if (!TextUtils.isEmpty(deviceId)) {
      return deviceId;
    }
    return getHSId();
  }

  protected String getHSId()
  {
    return Settings.Secure.getString(this.c.getContentResolver(), "android_id");
  }

  protected void deleteFiles(List<String> filenames)
  {
    for (int i = 0; i < filenames.size(); i++) {
      File file = new File(this.c.getFilesDir(), (String)filenames.get(i));
      file.delete();
    }
  }

  public void storeFile(String fileName) {
    try {
      JSONArray fileJsonList = this.storage.getStoredFiles();
      List fileArrayList = new ArrayList();

      for (int i = 0; i < fileJsonList.length(); i++) {
        fileArrayList.add(fileJsonList.getString(i));
      }

      fileArrayList.add(0, fileName);

      if (fileArrayList.size() > 10) {
        List finalFileArrayList = fileArrayList.subList(0, 10);

        JSONArray finalFileJsonList = new JSONArray();
        for (int i = 0; i < finalFileArrayList.size(); i++) {
          finalFileJsonList.put(finalFileArrayList.get(i));
        }
        deleteFiles(fileArrayList.subList(10, fileArrayList.size()));
        this.storage.setStoredFiles(finalFileJsonList);
      } else {
        JSONArray finalFileJsonList = new JSONArray();
        for (int i = 0; i < fileArrayList.size(); i++) {
          finalFileJsonList.put(fileArrayList.get(i));
        }
        this.storage.setStoredFiles(finalFileJsonList);
      }
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "storeFile", e);
    }
  }

  protected void clearUserData() {
    try {
      JSONArray filenames = this.storage.getStoredFiles();
      this.storage.clearUserData();

      List fileArrayList = new ArrayList();
      for (int i = 0; i < filenames.length(); i++) {
        fileArrayList.add(filenames.getString(i));
      }
      deleteFiles(fileArrayList);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Clearing user data", e);
    }
  }

  protected void registerProfile(final Handler success, Handler failure, String username, String email, String identifier)
  {
    String crittercismId = null;
    try
    {
      Class Crittercism = Class.forName("com.crittercism.app.Crittercism");
      Method getUserUUID = Crittercism.getMethod("getUserUUID", (Class[])null);
      crittercismId = (String)getUserUUID.invoke(null, (Object[])null);
    } catch (ClassNotFoundException cnfe) {
      Log.d("HelpShiftDebug", "If you are not using Crittercism. Please ignore this", cnfe);
    } catch (Exception e) {
      Log.d("HelpShiftDebug", "If you are not using Crittercism. Please ignore this", e);
    }

    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        startInAppService();
        Message msgToPost = success.obtainMessage();
        msgToPost.obj = msg.obj;
        success.sendMessage(msgToPost);
      }
    };
    this.client.registerProfile(localSuccess, failure, username, email, identifier, crittercismId);
  }

  protected void updateUAToken()
  {
    String profileId = this.storage.getIdentity();
    String deviceToken = this.storage.getDeviceToken();

    JSONObject params = new JSONObject();
    try {
      params.put("profile-id", profileId);
      params.put("device-token", deviceToken);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }

    Handler apiFailHandler = getApiFailHandler(new Handler(), "push_token_" + profileId, 1, params);

    updateUAToken(new Handler(), apiFailHandler, profileId, deviceToken);
  }

  private void updateUAToken(final Handler success, final Handler failure, String profileId, String deviceToken)
  {
    if (!TextUtils.isEmpty(deviceToken)) {
      Handler localSuccess = new Handler()
      {
        public void handleMessage(Message msg) {
          Message msgToPost = success.obtainMessage();
          msgToPost.obj = msg.obj;
          success.sendMessage(msgToPost);
          stopInAppService();
          Log.d("HelpShiftDebug", "Update UA Token Success Message" + msg.toString());
        }
      };
      Handler localFailure = new Handler()
      {
        public void handleMessage(Message msg) {
          Message msgToPost = failure.obtainMessage();
          msgToPost.obj = msg.obj;
          failure.sendMessage(msgToPost);
          Log.d("HelpShiftDebug", "Update UA Token Failure Message" + msg.toString());
        }
      };
      this.client.updateUAToken(localSuccess, localFailure, deviceToken, profileId);
    }
    else {
      Log.d("HelpShiftDebug", "deviceToken is not set");
    }
  }

  protected Boolean isCustomIdentifier() {
    return Boolean.valueOf(!TextUtils.isEmpty(this.storage.getDeviceIdentifier()));
  }

  private void updateIndex() {
    this.storage.deleteIndex();
    updateFlatList();

    HashMap index = HSSearch.indexDocuments(new ArrayList(this.flatFaqList));
    if (index != null)
      this.storage.storeIndex(index);
  }

  protected void loadIndex()
  {
    Thread loadIndexThread = new Thread(new Runnable() {
      public void run() {
        storage.loadIndex();
      }
    });
    loadIndexThread.setDaemon(true);
    loadIndexThread.start();
  }

  protected void getSection(final String publishId, final Handler success, Handler failure) {
    try {
      Section section = this.sectionDataSource.getSection(publishId);
      if (section != null) {
        Message msgToPost = success.obtainMessage();
        msgToPost.obj = section;
        success.sendMessage(msgToPost);
      }
      Handler localSuccess = new Handler()
      {
        public void handleMessage(Message msg) {
          super.handleMessage(msg);
          ArrayList sections = (ArrayList)msg.obj;
          Section toReturn = sectionDataSource.getSection(publishId);

          Message msgToPost = success.obtainMessage();
          msgToPost.obj = toReturn;
          success.sendMessage(msgToPost);
        }
      };
      getAndStoreSections(localSuccess, failure);
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting section data ", s);
    }
  }

  protected Section getSection(String publishId)
  {
    return this.sectionDataSource.getSection(publishId);
  }

  protected void getSectionSync(String publishId, Handler success, Handler failure)
  {
    try {
      Section section = this.sectionDataSource.getSection(publishId);
      if (section != null) {
        Message msgToPost = success.obtainMessage();
        msgToPost.obj = section;
        success.sendMessage(msgToPost);
      } else {
        Message msgToPost = failure.obtainMessage();
        failure.sendMessage(msgToPost);
      }
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting section data ", s);
    }
  }

  private String getPublishIdFromSectionId(String sectionId) {
    ArrayList sections = getSections();
    String sectionPublishId = "";
    for (int i = 0; i < sections.size(); i++) {
      Section sectionItem = (Section)sections.get(i);
      if (sectionItem.getSectionId().equals(sectionId)) {
        sectionPublishId = sectionItem.getPublishId();
      }
    }
    return sectionPublishId;
  }

  private void getQuestionAsync(String publishId, final Handler success, Handler failure) {
    Handler localSuccess = new Handler()
    {
      public void handleMessage(Message msg) {
        Message msgToPost = success.obtainMessage();
        HashMap result = (HashMap)msg.obj;
        try {
          if (result != null) {
            JSONObject question = (JSONObject)result.get("response");
            Faq newFaq = new Faq(0L, question.getString("id"), question.getString("publish_id"), getPublishIdFromSectionId(question.getString("section_id")), question.getString("title"), question.getString("body"), 0, Boolean.valueOf(question.getString("is_rtl") == "true"));

            msgToPost.obj = newFaq;
            success.sendMessage(msgToPost);
            faqDataSource.createFaq(newFaq);
          }
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "Exception in getting question " + e);
        }
      }
    };
    this.client.getQuestion(publishId, localSuccess, failure);
  }

  protected void getQuestion(String publishId, Handler success, Handler failure) {
    Faq question = null;
    try {
      question = this.faqDataSource.getFaq(publishId);
    } catch (SQLException s) {
      Log.d("HelpShiftDebug", "Database exception in getting faq ", s);
    }
    if (question == null) {
      getQuestionAsync(publishId, success, failure);
    } else {
      Message msgToPost = success.obtainMessage();
      msgToPost.obj = question;
      success.sendMessage(msgToPost);
      getQuestionAsync(publishId, success, failure);
    }
  }

  protected void resetServiceInterval() {
    HSService.resetInterval();
  }

  protected void stopInAppService() {
    Intent service = new Intent(this.c, HSService.class);
    this.c.stopService(service);
  }

  protected void startInAppService() {
    Boolean enableInAppNotification = Boolean.valueOf(true);
    JSONObject config = new JSONObject();
    try
    {
      config = this.storage.getAppConfig();
      if (config.has("enableInAppNotification"))
        enableInAppNotification = (Boolean)config.get("enableInAppNotification");
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "startInAppService JSONException", e);
    }

    if (enableInAppNotification.booleanValue()) {
      String identity = this.storage.getIdentity();
      String conversation = this.storage.getActiveConversation();
      if ((!TextUtils.isEmpty(identity)) && (!TextUtils.isEmpty(conversation))) {
        Intent service = new Intent(this.c, HSService.class);
        if (!this.storage.getLibraryVersion().equals("3.4.1"))
        {
          stopInAppService();
        }
        this.c.startService(service);
      }
    } else {
      stopInAppService();
    }
  }

  protected JSONArray getMessagesWithFails(String issueId) {
    try {
      return this.storage.mergeMessages(this.storage.getFailedMessages(issueId), this.storage.getIssue(issueId).getJSONArray("messages"));
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }return new JSONArray();
  }

  protected void sendFailedMessages()
  {
    try
    {
      JSONObject failedMessage = this.storage.popFailedMessage();

      if (failedMessage == null) {
        return;
      }

      Handler localSuccess = new Handler()
      {
        public void handleMessage(Message msg) {
          sendFailedMessages();
          Intent i = new Intent("com.elex.chatservice.failedMessageRequest");
          c.sendBroadcast(i);
        }
      };
      addMessage(localSuccess, localSuccess, failedMessage.getString("issue_id"), failedMessage.getString("body"), failedMessage.getString("type"), failedMessage.getString("refers"), failedMessage.optInt("state", 0) - 1);
    }
    catch (JSONException e)
    {
      Log.d("HelpShiftDebug", "SendfailedMessages failed", e);
    }
  }

  protected Handler getApiFailHandler(final Handler failure, final String key, final int type, final JSONObject params)
  {
    Handler apiFailHandler = new Handler()
    {
      public void handleMessage(Message msg) {
        Message msgToPost = failure.obtainMessage();
        msgToPost.obj = msg.obj;
        failure.sendMessage(msgToPost);
        try
        {
          HashMap result = (HashMap)msg.obj;
          Integer status = (Integer)result.get("status");
          if ((status.intValue() == 403) || (status.intValue() == 422)) {
            storage.storeFailedApiCall(key, null);
          } else {
            JSONObject failedApiCall = new JSONObject();
            failedApiCall.put("t", type);
            failedApiCall.put("p", params);
            storage.storeFailedApiCall(key, failedApiCall);
          }
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "JSONException", e);
        }
      }
    };
    return apiFailHandler;
  }

  protected Handler getApiSuccessHandler(final Handler success, final String key, int type, JSONObject params)
  {
    Handler apiSuccessHandler = new Handler()
    {
      public void handleMessage(Message msg) {
        Message msgToPost = success.obtainMessage();
        msgToPost.obj = msg.obj;
        success.sendMessage(msgToPost);
        try
        {
          storage.storeFailedApiCall(key, null);
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", "JSONException", e);
        }
      }
    };
    return apiSuccessHandler;
  }

  protected void sendFailedApiCalls() {
    Handler localHandler = new Handler()
    {
      public void handleMessage(Message msg) {
        sendFailedApiCalls();
      }
    };
    try
    {
      JSONObject failedApiCalls = this.storage.getFailedApiCalls();

      if (this.failedApiKeys == null) {
        this.failedApiKeys = failedApiCalls.keys();
      }

      if (this.failedApiKeys.hasNext()) {
        String key = (String)this.failedApiKeys.next();
        JSONObject failedApiCall = (JSONObject)failedApiCalls.get(key);
        JSONObject params = failedApiCall.getJSONObject("p");
        int type = failedApiCall.getInt("t");

        Handler apiSuccessHandler = getApiSuccessHandler(localHandler, key, type, params);
        Handler apiFailHandler = getApiFailHandler(localHandler, key, type, params);

        switch (type) {
        case 0:
          markQuestion(apiSuccessHandler, apiFailHandler, params.getString("f"), Boolean.valueOf(params.getBoolean("h")));

          break;
        case 1:
          updateUAToken(apiSuccessHandler, apiFailHandler, params.getString("profile-id"), params.getString("device-token"));
        case 2:
          reportActionEvents(apiSuccessHandler, apiFailHandler, params.getJSONArray("e"), params.getString("id"), params.getString("uid"), params.getString("pid"), params.getString("v"), params.getString("s"), params.getString("dm"), params.getString("os"), params.getString("av"), params.getString("rs"));

          break;
        case 3:
          updateMessageSeenState(apiSuccessHandler, apiFailHandler, params.getJSONArray("mids"), params.getString("src"), params.getString("at"));
        }

      }
      else
      {
        this.failedApiKeys = null;
      }
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
  }

  protected void updateMessageSeenState(String issueId, String chatLaunchSource) {
    JSONArray messageIds = new JSONArray();
    String readAt = HSFormat.tsSecFormatter.format(System.currentTimeMillis() / 1000.0D);
    try
    {
      JSONObject issue = this.storage.getIssue(issueId);
      JSONArray messages = issue.getJSONArray("messages");
      JSONArray newMessages = new JSONArray();

      for (int i = 0; i < messages.length(); i++) {
        JSONObject message = messages.getJSONObject(i);
        if ((!message.getString("origin").equals("mobile")) && (!message.optBoolean("seen", false)))
        {
          messageIds.put(message.getString("id"));
          message.put("seen", true);
        }
        newMessages.put(message);
      }

      issue.put("messages", newMessages);
      this.storage.storeIssue(issue);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Update messages seen state callback", e);
    }

    JSONObject params = new JSONObject();
    try
    {
      params.put("mids", messageIds);
      params.put("src", chatLaunchSource);
      params.put("at", readAt);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSONException", e);
    }
    String key = "msg_seen_" + Long.toString(System.currentTimeMillis());

    Handler apiFailHandler = getApiFailHandler(new Handler(), key, 3, params);

    updateMessageSeenState(new Handler(), apiFailHandler, messageIds, chatLaunchSource, readAt);
  }

  private void updateMessageSeenState(Handler success, Handler failure, JSONArray messageIds, String source, String readAt)
  {
    this.client.updateMessageSeenState(messageIds, source, readAt, success, failure);
  }

  protected static void addFaqSyncStatusObserver(HSFaqSyncStatusEvents observer)
  {
    if (observers == null) {
      observers = new ArrayList();
    }

    observers.add(observer);
  }

  protected static void removeFaqSyncStatusObserver(HSFaqSyncStatusEvents observer)
  {
    if (observers != null)
      observers.remove(observer);
  }

  protected static void signalFaqsUpdated()
  {
    faqsSyncing = false;
    for (int i = 0; i < observers.size(); i++) {
      HSFaqSyncStatusEvents observer = (HSFaqSyncStatusEvents)observers.get(i);
      if (observer != null)
        observer.faqsUpdated();
    }
  }

  protected static void signalSearchIndexesUpdated()
  {
    for (int i = 0; i < observers.size(); i++) {
      HSFaqSyncStatusEvents observer = (HSFaqSyncStatusEvents)observers.get(i);
      if (observer != null)
        observer.searchIndexesUpdated();
    }
  }
}