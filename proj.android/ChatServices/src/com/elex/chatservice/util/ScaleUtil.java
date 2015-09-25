package com.elex.chatservice.util;

import android.content.Context;
import android.util.TypedValue;
import android.widget.TextView;

public class ScaleUtil
{
	public static int getAdjustTextSize(float size, double textRatio)
	{
		int newTextSize = (int) (size * textRatio);
		return newTextSize;
	}
	
	public static void adjustTextSize(TextView textView, double textRatio)
	{
		float newTextSize = (int) (textView.getTextSize() * textRatio);
		textView.setTextSize(TypedValue.COMPLEX_UNIT_PX, newTextSize);
	}

	public static int dip2px(Context context, float dipValue)
	{
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (dipValue * scale + 0.5f);
	}

	public static int px2dip(Context context, float pxValue)
	{
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxValue / scale + 0.5f);
	}

	public static float dipToPixels(Context context, float dipValue)
	{
		return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dipValue, context.getResources().getDisplayMetrics());
	}
}
