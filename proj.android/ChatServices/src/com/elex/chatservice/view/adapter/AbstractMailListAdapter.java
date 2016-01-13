package com.elex.chatservice.view.adapter;

import java.util.ArrayList;

import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.SortUtil;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;
import com.elex.chatservice.view.MainListFragment;
import com.elex.chatservice.view.MsgMailListFragment;

public class AbstractMailListAdapter extends BaseAdapter
{
	public static final int	VIEW_TYPE_NONE				= 0;
	public static final int	VIEW_TYPE_DELETE			= 1;
	public static final int	VIEW_TYPE_READ_AND_DELETE	= 2;
	public static final int	VIEW_TYPE_READ				= 3;
	
	public ChannelListFragment			fragment;

	protected ChannelListActivity		context;
	public ArrayList<ChannelListItem>	list	= new ArrayList<ChannelListItem>();
	public boolean						isLoadingMore;

	public AbstractMailListAdapter(ChannelListActivity context, ChannelListFragment fragment)
	{
		this.context = context;
		this.fragment = fragment;
	}

	public void reloadData()
	{
	}

	public void refreshOrder()
	{
		SortUtil.getInstance().refreshListOrder(list, ChannelListItem.class);

		notifyDataSetChangedOnUI();
	}
	
	public void refreshAdapterList()
	{
		
	}

	public boolean hasMoreData()
	{
		return false;
	}

	public synchronized void loadMoreData()
	{
	}

	public int getCount()
	{
		return list.size();
	}

	@Override
	public int getViewTypeCount()
	{
		return 4;
	}

	@Override
	public int getItemViewType(int position)
	{
		return VIEW_TYPE_READ;
	}

	@Override
	public ChannelListItem getItem(int position)
	{
		if (position >= 0 && position < list.size())
			return list.get(position);
		return null;
	}

	@Override
	public long getItemId(int position)
	{
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent)
	{
		if (convertView == null)
		{
			if (ChatServiceController.isNewMailUIEnable)
			{
				if (fragment instanceof MainListFragment && !(fragment instanceof MsgMailListFragment))
				{
					convertView = View.inflate(context, R.layout.cs__channel_list_item_category, null);
					convertView.setTag(new CategoryViewHolder(convertView));
				}
				else
				{
					convertView = View.inflate(context, R.layout.cs__channel_list_item_mail, null);
					convertView.setTag(new MailViewHolder(convertView));
				}
			}
			else
			{
				convertView = View.inflate(context, R.layout.cs__channel_list_item, null);
				convertView.setTag(new MailViewHolder(convertView));
			}
		}
		return convertView;
	}

	protected void refreshMenu()
	{
		if (fragment.isInEditMode())
		{
			fragment.getListView().closeMenu();
			fragment.getListView().enabled = false;
		}
		else
		{
			fragment.getListView().enabled = true;
		}
	}

	public void notifyDataSetChangedOnUI()
	{
		if (context == null)
			return;

		context.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					notifyDataSetChanged();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public void destroy()
	{
		list.clear();
		notifyDataSetChanged();
		fragment = null;
		context = null;
		list = null;
	}
}
