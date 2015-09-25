package com.elex.chatservice.customviews;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.Button;

import com.elex.chatservice.util.HSCustomFont;
import com.elex.chatservice.util.HSRes;

final class HSButton extends Button
{
  public HSButton(Context context)
  {
    super(context);
  }

  public HSButton(Context context, AttributeSet attrs) {
    super(context, attrs);
    HSCustomFont.setCustomFont(this, context, attrs, HSRes.getIdArray(context, "styleable", "HSButton"), HSRes.getId(context, "styleable", "HSButton_font"));
  }

  public HSButton(Context context, AttributeSet attrs, int defStyle)
  {
    super(context, attrs, defStyle);
    HSCustomFont.setCustomFont(this, context, attrs, HSRes.getIdArray(context, "styleable", "HSButton"), HSRes.getId(context, "styleable", "HSButton_font"));
  }
}