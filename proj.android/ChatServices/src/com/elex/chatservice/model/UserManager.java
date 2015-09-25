package com.elex.chatservice.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.LogUtil;

public class UserManager
{
	private static UserManager instance;

//	private UserInfo currentUser;
	private String currentUserId;
	private MailInfo currentMail;

	public static final int BLOCK_LIST = 1;
	public static final int BAN_LIST = 2;
	public static final int REPORT_LIST = 3;
	private ArrayList<String> banUidList;
	private ArrayList<String> blockUidList;
	private ArrayList<String> reportUidList;
	private ArrayList<UserInfo> userList;
	private HashMap<String,ArrayList<UserInfo>> allianceMemberMap;
	private HashMap<String,Integer> rankMap;		//用于联盟成员等级排序
	public HashMap<String,UserInfo> allianceMemberInfoMap;
	public HashMap<String,UserInfo> nonAllianceMemberInfoMap;	//非联盟成员信息map

	private UserManager()
	{
		reset();
	}
	
	public void reset()
	{
		banUidList = new ArrayList<String>();
		blockUidList = new ArrayList<String>();
		reportUidList = new ArrayList<String>();
		userList = new ArrayList<UserInfo>();
		currentMail = new MailInfo();
		allianceMemberMap=new HashMap<String, ArrayList<UserInfo>>();
		rankMap=new HashMap<String, Integer>();
		allianceMemberInfoMap=new HashMap<String, UserInfo>();
		nonAllianceMemberInfoMap=new HashMap<String, UserInfo>();
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
		if(!isUserExists(user.uid))
		{
			_addUser(user);

			if(!user.isDummy){
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
	 *
	 * @return 可能为null
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
		if(result != null)
		{
			_addUser(result);
		}

		return result;
	}
	
	public void setCurrentUserId(String id)
	{
		if(!StringUtils.isEmpty(id))
			currentUserId = id;
	}
	
	public String getCurrentUserId()
	{
		return currentUserId;
	}

	public UserInfo getCurrentUser()
	{
		if(!StringUtils.isEmpty(currentUserId)){
			UserInfo user = getUser(currentUserId);
			if(user == null){
				user = new UserInfo();
				user.uid = currentUserId;
				addUser(user);
			}
			return user;
		}else{
			System.out.println("UserManager getCurrentUser() currentUserId为空 !");
			LogUtil.trackMessage("UserManager.getCurrentUser() currentUserId is empty", "", "");
			return null;
		}
//		if(currentUser == null){
//			currentUser = new UserInfo();
////			_addUser(currentUser);
//		}
//		return currentUser;
	}

	/**
	 * 初始登陆时会调，此时数据库还未初始化
	 */
	public void updateCurrentUser()
	{
		System.out.println("updateCurrentUser()");
		DBManager.getInstance().updateUser(getCurrentUser());
	}

	public boolean isCurrentUserInAlliance()
	{
		if(getCurrentUser()!=null && StringUtils.isNotEmpty(getCurrentUser().allianceId))
			return true;
		return false;
	}

	public MailInfo getCurrentMail()
	{
		return currentMail;
	}

	public void addRestrictUser(String uid, int type)
	{
		if(!isInRestrictList(uid, type))
		{
			if (type == BLOCK_LIST)
				blockUidList.add(uid);
			else if (type == BAN_LIST) 
				banUidList.add(uid);
			else if (type == REPORT_LIST) 
				reportUidList.add(uid);
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
	}

	public boolean isInRestrictList(String uid, int type)
	{
		if (type == BLOCK_LIST)
		{
			for (int i = 0; i < blockUidList.size(); i++)
			{
				String n = blockUidList.get(i);
				if (n.equals(uid)) return true;
			}
		}
		else if (type == BAN_LIST)
		{
			for (int i = 0; i < banUidList.size(); i++)
			{
				String n = banUidList.get(i);
				if (n.equals(uid)) return true;
			}
		}
		else if (type == REPORT_LIST)
		{
			for (int i = 0; i < reportUidList.size(); i++)
			{
				String n = reportUidList.get(i);
				if (n.equals(uid)) return true;
			}
		}

		return false;
	}
	
	public void checkUserExist(String uid)
	{
		UserInfo user = UserManager.getInstance().getUser(uid);

		if (user == null)
		{
			if (user == null){
				user = new UserInfo(uid);
				UserManager.getInstance().addUser(user);
			}
			ArrayList<String> uids = new ArrayList<String>();
			uids.add(uid);
			UserManager.getInstance().getMultiUserInfo(uids);
		}
	}
	
	long lastAddUidTime = 0;
	long lastCallSuccessTime = 0;
	long lastCallTime = 0;
	long CALL_TIME_OUT = 8000;
	private static final int GET_USER_INFO_UID_COUNT = 20;
	// 实际向后台发送了请求的uid列表
	private ArrayList<String> fechingUids = new ArrayList<String>();
	// 请求的uid队列
	private ArrayList<String> queueUids = new ArrayList<String>();
	public synchronized void getMultiUserInfo(ArrayList<String> uids)
	{
		for (int i = 0; i < uids.size(); i++)
		{
			String uid = uids.get(i);
			if(!fechingUids.contains(uid) && !queueUids.contains(uid)){
				queueUids.add(uid);
				lastAddUidTime = System.currentTimeMillis();
				if(getMultiUserInfoTimer == null){
					startTimer();
				}
			}
		}
	}

	private Timer getMultiUserInfoTimer;

	public synchronized void stopTimer() {
		if (getMultiUserInfoTimer != null){
			System.out.println("getMultiUserInfoTimer.stopTimer: " + getMultiUserInfoTimer);
			getMultiUserInfoTimer.cancel();
			getMultiUserInfoTimer.purge();
			getMultiUserInfoTimer = null;
		}
	}
	
	private boolean isStackClear()
	{
		return queueUids.size() == 0 && fechingUids.size() == 0;
	}
	
	private synchronized void startTimer()
	{
		// stopTimer()之后，如果被getMultiUserInfo线程抢占，可能会调两次startTimer()，导致开启多个TimerTask，且不被中止
		// 可能会导致OutOfMemoryError
		if (getMultiUserInfoTimer != null){
			return;
		}
		
		getMultiUserInfoTimer = new Timer();
		System.out.println("getMultiUserInfoTimer.startTimer: " + getMultiUserInfoTimer);

		TimerTask timerTask = new TimerTask() {
			@Override
			public void run() {
				long now = System.currentTimeMillis();
				
				if(isStackClear()){
					stopTimer();
				}
				else if((now - lastAddUidTime) > 500 && (isLastCallSuccess() || isLastCallTimeOut())){
					System.out.println("TimerTask.run() can actual call, now:"+now+"   lastCallSuccessTime:"+lastCallSuccessTime);
					actualCall();
					stopTimer();
					if(!isStackClear()){
						startTimer();
					}
				}
			}

		};
		
		try
		{
			// Fatal Exception: java.lang.NullPointerException (每天1k左右)
			// Attempt to invoke virtual method 'void java.util.Timer.schedule(java.util.TimerTask, long, long)' on a null object reference
			getMultiUserInfoTimer.schedule(timerTask, 0, 100);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
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
		System.out.println("1 fechingUids.size()="+fechingUids.size()+" queueUids.size()="+queueUids.size());
		if(fechingUids.size() > 0){
			if(isLastCallSuccess()){
				// 成功返回，但有些uid取不到 
				LogUtil.trackMessage("取不到：fechingUids is not empty", "", "");
				fechingUids.clear();
			}else{
				// 1.如果正有别的命令在调用，会放弃调用，导致超时; 2.后台接口未返回，导致超时
				LogUtil.trackMessage("超时：fechingUids is not empty", "", "");
			}
		}
		int count = queueUids.size() > (GET_USER_INFO_UID_COUNT - fechingUids.size()) ? (GET_USER_INFO_UID_COUNT - fechingUids.size()) : queueUids.size();
		for (int i = 0; i < count; i++)
		{
			fechingUids.add(queueUids.remove(0));
		}
		System.out.println("2 fechingUids.size()="+fechingUids.size()+" queueUids.size()="+queueUids.size());
		String uidsStr = ChatChannel.getMembersString(fechingUids);
		ChatServiceController.getInstance().host.getMultiUserInfo(uidsStr);
		lastCallTime = System.currentTimeMillis();
		lastCallSuccessTime = System.currentTimeMillis() * 2;
		System.out.println("actualCall() uidsStr = " + uidsStr + " lastCallSuccessTime = " + lastCallSuccessTime);
	}
	
	public synchronized void onReceiveUserInfo(UserInfo[] userInfoArray)
	{
		System.out.println("onReceiveUserInfo(): "+userInfoArray.length);
		
		for(int i=0;i<userInfoArray.length;i++)
		{
			UserInfo user = userInfoArray[i];
//			System.out.println(i + "/" + userInfoArray.length + " user id:" + user.uid);
			putChatRoomMemberInMap(user);
			user.initNullField();
			UserInfo oldUser = getUser(user.uid);
			if(fechingUids.contains(user.uid)){
				fechingUids.remove(user.uid);
			}
			
			if (oldUser == null)
			{
				LogUtil.trackMessage("onReceiveUserInfo(): oldUser is null (impossible): " + user.uid, "", "");
//				System.out.println(LogUtil.typeToString(user));
				addUser(user);
			}
			else if (oldUser.isDummy || user.lastUpdateTime > oldUser.lastUpdateTime)
			{
//				System.out.println("onReceiveUserInfo(): update user");
//				System.out.println(LogUtil.compareObjects(new Object[] { user, oldUser }));
				updateUser(user);
			}
			else
			{
				// 若超时调用返回了
				LogUtil.trackMessage("onReceiveUserInfo(): user is not newer: " + user.uid, "", "");
//				System.out.println(LogUtil.compareObjects(new Object[] { user, oldUser }));
			}
		}
		lastCallSuccessTime = System.currentTimeMillis();
		lastAddUidTime = System.currentTimeMillis();
		ChatServiceController.getInstance().notifyCurrentDataSetChanged();
	}
	
	public void onReceiveSearchUserInfo(UserInfo[] userInfoArray)
	{
		final ArrayList<UserInfo> userArr=new ArrayList<UserInfo>();
		ArrayList<String> nonAllianceMemberArr=getSelctedMemberArr(false);
		for(int i=0;i<userInfoArray.length;i++)
		{
			UserInfo user = userInfoArray[i];
			if(nonAllianceMemberArr.contains(user.uid))
				continue;
			userArr.add(user);
			putChatRoomMemberInMap(user);
			user.initNullField();
			UserInfo oldUser = getUser(user.uid);
			
			if (oldUser == null)
			{
//				System.out.println("impossible >>>>> onReceiveUserInfo(): oldUser is null");
//				System.out.println(LogUtil.typeToString(user));
				addUser(user);
			}
			else if (oldUser.isDummy || user.lastUpdateTime > oldUser.lastUpdateTime)
			{
//				System.out.println("onReceiveUserInfo(): update user");
//				System.out.println(LogUtil.compareObjects(new Object[] { user, oldUser }));
				updateUser(user);
				ChatServiceController.getInstance().notifyCurrentDataSetChanged();
			}
		}
		System.out.println("onReceiveSearchUserInfo userArr:"+userArr.size());
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if(ChatServiceController.getMemberSelectorFragment() != null)
					{
						ChatServiceController.getMemberSelectorFragment().refreshSearchListData(userArr);
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
		
	}
	
	public void putNonAllianceInMap(UserInfo user)
	{
		if(user==null)
			return;
//		System.out.println("putNonAllianceInMap name:"+user.userName);
		String uid=user.uid;
		nonAllianceMemberInfoMap.put(uid, user);
	}
	
	public void putChatRoomMemberInMap(UserInfo user)
	{
//		System.out.println("putChatRoomMemberInMap useruid:"+user.uid+"  rank:"+user.allianceRank+"  userName:"+user.userName);
		int rank=user.allianceRank;
		String uid=user.uid;
		String allianceId=UserManager.getInstance().getCurrentUser().allianceId;
		if(allianceId!=null && allianceId.equals(user.allianceId))
		{
			allianceMemberInfoMap.put(uid, user);
			
			if(rank>0)
			{
				String rankKey=getRankLang(rank);
				rankMap.put(rankKey, Integer.valueOf(rank));
//				System.out.println("putChatRoomMemberInMap rankMap size:"+rankMap.size());
				resetAllianceRank(rankKey);
				ArrayList<UserInfo> userArr=allianceMemberMap.get(rankKey);
				boolean isInRank=false;
				for(int i=0;i<userArr.size();i++)
				{
					UserInfo info=userArr.get(i);
					if(info.uid.equals(user.uid))
					{
						allianceMemberMap.get(rankKey).remove(info);
						allianceMemberMap.get(rankKey).add(user);
						isInRank=true;
						return;
					}
				}
				if(!isInRank)
					allianceMemberMap.get(rankKey).add(user);
			}
		}
		else
		{
			putNonAllianceInMap(user);
		}
	}
	
	public String getRankLang(int rank)
	{
		String rankStr="";
		switch(rank)
		{
		case 1:
			rankStr=LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK1);
			break;
		case 2:
			rankStr=LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK2);
			break;
		case 3:
			rankStr=LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK3);
			break;
		case 4:
			rankStr=LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK4);
			break;
		case 5:
			rankStr=LanguageManager.getLangByKey(LanguageKeys.TITLE_RANK5);
			break;
		}
		return rankStr;
	}
	
	private void resetAllianceRank(String key)
	{
		if(allianceMemberMap.containsKey(key))
			return;
		ArrayList<UserInfo> userInfoArray=new ArrayList<UserInfo>();
		allianceMemberMap.put(key, userInfoArray);
	}
	
	public void clearAllianceMember()
	{
		if(allianceMemberMap!=null)
			allianceMemberMap.clear();
		if(allianceMemberInfoMap!=null)
			allianceMemberInfoMap.clear();
		if(rankMap!=null)
			rankMap.clear();
	}
	
	public void clearNonAllianceMember()
	{
		if(nonAllianceMemberInfoMap!=null)
			nonAllianceMemberInfoMap.clear();
	}
	
	public HashMap<String,ArrayList<UserInfo>> getChatRoomMemberMap()
	{
		return allianceMemberMap;
	}
	
	public HashMap<String, UserInfo> getChatRoomMemberInfoMap()
	{
		return allianceMemberInfoMap;
	}
	
	public HashMap<String, UserInfo> getNonAllianceMemberInfoMap() {
		return nonAllianceMemberInfoMap;
	}

	public HashMap<String, Integer> getRankMap() {
		return rankMap;
	}

	public boolean isMoreThanOneMember()
	{
		boolean ret=false;
		Set<String> keySet=allianceMemberMap.keySet();
		if(keySet.size()>1)
		{
			ret=true;
		}
		else if(keySet.size()==1)
		{
			for(String key:keySet)
			{
				ret=allianceMemberMap.get(key).size()>1;
			}
		}
		
		return ret;
	}
	
	public String createUidStr(ArrayList<String> uidArr)
	{
		String uidStr="";
		for(int i=0;i<uidArr.size();i++)
		{
			if(!uidArr.get(i).equals(""))
			{
				if(!uidStr.equals(""))
					uidStr=uidStr+"|"+uidArr.get(i);
				else
					uidStr=uidArr.get(i);
			}
		}
		return uidStr;
		
	}
	
	public String createNameStr(ArrayList<String> uidArr)
	{
		String nameStr="";
		for(int i=0;i<uidArr.size();i++)
		{
			if(!uidArr.get(i).equals(""))
			{
				String uid=uidArr.get(i);
				
				UserInfo user=null;
				if(allianceMemberInfoMap.containsKey(uid))
				{
					user=allianceMemberInfoMap.get(uid);
				}
				else if(nonAllianceMemberInfoMap.containsKey(uid))
				{
					user=nonAllianceMemberInfoMap.get(uid);
				}
				
				if(user==null)
					user=getUser(uid);
				
				if(user!=null)
				{
					if(!nameStr.equals(""))
						nameStr=nameStr+"、"+user.userName;
					else
						nameStr=user.userName;
				}
			}
		}
		return nameStr;
	}
	
	public ArrayList<String> getSelectMemberUidArr()
	{
		ArrayList<String> memberUidArray=new ArrayList<String>();
		if(ChatServiceController.isCreateChatRoom)
    	{
			System.out.println("getSelectMemberUidArr 11 uid:"+UserManager.getInstance().getCurrentUser().uid);
    		
    		if(!UserManager.getInstance().getCurrentUser().uid.equals(""))
    			memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
    	}
    	else
    	{
    		if(!ChatServiceController.isInMailDialog())
    			return null;
    		System.out.println("getSelectMemberUidArr 22");
    		if(!ChatServiceController.isInChatRoom())
        	{
        		if(!UserManager.getInstance().getCurrentUser().uid.equals(""))
        			memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
        	}
        	else
        	{
        		memberUidArray=ChannelManager.getInstance().getChatRoomMemberArrayByKey(getCurrentMail().opponentUid);
        	}
    	}
    	return memberUidArray;
	}
	
	public ArrayList<String> getSelctedMemberArr(boolean isFromAlliance)
	{
		ArrayList<String> memberUidArray=new ArrayList<String>();
		boolean isInAlliance=!UserManager.getInstance().getCurrentUser().allianceId.equals("");
		
		if(ChatServiceController.isCreateChatRoom || (ChatServiceController.isInMailDialog() && !ChatServiceController.isInChatRoom()))
    	{
    		if(((isInAlliance && isFromAlliance) ||(!isInAlliance && !isFromAlliance)) && !UserManager.getInstance().getCurrentUser().uid.equals(""))
    			memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
    	}
    	else if(ChatServiceController.isInChatRoom())
    	{
    		memberUidArray=new ArrayList<String>();
    		HashMap<String, UserInfo> memberInfoMap=UserManager.getInstance().getChatRoomMemberInfoMap();
    		Set<String> uidKeySet=memberInfoMap.keySet();
    		List<String> userArray=ChannelManager.getInstance().getChatRoomMemberArrayByKey(getCurrentMail().opponentUid);
    		for(int i=0;i<userArray.size();i++)
    		{
    			String uid=userArray.get(i);
    			if(!uid.equals("") && (isFromAlliance &&uidKeySet.contains(uid)) ||(!isFromAlliance && !uidKeySet.contains(uid)))
    				memberUidArray.add(uid);
    		}
    	}
    	return memberUidArray;
	}
	
	public HashMap<String, ArrayList<UserInfo>> getJoinedMemberMap(String key,List<String> uidArr)
	{
		HashMap<String, ArrayList<UserInfo>> map=new HashMap<String, ArrayList<UserInfo>>();
		
		if(uidArr!=null && uidArr.size()>0)
		{
			System.out.println("getJoinedMemberMap uidArr size:"+uidArr.size());
			ArrayList<UserInfo> userArr=new ArrayList<UserInfo>();
			ArrayList<String> userUidArray=new ArrayList<String>();
			HashMap<String, UserInfo> memberInfoMap=UserManager.getInstance().getNonAllianceMemberInfoMap();
			for(int i=0;i<uidArr.size();i++)
			{
				String uid=uidArr.get(i);
				if(!uid.equals(""))
				{
					if(memberInfoMap.containsKey(uid))
						userArr.add(memberInfoMap.get(uid));
					else
					{
						UserInfo user=getUser(uid);
						if(user!=null)
						{
//							System.out.println("getJoinedMemberMap getUser !=null");
							userArr.add(user);
						}
						else
						{
//							System.out.println("getMultiUserInfo() getJoinedMemberMap getUser " + uid + " == null");
							userUidArray.add(uid);
						}
					}
				}
			}
			if(userUidArray.size()>0)
				getMultiUserInfo(userUidArray);
			
			if(userArr.size()>0)
				map.put(key, userArr);
		}
		return map;
	}
}
