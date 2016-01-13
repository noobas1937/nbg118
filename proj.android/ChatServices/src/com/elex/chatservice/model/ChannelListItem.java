package com.elex.chatservice.model;

public class ChannelListItem
{
	public transient boolean	checked		= false;
	public transient int		unreadCount	= 0;

	public boolean isUnread()
	{
		return false;
	}

	public long getChannelTime()
	{
		return 0;
	}

	public boolean hasReward()
	{
		return false;
	}

	public boolean isLock()
	{
		return false;
	}
}
