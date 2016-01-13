package com.elex.chatservice.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.commons.lang.StringUtils;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;

public class MemberSelectorFragment extends ActionBarFragment
{
	private LinearLayout								chatroom_name_layout;
	private TextView									chatroom_name;
	private EditText									chatroom_input;
	private LinearLayout								headerRelativeLayout;

	private Button										cs__cancelBtn;
	private Button										cs__okBtn;
	private Button										cs__quitBtn;

	private ExpandableListAdapter						listAdapter;
	private ExpandableListView							expListView;
	private static List<String>							listDataHeader;
	private static HashMap<String, ArrayList<UserInfo>>	listDataChild;

	private Button										buttonAlliance;
	private Button										buttonSearch;
	private LinearLayout								search_name_layout;
	private TextView									search_name;
	private EditText									search_name_input;
	private Button										search_btn;
	private LinearLayout								buttonsLinearLayout;
	private ExpandableListAdapter						selectedListViewAdapter;
	private ExpandableListView							selectedListView;
	private static List<String>							searchedListDataHeader;
	private static HashMap<String, ArrayList<UserInfo>>	searchListDataChild;

	private ExpandableListAdapter						friendListViewAdapter;
	private ExpandableListView							friendListView;
	private static List<String>							friendListDataHeader;
	private static HashMap<String, ArrayList<UserInfo>>	friendListDataChild;

	public ArrayList<String>							memberUidAdded			= null;
	public ArrayList<String>							memberUidRemoved		= null;

	public ArrayList<String>							commonMemberUidAdded	= null;
	public ArrayList<String>							commonMemberUidRemoved	= null;

	public static String								roomName				= "";

	public static int									memberTab				= 1;

	public MemberSelectorFragment()
	{
		System.out.println("MemberSelectorFragment");
		resetMemberData();
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
	}

	private static Bundle	instanceState;

	public void saveState()
	{
		instanceState = new Bundle();
	}

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = ((MemberSelectorActivity) getActivity());

