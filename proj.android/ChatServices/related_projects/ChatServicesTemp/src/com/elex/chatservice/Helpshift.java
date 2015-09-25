package com.elex.chatservice;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;

import com.elex.chatservice.res.drawable.HSImages;
import com.elex.chatservice.res.values.HSConfig;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSNotification;
import com.elex.chatservice.util.HSPattern;
import com.elex.chatservice.util.HSTimeUtil;
import com.elex.chatservice.util.Meta;

import java.util.Calendar;
import java.util.HashMap;
import org.json.JSONException;
import org.json.JSONObject;

public final class Helpshift
{
  public static final String JSON_PREFS = "HSJsonData";
  public static final String TAG = "HelpShiftDebug";
  public static final String libraryVersion = "3.4.1";

  /** @deprecated */
  public static final String HSIssueTagsKey = "hs-tags";
  public static final String HSTagsKey = "hs-tags";
  public static final String HSCustomMetadataKey = "hs-custom-metadata";
  private static HSApiData data = null;
  private static HSStorage storage = null;
  private static Context c = null;
  private static Handler mainThreadHandler = null;

  private static void init(Application application)
  {
    initialize(application.getApplicationContext());
  }

  private static void init(Context context) {
    initialize(context.getApplicationContext());
  }

  private static void initialize(Context context) {
    if (c == null) {
      data = new HSApiData(context);
      storage = data.storage;
      ContactUsFilter.init(context);
      c = context;
      mainThreadHandler = new Handler(Looper.getMainLooper());
    }
  }

  public static void install(Application application, String apiKey, String domain, String appId)
  {
    install(application, apiKey, domain, appId, new HashMap());
  }

  public static void install(Application application, String apiKey, String domain, String appId, HashMap config)
  {
    init(application);

    String identity = storage.getIdentity();
    Boolean monthCrossed = storage.getMonthCrossed();
    Float timeDelta = storage.getServerTimeDelta();

    String oldVersion = storage.getLibraryVersion();
    if ((oldVersion.length() > 0) && (!oldVersion.equals("3.4.1")))
    {
      storage.clearDatabase();
      storage.setIdentity(identity);

      storage.setMonthCrossed(monthCrossed);
      storage.setSdkUpdated(Boolean.valueOf(true));
      storage.setServerTimeDelta(timeDelta);

      Calendar today = Calendar.getInstance();
      today.setTimeInMillis(HSTimeUtil.getAdjustedTimeInMillis(timeDelta));
      storage.setUpdateMonth(today.get(2));
      storage.setUpdateYear(today.get(1));
    }
    storage.setLibraryVersion("3.4.1");

    if ((String)config.get("sdkType") != null)
      storage.setSdkType((String)config.get("sdkType"));
    else {
      storage.setSdkType("android");
    }
    try
    {
      String applicationVersion = c.getPackageManager().getPackageInfo(c.getPackageName(), 0).versionName;

      if (!storage.getApplicationVersion().equals(applicationVersion)) {
        data.resetReviewCounter();
        data.enableReview();
        storage.setApplicationVersion(applicationVersion);
      }
    } catch (PackageManager.NameNotFoundException e) {
      Log.d("HelpShiftDebug", "Application Name Not Found", e);
    }

    HSImages.initImages(c);
    storage.setAppConfig(new JSONObject(config));

    data.install(apiKey, domain, appId);

    if (!TextUtils.isEmpty(identity)) {
      try {
        data.getLatestIssues(new Handler(), new Handler());
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "Install - Get Latest Issues", e);
      }
    }

    data.startInAppService();

