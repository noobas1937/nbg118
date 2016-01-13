package com.elex.chatservice.util;

public interface IAnalyticTracker
{
	public void trackException(String exceptionType, String funcInfo, String cause, String message);

	public void transportMail(String jsonStr, boolean isShowDetectMail);
	public String getPublishChannelName();

	public void trackMessage(String messageType, String... args);
}
