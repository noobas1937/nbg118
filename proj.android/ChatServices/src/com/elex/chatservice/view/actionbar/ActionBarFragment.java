package com.elex.chatservice.view.actionbar;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.graphics.Rect;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;

public class ActionBarFragment extends Fragment
{
	protected MyActionBarActivity activity;
    protected RelativeLayout fragmentLayout;

	public void onViewCreated(View view, Bundle savedInstanceState)
    {
		super.onViewCreated(view, savedInstanceState);
		
		fragmentLayout = ((RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "fragmentLayout")));
		
		getMemberSelectButton().setVisibility(View.GONE);
		getEditButton().setVisibility(View.GONE);
		getReturnButton().setVisibility(View.GONE);
		getWriteButton().setVisibility(View.GONE);
    }
	
	protected TextView getTitleLabel()
	{
		return activity.titleLabel;
	}
	protected Button getMemberSelectButton()
	{
		return activity.optionButton;
	}
	protected Button getEditButton()
	{
		return activity.editButton;
	}
	protected Button getWriteButton()
	{
		if(activity == null || activity.writeButton == null){
			return null;
		}
		return activity.writeButton;
	}
	protected Button getReturnButton()
	{
		return activity.returnButton;
	}

	protected int computeUsableHeight() {
        Rect r = new Rect();
        //In effect, this tells you the available area where content can be placed and remain visible to users.
        fragmentLayout.getWindowVisibleDisplayFrame(r);
        return (r.bottom - r.top);
    }

    protected int usableHeight = -1;
	protected void hideSoftKeyBoard()
	{
		InputMethodManager inputManager = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);  
		inputManager.hideSoftInputFromWindow(fragmentLayout.getWindowToken(), 0); 
	}
	
	public void saveState()
	{
		
	}
	
	protected Timer timer;
	protected boolean inited = false;

	protected void stopTimer() {
		if (timer != null){
			timer.cancel();
			timer.purge();
			timer = null;
		}
	}

	protected void onBecomeVisible() {
		
	}

	// 200ms是动画的时间
	protected int timerDelay = 200;
	protected void startTimer()
	{
		timer = new Timer();

		TimerTask timerTask = new TimerTask() {
			@Override
			public void run() {
				if(activity == null) {
					stopTimer();
					return;
				}
				activity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						stopTimer();
						initData();
					}
				});
			}
		};
		timer.schedule(timerTask, timerDelay, 1000000);
	}
	
	protected void initData()
	{
		if (inited) return;

		if (activity == null) return;
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				// 可能在加载过程中关掉activity，导致出异常
				try
				{
					createList();

					renderList();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});

		inited = true;
	}
	
	protected void createList()
	{
		
	}
	
	protected void renderList()
	{
		
	}
	
    public void onDestroy() {
    	System.out.println("ActionBarFragment.onDestroy()");
    	activity = null;
    	fragmentLayout = null;
    	super.onDestroy();
    }
}
