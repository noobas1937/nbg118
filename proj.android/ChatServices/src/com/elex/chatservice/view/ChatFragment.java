package com.elex.chatservice.view;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.MeasureSpec;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChannelView;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;
import com.elex.chatservice.view.listview.ListViewLoadListener;
import com.elex.chatservice.view.listview.PullDownToLoadMoreView;
import com.nineoldandroids.view.ViewHelper;

public class ChatFragment extends ActionBarFragment
{
	private RelativeLayout messagesListFrameLayout;
	private FrameLayout noAllianceFrameLayout;
	private RelativeLayout relativeLayout1;
	private LinearLayout buttonsLinearLayout;
	private EditText replyField;
	private RelativeLayout messageBox;
	private LinearLayout header;
	private MenuItem attachScreenshotMenu;
	private TextView wordCount;
	private Button addReply;
	private Button buttonCountry;
	private Button buttonAlliance;
	private ArrayList<Button> channelButton;
	private ImageView imageView1;
	private ImageView imageView2;
	private Button buttonJoinAlliance; // 加入联盟按钮
	private TextView noAllianceTipText;
	private Timer mTimer;
	private TimerTask mTimerTask;
	private CheckBox horn_checkbox;
	private LinearLayout horn_tip_layout;
	private TextView horn_text_tip;
	private LinearLayout tooltipLayout;
	private TextView tooltipLabel;
	private ImageView tooltipArrow;

	private int oldAdapterCount = 0;
	private int loadMoreCount = 0;

	protected int loadingHeaderHeight;
	private boolean isKeyBoardFirstShowed=false;
	private int curMaxInputLength=500;

	public static boolean rememberPosition = false;

	private static String savedText = "";
	private boolean isJoinAlliancePopupShowing = false;
	public static String gmailAccount = "";

	public boolean isKeyBoradShowing = false;
	public boolean isKeyBoradChange = false;
	
	private boolean isSelectMemberBtnEnable=false;

	public boolean isSelectMemberBtnEnable() {
		return isSelectMemberBtnEnable;
	}

	public ChannelView getCurrentChannel()
	{
		return getCurrentChannelView();
	}

	public ChatFragment()
	{
		isKeyBoardFirstShowed=false;
		System.out.println("ChatFragment");
		initChannelViews();
	}
	
