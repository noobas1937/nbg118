/**
 * Copyright (c) 2013-2014 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.elex.chatservice;

import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.content.Intent;
import android.app.Activity;
import java.util.HashMap;
import java.util.ArrayList;

import com.elex.chatservice.Helpshift;

public class HelpshiftBridge
{
  private static final String TAG = "HSCocosPlugin";
  private static Activity activity;
  public static native void alertToRateAppAction(int result);
  public static void init (Activity a) {
    HelpshiftBridge.activity = a;
  }

  private static HashMap parseConfigDictionary (HashMap config) {
    if(config == null) {
      return new HashMap();
    }
    if (config.get("enableInAppNotification") != null) {
      if(config.get("enableInAppNotification").equals("yes")) {
        config.put("enableInAppNotification", true);
      } else {
        config.put("enableInAppNotification", false);
      }
    }

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
  }

  public static void showConversation() {
	  try{
		  HelpshiftBridge.showConversation(new HashMap());
	  }catch(Exception e)
	  {
		  
	  }
  }

  public static void showConversation(HashMap config) {
	  try{
		  Helpshift.showConversation(HelpshiftBridge.activity, parseConfigDictionary(config));
	  }catch(Exception e)
	  {
		  
	  }
  }

  public static void showFAQSection(String sectionPublishId) {
    HelpshiftBridge.showFAQSection(sectionPublishId, new HashMap());
  }

  public static void showFAQSection(String sectionPublishId, HashMap config) {
    Helpshift.showFAQSection(HelpshiftBridge.activity, sectionPublishId, parseConfigDictionary(config));
  }

  public static void showSingleFAQ(String questionPublishId) {
    HelpshiftBridge.showSingleFAQ(questionPublishId, new HashMap());
  }

  public static void showSingleFAQ(String questionPublishId, HashMap config) {
    Helpshift.showSingleFAQ(HelpshiftBridge.activity, questionPublishId, parseConfigDictionary(config));
  }

  public static void showFAQs() {
	  try{
		  HelpshiftBridge.showFAQs(new HashMap());
	  }catch(Exception e)
	  {
		  
	  }
  }

  public static void showFAQs(HashMap config) {
	  try{
		  Helpshift.showFAQs(HelpshiftBridge.activity, parseConfigDictionary(config));
	  }catch(Exception e)
	  {
		  
	  }
  }

  public static void setNameAndEmail(String name, String email) {
    Helpshift.setNameAndEmail(name, email);
  }

  public static void setUserIdentifier(String identifier) {
    Helpshift.setUserIdentifier(identifier);
  }

  public static void registerDeviceToken(String deviceToken) {
    Helpshift.registerDeviceToken(HelpshiftBridge.activity, deviceToken);
  }

  public static void leaveBreadCrumb(String breadCrumb) {
    Helpshift.leaveBreadCrumb(breadCrumb);
  }

  public static void clearBreadCrumbs() {
    Helpshift.clearBreadCrumbs();
  }

  public static Integer getNotificationCount() {
    return Helpshift.getNotificationCount();
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
