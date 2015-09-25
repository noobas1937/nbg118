package com.elex.chatservice.util;

import android.net.ParseException;
import android.util.Log;
import java.text.DecimalFormat;
import java.util.Date;

public final class HSTimeUtil
{
  private static final String TAG = "HelpShiftDebug";

  public static Float calculateTimeAdjustment(String serverTime)
  {
    Float timeDelta = new Float(0.0F);
    String deviceTime = HSFormat.tsSecFormatter.format(System.currentTimeMillis() / 1000.0D);
    try {
      Double timeStamp = Double.valueOf(Double.parseDouble(serverTime) * 1000.0D);
      Date serverDate = new Date((long)timeStamp.doubleValue());
      timeDelta = Float.valueOf((float)(serverDate.getTime() / 1000L - Double.parseDouble(deviceTime)));
    } catch (ParseException e) {
      Log.d("HelpShiftDebug", "Could not parse the server date");
    }

    return timeDelta;
  }

  public static String getAdjustedTimestamp(Float timeDelta)
  {
    String deviceTs = HSFormat.tsSecFormatter.format(System.currentTimeMillis() / 1000.0D);
    if (timeDelta.floatValue() != 0.0F) {
      Double deviceTsFloat = Double.valueOf(System.currentTimeMillis() / 1000.0D);
      deviceTs = HSFormat.tsSecFormatter.format(deviceTsFloat.doubleValue() + timeDelta.floatValue());
    }
    return deviceTs;
  }

  public static long getAdjustedTimeInMillis(Float timeDelta) {
    long deviceTs = System.currentTimeMillis();
    if (timeDelta.floatValue() != 0.0F) {
      deviceTs = (long)((float)deviceTs + timeDelta.floatValue() * 1000.0F);
    }
    return deviceTs;
  }
}