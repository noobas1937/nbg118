package com.elex.chatservice.view;

import org.apache.commons.lang.StringUtils;

import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;

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

	public void refreshScrollLoadEnabled()
	{
		channelListPullView.setPullLoadEnabled(false);
		channelListPullView.setPullRefreshEnabled(false);

		if (adapter != null && adapter.hasMoreData())
		{
			System.out.println("MsgMailListFragment.refreshScrollLoadEnabled() setScrollLoadEnabled true");
			channelListPullView.setScrollLoadEnabled(true);
		}
		else
		{
			System.out.println("MsgMailListFragment.refreshScrollLoadEnabled() setScrollLoadEnabled false");
			channelListPullView.setScrollLoadEnabled(false);
		}
	}
}
