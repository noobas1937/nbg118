package com.elex.chatservice.view.adapter;


import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class MsgChannelAdapter extends ChannelAdapter
{
	protected String mChannelId = "";
	public MsgChannelAdapter(ChannelListActivity context, ChannelListFragment fragment,String channelId) {
		super(context, fragment);
		mChannelId = channelId;
		if(channelId.equals(MailManager.CHANNELID_MOD))
		{
			System.out.println("MsgChannelAdapter ChatServiceController.isContactMod = true");
			ChatServiceController.isContactMod = true;
		}
		reloadData();
	}
	
	public void reloadData()
	{
		list.clear();
		if(mChannelId.equals(MailManager.CHANNELID_MOD))
			list.addAll(ChannelManager.getInstance().getAllModChannel());
		else if(mChannelId.equals(MailManager.CHANNELID_MESSAGE))
			list.addAll(ChannelManager.getInstance().getAllMessageChannel());
		refreshOrder();
		fragment.setNoMailTipVisible(list.size()<=0);
	}
	
	@Override
	public void destroy()
	{
		System.out.println("MsgChannelAdapter destroy");
		super.destroy();
		ChatServiceController.isContactMod = false;
	}
}
