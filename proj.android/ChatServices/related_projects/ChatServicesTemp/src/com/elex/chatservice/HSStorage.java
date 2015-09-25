package com.elex.chatservice;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.text.TextUtils;
import android.util.Log;

import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSFormat;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSStorage
{
  private Context context;
  private SharedPreferences storage;
  private final String dbFile = "tfidf.db";
  private static HashMap cacheTFIDF;

  public HSStorage(Context c)
  {
    this.context = c;
    this.storage = c.getSharedPreferences("HSJsonData", 0);
  }

  public Context getContext() {
    return this.context;
  }

  private JSONArray concatJSONArray(JSONArray arr1, JSONArray arr2) throws JSONException {
    JSONArray result = new JSONArray();
    for (int i = 0; i < arr1.length(); i++) {
      result.put(arr1.get(i));
    }
    for (int i = 0; i < arr2.length(); i++) {
      result.put(arr2.get(i));
    }
    return result;
  }

  private JSONObject storageGetObj(String key) throws JSONException {
    return new JSONObject(this.storage.getString(key, "{}"));
  }

  private JSONArray storageGetArr(String key) throws JSONException {
    return new JSONArray(this.storage.getString(key, "[]"));
  }

  private String storageGet(String key) {
    return this.storage.getString(key, "");
  }

  private Integer storageGetInt(String key) {
    return Integer.valueOf(this.storage.getInt(key, 0));
  }

  private long storageGetLong(String key) {
    return this.storage.getLong(key, 0L);
  }

  private Float storageGetFloat(String key) {
    return Float.valueOf(this.storage.getFloat(key, 0.0F));
  }

  private Boolean storageGetBoolean(String key) {
    return Boolean.valueOf(this.storage.getBoolean(key, false));
  }

  private void massStorageSet(String key, JSONArray data) {
    SharedPreferences.Editor editor = this.storage.edit();
    for (int i = 0; i < data.length(); i++) {
      try {
        JSONObject item = data.getJSONObject(i);
        editor.putString(item.getString(key), item.toString());
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", e.toString());
      }
    }
    editor.commit();
  }

  private void storageSet(String key, JSONArray data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putString(key, data.toString());
    editor.commit();
  }

  private void storageSet(String key, JSONObject data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putString(key, data.toString());
    editor.commit();
  }

  private void storageSet(String key, String data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putString(key, data);
    editor.commit();
  }

  private void storageSet(String key, Integer data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putInt(key, data.intValue());
    editor.commit();
  }

  private void storageSet(String key, Long data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putLong(key, data.longValue());
    editor.commit();
  }

  private void storageSet(String key, Float data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putFloat(key, data.floatValue());
    editor.commit();
  }

  private void storageSet(String key, Boolean data) {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.putBoolean(key, data.booleanValue());
    editor.commit();
  }

  protected JSONArray mergeMessages(JSONArray messages1, JSONArray messages2) throws JSONException
  {
    HashMap messagesMap = new HashMap();
    JSONArray messagesList = new JSONArray();
    ArrayList messagesMapList = new ArrayList();
    JSONArray messages = concatJSONArray(messages1, messages2);

    for (int i = 0; i < messages.length(); i++) {
      JSONObject message = messages.getJSONObject(i);
      if (!messagesMap.containsKey(message.get("id"))) {
        messagesMap.put(message.get("id").toString(), message);
        HashMap messageMap = new HashMap();
        messageMap.put("id", message.get("id"));
        messageMap.put("created_at", message.get("created_at"));
        messagesMapList.add(messageMap);
      }

    }

    Collections.sort(messagesMapList, new Comparator() {
      public int compare(HashMap a, HashMap b) {
        try {
          Date dateA = HSFormat.issueTsFormat.parse(a.get("created_at").toString());
          Date dateB = HSFormat.issueTsFormat.parse(b.get("created_at").toString());
          return dateA.compareTo(dateB);
        } catch (Exception e) {
        }
        return 0;
      }

		public int compare(Object lhs, Object rhs) {
			return compare((HashMap)lhs, (HashMap)rhs);
		}
    });
    for (int i = 0; i < messagesMapList.size(); i++) {
      messagesList.put(messagesMap.get(((HashMap)messagesMapList.get(i)).get("id")));
    }

    return messagesList;
  }

  protected JSONObject addMessages(JSONObject issue, JSONArray messages) throws JSONException
  {
    issue.put("messages", mergeMessages(messages, issue.getJSONArray("messages")));
    return issue;
  }

  public void storeMessages(String issueId, JSONArray messages) throws JSONException {
    if (messages.length() == 0) {
      return;
    }

    JSONObject issue = storageGetObj(issueId);
    storageSet(issueId, addMessages(issue, messages));
  }

  protected void updateMessage(JSONObject message) throws JSONException {
    String issueId = message.getString("issue_id");
    JSONObject issue = storageGetObj(issueId);
    JSONArray issueMessages = issue.getJSONArray("messages");
    JSONArray messages = new JSONArray();
    for (int i = 0; i < issueMessages.length(); i++) {
      JSONObject oldMessage = issueMessages.getJSONObject(i);
      if (oldMessage.getString("id").equals(message.getString("id")))
        messages.put(message);
      else {
        messages.put(oldMessage);
      }
    }
    issue.put("messages", messages);
    storageSet(issueId, issue);
  }

  protected void deleteMessage(String issueId, String messageId) throws JSONException {
    JSONObject issue = storageGetObj(issueId);
    JSONArray issueMessages = issue.getJSONArray("messages");
    JSONArray messages = new JSONArray();
    for (int i = 0; i < issueMessages.length(); i++) {
      JSONObject oldMessage = issueMessages.getJSONObject(i);
      if (!oldMessage.getString("id").equals(messageId)) {
        messages.put(oldMessage);
      }
    }
    issue.put("messages", messages);
    storageSet(issueId, issue);
  }

  protected JSONObject getMessage(String messageId, String issueId) throws JSONException {
    JSONObject issue = storageGetObj(issueId);
    JSONArray issueMessages = issue.getJSONArray("messages");
    JSONObject message = new JSONObject();
    for (int i = 0; i < issueMessages.length(); i++) {
      JSONObject oldMessage = issueMessages.getJSONObject(i);
      if (oldMessage.getString("id").equals(messageId)) {
        return oldMessage;
      }
    }

    return message;
  }

  protected void storeIssues(JSONArray issues) {
    try {
      JSONObject issuesMap = new JSONObject();
      Integer totalNotifCnt = Integer.valueOf(0);
      JSONArray issuesIdList = storageGetArr("issuesIdList");
      for (int i = 0; i < issuesIdList.length(); i++) {
        String issueId = issuesIdList.getString(i);
        JSONObject issue = storageGetObj(issueId);
        issuesMap.put(issueId, issue);
      }

      ArrayList issuesList = new ArrayList();
      JSONArray issuesToStore = new JSONArray();

      for (int i = 0; i < issues.length(); i++) {
        JSONObject issue = issues.getJSONObject(i);
        String issueId = issue.getString("id");
        if (!issuesMap.has(issueId)) {
          if (!issue.getJSONArray("messages").isNull(0)) {
            issuesToStore.put(issue);
            issuesMap.put(issueId, issue);
          }
        }
        else {
          JSONObject issueFromMap = issuesMap.getJSONObject(issueId);
          JSONArray messages = issueFromMap.getJSONArray("messages");
          JSONObject newIssue = issueFromMap;
          JSONArray newMessages = issue.getJSONArray("messages");
          if ((!messages.isNull(0)) && (!newMessages.isNull(0)) && (messages.getJSONObject(0).getString("id") != newMessages.getJSONObject(0).getString("id")))
          {
            int newMessagesCnt = issueFromMap.optInt("newMessagesCnt");
            newIssue = addMessages(issueFromMap, newMessages);

            JSONArray newIssueMessages = newIssue.getJSONArray("messages");
            int newIssueMessagesLen = 0;
            int messagesCnt = 0;
            for (int j = 0; j < newIssueMessages.length(); j++) {
              if (newIssueMessages.getJSONObject(j).getString("origin").equals("admin")) {
                newIssueMessagesLen++;
              }
            }

            for (int j = 0; j < messages.length(); j++) {
              if (messages.getJSONObject(j).getString("origin").equals("admin")) {
                messagesCnt++;
              }
            }

            newMessagesCnt += newIssueMessagesLen - messagesCnt;

            newIssue.put("newMessagesCnt", newMessagesCnt);
          }
          newIssue.put("updated_at", issue.getString("updated_at"));
          newIssue.put("status", issue.getString("status"));
          issuesToStore.put(newIssue);
          issuesMap.put(issueId, newIssue);
        }
      }

      JSONArray issueIds = issuesMap.names();
      for (int i = 0; i < issueIds.length(); i++) {
        JSONObject issue = issuesMap.getJSONObject(issueIds.getString(i));
        totalNotifCnt = Integer.valueOf(totalNotifCnt.intValue() + issue.optInt("newMessagesCnt", 0));
        HashMap issueIdMap = new HashMap();
        issueIdMap.put("id", issue.getString("id"));
        issueIdMap.put("updated_at", issue.getString("updated_at"));
        issueIdMap.put("status", issue.getString("status"));
        issuesList.add(issueIdMap);
      }

      Collections.sort(issuesList, new Comparator()
      {
        public int compare(HashMap a, HashMap b) {
          try {
            Date dateA = HSFormat.issueTsFormat.parse(a.get("updated_at").toString());
            Date dateB = HSFormat.issueTsFormat.parse(b.get("updated_at").toString());
            return dateB.compareTo(dateA);
          } catch (Exception e) {
            Log.d("HelpShiftDebug", e.toString());
          }return 0;
        }
        
		public int compare(Object lhs, Object rhs) {
			return compare((HashMap)lhs, (HashMap)rhs);
		}
      });
      issuesIdList = new JSONArray();

      for (int i = 0; i < issuesList.size(); i++) {
        issuesIdList.put(((HashMap)issuesList.get(i)).get("id").toString());
      }

      if (totalNotifCnt.intValue() > 0) {
        storageSet("totalNotifCnt", totalNotifCnt);
      }
      storageSet("issuesIdList", issuesIdList);
      massStorageSet("id", issuesToStore);
      updateOpenConversations();
      updateActiveConversation();
    } catch (Exception e) {
      Log.d("HelpShiftDebug", "Store issues", e);
    }
  }

  protected void updateOpenConversations() {
    try {
      JSONArray issuesList = getIssues();
      List openConversations = getOpenConversations();
      for (int i = 0; i < issuesList.length(); i++) {
        JSONObject issue = issuesList.getJSONObject(i);
        String issueId = issue.getString("id");
        String status = issue.getString("status");

        if ((!openConversations.contains(issueId)) && ((status.equals("0")) || (status.equals("1"))))
        {
          openConversations.add(issueId);
        }
      }
      setOpenConversations(openConversations);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "updateOpenConversations", e);
    }
  }

  protected void updateActiveConversation() {
    Boolean dia = (Boolean)HSConfig.configData.get("dia");
    setActiveConversation("");
    if (!dia.booleanValue())
      try {
        JSONArray issuesList = getIssues();
        for (int i = 0; i < issuesList.length(); i++) {
          JSONObject issue = issuesList.getJSONObject(i);
          String issueId = issue.getString("id");
          String status = issue.getString("status");
          List openConversations = getOpenConversations();

          if ((status.equals("0")) || (status.equals("1")) || ((status.equals("2")) && (openConversations.contains(issueId))))
          {
            setActiveConversation(issueId);
            break;
          }
        }
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "updateActiveConversation", e);
      }
  }

  protected void clearAndUpdateActiveConversation(String issueId)
  {
    List openConvs = getOpenConversations();

    if (openConvs.contains(issueId)) {
      openConvs.remove(openConvs.indexOf(issueId));
      setOpenConversations(openConvs);
    }
    setArchivedConversation(issueId);
    updateActiveConversation();
  }

  protected JSONArray getIssues() throws JSONException {
    JSONArray issuesIdList = storageGetArr("issuesIdList");
    JSONArray issuesList = new JSONArray();
    for (int i = 0; i < issuesIdList.length(); i++) {
      issuesList.put(storageGetObj(issuesIdList.getString(i)));
    }
    return issuesList;
  }

  public JSONObject getIssue(String issueId) throws JSONException {
    return storageGetObj(issueId);
  }

  protected void storeFaqs(JSONArray faqs) {
    storageSet("faqs", faqs);
  }

  protected JSONArray getFaqs() throws JSONException {
    return storageGetArr("faqs");
  }

  protected HashMap getIssuesTs() {
    HashMap result = new HashMap();
    String issuesTs = storageGet("issuesTs");
    if (!TextUtils.isEmpty(issuesTs)) {
      result.put("ts", storageGet("issuesTs"));
      result.put("success", Boolean.valueOf(true));
    }
    return result;
  }

  protected void setIssuesTs(String dateStr) {
    storageSet("issuesTs", dateStr);
  }

  protected void setIdentity(String identity) {
    storageSet("identity", identity);
  }

  protected String getIdentity() {
    return storageGet("identity");
  }

  protected JSONArray getIssuesIdList() throws JSONException {
    return storageGetArr("issuesIdList");
  }

  protected Integer getTotalNotifCnt() {
    return storageGetInt("totalNotifCnt");
  }

  protected Integer getActiveNotifCnt() {
    Integer activeNotifCnt = Integer.valueOf(0);
    String issueId = getActiveConversation();

    if (!issueId.equals("")) {
      try {
        JSONObject issue = getIssue(issueId);
        activeNotifCnt = Integer.valueOf(issue.optInt("newMessagesCnt", 0));
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "get active notif count", e);
      }
    }

    return activeNotifCnt;
  }

  protected void resetIssueCount(String issueId) throws JSONException {
    JSONObject issue = storageGetObj(issueId);
    int issueNotifCnt = issue.optInt("newMessagesCnt");
    issue.put("newMessagesCnt", 0);
    storageSet(issueId, issue);
    Integer totalNotifCnt = storageGetInt("totalNotifCnt");
    totalNotifCnt = Integer.valueOf(totalNotifCnt.intValue() - issueNotifCnt);
    storageSet("totalNotifCnt", totalNotifCnt);

    JSONObject pushData = storageGetObj("pushData");
    pushData.put(issueId, 0);
    storageSet("pushData", pushData);
  }

  protected void clearDatabase() {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.clear();
    editor.commit();
  }

  protected String getApiKey() {
    return storageGet("apiKey");
  }

  protected String getDomain() {
    return storageGet("domain");
  }

  protected String getAppId() {
    return storageGet("appId");
  }

  protected String getEmail() {
    return storageGet("email");
  }

  protected String getUsername() {
    return storageGet("username");
  }

  protected String getDeviceIdentifier() {
    return storageGet("deviceId");
  }

  public String getDeviceToken() {
    return storageGet("deviceToken");
  }

  protected Boolean getForeground() {
    return storageGetBoolean("foreground");
  }

  public String getForegroundIssue() {
    return storageGet("foregroundIssue");
  }

  protected String getLibraryVersion() {
    return storageGet("libraryVersion");
  }

  protected String getApplicationVersion() {
    return storageGet("applicationVersion");
  }

  protected String getSdkType() {
    return storageGet("sdkType");
  }

  public JSONObject getMetaData() {
    JSONObject meta = null;
    try {
      meta = storageGetObj("customMetaData");
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "JSON Exception : " + e);
    }
    return meta;
  }

  protected Boolean getSwapComplete() {
    return storageGetBoolean("swapComplete");
  }

  protected String getActiveConversation() {
    return storageGet("activeConversation");
  }

  protected List getOpenConversations() {
    ArrayList list = new ArrayList();
    try {
      JSONArray openConversations = storageGetArr("openConversations");
      if (openConversations != null) {
        int len = openConversations.length();
        for (int i = 0; i < len; i++)
          list.add(openConversations.getString(i));
      }
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "getOpenConversations", e);
    }
    return list;
  }

  protected String getArchivedConversation() {
    return storageGet("archivedConversationId");
  }

  protected int getIssuePushCount(String issueId) throws JSONException {
    int updatedCount = 1;
    JSONObject pushData = storageGetObj("pushData");

    if (pushData.has(issueId)) {
      updatedCount = pushData.getInt(issueId) + 1;
    }
    pushData.put(issueId, updatedCount);
    storageSet("pushData", pushData);

    return updatedCount;
  }

  protected void setApiKey(String apiKey) {
    storageSet("apiKey", apiKey);
  }

  protected void setDomain(String domain) {
    storageSet("domain", domain);
  }

  protected void setAppId(String appId) {
    storageSet("appId", appId);
  }

  protected void setEmail(String email) {
    storageSet("email", email);
  }

  protected void setUsername(String username) {
    storageSet("username", username);
  }

  protected void setDeviceIdentifier(String deviceId) {
    storageSet("deviceId", deviceId);
  }

  protected void setDeviceToken(String deviceToken) {
    storageSet("deviceToken", deviceToken);
  }

  protected void setForeground(Boolean state) {
    storageSet("foreground", state);
  }

  protected void setForegroundIssue(String issueId) {
    storageSet("foregroundIssue", issueId);
  }

  protected void setLibraryVersion(String libraryVersion) {
    storageSet("libraryVersion", libraryVersion);
  }

  protected void setApplicationVersion(String applicationVersion) {
    storageSet("applicationVersion", applicationVersion);
  }

  protected void setSdkType(String sdkType) {
    storageSet("sdkType", sdkType);
  }

  public void setMetaData(JSONObject metaData) {
    try {
      JSONArray tags = metaData.getJSONArray("hs-tags");
      metaData.remove("hs-tags");
      JSONArray newTags = new JSONArray();
      for (int i = 0; i < tags.length(); i++) {
        String tag = tags.getString(i);
        if ((tag != null) && (!tag.equals("null"))) {
          newTags.put(tag);
        }
      }
      metaData.put("hs-tags", newTags);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Exception in processing tags " + e);
    }
    storageSet("customMetaData", metaData);
  }

  protected void setSwapComplete(Boolean swapComplete) {
    storageSet("swapComplete", swapComplete);
  }

  protected void setActiveConversation(String conversationId) {
    storageSet("activeConversation", conversationId);
  }

  protected void setOpenConversations(List openConversationsList) {
    JSONArray openConversations = new JSONArray(openConversationsList);
    storageSet("openConversations", openConversations);
  }

  protected void setArchivedConversation(String archivedConversationId) {
    storageSet("archivedConversationId", archivedConversationId);
  }

  public JSONArray getBreadCrumbs()
    throws JSONException
  {
    return storageGetArr("breadCrumbs");
  }

  protected void pushBreadCrumb(String breadCrumb)
  {
    int limit = getBreadCrumbsLimit().intValue();
    try
    {
      JSONArray crumbsList = getBreadCrumbs();
      JSONObject crumbObj = new JSONObject();
      crumbObj.put("action", breadCrumb);
      crumbObj.put("datetime", HSFormat.breadCrumbTsFormat.format(new Date()));

      crumbsList.put(crumbObj);
      int length = crumbsList.length();
      if (length > limit) {
        JSONArray newArray = new JSONArray();
        int diff = length - limit;

        for (int i = 0; i < limit; i++) {
          newArray.put(i, crumbsList.get(i + diff));
        }
        crumbsList = newArray;
      }
      storageSet("breadCrumbs", crumbsList);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Error while leaving breadcrumb", e);
    }
  }

  protected void clearBreadCrumbs()
  {
    storageSet("breadCrumbs", "[]");
  }

  protected Integer getBreadCrumbsLimit()
  {
    Integer limit;
    try
    {
      limit = (Integer)getConfig().get("bcl");
    } catch (JSONException e) {
      limit = Integer.valueOf(10);
    }
    return limit;
  }

  protected void updateBreadCrumbsLimit(Integer newLimit)
  {
    Integer currentLimit = getBreadCrumbsLimit();

    if (currentLimit.intValue() > newLimit.intValue())
      try {
        JSONArray crumbsList = getBreadCrumbs();
        JSONArray newArray = new JSONArray();
        Integer diff = Integer.valueOf(currentLimit.intValue() - newLimit.intValue());

        for (int i = 0; i < newLimit.intValue(); i++) {
          newArray.put(i, crumbsList.get(i + diff.intValue()));
        }

        storageSet("breadCrumbs", newArray);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "Error while updating breadcrumbs limit", e);
      }
  }

  protected void setConfig(JSONObject config)
  {
    storageSet("config", config);
  }

  protected JSONObject getConfig() throws JSONException {
    return storageGetObj("config");
  }

  protected void setAppConfig(JSONObject appConfig) {
    storageSet("appConfig", appConfig);
  }

  public JSONObject getAppConfig() throws JSONException {
    return storageGetObj("appConfig");
  }

  protected void clearUserData() {
    SharedPreferences.Editor editor = this.storage.edit();
    editor.remove("identity");
    try {
      JSONArray issuesIdList = storageGetArr("issuesIdList");
      for (int i = 0; i < issuesIdList.length(); i++)
        editor.remove(issuesIdList.getString(i));
    }
    catch (JSONException e) {
      Log.d("HelpShiftDebug", "clearUserData", e);
    }
    editor.remove("faqMap");
    editor.remove("cachedImages");
    editor.remove("issuesTs");
    editor.remove("totalNotifCnt");
    editor.remove("issuesIdList");
    editor.remove("reviewCounter");
    editor.remove("reviewed");
    editor.remove("foreground");
    editor.commit();
    deleteIndex();
  }

  protected void setReviewCounter(int count) {
    storageSet("reviewCounter", Integer.valueOf(count));
  }

  protected int getReviewCounter() {
    return storageGetInt("reviewCounter").intValue();
  }

  protected void setLaunchReviewCounter(int count) {
    storageSet("launchReviewCounter", Integer.valueOf(count));
  }

  protected int getLaunchReviewCounter() {
    return storageGetInt("launchReviewCounter").intValue();
  }

  protected void setReviewed() {
    storageSet("reviewed", Integer.valueOf(1));
  }

  protected void enableReview() {
    storageSet("reviewed", Integer.valueOf(0));
  }

  protected int getReviewed() {
    return storageGetInt("reviewed").intValue();
  }

  protected void setStoredFiles(JSONArray filenames) {
    storageSet("cachedImages", filenames);
  }

  protected JSONArray getStoredFiles() throws JSONException {
    return storageGetArr("cachedImages");
  }

  protected JSONObject getEtags() throws JSONException {
    return storageGetObj("etags");
  }

  protected void setEtag(String url, String etag) {
    try {
      JSONObject etags = getEtags();
      etags.put(url, etag);
      storageSet("etags", etags);
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "set etag", e);
    }
  }

  protected String getEtag(String url) {
    String etag = "";
    try {
      JSONObject etags = getEtags();
      etag = etags.optString(url, "");
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "set etag", e);
    }
    return etag;
  }

  protected void storeIndex(HashMap tfidf) {
    cacheTFIDF = tfidf;
    try {
      FileOutputStream fStream = this.context.openFileOutput("tfidf.db", 0);
      ObjectOutputStream oStream = new ObjectOutputStream(fStream);

      oStream.writeObject(tfidf);
      oStream.flush();
      oStream.close();
      setDBFlag();
    } catch (Exception e) {
      Log.d("HelpShiftDebug", "store index", e);
    }
  }

  protected void loadIndex() {
    if (cacheTFIDF == null) {
      HashMap tfidf = null;
      try {
        FileInputStream fStream = this.context.openFileInput("tfidf.db");
        ObjectInputStream oStream = new ObjectInputStream(fStream);
        tfidf = (HashMap)oStream.readObject();
        cacheTFIDF = tfidf;
        oStream.close();
      } catch (Exception e) {
        Log.d("HelpShiftDebug", "read index", e);
      }
    }
  }

  protected HashMap readIndex() {
    return cacheTFIDF;
  }

  protected Boolean getDBFlag() {
    return storageGetBoolean("dbFlag");
  }

  protected void setDBFlag() {
    storageSet("dbFlag", Boolean.valueOf(true));
  }

  protected void unsetDBFlag() {
    storageSet("dbFlag", Boolean.valueOf(false));
  }

  protected void deleteIndex() {
    cacheTFIDF = null;
    this.context.deleteFile("tfidf.db");
    unsetDBFlag();
  }

  protected JSONArray getFailedMessages() throws JSONException {
    return storageGetArr("failedMessages");
  }

  protected JSONObject popFailedMessage() throws JSONException {
    JSONArray failedMessages = storageGetArr("failedMessages");
    JSONArray failedMessagesStore = new JSONArray();
    JSONObject failedMessageRes = null;
    if (failedMessages.length() > 0) {
      for (int i = 0; i < failedMessages.length(); i++) {
        JSONObject failedMessage = failedMessages.getJSONObject(i);
        if ((failedMessageRes == null) && (failedMessage.getInt("state") > -2))
        {
          failedMessageRes = failedMessage;
        }
        else failedMessagesStore.put(failedMessage);
      }

      storageSet("failedMessages", failedMessagesStore);
    }
    return failedMessageRes;
  }

  protected JSONObject popFailedMessage(String id) throws JSONException
  {
    JSONArray failedMessages = storageGetArr("failedMessages");
    JSONArray failedMessagesStore = new JSONArray();
    JSONObject failedMessageRes = null;
    if (failedMessages.length() > 0) {
      for (int i = 0; i < failedMessages.length(); i++) {
        JSONObject failedMessage = failedMessages.getJSONObject(i);
        if ((failedMessageRes == null) && (failedMessage.getString("id").equals(id)))
        {
          failedMessageRes = failedMessage;
        }
        else failedMessagesStore.put(failedMessage);
      }

      storageSet("failedMessages", failedMessagesStore);
    }
    return failedMessageRes;
  }

  protected JSONArray getFailedMessages(String issueId) throws JSONException {
    JSONArray failedMessages = storageGetArr("failedMessages");
    JSONArray failedMessagesIssue = new JSONArray();
    for (int i = 0; i < failedMessages.length(); i++) {
      JSONObject failedMessage = failedMessages.getJSONObject(i);
      if (failedMessage.getString("issue_id").equals(issueId)) {
        failedMessagesIssue.put(failedMessage);
      }
    }
    return failedMessagesIssue;
  }

  protected void storeFailedMessage(String issueId, String messageText, String type, String refers, int failedState)
    throws JSONException
  {
    Date now = new Date();
    JSONObject failedMessage = new JSONObject();
    failedMessage.put("issue_id", issueId);
    failedMessage.put("body", messageText);
    failedMessage.put("created_at", HSFormat.issueTsFormat.format(now));
    failedMessage.put("type", type);
    failedMessage.put("state", -1);
    failedMessage.put("origin", "mobile");
    failedMessage.put("refers", refers);
    failedMessage.put("id", now.getTime() + "");
    failedMessage.put("state", failedState);
    storeFailedMessage(failedMessage);
  }

  protected void storeFailedMessage(JSONObject failedMessage) throws JSONException
  {
    JSONArray failedMessages = getFailedMessages();
    failedMessages.put(failedMessage);
    storageSet("failedMessages", failedMessages);
  }

  protected JSONObject getFailedApiCalls() throws JSONException {
    return storageGetObj("failedApiCalls");
  }

  protected void storeFailedApiCall(String key, JSONObject failedApiCall) throws JSONException
  {
    JSONObject failedApiCalls = getFailedApiCalls();
    failedApiCalls.put(key, failedApiCall);
    storageSet("failedApiCalls", failedApiCalls);
  }

  protected void storeIssue(JSONObject issue) throws JSONException {
    storageSet(issue.getString("id"), issue);
  }

  protected void storeReply(String replyText) {
    storageSet("replyText", replyText);
  }

  protected String getReply() {
    return storageGet("replyText");
  }

  protected void storeConversationDetail(String conversationText) {
    storageSet("conversationDetail", conversationText);
    storageSet("conversationDetailSetTime", Long.valueOf(System.nanoTime()));
  }

  protected String getConversationDetail() {
    long timeDiff = System.nanoTime() - storageGetLong("conversationDetailSetTime");

    if ((timeDiff < 0L) || (TimeUnit.NANOSECONDS.toSeconds(timeDiff) > 7200L))
    {
      storageSet("conversationDetail", "");
    }

    return storageGet("conversationDetail");
  }

  protected void setServerTimeDelta(Float delta) {
    storageSet("serverTimeDelta", delta);
  }

  protected Float getServerTimeDelta() {
    return storageGetFloat("serverTimeDelta");
  }

  protected void setUUID(String id) {
    storageSet("uuid", id);
  }

  protected String getUUID() {
    return storageGet("uuid");
  }

  protected void setMonthCrossed(Boolean monthCrossed) {
    storageSet("monthCrossed", monthCrossed);
  }

  protected Boolean getMonthCrossed() {
    return storageGetBoolean("monthCrossed");
  }

  protected void setUpdateYear(int year) {
    storageSet("updateYear", Integer.valueOf(year));
  }

  protected int getUpdateYear() {
    return storageGetInt("updateYear").intValue();
  }

  protected void setUpdateMonth(int year) {
    storageSet("updateMonth", Integer.valueOf(year));
  }

  protected int getUpdateMonth() {
    return storageGetInt("updateMonth").intValue();
  }

  protected void setSdkUpdated(Boolean monthCrossed) {
    storageSet("sdkUpdated", monthCrossed);
  }

  protected Boolean getSdkUpdated() {
    return storageGetBoolean("sdkUpdated");
  }

  public void setConversationScreenshot(String screenshotPath) {
    storageSet("screenshotPath", screenshotPath);
  }

  public String getConversationScreenshot() {
    return storageGet("screenshotPath");
  }

  public void setScreenShotDraft(Boolean status) {
    storageSet("screenShotDraft", status);
  }

  public Boolean getScreenShotDraft() {
    return storageGetBoolean("screenShotDraft");
  }

  public void setIsConversationShowing(Boolean status) {
    storageSet("isConversationShowing", status);
  }

  public Boolean getIsConversationShowing() {
    return storageGetBoolean("isConversationShowing");
  }

  public void setRequireEmail(Boolean status) {
    storageSet("requireEmail", status);
  }

  public Boolean getRequireEmail() {
    return storageGetBoolean("requireEmail");
  }
}