	public void refreshMemberSelectBtn()
	{
		System.out.println("refreshMemberSelectBtn");
		if(!(ChatServiceController.isInMailDialog() && !ChatServiceController.isCreateChatRoom)) return;
		
		try
		{
			if(ChatServiceController.isInChatRoom() && !ChannelManager.getInstance().getIsMemberFlag(UserManager.getInstance().getCurrentMail().opponentUid))
			{
				System.out.println("isNot Member");
				isSelectMemberBtnEnable=false;
				return;
			}
			ArrayList<String> memberUidArray=UserManager.getInstance().getSelectMemberUidArr();
			if(memberUidArray==null)
			{
				isSelectMemberBtnEnable=false;
				return;
			}
//			for(int i=0;i<memberUidArray.size();i++)
//			{
//				System.out.println("ChatFragment*****");
//				System.out.println("memberUidArray "+i+":"+memberUidArray.get(i));
//				System.out.println("ChatFragment*****");
//			}
			
			HashMap<String, UserInfo> memberInfoMap=UserManager.getInstance().getChatRoomMemberInfoMap();
			isSelectMemberBtnEnable=true;
			if(memberUidArray==null || memberUidArray.size()<=0 || 
					(memberUidArray!=null && memberUidArray.size()>0&& !memberUidArray.contains(UserManager.getInstance().getCurrentUserId())))
				isSelectMemberBtnEnable=false;
//			if(memberInfoMap!=null && memberInfoMap.size()>0 && memberUidArray!=null && memberUidArray.size()>0)
//			{
//				Set<String> uidKeySet=memberInfoMap.keySet();
//				if(uidKeySet!=null && uidKeySet.size()>0)
//				{
//					System.out.println("keySet:");
//					// 可能出异常ConcurrentModificationException：java.util.HashMap$KeyIterator.next (HashMap.java:814)
//					for(String uid:uidKeySet)
//					{
//						System.out.println("uid:"+uid);
//					}
//					isSelectMemberBtnEnable=true;
//					for(int i=0;i<memberUidArray.size();i++)
//					{
//						String uid=memberUidArray.get(i);
//						System.out.println("isSelectMemberBtnEnable=true "+i+":"+memberUidArray.get(i));
//						if(!uidKeySet.contains(uid))
//						{
//							isSelectMemberBtnEnable=false;
//						}
//					}
//					
//					if(!ChatActivity.isInChatRoom() && !uidKeySet.contains(UserManager.getInstance().getCurrentMail().opponentUid))
//						isSelectMemberBtnEnable=false;
//				}
//			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	public void setSelectMemberBtnState()
	{
		if(getMemberSelectButton() != null){
			getMemberSelectButton().setVisibility(isSelectMemberBtnEnable?View.VISIBLE:View.GONE);
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
	}

	private void showMessageBox()
	{
		this.messageBox.setVisibility(View.VISIBLE);
		this.header.setVisibility(View.VISIBLE);
		refreshWordCount();

		if (this.attachScreenshotMenu != null)
		{
			this.attachScreenshotMenu.setVisible(true);
		}
	}

	public void saveState()
	{
//    	System.out.println("ChatFragment.saveState()");
		for (int i = 0; i < getChannelViewCount(); i++)
		{
			ChannelView channelView = getChannelView(i);
			if(channelView!=null)
			{
				ChatChannel channel = channelView.chatChannel;
				if(channel != null && channelView.messagesListView != null)
				{
					channel.lastPosition.x = channelView.messagesListView.getFirstVisiblePosition();
					View v = channelView.messagesListView.getChildAt(0);
					channel.lastPosition.y = (v == null) ? 0 : (v.getTop() - channelView.messagesListView.getPaddingTop());
				}
			}
		}
		savedText = replyField.getText().toString();
	}

	private boolean isJustCreated = true;

	public void checkFirstGlobalLayout()
	{
//		System.out.println("checkFirstGlobalLayout");
		if (isJustCreated)
		{
			isJustCreated = false;
			refreshTab();
		}
//		System.out.println("checkFirstGlobalLayout 1");

		if (oldChatFragmentHeight == -1 && computeUsableHeight() > 0)
		{
			oldChatFragmentHeight = computeUsableHeight();
		}
		else if (oldChatFragmentHeight > computeUsableHeight())
		{
			oldChatFragmentHeight = computeUsableHeight();
			if (isKeyBoardFirstShowed) isKeyBoradShowing = true;
			if (!rememberPosition)
			{
				gotoLastLine();
			}
			else
			{
				rememberPosition = false;
			}
		}
		else if (oldChatFragmentHeight == computeUsableHeight())
		{
			if (isKeyBoradChange)
			{
				keyBoardChangeCount++;
			}
			if (keyBoardChangeCount == 2)
			{
				isKeyBoradChange = false;
			}
		}
		else if (oldChatFragmentHeight < computeUsableHeight())
		{
			keyBoardChangeCount = 0;
			isKeyBoradChange = true;
			oldChatFragmentHeight = computeUsableHeight();
			isKeyBoradShowing = false;
			isKeyBoardFirstShowed=true;
		}
//		System.out.println("checkFirstGlobalLayout 2");

		int usableHeightNow = computeUsableHeight();

		if (usableHeight == -1 && usableHeightNow > 0)
		{
			usableHeight = usableHeightNow;
		}

		if (usableHeight != -1 && usableHeight > usableHeightNow)
		{
			if (!isSystemBarResized)
			{
				isSystemBarResized = true;
				return;
			}
			for (int i = 0; i < getChannelViewCount(); i++)
			{
				if (getChannelView(i).chatChannel != null && getChannelView(i).chatChannel.lastPosition.x == -1
						&& getChannelView(i).messagesListView != null && getChannelView(i).getMessagesAdapter() != null)
				{
					getChannelView(i).messagesListView.setSelection(getChannelView(i).getMessagesAdapter().getCount() - 1);
				}
			}
			usableHeight = usableHeightNow;
		}
//		System.out.println("checkFirstGlobalLayout 3");
	}

	private int keyBoardChangeCount = 0;
	private int oldChatFragmentHeight = -1;
	private boolean isSystemBarResized = false;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = ((ChatActivity) getActivity());

		return inflater.inflate(ResUtil.getId(this, "layout", "cs__messages_fragment"), container, false);
	}

	private FrameLayout.LayoutParams getLayoutParams()
	{
		FrameLayout.LayoutParams param = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		param.gravity = Gravity.CENTER;
		return param;
	}

	public void refreshIsInLastScreen(int channelType)
	{
		if (isSameChannel(channelType))
		{
			try
			{
				inLastScreen = isInLastScreen();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}

	}

	private boolean isInLastScreen()
	{
		// messagesListView存在时messagesListView.getChildAt(0)也可能为0
		if (getCurrentChannel() == null || getCurrentChannel().getMessagesAdapter() == null
				|| getCurrentChannel().getMessagesAdapter().getCount() == 0 || getCurrentChannel().messagesListView == null)
		{
			return true;
		}
		// 遍历从view.getFirstVisiblePosition()可见高度及到最下方的各个item的高度，计算这高度和是否小于一定的值（1.6屏）
		View v = getCurrentChannel().messagesListView.getChildAt(0);
		if (v == null)
		{
			return true;
		}

		int firstOffset = v.getTop() - getCurrentChannel().messagesListView.getPaddingTop();// 第一个item被上方盖住的部分

		int totalHeight = v.getHeight() + firstOffset;
		if ((getCurrentChannel().getMessagesAdapter().getCount() - getCurrentChannel().messagesListView.getFirstVisiblePosition()) > 20)
		{
			return false;
		}

		for (int i = (getCurrentChannel().messagesListView.getFirstVisiblePosition() + 1); i < getCurrentChannel().getMessagesAdapter()
				.getCount(); i++)
		{
			View listItem = getCurrentChannel().getMessagesAdapter().getView(i, null, getCurrentChannel().messagesListView);
			listItem.measure(MeasureSpec.makeMeasureSpec(getCurrentChannel().messagesListView.getWidth(), MeasureSpec.EXACTLY),
					MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));
			int h = listItem.getMeasuredHeight();
			totalHeight += h + getCurrentChannel().messagesListView.getDividerHeight();
		}

		if (totalHeight <= (getCurrentChannel().messagesListView.getHeight() * 1.75))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	boolean inLastScreen = false;

	// 处理接收消息
	public void updateListPositionForNewMsg(int channelType, boolean isSelfMsg)
	{
		if (!isSameChannel(channelType) || getCurrentChannel().messagesListView == null
				|| getCurrentChannel().getMessagesAdapter() == null)
		{
			return;
		}

		if (!isSelfMsg)
		{
			if (isKeyBoradShowing)
			{
				gotoLastLine();
			}
			else if (inLastScreen)
			{
				gotoLastLine();
			}
		}
		inLastScreen = false;
	}

	public void updateListPositionForOldMsg(int channelType, int loadCount)
	{
		final ListView listView = getCurrentChannel().messagesListView;
		if (!isSameChannel(channelType) || listView == null || getCurrentChannel().getMessagesAdapter() == null)
		{
			return;
		}
		loadMoreCount = loadCount;

		if(activity == null) return;
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if(!getCurrentChannel().chatChannel.isLoadingAllNew){
						listView.setSelectionFromTop(loadMoreCount, getCurrentChannel().pullDownToLoadListView.getPullDownHeight());
					}else{
						listView.setSelectionFromTop(0, 0);
					}
					refreshToolTip();
					getCurrentChannel().pullDownToLoadListView.hideProgressBar();
					stopTimerTask();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}
	
	public void changeChatRoomName(String name)
	{
		if(!name.equals("")){
			getTitleLabel().setText(name);
		}
	}

	public void notifyDataSetChanged(int channelType)
	{
		try
		{
			MessagesAdapter adapter = getChannelView(ChannelManager.channelType2tab(channelType)).getMessagesAdapter();
			if (adapter != null)
			{
				adapter.notifyDataSetChanged();
			}
			this.refreshHasMoreData();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	// 可能和notifyDataSetChanged差不了太多
	public void refreshListItem(MsgItem msgItem)
	{
		try
		{
			MessagesAdapter adapter = getChannelView(ChannelManager.channelType2tab(msgItem.channelType)).getMessagesAdapter();
			final ListView list = getChannelView(ChannelManager.channelType2tab(msgItem.channelType)).messagesListView;
			if (adapter != null && list != null)
			{
				int start = list.getFirstVisiblePosition();
				for(int i=start, j=list.getLastVisiblePosition();i<=j;i++)
			    if(msgItem==list.getItemAtPosition(i)){
			        View view = list.getChildAt(i-start);
			        list.getAdapter().getView(i, view, list);
//			        list.invalidateViews();
			        activity.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                        	// 会导致可见的item全部重新调用setConvertView
                        	list.invalidateViews();
                        }
                    });
			        break;
			    }
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public void afterSendMsgShowed(int channelType)
	{
		ListView listView = getCurrentChannel().messagesListView;
		MessagesAdapter adapter = getCurrentChannel().getMessagesAdapter();

		if (listView != null && adapter != null && isSameChannel(channelType))
		{
			gotoLastLine();
		}
	}

	public void resetMoreDataStart(int channelType)
	{
		if (isSameChannel(channelType))
		{
			getCurrentChannel().setLoadingStart(false);
		}
	}

	private void gotoLastLine()
	{
		if(activity == null) return;
		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (getCurrentChannel() != null && getCurrentChannel().messagesListView != null
							&& getCurrentChannel().getMessagesAdapter() != null)
					{
						getCurrentChannel().messagesListView.setAdapter(getCurrentChannel().getMessagesAdapter());
						getCurrentChannel().messagesListView.setSelection(getCurrentChannel().getMessagesAdapter().getCount() - 1);
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	private boolean isSameChannel(int channelType)
	{
		if(getCurrentChannel() == null) return false;
		return getCurrentChannel().channelType == channelType;
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		System.out.println("ChatFragment onViewCreated");
		super.onViewCreated(view, savedInstanceState);
		refreshMemberSelectBtn();
		this.noAllianceFrameLayout = (FrameLayout) view.findViewById(R.id.hs__noAllianceLayout);
		this.relativeLayout1 = (RelativeLayout) view.findViewById(R.id.relativeLayout1);
		this.buttonsLinearLayout = (LinearLayout) view.findViewById(R.id.buttonsLinearLayout);
		this.messagesListFrameLayout = (RelativeLayout) view.findViewById(R.id.hs__messagesListLayout);
		imageView1 = (ImageView) view.findViewById(R.id.imageView1);
		imageView2 = (ImageView) view.findViewById(R.id.imageView2);
		horn_checkbox=(CheckBox) view.findViewById(R.id.horn_checkbox);
		horn_tip_layout=(LinearLayout)view.findViewById(R.id.horn_tip_layout);
		horn_text_tip=(TextView)view.findViewById(R.id.horn_text_tip);
		horn_text_tip.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_HORN_TEXT));
		
		if(!lazyLoading) renderList();
		
		this.replyField = ((EditText) view.findViewById(ResUtil.getId(this.activity, "id", "hs__messageText")));
		this.wordCount = ((TextView) view.findViewById(ResUtil.getId(this.activity, "id", "wordCountTextView")));
		if (!ChatServiceController.isInMailDialog())
		{
			getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TITLE_CHAT));
		}
		else
		{
//			ArrayList<String> memberUidArray=UserManager.getInstance().getSelectMemberUidArr();
			String title=UserManager.getInstance().getCurrentMail().opponentName;
			if(ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_USER)
			{
				String fromUid = ChannelManager.getInstance().getModChannelFromUid(UserManager.getInstance().getCurrentMail().opponentUid);
				if(StringUtils.isNotEmpty(fromUid))
				{
					if(fromUid.equals(UserManager.getInstance().getCurrentUserId()))
					{
						title = LanguageManager.getLangByKey(LanguageKeys.TIP_ALLIANCE);
					}
					else
					{
						UserInfo fromUser=UserManager.getInstance().getUser(fromUid);
						if(fromUser!=null && StringUtils.isNotEmpty(fromUser.userName))
						{
							title = fromUser.userName;
						}
					}
				}
				
			}
			
			if(ChatServiceController.isInChatRoom() && title.length()>16)
			{
				title=LanguageManager.getLangByKey(LanguageKeys.TITLE_CHATROOM);
				if(title.equals(""))
					title="Group";
			}
			if(UserManager.getInstance().getCurrentMail().opponentUid.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
				title += "(MOD)";
			getTitleLabel().setText(title);
		}

		replyField.setText(savedText);

		addReply = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "hs__sendMessageBtn"));
		addReply.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEND));

		this.messageBox = ((RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "relativeLayout1")));
		header = ((LinearLayout) view.findViewById(ResUtil.getId(this.activity, "id", "headerRelativeLayout")));

