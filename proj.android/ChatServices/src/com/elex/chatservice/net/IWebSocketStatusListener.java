package com.elex.chatservice.net;

public interface IWebSocketStatusListener
{
	public void onConsoleOutput(String message);
	public void onStatus(String message);
	public void onStremInput();
}
