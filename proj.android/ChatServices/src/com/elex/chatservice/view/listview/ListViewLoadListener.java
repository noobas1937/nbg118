package com.elex.chatservice.view.listview;

public abstract interface ListViewLoadListener
{
	public abstract boolean getIsListViewToTop();

	public abstract boolean getIsListViewToBottom();

	public abstract void refreshData();
}
