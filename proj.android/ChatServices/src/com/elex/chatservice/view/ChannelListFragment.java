package com.elex.chatservice.view;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import android.annotation.TargetApi;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.ResolveInfo;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Bundle;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.AdapterView;
import android.widget.CheckBox;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.alibaba.fastjson.JSON;
import com.baoyz.swipemenulistview.SwipeMenu;
import com.baoyz.swipemenulistview.SwipeMenuCreator;
import com.baoyz.swipemenulistview.SwipeMenuItem;
import com.baoyz.swipemenulistview.SwipeMenuListView;
import com.baoyz.swipemenulistview.SwipeMenuListView.OnMenuItemClickListener;
import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ApplicationItem;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.ImageUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;
import com.elex.chatservice.view.adapter.AbstractMailListAdapter;
import com.elex.chatservice.view.adapter.SysMailAdapter;
import com.lee.pullrefresh.ui.PullToRefreshBase;
import com.lee.pullrefresh.ui.PullToRefreshBase.OnRefreshListener;
import com.lee.pullrefresh.ui.PullToRefreshSwipeListView;

public class ChannelListFragment extends ActionBarFragment
{
	protected AbstractMailListAdapter		adapter				= null;

	protected SwipeMenuListView				mListView;
	protected PullToRefreshSwipeListView	channelListPullView;
	protected TextView						tip_no_mail_textView;

	private LinearLayout					mailButtonBarLayout;
	private ImageView						mailButtonBarWrite;
	private View							mailButtonBarAll;
	private TextView						checkboxLabel;
	private ImageView						mailButtonBarDelete;

	public String							channelId			= "";
	private boolean							allSelectedValue	= false;

	protected ChannelListActivity			channelListActivity;
	
	public boolean isInEditMode()
	{
		return isInEditMode;
	}
	public SwipeMenuListView getListView()
	{
		return mListView;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = (ChannelListActivity) getActivity();
		channelListActivity = (ChannelListActivity) activity;
		
//		isSecondLvList=false;
		Bundle extras = this.activity.getIntent().getExtras();
		if(extras != null){
//			if(extras.containsKey("isSecondLvList"))
//				isSecondLvList=extras.getBoolean("isSecondLvList");
			if(extras.containsKey("channelId"))
				channelId=extras.getString("channelId");
		}
		else
		{
			channelId = "";
		}
		System.out.println("onCreateView channelId:"+channelId);
		if(channelId.equals(MailManager.CHANNELID_MOD))
		{
			ChatServiceController.isContactMod = true;
		}
		else
		{
			ChatServiceController.isContactMod = false;
		}

		int layoutId = ChatServiceController.isNewMailUIEnable ? R.layout.cs__channel_list_new : R.layout.cs__channel_list;
		return inflater.inflate(layoutId, container, false);
	}
	
	protected static boolean dataChanged = false;
	public static void onChannelAdd()
	{
		dataChanged = true;
		if(ChatServiceController.getChannelListFragment() != null){
			System.out.println("onChannelAdd");
			ChatServiceController.getChannelListFragment().reload();
			dataChanged = false;
		}
	}
	
	public static void onMailAdded()
	{
		// TODO 判断是否已经在一级列表中
		onChannelAdd();
	}
	
	public static void onMsgAdded(ChatChannel channel)
	{
		if(ChatServiceController.getMainListFragment() != null && ChatServiceController.getMainListFragment().adapter!=null && ChatServiceController.getMainListFragment().adapter.list!=null){
			for (int i = 0; i < ChatServiceController.getMainListFragment().adapter.list.size(); i++)
			{
				ChannelListItem item = ChatServiceController.getMainListFragment().adapter.list.get(i);
				if(item instanceof ChatChannel && channel.channelID.equals(((ChatChannel)item).channelID)){
					dataChanged = false;
					ChatServiceController.getMainListFragment().adapter.refreshOrder();
					return;
				}
			}
		}
		
		onChannelAdd();
	}
	
