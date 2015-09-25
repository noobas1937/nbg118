package com.elex.chatservice.util;

import android.app.ProgressDialog;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import java.util.HashMap;

public final class HSErrors
{
  private static final String TAG = "HelpShiftDebug";
  private static HashMap<Integer, Long> errors = new HashMap();
  private static Long previousTimestamp;
  private static Long cooldown;
  private static HashMap<Integer, Long> cooldowns = new HashMap();

  public static void showFailToast(int status, ProgressDialog progressDialog, Context c)
  {
    if (progressDialog != null) {
      progressDialog.dismiss();
    }
    Log.d("HelpShiftDebug", status + "");
    CharSequence text;
    if (status == 0) {
      text = HSRes.getString(c, "hs__network_unavailable_msg");
    }
    else
    {
      if (status == 404) {
        text = HSRes.getString(c, "hs__data_not_found_msg");
      }
      else
      {
        if (status == 2)
          text = HSRes.getString(c, "hs__screenshot_upload_error_msg");
        else
          text = HSRes.getString(c, "hs__network_error_msg");
      }
    }
    previousTimestamp = (Long)errors.get(Integer.valueOf(status));
    if (cooldowns.containsKey(Integer.valueOf(status)))
      cooldown = (Long)cooldowns.get(Integer.valueOf(status));
    else {
      cooldown = new Long(1000L);
    }

    if (status != -1) {
      if (previousTimestamp == null)
        showFailToast(c, text);
      else if (System.currentTimeMillis() - previousTimestamp.longValue() > cooldown.longValue()) {
        showFailToast(c, text);
      }
    }

    errors.put(Integer.valueOf(status), Long.valueOf(System.currentTimeMillis()));
  }

  private static void showFailToast(Context c, CharSequence text) {
    int duration = 0;
    Toast toast = Toast.makeText(c, text, duration);
    toast.setGravity(16, 0, 0);
    toast.show();
  }

  static
  {
    cooldowns.put(Integer.valueOf(0), new Long(90000L));
    cooldowns.put(Integer.valueOf(404), new Long(1000L));
    cooldowns.put(Integer.valueOf(1), new Long(5000L));
    cooldowns.put(Integer.valueOf(2), new Long(6000L));
  }
}