package com.elex.chatservice;

import android.app.Activity;

final class HSAnalytics
{
  private static HSApiData data = null;
  private static int started;
  private static int stopped;
  private static boolean isForeground;
  protected static boolean decomp;

  public static void onActivityStarted(Activity activity)
  {
    if (data == null) {
      data = new HSApiData(activity);
    }

    started += 1;

    if (!isForeground) {
      if (decomp)
        HSFunnel.pushEvent("d");
      else {
        HSFunnel.pushEvent("o");
      }
    }

    isForeground = true;
  }

  public static void onActivityStopped(Activity activity) {
    stopped += 1;
    if (started == stopped) {
      isForeground = false;
      HSFunnel.pushEvent("q");
      data.reportActionEvents();
    }
  }
}