		return inflater.inflate(ResUtil.getId(this, "layout", "cs__member_selector_fragment"), container, false);
	}

	public static int	index	= 0;

	public void notifyDataSetChanged()
	{
		boolean isAlliance = ((!ChatServiceController.isFriendEnable && selectedListView != null && selectedListView.getVisibility() == View.GONE) || (ChatServiceController.isFriendEnable
				&& friendListView != null && friendListView.getVisibility() == View.GONE))
				&& (expListView != null && expListView.getVisibility() == View.VISIBLE);
		try
		{
			if (listAdapter != null && isAlliance)
			{
				System.out.println("notifyDataSetChanged  ttt");
				listAdapter.notifyDataSetChanged();
			}
			else if (selectedListViewAdapter != null && !ChatServiceController.isFriendEnable && !isAlliance)
			{
				System.out.println("notifyDataSetChanged  ssss");
				selectedListViewAdapter.notifyDataSetChanged();
				for (int j = 0; j < searchedListDataHeader.size(); j++)
				{
					selectedListView.expandGroup(j);
				}
			}
			else if (friendListViewAdapter != null && ChatServiceController.isFriendEnable && !isAlliance)
			{
				System.out.println("notifyDataSetChanged  yyy");
				friendListViewAdapter.notifyDataSetChanged();
				for (int j = 0; j < friendListDataHeader.size(); j++)
				{
					friendListView.expandGroup(j);
				}
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private ArrayList<String> sortRank()
	{
		ArrayList<String> rankArr = new ArrayList<String>();
		HashMap<String, Integer> rankMap = UserManager.getInstance().getRankMap();
		if (rankMap == null)
			return rankArr;

		Set<String> rankSet = rankMap.keySet();

		String[] tempKeyArr = { "", "", "", "", "" };
		for (int i = 5; i > 0; i--)
		{
			String key = UserManager.getInstance().getRankLang(i);
			if (rankSet.contains(key))
				tempKeyArr[i - 1] = key;
		}

		for (int i = 4; i >= 0; i--)
		{
			if (!tempKeyArr[i].equals(""))
				rankArr.add(tempKeyArr[i]);
		}
		return rankArr;
	}

	private void prepareListData()
	{

		HashMap<String, ArrayList<UserInfo>> chatRoomMemberMap = UserManager.getInstance().getChatRoomMemberMap();
		if (chatRoomMemberMap == null)
			return;

		listDataHeader = sortRank();
		listDataChild = chatRoomMemberMap;
	}

	private void prepareJoinedListData()
	{
		searchedListDataHeader = new ArrayList<String>();
		searchListDataChild = new HashMap<String, ArrayList<UserInfo>>();
		ArrayList<String> arr = UserManager.getInstance().getSelctedMemberArr(false);
		if (arr != null && arr.size() > 0)
		{
			String title = LanguageManager.getLangByKey(LanguageKeys.TIP_SELECTED_MEMBER);
			if (title.trim().equals(""))
				title = "已加入";
			searchedListDataHeader.add(title);
			searchListDataChild = UserManager.getInstance().getJoinedMemberMap(title, arr);
		}
	}

	private void prepareFriendListData()
	{
		friendListDataHeader = new ArrayList<String>();
		friendListDataChild = new HashMap<String, ArrayList<UserInfo>>();
		ArrayList<String> arr = UserManager.getInstance().getFriendMemberArr();
		if (arr != null && arr.size() > 0)
		{
			String title = LanguageManager.getLangByKey(LanguageKeys.TITLE_FRIEND_LIST);
			if (title.trim().equals(""))
				title = "Friend List";
			friendListDataHeader.add(title);
			friendListDataChild = UserManager.getInstance().getFriendMemberMap(title, arr);
		}
	}

	public void refreshSearchListData(ArrayList<UserInfo> searchUserInfoArr)
	{
		if (searchUserInfoArr == null)
			return;
		System.out.println("refreshSearchListData searchUserInfoArr size:" + searchUserInfoArr.size());
		String resultStr = LanguageManager.getLangByKey(LanguageKeys.TIP_SEARCH_RESULT);
		if (resultStr.trim().equals(""))
			resultStr = "搜索结果";
		if (searchUserInfoArr.size() > 0)
		{
			if (!searchedListDataHeader.contains(resultStr))
				searchedListDataHeader.add(resultStr);
			if (searchListDataChild.containsKey(resultStr))
			{
				searchListDataChild.remove(resultStr);
			}
			searchListDataChild.put(resultStr, searchUserInfoArr);
		}
		else
		{
			if (searchedListDataHeader.contains(resultStr))
				searchedListDataHeader.remove(resultStr);
			if (searchListDataChild.containsKey(resultStr))
				searchListDataChild.remove(resultStr);
		}

		selectedListViewAdapter = new ExpandableListAdapter(this.activity, this, searchedListDataHeader, searchListDataChild, false);
		selectedListView.setAdapter(selectedListViewAdapter);
		selectedListViewAdapter.fragment = this;

		notifyDataSetChanged();

		if (commonMemberUidAdded != null)
			commonMemberUidAdded.clear();
		if (commonMemberUidRemoved != null)
			commonMemberUidRemoved.clear();
		onSelectionChanged();
	}

	public void onSelectionChanged()
	{
		String titleName = "";
		if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_CHATROOM && !ChatServiceController.isCreateChatRoom)
		{
			titleName = UserManager.getInstance().getCurrentMail().opponentName;
		}
		if (ChatServiceController.isInChatRoom() && titleName.length() > 16 || titleName.equals(""))
		{
			titleName = LanguageManager.getLangByKey(LanguageKeys.TITLE_CHATROOM);
			if (titleName.equals(""))
				titleName = "群聊";
		}

		getMemberSelectButton().setVisibility(View.GONE);

		ArrayList<String> memberUidArray = UserManager.getInstance().getSelectMemberUidArr();

		int size = 0;
		if (memberUidArray != null && memberUidAdded != null && memberUidRemoved != null && commonMemberUidAdded != null
				&& commonMemberUidRemoved != null)
		{
			size = memberUidArray.size() + memberUidAdded.size() - memberUidRemoved.size() + commonMemberUidAdded.size()
					- commonMemberUidRemoved.size();
		}

		System.out.println("onSelectionChanged size:" + size);
		if (size < 0)
			size = 0;
		getTitleLabel().setText(titleName + "(" + size + ")");

		if (ChatServiceController.isFriendEnable)
		{
			chatroom_name_layout.setVisibility(View.VISIBLE);
			search_name_layout.setVisibility(View.GONE);
			headerRelativeLayout.setVisibility(size > 1 ? View.VISIBLE : View.GONE);
		}
		else
		{
			if(memberTab == 1 && size <= 1)
				headerRelativeLayout.setVisibility(View.GONE);
		}
	}

	private void refreshSelectedState()
	{
		HashMap<String, UserInfo> memberInfoMap = UserManager.getInstance().getChatRoomMemberInfoMap();
		Set<String> allianceUidKeySet = null;
		if (memberInfoMap != null && memberInfoMap.size() > 0)
		{
			allianceUidKeySet = memberInfoMap.keySet();
		}

		ArrayList<String> memberUidArray = UserManager.getInstance().getSelectMemberUidArr();
		if (memberUidArray == null)
		{
			memberUidArray = new ArrayList<String>();
		}

		if (allianceUidKeySet == null)
			return;

		boolean isStateChanged = false;
		HashMap<String, Boolean> map = null;
		if (memberTab == 1 && listAdapter != null)
		{
			map = listAdapter.getSelectStateMap();
		}
		else if (memberTab == 2)
		{
			if (ChatServiceController.isFriendEnable)
				map = friendListViewAdapter.getSelectStateMap();
			else
				map = selectedListViewAdapter.getSelectStateMap();
		}

		if (map != null)
		{
			for (int i = 0; i < memberUidAdded.size(); i++)
			{
				String uid = memberUidAdded.get(i);
				if (!uid.equals("") && map.containsKey(uid) && !map.get(uid))
				{
					map.put(uid, Boolean.TRUE);
					isStateChanged = true;
				}
			}

			for (int i = 0; i < memberUidRemoved.size(); i++)
			{
				String uid = memberUidRemoved.get(i);
				if (!uid.equals("") && map.containsKey(uid) && map.get(uid))
				{
					map.put(uid, Boolean.FALSE);
					isStateChanged = true;
				}
			}

			Set<String> selectedMapKeySet = map.keySet();
			if (selectedMapKeySet != null && selectedMapKeySet.size() > 0)
			{
				System.out.println("refreshSelectedState 2");
				for (String uidKey : selectedMapKeySet)
				{
					if (!uidKey.equals(""))
					{
						if (!memberUidAdded.contains(uidKey) && !commonMemberUidAdded.contains(uidKey) && !memberUidArray.contains(uidKey)
								&& map.get(uidKey))
						{
							System.out.println("refreshSelectedState 21");
							map.put(uidKey, Boolean.FALSE);
							isStateChanged = true;
						}
						else if (!memberUidRemoved.contains(uidKey) && !commonMemberUidRemoved.contains(uidKey)
								&& memberUidArray.contains(uidKey) && !map.get(uidKey))
						{
							System.out.println("refreshSelectedState 22");
							map.put(uidKey, Boolean.TRUE);
							isStateChanged = true;
						}
					}
				}
			}
		}

		if (isStateChanged)
			notifyDataSetChanged();
	}

	private int getSelectionCount()
	{
		int result = 0;
		Iterator<Entry<String, ArrayList<UserInfo>>> iter = listDataChild.entrySet().iterator();
		while (iter.hasNext())
		{
			Entry<String, ArrayList<UserInfo>> entry = iter.next();
			List<UserInfo> rank = entry.getValue();
			for (int i = 0; i < rank.size(); i++)
			{
				if (rank.get(i).isSelected)
				{
					result++;
				}
			}
		}
		return result;
	}

	@SuppressLint("SetJavaScriptEnabled")
	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		super.onViewCreated(view, savedInstanceState);

		headerRelativeLayout = (LinearLayout) view.findViewById(R.id.headerRelativeLayout);

		expListView = (ExpandableListView) view.findViewById(ResUtil.getId(this.activity, "id", "cs__myExpandableListView"));
		prepareListData();

		listAdapter = new ExpandableListAdapter(this.activity, this, listDataHeader, listDataChild, true);
		expListView.setAdapter(listAdapter);

		for (int i = 0; i < 5; i++)
		{
			expListView.expandGroup(i);
		}

		selectedListView = (ExpandableListView) view.findViewById(R.id.cs__selectedListView);
		prepareJoinedListData();
		selectedListViewAdapter = new ExpandableListAdapter(this.activity, this, searchedListDataHeader, searchListDataChild, false);
		selectedListView.setAdapter(selectedListViewAdapter);

		for (int j = 0; j < searchedListDataHeader.size(); j++)
		{
			selectedListView.expandGroup(j);
		}

		friendListView = (ExpandableListView) view.findViewById(R.id.cs__friendListView);
		prepareFriendListData();
		friendListViewAdapter = new ExpandableListAdapter(this.activity, this, friendListDataHeader, friendListDataChild, false);
		friendListView.setAdapter(friendListViewAdapter);

		for (int k = 0; k < friendListDataHeader.size(); k++)
		{
			friendListView.expandGroup(k);
		}

		expListView.setOnTouchListener(new OnTouchListener()
		{

			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				hideSoftKeyBoard();
				return false;
			}
		});

		selectedListView.setOnTouchListener(new OnTouchListener()
		{

			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				hideSoftKeyBoard();
				return false;
			}
		});

		expListView.setVisibility(View.VISIBLE);
		selectedListView.setVisibility(View.GONE);
		friendListView.setVisibility(View.GONE);

		fragmentLayout.setOnTouchListener(new OnTouchListener()
		{

			@Override
			public boolean onTouch(View v, MotionEvent event)
			{
				System.out.println("expListView 33");
				hideSoftKeyBoard();
				return false;
			}
		});

		cs__cancelBtn = ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__cancelBtn")));
		cs__cancelBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CANCEL));
		cs__cancelBtn.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				hideSoftKeyBoard();
				if (ChatServiceController.isCreateChatRoom)
					activity.exitActivity();
				else
					activity.exitActivity();
				;
			}
		});

		cs__okBtn = ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__okBtn")));
		cs__okBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
		cs__okBtn.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				onConfirm();
			}
		});

		cs__quitBtn = ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__quitBtn")));
		cs__quitBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_QUIT_CHATROOM));
		cs__quitBtn.setVisibility(ChatServiceController.isCreateChatRoom ? View.GONE : View.VISIBLE);
		cs__quitBtn.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				String content = LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_QUIT);
				MenuController.quitChatRoomConfirm(activity, content);
			}
		});

		chatroom_name_layout = (LinearLayout) view.findViewById(ResUtil.getId(this.activity, "id", "chatroom_name_layout"));
		chatroom_input = (EditText) view.findViewById(ResUtil.getId(this.activity, "id", "chatroom_input"));
		chatroom_input.setOnEditorActionListener(new OnEditorActionListener()
		{

			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
			{
				// 极少量NullPointerException
				if (event == null)
					return false;
				return (event.getKeyCode() == KeyEvent.KEYCODE_ENTER);
			}
		});

		chatroom_name = (TextView) view.findViewById(R.id.chatroom_name);
		chatroom_name.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_MODIFY_CHATROOM_NAME));

		search_name_layout = (LinearLayout) view.findViewById(R.id.search_name_layout);
		search_name_layout.setVisibility(View.GONE);

		buttonAlliance = (Button) view.findViewById(R.id.buttonAlliance);
		buttonAlliance.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_ALLIANCE));
		buttonSearch = (Button) view.findViewById(R.id.buttonSearch);
		if (ChatServiceController.isFriendEnable)
			buttonSearch.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_FRIEND));
		else
			buttonSearch.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));

		CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, true);
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonSearch, false);

		buttonAlliance.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				showAlliancePage();
			}
		});

		buttonSearch.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				showFriendPage();
			}
		});

		search_name = (TextView) view.findViewById(R.id.search_name);
		search_name.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));

		search_name_input = (EditText) view.findViewById(R.id.search_name_input);

		search_name_input.setOnEditorActionListener(new OnEditorActionListener()
		{

			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
			{
				return (event.getKeyCode() == KeyEvent.KEYCODE_ENTER);
			}
		});

		search_btn = (Button) view.findViewById(R.id.search_btn);
		search_btn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));
		search_btn.setOnClickListener(new View.OnClickListener()
		{
			public void onClick(View view)
			{
				hideSoftKeyBoard();
				String name = search_name_input.getText().toString();
				if (!name.trim().equals(""))
				{
					JniController.getInstance().excuteJNIVoidMethod("searchPlayer", new Object[] { name.trim() });
				}
			}

		});

		buttonsLinearLayout = (LinearLayout) view.findViewById(R.id.buttonsLinearLayout);

		fragmentLayout.getChildAt(0).getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener()
		{
			public void onGlobalLayout()
			{
				adjustHeight();
			}
		});

		memberTab = 1;

		onSelectionChanged();

		((MemberSelectorActivity) getActivity()).fragment = this;
	}

	public void onStop()
	{
		super.onStop();
		resetMemberData();
	}

	public void resetMemberData()
	{
		System.out.println("resetMemberData");
		memberUidAdded = new ArrayList<String>();
		if (!ChatServiceController.isCreateChatRoom && !ChatServiceController.isInChatRoom()
				&& !UserManager.getInstance().getCurrentMail().opponentUid.equals(""))
			memberUidAdded.add(UserManager.getInstance().getCurrentMail().opponentUid);
		memberUidRemoved = new ArrayList<String>();

		commonMemberUidAdded = new ArrayList<String>();
		commonMemberUidRemoved = new ArrayList<String>();
	}

	public void onBackButtonClick()
	{
		hideSoftKeyBoard();
	}

	public boolean handleBackPressed()
	{
		return false;
	}

	public void adjustHeight()
	{
		if (!ConfigManager.getInstance().scaleFontandUI)
		{
			adjustSizeCompleted = true;
			return;
		}

		if (buttonSearch.getWidth() != 0 && !adjustSizeCompleted && ConfigManager.scaleRatio != 0 && ConfigManager.scaleRatioButton != 0)
		{
			RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) buttonsLinearLayout.getLayoutParams();
			layoutParams.height = (int) (88 * ConfigManager.scaleRatioButton);
			buttonsLinearLayout.setLayoutParams(layoutParams);

			LinearLayout.LayoutParams layoutParams2 = (LinearLayout.LayoutParams) buttonSearch.getLayoutParams();
			layoutParams2.height = (int) (79 * ConfigManager.scaleRatioButton);
			buttonSearch.setLayoutParams(layoutParams2);

			LinearLayout.LayoutParams layoutParams3 = (LinearLayout.LayoutParams) buttonAlliance.getLayoutParams();
			layoutParams3.height = (int) (79 * ConfigManager.scaleRatioButton);
			buttonAlliance.setLayoutParams(layoutParams3);

			ScaleUtil.adjustTextSize(buttonSearch, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(buttonAlliance, ConfigManager.scaleRatio);
			ScaleUtil.adjustTextSize(search_btn, ConfigManager.scaleRatio);

			adjustSizeCompleted = true;
		}
	}

	private void showAlliancePage()
	{
		showPage(true);
	}

	private void showFriendPage()
	{
		showPage(false);
	}

	private void showPage(boolean isAlliance)
	{
		hideSoftKeyBoard();

		if (ChatServiceController.isFriendEnable)
		{
			friendListView.setVisibility(isAlliance ? View.GONE : View.VISIBLE);
			selectedListView.setVisibility(View.GONE);
			chatroom_name_layout.setVisibility(View.VISIBLE);
			search_name_layout.setVisibility(View.GONE);
		}
		else
		{
			friendListView.setVisibility(View.GONE);
			if (isAlliance)
			{
				chatroom_name_layout.setVisibility(View.VISIBLE);
				search_name_layout.setVisibility(View.GONE);
				selectedListView.setVisibility(View.GONE);
			}
			else
			{
				chatroom_name_layout.setVisibility(View.GONE);
				headerRelativeLayout.setVisibility(View.VISIBLE);
				search_name_layout.setVisibility(View.GONE);
				selectedListView.setVisibility(View.VISIBLE);
			}
		}

		expListView.setVisibility(isAlliance ? View.VISIBLE : View.GONE);
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, isAlliance ? true : false);
		CompatibleApiUtil.getInstance().setButtonAlpha(buttonSearch, isAlliance ? false : true);
		memberTab = isAlliance ? 1 : 2;
		onSelectionChanged();
		refreshSelectedState();
	}

	private void onConfirm()
	{
		LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_VIEW, "memberUidAdded.size()", memberUidAdded.size(),
				"commonMemberUidAdded.size()", commonMemberUidAdded.size());

		roomName = chatroom_input.getText().toString().trim();
		hideSoftKeyBoard();
		if (ChatServiceController.isCreateChatRoom)
		{
			createChatroom();
		}
		else if (StringUtils.isNotEmpty(UserManager.getInstance().getCurrentMail().opponentUid))
		{
			modifyChatroom();
		}
		else
		{
			LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_VIEW, "condition error");
			LogUtil.trackMessage("MemberSelectorFragment.onConfirm() condition error", "", "");
			activity.exitActivity();
		}
	}

	private void createChatroom()
	{
		if (!(memberUidAdded.size() > 0 || commonMemberUidAdded.size() > 0))
		{
			return;
		}

		ArrayList<String> uidAdded = new ArrayList<String>();
		uidAdded.addAll(memberUidAdded);
		uidAdded.addAll(commonMemberUidAdded);

		String content = "";
		if (uidAdded.size() > 1)
		{
			if (LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE).equals(""))
				content = "是否邀请" + UserManager.getInstance().createNameStr(uidAdded);
			else
				content = LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE, UserManager.getInstance().createNameStr(uidAdded));
			MenuController.showInviteChatRoomMemberConfirm(activity, content, uidAdded);
		}
		else
		{
			if (ChatServiceController.isNewMailListEnable)
			{
				ChatServiceController.isCreateChatRoom = true;
				Intent intent = new Intent();
				intent.putExtra("roomName", MemberSelectorFragment.roomName);
				intent.putExtra("uidStr", UserManager.getInstance().createUidStr(uidAdded));
				intent.putExtra("nameStr", UserManager.getInstance().createNameStr(uidAdded));
				activity.setResult(Activity.RESULT_OK, intent);
				activity.exitActivity();
			}
			else
			{
				JniController.getInstance().excuteJNIVoidMethod(
						"selectChatRoomMember",
						new Object[] {
								MemberSelectorFragment.roomName,
								UserManager.getInstance().createNameStr(uidAdded),
								UserManager.getInstance().createUidStr(uidAdded) });
				activity.exitActivity();
			}
		}
	}

	private void modifyChatroom()
	{
		ArrayList<String> uidAdded = new ArrayList<String>();
		ArrayList<String> uidRemoved = new ArrayList<String>();
		String modifyName = "";
		String content = "";
		if (memberUidAdded.size() > 0 || commonMemberUidAdded.size() > 0)
		{
			uidAdded.addAll(memberUidAdded);
			uidAdded.addAll(commonMemberUidAdded);
			LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_VIEW, "uidAdded.size()", uidAdded.size());

			if (uidAdded.size() > 0)
			{
				content = LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE, UserManager.getInstance().createNameStr(uidAdded));
			}

			if (!ChatServiceController.isInChatRoom())
			{
				MenuController.showCreateChatRoomConfirm(activity, content, uidAdded);
				return;
			}
		}

		if (memberUidRemoved.size() > 0 || commonMemberUidRemoved.size() > 0)
		{
			LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_VIEW, "memberUidRemoved.size()", memberUidRemoved.size(),
					"commonMemberUidRemoved.size()", commonMemberUidRemoved.size());

			uidRemoved.addAll(memberUidRemoved);
			uidRemoved.addAll(commonMemberUidRemoved);

			if (uidRemoved.size() > 0)
			{
				String temp = LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_KICK,
						UserManager.getInstance().createNameStr(uidRemoved));
				content = appendStr(content, temp);
			}
		}

		if (!roomName.equals("") && UserManager.getInstance().getCurrentMail().opponentName != null
				&& !UserManager.getInstance().getCurrentMail().opponentName.equals(roomName))
		{
			modifyName = roomName;
			String temp = LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_MODIFYNAME, roomName);
			content = appendStr(content, temp);
		}

		if (uidAdded.size() > 0 || uidRemoved.size() > 0 || StringUtils.isNotEmpty(modifyName))
			MenuController.showChatRoomManagerConfirm(activity, content, uidAdded, uidRemoved, modifyName);
	}

	private String appendStr(String content, String content2)
	{
		return content + (StringUtils.isNotEmpty(content) ? "\n" : "") + content2;
	}
}