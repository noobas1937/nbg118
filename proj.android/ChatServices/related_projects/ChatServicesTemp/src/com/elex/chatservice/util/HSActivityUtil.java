package com.elex.chatservice.util;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

import com.elex.chatservice.HSSearch;

public final class HSActivityUtil
{
  private static final String TAG = "HelpShiftDebug";

  public static Boolean isFullScreen(Activity a)
  {
    Window win = a.getWindow();
    WindowManager.LayoutParams parameters = win.getAttributes();
    return Boolean.valueOf((parameters.flags & 0x400) == 1024);
  }

  public static void forceNotFullscreen(Activity a)
  {
    Boolean isFullScreen = Boolean.valueOf(a.getIntent().getExtras().getBoolean("showInFullScreen"));
    if (isFullScreen.booleanValue()) {
      a.getWindow().clearFlags(1024);
      a.getWindow().addFlags(2048);
    }
  }

  public static void restoreFullscreen(Activity a) {
    Boolean isFullScreen = Boolean.valueOf(a.getIntent().getExtras().getBoolean("showInFullScreen"));
    if (isFullScreen.booleanValue()) {
      a.getWindow().clearFlags(2048);
      a.getWindow().addFlags(1024);
    }
  }

  public static void sessionEnding() {
    Log.d("HelpShiftDebug", "Helpshift session ended");
    HSSearch.deinit();
  }

  public static void sessionBeginning() {
    Log.d("HelpShiftDebug", "Helpshift session beginning");
    HSSearch.init();
  }
}