package com.elex.chatservice.customviews;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.TextView;

import com.elex.chatservice.util.HSCustomFont;
import com.elex.chatservice.util.HSRes;

final class HSTextView extends TextView
{
  public HSTextView(Context context)
  {
    super(context);
  }

  public HSTextView(Context context, AttributeSet attrs) {
    super(context, attrs);
    HSCustomFont.setCustomFont(this, context, attrs, HSRes.getIdArray(context, "styleable", "HSTextView"), HSRes.getId(context, "styleable", "HSTextView_font"));
  }

  public HSTextView(Context context, AttributeSet attrs, int defStyle)
  {
    super(context, attrs, defStyle);
    HSCustomFont.setCustomFont(this, context, attrs, HSRes.getIdArray(context, "styleable", "HSTextView"), HSRes.getId(context, "styleable", "HSTextView_font"));
  }
}