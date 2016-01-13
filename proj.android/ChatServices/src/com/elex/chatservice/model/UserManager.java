package com.elex.chatservice.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang.StringUtils;

import android.util.Log;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.LogUtil;

public class UserManager
{
	private static UserManager						instance;
	private String									currentUserId;
	private MailInfo								currentMail;
	private ArrayList<String>						banUidList;
	private ArrayList<String>						banNoticeUidList;
	private ArrayList<String>						blockUidList;
	private ArrayList<String>						reportUidList;
	private ArrayList<MsgItem>						reportContentList;
	private ArrayList<MsgItem>						reportContentTranslationList;
	private ArrayList<UserInfo>						userList;
	private HashMap<String, ArrayList<UserInfo>>	allianceMemberMap;
	/** 好友列表 */
	private HashMap<String, UserInfo>				friendMemberMap;
	/** 用于联盟成员等级排序 */
	private HashMap<String, Integer>				rankMap;
	public HashMap<String, UserInfo>				allianceMemberInfoMap;
	/** 非联盟成员信息map */
	public HashMap<String, UserInfo>				nonAllianceMemberInfoMap;
	private ScheduledExecutorService				service;
	private TimerTask								timerTask;
	private long									lastAddUidTime					= 0;
	private long									lastCallSuccessTime				= 0;
	private long									lastCallTime					= 0;
	private long									CALL_TIME_OUT					= 8000;
	private static final int						GET_USER_INFO_UID_COUNT			= 20;
	/** 实际向后台发送了请求的uid列表 */
	private ArrayList<String>						fechingUids						= new ArrayList<String>();
	/** 请求的uid队列 */
	private ArrayList<String>						queueUids						= new ArrayList<String>();

	public static final int							BLOCK_LIST						= 1;
	public static final int							BAN_LIST						= 2;
	public static final int							REPORT_LIST						= 3;
	public static final int							REPORT_CONTETN_LIST				= 4;
	public static final int							REPORT_TRANSLATION_LIST			= 5;
	public static final int							BAN_NOTICE_LIST					= 6;

	public static final int							NOTIFY_USERINFO_TYPE_ALLIANCE	= 0;
	public static final int							NOTIFY_USERINFO_TYPE_FRIEND		= 1;

	private UserManager()
	{
		reset();
	}

	public void reset()
	{
		banUidList = new ArrayList<String>();
		banNoticeUidList = new ArrayList<String>();
		blockUidList = new ArrayList<String>();
		reportUidList = new ArrayList<String>();
		reportContentList = new ArrayList<MsgItem>();
		reportContentTranslationList = new ArrayList<MsgItem>();
		userList = new ArrayList<UserInfo>();
		currentMail = new MailInfo();
		allianceMemberMap = new HashMap<String, ArrayList<UserInfo>>();
		rankMap = new HashMap<String, Integer>();
		allianceMemberInfoMap = new HashMap<String, UserInfo>();
		nonAllianceMemberInfoMap = new HashMap<String, UserInfo>();
		friendMemberMap = new HashMap<String, UserInfo>();
	}

	public static UserManager getInstance()
	{
		if (instance == null)
		{
			instance = new UserManager();
		}
		return instance;
	}

	/**
	 * 仅在get不到的时候才调用
	 */
	public void addUser(UserInfo user)
	{
		if (!isUserExists(user.uid))
		{
			_addUser(user);

			if (!user.isDummy)
			{
				DBManager.getInstance().insertUser(user);
			}
		}
	}

	/**
	 * 实际添加，不触发数据库刷新
	 */
	private void _addUser(UserInfo user)
	{
		userList.add(user);
	}

	public void updateUser(UserInfo user)
	{
		for (int i = 0; i < userList.size(); i++)
		{
			if (user.uid.equals(userList.get(i).uid))
			{
				userList.set(i, user);
			}
		}
		DBManager.getInstance().updateUser(user);
	}

	public boolean isUserExists(String userID)
	{
		for (int i = 0; i < userList.size(); i++)
		{
			if (userID.equals(userList.get(i).uid))
				return true;
		}
		return false;
	}

	/**
	 * 如果UserManager获取不到，就从DB获取
	 */
	public UserInfo getUser(String userID)
	{
		for (int i = 0; i < userList.size(); i++)
		{
			if (userID.equals(userList.get(i).uid))
				return userList.get(i);
		}

		UserInfo result = null;
		result = DBManager.getInstance().getUser(userID);
		if (result != null)
		{
			_addUser(result);
		}

		return result;
	}

