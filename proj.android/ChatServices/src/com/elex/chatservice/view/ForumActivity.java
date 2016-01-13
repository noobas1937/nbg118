package com.elex.chatservice.view;

import android.os.Bundle;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class ForumActivity extends MyActionBarActivity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		Bundle extras = getIntent().getExtras();
		if (extras != null)
		{
			this.bundle = new Bundle(extras);
			if (extras.getString("forumUrl") != null)
			{
				ForumFragment.targetUrl = extras.getString("forumUrl");
			}

			if (extras.getInt("webViewType") != -1)
			{
				ForumFragment.webViewType = extras.getInt("webViewType", ForumFragment.WEBVIEW_TYPE_FORFUM);
			}
		}
		fragmentClass = ForumFragment.class;

		ChatServiceController.toggleFullScreen(true, true, this);

		super.onCreate(savedInstanceState);
	}
}
