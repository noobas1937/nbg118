package com.elex.chatservice.view.adapter;

import java.util.ArrayList;
import java.util.List;

import android.view.View;
import android.view.ViewGroup;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class MsgChannelAdapter extends ChannelAdapter
{
	protected String	mChannelId	= "";
	private boolean		initLoaded	= false;

	public MsgChannelAdapter(ChannelListActivity context, ChannelListFragment fragment, String channelId)
	{
		super(context, fragment);
		mChannelId = channelId;
		if (channelId.equals(MailManager.CHANNELID_MOD))
		{
			System.out.println("MsgChannelAdapter ChatServiceController.isContactMod = true");
			ChatServiceController.isContactMod = true;
		}
		reloadData();
	}

	public boolean hasMoreData()
	{
		if (ServiceInterface.isHandlingGetNewMailMsg)
		{
			return false;
		}
		else
		{
			int count = getAllMsgChannels().size();

			return count > list.size();
		}
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		ChatChannel channel = (ChatChannel) getItem(position);
		if (channel != null)
			channel.refreshRenderData();
		return super.getView(position, convertView, parent);
	}

	public synchronized void loadMoreData()
	{
		System.out.println("MsgChannelAdapter loadMoreData()");

		initLoaded = true;
		List<ChatChannel> allMsgChannels = getAllMsgChannels();
		List<ChatChannel> loadedMsgChannels = getLoadedMsgChannels();
		System.out.println("MsgChannelAdapter allMsgChannels size:" + allMsgChannels.size());

		int moreCount = loadedMsgChannels.size() + 10;
		int actualCount = allMsgChannels.size() > moreCount ? moreCount : allMsgChannels.size();

		List<ChatChannel> subMsgChannels = allMsgChannels.subList(0, actualCount);
		System.out.println("MsgChannelAdapter moreCount:" + moreCount + " actualCount:" + actualCount + " subMsgChannels.size():"
				+ subMsgChannels.size());
		int addCnt = 0;

		List<ChatChannel> tempList = new ArrayList<ChatChannel>();
		for (int i = 0; i < subMsgChannels.size(); i++)
		{
			ChatChannel chatChannel = subMsgChannels.get(i);
			if (chatChannel != null && !ChannelManager.isChannelInList(chatChannel, loadedMsgChannels))
			{
				addCnt++;
				tempList.add(chatChannel);
			}
		}

		for (int i = 0; i < tempList.size(); i++)
		{
			ChatChannel chatChannel = tempList.get(i);
			if (chatChannel != null && !chatChannel.hasInitLoaded())
			{
				chatChannel.loadMoreMsg();
			}
			loadedMsgChannels.add(chatChannel);
		}

		tempList.clear();

		for (int i = 0; i < loadedMsgChannels.size(); i++)
		{
			ChatChannel chatChannel = loadedMsgChannels.get(i);
			if (chatChannel != null && !ChannelManager.isChannelInList(chatChannel, list))
			{
				tempList.add(chatChannel);
			}
		}

		for (int i = 0; i < tempList.size(); i++)
		{
			ChatChannel chatChannel = tempList.get(i);
			if (chatChannel != null)
			{
				list.add(chatChannel);
			}
		}

		System.out.println("MsgChannelAdapter addCnt:" + addCnt);

		refreshOrder();

		fragment.onLoadMoreComplete();
	}

	public void reloadData()
	{
		List<ChatChannel> loadedMsgChannels = getLoadedMsgChannels();
		System.out.println("MsgChannelAdapter reloadData() : " + loadedMsgChannels.size());

		list.clear();

		// 第一次进入列表时，可能loadedMsgChannels已经有内容（刚收到的消息），此时需要loadMoreData一次，否则会只显示刚收到的channel
		if (loadedMsgChannels.size() == 0 || !initLoaded)
		{
			System.out.println("初次加载");
			loadMoreData();
		}
		else
		{
			System.out.println("重新加载");
			list.addAll(loadedMsgChannels);
		}

		refreshOrder();

		fragment.setNoMailTipVisible(list.size() <= 0);
	}
	
	public void refreshAdapterList()
	{
		list.clear();
		List<ChatChannel> loadedMsgChannels = getLoadedMsgChannels();
		list.addAll(loadedMsgChannels);
		refreshOrder();
		fragment.setNoMailTipVisible(list.size() <= 0);
	}

	private List<ChatChannel> getAllMsgChannels()
	{
		if (mChannelId.equals(MailManager.CHANNELID_MOD))
		{
			return ChannelManager.getInstance().getAllModChannel();
		}
		else if (mChannelId.equals(MailManager.CHANNELID_MESSAGE))
		{
			return ChannelManager.getInstance().getAllMessageChannel();
		}
		return new ArrayList<ChatChannel>();
	}

	private List<ChatChannel> getLoadedMsgChannels()
	{
		if (mChannelId.equals(MailManager.CHANNELID_MOD))
		{
			return ChannelManager.getInstance().getLoadedModChannel();
		}
		else if (mChannelId.equals(MailManager.CHANNELID_MESSAGE))
		{
			return ChannelManager.getInstance().getLoadedMessageChannel();
		}
		return new ArrayList<ChatChannel>();
	}

	@Override
	public void destroy()
	{
		System.out.println("MsgChannelAdapter destroy");
		super.destroy();
		ChatServiceController.isContactMod = false;
	}
}
