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
	public ChannelListFragment fragment;
	
	protected ChannelListActivity		context;
	public ArrayList<ChannelListItem>	list	= new ArrayList<ChannelListItem>();

	public AbstractMailListAdapter(ChannelListActivity context, ChannelListFragment fragment) {
		this.context = context;
		this.fragment = fragment;
	}
	
	public void reloadData()
	{
	}
	
	public void refreshOrder()
	{
		SortUtil.getInstance().refreshListOrder(list);
		
		notifyDataSetChangedOnUI();
	}
	
	public boolean hasMoreData()
	{
		return false;
	}
	
	public void loadMoreData()
	{
	}
	
	public int getCount() {
		return list.size();
	}
	
	@Override
    public int getViewTypeCount() {
        // menu type count
        return 2;
    }

	public static final int VIEW_TYPE_NONE = 0;
	public static final int VIEW_TYPE_DELETE = 1;
	
    @Override
    public int getItemViewType(int position) {
        return VIEW_TYPE_DELETE;
    }
    
	@Override
	public ChannelListItem getItem(int position)
	{
		if (position >= 0 && position < list.size()) return list.get(position);
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
//		if(position == 0){
//			System.out.println(position + " getItem(position) = " + getItem(position));
//			System.out.println("getItem(position).isCategory() = " + getItem(position).isCategory());
//			if(convertView != null){
//				System.out.println("getItem isViewChanged = " + isViewChanged(convertView, getItem(position).isCategory(), true));
//				
//			}
//		}
		if (convertView == null)// || isViewChanged(convertView, getItem(position).isCategory(), false)
		{
			if(ChatServiceController.isNewMailUIEnable){
				if(fragment instanceof MainListFragment && !(fragment instanceof MsgMailListFragment)){
					convertView = View.inflate(context, R.layout.cs__channel_list_item_category, null);
					convertView.setTag(new CategoryViewHolder(convertView));
				}else{
					convertView = View.inflate(context, R.layout.cs__channel_list_item_mail, null);
					convertView.setTag(new MailViewHolder(convertView));
				}
			}else{
				convertView = View.inflate(context, R.layout.cs__channel_list_item, null);
				convertView.setTag(new MailViewHolder(convertView));
			}
		}
		return convertView;
	}
	
//	private boolean isViewChanged(View convertView, boolean isCategory, boolean output)
//	{
//		if(isCategory && convertView.findViewById(R.id.channel_lock_icon) != null){
//			if(output) System.out.println("getItem not isCategory");
//			return true;
//		}else if(!isCategory && convertView.findViewById(R.id.channel_lock_icon) == null){
//			if(output) System.out.println("getItem not not isCategory");
//			return true;
//		}
//		return false;
//	}
	
	protected void refreshMenu()
	{
		if(fragment.isInEditMode()){
			fragment.getListView().closeMenu();
			fragment.getListView().enabled = false;
		}else{
			fragment.getListView().enabled = true;
		}
	}
	
	public void notifyDataSetChangedOnUI()
	{
		System.out.println("notifyDataSetChangedOnUI 1");
		if(context == null) return;
		System.out.println("notifyDataSetChangedOnUI");
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					notifyDataSetChanged();
				} catch (Exception e) {
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
