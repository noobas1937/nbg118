package com.elex.chatservice.model.db;

import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.UserManager;

public class GetNewMsgParam
{
	public int		dbMaxSeqId;
	public String	chatTableName;

	public int		channelType;
	public String	channelId;

	public String getChannelId()
	{
		return "";
	}

	public String getChannelType()
	{
		if (chatTableName == ChannelManager.getInstance().getCountryChannel().getChatTable().getTableNameAndCreate())
		{
			channelId = ChannelManager.getInstance().getCountryChannel().getChatTable().channelID;
			channelType = ChannelManager.getInstance().getCountryChannel().channelType;
		}
		else if (UserManager.getInstance().isCurrentUserInAlliance()
				&& chatTableName == ChannelManager.getInstance().getAllianceChannel().getChatTable().getTableNameAndCreate())
		{
			channelId = ChannelManager.getInstance().getAllianceChannel().getChatTable().channelID;
			channelType = ChannelManager.getInstance().getAllianceChannel().channelType;
		}

		return "";
	}
}
