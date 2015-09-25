package com.elex.chatservice.util;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.util.Log;

public final class HSPolling
{
  public static final String TAG = "HelpShiftDebug";
  private Handler task;
  private float interval;
  private float minInterval;
  private float maxInterval = 60000.0F;
  private Boolean smartPolling = Boolean.valueOf(false);
  private Context c = null;

  Runnable pollerRun = new Runnable()
  {
    public void run() {
      task.sendMessage(task.obtainMessage());
      task.postDelayed(pollerRun, (long)interval);
      if (smartPolling.booleanValue())
        updateInterval(interval);
    }
  };

  private static Float getBatteryLevel(Intent batteryStatus)
  {
    int level = batteryStatus.getIntExtra("level", -1);
    int scale = batteryStatus.getIntExtra("scale", -1);

    return Float.valueOf(level / scale);
  }

  private static String getNetworkType(Context c)
  {
    ConnectivityManager cm = (ConnectivityManager)c.getSystemService("connectivity");
    NetworkInfo ani = cm.getActiveNetworkInfo();
    String type = "Unknown";
    if (ani != null) {
      type = ani.getTypeName();
    }
    return type;
  }

  public HSPolling(Handler task, int interval)
  {
    this.task = task;
    this.minInterval = (interval * 1000);
    this.interval = (interval * 1000);
  }

  public HSPolling(Handler task, int interval, Boolean smartPolling, Context c)
  {
    this.task = task;
    this.interval = (interval * 1000);
    this.minInterval = (interval * 1000);
    this.smartPolling = smartPolling;
    this.c = c;
  }

  public void resetInterval() {
    this.interval = this.minInterval;
    this.task.removeCallbacks(this.pollerRun);
    this.task.postDelayed(this.pollerRun, (long)this.interval);
  }

  public void startRepeatingTask() {
    this.pollerRun.run();
  }

  public void stopRepeatingTask() {
    this.task.removeCallbacks(this.pollerRun);
  }

  public void updateInterval(float interval) {
    if (interval < this.maxInterval) {
      interval = (interval + this.minInterval) * 1.618F;

      IntentFilter ifilter = new IntentFilter("android.intent.action.BATTERY_CHANGED");
      Intent batteryStatus = this.c.registerReceiver(null, ifilter);
      interval *= (2.0F - getBatteryLevel(batteryStatus).floatValue());
      try
      {
        if (!getNetworkType(this.c).equals("WIFI"))
          interval *= 1.618F;
      }
      catch (SecurityException e) {
        Log.d("HelpShiftDebug", "No permission for Network Access", e);
      }

      if (interval > this.maxInterval)
        this.interval = this.maxInterval;
      else
        this.interval = interval;
    }
  }
}