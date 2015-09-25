package com.elex.chatservice.view;

import org.apache.commons.lang.StringUtils;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.view.MainListFragment;
import com.elex.chatservice.view.adapter.AppAdapter;
import com.elex.chatservice.view.adapter.MainChannelAdapter;
import com.elex.chatservice.view.adapter.MsgChannelAdapter;

public class MsgMailListFragment extends MainListFragment
{
	protected void setTitleLabel()
	{
		if (StringUtils.isNotEmpty(channelId))
		{
			ChatChannel channel = null;
			if (channelId.equals(MailManager.CHANNELID_MOD))
			{
				channel = ChannelManager.getInstance().getModChannel();
			}
			else if (channelId.equals(MailManager.CHANNELID_MESSAGE))
			{
				channel = ChannelManager.getInstance().getMessageChannel();
			}
			if (channel != null)
			{
				this.getTitleLabel().setText(channel.nameText);
			}
		}
	}
	
	@Override
	public void onStop()
	{
		super.onStop();
		dataChanged = true;
	}
	
	@Override
	public void onResume()
	{
		super.onResume();
		ChannelManager.getInstance().isInRootChannelList = true;
	}

	@Override
	protected void showWriteButton()
	{
		
	}
}
