package com.elex.chatservice.view;

import java.util.List;

import org.apache.commons.lang.StringUtils;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.view.adapter.AppAdapter;
import com.elex.chatservice.view.adapter.MainChannelAdapter;
import com.elex.chatservice.view.adapter.MsgChannelAdapter;

public class MainListFragment extends ChannelListFragment
{	
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = (ChannelListActivity) getActivity();
		channelListActivity = (ChannelListActivity) activity;
		
		Bundle extras = this.activity.getIntent().getExtras();
		if(extras != null){
			if(extras.containsKey("channelId"))
				channelId=extras.getString("channelId");
		}
		else
		{
			channelId = "";
		}

		return super.onCreateView(inflater, container, savedInstanceState);
	}

	// 不override的话，父类的onViewCreated会调两次
	public void onViewCreated(final View view, Bundle savedInstanceState)
    {
		super.onViewCreated(view, savedInstanceState);
		
		showWriteButton();
    }
	
	protected void showWriteButton()
	{
		if(ChatServiceController.isNewMailUIEnable){
			getEditButton().setVisibility(View.GONE);
			getReturnButton().setVisibility(View.GONE);
			
			getWriteButton().setVisibility(View.VISIBLE);
			getWriteButton().setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View view)
				{
					writeNewMail();
				}
			});
		}else{
			getWriteButton().setVisibility(View.GONE);
		}
	}

	protected void jumpToSecondaryList()
	{
		if(rememberSecondChannelId && StringUtils.isNotEmpty(lastSecondChannelId)){
			ServiceInterface.showChannelListActivity(channelListActivity, true, DBDefinition.CHANNEL_TYPE_OFFICIAL, lastSecondChannelId, true);
			rememberSecondChannelId = false;
			lastSecondChannelId = "";
			return;
		}
	}
	
	protected void refreshChannel()
	{
		System.out.println("refreshChannel");
		notifyDataSetChanged();
		dataChanged = false;
	}

	protected void createList()
	{
		if(StringUtils.isNotEmpty(channelId) && (channelId.equals(MailManager.CHANNELID_MOD) || channelId.equals(MailManager.CHANNELID_MESSAGE))){
			adapter = new MsgChannelAdapter(channelListActivity, this,channelId);
		}else{
			if(ChatServiceController.getInstance().isInDummyHost() && !ChatServiceController.isNewMailUIEnable){
				ChatChannel parentChannel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
				adapter = new AppAdapter(channelListActivity, this, parentChannel);
			}else{
				adapter = new MainChannelAdapter(channelListActivity, this);
			}
		}
		super.createList();
	}

	protected void restorePosition()
	{
		int lastX = lastScrollX;
        int lastY = lastScrollY;
		if (lastX != -1)
		{
			mListView.setSelectionFromTop(lastX, lastY);
		}
		lastScrollX = lastScrollY = -1;
	}
	
	protected void onDeleteMenuClick(int position)
	{
		if(ChatServiceController.getInstance().isInDummyHost())
		{
			deleteDummyItem(position);
		}else{
			deleteChannel(position);
		}
	}

	protected void openItem(ChannelListItem item)
	{
		if(item instanceof ChatChannel){
			openChannel((ChatChannel) item);
		}
	}
	
	public void saveState()
	{
		if(inited && getCurrentPos() != null)
		{
			lastScrollX = getCurrentPos().x;
			lastScrollY = getCurrentPos().y;
		}
	}

	public void comfirmDeleteMutiMail(List<ChannelListItem> checkedItems)
	{
		actualDeleteChannels(checkedItems);
	}

    public void onDestroy() {
    	if(getWriteButton() != null){
    		getWriteButton().setOnClickListener(null);
    	}
		super.onDestroy();
    }
}
