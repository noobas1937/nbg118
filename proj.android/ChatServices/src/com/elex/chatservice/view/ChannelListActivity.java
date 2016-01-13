package com.elex.chatservice.view;

import android.os.Bundle;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class ChannelListActivity extends MyActionBarActivity
{
	public int	channelType;

	public ChannelListFragment getFragment()
	{
		return (ChannelListFragment) fragment;
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		Bundle extras = getIntent().getExtras();

		boolean isSecondLvList = false;
		String channelId = "";
		if (extras != null)
		{
			this.bundle = new Bundle(extras);
			if (extras.containsKey("channelType"))
			{
				channelType = extras.getInt("channelType");
				ChatServiceController.setCurrentChannelType(channelType);
			}

			if (extras.containsKey("isSecondLvList"))
			{
				isSecondLvList = extras.getBoolean("isSecondLvList");
			}

			if (extras.containsKey("channelId"))
			{
				channelId = extras.getString("channelId");
			}
		}

		if (!isSecondLvList)
		{
			if (channelId.equals(MailManager.CHANNELID_MOD) || channelId.equals(MailManager.CHANNELID_MESSAGE))
			{
				fragmentClass = MsgMailListFragment.class;
			}
			else
			{
				fragmentClass = MainListFragment.class;
			}
		}
		else
		{
			fragmentClass = SysMailListFragment.class;
		}

		ChatServiceController.toggleFullScreen(true, true, this);

		super.onCreate(savedInstanceState);
	}
	
	protected void showBackground()
	{
		ImageUtil.setYRepeatingBG(this, fragmentLayout, R.drawable.mail_list_bg);
	}

	@Override
	public void onResume()
	{
		super.onResume();

		ChatServiceController.setCurrentChannelType(channelType);
	}

	@Override
	public void onDestroy()
	{
		if (ChatServiceController.isReturningToGame && !ChannelListFragment.preventSecondChannelId)
		{
			ChannelListFragment.rememberSecondChannelId = true;
		}
		else
		{
			ChannelListFragment.rememberSecondChannelId = false;
		}
		super.onDestroy();
	}

	public void onBackButtonClick()
	{
		if (fragment != null && fragment instanceof ChannelListFragment && ((ChannelListFragment) fragment).handleBackPressed())
		{
			return;
		}
		super.onBackButtonClick();
	}

	@Override
	public void onBackPressed()
	{
		if (fragment != null && fragment instanceof ChannelListFragment && ((ChannelListFragment) fragment).handleBackPressed())
		{
			return;
		}
		super.onBackPressed();
	}

	public void onWindowFocusChanged(boolean hasFocus)
	{
		super.onWindowFocusChanged(hasFocus);

		if (hasFocus)
		{
			// 从这里调的话，其它没问题，但退出系统邮件后，两层activity的打开动画都会看到，不如onResume看起来只打开了一层
			// getFragment().onBecomeVisible();
		}
		else
		{

		}
	}

	public void hideProgressBar()
	{
		// 首次进入列表，加载系统邮件时，防止关掉进度圈
		if (!(getFragment() != null && getFragment().adapter != null && getFragment().adapter.isLoadingMore))
		{
			super.hideProgressBar();
		}
	}
}
