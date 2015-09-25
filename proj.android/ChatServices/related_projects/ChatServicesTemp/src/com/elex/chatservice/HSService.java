package com.elex.chatservice;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Service;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Binder;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

import com.elex.chatservice.util.HSNotification;
import com.elex.chatservice.util.HSPolling;

import java.util.Iterator;
import java.util.List;

public final class HSService extends Service
{
  private HSApiData data = null;
  public static final String TAG = "HelpShiftDebug";
  private Boolean foreground;
  private HSLifecycleCallbacks hsLifecycleCallbacks = null;

  private final IBinder hsBinder = new HSBinder();

  private Handler getCountHandler = new Handler() {
    public void handleMessage(Message msg) {
      if (isAppOnForeground()) {
        if ((!foreground.booleanValue()) && (HSService.notifCountPoller != null))
        {
          HSService.notifCountPoller.resetInterval();
        }
        foreground = Boolean.valueOf(true);
        Handler notifHandler = HSNotification.getNotifHandler(HSService.this, HSService.notifCountPoller);

        data.getNotificationData(notifHandler, new Handler());
      } else {
        foreground = Boolean.valueOf(false);
      }
    }
  };

  private static HSPolling notifCountPoller = null;

  private String getAppName()
  {
    PackageManager pm = getPackageManager();
    ApplicationInfo ai;
    try
    {
      ai = pm.getApplicationInfo(getPackageName(), 0);
    } catch (PackageManager.NameNotFoundException e) {
      ai = null;
    }
    String applicationName = (String)(ai != null ? pm.getApplicationLabel(ai) : "(unknown)");

    return applicationName;
  }

  private boolean isAppOnForeground() {
    if (Build.VERSION.SDK_INT >= 14) {
      if (this.hsLifecycleCallbacks == null) {
        this.hsLifecycleCallbacks = HSLifecycleCallbacks.getInstance();
      }
      return HSLifecycleCallbacks.isForeground();
    }
    ActivityManager activityManager = (ActivityManager)getSystemService("activity");
    List appProcesses = activityManager.getRunningAppProcesses();
    if (appProcesses == null) {
      return false;
    }
    String packageName = getPackageName();
//    for (ActivityManager.RunningAppProcessInfo appProcess : appProcesses) {
//      if ((appProcess.importance == 100) && (appProcess.processName.equals(packageName)))
//      {
//        return true;
//      }
//    }
    for(Iterator i$ = appProcesses.iterator(); i$.hasNext();)
    {
        android.app.ActivityManager.RunningAppProcessInfo appProcess = (android.app.ActivityManager.RunningAppProcessInfo)i$.next();
        if(appProcess.importance == 100 && appProcess.processName.equals(packageName))
            return true;
    }

    return false;
  }

  public int onStartCommand(Intent intent, int flags, int startId)
  {
    if (this.data == null) {
      this.data = new HSApiData(this);
    }

    if (this.foreground == null) {
      this.foreground = Boolean.valueOf(true);
    }

    String deviceToken = this.data.storage.getDeviceToken();
    if ((notifCountPoller == null) && ((deviceToken.equals("")) || (deviceToken.equals("unreg"))))
    {
      notifCountPoller = new HSPolling(this.getCountHandler, 5, Boolean.valueOf(true), this);
      notifCountPoller.startRepeatingTask();
    }

    return 1;
  }

  public IBinder onBind(Intent intent)
  {
    return this.hsBinder;
  }

  public void onDestroy() {
    if (notifCountPoller != null) {
      notifCountPoller.stopRepeatingTask();
      notifCountPoller = null;
    }

    super.onDestroy();
  }

  public static void resetInterval() {
    if (notifCountPoller != null)
      notifCountPoller.resetInterval();
  }

  public static void stopPolling()
  {
    if (notifCountPoller != null)
      notifCountPoller.stopRepeatingTask();
  }

  public class HSBinder extends Binder
  {
    public HSBinder()
    {
    }
  }
}