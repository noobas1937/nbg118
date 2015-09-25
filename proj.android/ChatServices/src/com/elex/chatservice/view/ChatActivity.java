package com.elex.chatservice.view;

import android.os.Bundle;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.host.DummyHost;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public final class ChatActivity extends MyActionBarActivity
{
	public int channelType;
	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		Bundle extras = getIntent().getExtras();
		if(extras != null){
			this.bundle = new Bundle(extras);
			if (extras.getInt("channelType") >= 0)
			{
				channelType = extras.getInt("channelType");
				ChatServiceController.setCurrentChannelType(channelType);
			}
		}
		
		fragmentClass = ChatFragment.class;
		
		ChatServiceController.toggleFullScreen(false, true, this);
		
		super.onCreate(savedInstanceState);
	}

	@Override
	public void onResume()
	{
		super.onResume();
		
		ChatServiceController.setCurrentChannelType(channelType);
	}
	
	@Override
	public void exitActivity()
	{
		if(ChatServiceController.getInstance().isInDummyHost() && ((DummyHost)(ChatServiceController.getInstance().host)).actionAfterResume != null)
		{
			((DummyHost)(ChatServiceController.getInstance().host)).actionAfterResume = null;
			return;
		}
		
		super.exitActivity();
	}
	
	public void onWindowFocusChanged(boolean hasFocus)
	{
		super.onWindowFocusChanged(hasFocus);
		
		if(hasFocus){
			// 这里调onBecomeVisible()与adjustHeight中调差不多
//			showProgressBar();
//			((ChatFragment) fragment).onBecomeVisible();
		}else{
			
		}
	}
}