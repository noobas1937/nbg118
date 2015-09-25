package com.elex.chatservice.view;

import android.content.Intent;
import android.os.Bundle;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class WriteMailActivity extends MyActionBarActivity
{
	public String	roomName;
	public String	memberUids;
	public String	memberNames;

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		fragmentClass = WriteMailFragment.class;

		ChatServiceController.toggleFullScreen(true, true, this);
		
		super.onCreate(savedInstanceState);
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
		if (resultCode == RESULT_OK)
		{
			roomName = data.getStringExtra("roomName");
			memberUids = data.getStringExtra("uidStr");
			memberNames = data.getStringExtra("nameStr");
		}
	}
}