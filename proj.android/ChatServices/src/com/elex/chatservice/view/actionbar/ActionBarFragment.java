package com.elex.chatservice.view.actionbar;

import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

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
	protected MyActionBarActivity	activity;
	protected RelativeLayout		fragmentLayout;
	protected boolean				adjustSizeCompleted	= false;

	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		super.onViewCreated(view, savedInstanceState);

		fragmentLayout = ((RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "fragmentLayout")));

		getMemberSelectButton().setVisibility(View.GONE);
		getEditButton().setVisibility(View.GONE);
		getReturnButton().setVisibility(View.GONE);
		getWriteButton().setVisibility(View.GONE);
		getShowFriendButton().setVisibility(View.GONE);
	}

	protected TextView getTitleLabel()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.titleLabel;
	}

	protected Button getMemberSelectButton()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.optionButton;
	}

	protected Button getEditButton()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.editButton;
	}

	protected Button getWriteButton()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.writeButton;
	}

	protected Button getShowFriendButton()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.showFriend;
	}

	protected Button getReturnButton()
	{
		if (activity == null)
		{
			return null;
		}
		return activity.returnButton;
	}

	protected int computeUsableHeight()
	{
		Rect r = new Rect();
		// In effect, this tells you the available area where content can be
		// placed and remain visible to users.
		fragmentLayout.getWindowVisibleDisplayFrame(r);
		return (r.bottom - r.top);
	}

	protected int	usableHeight	= -1;

	protected void hideSoftKeyBoard()
	{
		InputMethodManager inputManager = (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		inputManager.hideSoftInputFromWindow(fragmentLayout.getWindowToken(), 0);
	}

	public void saveState()
	{
	}

	protected boolean	inited	= false;

	protected void stopTimer()
	{
		if (service != null)
		{
			service.shutdown();
		}
	}

	protected void onBecomeVisible()
	{

	}

	// 200ms是动画的时间
	protected int	timerDelay	= 200;
	private ScheduledExecutorService service;

	protected void startTimer()
	{
		TimerTask timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					if (activity == null)
					{
						stopTimer();
						return;
					}
					// 有时activity为null
					activity.runOnUiThread(new Runnable()
					{
						@Override
						public void run()
						{
							stopTimer();
							initData();
						}
					});
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};
		
		service = Executors.newSingleThreadScheduledExecutor();
		service.schedule(timerTask, timerDelay, TimeUnit.MILLISECONDS);
	}

	protected void initData()
	{
		if (inited)
			return;

		if (activity == null)
			return;

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

	public void onDestroy()
	{
		System.out.println("ActionBarFragment.onDestroy()");
		activity = null;
		fragmentLayout = null;
		super.onDestroy();
	}
}