	public void setCurrentUserId(String id)
	{
		if (!StringUtils.isEmpty(id))
			currentUserId = id;
	}

	public String getCurrentUserId()
	{
		return currentUserId;
	}

	public UserInfo getCurrentUser()
	{
		if (!StringUtils.isEmpty(currentUserId))
		{
			UserInfo user = getUser(currentUserId);
			if (user == null)
			{
				user = new UserInfo();
				user.uid = currentUserId;
				addUser(user);
			}
			return user;
		}
		else
		{
			LogUtil.trackMessage("UserManager.getCurrentUser() currentUserId is empty", "", "");
			return null;
		}
	}

	/**
	 * 初始登录时会调，此时数据库还未初始化
	 */
	public void updateCurrentUser()
	{
		DBManager.getInstance().updateUser(getCurrentUser());
	}

	public boolean isCurrentUserInAlliance()
	{
		if (getCurrentUser() != null && StringUtils.isNotEmpty(getCurrentUser().allianceId))
			return true;
		return false;
	}

	public MailInfo getCurrentMail()
	{
		return currentMail;
	}

	public void addRestrictUser(String uid, int type)
	{
		if (!isInRestrictList(uid, type))
		{
			if (type == BLOCK_LIST)
				blockUidList.add(uid);
			else if (type == BAN_LIST)
				banUidList.add(uid);
			else if (type == REPORT_LIST)
				reportUidList.add(uid);
			else if (type == BAN_NOTICE_LIST)
				banNoticeUidList.add(uid);
		}
	}
	
	public String getShieldSql()
	{
		if(blockUidList == null || blockUidList.size() <=0)
			return "";
		String result = "";
		for (int i = 0; i < blockUidList.size(); i++)
		{
			String uid = blockUidList.get(i);
			if(StringUtils.isNotEmpty(uid))
				result += " AND UserID <> " + uid;
		}
		return result;
	}

	public void addReportContent(MsgItem item, int type)
	{
		if (!isInReportContentList(item, type))
		{
			if (type == REPORT_CONTETN_LIST)
				reportContentList.add(item);
			else
				reportContentTranslationList.add(item);
		}
	}

	public void removeRestrictUser(String uid, int type)
	{
		if (type == BLOCK_LIST)
		{
			for (int i = 0; i < blockUidList.size(); i++)
			{
				String n = blockUidList.get(i);
				if (n.equals(uid))
				{
					blockUidList.remove(i);
				}
			}
		}
		else if (type == BAN_LIST)
		{
			for (int i = 0; i < banUidList.size(); i++)
			{
				String n = banUidList.get(i);
				if (n.equals(uid))
				{
					banUidList.remove(i);
				}
			}
		}
		else if (type == BAN_NOTICE_LIST)
		{
			for (int i = 0; i < banNoticeUidList.size(); i++)
			{
				String n = banNoticeUidList.get(i);
				if (n.equals(uid))
				{
					banNoticeUidList.remove(i);
				}
			}
		}
	}

	public boolean isInReportContentList(MsgItem msgItem, int type)
	{
		if (type == REPORT_CONTETN_LIST && reportContentList != null && reportContentList.contains(msgItem))
			return true;
		else if (type == REPORT_TRANSLATION_LIST && reportContentTranslationList != null && reportContentTranslationList.contains(msgItem))
			return true;
		return false;
	}

	public boolean isInRestrictList(String uid, int type)
	{
		if (type == BLOCK_LIST)
		{
			for (int i = 0; i < blockUidList.size(); i++)
			{
				String n = blockUidList.get(i);
				if (n.equals(uid))
					return true;
			}
		}
		else if (type == BAN_LIST)
		{
			for (int i = 0; i < banUidList.size(); i++)
			{
				String n = banUidList.get(i);
				if (n.equals(uid))
					return true;
			}
		}
		else if (type == BAN_NOTICE_LIST)
		{
			for (int i = 0; i < banNoticeUidList.size(); i++)
			{
				String n = banNoticeUidList.get(i);
				if (n.equals(uid))
					return true;
			}
		}
		else if (type == REPORT_LIST)
		{
			for (int i = 0; i < reportUidList.size(); i++)
			{
				String n = reportUidList.get(i);
				if (n.equals(uid))
					return true;
			}
		}

		return false;
	}

