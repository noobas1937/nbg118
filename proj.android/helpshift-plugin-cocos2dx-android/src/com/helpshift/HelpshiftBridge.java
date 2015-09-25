/**
 * Copyright (c) 2014-2015 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;
import android.os.Looper;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.util.Log;
import android.content.Intent;
import android.app.Activity;

import java.util.HashMap;
import java.util.ArrayList;


import com.helpshift.Helpshift;

public class HelpshiftBridge
{
  private static final String TAG = "HSCocosPlugin";
  private static Activity activity;
  private static String gcmRegId;

  public static native void alertToRateAppAction(int result);
  public static native void didReceiveNotificationCount(int result);
  public static native void helpshiftSessionBegan();
  public static native void helpshiftSessionEnded();
  public static native void newConversationStarted(String newConversationMessage);
  public static native void userRepliedToConversation(String newMessage);
  public static native void userCompletedCustomerSatisfactionSurvey(int rating, String feedback);

  public static void init (Activity a) {
    HelpshiftBridge.activity = a;
  }

  private static HashMap parseConfigDictionary (HashMap config) {
    if(config == null) {
      return new HashMap();
    }
    /*
      Install time config flags
    */
    if (config.get("enableInAppNotification") != null) {
      if(config.get("enableInAppNotification").equals("yes")) {
        config.put("enableInAppNotification", true);
      } else {
        config.put("enableInAppNotification", false);
      }
    }

    if (config.get("enableDialogUIForTablets") != null) {
      if(config.get("enableDialogUIForTablets").equals("yes")) {
        config.put("enableDialogUIForTablets", true);
      } else {
        config.put("enableDialogUIForTablets", false);
      }
    }

    /*
      API configs
    */
    if (config.get("enableContactUs") != null) {
      if(config.get("enableContactUs").equals("yes") ||
         config.get("enableContactUs").equals("always")) {
        config.put("enableContactUs", Helpshift.ENABLE_CONTACT_US.ALWAYS);
      } else if (config.get("enableContactUs").equals("no") ||
                 config.get("enableContactUs").equals("never")){
        config.put("enableContactUs", Helpshift.ENABLE_CONTACT_US.NEVER);
      } else if (config.get("enableContactUs").equals("after_viewing_faqs")) {
        config.put("enableContactUs", Helpshift.ENABLE_CONTACT_US.AFTER_VIEWING_FAQS);
      }
    }

    if (config.get("gotoConversationAfterContactUs") != null) {
      if(config.get("gotoConversationAfterContactUs").equals("yes")) {
        config.put("gotoConversationAfterContactUs", true);
      } else {
        config.put("gotoConversationAfterContactUs", false);
      }
    }

    if (config.get("requireEmail") != null) {
      if(config.get("requireEmail").equals("yes")) {
        config.put("requireEmail", true);
      } else {
        config.put("requireEmail", false);
      }
    }

    if (config.get("hideNameAndEmail") != null) {
      if(config.get("hideNameAndEmail").equals("yes")) {
        config.put("hideNameAndEmail", true);
      } else {
        config.put("hideNameAndEmail", false);
      }
    }

    if (config.get("showSearchOnNewConversation") != null) {
      if(config.get("showSearchOnNewConversation").equals("yes")) {
        config.put("showSearchOnNewConversation", true);
      } else {
        config.put("showSearchOnNewConversation", false);
      }
    }

    if (config.get("enableFullPrivacy") != null) {
      if(config.get("enableFullPrivacy").equals("yes")) {
        config.put("enableFullPrivacy", true);
      } else {
        config.put("enableFullPrivacy", false);
      }
    }

    HashMap customMeta = (HashMap) (config.get(Helpshift.HSCustomMetadataKey));
    if(customMeta != null) {
      ArrayList<String> tags = (ArrayList<String>) customMeta.get(Helpshift.HSTagsKey);
      if(tags != null && tags.size() > 0) {
        String[] tagsArray = (String[])tags.toArray(new String[tags.size()]);
        customMeta.put(Helpshift.HSTagsKey, tagsArray);
      }
    }
    return config;
  }

  public static void install (Activity a, String apiKey, String domain, String appId) {
    HashMap<String, Object> config = new HashMap<String, Object>();
    config.put ("sdkType", "cocos2dx");
    HelpshiftBridge.install (a, apiKey, domain, appId, config);
  }

  public static void install (Activity a, String apiKey, String domain, String appId, HashMap config) {
    HelpshiftBridge.activity = a;
    HashMap parsedConfig = parseConfigDictionary(config);
    parsedConfig.put("sdkType", "cocos2dx");
    Helpshift.install(HelpshiftBridge.activity.getApplication(), apiKey, domain, appId, parsedConfig);
    HelpshiftSessionDelegate sessionDelegate = new HelpshiftSessionDelegate();
    Helpshift.setDelegate(sessionDelegate);
    regGCM();
  }

  public static void showConversation() {
    HelpshiftBridge.showConversation(new HashMap());
  }

  public static void showConversation(HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Helpshift.showConversation(HelpshiftBridge.activity, parseConfigDictionary(config));
  }

  public static void showFAQSection(String sectionPublishId) {
	HelpshiftBridge.showFAQSection(sectionPublishId, new HashMap());
  }

  public static void showFAQSection(String sectionPublishId, HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Helpshift.showFAQSection(HelpshiftBridge.activity, sectionPublishId, parseConfigDictionary(config));
  }

  public static void showSingleFAQ(String questionPublishId) {
    HelpshiftBridge.showSingleFAQ(questionPublishId, new HashMap());
  }

  public static void showSingleFAQ(String questionPublishId, HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Helpshift.showSingleFAQ(HelpshiftBridge.activity, questionPublishId, parseConfigDictionary(config));
  }

  public static void showFAQs() {
    HelpshiftBridge.showFAQs(new HashMap());
  }

  public static void showFAQs(HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Helpshift.showFAQs(HelpshiftBridge.activity, parseConfigDictionary(config));
  }

  public static void setNameAndEmail(String name, String email) {
    Helpshift.setNameAndEmail(name, email);
  }

  public static void login(String identifier, String name, String email) {
    Helpshift.login(identifier, name, email);
  }

  public static void logout() {
    Helpshift.logout();
  }

  public static void setUserIdentifier(String identifier) {
    Helpshift.setUserIdentifier(identifier);
  }

  public static void registerDeviceToken(String deviceToken) {
	gcmRegId = deviceToken;
	regGCM();
  }
  
  private static void regGCM(){
	if(HelpshiftBridge.activity != null && gcmRegId != null){
		Log.d("COK", "COK Helpshift registerDeviceToken " + gcmRegId);
		Helpshift.registerDeviceToken(HelpshiftBridge.activity, gcmRegId);
	}else{
		Log.d("COK", "COK Helpshift registerDeviceToken delay");
	}
  }

  public static void leaveBreadCrumb(String breadCrumb) {
    Helpshift.leaveBreadCrumb(breadCrumb);
  }

  public static void clearBreadCrumbs() {
    Helpshift.clearBreadCrumbs();
  }

  public static int getNotificationCount(boolean isAsync) {
    int count = -1;
    if (!isAsync) {
      count = Helpshift.getNotificationCount();
    } else {
      Handler mainThreadHandler = new Handler(Looper.getMainLooper());
      mainThreadHandler.post (new Runnable() {
          public void run () {
            Handler countHandler = new Handler() {
                public void handleMessage(Message msg) {
                  super.handleMessage(msg);
                  Bundle countData = (Bundle) msg.obj;
                  Integer count = countData.getInt("value");
                  didReceiveNotificationCount(count);
                }
              };
            Helpshift.getNotificationCount(countHandler, new Handler());
          }
        });
    }
    return count;
  }

  public static void handlePush (String issueId) {
    Helpshift.handlePush(HelpshiftBridge.activity, issueId);
  }

  public static void showAlertToRateApp (String url) {
    HSAlertToRateAppListener actionListener =  new HSAlertToRateAppListener() {
        @Override
        public void onAction(Helpshift.HS_RATE_ALERT action) {
          int msg = 0;
          switch (action) {
          case CLOSE:
            msg = 0;
            break;
          case FEEDBACK:
            msg = 1;
            break;
          case SUCCESS:
            msg = 2;
            break;
          case FAIL:
            msg = 3;
            break;
          }
          alertToRateAppAction(msg);
        }
      };
    Helpshift.showAlertToRateApp(url, actionListener);
  }
}
