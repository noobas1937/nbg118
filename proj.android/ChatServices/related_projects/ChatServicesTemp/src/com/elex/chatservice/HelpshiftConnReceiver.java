package com.elex.chatservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import java.util.Date;

public final class HelpshiftConnReceiver extends BroadcastReceiver
{
  private static Date lastTs;

  private boolean isOnline(Context context)
  {
    ConnectivityManager connectivityManager = (ConnectivityManager)context.getSystemService("connectivity");

    NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
    return (activeNetworkInfo != null) && (activeNetworkInfo.isConnected());
  }

  public void onReceive(Context context, Intent intent)
  {
    Date now = new Date();
    if (lastTs == null) {
      lastTs = now;
    }

    if (((now.getTime() - lastTs.getTime() > 6000L) || (now.getTime() - lastTs.getTime() == 0L)) && (isOnline(context)))
    {
      context.startService(new Intent(context, HSRetryService.class));
    }
  }
}