	/**
	 * 检查uid指向的用户在db中是否存在且是最新的，如果不是则从后台获取用户信息
	 * <p>
	 * 如果用户不存在，会创建一个dummy user
	 * <p>
	 * 
	 * @param name
	 *            可为""，如果指定的话，创建dummy user时，设置其name
	 * @param updateTime
	 *            为0时只检查存在性(认为是新的，可能是db中以前没存)，大于0时检查新旧性
	 */
	public static void checkUser(String uid, String name, int updateTime)
	{
		UserInfo user = UserManager.getInstance().getUser(uid);

		boolean isOld = false;
		if (user != null)
		{
			isOld = updateTime > 0 ? updateTime > user.lastUpdateTime : false;
		}

		// 以前有!user.isValid()条件，是多余的。dummy
		// user只有本函数创建，如果是dummy的，说明已经获取过了，不需要再次获取
		if (user == null || (isOld && !user.uid.equals(UserManager.getInstance().getCurrentUserId())) || user.lang == null)
		{
			if (user != null && !ChatServiceController.getInstance().isUsingDummyHost())
			{
				LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "uid", uid, "user", user, "updateTime", updateTime,
						"user.lastUpdateTime", user.lastUpdateTime, "isOld", isOld);
			}
			else if(!ChatServiceController.getInstance().isUsingDummyHost())
			{
				LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "uid", uid, "user", user, "updateTime", updateTime,
						"isOld", isOld);
			}

			if (user == null)
			{
				user = new UserInfo(uid);
				if (StringUtils.isNotEmpty(name))
					user.userName = name;
				UserManager.getInstance().addUser(user);
			}

