package com.elex.chatservice.view.autoscroll;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.TextView;

public class AutoScroll extends TextView {
	 private float textLength = 0f;
	 private float viewWidth = 0f;
	 private float step = 0f;
	 private float y = 0f;
	 private float temp_view_plus_text_length = 0.0f;
	 private float temp_view_plus_two_text_length = 0.0f;
	 public boolean isStarting = false;
	 private Paint paint = null;
	 private String text = "";
	 Canvas canvas;
	 
	 private Handler handler = new Handler()
	 {
	    	@SuppressLint("WrongCall")
			@Override
			public void handleMessage(Message msg) 
	    	{
	    		onDraw(canvas);
			}
	    	
	};
	 
	public AutoScroll(Context context)
	{
		super(context);
	}
	public AutoScroll(Context context, AttributeSet attrs) 
	{
		super(context, attrs);
	}
	public AutoScroll(Context context, AttributeSet attrs, int defStyle) 
	{
		super(context, attrs, defStyle);
	}
	
	 public void init(WindowManager windowManager)
	 {
	        paint = getPaint();
	        text = getText().toString();
	        textLength = paint.measureText(text);
	        viewWidth = getWidth();
	        if(viewWidth == 0)
	        {
	            if(windowManager != null)
	            {
	                Display display = windowManager.getDefaultDisplay();
	                viewWidth = display.getWidth();
	            }
	        }
	        step = textLength;
	        temp_view_plus_text_length = viewWidth + textLength;
	        temp_view_plus_two_text_length = viewWidth + textLength * 2;
	        y = getTextSize() + getPaddingTop();
	}
	 
	    public void startScroll()
	    {
	        isStarting = true;
	        invalidate();
	    }
	   
	   
	    public void stopScroll()
	    {
	        isStarting = false;
	        invalidate();
	    }
	    
	    public void onDraw(Canvas canvas) {
	    	
	        if(!isStarting)
	        {
	            return;
	        }
	        this.canvas = canvas;
	        canvas.drawText(text, temp_view_plus_text_length - step, y, paint);
	        step += 2;
	        if(step > temp_view_plus_two_text_length)
	            step = textLength;
			invalidate();
	    }

}