		getMemberSelectButton().setVisibility(isSelectMemberBtnEnable?View.VISIBLE:View.GONE);

		buttonCountry = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "buttonCountry"));
		buttonAlliance = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "buttonAllie"));
		if(ChatServiceController.isInCrossFightServer()){
			buttonCountry.setText(LanguageManager.getLangByKey(LanguageKeys.BATTLE_FIELD));
		}else{
			buttonCountry.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_COUNTRY));
		}
		buttonAlliance.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_ALLIANCE));
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonCountry, true);
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, false);

		tooltipLayout = ((LinearLayout) view.findViewById(ResUtil.getId(this.activity, "id", "tooltipLayout")));
		tooltipLabel = ((TextView) view.findViewById(ResUtil.getId(this.activity, "id", "tooltipLabel")));
		tooltipArrow = ((ImageView) view.findViewById(ResUtil.getId(this.activity, "id", "tooltipArrow")));
		showToolTip(false);
		tooltipLayout.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				onClickToolTip();
			}
		});

		channelButton = new ArrayList<Button>();
		channelButton.add(buttonCountry);
		channelButton.add(buttonAlliance);
		
		buttonJoinAlliance = (Button) view.findViewById(ResUtil.getId(this.activity, "id", "joinAllianceBtn"));
		// guojiang
//		buttonJoinAlliance.setVisibility(Button.INVISIBLE);
		buttonJoinAlliance.setText(LanguageManager.getLangByKey(LanguageKeys.MENU_JOIN));
			
		buttonJoinAlliance.setOnClickListener(new View.OnClickListener()
		{

			@Override
			public void onClick(View v)
			{			
				ChatServiceController.doHostAction("joinAllianceBtnClick", "", "", "", true);
			}
		});

		noAllianceTipText = ((TextView) view.findViewById(ResUtil.getId(this.activity, "id", "joinAllianceTipText")));
		noAllianceTipText.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_JOIN_ALLIANCE));

		refreshSendButton();

		for (int i = 0; i < channelButton.size(); i++)
		{
			channelButton.get(i).setTag(getChannelView(i));
			final int index = i;
			channelButton.get(i).setOnClickListener(new View.OnClickListener()
			{
				@Override
				public void onClick(View view)
				{
					ChannelView channel = ((ChannelView) view.getTag());

					channel.setLoadingStart(false);
					
					showTab(channel.tab);
					
					if (channel.tab == TAB_COUNTRY)
					{
						ChatServiceController.getInstance().host.postCurChannel(DBDefinition.CHANNEL_TYPE_COUNTRY);
					}
					else if (channel.tab == TAB_ALLIANCE)
					{
						ChatServiceController.getInstance().host.postCurChannel(DBDefinition.CHANNEL_TYPE_ALLIANCE);
					}
					if (channel.tab == TAB_ALLIANCE && UserManager.getInstance().getCurrentUser().allianceId.equals(""))
					{
						return;
					}

					getChannelView(0).isFirstVisit =!ChannelManager.getInstance().getCountryChannel().hasRequestDataBefore;
					if(UserManager.getInstance().isCurrentUserInAlliance()){
						getChannelView(1).isFirstVisit =!ChannelManager.getInstance().getAllianceChannel().hasRequestDataBefore;
					}
					
//					if (channel.isFirstVisit)
//					{
//						if (getCurrentChannel().tab == TAB_COUNTRY)
//						{
//							ChatServiceController.getInstance().host.requestChatMsg(0);
//						}
//						else if (getCurrentChannel().tab == TAB_ALLIANCE)
//						{
//							requestMsgRecord(DBDefinition.CHANNEL_TYPE_ALLIANCE);
//						}
//
//						channel.isFirstVisit = false;
//					}
//					else if (getCurrentChannel().tab == TAB_ALLIANCE && ChannelManager.getInstance().getCurMsgListByIndex(DBDefinition.CHANNEL_TYPE_ALLIANCE).size()<=0)
//					{
//						requestMsgRecord(DBDefinition.CHANNEL_TYPE_ALLIANCE);
//					}
				}
			});
		}

		getMemberSelectButton().setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				hideSoftKeyBoard();
				ServiceInterface.showMemberSelectorActivity(activity, true);
//				activity.showFragment(MemberSelectorFragment.class.getName());
				