			ArrayList<String> uids = new ArrayList<String>();
			uids.add(uid);
			UserManager.getInstance().getMultiUserInfo(uids);
		}
	}

	private static String array2Str(ArrayList<String> arr)
	{
		String result = "";
		for (int i = 0; i < arr.size(); i++)
		{
			if (i > 0)
			{
				result += ",";
			}
			result += arr.get(i);
		}
		return result;
	}

	private synchronized void getMultiUserInfo(ArrayList<String> uids)
	{
		if (ChatServiceController.getInstance().isUsingDummyHost())
		{
			return;
		}
		
		synchronized (this)
		{
			boolean hasNewUid = false;

			for (int i = 0; i < uids.size(); i++)
			{
				String uid = uids.get(i);
				if (!fechingUids.contains(uid) && !queueUids.contains(uid))
				{
					// LogUtil.printVariablesWithFuctionName(Log.INFO,
					// LogUtil.TAG_MSG, "uid", uid, "fechingUids",
					// array2Str(fechingUids),
					// "queueUids", array2Str(queueUids), "user",
					// UserManager.getInstance().getUser(uid));

					queueUids.add(uid);
					hasNewUid = true;
					lastAddUidTime = System.currentTimeMillis();
				}
			}

			if (hasNewUid && service == null)
			{
				startTimer();
			}
		}
	}

	private synchronized void startTimer()
	{
		if (service != null)
		{
			return;
		}

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG);

		service = Executors.newSingleThreadScheduledExecutor();
		timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					checkUidQueue();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};

		service.scheduleWithFixedDelay(timerTask, 100, 500, TimeUnit.MILLISECONDS);
	}

	private boolean isQueueClear()
	{
		return queueUids.size() == 0 && fechingUids.size() == 0;
	}

	private synchronized void checkUidQueue()
	{
		if (isQueueClear())
		{
			return;
		}

		synchronized (this)
		{
			long now = System.currentTimeMillis();

			if ((now - lastAddUidTime) > 500 && (isLastCallSuccess() || isLastCallTimeOut()))
			{
				actualCall();
			}
		}
	}

	private boolean isLastCallSuccess()
	{
		return (System.currentTimeMillis() - lastCallSuccessTime) > 0;
	}

	private boolean isLastCallTimeOut()
	{
		return lastCallTime > 0 && (System.currentTimeMillis() - lastCallTime) > CALL_TIME_OUT;
	}

	private synchronized void actualCall()
	{
		if (fechingUids.size() > 0)
		{
			if (isLastCallSuccess())
			{
				// 成功返回，但有些uid取不到
				LogUtil.trackMessage("取不到：fechingUids is not empty", "", "");
				LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_MSG, "取不到：fechingUids is not empty");
				fechingUids.clear();
			}
			else
			{
				// 1.如果正有别的命令在调用，会放弃调用，导致超时; 2.后台接口未返回，导致超时
				LogUtil.trackMessage("超时：fechingUids is not empty", "", "");
				LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_MSG, "超时：fechingUids is not empty");
			}
		}
		int count = queueUids.size() > (GET_USER_INFO_UID_COUNT - fechingUids.size()) ? (GET_USER_INFO_UID_COUNT - fechingUids.size())
				: queueUids.size();
		for (int i = 0; i < count; i++)
		{
			fechingUids.add(queueUids.remove(0));
		}
		String uidsStr = ChatChannel.getMembersString(fechingUids);
		JniController.getInstance().excuteJNIVoidMethod("getMultiUserInfo", new Object[] { uidsStr });
		lastCallTime = System.currentTimeMillis();
		lastCallSuccessTime = System.currentTimeMillis() * 2;
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "uidsStr", uidsStr, "fechingUids", array2Str(fechingUids),
				"queueUids", array2Str(queueUids));
	}

	public synchronized void onReceiveUserInfo(Object[] userInfoArray)
	{
		if (userInfoArray == null)
			return;

		synchronized (this)
		{
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG);
			for (int i = 0; i < userInfoArray.length; i++)
			{
				UserInfo user = (UserInfo) userInfoArray[i];
				LogUtil.printVariables(Log.INFO, LogUtil.TAG_MSG, "uid", user.uid);
				if (friendMemberMap.containsKey(user.uid))
				{
					putFriendMemberInMap(user);
				}
				if (allianceMemberInfoMap.containsKey(user.uid))
					putChatRoomMemberInMap(user);

				user.initNullField();
				UserInfo oldUser = getUser(user.uid);

				if (fechingUids.contains(user.uid))
				{
					fechingUids.remove(user.uid);
				}

				if (oldUser == null)
				{
					LogUtil.trackMessage("onReceiveUserInfo(): oldUser is null (impossible): " + user.uid, "", "");
					LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_MSG, "oldUser is null (impossible):" + user.uid);
					addUser(user);
				}
				else if (oldUser.isDummy || user.lastUpdateTime > oldUser.lastUpdateTime || (oldUser.lang == null && user.lang != null))
				{
					updateUser(user);
				}
				else
				{
					LogUtil.trackMessage("onReceiveUserInfo(): user is not newer: " + user.uid, "", "");
					LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_MSG, "user is not newer:" + user.uid);
					LogUtil.printVariables(Log.WARN, LogUtil.TAG_MSG,
							"compare user:\n" + LogUtil.compareObjects(new Object[] { oldUser, user }));
				}
			}

			lastCallSuccessTime = System.currentTimeMillis();
			lastAddUidTime = System.currentTimeMillis();

			ChatServiceController.getInstance().notifyCurrentDataSetChanged();
		}
	}

	public void onReceiveSearchUserInfo(Object[] userInfoArray)
	{
		if (userInfoArray == null)
			return;

		final ArrayList<UserInfo> userArr = new ArrayList<UserInfo>();
		ArrayList<String> nonAllianceMemberArr = getSelctedMemberArr(false);
		for (int i = 0; i < userInfoArray.length; i++)
		{
			UserInfo user = (UserInfo) userInfoArray[i];
			if (nonAllianceMemberArr.contains(user.uid))
				continue;
			userArr.add(user);
			putChatRoomMemberInMap(user);
			user.initNullField();
			UserInfo oldUser = getUser(user.uid);

			if (oldUser == null)
			{
				addUser(user);
			}
			else if (oldUser.isDummy || user.lastUpdateTime > oldUser.lastUpdateTime || (oldUser.lang == null && user.lang != null))
			{
				updateUser(user);
				ChatServiceController.getInstance().notifyCurrentDataSetChanged();
			}
		}
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getMemberSelectorFragment() != null)
					{
						ChatServiceController.getMemberSelectorFragment().refreshSearchListData(userArr);
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});

	}

	public void putNonAllianceInMap(UserInfo user)
	{
		if (user == null)
			return;
		String uid = user.uid;
		nonAllianceMemberInfoMap.put(uid, user);
	}

	public void putChatRoomMemberInMap(UserInfo user)
	{
		int rank = user.allianceRank;
		String uid = user.uid;
		String allianceId = UserManager.getInstance().getCurrentUser().allianceId;
		if (allianceId != null && allianceId.equals(user.allianceId))
		{
			allianceMemberInfoMap.put(uid, user);

			if (rank > 0)
			{
				String rankKey = getRankLang(rank);
				rankMap.put(rankKey, Integer.valueOf(rank));
				resetAllianceRank(rankKey);
				ArrayList<UserInfo> userArr = allianceMemberMap.get(rankKey);
				boolean isInRank = false;
				for (int i = 0; i < userArr.size(); i++)
				{
					UserInfo info = userArr.get(i);
					if (info.uid.equals(user.uid))
					{
						allianceMemberMap.get(rankKey).remove(info);
						allianceMemberMap.get(rankKey).add(user);
						isInRank = true;
						return;
					}
				}
				if (!isInRank)
					allianceMemberMap.get(rankKey).add(user);
			}
		}
		else
		{
			putNonAllianceInMap(user);
		}
	}

	public void putFriendMemberInMap(UserInfo user)
	{
		if (user == null)
			return;
		String uid = user.uid;
		friendMemberMap.put(uid, user);
	}

	public String getRankLang(int rank)
	{
		String rankStr = "";
		switch (rank)
		{
			case 1:
				rankStr = LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK1);
				break;
			case 2:
				rankStr = LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK2);
				break;
			case 3:
				rankStr = LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK3);
				break;
			case 4:
				rankStr = LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK4);
				break;
			case 5:
				rankStr = LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK5);
				break;
		}
		return rankStr;
	}

	private void resetAllianceRank(String key)
	{
		if (allianceMemberMap.containsKey(key))
			return;
		ArrayList<UserInfo> userInfoArray = new ArrayList<UserInfo>();
		allianceMemberMap.put(key, userInfoArray);
	}

	public void clearAllianceMember()
	{
		if (allianceMemberMap != null)
			allianceMemberMap.clear();
		if (allianceMemberInfoMap != null)
			allianceMemberInfoMap.clear();
		if (rankMap != null)
			rankMap.clear();
	}

	public void clearFriendMember()
	{
		if (friendMemberMap != null)
			friendMemberMap.clear();
	}

	public void clearNonAllianceMember()
	{
		if (nonAllianceMemberInfoMap != null)
			nonAllianceMemberInfoMap.clear();
	}

	public HashMap<String, ArrayList<UserInfo>> getChatRoomMemberMap()
	{
		return allianceMemberMap;
	}

	public HashMap<String, UserInfo> getChatRoomMemberInfoMap()
	{
		return allianceMemberInfoMap;
	}

	public HashMap<String, UserInfo> getNonAllianceMemberInfoMap()
	{
		return nonAllianceMemberInfoMap;
	}

	public HashMap<String, Integer> getRankMap()
	{
		return rankMap;
	}

	public boolean isMoreThanOneMember()
	{
		boolean ret = false;
		Set<String> keySet = allianceMemberMap.keySet();
		if (keySet.size() > 1)
		{
			ret = true;
		}
		else if (keySet.size() == 1)
		{
			for (String key : keySet)
			{
				ret = allianceMemberMap.get(key).size() > 1;
			}
		}

		return ret;
	}

	public String createUidStr(ArrayList<String> uidArr)
	{
		String uidStr = "";
		for (int i = 0; i < uidArr.size(); i++)
		{
			if (!uidArr.get(i).equals(""))
			{
				if (!uidStr.equals(""))
					uidStr = uidStr + "|" + uidArr.get(i);
				else
					uidStr = uidArr.get(i);
			}
		}
		return uidStr;

	}

	public String createNameStr(ArrayList<String> uidArr)
	{
		String nameStr = "";
		for (int i = 0; i < uidArr.size(); i++)
		{
			if (!uidArr.get(i).equals(""))
			{
				String uid = uidArr.get(i);

				UserInfo user = null;
				if (allianceMemberInfoMap.containsKey(uid))
				{
					user = allianceMemberInfoMap.get(uid);
				}
				else if (nonAllianceMemberInfoMap.containsKey(uid))
				{
					user = nonAllianceMemberInfoMap.get(uid);
				}
				else if (friendMemberMap.containsKey(uid))
				{
					user = friendMemberMap.get(uid);
				}

				if (user == null)
					user = getUser(uid);

				if (user != null)
				{
					if (!nameStr.equals(""))
						nameStr = nameStr + "、" + user.userName;
					else
						nameStr = user.userName;
				}
			}
		}
		return nameStr;
	}

	public ArrayList<String> getSelectMemberUidArr()
	{
		ArrayList<String> memberUidArray = new ArrayList<String>();
		if (ChatServiceController.isCreateChatRoom)
		{
			if (!UserManager.getInstance().getCurrentUser().uid.equals(""))
				memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
		}
		else
		{
			if (!ChatServiceController.isInMailDialog())
				return memberUidArray;
			if (!ChatServiceController.isInChatRoom())
			{
				if (!UserManager.getInstance().getCurrentUser().uid.equals(""))
					memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
			}
			else
			{
				memberUidArray = ChannelManager.getInstance().getChatRoomMemberArrayByKey(getCurrentMail().opponentUid);
			}
		}
		return memberUidArray;
	}

	public ArrayList<String> getSelctedMemberArr(boolean isFromAlliance)
	{
		ArrayList<String> memberUidArray = new ArrayList<String>();
		boolean isInAlliance = !UserManager.getInstance().getCurrentUser().allianceId.equals("");

		if (ChatServiceController.isCreateChatRoom || (ChatServiceController.isInMailDialog() && !ChatServiceController.isInChatRoom()))
		{
			if (((isInAlliance && isFromAlliance) || (!isInAlliance && !isFromAlliance))
					&& !UserManager.getInstance().getCurrentUser().uid.equals(""))
				memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
		}
		else if (ChatServiceController.isInChatRoom())
		{
			memberUidArray = new ArrayList<String>();
			HashMap<String, UserInfo> memberInfoMap = UserManager.getInstance().getChatRoomMemberInfoMap();
			Set<String> uidKeySet = memberInfoMap.keySet();
			List<String> userArray = ChannelManager.getInstance().getChatRoomMemberArrayByKey(getCurrentMail().opponentUid);
			for (int i = 0; i < userArray.size(); i++)
			{
				String uid = userArray.get(i);
				if (!uid.equals("") && (isFromAlliance && uidKeySet.contains(uid)) || (!isFromAlliance && !uidKeySet.contains(uid)))
					memberUidArray.add(uid);
			}
		}
		return memberUidArray;
	}

	public ArrayList<String> getFriendMemberArr()
	{
		ArrayList<String> memberUidArray = new ArrayList<String>();

		if (friendMemberMap != null && friendMemberMap.size() > 0)
		{
			Set<String> uidKeySet = friendMemberMap.keySet();
			for (String uid : uidKeySet)
			{
				if (StringUtils.isNotEmpty(uid))
					memberUidArray.add(uid);
			}
		}
		return memberUidArray;
	}

	public HashMap<String, ArrayList<UserInfo>> getFriendMemberMap(String key, List<String> uidArr)
	{
		HashMap<String, ArrayList<UserInfo>> map = new HashMap<String, ArrayList<UserInfo>>();
		if (uidArr != null && uidArr.size() > 0)
		{
			ArrayList<UserInfo> userArr = new ArrayList<UserInfo>();
			for (int i = 0; i < uidArr.size(); i++)
			{
				String uid = uidArr.get(i);
				if (!uid.equals(""))
				{
					if (friendMemberMap.containsKey(uid) && friendMemberMap.get(uid) != null && !friendMemberMap.get(uid).isDummy)
						userArr.add(friendMemberMap.get(uid));
					else
					{
						UserInfo user = getUser(uid);
						if (user != null)
						{
							userArr.add(user);
						}
						checkUser(uid, "", 0);
					}
				}
			}

			if (userArr.size() > 0)
				map.put(key, userArr);
		}
		return map;
	}

	public HashMap<String, ArrayList<UserInfo>> getJoinedMemberMap(String key, List<String> uidArr)
	{
		HashMap<String, ArrayList<UserInfo>> map = new HashMap<String, ArrayList<UserInfo>>();

		if (uidArr != null && uidArr.size() > 0)
		{
			ArrayList<UserInfo> userArr = new ArrayList<UserInfo>();
			HashMap<String, UserInfo> memberInfoMap = UserManager.getInstance().getNonAllianceMemberInfoMap();
			for (int i = 0; i < uidArr.size(); i++)
			{
				String uid = uidArr.get(i);
				if (!uid.equals(""))
				{
					if (memberInfoMap.containsKey(uid))
						userArr.add(memberInfoMap.get(uid));
					else
					{
						UserInfo user = getUser(uid);
						if (user != null)
						{
							userArr.add(user);
						}
						checkUser(uid, "", 0);
					}
				}
			}

			if (userArr.size() > 0)
				map.put(key, userArr);
		}
		return map;
	}
}
