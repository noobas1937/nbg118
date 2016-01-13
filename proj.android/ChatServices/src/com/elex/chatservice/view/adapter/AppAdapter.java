package com.elex.chatservice.view.adapter;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import android.content.pm.ApplicationInfo;
import android.view.View;
import android.view.ViewGroup;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ApplicationItem;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class AppAdapter extends AbstractMailListAdapter
{
	public ArrayList<ChannelListItem>	allAppInfo	= new ArrayList<ChannelListItem>();
	private ChatChannel					parentChannel;

	public synchronized void loadMoreData()
	{
		int count = allAppInfo.size() > 9 ? 9 : allAppInfo.size();
		for (int i = 0; i < count; i++)
		{
			list.add(allAppInfo.remove(0));
		}
		fragment.refreshScrollLoadEnabled();

		fragment.onLoadMoreComplete();
	}

	public AppAdapter(ChannelListActivity context, ChannelListFragment fragment, ChatChannel parentChannel)
	{
		super(context, fragment);

		this.parentChannel = parentChannel;
		reloadData();
	}

	public void reloadData()
	{
		list.clear();
		List<ApplicationInfo> infos = context.getPackageManager().getInstalledApplications(0);
		for (Iterator<ApplicationInfo> iterator = infos.iterator(); iterator.hasNext();)
		{
			ApplicationInfo applicationInfo = (ApplicationInfo) iterator.next();
			ApplicationItem item = new ApplicationItem(applicationInfo);
			allAppInfo.add(item);
			if (allAppInfo.size() > 35)
				break;
		}
		loadMoreData();
		refreshOrder();
	}

	public boolean hasMoreData()
	{
		return allAppInfo.size() > 0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		convertView = super.getView(position, convertView, parent);

		CategoryViewHolder holder = (CategoryViewHolder) convertView.getTag();
		ApplicationItem item = (ApplicationItem) getItem(position);
		ApplicationInfo info = item.appInfo;
		int bgColor = 0;
		if (ChatServiceController.isNewMailUIEnable)
		{
			bgColor = MailManager.getColorByChannelId(parentChannel.channelID);
		}
		holder.setContent(context, item, item.showUreadAsText, info.loadIcon(context.getPackageManager()),
				(String) info.loadLabel(context.getPackageManager()), item.summary, TimeManager.getReadableTime(item.time),
				fragment.isInEditMode(), position, bgColor);
		refreshMenu();

		return convertView;
	}

	public void destroy()
	{
		allAppInfo.clear();
		allAppInfo = null;
		super.destroy();
	}
}
