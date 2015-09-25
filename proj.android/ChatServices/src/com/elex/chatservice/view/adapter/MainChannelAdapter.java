package com.elex.chatservice.view.adapter;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class MainChannelAdapter extends ChannelAdapter
{
	public MainChannelAdapter(ChannelListActivity context, ChannelListFragment fragment) {
		super(context, fragment);
		
		reloadData();
	}
	
	public void reloadData()
	{
		list.clear();
		list.addAll(ChannelManager.getInstance().getAllMailChannel());
		refreshOrder();
		fragment.setNoMailTipVisible(list.size()<=0);
	}

    @Override
    public int getItemViewType(int position) {
    	if(ChatServiceController.isNewMailUIEnable){
    		return VIEW_TYPE_NONE;
    	}else{
    		return VIEW_TYPE_DELETE;
    	}
    }
    
	public static final String[] MAIN_CHANNEL_ORDERS = {MailManager.CHANNELID_MESSAGE,
														MailManager.CHANNELID_ALLIANCE,
														MailManager.CHANNELID_FIGHT,
														MailManager.CHANNELID_NOTICE,
														MailManager.CHANNELID_STUDIO,
														MailManager.CHANNELID_SYSTEM,
//														MailManager.CHANNELID_RESOURCE_HELP,
														MailManager.CHANNELID_MOD,
														MailManager.CHANNELID_MONSTER,
														MailManager.CHANNELID_RESOURCE};
	public void refreshOrder()
	{
		if(ChatServiceController.isNewMailUIEnable){
			for (int i = MAIN_CHANNEL_ORDERS.length - 1; i >= 0; i--)
			{
				String type = MAIN_CHANNEL_ORDERS[i];
				for (int j = 0; j < list.size(); j++)
				{
					ChatChannel channel = (ChatChannel) list.get(j);
					if(channel.channelID.equals(type)){
						moveToHead(j);
						break;
					}
				}
			}
			
			notifyDataSetChangedOnUI();
		}else{
			super.refreshOrder();
		}
	}
	
	private void moveToHead(int i)
	{
		list.add(0, list.remove(i));
	}
}
