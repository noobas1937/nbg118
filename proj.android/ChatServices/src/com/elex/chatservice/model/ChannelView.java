package com.elex.chatservice.model;

import java.util.ArrayList;

import android.widget.ListView;

import com.elex.chatservice.view.MessagesAdapter;
import com.elex.chatservice.view.listview.PullDownToLoadMoreView;

public class ChannelView
{
	private boolean					isLoadingStart;
	public ListView					messagesListView;
	private MessagesAdapter			messagesAdapter;
	public PullDownToLoadMoreView	pullDownToLoadListView;
	public boolean					isFirstVisit;
	public int						tab;
	public int						channelType;
	public ChatChannel				chatChannel;

	public ChannelView()
	{
		init();
	}

	public boolean getLoadingStart()
	{
		return isLoadingStart;
	}

	public void setLoadingStart(boolean b)
	{
		isLoadingStart = b;
	}

	public void init()
	{
		if (messagesAdapter != null)
			messagesAdapter.destroy();
		if (messagesListView != null)
		{
			messagesListView.setOnScrollListener(null);
			messagesListView.setAdapter(null);
		}
		if (pullDownToLoadListView != null)
		{
			pullDownToLoadListView.setListViewLoadListener(null);
			pullDownToLoadListView.removeAllViews();
		}
		isLoadingStart = false;
		isFirstVisit = true;
		messagesListView = null;
		messagesAdapter = null;
		pullDownToLoadListView = null;
		chatChannel = null;
	}

	public void setMessagesAdapter(MessagesAdapter adapter)
	{
		messagesAdapter = adapter;
	}

	public MessagesAdapter getMessagesAdapter()
	{
		return messagesAdapter;
	}
}
