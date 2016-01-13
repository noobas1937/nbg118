package com.elex.chatservice.model;

public class LatestCountryAllianceChatInfo
{
	private LatestChatInfo latestCountryChatInfo = null;
	private LatestChatInfo latestAllianceChatInfo = null;
	
	public LatestChatInfo getLatestCountryChatInfo()
	{
		return latestCountryChatInfo;
	}
	public void setLatestCountryChatInfo(LatestChatInfo latestCountryChatInfo)
	{
		this.latestCountryChatInfo = latestCountryChatInfo;
	}
	public LatestChatInfo getLatestAllianceChatInfo()
	{
		return latestAllianceChatInfo;
	}
	public void setLatestAllianceChatInfo(LatestChatInfo latestAllianceChatInfo)
	{
		this.latestAllianceChatInfo = latestAllianceChatInfo;
	}
	
	
}
