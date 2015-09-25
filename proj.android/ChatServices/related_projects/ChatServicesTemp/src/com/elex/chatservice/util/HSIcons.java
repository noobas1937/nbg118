package com.elex.chatservice.util;

import android.content.Context;
import android.content.res.Resources.Theme;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.PorterDuff.Mode;
import android.graphics.drawable.Drawable;

import com.elex.chatservice.app.ActionBarActivity;
import com.elex.chatservice.app.ActionBarHelper;

import android.graphics.PorterDuff;

public final class HSIcons
{
  private static final String TAG = "HelpShiftDebug";

  public static void applyMenuItemSelectedFilter(Context activity, Drawable icon)
  {
    if ((icon != null) && ((icon instanceof Drawable))) {
      TypedArray array = ((ActionBarActivity)activity).getActionBarHelper().getThemedContext().getTheme().obtainStyledAttributes(new int[] { 16842806 });

      int iconColor = array.getColor(0, 16777215);
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyTextColorPrimaryFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      TypedArray array = activity.getTheme().obtainStyledAttributes(new int[] { 16842806 });

      int iconColor = array.getColor(0, 12632256);
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyConfirmAcceptFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int iconColor = Color.parseColor("#669900");
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyConfirmRejectFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int iconColor = Color.parseColor("#FF4444");
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyAttachFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int iconColor = HSRes.getColor(activity, "hs__send_reply_icon_color");
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyReviewFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int iconColor = Color.parseColor("#FFBB33");
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyNotificationFilter(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int iconColor = HSRes.getColor(activity, "hs__notificationIconColor");
      applyColorFilter(activity, icon, iconColor);
    }
  }

  public static void applyColorFilter(Context activity, Drawable icon, int iconColor) {
    icon.setColorFilter(iconColor, PorterDuff.Mode.SRC_ATOP);
  }

  public static void highlightIcon(Context activity, Drawable icon) {
    if ((icon != null) && ((icon instanceof Drawable))) {
      int highlightColor = HSRes.getColor(activity, "hs__actionBarTabIndicatorColor");
      icon.setColorFilter(highlightColor, PorterDuff.Mode.SRC_ATOP);
    }
  }
}