//				ChatServiceController.showGameActivity(activity);
			}
		});

		addReply.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				String replyText = replyField.getText().toString().trim();
				// String identity = hsStorage.getIdentity();
				if (!TextUtils.isEmpty(replyText))
				{
					if(!horn_checkbox.isChecked())
					{
						ChatServiceController.sendMsg(replyText,false,false);
					}
					else
					{
						if(ChatServiceController.getInstance().host.getHornBanedTime()==0)
						{
							int price=ChatServiceController.getInstance().host.isHornEnough();
							String horn=LanguageManager.getLangByKey(LanguageKeys.TIP_HORN);
							if(price==0)
							{
								if(ConfigManager.isFirstUserHorn)
									MenuController.showSendHornMessageConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_USEITEM, horn), replyText);
								else
									ChatServiceController.sendMsg(replyText, true,false);
							}
							else if(price>0)
							{
								if(ConfigManager.isFirstUserCornForHorn)
									MenuController.showSendHornWithCornConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_ITEM_NOT_ENOUGH, horn), replyText, price);
								else
								{
									if(ChatServiceController.getInstance().host.isCornEnough(price))
									{
										ChatServiceController.sendMsg(replyText, true,true);
									}
									else
									{
										MenuController.showCornNotEnoughConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_CORN_NOT_ENOUGH));
									}
								}
							}
						}
					}
				}
			}
		});


		this.replyField.setOnEditorActionListener(new TextView.OnEditorActionListener()
		{
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
			{
				if (actionId == 4)
				{
					addReply.performClick();
				}
				return false;
			}
		});
		textChangedListener = new TextWatcher()
		{
			@Override
			public void afterTextChanged(Editable s)
			{
				replyField.post(new Runnable()
				{
					@Override
					public void run()
					{
						refreshWordCount();
					}
				});
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after)
			{
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count)
			{
				refreshSendButton();
			}
		};
		this.replyField.addTextChangedListener(textChangedListener);

		showMessageBox();

		horn_checkbox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//				System.out.println("horn_checkbox isChecked:"+isChecked);
				refreshBottomUI(isChecked);
				if(isChecked)
					ConfigManager.isHornBtnEnable = true;
				else
					ConfigManager.isHornBtnEnable = false;