	private static void refreshModChannel(ChatChannel chatChannel)
	{
		if(chatChannel!=null && chatChannel.isModChannel())
			dataChanged = true;
	}
	
	public static void onChannelRefresh()
	{
		dataChanged = true;
		if(ChatServiceController.getMainListFragment() != null && ChatServiceController.getMainListFragment().adapter!=null){
			System.out.println("onChannelRefresh");
			ChatServiceController.getMainListFragment().refreshChannel();
		}
	}
	
	
	public static void onMailDataAdded(final MailData mailData)
	{
		dataChanged = true;
		if(ChatServiceController.getSysMailListFragment() != null ){
			ChatServiceController.getSysMailListFragment().refreshMailDataList(mailData);
		}
	}
	
	public static void onMailDataRefresh(final MailData mailData)
	{
		dataChanged = true;
		if(ChatServiceController.getSysMailListFragment() != null ){
			ChatServiceController.getSysMailListFragment().updateMailDataList(mailData);
		}
	}
	
	public void reload()
	{
		if(adapter != null && activity != null){
			activity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						adapter.reloadData();
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	public void onResume()
	{
		super.onResume();
		
		refreshTitleLabel();
		
		if(!this.inited){
			activity.showProgressBar();
			onBecomeVisible();
			return;
		}
		
		if(dataChanged || ChannelManager.getInstance().isInRootChannelList){
			reload();
			dataChanged = false;
		}else{
			adapter.refreshOrder();
		}
		if(ChannelManager.getInstance().isInRootChannelList)
			ChannelManager.getInstance().isInRootChannelList = false;
	}
	
	public void refreshScrollLoadEnabled()
	{
		channelListPullView.setPullLoadEnabled(false);
		channelListPullView.setPullRefreshEnabled(false);
		
		channelListPullView.setScrollLoadEnabled(false);
	}
	
	public void setNoMailTipVisible(boolean isVisble)
	{
		if(tip_no_mail_textView!=null && ChatServiceController.isNewMailUIEnable)
			tip_no_mail_textView.setVisibility(isVisble?View.VISIBLE:View.GONE);
	}
	
	private boolean isInEditMode = false;
	public void onViewCreated(final View view, Bundle savedInstanceState)
    {
		super.onViewCreated(view, savedInstanceState);
		
		channelListPullView = (PullToRefreshSwipeListView) view.findViewById(R.id.channelListPullView);
		channelListPullView.setLanguage(LanguageManager.getLangByKey(LanguageKeys.TIP_LOADING));
		mListView = channelListPullView.getRefreshableView();
		mListView.setCacheColorHint(0x00000000);
		if(ChatServiceController.isNewMailUIEnable){
			mListView.setDivider(null);
			
			ImageUtil.setYRepeatingBG(activity, channelListPullView, R.drawable.mail_list_bg);
		}else{
			mListView.setDivider(activity.getResources().getDrawable(R.drawable.mail_separate3));
		}
	    
		channelListPullView.setOnRefreshListener(new OnRefreshListener<SwipeMenuListView>() {
            @Override
            public void onPullDownToRefresh(PullToRefreshBase<SwipeMenuListView> refreshView) {
//                mIsStart = true;
            }

            @Override
            public void onPullUpToRefresh(PullToRefreshBase<SwipeMenuListView> refreshView) {
				if (adapter != null && adapter.hasMoreData())
				{
					adapter.loadMoreData();
				}
//                mIsStart = false;
            }
        });
		
		tip_no_mail_textView = (TextView) view.findViewById(R.id.tip_no_mail);
		if(ChatServiceController.isNewMailUIEnable && tip_no_mail_textView!=null)
		{
			tip_no_mail_textView.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_NO_MAIL));
			tip_no_mail_textView.setVisibility(View.GONE);
		}
		
		
		mailButtonBarLayout = (LinearLayout) view.findViewById(R.id.mailButtonBarLayout);
		mailButtonBarWrite = (ImageView) view.findViewById(R.id.mailButtonBarWrite);
		mailButtonBarAll = view.findViewById(R.id.mailButtonBarAll);
		checkboxLabel = (TextView) view.findViewById(R.id.checkboxLabel);
		mailButtonBarDelete = (ImageView) view.findViewById(R.id.mailButtonBarDelete);
		if(ChatServiceController.isNewMailUIEnable) showBottomBar(false);
		refreshTitleLabel();
		
		getTitleLabel().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
//				ChatServiceController.getInstance().host.testMailCommand();
//				ChatServiceController.getInstance().host.changeMailListSwitch(!ChatServiceController.isNewMailListEnable);
//				ChatServiceController.isNewMailListEnable = !ChatServiceController.isNewMailListEnable;
			}
		});
		
		showEditButton(true);
		getEditButton().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				enterEditMode();
			}
		});
		getReturnButton().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				exitEditMode();
			}
		});
		
		mailButtonBarAll.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				selectAll();
			}
		});
		if(checkboxLabel != null){
			checkboxLabel.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View view)
				{
					((CheckBox)mailButtonBarAll).setChecked(!((CheckBox)mailButtonBarAll).isChecked());
					selectAll();
				}
			});
		}
		
		mailButtonBarDelete.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				deleteMultiple();
			}
		});
		
		mailButtonBarWrite.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				exitEditMode();
				writeNewMail();
			}
		});
		
		SwipeMenuCreator creator = new SwipeMenuCreator()
		{
			@Override
			public void create(SwipeMenu menu)
			{
				switch (menu.getViewType())
				{
					case AbstractMailListAdapter.VIEW_TYPE_DELETE:
						SwipeMenuItem deleteItem = new SwipeMenuItem(channelListActivity.getApplicationContext());
						deleteItem.setBackground(new ColorDrawable(Color.rgb(0xD0, 0x70, 0x50)));
						deleteItem.setTitle(LanguageManager.getLangByKey(LanguageKeys.DELETE));
						deleteItem.setTitleSize(ScaleUtil.getAdjustTextSize(16, ConfigManager.scaleRatio));
						deleteItem.setTitleColor(Color.WHITE);
						deleteItem.setWidth(dp2px(ScaleUtil.getAdjustTextSize(90, ConfigManager.scaleRatio)));
						// deleteItem.setIcon(R.drawable.ic_delete);
						menu.addMenuItem(deleteItem);
						break;
					case AbstractMailListAdapter.VIEW_TYPE_NONE:
						break;
				}
			}
		};
		mListView.setMenuCreator(creator);
		
		mListView.setOnItemClickListener(new AdapterView.OnItemClickListener()
		{
			// 在本例中 arg2==arg3
			public void onItemClick(AdapterView<?> adapterView, View view, int arg2, long arg3)
			{
				onListItemClick(adapterView, view, arg2);
			}
		});

		mListView.setOnMenuItemClickListener(new OnMenuItemClickListener()
		{
			@Override
			public void onMenuItemClick(int position, SwipeMenu menu, int index)
			{
				switch (index)
				{
					case 0:
						onDeleteMenuClick(position);
						break;
				}
			}
		});

        FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
        onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener() {
	        public void onGlobalLayout() {
				adjustHeight();
	        }
	    };
        content.getChildAt(0).getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);
        
        ((ChannelListActivity) getActivity()).fragment = this;
    }
	
	private void selectAll()
	{
		allSelectedValue = !allSelectedValue;
		for (Iterator<?> iterator = adapter.list.iterator(); iterator.hasNext();)
		{
			ChannelListItem item = (ChannelListItem) iterator.next();
			item.checked = allSelectedValue;
		}
		notifyDataSetChanged();
	}

	public void refreshTitleLabel()
	{
		if(ServiceInterface.isHandlingGetNewMailMsg){
			getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TIP_LOADING));
		}else{
			setTitleLabel();
		}
	}
	
	protected void setTitleLabel()
	{
		getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TITLE_MAIL));
	}

	protected void writeNewMail()
	{
		ServiceInterface.showWriteMailActivity(channelListActivity, false, null, null, null);
	}
	
	public void onLoadMoreComplete()
	{
        notifyDataSetChanged();

		if(activity != null){
			activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
				        channelListPullView.onPullDownRefreshComplete();
				        channelListPullView.onPullUpRefreshComplete();
//				        channelListPullView.setHasMoreData(adapter.hasMoreData());
				        refreshScrollLoadEnabled();
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	protected void onBecomeVisible() {
		if(inited) return;
		
		jumpToSecondaryList();

		timerDelay = 600;
		startTimer();
	}
	
	protected void jumpToSecondaryList()
	{
		
	}
	
	protected void createList()
	{
		adapter.fragment = this;
		refreshScrollLoadEnabled();
	}
	
	protected void renderList()
	{
		mListView.setAdapter(adapter);
		restorePosition();
		
		activity.hideProgressBar();
	}
	
	protected void restorePosition()
	{
	}
	
	private String appendStr(String originStr,String appendStr)
	{
		String ret = originStr;
		if (StringUtils.isNotEmpty(appendStr) && !ret.contains(appendStr))
		{
			if (ret.equals(""))
				ret = appendStr;
			else
				ret += "," + appendStr;
		}
		return ret;
	}
	
	protected void enterEditMode()
	{
		isInEditMode = true;
		showEditButton(false);
		showBottomBar(true);
	}
	
	protected void exitEditMode()
	{
		isInEditMode = false;
		showEditButton(true);
		showBottomBar(false);
	}

	public void notifyDataSetChanged()
	{
		System.out.println("channelListFragment notifyDataSetChanged");
    	try
		{
    		if(adapter != null)
    			adapter.notifyDataSetChangedOnUI();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private void openDummyChannel(ChannelListItem item, int index)
	{
		if(index % 2 == 0){
			ServiceInterface.showChatActivity(channelListActivity, DBDefinition.CHANNEL_TYPE_COUNTRY, false);
		}else{
			ServiceInterface.showChannelListActivity(channelListActivity, true, DBDefinition.CHANNEL_TYPE_OFFICIAL, "dummyList", false);
		}
	}
	
	protected void openChannel(ChatChannel channel)
	{
		ChatServiceController.isCreateChatRoom = false;
		// 打开具体聊天
		if (channel.channelType < DBDefinition.CHANNEL_TYPE_OFFICIAL || channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
		{
			if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER || channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
			{
				if(channel.channelID.equals(MailManager.CHANNELID_MOD) || channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
				{
					ServiceInterface.showChannelListActivity(channelListActivity, false, DBDefinition.CHANNEL_TYPE_USER, channel.channelID, false);
				}
				else
				{
					System.out.println("openChannel fromUid:"+channel.channelID+" channel.customName:"+channel.getCustomName());
					ServiceInterface.setMailInfo(channel.channelID, channel.latestId, channel.getCustomName(), MailManager.MAIL_USER);
					ServiceInterface.showChatActivity(channelListActivity, channel.channelType, false);
					if(channel.channelType == DBDefinition.CHANNEL_TYPE_USER && !channel.channelID.equals(MailManager.CHANNELID_MOD) && !channel.channelID.equals(MailManager.CHANNELID_MESSAGE) && StringUtils.isNotEmpty(channel.latestId))
					{
						if(ChatServiceController.isContactMod)
						{
							String fromUid = ChannelManager.getInstance().getModChannelFromUid(channel.channelID);
							ChatServiceController.getInstance().host.readChatMail(fromUid, true);
						}
						else
							ChatServiceController.getInstance().host.readChatMail(channel.channelID, false);
					}
				}
			}
			else
			{
				ServiceInterface.showChatActivity(channelListActivity, channel.channelType, false);
			}
			
			
		}
		// 打开二级列表
		else if(channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			if(channel.channelID.equals(MailManager.CHANNELID_MONSTER) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP))
			{
				MailData mail = null;
				if(channel.channelID.equals(MailManager.CHANNELID_MONSTER))
					mail = channel.getMonsterMailData();
				else if(channel.channelID.equals(MailManager.CHANNELID_RESOURCE))
					mail = channel.getResourceMailData();
				else if(channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP))
					mail = channel.getResourceHelpMailData();
				
				if(mail!=null)
				{
					transMailData(mail);
					ChatServiceController.getInstance().host.readMail(mail.getUid(), mail.getType());
					channel.markAsRead();
				}
				else
				{
					System.out.println("resource or monster mail is null！");
				}
			}
			else
			{
				ServiceInterface.showChannelListActivity(channelListActivity, true, DBDefinition.CHANNEL_TYPE_OFFICIAL, channel.channelID, false);
			}
			
		}
	}
	
	private void transMailData(MailData mailData)
	{
		if(mailData!=null)
		{
			try {
				String jsonStr="";
				if(mailData.getType()==MailManager.MAIL_BATTLE_REPORT)
				{
					MailData mail = MailManager.getInstance().parseMailDataContent(mailData);
					mail.setContents("");
					jsonStr = JSON.toJSONString(mail);
				}
				else
					jsonStr = JSON.toJSONString(mailData);
				MailManager.getInstance().transportMailInfo(jsonStr);
				ChatServiceController.doHostAction("showMailPopup", mailData.getUid(), "", "", true, true);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	
	protected void openMail(MailData mailData)
	{
		if(mailData!=null)
		{
			transMailData(mailData);
			if(mailData.isUnread()){
				// 更新mail
				 mailData.setStatus(1);
				 ChatServiceController.getInstance().host.readMail(mailData.getUid(), mailData.getType());
				 DBManager.getInstance().updateMail(mailData);

				ChatChannel parentChannel = ((SysMailAdapter) adapter).parentChannel;
				// 更新channel
				if (parentChannel.unreadCount > 0)
				{
					parentChannel.unreadCount--;
					ChannelManager.getInstance().calulateAllChannelUnreadNum();
				}
				parentChannel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
				DBManager.getInstance().updateChannel(parentChannel);
			}
		}
	}

	private void open(ApplicationInfo item)
	{
		// open app
		Intent resolveIntent = new Intent(Intent.ACTION_MAIN, null);
		resolveIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		resolveIntent.setPackage(item.packageName);
		List<ResolveInfo> resolveInfoList = channelListActivity.getPackageManager().queryIntentActivities(resolveIntent, 0);
		if (resolveInfoList != null && resolveInfoList.size() > 0)
		{
			ResolveInfo resolveInfo = resolveInfoList.get(0);
			String activityPackageName = resolveInfo.activityInfo.packageName;
			String className = resolveInfo.activityInfo.name;

			Intent intent = new Intent(Intent.ACTION_MAIN);
			intent.addCategory(Intent.CATEGORY_LAUNCHER);
			ComponentName componentName = new ComponentName(activityPackageName, className);

			intent.setComponent(componentName);
			startActivity(intent);
			// 设置切换动画，从右边进入，左边退出
			channelListActivity.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
		}
	}
	
	public boolean handleBackPressed()
	{
		if(isInEditMode){
			exitEditMode();
			return true;
		}else{
			return false;
		}
	}

	protected void showEditButton(boolean show)
	{
		getEditButton().setVisibility(show ? View.VISIBLE : View.GONE);
		getReturnButton().setVisibility(!show ? View.VISIBLE : View.GONE);
	}
	private void showBottomBar(boolean show)
	{
		mailButtonBarLayout.setVisibility(show ? View.VISIBLE : View.GONE);
	}
	
    private boolean adjustSizeCompleted = false;
	public void adjustHeight() {
    	if(!ConfigManager.getInstance().scaleFontandUI || ChatServiceController.isNewMailUIEnable){
			adjustSizeCompleted = true;
    		return;
    	}
    	
		if (mailButtonBarLayout.getWidth() != 0 && !adjustSizeCompleted) {
			RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams((int) mailButtonBarLayout.getWidth(), 
			 			                                                           (int) (70 * ConfigManager.scaleRatioButton));
			param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
			mailButtonBarLayout.setLayoutParams(param);
			
			adjustSizeCompleted = true;
			showBottomBar(false);
		}
	}

	private int dp2px(int dp)
	{
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, getResources().getDisplayMetrics());
	}

	protected static int lastScrollX = -1;
	protected static int lastScrollY = -1;
	protected static int secondLastScrollX = -1;
	protected static int secondLastScrollY = -1;
	public static boolean rememberSecondChannelId;
	protected static String lastSecondChannelId = "";
	
	protected Point getCurrentPos()
	{
		if(mListView == null)
		{
			return null;
		}
		int x = mListView.getFirstVisiblePosition();
		View v = mListView.getChildAt(0);
		int y = (v == null) ? 0 : (v.getTop() - mListView.getPaddingTop());
		
		return new Point(x, y);
	}

//	private static final String temp_delete_confirm = "Are you sure you want to delete?"; // "您确定要删除吗?"
//	private static final String temp_delete_confirm1 = "Do you really want to delete these mails?"; // "您真的要删除这些邮件吗?"
//	private static final String temp_delete_confirm2 = "This mail is locked, please unlock it first."; // "邮件已锁定，请解锁后再删除"
//	private static final String temp_delete_confirm3 = "Do you really want to delete all messages from this chatroom?"; // "您真的要清空聊天室消息吗?"

	protected void onDeleteMenuClick(int position)
	{
	}
	
	protected void deleteDummyItem(int position)
	{
		adapter.list.remove(position);
		
		notifyDataSetChanged();
	}
	
	protected void deleteChannel(int position)
	{
		ChatChannel channel = (ChatChannel) adapter.getItem(position);
		if (channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			return;
		}

		String content = "";
		if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
		{
			content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_COMFIRM);
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			content = LanguageManager.getLangByKey(LanguageKeys.MAIL_CLEAR_CHATROOM_COMFIRM);
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			boolean hasCannotDeleteMail = channel.hasCannotDeleteMail();
			if (hasCannotDeleteMail)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD_OR_LOCK);
			}
			else
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
			}
		}
		MenuController.showDeleteChannelConfirm(content, channel);
	}

	protected void deleteSysMail(int position)
	{
		if (adapter.getCount() <= 0) return;
		
		MailData mailData = (MailData) adapter.getItem(position);
		if (!mailData.canDelete())
		{
			if (!mailData.hasNotGetReward())
				MenuController.showContentConfirm(LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD));
			else if (mailData.getSave() == 1)
			{
				MenuController.showContentConfirm(LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_LOCK));
			}
		}
		else
		{
			String content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_COMFIRM);
			MenuController.showDeleteSysMailConfirm(content, mailData);
		}
	}

	
	private void deleteMultiple()
	{
		ArrayList<ChannelListItem> checkedItems = new ArrayList<ChannelListItem>();
		for (Iterator<?> iterator = adapter.list.iterator(); iterator.hasNext();)
		{
			ChannelListItem item = (ChannelListItem) iterator.next();
			if (item.checked)
			{
				checkedItems.add(item);
			}
		}

		String content = "";
		boolean hasCannotDeleteMail = false;

		for (int i = 0; i < checkedItems.size(); i++)
		{
			ChannelListItem item = checkedItems.get(i);
			if (item == null) continue;
			
			if (item instanceof ChatChannel)
			{
				ChatChannel channel = (ChatChannel) item;
				if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && channel.hasCannotDeleteMail())
				{
					hasCannotDeleteMail = true;
					break;
				}
			}
			else if (item instanceof MailData)
			{
				MailData mailData = (MailData) item;
				if (mailData.channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && !mailData.canDelete())
				{
					hasCannotDeleteMail = true;
					break;
				}
			}
		}

		if (hasCannotDeleteMail)
		{
			content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD_OR_LOCK) + "\n"
					+ LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
		}
		else
		{
			content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
		}

		MenuController.showDeleteMutiMail(content, checkedItems);
	}
	
	public void actualDeleteSingleSysMail(MailData mailData)
	{
		ChatServiceController.getInstance().host.deleteSingleMail(mailData.tabType,mailData.getType(),mailData.getUid(),"");
		ChannelManager.getInstance().deleteSysMailFromChannel(mailData.channel,mailData.getUid());
		adapter.list.remove(mailData);
		adapter.notifyDataSetChangedOnUI();
	}
	
	public void actualDeleteSingleChannel(ChatChannel channel)
	{
		List<ChannelListItem> channels = new ArrayList<ChannelListItem>();
		channels.add(channel);
		actualDeleteChannels(channels);
	}
	
	public void comfirmDeleteMutiMail(List<ChannelListItem> checkedItems)
	{
	}
	
	protected void actualDeleteChannels(List<ChannelListItem> channels)
	{
		String uids="";
		String types="";
		for (int i = 0; i < channels.size(); i++)
		{
			if (channels.get(i) != null && channels.get(i) instanceof ChatChannel)
			{
				ChatChannel channel = (ChatChannel) channels.get(i);
				if (channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
					continue;

				if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
				{
					List<String> uidArray = channel.getChannelDeleteUidArray();
					if (uidArray.size() > 0)
					{
						String mailUid = uidArray.get(0);
						uids = appendStr(uids, mailUid);
					}
					String type = channel.getChannelDeleteTypes();
					types = appendStr(types, type);

					ChannelManager.getInstance().deleteChannel(channel);
					adapter.list.remove(channel);
				}
				else if (channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
				{
					ChannelManager.getInstance().deleteChannel(channel);
					adapter.list.remove(channel);
				}
				else if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
				{

					List<String> uidArray = channel.getChannelDeleteUidArray();
					String mailUids = getDeleteUids(uidArray);
					String type = channel.getChannelDeleteTypes();

					uids = appendStr(uids, mailUids);
					types = appendStr(types, type);

					boolean hasCannotDeleteMail = channel.hasCannotDeleteMail();
					if (hasCannotDeleteMail)
					{
						for (int j = 0; j < uidArray.size(); j++)
						{
							String mailUid = uidArray.get(j);
							if (!mailUid.equals("")) ChannelManager.getInstance().deleteSysMailFromChannel(channel, mailUid);
						}
						DBManager.getInstance().deleteSysMailChannel(channel.getChatTable());
						ChannelManager.getInstance().calulateAllChannelUnreadNum();
						
					}
					else
					{
						ChannelManager.getInstance().deleteChannel(channel);
						adapter.list.remove(channel);
					}

				}
			}
		}

		adapter.notifyDataSetChangedOnUI();
		if (!(uids.equals("") && types.equals("")))
		{
			ChatServiceController.getInstance().host.deleteMutiMail(uids, types);
		}
	}

	protected void actualDeleteSysMails(List<ChannelListItem> sysMails)
	{
		String uids = "";
		String types = "";

		ChatChannel channel = null;
		int canDeleteStatus = 0;
		for (int i = 0; i < sysMails.size(); i++)
		{
			MailData mailData = (MailData) sysMails.get(i);
			if (mailData != null && mailData.channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
			{
				if (mailData.canDelete())
				{
					if (channel == null) channel = mailData.channel;
					channel = mailData.channel;
					uids = appendStr(uids, mailData.getUid());
					types = appendStr(types, "" + mailData.getType());
					if (!mailData.getUid().equals(""))
						ChannelManager.getInstance().deleteSysMailFromChannel(mailData.channel, mailData.getUid());
					adapter.list.remove(mailData);
					if (canDeleteStatus == 0) canDeleteStatus = 1;
				}
				else
				{
					if (canDeleteStatus == 1) canDeleteStatus = 2;
				}
			}
		}

		if (canDeleteStatus == 1 || canDeleteStatus == 2) // 只能删一部分
		{
			adapter.notifyDataSetChangedOnUI();
		}

		if (channel != null && channel.mailDataList.size() == 0)
		{
			ChannelManager.getInstance().deleteChannel(channel);
		}

		if (!(uids.equals("") && types.equals("")))
		{
			ChatServiceController.getInstance().host.deleteMutiMail(uids, types);
		}
	}
	
	private String getDeleteUids(List<String> uidArray)
	{
		String uids="";
		for(int i=0;i< uidArray.size();i++)
		{
			String uid=uidArray.get(i);
			if(!uid.equals("") && !uids.contains(uid))
			{
				if(uids.equals(""))
					uids=uid;
				else
					uids+=","+uid;
			}
		}
		return uids;
	}
	
	protected void onListItemClick(AdapterView<?> adapterView, View view, int arg2)
	{
		if(isInEditMode){
			CheckBox checkbox = (CheckBox) view.findViewById(R.id.channel_checkBox);
			ChannelListItem item = (ChannelListItem) adapterView.getItemAtPosition(arg2);
			item.checked = !item.checked;
			checkbox.setChecked(item.checked);
			return;
		}
		
		if(ChatServiceController.getInstance().isInDummyHost()){
			if(ChatServiceController.isNewMailUIEnable){
				openItem((ChannelListItem) adapterView.getItemAtPosition(arg2));
			}else{
				ApplicationItem item = (ApplicationItem) adapterView.getItemAtPosition(arg2);
				openDummyChannel(item, arg2);
			}
		}else{
			openItem((ChannelListItem) adapterView.getItemAtPosition(arg2));
		}
	}
	
	protected void openItem(ChannelListItem item)
	{
	}

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public void onDestroy() {
		if (adapter != null)
		{
			adapter.destroy();
			adapter = null;
		}
		
		if(mListView != null) {
			mListView.clearAdapter();
			mListView.setMenuCreator(null);
			mListView.setOnItemClickListener(null);
			mListView.setOnMenuItemClickListener(null);
			mListView = null;
		}
//		mListView.setOnItemClickListener(null);
//		mListView.setOnMenuItemClickListener(null);

		channelListPullView.setOnRefreshListener(null);
		channelListPullView = null;

		mailButtonBarAll.setOnClickListener(null);
		mailButtonBarDelete.setOnClickListener(null);
		mailButtonBarWrite.setOnClickListener(null);
		if(checkboxLabel != null) checkboxLabel.setOnClickListener(null);
		
		mailButtonBarLayout = null;
		mailButtonBarWrite = null;
		mailButtonBarAll = null;
		checkboxLabel = null;
		mailButtonBarDelete = null;
		
		getTitleLabel().setOnClickListener(null);
		getEditButton().setOnClickListener(null);
		getReturnButton().setOnClickListener(null);

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk >= android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
	        FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
	        content.getChildAt(0).getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
		}
		onGlobalLayoutListener = null;
		
        ((ChannelListActivity) getActivity()).fragment = null;
		
    	super.onDestroy();
    }
    
    private ViewTreeObserver.OnGlobalLayoutListener onGlobalLayoutListener;
}
