package com.elex.chatservice.controller;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;
import android.content.Intent;
import android.view.Gravity;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.R;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.ChatTable;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.updatedata.MailUpdateData;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.TranslatedByLuaResult;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;
import com.elex.chatservice.view.ChatActivity;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.ForumActivity;
import com.elex.chatservice.view.ForumFragment;
import com.elex.chatservice.view.ICocos2dxScreenLockListener;
import com.elex.chatservice.view.MemberSelectorActivity;
import com.elex.chatservice.view.WriteMailActivity;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class ServiceInterface {
	public static String allianceIdJoining;
	public static void onJoinAnnounceInvitationSuccess()
	{
		System.out.println("allianceIdJoining:"+allianceIdJoining);
		UserManager.getInstance().getCurrentUser().allianceId = allianceIdJoining;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getChatFragment() != null) {
						ChatServiceController.getChatFragment().onJoinAnnounceInvitationSuccess();
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void setGameLanguage(String gameLanguage)
	{
		ConfigManager.getInstance().gameLang=gameLanguage;
	}

	public static void toggleFullScreen(final boolean enabled){
		ChatServiceController.toggleFullScreen(enabled, true, ChatServiceController.hostActivity);
	}

	public static void setMailInfo(String mailFromUid, String mailUid, String mailName, int mailType)
	{
		System.out.println("mailType:"+mailType);
		UserManager.getInstance().getCurrentMail().opponentUid=mailFromUid;
		UserManager.getInstance().getCurrentMail().mailUid=mailUid;
		UserManager.getInstance().getCurrentMail().opponentName=mailName;
		UserManager.getInstance().getCurrentMail().type=mailType;
		if(mailType == MailManager.MAIL_MOD_PERSONAL || mailType == MailManager.MAIL_MOD_SEND)
			ChatServiceController.isContactMod = true;
	}
	
	public static void setContactModState()
	{
		System.out.println("setContactModState");
		ChatServiceController.isContactMod = true;
	}
	
	public static void resetPlayerFirstJoinAlliance()
	{
		ConfigManager.getInstance().isFirstJoinAlliance=false;
		ServiceInterface.resetPlayerIsInAlliance();
	}

	public static void postNoMoreMessage(int channelType)
	{
		if (ChatServiceController.getChatFragment() != null) {
			ChatServiceController.getChatFragment().resetMoreDataStart(channelType);
		}
	}
	
	//删除聊天室本地消息
	public static void deleteChatRoom(String groupId)
	{
		System.out.println("deleteChatRoom groupId:"+groupId);
		ChannelManager.getInstance().deleteChatroomChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
	}
	
	public static void deleteMail(String id,int channelType,int type)
	{
		System.out.println("deleteMail id:" + id + "type:" + type);
		if(channelType==DBDefinition.CHANNEL_TYPE_USER || channelType==DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			ChatChannel channel=ChannelManager.getInstance().getChannel(channelType,id);
			if(channel == null) return;
			ChannelManager.getInstance().deleteChannel(channel);
		}
		else if(channelType==DBDefinition.CHANNEL_TYPE_OFFICIAL && ChatServiceController.isNewMailListEnable)
		{
			if(type == MailManager.MAIL_RESOURCE || (type == MailManager.MAIL_RESOURCE_HELP && ChatServiceController.isNewMailUIEnable) || type == MailManager.MAIL_ATTACKMONSTER)
			{
				String channelId = "";
				if(type == MailManager.MAIL_RESOURCE)
					channelId = MailManager.CHANNELID_RESOURCE;
				else if(type == MailManager.MAIL_RESOURCE_HELP)
					channelId = MailManager.CHANNELID_RESOURCE_HELP;
				else if(type == MailManager.MAIL_ATTACKMONSTER)
					channelId = MailManager.CHANNELID_MONSTER;
				ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
				if(channel!=null)
				{
					channel.clearAllSysMail();
				}
			}
			else
			{
				MailData mail = DBManager.getInstance().getSysMailByID(id);
				if(mail!=null)
				{
					System.out.println("deleteMail channelId:" + mail.getChannelId());
					ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
					if(channel == null) return;
					ChannelManager.getInstance().deleteSysMailFromChannel(channel,id);
				}
			}
			
			if(ChatServiceController.hostActivity == null) return;
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						if (ChatServiceController.getChannelListFragment() != null) {
							ChatServiceController.getChannelListFragment().notifyDataSetChanged();
						}
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	public static void setChannelMemberArray(int channelType,String fromUid,String uidStr,String roomName)
	{
		System.out.println("setChannelMemberArray:"+fromUid+"  uidStr:"+uidStr + "  roomName:"+roomName);
		ChannelManager.getInstance().setChannelMemberArray(fromUid, uidStr,roomName);
		
		if(ChatServiceController.hostActivity == null || channelType!=DBDefinition.CHANNEL_TYPE_CHATROOM) return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getChatFragment() != null) {
						if(!ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{	
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}
	
	public static void updateChannelMemberArray(int channelType,String fromUid,String uidStr,boolean isAdd)
	{
		System.out.println("updateChannelMemberArray:"+fromUid+"  uidStr:"+uidStr+"  isAdd:"+isAdd);
		ChannelManager.getInstance().updateChannelMemberArray(fromUid, uidStr,isAdd);
		
		if(ChatServiceController.hostActivity == null || channelType!=DBDefinition.CHANNEL_TYPE_CHATROOM || !isAdd) return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getChatFragment() != null) {
						if(!ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{	
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}
	
	public static void setChatRoomFounder(String groupId,String founderUid)
	{
		System.out.println("setChatRoomFounder groupId:"+groupId+" founderUid:"+founderUid);
		ChannelManager.getInstance().setChatRoomFounder(groupId, founderUid);
	}

	public static void setChatHorn(boolean enableChatHorn)
	{
		System.out.println("enableChatHorn:"+enableChatHorn);
		ConfigManager.enableChatHorn=enableChatHorn;
	}
	
	private static void save2DB(MsgItem[] infoArr, final int channelType, final String channelId, String customName)
	{
		System.out.println("save2DB()" + " channelType:" + channelType + " fromId:" + channelId + " length:" + infoArr.length);
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
		if(infoArr.length == 0 || channel == null) return;

		if(channelType==DBDefinition.CHANNEL_TYPE_USER || channelType==DBDefinition.CHANNEL_TYPE_CHATROOM)
			channel.customName=customName;
		boolean isNewMessage = infoArr[0].isNewMsg;
		ArrayList<MsgItem> filteredArray = new ArrayList<MsgItem>();
		for (int i = 0; i < infoArr.length; i++)
		{
//			if (!(isNewMessage && infoArr[i].isSelfMsg && (!infoArr[i].isSystemMessage() || infoArr[i].isHornMessage())))
//			{
//				System.out.println(LogUtil.typeToString(infoArr[i]));
				filteredArray.add(infoArr[i]);
//			}
		}
		System.out.println("filteredArray.size():" + filteredArray.size());
		
		if(filteredArray.size() > 0)
			DBManager.getInstance().insertMessages(filteredArray, channel.getChatTable());
	}
	
	private static void updateDBMsg(MsgItem msg, final int channelType, final String fromId)
	{
		System.out.println("updateDBMsg()");
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, fromId);
		if(channel == null) return;
		
//		MsgItem dbItem = DBManager.getInstance().getMessage(msg, channel.getChatTable());
//		if(dbItem != null){
//			System.out.println("DB item:" + LogUtil.typeToString(dbItem));
//			System.out.println("Success item:" + LogUtil.typeToString(msg));
//			System.out.println("DB item vs Success item");
//			System.out.println(LogUtil.compareObjects(new Object[]{dbItem, msg}));
//			System.out.println("DB item user vs Success item user");
//			System.out.println(LogUtil.compareObjects(new Object[]{dbItem.getUser(), msg.getUser()}));
//		}
		
		DBManager.getInstance().updateMyMessageStatus(msg, channel.getChatTable());

//		MsgItem dbItem2 = DBManager.getInstance().getMessage(msg, channel.getChatTable());
//		if(dbItem2 != null && dbItem != null){
//			System.out.println("new DB item:" + LogUtil.typeToString(dbItem2));
//			System.out.println("new DB item vs Success item");
//			System.out.println(LogUtil.compareObjects(new Object[]{dbItem2, msg}));
//			System.out.println("new DB item user vs Success item user");
//			System.out.println(LogUtil.compareObjects(new Object[]{dbItem2.getUser(), msg.getUser()}));
//		}
	}
	
	/**
	 * 获取多条历史消息时，此函数会被多次调用（具体次数由MailCell、MailController决定），多次刷新界面（游戏中看不出来）
	 * 
	 * @param channelId 如果是邮件则总是指对方的uid，如果是聊天室为uid，如果是聊天fromUid为"0"
	 * @param customName 如果是邮件则总是指对方的name，如果是聊天室为自定义名称，如果是聊天为""
	 */
	public static void notifyMessageIndex(int chatInfoIndex, String channelId,String customName,boolean isModMsg)
	{
		System.out.println("notifyMessageIndex()： fromUid:" + channelId + " chatInfoIndex:" + chatInfoIndex+" customName:"+customName + " isModMsg:"+isModMsg);
		if (chatInfoIndex < 0 || channelId.equals("")) return;
		final MsgItem[] chatInfoArr = ChatServiceController.getInstance().host.getChatInfoArray(chatInfoIndex, channelId);
		if(isModMsg && !channelId.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
			channelId += DBDefinition.CHANNEL_ID_POSTFIX_MOD;
		if (chatInfoArr==null || chatInfoArr.length <= 0) return;
		System.out.println("chatInfoArr:" + chatInfoArr.length);
		handleMessage(chatInfoArr, channelId,customName);
	}
	
	public static synchronized void parseMailData(MailData mailData, boolean isFromDB)
	{
		boolean hasParseComplexOld = mailData.hasParseCompex;
		MailData mail=MailManager.getInstance().parseMailDataContent(mailData);
		boolean hasParseComplexNew = mail.hasParseCompex;
		boolean isComplexParsed = !hasParseComplexOld && hasParseComplexNew;
		handleMailData(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail, isFromDB ,isComplexParsed);
	}
	
	private static ArrayList<Integer> mailDataIndexArray = new ArrayList<Integer>();
	public static void notifyMailDataIndex(final int mailDataIndex, boolean isGetNew)
	{
		System.out.println("handleGetNewMailMsg notifyMailDataIndex() mailDataIndex：" + mailDataIndex + " isGetNew：" + isGetNew);
		if(isGetNew){
			mailDataIndexArray.add(Integer.valueOf(mailDataIndex));
		}else{
			Runnable run = new Runnable()
			{
				@Override
				public void run()
				{
					handleMailDataIndex(mailDataIndex, false);
				}
			};
			Thread thread = new Thread(run);
			thread.start();
		}
	}
	
	public static boolean isHandlingGetNewMailMsg = false;
	public static void handleGetNewMailMsg(final String channelInfo)
	{
		System.out.println("handleGetNewMailMsg channelInfo:"+ channelInfo);
		Runnable run = new Runnable()
		{
			@Override
			public void run()
			{
				isHandlingGetNewMailMsg = true;
				
				long startTime = System.currentTimeMillis();
				System.out.println("handleGetNewMailMsg mailDataIndexArray:"+ mailDataIndexArray);
				System.out.println("handleGetNewMailMsg handleMailDataIndex开始:"+ startTime);
				for (int i = 0; i < mailDataIndexArray.size(); i++)
				{
					handleMailDataIndex(mailDataIndexArray.get(i).intValue(), true);
				}
				mailDataIndexArray.clear();
				long offsetTime = System.currentTimeMillis() - startTime;
				System.out.println("handleGetNewMailMsg handleMailDataIndex时间:"+ offsetTime);
				System.out.println("handleGetNewMailMsg handleMailDataIndex完成:"+TimeManager.getInstance().getCurrentTime());
				
				postChannelInfo(channelInfo);
				
				isHandlingGetNewMailMsg = false;
				
				ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						try {
							if (ChatServiceController.getChannelListFragment() != null) {
								ChatServiceController.getChannelListFragment().refreshTitleLabel();
							}
						} catch (Exception e) {
							LogUtil.printException(e);
						}
					}
				});
			}
		};

		Thread thread = new Thread(run);
		thread.start();
	}

	private static synchronized void handleMailDataIndex(final int mailDataIndex, boolean isGetNew)
	{
		System.out.println("handleGetNewMailMsg handleMailDataIndex() mailDataIndex：" + mailDataIndex);
		
		if (mailDataIndex < 0) return;
		
		final MailData[] mailDataArr = ChatServiceController.getInstance().host.getMailDataArray(mailDataIndex);
		if(mailDataArr == null) return;
		System.out.println("handleGetNewMailMsg mailDataArr size：" + mailDataArr.length);
		
		for(int i=0;i<mailDataArr.length;i++)
		{
			MailData mailData=mailDataArr[i];
			if(mailData == null)
				continue;
			System.out.println(i + " " + mailData.getUid());
			
			if(!isGetNew){
				parseMailData(mailData, false);
			}else{
				mailData.parseMailTypeTab();
				ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mailData.getChannelId());
				if(channel != null){
					System.out.println("handleMailDataIndex channel != null");
					DBManager.getInstance().insertMailData(mailData, channel);
					channel.refreshRenderData();
				}else{
					System.out.println(i + " " + mailData.getChannelId() + " " + mailData.getType());
				}
			}
		}
		ChannelListFragment.onMailAdded();
	}
	
	public static void handleMailData(int channelType,final MailData mailData,boolean isFromDB,boolean isComplexParsed)
	{
		if(mailData==null)
			return;
		
		boolean isWorldBossKillRewardMail = false;
		if(mailData.isWorldBossKillRewardMail())
		{
			isWorldBossKillRewardMail = true;
			mailData.setType(MailManager.MAIL_WORLD_BOSS);
		}
		
//		System.out.println("handleMailData channelType:" + channelType + " isFromDB:" + isFromDB + " isComplexParsed:" + isComplexParsed + " type:" + mailData.getType() + " getChannelId:"+mailData.getChannelId() + " uid:" + mailData.getUid());
		final ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, mailData.getChannelId());
		if(channel == null) return;
//		System.out.println("handleMailData 2");
		if(!isFromDB)
		{
			DBManager.getInstance().insertMailData(mailData, channel);
		}
		else
		{
			if(isComplexParsed || isWorldBossKillRewardMail)
			{
				mailData.channelId = mailData.getChannelId();
				DBManager.getInstance().updateMail(mailData);
			}
		}

		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					channel.addNewMailData(mailData);
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
//		if(ChatServiceController.isNewMailListEnable && mailData.isParseFrom2dxType())
//		{
//			MailManager.getInstance().enterParseFrom2dxQueue(mailData);
//		}
	}

	public static void handleMessage(final MsgItem[] chatInfoArr, final String channelId,final String customName)
	{
		for (int i = 0; i < chatInfoArr.length; i++)
		{
			chatInfoArr[i].sendState = MsgItem.SEND_SUCCESS;
			// 存储用户信息
			chatInfoArr[i].initUserForReceivedMsg(channelId, customName);
			if(chatInfoArr[i].hasTranslation() && !chatInfoArr[i].isTranlateDisable() && !chatInfoArr[i].isOriginalSameAsTargetLang())
				chatInfoArr[i].hasTranslated = true;
			else
				chatInfoArr[i].hasTranslated = false;
		}

		final int channelType = chatInfoArr[0].channelType;
		final boolean isNewMessage = chatInfoArr[0].isNewMsg;
//		final ArrayList<MsgItem> curMsgList = ChannelManager.getInstance().getCurMsgList(channelType, fromUid);
		
		System.out.println("handleMessage channelType:"+channelType);
		save2DB(chatInfoArr, channelType, channelId, customName);

		if (ChatServiceController.getChatFragment() != null && isNewMessage)
		{
			ChatServiceController.getChatFragment().refreshIsInLastScreen(channelType);
		}
		
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					handleMessage2(channelType, isNewMessage, chatInfoArr, channelId, customName);
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
		
		if(ConfigManager.autoTranlateMode > 0)
		{
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				TranslateManager.getInstance().enterTranlateQueue(chatInfoArr[i]);
			}
		}
		
	}
	
	private static void handleMessage2(final int channelType, final boolean isNewMessage, final MsgItem[] chatInfoArr,
			final String channelId, String customName)
	{
		System.out.println("channelType:" + channelType + " fromUid:" + channelId);
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
		if (channel == null) return;
		
		if (channelType == DBDefinition.CHANNEL_TYPE_USER || channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			channel.customName = customName;
		}
		
		if (chatInfoArr.length <= 0) return;

		ArrayList<MsgItem> msgList = channel.msgList;
		ArrayList<MsgItem> sendingMsgList = channel.sendingMsgList;
		ChannelManager.getInstance().setHasRequestDataBeforeFlag(channelType, channelId, true);
		
		if (isNewMessage)
		{
			System.out.println("新消息");
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				MsgItem sendingMsg = null;
				MsgItem recievedMsg = chatInfoArr[i];
//				System.out.println("recievedMsg: " + LogUtil.typeToString(recievedMsg));
				if (msgList != null && msgList.size() > 0){
					for (int j = 0; j < sendingMsgList.size(); j++)
					{
						MsgItem sendMsg = sendingMsgList.get(j);
						if(sendMsg == null) continue;
						
						if (sendMsg.sendLocalTime != 0 && sendMsg.sendLocalTime == recievedMsg.sendLocalTime)
						{
							sendingMsg = sendMsg;
						}
					}
				}
				
				// 我发的消息
				if (sendingMsg != null && recievedMsg.isSelfMsg && (!recievedMsg.isSystemMessage() || recievedMsg.isHornMessage()))
				{
//					System.out.println("成功发送后，找到假msg: " + sendingMsg + " index:" + channel.msgList.indexOf(sendingMsg));
					System.out.println("	1.自己发的消息");
					sendingMsg.sendState = MsgItem.SEND_SUCCESS;
					sendingMsgList.remove(sendingMsg);
					channel.replaceDummyMsg(recievedMsg, msgList.indexOf(sendingMsg));
//					System.out.println("添加真消息: " + recievedMsg + " index:" + channel.msgList.indexOf(recievedMsg));
//					updateDBMsg(recievedMsg, channelType, fromUid);
				}
				else
				{
					System.out.println("	2.一般消息");
					channel.addNewMsg(recievedMsg);
				}
				
				if (ChatServiceController.getChatFragment() != null)
				{
					ChatServiceController.getChatFragment().notifyDataSetChanged(channelType);
					ChatServiceController.getChatFragment().updateListPositionForNewMsg(channelType, recievedMsg.isSelfMsg);
				}
			}
		}
		else
		{
			System.out.println("旧消息");
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				channel.addHistoryMsg(chatInfoArr[i]);
			}

			System.out.println("handleMessage curMsgList size:"+msgList.size());
			if (ChatServiceController.getChatFragment() != null)
			{
				ChatServiceController.getChatFragment().notifyDataSetChanged(channelType);
				ChatServiceController.getChatFragment().updateListPositionForOldMsg(channelType, chatInfoArr.length);
				ChatServiceController.getChatFragment().resetMoreDataStart(channelType);
			}
		}
		
		ChannelListFragment.onMsgAdded(channel);
	}
	
	public static void notifyChatRoomNameChanged(final String modifyName)
	{
		UserManager.getInstance().getCurrentMail().opponentName=modifyName;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						ChatServiceController.getChatFragment().changeChatRoomName(modifyName);
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void postChannelNoMoreData(int channelType,boolean hasNoMoreData)
	{
		if(StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId())) return;
		
		ChannelManager.getInstance().setNoMoreDataFlag(ChannelManager.channelType2tab(channelType), hasNoMoreData);
	}

	public static void removeAllMailByUid(String fromUid)
	{
		System.out.println("removeAllMailByUid  fromUid:"+fromUid);
		if(StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId())) return;
		
		ChannelManager.getInstance().removeAllMailMsgByUid(fromUid);
	}

	public static boolean getHasRequestDataBefore(String fromUid)
	{
		System.out.println("getHasRequestDataBefore  fromUid:"+fromUid);
		if(StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId())) return false;
		
		return ChannelManager.getInstance().getHasRequestMailDataBefore(fromUid);
	}

	public static void clearCurMailData()
	{
		if (!ChatServiceController.isInMailDialog()) 
			return;

//		ChatActivity.isInMailDialog = false;
//		ChatActivity.isInChatRoom=false;
//		if(ChannelManager.getInstance().mailMsgList!=null)
//		{
//			ChatServiceController.activity.runOnUiThread(new Runnable() {
//				@Override
//				public void run() {
//					try {
//						ChannelManager.getInstance().mailMsgList.clear();
//				set		if (ChatServiceController.chatActivity != null && ChatServiceController.chatActivity.chatFragment != null) {
//							ChatServiceController.chatActivity.chatFragment.notifyDataSetChanged(3);
//						}
//					} catch (Exception e) {
//						LogUtil.printException(e);
//					}
//				}
//			});
//		}
	}

	public static void setIsNewMailListEnable(boolean enable)
	{
		System.out.println("setIsNewMailListEnable() enable:" + enable);
		ChatServiceController.isNewMailListEnable = enable;
	}
	
	public static void setCurrentUserId(String uidStr)
	{
		System.out.println("setCurrentUserId() uid:" + uidStr);
		UserManager.getInstance().setCurrentUserId(uidStr);
	}
	
	private static UserInfo currentUserClone;
	/**
	 * 初始登录、重新登录、切服时会调用
	 * @param worldTime utc时间，单位为s
	 */
	public static void setPlayerInfo(int country, int worldTime, int gmod, int headPicVer, String name, String uidStr, String picStr,
			int vipLevel, int vipEndTime, int lastUpdateTime, int crossFightSrcServerId)
	{
		System.out.println("setPlayerInfo() uid:" + uidStr);
		TimeManager.getInstance().setServerBaseTime(worldTime);
//		UserManager.getInstance().setCurrentUserId(uidStr);
		currentUserClone = (UserInfo) UserManager.getInstance().getCurrentUser().clone();
//		System.out.println("before setPlayerInfo()\n" + LogUtil.typeToString(currentUserClone));
		UserManager.getInstance().getCurrentUser().serverId = country;
		ChatServiceController.serverId = country;
		
		UserManager.getInstance().getCurrentUser().headPicVer = headPicVer;
		UserManager.getInstance().getCurrentUser().mGmod = gmod;
		UserManager.getInstance().getCurrentUser().userName = name;
		UserManager.getInstance().getCurrentUser().headPic = picStr;
		UserManager.getInstance().getCurrentUser().vipLevel = vipLevel;
		UserManager.getInstance().getCurrentUser().vipEndTime = vipEndTime;
		UserManager.getInstance().getCurrentUser().lastUpdateTime = lastUpdateTime;
		UserManager.getInstance().getCurrentUser().crossFightSrcServerId = crossFightSrcServerId;
		ChatServiceController.crossFightSrcServerId = crossFightSrcServerId;
		
		UserManager.getInstance().updateUser(UserManager.getInstance().getCurrentUser());
		ChannelManager.getInstance().getCountryChannel();
//		System.out.println("headPicVer:"+headPicVer+" isCustomImgExist:"+isCustomImgExist+" customHeadImg:"+customHeadImg+" customHeadImgURL:"+customHeadImgURL+" setPlayerInfo gmod:"+UserManager.getInstance().getCurrentUser().mGmod+" userName:"+UserManager.getInstance().getCurrentUser().userName+" uid:"+UserManager.getInstance().getCurrentUser().uid+" headPic:"+UserManager.getInstance().getCurrentUser().headPic+" vipInfo:"+UserManager.getInstance().getCurrentUser().vipInfo);
	}

	/**
	 * 初始登录时会调用
	 * 打开聊天时，会紧接着setPlayerInfo后面调
	 * 重新登录、切服等时候，会调C++的parseData()刷新联盟信息，也调用此函数
	 */
	public static void setPlayerAllianceInfo(String asnStr,String allianceIdStr,int alliancerank,boolean isFirstJoinAlliance)
	{
		System.out.println("setPlayerAllianceInfo()  allianceIdStr:"+allianceIdStr);
		System.out.println("UserManager.getInstance().getCurrentUser().allianceId:"+UserManager.getInstance().getCurrentUser().allianceId);

		if (UserManager.getInstance().isCurrentUserInAlliance()
				&& !UserManager.getInstance().getCurrentUser().allianceId.equals(allianceIdStr) && ChannelManager.isInited()
				&& ChannelManager.getInstance().getAllianceChannel().msgList != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						ChannelManager.getInstance().getAllianceChannel().msgList.clear();
						if (ChatServiceController.getChatFragment() != null) {
							ChatServiceController.getChatFragment().notifyDataSetChanged(DBDefinition.CHANNEL_TYPE_ALLIANCE);
						}
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}

		UserManager.getInstance().getCurrentUser().asn=asnStr;
		UserManager.getInstance().getCurrentUser().allianceId=allianceIdStr;
		UserManager.getInstance().getCurrentUser().allianceRank=alliancerank;
		ConfigManager.getInstance().isFirstJoinAlliance=isFirstJoinAlliance;

		if(!currentUserClone.equals(UserManager.getInstance().getCurrentUser())){
			System.out.println("current user updated\n" + LogUtil.compareObjects(new Object[] { UserManager.getInstance().getCurrentUser(), currentUserClone }));
			UserManager.getInstance().updateCurrentUser();
		}
		ChannelManager.getInstance().getAllianceChannel();
	}

	// TODO 与setPlayerAllianceInfo总是成对调用，没必要调
	// 但切服之后可能需要调
	public static void resetPlayerIsInAlliance()
	{
		System.out.println("resetPlayerIsInAlliance()");
		if(StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId())) return;
		System.out.println("actual resetPlayerIsInAlliance()");
		
		UserManager.getInstance().clearAllianceMember();
		
		if(UserManager.getInstance().getCurrentUser().allianceId.equals(""))
			return;
		
		if(ChannelManager.isInited())
		{
			if(UserManager.getInstance().isCurrentUserInAlliance() && ChannelManager.getInstance().getAllianceChannel().msgList!=null)
			{
				ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						try {
							// 有时候会发生nullPointer异常
							ChannelManager.getInstance().getAllianceChannel().msgList.clear();
							if (ChatServiceController.getChatFragment() != null) {
								ChatServiceController.getChatFragment().notifyDataSetChanged(DBDefinition.CHANNEL_TYPE_ALLIANCE);
							}
						} catch (Exception e) {
							LogUtil.printException(e);
						}
					}
				});
			}
			ChannelManager.getInstance().setNoMoreDataFlag(1, false);
		}
		
		UserManager.getInstance().getCurrentUser().allianceId="";
		UserManager.getInstance().getCurrentUser().allianceRank=-1;
		
	}
	
	/**
	 * 锁屏时调用
	 * 以前是切换tab时才会获取数据，用chat.get接口，如果发现已经有数据，就不会再获取，所以得先clear一次
	 */
	public static void clearCountryMsg()
	{
	}
	
	// 以前就未调用
	public static void clearMailMsg()
	{
	}
	
	//论坛重新登录
	public static void onPlayerChanged()
	{
		ForumFragment.isFirstLogin = true;
	}

	public static void notifyChangeLanguage()
	{
		LanguageManager.initChatLanguage(ChatServiceController.getInstance().host.getChatLangArray());
	}

	public static void onCreateChatroomSuccess()
	{
		ChatServiceController.isCreateChatRoom = false;
		ServiceInterface.showChatActivity(ChatServiceController.getCurrentActivity(), DBDefinition.CHANNEL_TYPE_CHATROOM, false);
	}

	public static void showChatActivityFrom2dx(int maxHornInputCount, final int chatType,int sendInterval, final boolean rememberPosition,boolean enableCustomHeadImg,boolean isNoticeItemUsed) {
		LanguageManager.initChatLanguage(ChatServiceController.getInstance().host.getChatLangArray());
		ConfigManager.maxHornInputLength=maxHornInputCount;
		ConfigManager.enableCustomHeadImg=enableCustomHeadImg;
		ChatServiceController.isHornItemUsed=isNoticeItemUsed;
		System.out.println("showChatActivityFrom2dx chatType:"+chatType+" sendInterval:"+sendInterval+" rememberPosition:"+rememberPosition+" enableCustomHeadImg:"+enableCustomHeadImg+" isNoticeItemUsed:"+isNoticeItemUsed);
		ConfigManager.sendInterval=sendInterval;
		ChatServiceController.isCreateChatRoom=false;
		if (ChatServiceController.hostActivity != null) {
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						ServiceInterface.showChatActivity(ChatServiceController.hostActivity, chatType, rememberPosition);
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
		
//		ChatServiceController.getInstance().host.getAllianceMember();
	}

	public static void showForumFrom2dx(String url) {
		LanguageManager.initChatLanguage(ChatServiceController.getInstance().host.getChatLangArray());
		final String forumUrl = url;
		if (ChatServiceController.hostActivity != null) {
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						ServiceInterface.showForumActivity(ChatServiceController.hostActivity, forumUrl);
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	public static void showMemberSelectorFrom2dx() {
		if (ChatServiceController.hostActivity != null) {
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						ChatServiceController.isCreateChatRoom=true;
						ServiceInterface.showMemberSelectorActivity(ChatServiceController.hostActivity, false);
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	public static void showChannelListFrom2dx(final boolean isGoBack)
	{
//		ChatServiceController.isNewMailListEnable = true;
		if (ChatServiceController.hostActivity != null) {
			ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
						ServiceInterface.showChannelListActivity(ChatServiceController.hostActivity, false, DBDefinition.CHANNEL_TYPE_OFFICIAL, null, isGoBack);
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	public static void notifyUserInfo(int index)
	{
		System.out.println("notifyUserInfo: index:"+index);
		UserManager.getInstance().onReceiveUserInfo(ChatServiceController.getInstance().host.getUserInfoArray(index));
	}
	
	public static void notifySearchedUserInfo(int index)
	{
		System.out.println("notifySearchedUserInfo: index:"+index);
		UserManager.getInstance().onReceiveSearchUserInfo(ChatServiceController.getInstance().host.getUserInfoArray(index));
	}
	
	public static void notifyUserUids(String uidStr,String lastUpdateTimeStr)
	{
		System.out.println("notifyUserUids uidStr:"+uidStr+" lastUpdateTimeStr:"+lastUpdateTimeStr);
		
		if(uidStr.equals("") || lastUpdateTimeStr.equals(""))
			return;
		String[] uidArr=uidStr.split("_");
		String[] lastUpdateTimeArr=lastUpdateTimeStr.split("_");
		ArrayList<String> userUidArray=new ArrayList<String>();
//		System.out.println("uidArr length:"+uidArr.length);
		
		UserManager.getInstance().clearAllianceMember();
		for(int i=0;i<uidArr.length;i++)
		{
			if(!uidArr[i].equals(""))
			{
//				System.out.println("uidArr[i]"+uidArr[i]);
				UserInfo user=UserManager.getInstance().getUser(uidArr[i]);
				if(user!=null)
				{
					int lastUpdateTime = lastUpdateTimeArr[i].equals("") ? 1 : Integer.parseInt(lastUpdateTimeArr[i]);
					if (lastUpdateTime > user.lastUpdateTime)
					{
//						System.out.println("getMultiUserInfo() notifyUserUids user " + uidArr[i] + " is old: " + lastUpdateTime + " > " + user.lastUpdateTime);
						userUidArray.add(uidArr[i]);
					}
					UserManager.getInstance().putChatRoomMemberInMap(user);
				}
				else
				{
					user = new UserInfo(uidArr[i]);
					UserManager.getInstance().addUser(user);
					userUidArray.add(uidArr[i]);
				}
			}	
		}
		if(userUidArray.size()>0)
			UserManager.getInstance().getMultiUserInfo(userUidArray);
	}
	
	// C++主动关闭原生，发生在网络断开连接时，或创建聊天室之后
	public static void exitChatActivityFrom2dx() {
		System.out.println("exitChatActivityFrom2dx");
		if (ChatServiceController.getCurrentActivity() != null) {
			ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable() {
				@Override
				public void run() {
					try {
//						ChatServiceController.chatActivity.exitActivity();
						ChatServiceController.showGameActivity(ChatServiceController.getCurrentActivity());
					} catch (Exception e) {
						LogUtil.printException(e);
					}
				}
			});
		}
	}
	
	// 这个时机比较奇怪，可能只调了一个activity的onDestroy，就会到这里，之后才会调其它activity的onDestroy
	public static void onReturn2dxGame()
	{
	}

	public static void flyHint(String icon, String titleText, String contentText, float time, float dy, boolean useDefaultIcon) {
		System.out.println("flyHint");
		if (!ChatServiceController.isNativeShowing || ChatServiceController.getCurrentActivity() == null)
			return;

		final String text = contentText;
		final int duration = time > 0 ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT;

		ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if(ChatServiceController.getCurrentActivity().getApplicationContext() != null && text != null){
						Toast toast = Toast.makeText(ChatServiceController.getCurrentActivity().getApplicationContext(), text, duration);
						toast.setGravity(Gravity.TOP, 0, ChatServiceController.getCurrentActivity().getToastPosY());
						toast.show();
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}

	private static Timer flyHintTimer;

	public static void stopFlyHintTimer() {
		if (flyHintTimer != null){
			flyHintTimer.cancel();
			flyHintTimer.purge();
		}
	}

	private static int flyHintCount;		//flyHint显示次数
	public static void flySystemUpdateHint(double countDown, boolean isFlyHintLogin, boolean isLogin, String tip, String icon) {
		if (!ChatServiceController.isNativeShowing || ChatServiceController.getCurrentActivity() == null)
			return;
		stopFlyHintTimer();
		flyHintTimer = new Timer();
		final String text = tip;
		flyHintCount = (int) countDown / 10;
		final boolean flyHintLogin = isFlyHintLogin;

		TimerTask timerTask = new TimerTask() {
			@Override
			public void run() {
				ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable() {
					@Override
					public void run() {
						try {
							String showText = "";
							if (flyHintLogin)
								if (flyHintCount / 60 > 0)
									showText = text + "\n" + LanguageManager.getLangByKey(LanguageKeys.FLYHINT_DOWN_MIN, String.valueOf(flyHintCount / 60));
								else
									showText = text + "\n" + LanguageManager.getLangByKey(LanguageKeys.FLYHINT_DOWN_SECOND, String.valueOf(flyHintCount));
							if(ChatServiceController.getCurrentActivity().getApplicationContext() != null){
								Toast toast = Toast.makeText(ChatServiceController.getCurrentActivity().getApplicationContext(), showText, 1);
								toast.setGravity(Gravity.TOP, 0, ChatServiceController.getCurrentActivity().getToastPosY());
								toast.show();
							}
						} catch (Exception e) {
							LogUtil.printException(e);
						}
					}
				});

				flyHintCount--;
				if (flyHintCount <= 0){
					stopFlyHintTimer();
				}
			}

		};
		flyHintTimer.schedule(timerTask, 0, 10000);
	}

	public static final int TYPE_CHAT = 0;
	public static final int TYPE_FORUM = 1;
	public static final int TYPE_MEMBER_SELECTOR = 2;
	public static void showChatActivity(Activity a, int channelType, boolean rememberPosition) {
		System.out.println("showChatActivity()");
		if(a == null) return;

		ChatFragment.rememberPosition = rememberPosition;
		
		Intent intent = null;
		if(channelType >= 0){
			intent = new Intent(a, ChatActivity.class);
			intent.putExtra("channelType", channelType);
		}
		
		showActivity(a, ChatActivity.class, true, false, intent, false, false);
	}
	public static void showForumActivity(Activity a, String forumUrl) {
		System.out.println("showForumActivity()");

		Intent intent = null;
		if(StringUtils.isNotEmpty(forumUrl)){
			intent = new Intent(a, ForumActivity.class);
			intent.putExtra("forumUrl", forumUrl);
		}
		
		showActivity(a, ForumActivity.class, false, false, intent, false, false);
	}
	public static void showMemberSelectorActivity(Activity a, boolean requestResult) {
		System.out.println("showMemberSelectorActivity()");
		showActivity(a, MemberSelectorActivity.class, true, false, null, requestResult, false);
	}
	
	public static void showChannelListActivity(Activity a, boolean isSecondLvList, int channelType, String channelId, boolean isGoBack)
	{
		System.out.println("showChannelListActivity 2");

		Intent intent = new Intent(a, ChannelListActivity.class);
		intent.putExtra("isSecondLvList", isSecondLvList);
		if(channelType >= 0) intent.putExtra("channelType", channelType);
		if(StringUtils.isNotEmpty(channelId)) intent.putExtra("channelId", channelId);
		
		showActivity(a, ChannelListActivity.class, true, false, intent, false, isGoBack);
	}
	
	public static void showWriteMailActivity(Activity a, boolean clearTop, String roomName, String uidStr, String nameStr)
	{
		System.out.println("showWriteMailActivity roomName:" + roomName + " uidStr:" + uidStr + " nameStr:" + nameStr + " clearTop:" + clearTop);

		Intent intent = null;
		
		if(StringUtils.isNotEmpty(roomName) || StringUtils.isNotEmpty(uidStr) || StringUtils.isNotEmpty(nameStr)){
			intent = new Intent(a, WriteMailActivity.class);
			intent.putExtra("roomName", roomName);
			intent.putExtra("memberUids", uidStr);
			intent.putExtra("memberNames", nameStr);
		}
		
		showActivity(a, WriteMailActivity.class, true, clearTop, intent, false, clearTop);
	}
	
	private static void showActivity(Activity a, Class <?> cls, boolean newTask, boolean clearTop, Intent intent, boolean requestResult, boolean popStackAnimation) {
		ChatServiceController.isNativeStarting = true;
//		System.out.println("ChatServiceController.isNativeStarting:" + ChatServiceController.isNativeStarting);
		ChatServiceController.isNativeShowing = true;
		ChatServiceController.isReturningToGame = false;
		if(a instanceof ICocos2dxScreenLockListener){
			MyActionBarActivity.previousActivity = (ICocos2dxScreenLockListener) a;
		}
		
		lastCSClass = cls;
		
		Intent i = intent != null ? intent : new Intent(a, cls);
//		if(newTask) i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		if(clearTop) i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);

		if(!requestResult){
			a.startActivity(i);
		}else{
			a.startActivityForResult(i, 0);
		}
		
		if(!popStackAnimation){
			a.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
		}else{
			a.overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
		}
	}

	private static Class<?> lastCSClass;
	private static ArrayList<MyActionBarActivity> activityStack = new ArrayList<MyActionBarActivity>();
	
	public static void pushActivity(MyActionBarActivity a)
	{
		if(!activityStack.contains(a)){
			activityStack.add(a);
		}else{
			System.out.println("MyActionBarActivity  pushActivity already have !!! : " + activityStack.size());
		}
		System.out.println("MyActionBarActivity  pushActivity: " + activityStack.size());
	}

	public static void popActivity(MyActionBarActivity a)
	{
		if(activityStack.contains(a)){
			activityStack.remove(a);
		}
		System.out.println("MyActionBarActivity  popActivity: " + activityStack.size());
	}

	public static int getNativeActivityCount()
	{
		return activityStack.size();
	}
	
	public static void clearActivityStack()
	{
		activityStack.clear();
	}
	
//	public static void returnToChatServiceActivity(Activity a) {
//		if(lastCSClass == null) return;
//		
//		if(a instanceof ICocos2dxScreenLockListener){
//			ChannelListActivity.previousActivity = (ICocos2dxScreenLockListener) a;
//		}
//		Intent intent = new Intent(a, lastCSClass);
//		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK ); // Intent.FLAG_ACTIVITY_SINGLE_TOP
//		a.startActivity(intent);
//		a.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
//	}

	public static void postIsChatRoomMemberFlag(String groupId,boolean flag)
	{
		System.out.println("postIsChatRoomMemberFlag groupId:"+groupId);
		ChannelManager.getInstance().setIsMemberFlag(groupId,flag);
		
		if(ChatServiceController.hostActivity == null) return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getChatFragment() != null) {
						if(ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{	
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}
	
	public static void postChannelInfo(final String channelInfo)
	{
		ChannelManager.getInstance().isGetingNewMsg = false;

		long startTime = System.currentTimeMillis();
		System.out.println("handleGetNewMailMsg handleChannelInfo开始:"+ startTime);
		ChannelManager.getInstance().handleChannelInfo(channelInfo);
		if(ChatServiceController.hostActivity == null) return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getChatFragment() != null) {
						ChatServiceController.getChatFragment().refreshToolTip();
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
		long offsetTime = System.currentTimeMillis() - startTime;
		System.out.println("handleGetNewMailMsg handleChannelInfo耗时:"+ offsetTime);
		System.out.println("handleGetNewMailMsg handleChannelInfo完成:"+TimeManager.getInstance().getCurrentTime());
	}
	
	public static String getChannelInfo()
	{
		ChannelManager.getInstance().isGetingNewMsg = true;
		return ChannelManager.getInstance().getChannelInfo();
	}
	
	public static void setAutoTranlateMode(int mode)
	{
		System.out.println("setAutoTranlateEnable:"+mode);
		ConfigManager.autoTranlateMode=mode;
	}
	
	public static void setDisableLang(String disableLang)
	{
		System.out.println("setDisableLang:"+disableLang);
		TranslateManager.getInstance().disableLang=disableLang;
	}
	
	public static void setGlobalMailCount(int noticeR,int studioR,int fightR,int modR)
	{
		MailManager.getInstance().refreshGlobalMailCount(noticeR, studioR, fightR, modR);
	}
	
	public static void setMailSave(String mailId,int type,int saveFlag)
	{
		MailData mail = DBManager.getInstance().getSysMailByID(mailId);
		if(mail!=null)
		{
			System.out.println("setMailSave channelId:" + mail.getChannelId());
			if(mail.getSave()!=saveFlag)
			{
				mail.setSave(saveFlag);
				DBManager.getInstance().updateMail(mail);
			}
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
			if(channel == null || channel.mailDataList==null) return;
			channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
			DBManager.getInstance().updateChannel(channel);
			for(int i=0;i<channel.mailDataList.size();i++)
			{
				MailData mailData=channel.mailDataList.get(i);
				if(mailData!=null && mailId.equals(mailData.getUid()))
				{
					if(mailData.getSave()!=saveFlag)
					{
						mailData.setSave(saveFlag);
					}
					break;
				}
			}
		}
	}
	
	public static void setMailRewardStatus(String mailId,int type)
	{
		MailData mail = DBManager.getInstance().getSysMailByID(mailId);
		if(mail!=null)
		{
			System.out.println("setMailRewardStatus channelId:" + mail.getChannelId());
			if(mail.getRewardStatus()==0)
			{
				mail.setRewardStatus(1);
				DBManager.getInstance().updateMail(mail);
			}
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
			if(channel == null || channel.mailDataList==null) return;
			channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
			DBManager.getInstance().updateChannel(channel);
			for(int i=0;i<channel.mailDataList.size();i++)
			{
				MailData mailData=channel.mailDataList.get(i);
				if(mailData!=null && mailId.equals(mailData.getUid()))
				{
					if(mailData.getRewardStatus()==0)
					{
						mailData.setRewardStatus(1);
					}
					break;
				}
			}
		}
	}
	
	public static void postMailUpdate(String updateData)
	{
		System.out.println("postMailUpdate updateData:" + updateData);
		if(updateData.equals(""))
			return;
		ChannelManager.mailUpdateData = updateData;
		try
		{
			MailUpdateData updateDate = JSON.parseObject(updateData, MailUpdateData.class);
			ChannelManager.getInstance().updateMailData(updateDate);
		
		}
		catch (Exception e)
		{
			System.out.println("postMailUpdate parse error!"); 
		}
		
	}
	
	public static void postMailDeleteStatus(String mailUid)
	{
		System.out.println("postMailDeleteStatus mailUid:" + mailUid);
		if(mailUid.equals(""))
			return;
		ChannelManager.getInstance().deleteMailFrom2dx(mailUid);
	}
	
	public static String getLastMailUpdateTime()
	{
		String ret = "";
//		List<ChatChannel> channelList=ChannelManager.getInstance().getAllSysMailChannel();
		long latestModifyTime= ChannelManager.getInstance().getLatestSysMailModifyTime();
//		for(int i=0;i<channelList.size();i++)
//		{
//			ChatChannel channel=channelList.get(i);
//			if(channel!=null)
//			{
//				if(channel.latestModifyTime>latestModifyTime)
//				{
//					latestModifyTime=channel.latestModifyTime;
//				}
//			}
//		}
		
		if(latestModifyTime>0)
		{
			ret = Long.toString(latestModifyTime);
			System.out.println("getUpdateMail time:"+ret);
		}
		return ret;
	}
	
	public static void postMailDealStatus(String mailUid)
	{
		System.out.println("postMailDealStatus mailUid:" + mailUid);
		if(mailUid.equals(""))
			return;
		ChannelManager.getInstance().dealMailFrom2dx(mailUid);
	}
	
	public static void postTranslatedResult(String jsonRet)
	{
		System.out.println("postTranslatedResult jsonRet:" + jsonRet);
		if(StringUtils.isNotEmpty(jsonRet))
		{
			try
			{
				TranslatedByLuaResult result = JSON.parseObject(jsonRet, TranslatedByLuaResult.class);
				if(result!=null && StringUtils.isNotEmpty(result.getOriginalMsg()) && TranslateManager.getInstance().isInTranslateQueue(result.getOriginalMsg()))
				{
					TranslateManager.getInstance().handleTranslateResult(result);
				}
			}
			catch (Exception e)
			{
				System.out.println("postTranslatedResult exception");
			}
		}
	}
	
	public static void postTranslateByLuaStart()
	{
		System.out.println("postTranslateByLuaStart");
		TranslateManager.isTranslatedByLuaStart = true;
	}
	
	public static void postUIShow()
	{
		System.out.println("postUIShow");
		TranslateManager.isUIShow = true;
//		if(StringUtils.isNotEmpty(ChannelManager.mailUpdateData))
//		{
//			try
//			{
//				MailUpdateData updateDate = JSON.parseObject(ChannelManager.mailUpdateData, MailUpdateData.class);
//				ChannelManager.getInstance().updateMailData(updateDate);
//			}
//			catch (Exception e)
//			{
//				System.out.println("postMailUpdate parse error!"); 
//			}
//		}
	}
	
	public static void setMailNewUIEnable(boolean newUIEnable)
	{
		System.out.println("setMailNewUIEnable : "+newUIEnable);
		ChatServiceController.isNewMailUIEnable = newUIEnable;
	}
}
