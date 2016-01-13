package com.elex.chatservice.view.adapter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelListItem;
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
	public ChatChannel	parentChannel;

	public SysMailAdapter(ChannelListActivity context, ChannelListFragment fragment)
	{
		super(context, fragment);

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
			int dbCount = parentChannel.getSysMailCountInDB();
			return dbCount > parentChannel.mailDataList.size();
		}
		
	}

	public synchronized void loadMoreData()
	{
		MailData lastItem = null;
		if (parentChannel.mailDataList.size() > 0)
		{
			lastItem = parentChannel.mailDataList.get(parentChannel.mailDataList.size() - 1);
		}
		ChannelManager.getInstance().loadMoreSysMailFromDB(parentChannel, lastItem != null ? lastItem.getCreateTime() : -1);
	}

	public void reloadData()
	{
		parentChannel = ChannelManager.getInstance().getChannel(context.channelType, fragment.channelId);
		if (list.size() < ChannelManager.LOAD_MORE_COUNT && parentChannel.mailDataList.size() < ChannelManager.LOAD_MORE_COUNT
				&& hasMoreData())
		{
			context.showProgressBar();
			isLoadingMore = true;
			loadMoreData();
		}
		else
		{
			refreshAdapterList();
		}
	}
	
	public void refreshAdapterList()
	{
		list.clear();
		if (context.channelType != -1 && !fragment.channelId.equals(""))
		{
			if (parentChannel != null)
			{
				list.addAll(parentChannel.mailDataList);
			}
		}
		refreshOrder();
		fragment.setNoMailTipVisible(list.size() <= 0);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		MailData mailData = (MailData) getItem(position);
		if (mailData == null)
			return null;

//		parentChannel.refreshRenderData();
		convertView = super.getView(position, convertView, parent);

		CategoryViewHolder holder = (CategoryViewHolder) convertView.getTag();
		int bgColor = 0;
		if (ChatServiceController.isNewMailUIEnable)
		{
			bgColor = MailManager.getColorByChannelId(parentChannel.channelID);
		}
		holder.setContent(context, mailData, false, null, mailData.nameText, mailData.contentText, mailData.timeText,
				fragment.isInEditMode(), position, bgColor);
		setIcon(mailData, holder.item_icon);
		refreshMenu();

		return convertView;
	}

	@Override
	public int getItemViewType(int position)
	{
		ChannelListItem item = getItem(position);
		if (item != null)
		{
			if (item.isUnread())
				return VIEW_TYPE_READ_AND_DELETE;
			else
				return VIEW_TYPE_DELETE;
		}
		return VIEW_TYPE_READ_AND_DELETE;
	}

	private void setIcon(MailData mailData, ImageView iconView)
	{
		String mailIcon = mailData.mailIcon;
		if (mailIcon.equals(""))
		{
			int defaultId = ResUtil.getId(context, "drawable", "g026");
			try
			{
				if (defaultId != 0)
					iconView.setImageDrawable(context.getResources().getDrawable(defaultId));
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
		else
		{
			int idFlag = ResUtil.getId(context, "drawable", mailIcon);
			try
			{
				if (idFlag != 0)
				{
					// 极少情况可能发生 Fatal Exception: java.lang.OutOfMemoryError
					// ，且没有被try捕获
					iconView.setImageDrawable(context.getResources().getDrawable(idFlag));
				}
				else
				{
					int defaultId = ResUtil.getId(context, "drawable", "g026");
					if (defaultId != 0)
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