//				gotoLastLine();
			}
		});

		onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener()
		{
			@Override
			public void onGlobalLayout()
			{
				checkFirstGlobalLayout();
				adjustHeight();
			}
		};
		messagesListFrameLayout.getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);
		((ChatActivity) getActivity()).fragment = this;
		
		if(!lazyLoading){
			refreshToolTip();
			refreshHasMoreData();
		}
	}

	private boolean lazyLoading = true;
	
	protected void onBecomeVisible() {
		if(inited) return;
		
		timerDelay = 500;
		startTimer();
	}

	public static final int			CHANNEL_COUNT	= 3;
	private int						currentChannelViewIndex;
	private ArrayList<ChannelView>	channelViews;
	private void initChannelViews()
	{
		channelViews = new ArrayList<ChannelView>();
		for (int i = 0; i < CHANNEL_COUNT; i++)
		{
			ChannelView channelView = new ChannelView();

			channelView.tab = i;
//			channelView.channelType = i;

			channelViews.add(channelView);
		}
	}
	public int getChannelViewCount()
	{
		return CHANNEL_COUNT;
	}
	
	public ChannelView getChannelView(int index)
	{
		return channelViews.get(index);
	}

	public void setChannelViewIndex(int i)
	{
		if (i >= 0 && i < channelViews.size())
		{
			currentChannelViewIndex = i;
		}
	}

	public ChannelView getCurrentChannelView()
	{
		try
		{
			if (channelViews == null) return null;
			return channelViews.get(currentChannelViewIndex);
		}
		catch (Exception e)
		{
			return null;
		}
	}

	public ChannelView getCountryChannelView()
	{
		try
		{
			return getChannelView(0);
		}
		catch (Exception e)
		{
			return null;
		}
	}
	
	public void resetChannelView()
	{
		for (int i = 0; i < CHANNEL_COUNT; i++)
		{
			if (getChannelView(i).chatChannel != null)
			{
				getChannelView(i).chatChannel.setChannelView(null);
			}
			getChannelView(i).init();
		}
	}
	
	protected void renderList()
	{
		System.out.println("renderList: getChannelViewCount = " + getChannelViewCount());
		for (int i = 0; i < getChannelViewCount(); i++)
		{
			final ChannelView channelView = getChannelView(i);
			ChatChannel chatChannel = null;
			if(i < 2){
				chatChannel = ChannelManager.getInstance().getChannel(i, "");
				channelView.channelType = i;
			}else if(ChatServiceController.isInMailDialog()){
				String channelId = UserManager.getInstance().getCurrentMail().opponentUid;
				if(ChatServiceController.isContactMod && !channelId.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
					channelId += DBDefinition.CHANNEL_ID_POSTFIX_MOD;
				chatChannel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType(), channelId);
				channelView.channelType = ChatServiceController.getCurrentChannelType();
				System.out.println("onViewCreated: i = " + i + " channelType = " + ChatServiceController.getCurrentChannelType() + " channelId = "
						+ channelId + " chatChannel = " + chatChannel);
			}else{
				continue;
			}
			if(chatChannel != null){
				System.out.println("chatChannel = " + chatChannel + " chatChannel.msgList.size() = " + chatChannel.msgList.size());
				chatChannel.clearFirstNewMsg();
				chatChannel.setChannelView(channelView);
			}
			channelView.chatChannel = chatChannel;
			
			PullDownToLoadMoreView pullDownToLoadListView=new PullDownToLoadMoreView(activity);
			pullDownToLoadListView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

			pullDownToLoadListView.setTopViewInitialize(true);
			pullDownToLoadListView.setAllowPullDownRefersh(false);
			pullDownToLoadListView.setBottomViewWithoutScroll(false);
			pullDownToLoadListView.setListViewLoadListener(mListViewLoadListener);
			
			ListView messagesListView=new ListView(activity);
			messagesListView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
			messagesListView.setVerticalFadingEdgeEnabled(false);
			messagesListView.setCacheColorHint(Color.TRANSPARENT);
			messagesListView.setDivider(null);
			if(chatChannel != null && chatChannel.msgList != null){
				// this.getActivity() 可能为null
				channelView.setMessagesAdapter(new MessagesAdapter(this, 17367043, chatChannel.msgList));
			}else{
				channelView.setMessagesAdapter(new MessagesAdapter(this, 17367043, new ArrayList<MsgItem>()));
			}
			messagesListView.setAdapter(channelView.getMessagesAdapter());

			messagesListView.setOnScrollListener(mOnScrollListener);
			messagesListView.setTranscriptMode(AbsListView.TRANSCRIPT_MODE_DISABLED);
			messagesListView.setKeepScreenOn(true);
			
			pullDownToLoadListView.addView(messagesListView);

			channelView.pullDownToLoadListView = pullDownToLoadListView;
			channelView.messagesListView = messagesListView;
			messagesListView.post(new Runnable() {
				@Override
				public void run() {
					if(channelView != null && channelView.messagesListView != null){
						// 可能出NullPointerException异常
//						channelView.messagesListView.setSelection(channelView.messagesListView.getCount());
					}
				}
			});
			
			if (chatChannel != null)
			{
//				System.out.println(i + " chatChannel.lastPosition:" + chatChannel.lastPosition + " " + rememberPosition);
				if (chatChannel.lastPosition.x != -1 && rememberPosition)
				{
//					System.out.println("恢复位置");
					channelView.messagesListView.setSelectionFromTop(chatChannel.lastPosition.x, chatChannel.lastPosition.y);
				}
				else
				{
					channelView.messagesListView.setSelection(channelView.getMessagesAdapter().getCount() - 1);
				}
			}
			
			messagesListFrameLayout.addView(pullDownToLoadListView);
		}
		if(lazyLoading){
			refreshTab();
		}
		activity.hideProgressBar();
	}
	
	private void refreshTab()
	{
		if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			showTab(TAB_ALLIANCE);
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			showTab(TAB_COUNTRY);
		}
		else if (ChatServiceController.isInMailDialog())
		{
			showTab(TAB_MAIL);
		}
		else
		{
			showTab(TAB_COUNTRY);
		}
		refreshWordCount();
	}

	private void refreshSendButton()
	{
		if (this.replyField.getText().length() == 0)
		{
			addReply.setEnabled(false);
			CompatibleApiUtil.getInstance().setButtonAlpha(addReply, false);
		}
		else
		{
			addReply.setEnabled(true);
			CompatibleApiUtil.getInstance().setButtonAlpha(addReply, true);
		}
	}

	public void showToolTip(boolean b)
	{
		tooltipLayout.setVisibility(b ? View.VISIBLE : View.GONE);
	}
	
	private void onClickToolTip()
	{
		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if(channel != null && channel.canLoadAllNew()){
			getCurrentChannel().setLoadingStart(true);
			oldAdapterCount = getCurrentChannel().getMessagesAdapter().getCount();
			loadMoreCount = 0;
			channel.isLoadingAllNew = true;
			channel.hasLoadingAllNew = true;
			ChannelManager.getInstance().loadAllNew(channel);

			refreshToolTip();
		}
	}
	
	public void refreshToolTip()
	{
		if(isInMail()) return;
		
		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
//		System.out.println("refreshToolTip() currentChatType:" + ChatActivity.currentChatType + " " + getCurrentChannel() + " " + channel);
		if(channel != null && channel.canLoadAllNew()){
			String newMsgCount = channel.getNewMsgCount() < ChannelManager.LOAD_ALL_MORE_MAX_COUNT ? channel.getNewMsgCount() + ""
					: ChannelManager.LOAD_ALL_MORE_MAX_COUNT + "+";
			tooltipLabel.setText(LanguageManager.getLangByKey(LanguageKeys.NEW_MESSAGE_ALERT, newMsgCount));
			showToolTip(true);
		}else{
			showToolTip(false);
		}
		
//		if(ChannelManager.getInstance().isGetingNewMsg){
//			tooltipLabel.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_LOADING));
//			showToolTip(true);
//		}else{
//		}
	}

	public void clearInput()
	{
		replyField.setText("");
	}

	private void refreshBottomUI(boolean isChecked)
	{
		if(!isChecked)
			ChatServiceController.isHornItemUsed =false;
		boolean isHornUI = isChecked && ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_COUNTRY
				&& ConfigManager.enableChatHorn;
//		System.out.println("isHornUI:"+isHornUI);
		String background=isHornUI?"btn_green3":"btn_yellow";
		String bottomBg=isHornUI?"bottom_bg":"chuzheng_frame02";
		String inputBg=isHornUI?"input":"text_field_bg2";
		addReply.setBackgroundResource(ResUtil.getId(activity, "drawable", background));
		relativeLayout1.setBackgroundResource(ResUtil.getId(activity, "drawable", bottomBg));
		replyField.setBackgroundResource(ResUtil.getId(activity, "drawable", inputBg));
		horn_tip_layout.setVisibility(isHornUI?View.VISIBLE:View.GONE);
		imageView1.setVisibility(isHornUI?View.GONE:View.VISIBLE);
		setMaxInputLength(isHornUI);
	}
	
	private void setMaxInputLength(boolean isHornUI)
	{
		curMaxInputLength = isHornUI && ConfigManager.maxHornInputLength > 0 ? ConfigManager.maxHornInputLength : 500;
		replyField.setFilters(new InputFilter[] { new InputFilter.LengthFilter(curMaxInputLength) });
	}

	@Override
	public void onStart()
	{
		super.onStart();
	}

	@SuppressLint("ClickableViewAccessibility")
	private void showJoinAlliancePopup()
	{
		final AlertDialog dlg = new AlertDialog.Builder(activity).create();
		dlg.setCancelable(true);
		dlg.setCanceledOnTouchOutside(true);
		dlg.show();
		isJoinAlliancePopupShowing = true;
		Window window = dlg.getWindow();
		window.setBackgroundDrawable(new ColorDrawable());
		window.setContentView(R.layout.cs__first_alliance_popup);
		window.setLayout(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);

		ImageView bgImageView2 = (ImageView) window.findViewById(R.id.bg2);
		ViewHelper.setRotationY(bgImageView2, 180);

		TextView joinAllianceTipText = (TextView) window.findViewById(R.id.joinAllianceTipText);
		joinAllianceTipText.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_ADDALLIANCE_REWARD));

		TextView joinAllianceRewardText = (TextView) window.findViewById(R.id.joinAllianceRewardText);
		joinAllianceRewardText.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_ADDALLIANCE_COIN, "100"));

		TextView sendMethodText = (TextView) window.findViewById(R.id.sendMethodText);
		sendMethodText.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_ADDALLIANCE_REWARD_SENDBYMAIL));

		RelativeLayout firstJoinAllianceLayout = (RelativeLayout) window.findViewById(R.id.firstAllianceLayout);

		firstJoinAllianceLayout.setOnTouchListener(new OnTouchListener()
		{
			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				dlg.cancel();
				isJoinAlliancePopupShowing = false;
				return false;
			}
		});

		// 为加入联盟按钮添加事件
		Button joinAllianceBtn = (Button) window.findViewById(R.id.joinAllianceBtn);
		joinAllianceBtn.setText(LanguageManager.getLangByKey(LanguageKeys.MENU_JOIN));
		joinAllianceBtn.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				dlg.cancel();
				isJoinAlliancePopupShowing = false;

				ChatServiceController.doHostAction("joinAllianceBtnClick", "", "", "", true);
			}
		});

		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			ScaleUtil.adjustTextSize(joinAllianceTipText, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(joinAllianceRewardText, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(sendMethodText, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(joinAllianceBtn, ConfigManager.scaleRatio);
		}
	}
	
	public class LoadMoreMsgParam
	{
		public int requestMinSeqId;
		public int requestMaxSeqId;
		public boolean fetchFromServer;
		public int viewMinSeqId;
		public int desireMinSeqId;
		public int desireMaxSeqId;
		public int serverMinSeqId;
		public int serverMaxSeqId;
		public int requestCount;
		public Point intersection;

		public LoadMoreMsgParam(int minSeqId, int maxSeqId, boolean fetchFromServer, int viewMinSeqId, int desireMinSeqId,
				int desireMaxSeqId, int serverMinSeqId, int serverMaxSeqId, Point intersection)
		{
			this.requestMinSeqId = minSeqId;
			this.requestMaxSeqId = maxSeqId;
			this.fetchFromServer = fetchFromServer;

			this.viewMinSeqId = viewMinSeqId;
			this.desireMinSeqId = desireMinSeqId;
			this.desireMaxSeqId = desireMaxSeqId;
			this.serverMinSeqId = serverMinSeqId;
			this.serverMaxSeqId = serverMaxSeqId;
			this.intersection = intersection;
			this.requestCount = requestMaxSeqId - requestMinSeqId + 1;
		}
	}

	// 获取加载区间的逻辑，也是检查能否加载的逻辑
	private LoadMoreMsgParam getLoadMoreMsgParam(int channelType)
	{
		if (!(channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE 
				|| channelType == DBDefinition.CHANNEL_TYPE_CHATROOM))
		{
//			LogUtil.printStack();
			return null;
		}
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType);
		if (channel == null || channel.msgList == null || channel.getChannelView() == null)
		{
			return null;
		}

		int viewMinSeqId = channel.getChannelView().getViewMinSeqId();
//		System.out.println("getLoadMoreMsgParam() viewMinSeqId="+viewMinSeqId);
		// 不能加载: 没有消息时viewMinSeqId为0，有消息时seqId最小为1
		if(viewMinSeqId <= 1){
			return null;
		}

		// desireMaxSeqId可能等于desireMinSeqId，仅当二者都为1时
		int desireMaxSeqId = viewMinSeqId - 1;
		int desireMinSeqId = (desireMaxSeqId - 19) > 1 ? (desireMaxSeqId - 19) : 1;
//		System.out.println("getLoadMoreMsgParam() desireMaxSeqId="+desireMaxSeqId+" desireMinSeqId="+desireMinSeqId);
		
		// 如果desireMaxSeqId在本地db中有，就从db加载（不一定能满20条）
		if(DBManager.getInstance().isMsgExists(channel.getChatTable(), desireMaxSeqId)){
			return new LoadMoreMsgParam(desireMinSeqId, desireMaxSeqId, false, viewMinSeqId, desireMinSeqId,
			            				desireMaxSeqId, channel.serverMinSeqId, channel.serverMaxSeqId, null);
		}
		// 否则，如果在server范围内，从server加载
		// server中seqId连续，可以用交集判断
		Point inter = getIntersection(new Point(channel.serverMinSeqId, channel.serverMaxSeqId), new Point(desireMinSeqId,
				desireMaxSeqId));
		if (inter != null)
		{
			return new LoadMoreMsgParam(inter.x, inter.y, true, viewMinSeqId, desireMinSeqId,
			            				desireMaxSeqId, channel.serverMinSeqId, channel.serverMaxSeqId, inter);
		}
		
		return null;
	}
	
	/**
	 * 计算两段连续区间的交集
	 * @param sec1 [sec1.x, sec1.y]组成的区间
	 * @param sec2 [sec2.x, sec2.y]组成的区间
	 * @return null，如果无交集
	 */
	public static Point getIntersection(Point sec1, Point sec2)
	{
		int[] fourValue = {sec1.x, sec1.y, sec2.x, sec2.y};
		Arrays.sort(fourValue); // 升序排序
		int lower = -1;
		int upper = -1;
		for (int i = 0; i < fourValue.length; i++)
		{
			if(fourValue[i] >= sec1.x && fourValue[i] <= sec1.y && fourValue[i] >= sec2.x && fourValue[i] <= sec2.y){
				lower = fourValue[i];
				break;
			}
		}
		for (int i = fourValue.length - 1; i >= 0; i--)
		{
			if(fourValue[i] >= sec1.x && fourValue[i] <= sec1.y && fourValue[i] >= sec2.x && fourValue[i] <= sec2.y){
				upper = fourValue[i];
				break;
			}
		}
		if(lower != -1 && upper != -1){
			return new Point(lower, upper);
		}else{
			return null;
		}
	}

	private boolean checkMessagesAdapter()
	{
		if(getCurrentChannel() == null || getCurrentChannel().getMessagesAdapter() == null){
			LogUtil.trackMessage("checkMessagesAdapter() fail: currentChannel = " + getCurrentChannel()
					+ " messagesAdapter = " + (getCurrentChannel() == null ? "null" : getCurrentChannel().getMessagesAdapter())
					+ " currentChatType = " + ChatServiceController.getCurrentChannelType()
					+ " chatActivity = " + ChatServiceController.getChatActivity()
					+ " chatFragment = "+ ChatServiceController.getChatFragment(), "", "");
			return false;
		}
		return true;
	}
	
	private void loadMoreMsg()
	{
		if(!checkMessagesAdapter()) return;
		
		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		// 极少情况下会发生
		if(channel == null) return;
		LoadMoreMsgParam loadMoreMsgParam = getLoadMoreMsgParam(channel.channelType);
		
		if (!getCurrentChannel().getLoadingStart() && loadMoreMsgParam != null)
		{
			getCurrentChannel().setLoadingStart(true);
			oldAdapterCount = getCurrentChannel().getMessagesAdapter().getCount(); // 可能有异常 getCount() on a null object reference
			loadMoreCount = 0;
			channel.isLoadingAllNew = false;
			if(loadMoreMsgParam.fetchFromServer){
				System.out.println("从server加载消息");
//				System.out.println(LogUtil.typeToString(loadMoreMsgParam));
				ChatServiceController.getInstance().host.getMsgBySeqId(	loadMoreMsgParam.requestMinSeqId,
																		loadMoreMsgParam.requestMaxSeqId,
																		channel.channelType,
																		channel.channelID);
			}else{
				System.out.println("从db加载消息");
//				System.out.println(LogUtil.typeToString(loadMoreMsgParam));
				ChannelManager.getInstance().loadMoreFromDB(loadMoreMsgParam.requestMinSeqId, loadMoreMsgParam.requestMaxSeqId, channel);
			}
		}
		createTimerTask();
	}

	private void loadMoreMail()
	{
		if(!checkMessagesAdapter()) return;
		
		if(ChatServiceController.isNewMailListEnable){
			ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
			if (!getCurrentChannel().getLoadingStart() && hasMoreData())
			{
				ChannelManager.getInstance().loadMoreMailFromDB(channel, channel.getMinMailCreateTime());
			}
		}else{
			if (!getCurrentChannel().getLoadingStart() && hasMoreData())
			{
				getCurrentChannel().setLoadingStart(true);
				oldAdapterCount = getCurrentChannel().getMessagesAdapter().getCount();
				loadMoreCount = 0;
				ChatServiceController.getInstance().host.requestMoreMail(
								         UserManager.getInstance().getCurrentMail().opponentUid, 
								         UserManager.getInstance().getCurrentMail().mailUid, oldAdapterCount);
			}
		}
		
		createTimerTask();
	}
	
	/**
	 * 时机：各个参数变化时、初始化时
	 * server数据变化时：GetNewMsg返回时
	 * view数据变化时：获取到新消息时
	 */
	public void refreshHasMoreData()
	{
		if(!isInMail()){
			if(ChannelManager.getInstance().isGetingNewMsg){
				System.out.println("ChannelManager.getInstance().isGetingNewMsg");
				hasMoreData =  false;
			}else{
				hasMoreData =  getLoadMoreMsgParam(ChatServiceController.getCurrentChannelType()) != null;
			}
		}else if(ChatServiceController.isNewMailListEnable){
			ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
			if(channel == null){
				hasMoreData = false;
				return;
			}
			List<MsgItem> dbUserMails = DBManager.getInstance().getUserMailByTime(	channel.getChatTable(),
																					channel.getMinMailCreateTime(),
																					1);
			hasMoreData = dbUserMails.size() > 0;
		}
	}
	
	private boolean isInMail()
	{
		return getCurrentChannel().tab == TAB_MAIL && !ChatServiceController.isInChatRoom();
	}
	
	public boolean hasMoreData()
	{
//		System.out.println("hasMoreData()");
		if(isInMail()){
//			System.out.println("邮件中");
			if(ChatServiceController.isNewMailListEnable){
				return hasMoreData;
			}else{
				return !ChannelManager.getInstance().getNoMoreDataFlag(getCurrentChannel().tab);
			}
		}else{
			return hasMoreData;
//			if(ChannelManager.getInstance().isGetingNewMsg){
//				System.out.println("ChannelManager.getInstance().isGetingNewMsg");
//				return false;
//			}
//			return getLoadMoreMsgParam(ChatActivity.currentChatType) != null;
		}
	}
	
	private ListViewLoadListener mListViewLoadListener = new ListViewLoadListener()
	{
		@Override
		public void refreshData()
		{
			if(isInMail()){
				loadMoreMail();
			}else{
				loadMoreMsg();
			}
		}
		
		@Override
		public boolean getIsListViewToTop()
		{
			ListView listView=getCurrentChannel().messagesListView;
			if(listView==null)
				return false;

			View topListView =  listView.getChildAt(listView.getFirstVisiblePosition());
			if((topListView == null) || (topListView.getTop() != 0)){
				return false;
			}else{
				return true;
			}
		}

		@Override
		public boolean getIsListViewToBottom()
		{
			ListView listView=getCurrentChannel().messagesListView;
			if(listView==null)
				return false;
			View bottomView=listView.getChildAt(-1 + listView.getChildCount());
			if(bottomView==null)
				return false;
			if(bottomView.getBottom()> listView.getHeight()|| (listView.getLastVisiblePosition() != -1 + listView.getAdapter().getCount())){

				return false;
			}else{

				return true;
			}
		}
	};

	private void createTimerTask()
	{
		stopTimerTask();
		mTimer=new Timer();
		mTimerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				if(activity == null) return;
				activity.runOnUiThread(new Runnable()
				{
					@Override
					public void run()
					{
						try
						{
							if(getCurrentChannel().pullDownToLoadListView!=null)
								getCurrentChannel().pullDownToLoadListView.hideProgressBar();
						}
						catch (Exception e)
						{
							LogUtil.printException(e);
						}
					}
				});
				resetMoreDataStart(getCurrentChannel().tab);
			}
		};
		if(mTimer!=null)
			mTimer.schedule(mTimerTask,5000);
	}

	private void stopTimerTask()
	{
		if(mTimer!=null)
		{
			mTimer.cancel();
			mTimer.purge();
			mTimer=null;
		}
	}
	
	private boolean hasMoreData = true;
	
	private OnScrollListener mOnScrollListener = new AbsListView.OnScrollListener() {

		@Override
		public void onScrollStateChanged(AbsListView view, int scrollState) {
			if(scrollState == AbsListView.OnScrollListener.SCROLL_STATE_IDLE){
				if(getCurrentChannel().messagesListView!=null)
				{
					View topView = getCurrentChannel().messagesListView.getChildAt(getCurrentChannel().messagesListView.getFirstVisiblePosition());
					if ((topView != null) && (topView.getTop() == 0) && !getCurrentChannel().getLoadingStart()){
						getCurrentChannel().pullDownToLoadListView.startTopScroll();
					}
				}
			}
		}

		@Override
		public void onScroll(AbsListView view, int firstVisibleItem, int visibleItemCount, int totalItemCount) 
		{
			if (getCurrentChannel() != null
					&& getCurrentChannel().pullDownToLoadListView != null
					&& getCurrentChannel().pullDownToLoadListView.getVisibility() == View.VISIBLE)
			{
//				System.out.println("onScroll() 计算是否可加载");
					
				if(hasMoreData()){
					if(!getCurrentChannel().getLoadingStart())
					{
						getCurrentChannel().pullDownToLoadListView.setAllowPullDownRefersh(false);
					}
					else
					{
						getCurrentChannel().pullDownToLoadListView.setAllowPullDownRefersh(true);
					}
				}else{
					getCurrentChannel().pullDownToLoadListView.setAllowPullDownRefersh(true);
				}
			}
		}
	};

	public void onJoinAnnounceInvitationSuccess()
	{
		if (getCountryChannelView() != null)
		{
			// 隐藏noAllianceFrameLayout，点联盟自然会调用
			getCountryChannelView().getMessagesAdapter().onJoinAnnounceInvitationSuccess();
		}
	}

	public int getToastPosY()
	{
		int[] location = { 0, 0 };
		messagesListFrameLayout.getLocationOnScreen(location);
		return location[1] + ScaleUtil.dip2px(activity, 5);
	}

	private final int TAB_COUNTRY = 0;
	private final int TAB_ALLIANCE = 1;
	private final int TAB_MAIL = 2;

	private void showTab(int tab)
	{
		System.out.println("showTab tab:" + tab);
		
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonCountry, tab == TAB_COUNTRY);
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, tab == TAB_ALLIANCE);
		
		if (tab == TAB_MAIL)
		{
			buttonsLinearLayout.setVisibility(View.GONE);
			imageView2.setVisibility(View.GONE);
		}
		else
		{
			buttonsLinearLayout.setVisibility(View.VISIBLE);
			imageView2.setVisibility(View.VISIBLE);
		}

		boolean isInAlliance = UserManager.getInstance().getCurrentUser().allianceId.equals("") ? false : true;

		for (int i = 0; i < getChannelViewCount(); i++)
		{
			if(getChannelView(i).pullDownToLoadListView!=null)
			{
				getChannelView(i).pullDownToLoadListView.setVisibility(tab == i ? View.VISIBLE : View.GONE);
				if (i == TAB_ALLIANCE && !isInAlliance)
				{
					getChannelView(i).pullDownToLoadListView.setVisibility(View.GONE);
				}
			}
		}

		horn_checkbox.setVisibility((tab==0&&ConfigManager.enableChatHorn)?View.VISIBLE:View.GONE);
		
		noAllianceFrameLayout.setVisibility((tab == TAB_ALLIANCE && !isInAlliance) ? View.VISIBLE : View.GONE);
		relativeLayout1.setVisibility((tab == TAB_ALLIANCE && !isInAlliance) ? View.GONE : View.VISIBLE);

		if (tab == TAB_ALLIANCE && !isInAlliance && ConfigManager.getInstance().isFirstJoinAlliance
				&& !isJoinAlliancePopupShowing)
		{
			try
			{
				// guojiang
//				showJoinAlliancePopup();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}

		if (tab == TAB_COUNTRY)
		{
			ChatServiceController.setCurrentChannelType(DBDefinition.CHANNEL_TYPE_COUNTRY);
			if(ChatServiceController.isHornItemUsed && ConfigManager.enableChatHorn)
			{
				horn_checkbox.setChecked(true);
				refreshBottomUI(true);
				ConfigManager.isHornBtnEnable = true;
			}
			else
			{
				horn_checkbox.setChecked(ConfigManager.isHornBtnEnable);
				refreshBottomUI(ConfigManager.isHornBtnEnable);
			}
		}
		else if (tab == TAB_ALLIANCE)
		{
			ChatServiceController.setCurrentChannelType(DBDefinition.CHANNEL_TYPE_ALLIANCE);
			refreshBottomUI(false);
		}
		else
		{
			refreshBottomUI(false);
		}
		
//		horn_checkbox.setChecked(ChatServiceController.isHornItemUsed && ConfigManager.enableChatHorn);
		setChannelViewIndex(tab);
		
		if(checkMessagesAdapter()){
			oldAdapterCount = getCurrentChannel().getMessagesAdapter().getCount();
			refreshToolTip();
			this.refreshHasMoreData();

			if(getCurrentChannel().chatChannel != null)
			{
				getCurrentChannel().chatChannel.markAsRead();
			}
		}
	}
	
	int mIndex = 0;

	private void refreshWordCount()
	{
		if(replyField == null || wordCount == null) return;
		
		if (replyField.getLineCount() > 2)
		{
			wordCount.setVisibility(View.VISIBLE);
		}
		else
		{
			wordCount.setVisibility(View.GONE);
		}
		wordCount.setText(replyField.getText().length() + "/"+curMaxInputLength);
	}

	@Override
	public void onStop()
	{
		super.onStop();
	}

	public void onBackClicked()
	{
	}

	private boolean adjustSizeCompleted = false;
	private final static int sendButtonBaseWidth = 173;
	private final static int sendButtonBaseHeight = 84;
	private final static int hornCheckBoxWidth=70;
//	private final static int hornCheckBoxHeight=70;

	public void adjustHeight()
	{
		if (!ConfigManager.getInstance().scaleFontandUI)
		{
			if (addReply.getWidth() != 0 && !adjustSizeCompleted)
			{
				adjustSizeCompleted = true;
			}
			return;
		}

		if (addReply.getWidth() != 0 && !adjustSizeCompleted)
		{
			// S3手机上的尺寸(目标效果是在S3手机上调的好，界面、文字都相对于它进行缩放)
			// addReply宽度是宽度的1/4，让其高度保持长宽比，然后再计算出缩放的倍率（textRatio）
			double sendButtonRatio = (double) sendButtonBaseHeight / (double) sendButtonBaseWidth;
			float hornRatio=(float)(addReply.getWidth() * sendButtonRatio/hornCheckBoxWidth);
			ViewHelper.setScaleX(horn_checkbox, hornRatio>1?1:hornRatio);
			ViewHelper.setScaleY(horn_checkbox, hornRatio>1?1:hornRatio);

			addReply.setLayoutParams(new LinearLayout.LayoutParams(addReply.getWidth(), (int) (addReply.getWidth() * sendButtonRatio)));
			buttonCountry.setLayoutParams(new LinearLayout.LayoutParams(buttonCountry.getWidth(), (int) (79 * ConfigManager.scaleRatioButton), 1));
			buttonAlliance.setLayoutParams(new LinearLayout.LayoutParams(buttonAlliance.getWidth(), (int) (79 * ConfigManager.scaleRatioButton), 1));
			
			LinearLayout.LayoutParams param3 = new LinearLayout.LayoutParams((int) (13 * ConfigManager.scaleRatio), (int) (17 * ConfigManager.scaleRatio), 1);
			param3.gravity = Gravity.CENTER_VERTICAL;
			tooltipArrow.setLayoutParams(param3);

			// 9.png图片两端的宽度无法放大，只放大高度显得太狭长
			// RelativeLayout.LayoutParams param2 = new
			// RelativeLayout.LayoutParams(LayoutParams.FILL_PARENT,
			// LayoutParams.WRAP_CONTENT);
			// param2.setMargins(dip2px(activity, -4), dip2px(activity, -2),
			// dip2px(activity, -1), 0);
			// param2.addRule(RelativeLayout.ALIGN_PARENT_TOP);
			// imageView2.setLayoutParams(param2);
			// imageView2.setScaleType(ScaleType.FIT_XY);
			// ViewHelper.setScaleY(imageView2, (float) scaleRatioButton);

			ScaleUtil.adjustTextSize(addReply, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(replyField, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(wordCount, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(buttonCountry, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(buttonAlliance, ConfigManager.scaleRatio);

			ScaleUtil.adjustTextSize(buttonJoinAlliance, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(noAllianceTipText, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(tooltipLabel, ConfigManager.scaleRatio);
			
			adjustSizeCompleted = true;

			if(lazyLoading){
				activity.showProgressBar();
				onBecomeVisible();
			}
		}
	}

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public void onDestroy() {
    	System.out.println("ChatFragment onDestroy");
    	ChatServiceController.isContactMod = false;
		if(tooltipLayout != null) tooltipLayout.setOnClickListener(null);
		buttonJoinAlliance.setOnClickListener(null);
		for (int i = 0; i < channelButton.size(); i++)
		{
			channelButton.get(i).setTag(null);
			channelButton.get(i).setOnClickListener(null);
		}
		channelButton.clear();
		channelButton = null;
		
		getMemberSelectButton().setOnClickListener(null);
		addReply.setOnClickListener(null);

		this.replyField.setOnEditorActionListener(null);
		this.replyField.removeTextChangedListener(textChangedListener);
		textChangedListener = null;
		horn_checkbox.setOnCheckedChangeListener(null);

		int sdk = android.os.Build.VERSION.SDK_INT;
		if (sdk >= android.os.Build.VERSION_CODES.JELLY_BEAN)
		{
			messagesListFrameLayout.getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
		}
		onGlobalLayoutListener = null;
		
		mOnScrollListener = null;
		mListViewLoadListener = null;

		messagesListFrameLayout.removeAllViews();
		
		noAllianceFrameLayout = null;
		relativeLayout1 = null;
		buttonsLinearLayout = null;
		messagesListFrameLayout = null;
		imageView1 = null;
		imageView2 = null;
		horn_checkbox=null;
		horn_tip_layout=null;
		horn_text_tip=null;
		replyField = null;
		wordCount = null;
		addReply = null;
		messageBox = null;
		header = null;
		buttonCountry = null;
		buttonAlliance = null;
		tooltipLayout = null;
		tooltipLabel = null;
		tooltipArrow = null;
		buttonJoinAlliance = null;
		noAllianceTipText = null;
		
		((ChatActivity) getActivity()).fragment = null;
		
    	super.onDestroy();
    }
    
    private ViewTreeObserver.OnGlobalLayoutListener onGlobalLayoutListener;
    private TextWatcher textChangedListener;
}