package com.elex.chatservice;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import com.elex.chatservice.res.values.HSConfig;

import org.json.JSONException;
import org.json.JSONObject;

final class HSLifecycleCallbacks
  implements Application.ActivityLifecycleCallbacks
{
  private static final String TAG = "HelpShiftDebug";
  private static HSLifecycleCallbacks instance = null;
  private static HSApiData data = null;
  private static HSStorage storage = null;
  private static int started;
  private static int stopped;
  private static boolean isForeground;

  public static HSLifecycleCallbacks getInstance()
  {
    if (instance == null) {
      instance = new HSLifecycleCallbacks();
    }
    return instance;
  }

  public static boolean isForeground() {
    return isForeground;
  }
  public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
  }

  public void onActivityStarted(Activity activity) {
    if (data == null) {
      data = new HSApiData(activity.getApplication());
      storage = data.storage;
    }

    started += 1;

    if (!isForeground) {
      data.updateReviewCounter();
      final Context c = activity.getApplicationContext();
      if (data.showReviewP().booleanValue()) {
        Handler mainThreadHandler = new Handler(Looper.getMainLooper());
        mainThreadHandler.post(new Runnable()
        {
          public void run() {
            Intent i = new Intent(c, HSReview.class);
            i.setFlags(268435456);
            c.startActivity(i);
          }
        });
      }
      try
      {
        data.getConfig(new Handler() {
          public void handleMessage(Message msg) {
            HSConfig.updateConfig((JSONObject)msg.obj);
            HSLifecycleCallbacks.storage.updateActiveConversation();
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
    isForeground = true;
  }
  public void onActivityResumed(Activity activity) {
  }
  public void onActivityPaused(Activity activity) {
  }

  public void onActivityStopped(Activity activity) {
    stopped += 1;
    if (started == stopped)
      isForeground = false;
  }

  public void onActivityDestroyed(Activity activity)
  {
  }

  public void onActivitySaveInstanceState(Activity activity, Bundle outState)
  {
  }
}