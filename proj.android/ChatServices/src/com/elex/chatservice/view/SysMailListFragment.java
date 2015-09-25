package com.elex.chatservice.view;

import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.view.adapter.AppAdapter;
import com.elex.chatservice.view.adapter.SysMailAdapter;

public class SysMailListFragment extends ChannelListFragment
{
	protected void setTitleLabel()
	{
		if(StringUtils.isNotEmpty(channelId)){
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
			if(channel != null)
				this.getTitleLabel().setText(channel.nameText);
		}
	}
	
	public void updateMailDataList(MailData mailData)
	{
		if(mailData!=null && adapter != null && StringUtils.isNotEmpty(channelId) && mailData.channelId.equals(channelId))
		{
			MailData mail = null;
			boolean isInMailList = false;
			for(int i=0 ; i<adapter.list.size() ;i++)
			{
				mail = (MailData)adapter.list.get(i);
				if(mail!=null && mail.getUid().equals(mailData.getUid()))
				{
					isInMailList = true;
					System.out.println("updateMailDataList mail!=null");
					break;
				}
			}
			if(mail!=null && isInMailList)
			{
				adapter.list.remove(mail);
				adapter.list.add(mailData);
				adapter.refreshOrder();
			}
		}
	}
	
	public void refreshMailDataList(MailData mailData)
	{
		if (adapter != null && StringUtils.isNotEmpty(channelId) && mailData != null
				&& StringUtils.isNotEmpty(mailData.getChannelId()) && mailData.getChannelId().equals(channelId))
		{
			adapter.list.add(mailData);
			adapter.refreshOrder();
		}
	}

	public void refreshScrollLoadEnabled()
	{
		channelListPullView.setPullLoadEnabled(false);
		channelListPullView.setPullRefreshEnabled(false);
		
		if(adapter != null && adapter.hasMoreData()){
			System.out.println("refreshScrollLoadEnabled() setScrollLoadEnabled true");
			channelListPullView.setScrollLoadEnabled(true);
		}else{
			System.out.println("refreshScrollLoadEnabled() setScrollLoadEnabled false");
			channelListPullView.setScrollLoadEnabled(false);
		}
	}
	
	protected void createList()
	{
		if(ChatServiceController.getInstance().isInDummyHost()){
			ChatChannel parentChannel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
			adapter = new AppAdapter(channelListActivity, this, parentChannel);
		}else{
			adapter = new SysMailAdapter(channelListActivity, this);
		}
		
		super.createList();
	}
	
	protected void restorePosition()
	{
		int lastX = secondLastScrollX;
        int lastY = secondLastScrollY;
		if (lastX != -1)
		{
			mListView.setSelectionFromTop(lastX, lastY);
		}
		secondLastScrollX = secondLastScrollY = -1;
	}

	protected void onDeleteMenuClick(int position)
	{
		if(ChatServiceController.getInstance().isInDummyHost())
		{
			deleteDummyItem(position);
		}else{
			deleteSysMail(position);
		}
	}

	protected void openItem(ChannelListItem item)
	{
		if(item instanceof MailData){
			openMail((MailData) item);
		}
	}
	
	public void saveState()
	{
		if(rememberSecondChannelId && getCurrentPos() != null)
		{
			secondLastScrollX = getCurrentPos().x;
			secondLastScrollY = getCurrentPos().y;
		}
		
		if(rememberSecondChannelId){
			lastSecondChannelId = channelId;
		}
	}

	public void comfirmDeleteMutiMail(List<ChannelListItem> checkedItems)
	{
		actualDeleteSysMails(checkedItems);
	}
}
