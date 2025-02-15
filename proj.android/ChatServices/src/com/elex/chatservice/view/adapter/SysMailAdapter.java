package com.elex.chatservice.view.adapter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;

public class SysMailAdapter extends AbstractMailListAdapter
{
	public ChatChannel parentChannel;
	
	public SysMailAdapter(ChannelListActivity context, ChannelListFragment fragment) {
		super(context, fragment);
		
		reloadData();
	}
	
	public boolean hasMoreData()
	{
		System.out.println("ChannelManager.getInstance().getSysMailDBCount(parentChannel) = " + ChannelManager.getInstance().getSysMailDBCount(parentChannel));
		System.out.println("parentChannel.mailDataList.size() = " + parentChannel.mailDataList.size());
		if(ServiceInterface.isHandlingGetNewMailMsg){
			return false;
		}else{
			return ChannelManager.getInstance().getSysMailDBCount(parentChannel) > parentChannel.mailDataList.size();
		}
	}
	
	public void loadMoreData()
	{
		MailData lastItem = null;
		if(parentChannel.mailDataList.size() > 0){
			lastItem = parentChannel.mailDataList.get(parentChannel.mailDataList.size() - 1);
		}
		ChannelManager.getInstance().loadMoreSysMailFromDB(parentChannel, lastItem != null ? lastItem.getUid() : null);
//		ServiceInterface.postMailParseStart();
	}
	
	public void reloadData()
	{
		list.clear();
		if (context.channelType != -1 && !fragment.channelId.equals(""))
		{
			parentChannel = ChannelManager.getInstance().getChannel(context.channelType, fragment.channelId);
			if (parentChannel != null)
			{
				list.addAll(parentChannel.mailDataList);
			}
		}
		refreshOrder();
		fragment.setNoMailTipVisible(list.size()<=0);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		MailData mailData = (MailData) getItem(position);
		if (mailData == null) return null;

		parentChannel.refreshRenderData();
		convertView = super.getView(position, convertView, parent);

		CategoryViewHolder holder = (CategoryViewHolder) convertView.getTag();
		int bgColor = 0;
		if(ChatServiceController.isNewMailUIEnable) {
			bgColor = MailManager.getColorByChannelId(parentChannel.channelID);
		}
		holder.setContent(context, mailData, false, null, mailData.nameText, mailData.contentText, mailData.timeText, fragment.isInEditMode(), position, bgColor);
		setIcon(mailData, holder.item_icon);
		refreshMenu();
		
		return convertView;
	}
	
	private void setIcon(MailData mailData,ImageView iconView)
	{
//		System.out.println("setIcon type:"+mailData.getType()+"  icon:"+mailData.mailIcon);
		String mailIcon=mailData.mailIcon;
		if(mailIcon.equals(""))
		{
			int defaultId=ResUtil.getId(context, "drawable", "g026");
			try
			{
				if(defaultId!=0)
					iconView.setImageDrawable(context.getResources().getDrawable(defaultId));
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
		else
		{
			int idFlag=ResUtil.getId(context, "drawable", mailIcon);
			try
			{
				if(idFlag!=0){
					// 极少情况可能发生 Fatal Exception: java.lang.OutOfMemoryError ，且没有被try捕获
					iconView.setImageDrawable(context.getResources().getDrawable(idFlag));
				}
				else
				{
					int defaultId=ResUtil.getId(context, "drawable", "g026");
					if(defaultId!=0)
						iconView.setImageDrawable(context.getResources().getDrawable(defaultId));
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}


	public void destroy()
	{
		parentChannel = null;
		super.destroy();
	}
}
