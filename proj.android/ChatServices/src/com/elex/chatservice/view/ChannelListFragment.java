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
import android.view.ViewGroup.LayoutParams;
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
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ApplicationItem;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.FlyMutiRewardInfo;
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
import com.elex.chatservice.util.SortUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;
import com.elex.chatservice.view.adapter.AbstractMailListAdapter;
import com.elex.chatservice.view.adapter.MainChannelAdapter;
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
	private RelativeLayout					channelListFragmentLayout;

	private LinearLayout					mailButtonBarLayout;
	private ImageView						mailButtonBarWrite;
	private ImageView						mailButtonBarReward;
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

		Bundle extras = this.activity.getIntent().getExtras();
		if (extras != null)
		{
			if (extras.containsKey("channelId"))
				channelId = extras.getString("channelId");
		}
		else
		{
			channelId = "";
		}
		System.out.println("onCreateView channelId:" + channelId);
		if (channelId.equals(MailManager.CHANNELID_MOD))
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

	protected static boolean	dataChanged	= false;

	public static void onChannelAdd()
	{
		System.out.println("onChannelAdd");
		dataChanged = true;
		if (ChatServiceController.getChannelListFragment() != null)
		{
			try
			{
				// 少量NullPointerException异常
				ChatServiceController.getChannelListFragment().reload();
				dataChanged = false;
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}

	public static void onMailAdded()
	{
		// TODO 判断是否已经在一级列表中
		onChannelAdd();
	}

	public static void onMsgAdded(ChatChannel channel)
	{
		System.out.println("onMsgAdded() channel.channelID:" + channel.channelID);
		channel.refreshRenderData();
		ChannelManager.getInstance().addToLoadedChannel(channel);

		ChannelListFragment fragment = ChatServiceController.isNewMailUIEnable ? ChatServiceController.getMsgListFragment()
				: ChatServiceController.getMainListFragment();
		if (fragment != null && fragment.adapter != null && fragment.adapter.list != null)
		{
			for (int i = 0; i < fragment.adapter.list.size(); i++)
			{
				ChannelListItem item = fragment.adapter.list.get(i);
				if (item instanceof ChatChannel && channel.channelID.equals(((ChatChannel) item).channelID))
				{
					dataChanged = false;
					fragment.adapter.refreshOrder();
					return;
				}
			}

			// 重新加载
			onChannelAdd();
		}

		// 如果处于主界面，未读数变了，需要刷新界面
		if (ChatServiceController.getMainListFragment() != null)
		{
			ChatServiceController.getMainListFragment().notifyDataSetChanged();
		}
	}

	private static void refreshModChannel(ChatChannel chatChannel)
	{
		if (chatChannel != null && chatChannel.isModChannel())
			dataChanged = true;
	}

	public static void onChannelRefresh()
	{
		dataChanged = true;
		if (ChatServiceController.getMainListFragment() != null && ChatServiceController.getMainListFragment().adapter != null)
		{
			System.out.println("onChannelRefresh");
			ChatServiceController.getMainListFragment().refreshChannel();
		}
	}

	public static void onMailDataAdded(final MailData mailData)
	{
		dataChanged = true;
		if (ChatServiceController.getSysMailListFragment() != null)
		{
			ChatServiceController.getSysMailListFragment().refreshMailDataList(mailData);
		}
	}

	public static void onMailDataRefresh(final MailData mailData)
	{
		dataChanged = true;
		if (ChatServiceController.getSysMailListFragment() != null)
		{
			ChatServiceController.getSysMailListFragment().updateMailDataList(mailData);
		}
	}

	public void reload()
	{
		if (adapter != null && activity != null)
		{
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

		if (!this.inited)
		{
			activity.showProgressBar();
			onBecomeVisible();
			return;
		}

		if (dataChanged || ChannelManager.getInstance().isInRootChannelList)
		{
			reload();
			dataChanged = false;
		}
		else
		{
			if (adapter != null)
				adapter.refreshOrder();
		}
		if (ChannelManager.getInstance().isInRootChannelList)
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
		if (tip_no_mail_textView != null && ChatServiceController.isNewMailUIEnable)
			tip_no_mail_textView.setVisibility(isVisble ? View.VISIBLE : View.GONE);
	}

	private boolean	isInEditMode	= false;

	public void onViewCreated(final View view, Bundle savedInstanceState)
	{
		super.onViewCreated(view, savedInstanceState);

		channelListFragmentLayout = (RelativeLayout) view.findViewById(R.id.channelListFragmentLayout);

		channelListPullView = (PullToRefreshSwipeListView) view.findViewById(R.id.channelListPullView);
		channelListPullView.setLanguage(LanguageManager.getLangByKey(LanguageKeys.TIP_LOADING));
		// 极少量RuntimeException com.baoyz.swipemenulistview.SwipeMenuListView
		// cannot be cast to com.baoyz.swipemenulistview.SwipeMenuListView
		mListView = channelListPullView.getRefreshableView();
		mListView.setCacheColorHint(0x00000000);
		if (ChatServiceController.isNewMailUIEnable)
		{
			mListView.setDivider(null);

			ImageUtil.setYRepeatingBG(activity, channelListPullView, R.drawable.mail_list_bg);
		}
		else
		{
			mListView.setDivider(activity.getResources().getDrawable(R.drawable.mail_separate3));
		}

		channelListPullView.setOnRefreshListener(new OnRefreshListener<SwipeMenuListView>()
		{
			@Override
			public void onPullDownToRefresh(PullToRefreshBase<SwipeMenuListView> refreshView)
			{
			}

			@Override
			public void onPullUpToRefresh(PullToRefreshBase<SwipeMenuListView> refreshView)
			{
				if (adapter != null && adapter.hasMoreData())
				{
					adapter.loadMoreData();
				}
			}
		});

		tip_no_mail_textView = (TextView) view.findViewById(R.id.tip_no_mail);
		if (ChatServiceController.isNewMailUIEnable && tip_no_mail_textView != null)
		{
			tip_no_mail_textView.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_NO_MAIL));
			tip_no_mail_textView.setVisibility(View.GONE);
		}

		mailButtonBarLayout = (LinearLayout) view.findViewById(R.id.mailButtonBarLayout);
		mailButtonBarWrite = (ImageView) view.findViewById(R.id.mailButtonBarWrite);
		mailButtonBarReward = (ImageView) view.findViewById(R.id.mailButtonBarReward);
		mailButtonBarAll = view.findViewById(R.id.mailButtonBarAll);
		checkboxLabel = (TextView) view.findViewById(R.id.checkboxLabel);
		mailButtonBarDelete = (ImageView) view.findViewById(R.id.mailButtonBarDelete);
		if (ChatServiceController.isNewMailUIEnable)
			showBottomBar(false);
		refreshTitleLabel();

		getTitleLabel().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				// ChatServiceController.getInstance().host.testMailCommand();
				// String test =
				// "{\"flyToolReward\":[{\"itemPic\":\"item012.png\",\"itemNum\":1},"
				// + "{\"itemPic\":\"item4002.png\",\"itemNum\":2},"
				// + "{\"itemPic\":\"item012.png\",\"itemNum\":1},"
				// + "{\"itemPic\":\"item400.png\",\"itemNum\":2},"
				// + "{\"itemPic\":\"item012.png\",\"itemNum\":1},"
				// + "{\"itemPic\":\"item408.png\",\"itemNum\":8},"
				// + "{\"itemPic\":\"item011.png\",\"itemNum\":9},"
				// + "{\"itemPic\":\"item201.png\",\"itemNum\":10},"
				// + "{\"itemPic\":\"item403.png\",\"itemNum\":11},"
				// + "{\"itemPic\":\"item403.png\",\"itemNum\":11},"
				// +
				// "{\"itemPic\":\"item402.png\",\"itemNum\":15}],\"flyReward\":[{\"itemPic\":\"ui_gold.png\",\"itemNum\":1},{\"itemPic\":\"item402.png\",\"itemNum\":2},{\"itemPic\":\"item400.png\",\"itemNum\":3}]}";
				// ServiceInterface.postMutiRewardItem(test);
				// ChatServiceController.getInstance().host.changeMailListSwitch(!ChatServiceController.isNewMailListEnable);
				// ChatServiceController.isNewMailListEnable =
				// !ChatServiceController.isNewMailListEnable;
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
		if (checkboxLabel != null)
		{
			checkboxLabel.setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View view)
				{
					((CheckBox) mailButtonBarAll).setChecked(!((CheckBox) mailButtonBarAll).isChecked());
					selectAll();
				}
			});
		}

		mailButtonBarDelete.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				operateMultiple(ChannelManager.OPERATION_DELETE_MUTI);
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

		mailButtonBarReward.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				operateMultiple(ChannelManager.OPERATION_REWARD_MUTI);
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
						menu.addMenuItem(deleteItem);
						break;
					case AbstractMailListAdapter.VIEW_TYPE_READ_AND_DELETE:
						SwipeMenuItem readItem = new SwipeMenuItem(channelListActivity.getApplicationContext());
						readItem.setBackground(new ColorDrawable(Color.rgb(0x80, 0x80, 0x80)));
						readItem.setTitle(LanguageManager.getLangByKey(LanguageKeys.MENU_MARKASREAD));
						readItem.setTitleSize(ScaleUtil.getAdjustTextSize(16, ConfigManager.scaleRatio));
						readItem.setTitleColor(Color.WHITE);
						readItem.setWidth(dp2px(ScaleUtil.getAdjustTextSize(90, ConfigManager.scaleRatio)));
						menu.addMenuItem(readItem);

						SwipeMenuItem deleteItem2 = new SwipeMenuItem(channelListActivity.getApplicationContext());
						deleteItem2.setBackground(new ColorDrawable(Color.rgb(0xD0, 0x70, 0x50)));
						deleteItem2.setTitle(LanguageManager.getLangByKey(LanguageKeys.DELETE));
						deleteItem2.setTitleSize(ScaleUtil.getAdjustTextSize(16, ConfigManager.scaleRatio));
						deleteItem2.setTitleColor(Color.WHITE);
						deleteItem2.setWidth(dp2px(ScaleUtil.getAdjustTextSize(90, ConfigManager.scaleRatio)));
						menu.addMenuItem(deleteItem2);
						break;
					case AbstractMailListAdapter.VIEW_TYPE_READ:
						SwipeMenuItem readItem2 = new SwipeMenuItem(channelListActivity.getApplicationContext());
						readItem2.setBackground(new ColorDrawable(Color.rgb(0x80, 0x80, 0x80)));
						readItem2.setTitle(LanguageManager.getLangByKey(LanguageKeys.MENU_MARKASREAD));
						readItem2.setTitleSize(ScaleUtil.getAdjustTextSize(16, ConfigManager.scaleRatio));
						readItem2.setTitleColor(Color.WHITE);
						readItem2.setWidth(dp2px(ScaleUtil.getAdjustTextSize(90, ConfigManager.scaleRatio)));
						menu.addMenuItem(readItem2);
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
				if (adapter instanceof MainChannelAdapter)
				{
					if (index == 0)
					{
						System.out.println("onReadMenuClick 1");
						onReadMenuClick(position);
					}
				}
				else
				{
					ChannelListItem item = adapter.getItem(position);
					if (item != null)
					{
						if (item.isUnread())
						{
							switch (index)
							{
								case 0:
									System.out.println("onReadMenuClick 2");
									onReadMenuClick(position);
									break;
								case 1:
									System.out.println("onDeleteMenuClick 2");
									onDeleteMenuClick(position);
									break;
							}
						}
						else
						{
							if (index == 0)
							{
								System.out.println("onDeleteMenuClick 3");
								onDeleteMenuClick(position);
							}
						}
					}
				}

			}
		});

		onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener()
		{
			public void onGlobalLayout()
			{
				adjustHeight();
			}
		};
		channelListFragmentLayout.getChildAt(0).getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);

		((ChannelListActivity) getActivity()).fragment = this;
	}

	private void selectAll()
	{
		if (adapter != null && adapter.list != null)
		{
			allSelectedValue = !allSelectedValue;
			for (Iterator<?> iterator = adapter.list.iterator(); iterator.hasNext();)
			{
				ChannelListItem item = (ChannelListItem) iterator.next();
				if (item != null)
					item.checked = allSelectedValue;
			}
			notifyDataSetChanged();
		}
	}

	public void refreshTitleLabel()
	{
		if (ServiceInterface.isHandlingGetNewMailMsg)
		{
			getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TIP_LOADING));
		}
		else
		{
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

		if (activity != null)
		{
			activity.runOnUiThread(new Runnable()
			{
				public void run()
				{
					try
					{
						channelListPullView.onPullDownRefreshComplete();
						channelListPullView.onPullUpRefreshComplete();
						refreshScrollLoadEnabled();

						if (adapter != null)
						{
							adapter.isLoadingMore = false;
							adapter.refreshAdapterList();
						}
						activity.hideProgressBar();
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	protected void onBecomeVisible()
	{
		if (inited)
			return;

		jumpToSecondaryList();

		timerDelay = 0;
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

	private String appendStr(String originStr, String appendStr)
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
			if (adapter != null)
				adapter.notifyDataSetChangedOnUI();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public void showMutiRewardFlyAnimation(FlyMutiRewardInfo rewardInfo)
	{
		if (activity != null)
			activity.showFlyMutiReward(rewardInfo);
	}

	private void openDummyChannel(ChannelListItem item, int index)
	{
		if (index % 2 == 0)
		{
			ServiceInterface.showChatActivity(channelListActivity, DBDefinition.CHANNEL_TYPE_COUNTRY, false);
		}
		else
		{
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
				if (channel.channelID.equals(MailManager.CHANNELID_MOD) || channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
				{
					ServiceInterface.showChannelListActivity(channelListActivity, false, DBDefinition.CHANNEL_TYPE_USER, channel.channelID,
							false);
				}
				else
				{
					System.out.println("openChannel fromUid:" + channel.channelID + " channel.customName:" + channel.getCustomName());
					int mailType = channel.isModChannel() ? MailManager.MAIL_MOD_PERSONAL : MailManager.MAIL_USER;
					ServiceInterface.setMailInfo(channel.channelID, channel.latestId, channel.getCustomName(), mailType);
					ServiceInterface.showChatActivity(channelListActivity, channel.channelType, false);
					if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER && !channel.channelID.equals(MailManager.CHANNELID_MOD)
							&& !channel.channelID.equals(MailManager.CHANNELID_MESSAGE) && StringUtils.isNotEmpty(channel.latestId))
					{
						if (ChatServiceController.isContactMod)
						{
							String fromUid = ChannelManager.getInstance().getModChannelFromUid(channel.channelID);
							JniController.getInstance()
									.excuteJNIVoidMethod("readChatMail", new Object[] { fromUid, Boolean.valueOf(true) });
						}
						else
						{
							JniController.getInstance().excuteJNIVoidMethod("readChatMail",
									new Object[] { channel.channelID, Boolean.valueOf(false) });
						}
					}
				}
			}
			else
			{
				ServiceInterface.showChatActivity(channelListActivity, channel.channelType, false);
			}
		}
		// 打开二级列表
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			System.out.println("openChannel channelID:" + channel.channelID);
			if (channel.channelID.equals(MailManager.CHANNELID_MONSTER) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE)
					|| channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP)
					|| channel.channelID.equals(MailManager.CHANNELID_KNIGHT))
			{
				MailData mail = null;
				if (channel.channelID.equals(MailManager.CHANNELID_MONSTER))
					mail = channel.getMonsterMailData();
				else if (channel.channelID.equals(MailManager.CHANNELID_RESOURCE))
					mail = channel.getResourceMailData();
				else if (channel.channelID.equals(MailManager.CHANNELID_KNIGHT))
					mail = channel.getKnightMailData();
				else if (channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP))
					mail = channel.getResourceHelpMailData();

				if (mail != null)
				{
					transportAndShowMailData(mail);
					if (channel.channelID.equals(MailManager.CHANNELID_KNIGHT))
					{
						String uids = channel.getMailUidsByConfigType(DBManager.CONFIG_TYPE_READ);
						if (StringUtils.isNotEmpty(uids))
						{
							JniController.getInstance().excuteJNIVoidMethod("readMutiMail", new Object[] { uids });
						}
					}
					else
					{
						JniController.getInstance().excuteJNIVoidMethod("readDialogMail",
								new Object[] { Integer.valueOf(mail.getType()), Boolean.valueOf(false), "" });
					}
				}
				else
				{
					System.out.println("resource or monster mail is null！");
				}
			}
			else
			{
				ServiceInterface.showChannelListActivity(channelListActivity, true, DBDefinition.CHANNEL_TYPE_OFFICIAL, channel.channelID,
						false);
			}
		}
	}

	private void transportAndShowMailData(MailData mailData)
	{
		if (mailData != null)
		{
			MailManager.getInstance().transportMailData(mailData);
			MailManager.getInstance().transportNeiberMailData(mailData, true, true);
			ChatServiceController.doHostAction("showMailPopup", mailData.getUid(), "", "", true, true);
		}
	}

	protected void openMail(MailData mailData)
	{
		if (mailData != null)
		{
			transportAndShowMailData(mailData);
			if (mailData.isUnread())
			{
				// 更新mail
				mailData.setStatus(1);
				JniController.getInstance().excuteJNIVoidMethod("readMail",
						new Object[] { mailData.getUid(), Integer.valueOf(mailData.getType()) });
				DBManager.getInstance().updateMail(mailData);

				// 更新channel
				ChatChannel parentChannel = ((SysMailAdapter) adapter).parentChannel;
				if (parentChannel.unreadCount > 0)
				{
					parentChannel.unreadCount--;
					ChannelManager.getInstance().calulateAllChannelUnreadNum();
				}
				parentChannel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
				DBManager.getInstance().updateChannel(parentChannel);
			}
		}
	}

	private void open(ApplicationInfo item)
	{
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
			channelListActivity.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
		}
	}

	public boolean handleBackPressed()
	{
		if (isInEditMode)
		{
			exitEditMode();
			return true;
		}
		else
		{
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

	public void adjustHeight()
	{
		if (!ConfigManager.getInstance().scaleFontandUI || ChatServiceController.isNewMailUIEnable)
		{
			adjustSizeCompleted = true;
			return;
		}

		if (mailButtonBarLayout.getWidth() != 0 && !adjustSizeCompleted)
		{
			RelativeLayout.LayoutParams param = (RelativeLayout.LayoutParams) mailButtonBarLayout.getLayoutParams();
			param.height = (int) (70 * ConfigManager.scaleRatioButton);
			mailButtonBarLayout.setLayoutParams(param);

			adjustSizeCompleted = true;
			showBottomBar(false);
		}
	}

	private int dp2px(int dp)
	{
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, getResources().getDisplayMetrics());
	}

	protected static int	lastScrollX				= -1;
	protected static int	lastScrollY				= -1;
	protected static int	secondLastScrollX		= -1;
	protected static int	secondLastScrollY		= -1;
	public static boolean	rememberSecondChannelId;
	public static boolean	preventSecondChannelId	= false;
	protected static String	lastSecondChannelId		= "";

	protected Point getCurrentPos()
	{
		if (mListView == null)
		{
			return null;
		}
		int x = mListView.getFirstVisiblePosition();
		View v = mListView.getChildAt(0);
		int y = (v == null) ? 0 : (v.getTop() - mListView.getPaddingTop());

		return new Point(x, y);
	}

	protected void onDeleteMenuClick(int position)
	{
	}

	protected void onReadMenuClick(int position)
	{
	}

	protected void deleteDummyItem(int position)
	{
		adapter.list.remove(position);

		notifyDataSetChanged();
	}

	protected void readDummyItem(int position)
	{
		System.out.println("readDummyItem");
	}

	protected void readChannel(int position)
	{
		ChatChannel channel = (ChatChannel) adapter.getItem(position);
		if (channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			return;
		}

		if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
		{
			ChatServiceController.getChannelListFragment().actualReadSingleChannel(channel);
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			ChatServiceController.getChannelListFragment().actualReadSingleChannel(channel);
		}
	}

	protected void deleteChannel(int position)
	{
		ChatChannel channel = (ChatChannel) adapter.getItem(position);
		if (channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			return;
		}

		if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
		{
			ChatServiceController.getChannelListFragment().actualDeleteSingleChannel(channel);
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			ChatServiceController.getChannelListFragment().actualDeleteSingleChannel(channel);
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			boolean hasCannotDeleteMail = channel.cannotOperatedForMuti(ChannelManager.OPERATION_DELETE_MUTI);
			String content = "";
			if (hasCannotDeleteMail)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD_OR_LOCK);
			}
			else
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
			}
			MenuController.showDeleteChannelConfirm(content, channel);
		}
	}

	protected void deleteSysMail(int position)
	{
		if (adapter.getCount() <= 0)
			return;

		MailData mailData = (MailData) adapter.getItem(position);
		if (mailData == null)
			return;
		if (!mailData.canDelete())
		{
			System.out.println("deleteSysMail rewardid:" + mailData.getRewardId() + " rewardStatus:" + mailData.getRewardStatus());
			if (mailData.hasReward())
			{
				MenuController.showContentConfirm(LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD));
			}
			else if (mailData.getSave() == 1)
			{
				MenuController.showContentConfirm(LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_LOCK));
			}
		}
		else
		{
			ChatServiceController.getChannelListFragment().actualDeleteSingleSysMail(mailData);
		}
	}

	protected void readSysMail(int position)
	{
		if (adapter.getCount() <= 0)
			return;

		MailData mailData = (MailData) adapter.getItem(position);
		if (mailData != null)
		{
			ChatServiceController.getChannelListFragment().actualReadSingleSysMail(mailData);
		}
	}

	private void operateMultiple(int type)
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
		boolean hasCannotOperateMutiMail = false;

		boolean hasMailData = false;
		for (int i = 0; i < checkedItems.size(); i++)
		{
			ChannelListItem item = checkedItems.get(i);
			if (item == null)
				continue;

			if (item instanceof ChatChannel)
			{
				ChatChannel channel = (ChatChannel) item;
				if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && channel.cannotOperatedForMuti(type))
				{
					hasCannotOperateMutiMail = true;
					break;
				}
			}
			else if (item instanceof MailData)
			{
				MailData mailData = (MailData) item;
				if (mailData != null
						&& mailData.channel != null
						&& mailData.channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL
						&& ((type == ChannelManager.OPERATION_DELETE_MUTI && !mailData.canDelete()) || (type == ChannelManager.OPERATION_REWARD_MUTI && mailData
								.hasReward())))
				{
					hasCannotOperateMutiMail = true;
					hasMailData = true;
					break;
				}
			}
		}

		if (type == ChannelManager.OPERATION_REWARD_MUTI)
		{
			if (hasMailData)
				hasCannotOperateMutiMail = false;
		}

		if (hasCannotOperateMutiMail || (type == ChannelManager.OPERATION_REWARD_MUTI && !hasCannotOperateMutiMail && !hasMailData))
		{
			if (type == ChannelManager.OPERATION_REWARD_MUTI)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.TIP_MAIL_NOREWARD);
			}
			else if (type == ChannelManager.OPERATION_DELETE_MUTI)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_NOTIFY_REWARD_OR_LOCK) + "\n"
						+ LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
			}
		}
		else
		{
			if (type == ChannelManager.OPERATION_REWARD_MUTI)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.TIP_REWARD_THESE_MAIL);
			}
			else if (type == ChannelManager.OPERATION_DELETE_MUTI)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.MAIL_DELETE_THESE_COMFIRM);
			}
		}

		MenuController.showOperateMutiMail(content, checkedItems, type);
	}

	public void actualDeleteSingleSysMail(MailData mailData)
	{
		JniController.getInstance().excuteJNIVoidMethod("deleteSingleMail",
				new Object[] { Integer.valueOf(mailData.tabType), Integer.valueOf(mailData.getType()), mailData.getUid(), "" });
		ChannelManager.getInstance().deleteSysMailFromChannel(mailData.channel, mailData.getUid(), false);
		adapter.list.remove(mailData);
		adapter.notifyDataSetChangedOnUI();
	}

	public void actualReadSingleSysMail(MailData mailData)
	{
		if (mailData.isUnread())
		{
			// 更新mail
			mailData.setStatus(1);
			JniController.getInstance().excuteJNIVoidMethod("readMail",
					new Object[] { mailData.getUid(), Integer.valueOf(mailData.getType()) });
			DBManager.getInstance().updateMail(mailData);

			// 更新channel
			ChatChannel parentChannel = ((SysMailAdapter) adapter).parentChannel;
			if (parentChannel.unreadCount > 0)
			{
				parentChannel.unreadCount--;
				ChannelManager.getInstance().calulateAllChannelUnreadNum();
			}
			parentChannel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
			DBManager.getInstance().updateChannel(parentChannel);
		}
		adapter.notifyDataSetChangedOnUI();
	}

	public void actualDeleteSingleChannel(ChatChannel channel)
	{
		List<ChannelListItem> channels = new ArrayList<ChannelListItem>();
		channels.add(channel);
		actualDeleteChannels(channels);
	}

	public void actualReadSingleChannel(ChatChannel channel)
	{
		if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER || channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			if (channel.channelID.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
			{
				String fromUid = ChannelManager.getInstance().getModChannelFromUid(channel.channelID);
				JniController.getInstance().excuteJNIVoidMethod("readChatMail", new Object[] { fromUid, Boolean.valueOf(true) });
			}
			else
			{
				JniController.getInstance().excuteJNIVoidMethod("readChatMail", new Object[] { channel.channelID, Boolean.valueOf(false) });
			}
			channel.markAsRead();
		}
	}

	public void comfirmOperateMutiMail(List<ChannelListItem> checkedItems, int type)
	{
	}

	protected void actualDeleteChannels(List<ChannelListItem> channels)
	{
		String uids = "";
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
					String mailUids = getUidsByArray(uidArray);
					uids = appendStr(uids, mailUids);

					boolean hasCannotDeleteMail = channel.cannotOperatedForMuti(ChannelManager.OPERATION_DELETE_MUTI);
					if (hasCannotDeleteMail)
					{
						for (int j = 0; j < uidArray.size(); j++)
						{
							String mailUid = uidArray.get(j);
							if (!mailUid.equals(""))
								ChannelManager.getInstance().deleteSysMailFromChannel(channel, mailUid, true);
						}
						DBManager.getInstance().deleteSysMailChannel(channel.getChatTable());
						channel.querySysMailCountFromDB();
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

		afterDeleteMsgChannel();

		adapter.notifyDataSetChangedOnUI();
		if (StringUtils.isNotEmpty(uids))
		{
			JniController.getInstance().excuteJNIVoidMethod("deleteMutiMail", new Object[] { uids, "" });
		}
	}

	private void afterDeleteMsgChannel()
	{
		boolean hasMoreData = false;
		if (!ChatServiceController.isContactMod)
		{
			ChatChannel messageChannel = ChannelManager.getInstance().getMessageChannel();
			List<ChatChannel> messageChannelList = ChannelManager.getInstance().getLoadedMessageChannel();
			if (messageChannel != null && messageChannelList != null && messageChannelList.size() == 0
					&& DBManager.getInstance().isMessageChannelExist())
			{
				hasMoreData = true;
			}
		}
		else
		{
			ChatChannel modChannel = ChannelManager.getInstance().getModChannel();
			List<ChatChannel> modChannelList = ChannelManager.getInstance().getLoadedModChannel();
			if (modChannel != null && modChannelList != null && modChannelList.size() == 0 && DBManager.getInstance().isModChannelExist())
			{
				hasMoreData = true;
			}
		}

		if (hasMoreData)
			adapter.loadMoreData();
	}

	protected void actualRewardChannels(List<ChannelListItem> channels)
	{
		String uids = "";
		for (int i = 0; i < channels.size(); i++)
		{
			if (channels.get(i) != null && channels.get(i) instanceof ChatChannel)
			{
				ChatChannel channel = (ChatChannel) channels.get(i);
				if (channel != null && channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
				{
					List<String> uidArray = channel.getChannelRewardUidArray();
					String mailUids = getUidsByArray(uidArray);
					String type = channel.getChannelRewardTypes();

					uids = appendStr(uids, mailUids);
				}
			}
		}

		if (!(uids.equals("")))
		{
			JniController.getInstance().excuteJNIVoidMethod("rewardMutiMail", new Object[] { uids, "" });
			activity.showRewardLoadingPopup();
		}
	}

	protected void actualDeleteSysMails(List<ChannelListItem> sysMails)
	{
		String uids = "";

		ChatChannel channel = null;
		int canDeleteStatus = 0;
		boolean hasDetectMail = false;
		for (int i = 0; i < sysMails.size(); i++)
		{
			MailData mailData = (MailData) sysMails.get(i);
			if (mailData != null && mailData.channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
			{
				if (mailData.canDelete())
				{
					channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mailData.getChannelId());
					uids = appendStr(uids, mailData.getUid());
					if (channel != null && StringUtils.isNotEmpty(mailData.getUid()))
					{
						if (!hasDetectMail && mailData.getType() == MailManager.MAIL_DETECT_REPORT)
							hasDetectMail = true;
						ChannelManager.getInstance().deleteSysMailFromChannel(channel, mailData.getUid(), true);
					}
					adapter.list.remove(mailData);
					if (canDeleteStatus == 0)
						canDeleteStatus = 1;
				}
				else
				{
					if (canDeleteStatus == 1)
						canDeleteStatus = 2;
				}
			}
		}

		if (hasDetectMail)
			DBManager.getInstance().getDetectMailInfo();

		ChannelManager.getInstance().calulateAllChannelUnreadNum();

		if (canDeleteStatus == 1 || canDeleteStatus == 2) // 只能删一部分
		{
			adapter.notifyDataSetChangedOnUI();
		}

		if (channel != null && channel.mailDataList.size() == 0)
		{
			ChannelManager.getInstance().deleteChannel(channel);
		}

		if (StringUtils.isNotEmpty(uids))
		{
			JniController.getInstance().excuteJNIVoidMethod("deleteMutiMail", new Object[] { uids, "" });
		}
	}

	protected void actualRewardSysMails(List<ChannelListItem> sysMails)
	{
		System.out.println("actualRewardSysMails:" + sysMails.size());

		String uids = "";

		for (int i = 0; i < sysMails.size(); i++)
		{
			MailData mailData = (MailData) sysMails.get(i);
			if (mailData != null && mailData.channel != null && mailData.channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
			{
				System.out.println("actualRewardSysMails 2" + mailData.getRewardStatus());
				if (mailData.hasReward())
				{
					uids = appendStr(uids, mailData.getUid());
				}
			}
		}

		if (!(uids.equals("")))
		{
			JniController.getInstance().excuteJNIVoidMethod("rewardMutiMail", new Object[] { uids, "" });
		}
	}

	public static String getUidsByArray(List<String> uidArray)
	{
		String uids = "";
		for (int i = 0; i < uidArray.size(); i++)
		{
			String uid = uidArray.get(i);
			if (!uid.equals("") && !uids.contains(uid))
			{
				if (uids.equals(""))
					uids = uid;
				else
					uids += "," + uid;
			}
		}
		return uids;
	}

	protected void onListItemClick(AdapterView<?> adapterView, View view, int arg2)
	{
		ChannelListItem item = (ChannelListItem) adapterView.getItemAtPosition(arg2);
		if (item == null)
		{
			return;
		}
		if (isInEditMode)
		{
			CheckBox checkbox = (CheckBox) view.findViewById(R.id.channel_checkBox);
			item.checked = !item.checked;
			checkbox.setChecked(item.checked);
			return;
		}

		if (ChatServiceController.getInstance().isInDummyHost())
		{
			if (ChatServiceController.isNewMailUIEnable)
			{
				openItem((ChannelListItem) adapterView.getItemAtPosition(arg2));
			}
			else
			{
				ApplicationItem item2 = (ApplicationItem) adapterView.getItemAtPosition(arg2);
				openDummyChannel(item2, arg2);
			}
		}
		else
		{
			openItem(item);
		}
	}

	protected void openItem(ChannelListItem item)
	{
	}

	private ViewTreeObserver.OnGlobalLayoutListener	onGlobalLayoutListener;

	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	public void onDestroy()
	{
		if (adapter != null)
		{
			adapter.destroy();
			adapter = null;
		}

		if (mListView != null)
		{
			mListView.clearAdapter();
			mListView.setMenuCreator(null);
			mListView.setOnItemClickListener(null);
			mListView.setOnMenuItemClickListener(null);
			mListView = null;
		}

		if (channelListPullView != null)
		{
			channelListPullView.setOnRefreshListener(null);
			channelListPullView = null;
		}

		if (mailButtonBarAll != null)
		{
			mailButtonBarAll.setOnClickListener(null);
			mailButtonBarAll = null;
		}
		if (mailButtonBarDelete != null)
		{
			mailButtonBarDelete.setOnClickListener(null);
			mailButtonBarDelete = null;
		}
		if (mailButtonBarWrite != null)
		{
			mailButtonBarWrite.setOnClickListener(null);
			mailButtonBarWrite = null;
		}
		if (checkboxLabel != null)
		{
			checkboxLabel.setOnClickListener(null);
			checkboxLabel = null;
		}

		mailButtonBarLayout = null;

		if (getTitleLabel() != null)
		{
			getTitleLabel().setOnClickListener(null);
		}
		if (getEditButton() != null)
		{
			getEditButton().setOnClickListener(null);
		}
		if (getReturnButton() != null)
		{
			getReturnButton().setOnClickListener(null);
		}

		if (activity != null && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
			if (content != null && content.getChildAt(0) != null && content.getChildAt(0).getViewTreeObserver() != null)
			{
				content.getChildAt(0).getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
			}
		}
		onGlobalLayoutListener = null;

		if (getActivity() != null)
		{
			((ChannelListActivity) getActivity()).fragment = null;
		}

		super.onDestroy();
	}
}
