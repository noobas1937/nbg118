package com.elex.chatservice.net;

import com.elex.chatservice.view.ChatFragment;

public class WebSocketStatusHandler implements IWebSocketStatusListener
{
	private static WebSocketStatusHandler	instance;

	public static WebSocketStatusHandler getInstance()
	{
		if (instance == null)
		{
			instance = new WebSocketStatusHandler();
		}
		return instance;
	}

	@Override
	public void onConsoleOutput(String message)
	{
	}

	@Override
	public void onStatus(String message)
	{
		ChatFragment.setConnectionStatus(message);
	}

	@Override
	public void onStremInput()
	{
	}
}