    if (Build.VERSION.SDK_INT >= 14) {
      HSLifecycleCallbacks hsLifecycleCallbacks = HSLifecycleCallbacks.getInstance();

      application.unregisterActivityLifecycleCallbacks(hsLifecycleCallbacks);
      application.registerActivityLifecycleCallbacks(hsLifecycleCallbacks);
    } else {
      data.updateReviewCounter();
      try
      {
        data.getConfig(new Handler() {
          public void handleMessage(Message msg) {
            HSConfig.updateConfig((JSONObject)msg.obj);
            Helpshift.storage.updateActiveConversation();
          }
        }
        , new Handler());
      }
      catch (JSONException e)
      {
        Log.d("HelpShiftDebug", e.toString(), e);
      }

      data.reportAppStartEvent();
    }
    Log.d("HelpShiftDebug", "install() call complete");
  }

  public static Integer getNotificationCount()
  {
    if (data != null) {
      return data.storage.getActiveNotifCnt();
    }
    return Integer.valueOf(0);
  }

  public static void getNotificationCount(Handler success, final Handler failure)
  {
    if (success == null) {
      Log.d("HelpShiftDebug", "success handler in getNotificationCount(Handler success, Handler failure) is null");
    } else {
      Integer activeCnt = data.storage.getActiveNotifCnt();
      Message msgToPost = success.obtainMessage();
      Bundle countData = new Bundle();
      countData.putInt("value", activeCnt.intValue());
      countData.putBoolean("cache", true);
      msgToPost.obj = countData;
      success.sendMessage(msgToPost);

      Handler localFailure = new Handler()
      {
        public void handleMessage(Message msg) {
          if (failure == null) {
            Log.d("HelpShiftDebug", "failure handler in getNotificationCount(Handler success, Handler failure) is null");
          } else {
            Message msgToPost = failure.obtainMessage();
            msgToPost.obj = msg.obj;
            failure.sendMessage(msgToPost);
          }
        }
      };
      String identity = storage.getIdentity();

      if (!TextUtils.isEmpty(identity)) {
        data.getNotificationCount(success, localFailure);
      } else {
        Message failureMsg = localFailure.obtainMessage();
        Bundle failureCount = new Bundle();
        failureCount.putInt("value", -1);
        failureMsg.obj = failureCount;
        localFailure.sendMessage(failureMsg);
      }
    }
  }

  public static void setNameAndEmail(String name, String email)
  {
    if ((name != null) && (!HSPattern.checkSpecialCharacters(name))) {
      storage.setUsername(name.trim());
    }

    if ((email != null) && (HSPattern.checkEmail(email))) {
      storage.setEmail(email.trim());
    }

    if ((name == null) && (email == null)) {
      storage.setUsername("");
      storage.setEmail("");
    }
  }

  public static void setUserIdentifier(String userIdentifier)
  {
    if (userIdentifier != null)
      storage.setDeviceIdentifier(userIdentifier.trim());
  }

  public static void registerDeviceToken(Context context, final String deviceToken)
  {
    init(context);
    mainThreadHandler.post(new Runnable()
    {
      public void run() {
        if (deviceToken != null) {
          String profileId = Helpshift.storage.getIdentity();
          Helpshift.storage.setDeviceToken(deviceToken);

          if (!TextUtils.isEmpty(profileId))
            Helpshift.data.updateUAToken();
        }
        else {
          Log.d("HelpShiftDebug", "Device Token is null");
        }
      }
    });
  }

  public static void leaveBreadCrumb(String breadCrumb)
  {
    if ((breadCrumb != null) && (!TextUtils.isEmpty(breadCrumb.trim())))
    {
      storage.pushBreadCrumb(breadCrumb);
    }
  }

  public static void clearBreadCrumbs()
  {
    storage.clearBreadCrumbs();
  }

  public static void showConversation(Activity a)
  {
	  System.out.println("showConversation1");
    showConversation(a, new HashMap());
  }

  public static void showChatActivity(Activity a)
  {
	  System.out.println("Helpshift showChatActivity()");
    Intent i = new Intent(a, ChatActivity.class);
    i.putExtra("decomp", true);
    i.putExtras(cleanConfig(new HashMap()));
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(a));
    i.putExtra("chatLaunchSource", "support");
    i.putExtra("isRoot", true);
    HSActivityUtil.sessionBeginning();
    a.startActivity(i);
  }

  public static void showConversation(Activity a, HashMap config)
  {
	  System.out.println("showConversation2");
    Intent i = new Intent(a, HSConversation.class);
    i.putExtra("decomp", true);
    i.putExtras(cleanConfig(config));
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(a));
    i.putExtra("chatLaunchSource", "support");
    i.putExtra("isRoot", true);
    HSActivityUtil.sessionBeginning();
    a.startActivity(i);
  }

  public static void showFAQSection(Activity a, String sectionPublishId)
  {
    showFAQSection(a, sectionPublishId, new HashMap());
  }

  public static void showFAQSection(Activity a, String sectionPublishId, HashMap config)
  {
    Intent i = new Intent(a, HSSection.class);
    i.putExtras(cleanConfig(config));
    i.putExtra("sectionPublishId", sectionPublishId);
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(a));
    i.putExtra("decomp", true);
    i.putExtra("isRoot", true);
    HSActivityUtil.sessionBeginning();
    a.startActivity(i);
  }

  public static void showSingleFAQ(Activity a, String questionPublishId)
  {
    showSingleFAQ(a, questionPublishId, new HashMap());
  }

  public static void showSingleFAQ(Activity a, String questionPublishId, HashMap config)
  {
    Intent i = new Intent(a, HSQuestion.class);
    i.putExtras(cleanConfig(config));
    i.putExtra("questionPublishId", questionPublishId);
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(a));
    i.putExtra("decomp", true);
    i.putExtra("isRoot", true);
    HSActivityUtil.sessionBeginning();
    a.startActivity(i);
  }

  public static void setMetadataCallback(HSCallable f)
  {
    Meta.setMetadataCallback(f);
  }

  private static void createMetadataCallback(final HashMap config) {
    HSCallable f = null;
    if (config.containsKey("hs-custom-metadata")) {
      f = new HSCallable()
      {
        public HashMap call() {
          if ((config.get("hs-custom-metadata") instanceof HashMap)) {
            return (HashMap)config.get("hs-custom-metadata");
          }

          return null;
        }
      };
      setMetadataCallback(f);
    }
  }

  public static void setMetaData(String metaDataString)
  {
    try
    {
      storage.setMetaData(new JSONObject(metaDataString));
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", "Error setting metaData ", e);
    }
  }

  public static void showFAQs(Activity a)
  {
	  System.out.println("showFAQs1");
    showFAQs(a, new HashMap());
  }

  public static void showFAQs(Activity a, HashMap config)
  {
	  System.out.println("showFAQs2");
    Intent i = new Intent(a, HSFaqs.class);
    i.putExtras(cleanConfig(config));
    i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(a));
    i.putExtra("decomp", false);
    i.putExtra("isRoot", true);
    HSActivityUtil.sessionBeginning();
    a.startActivity(i);
  }

  private static Bundle cleanConfig(HashMap configMap) {
    ContactUsFilter.setConfig(configMap);
    Bundle cleanConfig = new Bundle();
    if (configMap != null) {
      createMetadataCallback(configMap);
      JSONObject config = new JSONObject();
      config = new JSONObject(configMap);

      Boolean showConvOnReportIssue = Boolean.valueOf((config.optBoolean("gotoCoversationAfterContactUs", false)) || (config.optBoolean("gotoConversationAfterContactUs", false)));

      storage.setRequireEmail(Boolean.valueOf(config.optBoolean("requireEmail", false)));

      cleanConfig.putBoolean("showConvOnReportIssue", showConvOnReportIssue.booleanValue());
    }

    return cleanConfig;
  }

  public static void handlePush(Context context, Intent i)
  {
    init(context);
    String issueId = i.getExtras().getString("issue_id");

    if (!storage.getForegroundIssue().equals(issueId))
      try {
        int messCnt = storage.getIssuePushCount(issueId);
        JSONObject issue = storage.getIssue(issueId);
        HSNotification.showNotif(c, issue, messCnt, "push");
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "handlePush JSONException", e);
      }
  }

  public static void showAlertToRateApp(String url, HSAlertToRateAppListener alertToRateAppListener)
  {
    Intent intent = new Intent("android.intent.action.VIEW");
    if (!TextUtils.isEmpty(url)) {
      intent.setData(Uri.parse(url.trim()));
    }

    if ((TextUtils.isEmpty(url)) || (intent.resolveActivity(c.getPackageManager()) == null))
    {
      if (alertToRateAppListener != null) {
        alertToRateAppListener.onAction(HS_RATE_ALERT.FAIL);
      }
      return;
    }

    HSReviewFragment.setAlertToRateAppListener(alertToRateAppListener);

    Intent i = new Intent(c, HSReview.class);
    i.putExtra("disableReview", false);
    i.putExtra("rurl", url.trim());
    i.setFlags(268435456);
    c.startActivity(i);
  }

  public static void handlePush(Context context, String issueId)
  {
    Intent i = new Intent();
    i.putExtra("issue_id", issueId);
    handlePush(context, i);
  }

  public static enum ENABLE_CONTACT_US
  {
    ALWAYS, 

    NEVER, 

    AFTER_VIEWING_FAQS;
  }

  public static enum HS_RATE_ALERT
  {
    SUCCESS, 

    FEEDBACK, 

    CLOSE, 

    FAIL;
  }
}