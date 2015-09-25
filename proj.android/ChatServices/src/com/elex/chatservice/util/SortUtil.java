package com.elex.chatservice.util;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;

import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.mail.MailData;

public class SortUtil
{
	private static SortUtil instance;

	public static SortUtil getInstance()
	{
		if (instance == null)
		{
			instance = new SortUtil();
		}
		return instance;
	}
	
	public void refreshListOrder(ArrayList<ChannelListItem> target)
	{
		ArrayList<ChannelListItem> unreadRewardList = new ArrayList<ChannelListItem>();
		ArrayList<ChannelListItem> unreadList = new ArrayList<ChannelListItem>();
		ArrayList<ChannelListItem> readedRewardList = new ArrayList<ChannelListItem>();
		ArrayList<ChannelListItem> readedList = new ArrayList<ChannelListItem>();
		
		for (Iterator<ChannelListItem> iterator = target.iterator(); iterator.hasNext();)
		{
			ChannelListItem item = (ChannelListItem) iterator.next();
			if(item.isUnread()){
				if(item.hasReward()){
					unreadRewardList.add(item);
				}else{
					unreadList.add(item);
				}
			}else{
				if(item.hasReward()){
					readedRewardList.add(item);
				}else{
					readedList.add(item);
				}
			}
		}
		
		Collections.sort(unreadRewardList, new SortByTime());
		Collections.sort(unreadList, new SortByTime());
		Collections.sort(readedRewardList, new SortByTime());
		Collections.sort(readedList, new SortByTime());
		
		target.clear();
		target.addAll(unreadRewardList);
		target.addAll(unreadList);
		target.addAll(readedRewardList);
		target.addAll(readedList);
	}
	
	public void refreshNewMailListOrder(ArrayList<MailData> target)
	{
		Collections.sort(target, new SortByTime());
	}
	
	private class SortByTime implements Comparator<ChannelListItem>
	{
		@Override
		public int compare(ChannelListItem o1, ChannelListItem o2)
		{
			if (o1.getChannelTime() < o2.getChannelTime())
			{
				return 1;
			}
			else if (o1.getChannelTime() == o2.getChannelTime())
			{
				return 0;
			}
			
			return -1;
		}
	}
}
