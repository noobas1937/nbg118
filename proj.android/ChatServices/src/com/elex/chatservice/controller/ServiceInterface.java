package com.elex.chatservice.controller;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;
import android.content.Intent;
import android.provider.Settings;
import android.util.Log;
import android.view.Gravity;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.R;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.FlyMutiRewardInfo;
import com.elex.chatservice.model.FriendLatestMail;
import com.elex.chatservice.model.LanguageItem;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.LatestChatInfo;
import com.elex.chatservice.model.LatestCountryAllianceChatInfo;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.StickManager;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.ChatTable;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.updatedata.MailUpdateData;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.net.WebSocketStatusHandler;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.SortUtil;
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
import com.elex.chatservice.view.autoscroll.ScrollTextManager;

public class ServiceInterface
{
	public static String	allianceIdJoining;
	private static boolean	oldHornMsgPushed	= false;

	public static void onJoinAnnounceInvitationSuccess()
	{
		UserManager.getInstance().getCurrentUser().allianceId = allianceIdJoining;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						ChatServiceController.getChatFragment().onJoinAnnounceInvitationSuccess();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void setGameLanguage(String gameLanguage)
	{
		ConfigManager.getInstance().gameLang = gameLanguage;
	}

	public static void toggleFullScreen(final boolean enabled)
	{
		ChatServiceController.toggleFullScreen(enabled, true, ChatServiceController.hostActivity);
	}

	public static void setMailInfo(String mailFromUid, String mailUid, String mailName, int mailType)
	{
		UserManager.getInstance().getCurrentMail().opponentUid = mailFromUid;
		UserManager.getInstance().getCurrentMail().mailUid = mailUid;
		UserManager.getInstance().getCurrentMail().opponentName = mailName;
		UserManager.getInstance().getCurrentMail().type = mailType;
		if (mailType == MailManager.MAIL_MOD_PERSONAL || mailType == MailManager.MAIL_MOD_SEND)
			ChatServiceController.isContactMod = true;
	}

	public static void setContactModState()
	{
		ChatServiceController.isContactMod = true;
	}

	public static void resetPlayerFirstJoinAlliance()
	{
		ConfigManager.getInstance().isFirstJoinAlliance = false;
		ServiceInterface.resetPlayerIsInAlliance();
	}

	public static void postNoMoreMessage(int channelType)
	{
		if (ChatServiceController.getChatFragment() != null)
		{
			ChatServiceController.getChatFragment().resetMoreDataStart(channelType);
		}
	}

	public static void deleteChatRoom(String groupId)
	{
		ChannelManager.getInstance().deleteChatroomChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
	}

	public static void deleteMail(String id, int channelType, int type)
	{
		if (channelType == DBDefinition.CHANNEL_TYPE_USER || channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, id);
			if (channel == null)
				return;
			ChannelManager.getInstance().deleteChannel(channel);
		}
		else if (channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && ChatServiceController.isNewMailListEnable)
		{
			if (type == MailManager.MAIL_RESOURCE || (type == MailManager.MAIL_RESOURCE_HELP && ChatServiceController.isNewMailUIEnable)
					|| type == MailManager.MAIL_ATTACKMONSTER || id.equals("knight"))
			{
				String channelId = "";
				if (type == MailManager.MAIL_RESOURCE)
					channelId = MailManager.CHANNELID_RESOURCE;
				else if (type == MailManager.MAIL_RESOURCE_HELP)
					channelId = MailManager.CHANNELID_RESOURCE_HELP;
				else if (type == MailManager.MAIL_ATTACKMONSTER)
					channelId = MailManager.CHANNELID_MONSTER;
				else if (id.equals("knight"))
					channelId = MailManager.CHANNELID_KNIGHT;
				ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
				if (channel != null)
				{
					channel.clearAllSysMail();
				}
			}
			else
			{
				MailData mail = DBManager.getInstance().getSysMailByID(id);
				if (mail != null)
				{
					ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
					if (channel == null)
						return;
					ChannelManager.getInstance().deleteSysMailFromChannel(channel, id, false);
				}
			}

			if (ChatServiceController.hostActivity == null)
				return;
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						if (ChatServiceController.getChannelListFragment() != null)
						{
							ChatServiceController.getChannelListFragment().notifyDataSetChanged();
						}
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void setChannelMemberArray(int channelType, String fromUid, String uidStr, String roomName)
	{
		ChannelManager.getInstance().setChannelMemberArray(fromUid, uidStr, roomName);

		if (ChatServiceController.hostActivity == null || channelType != DBDefinition.CHANNEL_TYPE_CHATROOM)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						if (!ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void updateChannelMemberArray(int channelType, String fromUid, String uidStr, boolean isAdd)
	{
		ChannelManager.getInstance().updateChannelMemberArray(fromUid, uidStr, isAdd);

		if (ChatServiceController.hostActivity == null || channelType != DBDefinition.CHANNEL_TYPE_CHATROOM || !isAdd)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						if (!ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void setChatRoomFounder(String groupId, String founderUid)
	{
		ChannelManager.getInstance().setChatRoomFounder(groupId, founderUid);
	}

	public static void setChatHorn(boolean enableChatHorn)
	{
		ConfigManager.enableChatHorn = enableChatHorn;
	}

	private static void save2DB(MsgItem[] infoArr, final int channelType, final String channelId, String customName)
	{
		LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_MSG, "channelType", channelType, "channelId", channelId, "size",
				infoArr.length);

		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
		if (infoArr.length == 0 || channel == null)
			return;

		if (channelType == DBDefinition.CHANNEL_TYPE_USER || channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
			channel.customName = customName;

		DBManager.getInstance().insertMessages(infoArr, channel.getChatTable());
	}

	private static void updateDBMsg(MsgItem msg, final int channelType, final String fromId)
	{
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, fromId);
		if (channel == null)
			return;

		DBManager.getInstance().updateMyMessageStatus(msg, channel.getChatTable());
	}

	/**
	 * 获取多条历史消息时，此函数会被多次调用（具体次数由MailCell、MailController决定），多次刷新界面（游戏中看不出来）
	 * 
	 * @param channelId
	 *            如果是邮件则总是指对方的uid，如果是聊天室为uid，如果是聊天fromUid为"0"
	 * @param customName
	 *            如果是邮件则总是指对方的name，如果是聊天室为自定义名称，如果是聊天为""
	 */
	public static void notifyMessageIndex(int chatInfoIndex, String channelId, String customName, boolean isModMsg)
	{
		if (chatInfoIndex < 0 || StringUtils.isEmpty(channelId))
			return;
		final Object[] chatInfoArr = ChatServiceController.getInstance().host.getChatInfoArray(chatInfoIndex, channelId);

		if (isModMsg && !channelId.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
			channelId += DBDefinition.CHANNEL_ID_POSTFIX_MOD;
		if (chatInfoArr == null || chatInfoArr.length <= 0)
			return;

		MsgItem[] _itemArray = new MsgItem[chatInfoArr.length];
		for (int i = 0; i < chatInfoArr.length; i++)
		{
			Object obj = chatInfoArr[i];
			if (obj != null)
			{
				_itemArray[i] = (MsgItem) obj;
			}
		}

		if (WebSocketManager.isRecieveFromWebSocket(_itemArray[0].channelType))
			return;

		handleMessage(_itemArray, channelId, customName, true, true);
	}

	public static synchronized MailData parseMailData(MailData mailData, boolean isFromDB)
	{
		MailData mail = null;
		try
		{
			mail = MailManager.getInstance().parseMailDataContent(mailData);
			boolean needUpdateDB = false;
			if (isFromDB)
			{
				boolean needParsedBefore = mailData.needParseContent();
				boolean needParsedNow = mail.needParseContent();
				needUpdateDB = needParsedBefore && !needParsedNow;
			}
			return handleMailData(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail, isFromDB, needUpdateDB);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		return null;
	}

	private static ArrayList<Integer>	mailDataIndexArray	= new ArrayList<Integer>();

	public static void notifyMailDataIndex(final int mailDataIndex, boolean isGetNew)
	{
		LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_MSG, "mailDataIndex", mailDataIndex, "isGetNew", isGetNew);
		if (isGetNew)
		{
			mailDataIndexArray.add(Integer.valueOf(mailDataIndex));
		}
		else
		{
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

	public static boolean	isHandlingGetNewMailMsg	= false;

	public static void handleGetNewMailMsg(final String channelInfo)
	{
		Runnable run = new Runnable()
		{
			@Override
			public void run()
			{
				isHandlingGetNewMailMsg = true;

				for (int i = 0; i < mailDataIndexArray.size(); i++)
				{
					handleMailDataIndex(mailDataIndexArray.get(i).intValue(), true);
				}
				mailDataIndexArray.clear();

				postChannelInfo(channelInfo);

				isHandlingGetNewMailMsg = false;

				ChatServiceController.hostActivity.runOnUiThread(new Runnable()
				{
					@Override
					public void run()
					{
						try
						{
							if (ChatServiceController.getChannelListFragment() != null)
							{
								ChatServiceController.getChannelListFragment().refreshTitleLabel();
							}
						}
						catch (Exception e)
						{
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
		if (mailDataIndex < 0)
			return;

		final Object[] mailDataArr = ChatServiceController.getInstance().host.getMailDataArray(mailDataIndex);
		if (mailDataArr == null)
			return;

		boolean hasDetectMail = false;
		String channelId = "";

		for (int i = 0; i < mailDataArr.length; i++)
		{
			MailData mailData = (MailData) mailDataArr[i];
			if (mailData == null)
				continue;
			if (!hasDetectMail && mailData.getType() == MailManager.MAIL_DETECT_REPORT)
				hasDetectMail = true;
			if (!isGetNew)
			{
				final MailData mail = parseMailData(mailData, false);
				if (mail != null)
				{

					ChatServiceController.hostActivity.runOnUiThread(new Runnable()
					{
						@Override
						public void run()
						{
							try
							{
								ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL,
										mail.getChannelId());
								if (channel != null)
								{
									channel.addNewMailData(mail);
								}
								ChannelManager.getInstance().calulateAllChannelUnreadNum();
							}
							catch (Exception e)
							{
								LogUtil.printException(e);
							}
						}
					});
				}
				if (StringUtils.isEmpty(channelId))
					channelId = mail.getChannelId();
			}
			else
			{
				mailData.parseMailTypeTab();
				ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mailData.getChannelId());
				if (channel != null)
				{
					DBManager.getInstance().insertMailData(mailData, channel);
					channel.refreshRenderData();
					channel.updateSysMailCountFromDB(1);
				}
			}
		}

		if (StringUtils.isNotEmpty(ChannelManager.currentOpenedChannel) && ChannelManager.currentOpenedChannel.equals(channelId))
			ChannelManager.getInstance().postNotifyPopup(channelId);

		if (hasDetectMail)
			DBManager.getInstance().getDetectMailInfo();
		ChannelListFragment.onMailAdded();
	}

	public static MailData handleMailData(int channelType, final MailData mailData, boolean isFromDB, boolean needUpdateDB)
	{
		if (mailData == null)
			return null;

		boolean isWorldBossKillRewardMail = false;
		if (mailData.isWorldBossKillRewardMail())
		{
			isWorldBossKillRewardMail = true;
			mailData.setType(MailManager.MAIL_WORLD_BOSS);
		}

		final ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, mailData.getChannelId());
		if (channel == null)
			return mailData;
		if (!isFromDB)
		{
			DBManager.getInstance().insertMailData(mailData, channel);
			channel.updateSysMailCountFromDB(1);
		}
		else
		{
			if (needUpdateDB || isWorldBossKillRewardMail)
			{
				mailData.channelId = mailData.getChannelId();
				DBManager.getInstance().updateMail(mailData);
			}
		}

		return mailData;
	}

	public static void handleMessage(final MsgItem[] chatInfoArr, final String channelId, final String customName,
			final boolean calulateUnread, final boolean isFromServer)
	{
		for (int i = 0; i < chatInfoArr.length; i++)
		{
			if (isFromServer && chatInfoArr[i].hasTranslation())
				chatInfoArr[i].translatedLang = ConfigManager.getInstance().gameLang;
			if (!chatInfoArr[i].isRedPackageMessage())
				chatInfoArr[i].sendState = MsgItem.SEND_SUCCESS;
			// 存储用户信息
			chatInfoArr[i].initUserForReceivedMsg(channelId, customName);
			if (TranslateManager.getInstance().hasTranslated(chatInfoArr[i]))
				chatInfoArr[i].hasTranslated = true;
			else
				chatInfoArr[i].hasTranslated = false;
		}

		final int channelType = chatInfoArr[0].channelType;
		final boolean isNewMessage = chatInfoArr[0].isNewMsg;

		LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_MSG, "channelType", channelType, "channelId", channelId,
				"calulateUnread", calulateUnread, "isFromServer", isFromServer);
		if (isFromServer)
		{
			save2DB(chatInfoArr, channelType, channelId, customName);
		}

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
					handleMessage2(channelType, isNewMessage, chatInfoArr, channelId, customName, isFromServer);

					// TODO 如果消息属于当前的聊天窗口，则无需刷新unread
					// && ChatServiceController.getChatFragment() == null
					if (calulateUnread)
						ChannelManager.getInstance().calulateAllChannelUnreadNum();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});

		if (ConfigManager.autoTranlateMode > 0)
		{
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				TranslateManager.getInstance().loadTranslation(chatInfoArr[i], null);
			}
		}

	}

	private static void handleMessage2(final int channelType, final boolean isNewMessage, final MsgItem[] chatInfoArr,
			final String channelId, String customName, boolean isFromServer)
	{
		LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_MSG, "isNewMessage", isNewMessage, "isFromServer", isFromServer);
		ChatChannel channel = ChannelManager.getInstance().getChannel(channelType, channelId);
		if (channel == null)
			return;

		if (channelType == DBDefinition.CHANNEL_TYPE_USER || channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			channel.customName = customName;
		}

		if (chatInfoArr.length <= 0)
			return;

		ArrayList<MsgItem> msgList = channel.msgList;
		ArrayList<MsgItem> sendingMsgList = channel.sendingMsgList;
		ChannelManager.getInstance().setHasRequestDataBeforeFlag(channelType, channelId, true);

		if (isNewMessage)
		{
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				MsgItem sendingMsg = null;
				MsgItem recievedMsg = chatInfoArr[i];
				LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_MSG, "recievedMsg.msg", recievedMsg.msg,
						"recievedMsg sendLocalTime", recievedMsg.sendLocalTime, "recievedMsg.isSelfMsg()", recievedMsg.isSelfMsg(),
						"recievedMsg.isSystemMessage()", recievedMsg.isSystemMessage(), "recievedMsg.isHornMessage()",
						recievedMsg.isHornMessage());
				if (msgList != null && msgList.size() > 0)
				{
					for (int j = 0; j < sendingMsgList.size(); j++)
					{
						MsgItem sendMsg = sendingMsgList.get(j);
						if (sendMsg == null)
							continue;
						LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "sendMsg.msg", sendMsg.msg,
								"sendMsg.sendLocalTime", sendMsg.sendLocalTime);
						if (sendMsg.sendLocalTime != 0 && sendMsg.sendLocalTime == recievedMsg.sendLocalTime)
						{
							sendingMsg = sendMsg;
						}
					}
				}

				// 我发的消息
				if (sendingMsg != null && recievedMsg.isSelfMsg() && (!recievedMsg.isSystemMessage() || recievedMsg.isHornMessage()))
				{
					sendingMsg.sendState = MsgItem.SEND_SUCCESS;
					sendingMsgList.remove(sendingMsg);
					channel.replaceDummyMsg(recievedMsg, msgList.indexOf(sendingMsg));
				}
				else
				{
					channel.addNewMsg(recievedMsg);
				}

				if (ChatServiceController.getChatFragment() != null)
				{
					ChatServiceController.getChatFragment().notifyDataSetChanged(channelType, true);
					ChatServiceController.getChatFragment().updateListPositionForNewMsg(channelType, recievedMsg.isSelfMsg);

					final MsgItem msgItem = recievedMsg;
					ChatServiceController.hostActivity.runOnUiThread(new Runnable()
					{
						@Override
						public void run()
						{
							try
							{
								if (msgItem.isHornMessage() && ChatServiceController.getChatFragment() != null)
									ChatServiceController.getChatFragment().showHornScrollText(msgItem);
							}
							catch (Exception e)
							{
								LogUtil.printException(e);
							}
						}
					});

				}
				else
				{
					if (recievedMsg.isHornMessage())
					{
						ScrollTextManager.getInstance().clear();
						ScrollTextManager.getInstance().push(recievedMsg);
					}
				}
			}
		}
		else
		{
			int loadCount = 0;
			MsgItem oldFirstItem = null;
			if (channel.msgList != null && channel.msgList.size() > 0)
				oldFirstItem = channel.msgList.get(0);
			for (int i = 0; i < chatInfoArr.length; i++)
			{
				boolean isAddSuccess = channel.addHistoryMsg(chatInfoArr[i]);
				if (isAddSuccess)
					loadCount++;
			}

			if (loadCount > 0)
				channel.getLoadedTimeNeedShowMsgIndex(loadCount);

			if (channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
			{
				if (!oldHornMsgPushed && channel.msgList != null && channel.msgList.size() > 0)
				{
					for (int i = 0; i < channel.msgList.size(); i++)
					{
						MsgItem msgItem = channel.msgList.get(i);
						if (msgItem != null && msgItem.isHornMessage())
						{
							ScrollTextManager.getInstance().clear();
							ScrollTextManager.getInstance().push(msgItem);
							oldHornMsgPushed = true;
						}
					}
				}
			}

			if (ChatServiceController.getChatFragment() != null)
			{
				ChatServiceController.getChatFragment().notifyDataSetChanged(channelType, false);
				ChatServiceController.getChatFragment().updateListPositionForOldMsg(channelType, loadCount,
						!ChatServiceController.getInstance().isDifferentDate(oldFirstItem, channel.msgList));
				ChatServiceController.getChatFragment().resetMoreDataStart(channelType);
			}
		}

		if (isFromServer)
		{
			// 会触发reload，仅在服务器端来了新消息才调用
			ChannelListFragment.onMsgAdded(channel);
		}

		if (WebSocketManager.isRecieveFromWebSocket(channelType) && (channel.isCountryChannel() || channel.isAllianceChannel()))
		{
			sendChatLatestMessage(channel);
		}
	}

	public static void notifyChatRoomNameChanged(final String modifyName)
	{
		UserManager.getInstance().getCurrentMail().opponentName = modifyName;
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

	public static void postChannelNoMoreData(int channelType, boolean hasNoMoreData)
	{
		if (StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId()))
			return;

		ChannelManager.getInstance().setNoMoreDataFlag(ChannelManager.channelType2tab(channelType), hasNoMoreData);
	}

	public static void removeAllMailByUid(String fromUid)
	{
		if (StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId()))
			return;

		ChannelManager.getInstance().removeAllMailMsgByUid(fromUid);
	}

	public static void setIsNewMailListEnable(boolean enable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "isNewMailListEnable", enable);
		ChatServiceController.isNewMailListEnable = enable;
	}

	public static void setCurrentUserId(String uidStr)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "uid", uidStr);
		UserManager.getInstance().setCurrentUserId(uidStr);
	}

	private static UserInfo	currentUserClone;

	/**
	 * 初始登录、重新登录、切服时会调用
	 * 
	 * @param worldTime
	 *            utc时间，单位为s
	 */
	public static void setPlayerInfo(int country, int worldTime, int gmod, int headPicVer, String name, String uidStr, String picStr,
			int vipLevel, int vipEndTime, int lastUpdateTime, int crossFightSrcServerId)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "uid", uidStr, "name", name, "country", country,
				"crossFightSrcServerId", crossFightSrcServerId);

		TimeManager.getInstance().setServerBaseTime(worldTime);
		currentUserClone = (UserInfo) UserManager.getInstance().getCurrentUser().clone();
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
	}

	/**
	 * 初始登录时会调用 打开聊天时，会紧接着setPlayerInfo后面调
	 * 重新登录、切服等时候，会调C++的parseData()刷新联盟信息，也调用此函数
	 */
	public static void setPlayerAllianceInfo(String asnStr, String allianceIdStr, int alliancerank, boolean isFirstJoinAlliance)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "allianceIdStr", allianceIdStr, "current allianceId", UserManager
				.getInstance().getCurrentUser().allianceId);

		// 变更联盟（退出联盟）
		if (UserManager.getInstance().isCurrentUserInAlliance()
				&& !UserManager.getInstance().getCurrentUser().allianceId.equals(allianceIdStr) && ChannelManager.isInited()
				&& ChannelManager.getInstance().getAllianceChannel().msgList != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ChannelManager.getInstance().getAllianceChannel().msgList.clear();
						if (ChatServiceController.getChatFragment() != null)
						{
							ChatServiceController.getChatFragment().notifyDataSetChanged(DBDefinition.CHANNEL_TYPE_ALLIANCE,true);
						}
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
			if (ConfigManager.useWebSocketServer)
			{
				// 可能在登陆时调用，此时ws未初始化，调用无效
				WebSocketManager.getInstance().leaveAllianceRoom();
			}
		}

		// 加入联盟
		if (ConfigManager.useWebSocketServer && !UserManager.getInstance().isCurrentUserInAlliance()
				&& StringUtils.isNotEmpty(allianceIdStr))
		{
			// 可能在登陆时调用，此时ws未初始化，调用无效
			WebSocketManager.getInstance().joinRoom();
		}

		if (currentUserClone == null)
			currentUserClone = (UserInfo) UserManager.getInstance().getCurrentUser().clone();

		UserManager.getInstance().getCurrentUser().asn = asnStr;
		UserManager.getInstance().getCurrentUser().allianceId = allianceIdStr;
		UserManager.getInstance().getCurrentUser().allianceRank = alliancerank;
		ConfigManager.getInstance().isFirstJoinAlliance = isFirstJoinAlliance;

		if (!currentUserClone.equalsLogically(UserManager.getInstance().getCurrentUser()))
		{
			LogUtil.printVariables(
					Log.INFO,
					LogUtil.TAG_CORE,
					"current user updated:\n"
							+ LogUtil.compareObjects(new Object[] { UserManager.getInstance().getCurrentUser(), currentUserClone }));
			UserManager.getInstance().updateCurrentUser();
		}
		ChannelManager.getInstance().getAllianceChannel();
	}

	// TODO 与setPlayerAllianceInfo总是成对调用，没必要调
	// 但切服之后可能需要调
	public static void resetPlayerIsInAlliance()
	{
		if (StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId()))
			return;

		LogUtil.printVariables(Log.INFO, LogUtil.TAG_CORE, "actual resetPlayerIsInAlliance()");

		UserManager.getInstance().clearAllianceMember();

		if (UserManager.getInstance().getCurrentUser().allianceId.equals(""))
			return;

		if (ChannelManager.isInited())
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						if (UserManager.getInstance().isCurrentUserInAlliance()
								&& ChannelManager.getInstance().getAllianceChannel().msgList != null)
						{
							// 有时候会发生nullPointer异常
							ChannelManager.getInstance().getAllianceChannel().msgList.clear();
							if (ChatServiceController.getChatFragment() != null)
							{
								ChatServiceController.getChatFragment().notifyDataSetChanged(DBDefinition.CHANNEL_TYPE_ALLIANCE,true);
							}
						}
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});

			ChannelManager.getInstance().setNoMoreDataFlag(1, false);
		}

		UserManager.getInstance().getCurrentUser().allianceId = "";
		UserManager.getInstance().getCurrentUser().allianceRank = -1;

	}

	/**
	 * 锁屏时调用 以前是切换tab时才会获取数据，用chat.get接口，如果发现已经有数据，就不会再获取，所以得先clear一次
	 */
	public static void clearCountryMsg()
	{
	}

	// 以前就未调用
	public static void clearMailMsg()
	{
	}

	/**
	 * 论坛重新登录
	 */
	public static void onPlayerChanged()
	{
		ForumFragment.isFirstLogin = true;
	}

	public static void notifyChangeLanguage()
	{
		Object[] langItemArray = ChatServiceController.getInstance().host.getChatLangArray();
		if (langItemArray == null)
			return;

		LanguageItem[] langArray = new LanguageItem[langItemArray.length];
		for (int i = 0; i < langItemArray.length; i++)
		{
			Object obj = langItemArray[i];
			if (obj != null)
			{
				langArray[i] = (LanguageItem) obj;
			}
		}
		LanguageManager.initChatLanguage(langArray);
	}

	public static void onCreateChatroomSuccess()
	{
		ChatServiceController.isCreateChatRoom = false;
		ServiceInterface.showChatActivity(ChatServiceController.getCurrentActivity(), DBDefinition.CHANNEL_TYPE_CHATROOM, false);
	}

	public static boolean isDontKeepActivitiesEnabled()
	{
		int finishActivitiesEnabled = Settings.System.getInt(ChatServiceController.hostActivity.getContentResolver(),
				Settings.Global.ALWAYS_FINISH_ACTIVITIES, 0);

		return finishActivitiesEnabled == 1;
	}

	public static void gotoDevelopmentSetting()
	{
		ChatServiceController.hostActivity.startActivityForResult(new Intent(
				android.provider.Settings.ACTION_APPLICATION_DEVELOPMENT_SETTINGS), 0);
	}

	public static void showChatActivityFrom2dx(int maxHornInputCount, final int chatType, int sendInterval, final boolean rememberPosition,
			boolean enableCustomHeadImg, boolean isNoticeItemUsed)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, "chatType", chatType, "sendInterval", sendInterval,
				"rememberPosition", rememberPosition, "enableCustomHeadImg", enableCustomHeadImg, "isNoticeItemUsed", isNoticeItemUsed);

		ConfigManager.maxHornInputLength = maxHornInputCount;
		ConfigManager.enableCustomHeadImg = enableCustomHeadImg;
		ChatServiceController.isHornItemUsed = isNoticeItemUsed;
		ConfigManager.sendInterval = sendInterval * 1000;
		ChatServiceController.isCreateChatRoom = false;
		if (ChatServiceController.hostActivity != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ServiceInterface.showChatActivity(ChatServiceController.hostActivity, chatType, rememberPosition);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void showForumFrom2dx(String url)
	{
		final String forumUrl = url;
		if (ChatServiceController.hostActivity != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ServiceInterface.showForumActivity(ForumFragment.WEBVIEW_TYPE_FORFUM, ChatServiceController.hostActivity, forumUrl);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void showTranslationOptimizationFrom2dx(String url)
	{
		final String forumUrl = url;
		if (ChatServiceController.hostActivity != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ServiceInterface.showForumActivity(ForumFragment.WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION,
								ChatServiceController.hostActivity, forumUrl);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void showMemberSelectorFrom2dx()
	{
		if (ChatServiceController.hostActivity != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ChatServiceController.isCreateChatRoom = true;
						ServiceInterface.showMemberSelectorActivity(ChatServiceController.hostActivity, false);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void showChannelListFrom2dx(final boolean isGoBack)
	{
		if (ChatServiceController.hostActivity != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ServiceInterface.showChannelListActivity(ChatServiceController.hostActivity, false,
								DBDefinition.CHANNEL_TYPE_OFFICIAL, null, isGoBack);
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	public static void notifyUserInfo(int index)
	{
		UserManager.getInstance().onReceiveUserInfo(ChatServiceController.getInstance().host.getUserInfoArray(index));
	}

	public static void notifySearchedUserInfo(int index)
	{
		UserManager.getInstance().onReceiveSearchUserInfo(ChatServiceController.getInstance().host.getUserInfoArray(index));
	}

	public static void notifyUserUids(String uidStr, String lastUpdateTimeStr, int type)
	{
		if (uidStr.equals("") || lastUpdateTimeStr.equals(""))
			return;
		String[] uidArr = uidStr.split("_");
		String[] lastUpdateTimeArr = lastUpdateTimeStr.split("_");

		if (type == UserManager.NOTIFY_USERINFO_TYPE_ALLIANCE)
			UserManager.getInstance().clearAllianceMember();
		else if (type == UserManager.NOTIFY_USERINFO_TYPE_FRIEND)
			UserManager.getInstance().clearFriendMember();
		for (int i = 0; i < uidArr.length; i++)
		{
			if (!uidArr[i].equals(""))
			{
				UserInfo user = UserManager.getInstance().getUser(uidArr[i]);

				if (user != null)
				{
					if (type == UserManager.NOTIFY_USERINFO_TYPE_ALLIANCE)
						UserManager.getInstance().putChatRoomMemberInMap(user);
					else if (type == UserManager.NOTIFY_USERINFO_TYPE_FRIEND)
						UserManager.getInstance().putFriendMemberInMap(user);
				}

				// lastUpdateTimeArr[i]至少为0（C++中将空字符串设为"0"），redis中有的老用户还没有被更新过时，可能会有这种情况
				// 保险起见，这里再检查一下空字符串
				int lastUpdateTime = lastUpdateTimeArr[i].equals("") ? 0 : Integer.parseInt(lastUpdateTimeArr[i]);
				UserManager.checkUser(uidArr[i], "", lastUpdateTime);
			}
		}
	}

	/**
	 * C++主动关闭原生，发生在网络断开连接时，或创建聊天室之后
	 */
	public static void exitChatActivityFrom2dx(boolean needRemeberActivityStack)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, "needRemeberActivityStack", needRemeberActivityStack);

		if (!needRemeberActivityStack)
			ChannelListFragment.preventSecondChannelId = true;
		if (ChatServiceController.getCurrentActivity() != null)
		{
			ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						ChatServiceController.showGameActivity(ChatServiceController.getCurrentActivity());
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}
	}

	/**
	 * 这个时机比较奇怪，可能只调了一个activity的onDestroy，就会到这里，之后才会调其它activity的onDestroy
	 */
	public static void onReturn2dxGame()
	{
	}

	public static void flyHint(String icon, String titleText, String contentText, float time, float dy, boolean useDefaultIcon)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, "contentText", contentText, "titleText", titleText);

		if (!ChatServiceController.isNativeShowing || ChatServiceController.getCurrentActivity() == null)
			return;

		final String text = contentText;
		final int duration = time > 0 ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT;

		ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getCurrentActivity().getApplicationContext() != null && text != null)
					{
						Toast toast = Toast.makeText(ChatServiceController.getCurrentActivity().getApplicationContext(), text, duration);
						toast.setGravity(Gravity.TOP, 0, ChatServiceController.getCurrentActivity().getToastPosY());
						toast.show();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	private static Timer	flyHintTimer;

	public static void stopFlyHintTimer()
	{
		if (flyHintTimer != null)
		{
			flyHintTimer.cancel();
			flyHintTimer.purge();
		}
	}

	private static int	flyHintCount;

	public static void flySystemUpdateHint(double countDown, boolean isFlyHintLogin, boolean isLogin, String tip, String icon)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, "tip", tip, "isFlyHintLogin", isFlyHintLogin, "countDown",
				countDown);

		if (!ChatServiceController.isNativeShowing || ChatServiceController.getCurrentActivity() == null)
			return;
		stopFlyHintTimer();
		flyHintTimer = new Timer();
		final String text = tip;
		flyHintCount = (int) countDown / 10;
		final boolean flyHintLogin = isFlyHintLogin;

		TimerTask timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable()
				{
					@Override
					public void run()
					{
						try
						{
							String showText = "";
							if (flyHintLogin)
								if (flyHintCount / 60 > 0)
									showText = text
											+ "\n"
											+ LanguageManager.getLangByKey(LanguageKeys.FLYHINT_DOWN_MIN, String.valueOf(flyHintCount / 60));
								else
									showText = text + "\n"
											+ LanguageManager.getLangByKey(LanguageKeys.FLYHINT_DOWN_SECOND, String.valueOf(flyHintCount));
							if (ChatServiceController.getCurrentActivity().getApplicationContext() != null)
							{
								Toast toast = Toast.makeText(ChatServiceController.getCurrentActivity().getApplicationContext(), showText,
										1);
								toast.setGravity(Gravity.TOP, 0, ChatServiceController.getCurrentActivity().getToastPosY());
								toast.show();
							}
						}
						catch (Exception e)
						{
							LogUtil.printException(e);
						}
					}
				});

				flyHintCount--;
				if (flyHintCount <= 0)
				{
					stopFlyHintTimer();
				}
			}

		};
		flyHintTimer.schedule(timerTask, 0, 10000);
	}

	public static final int	TYPE_CHAT				= 0;
	public static final int	TYPE_FORUM				= 1;
	public static final int	TYPE_MEMBER_SELECTOR	= 2;

	public static void showChatActivity(Activity a, int channelType, boolean rememberPosition)
	{
		if (a == null)
			return;

		ChatFragment.rememberPosition = rememberPosition;

		Intent intent = null;
		try
		{
			if (channelType >= 0)
			{
				// 可能出异常
				intent = new Intent(a, ChatActivity.class);
				intent.putExtra("channelType", channelType);
			}

			showActivity(a, ChatActivity.class, true, false, intent, false, false);

		}
		catch (Exception e)
		{
			LogUtil.printException(e);
			return;
		}
	}

	public static void showForumActivity(int webViewType, Activity a, String forumUrl)
	{
		Intent intent = new Intent(a, ForumActivity.class);
		if (StringUtils.isNotEmpty(forumUrl))
		{
			intent.putExtra("forumUrl", forumUrl);
		}
		intent.putExtra("webViewType", webViewType);
		showActivity(a, ForumActivity.class, false, false, intent, false, false);
	}

	public static void showMemberSelectorActivity(Activity a, boolean requestResult)
	{
		showActivity(a, MemberSelectorActivity.class, true, false, null, requestResult, false);
	}

	public static void showChannelListActivity(Activity a, boolean isSecondLvList, int channelType, String channelId, boolean isGoBack)
	{
		Intent intent = new Intent(a, ChannelListActivity.class);
		intent.putExtra("isSecondLvList", isSecondLvList);
		if (channelType >= 0)
			intent.putExtra("channelType", channelType);
		if (StringUtils.isNotEmpty(channelId))
			intent.putExtra("channelId", channelId);

		showActivity(a, ChannelListActivity.class, true, false, intent, false, isGoBack);
	}

	public static void showWriteMailActivity(Activity a, boolean clearTop, String roomName, String uidStr, String nameStr)
	{
		Intent intent = null;

		if (StringUtils.isNotEmpty(roomName) || StringUtils.isNotEmpty(uidStr) || StringUtils.isNotEmpty(nameStr))
		{
			intent = new Intent(a, WriteMailActivity.class);
			intent.putExtra("roomName", roomName);
			intent.putExtra("memberUids", uidStr);
			intent.putExtra("memberNames", nameStr);
		}

		showActivity(a, WriteMailActivity.class, true, clearTop, intent, false, clearTop);
	}

	private static void showActivity(Activity a, Class<?> cls, boolean newTask, boolean clearTop, Intent intent, boolean requestResult,
			boolean popStackAnimation)
	{
		ArrayList<Object> args = new ArrayList<Object>();
		args.add("class");
		args.add(cls.getSimpleName());
		if (intent != null)
		{
			for (Iterator<String> iterator = intent.getExtras().keySet().iterator(); iterator.hasNext();)
			{
				String key = (String) iterator.next();
				args.add(key);
				args.add(intent.getExtras().get(key));
			}
		}
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, (Object[]) args.toArray(new Object[0]));

		ChatServiceController.isNativeStarting = true;
		ChatServiceController.isNativeShowing = true;
		ChatServiceController.isReturningToGame = false;
		ChannelListFragment.preventSecondChannelId = false;
		if (a instanceof ICocos2dxScreenLockListener)
		{
			MyActionBarActivity.previousActivity = (ICocos2dxScreenLockListener) a;
		}

		Intent i = intent != null ? intent : new Intent(a, cls);
		if (clearTop)
			i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);

		if (!requestResult)
		{
			a.startActivity(i);
		}
		else
		{
			a.startActivityForResult(i, 0);
		}

		if (!popStackAnimation)
		{
			a.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);
		}
		else
		{
			a.overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
		}
	}

	private static ArrayList<MyActionBarActivity>	activityStack	= new ArrayList<MyActionBarActivity>();

	public static void pushActivity(MyActionBarActivity a)
	{
		if (!activityStack.contains(a))
		{
			activityStack.add(a);
		}
		else
		{
			LogUtil.printVariablesWithFuctionName(Log.WARN, LogUtil.TAG_VIEW, "pushActivity already have", activityStack.size());
		}
		LogUtil.printVariables(Log.INFO, LogUtil.TAG_VIEW, "activityStack.size()", activityStack.size());
	}

	public static void popActivity(MyActionBarActivity a)
	{
		if (activityStack.contains(a))
		{
			activityStack.remove(a);
		}
		LogUtil.printVariables(Log.INFO, LogUtil.TAG_VIEW, "activityStack.size()", activityStack.size());
	}

	public static int getNativeActivityCount()
	{
		return activityStack.size();
	}

	public static void clearActivityStack()
	{
		activityStack.clear();
	}

	public static void postIsChatRoomMemberFlag(String groupId, boolean flag)
	{
		ChannelManager.getInstance().setIsMemberFlag(groupId, flag);

		if (ChatServiceController.hostActivity == null)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						if (ChatServiceController.getChatFragment().isSelectMemberBtnEnable())
						{
							ChatServiceController.getChatFragment().refreshMemberSelectBtn();
							ChatServiceController.getChatFragment().setSelectMemberBtnState();
						}
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void postChannelInfo(final String channelInfo)
	{
		ChannelManager.getInstance().isGetingNewMsg = false;

		long startTime = System.currentTimeMillis();
		ChannelManager.getInstance().handleChannelInfo(channelInfo);

		// channelInfo可能为空，handleChannelInfo可能不执行，但此时依然要执行ws的连接
		if (WebSocketManager.isWebSocketEnabled())
		{
			if (!ChatServiceController.getInstance().isUsingDummyHost())
			{
				WebSocketManager.getInstance().setStatusListener(WebSocketStatusHandler.getInstance());
			}
			WebSocketManager.getInstance().connect();
		}

		if (ChatServiceController.hostActivity == null)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						ChatServiceController.getChatFragment().refreshToolTip();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
		long offsetTime = System.currentTimeMillis() - startTime;
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "handleChannelInfo耗时", offsetTime);
	}

	public static String getChannelInfo()
	{
		ChannelManager.getInstance().isGetingNewMsg = true;
		String result = "";
		try
		{
			result = ChannelManager.getInstance().getChannelInfo();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		return result;
	}

	public static void setAutoTranlateMode(int mode)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "mode", mode);
		ConfigManager.autoTranlateMode = mode;
	}

	public static void setDisableLang(String disableLang)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "disableLang", disableLang);
		TranslateManager.getInstance().disableLang = disableLang;
	}

	public static void setMailSave(String mailId, int saveFlag)
	{
		MailData mail = DBManager.getInstance().getSysMailByID(mailId);
		if (mail != null)
		{
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "mailId", mailId, "saveFlag", saveFlag);
			if (mail.getSave() != saveFlag)
			{
				mail.setSave(saveFlag);
				DBManager.getInstance().updateMail(mail);
			}
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
			if (channel == null || channel.mailDataList == null)
				return;
			channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
			DBManager.getInstance().updateChannel(channel);
			for (int i = 0; i < channel.mailDataList.size(); i++)
			{
				MailData mailData = channel.mailDataList.get(i);
				if (mailData != null && mailId.equals(mailData.getUid()))
				{
					if (mailData.getSave() != saveFlag)
					{
						mailData.setSave(saveFlag);
					}
					break;
				}
			}
		}
	}

	public static void setMailRewardStatus(String mailId)
	{
		MailData mail = DBManager.getInstance().getSysMailByID(mailId);
		if (mail != null)
		{
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "mailId", mailId, "channelId", mail.getChannelId());
			if (mail.getRewardStatus() == 0)
			{
				mail.setRewardStatus(1);
				DBManager.getInstance().updateMail(mail);
			}
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
			if (channel == null || channel.mailDataList == null)
				return;
			channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
			DBManager.getInstance().updateChannel(channel);
			for (int i = 0; i < channel.mailDataList.size(); i++)
			{
				MailData mailData = channel.mailDataList.get(i);
				if (mailData != null && mailId.equals(mailData.getUid()))
				{
					if (mailData.getRewardStatus() == 0)
					{
						mailData.setRewardStatus(1);
					}
					break;
				}
			}
		}
	}

	public static void setMutiMailRewardStatus(String mailUids)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "mailUids", mailUids);
		if (StringUtils.isEmpty(mailUids))
			return;
		String[] mailUidArr = mailUids.split(",");
		for (int i = 0; i < mailUidArr.length; i++)
		{
			if (StringUtils.isNotEmpty(mailUidArr[i]))
			{
				MailData mail = DBManager.getInstance().getSysMailByID(mailUidArr[i]);
				if (mail != null)
				{
					if (mail.getRewardStatus() == 0)
					{
						if (mail.getStatus() == 0)
							mail.setStatus(1);
						mail.setRewardStatus(1);
						DBManager.getInstance().updateMail(mail);
					}
					ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
					if (channel == null || channel.mailDataList == null)
						return;
					channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();

					for (int j = 0; j < channel.mailDataList.size(); j++)
					{
						MailData mailData = channel.mailDataList.get(j);
						if (mailData != null && mailUidArr[i].equals(mailData.getUid()))
						{
							if (mailData.getRewardStatus() == 0)
							{
								if (mailData.getStatus() == 0)
								{
									channel.unreadCount--;
									mailData.setStatus(1);
								}
								mailData.setRewardStatus(1);
							}
							break;
						}
					}
					DBManager.getInstance().updateChannel(channel);
				}
			}
		}

		ChannelManager.getInstance().calulateAllChannelUnreadNum();
		if (ChatServiceController.hostActivity == null)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChannelListFragment() != null)
					{
						ChatServiceController.getChannelListFragment().notifyDataSetChanged();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	private static boolean isKnightMail(String[] mailUidArr)
	{
		if (mailUidArr.length > 0)
		{
			MailData mail = DBManager.getInstance().getSysMailByID(mailUidArr[0]);
			if (mail != null)
			{
				mail.setNeedParseByForce(true);
				MailData mailData = MailManager.getInstance().parseMailDataContent(mail);
				if (mailData != null && mailData.isKnightMail())
					return true;
			}
		}
		return false;
	}

	public static void setMutiMailStatusByConfigType(String mailUids, int configType, boolean isUnLock)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "mailUids", mailUids, "configType", configType, "isUnLock",
				isUnLock);

		if (StringUtils.isEmpty(mailUids)
				|| !(configType == DBManager.CONFIG_TYPE_READ || configType == DBManager.CONFIG_TYPE_SAVE || configType == DBManager.CONFIG_TYPE_DELETE))
			return;
		String[] mailUidArr = mailUids.split(",");
		boolean hasDetectMail = false;
		ChatChannel channel = null;

		if (isKnightMail(mailUidArr))
		{
			deleteMail("knight", DBDefinition.CHANNEL_TYPE_OFFICIAL, -1);
			return;
		}

		for (int i = 0; i < mailUidArr.length; i++)
		{
			if (StringUtils.isNotEmpty(mailUidArr[i]))
			{
				MailData mail = DBManager.getInstance().getSysMailByID(mailUidArr[i]);
				if (mail != null)
				{
					channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());

					if (configType == DBManager.CONFIG_TYPE_READ && mail.isUnread())
					{
						if (channel != null)
							channel.unreadCount--;
						mail.setStatus(1);
						DBManager.getInstance().updateMail(mail);
					}
					else if (configType == DBManager.CONFIG_TYPE_SAVE)
					{
						if (!isUnLock && !mail.isLock())
						{
							mail.setSave(1);
							DBManager.getInstance().updateMail(mail);
						}
						else if (isUnLock && mail.isLock())
						{
							mail.setSave(0);
							DBManager.getInstance().updateMail(mail);
						}
					}
					else if (configType == DBManager.CONFIG_TYPE_DELETE)
					{
						ChannelManager.getInstance().deleteSysMailFromChannel(channel, mailUidArr[i], true);
						if (!hasDetectMail && mail.getType() == MailManager.MAIL_DETECT_REPORT)
							hasDetectMail = true;
						continue;
					}

					if (channel == null || channel.mailDataList == null)
						return;

					for (int j = 0; j < channel.mailDataList.size(); j++)
					{
						MailData mailData = channel.mailDataList.get(j);
						if (mailData != null && mailUidArr[i].equals(mailData.getUid()))
						{
							if (configType == DBManager.CONFIG_TYPE_READ && mailData.isUnread())
							{
								mailData.setStatus(1);
								break;
							}
							else if (configType == DBManager.CONFIG_TYPE_SAVE)
							{
								if (!isUnLock && !mailData.isLock())
								{
									mailData.setSave(1);
								}
								else if (isUnLock && mailData.isLock())
								{
									mailData.setSave(0);
								}
								break;
							}
						}
					}

					if (configType == DBManager.CONFIG_TYPE_READ)
					{
						channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
						DBManager.getInstance().updateChannel(channel);
					}
				}
			}
		}

		if (hasDetectMail)
			DBManager.getInstance().getDetectMailInfo();

		if (channel != null && channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && configType == DBManager.CONFIG_TYPE_DELETE)
			channel.querySysMailCountFromDB();
		ChannelManager.getInstance().calulateAllChannelUnreadNum();

		if (ChatServiceController.hostActivity == null)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChannelListFragment() != null)
					{
						ChatServiceController.getChannelListFragment().notifyDataSetChanged();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void setMutiMailStatusByType(int type, int configType, boolean isUnLock)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "mailUids", type, "configType", configType, "isUnLock", isUnLock);

		if (type < 0
				|| !(configType == DBManager.CONFIG_TYPE_READ || configType == DBManager.CONFIG_TYPE_SAVE || configType == DBManager.CONFIG_TYPE_DELETE))
			return;
		String channelId = "";
		if (type == MailManager.MAIL_RESOURCE)
			channelId = MailManager.CHANNELID_RESOURCE;
		else if (type == MailManager.MAIL_RESOURCE_HELP)
			channelId = MailManager.CHANNELID_RESOURCE_HELP;
		else if (type == MailManager.MAIL_ATTACKMONSTER)
			channelId = MailManager.CHANNELID_MONSTER;
		else if (type == MsgItem.MAIL_MOD_PERSON)
			channelId = MailManager.CHANNELID_MOD;
		else if (type == 0 || type == 1 || type == MailManager.MAIL_Alliance_ALL)
			channelId = MailManager.CHANNELID_MESSAGE;

		if (StringUtils.isEmpty(channelId))
			return;
		if (channelId.equals(MailManager.CHANNELID_RESOURCE) || channelId.equals(MailManager.CHANNELID_RESOURCE_HELP)
				|| channelId.equals(MailManager.CHANNELID_MONSTER))
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
			if (channel != null)
			{
				List<String> unreadUids = channel.getMailUidArrayByConfigType(configType);
				for (int i = 0; i < unreadUids.size(); i++)
				{
					String uid = unreadUids.get(i);
					if (StringUtils.isNotEmpty(uid))
					{
						MailData mail = DBManager.getInstance().getSysMailByID(uid);
						if (mail != null)
						{
							if (configType == DBManager.CONFIG_TYPE_READ && mail.isUnread())
							{
								mail.setStatus(1);
								DBManager.getInstance().updateMail(mail);
							}
							else if (configType == DBManager.CONFIG_TYPE_SAVE)
							{
								if (!isUnLock && !mail.isLock())
								{
									mail.setSave(1);
									DBManager.getInstance().updateMail(mail);
								}
								else if (isUnLock && mail.isLock())
								{
									mail.setSave(0);
									DBManager.getInstance().updateMail(mail);
								}
							}
						}
					}
				}

				for (int j = 0; j < channel.mailDataList.size(); j++)
				{
					MailData mailData = channel.mailDataList.get(j);
					if (mailData != null)
					{
						if (configType == DBManager.CONFIG_TYPE_READ && mailData.isUnread())
						{
							mailData.setStatus(1);
						}
						else if (configType == DBManager.CONFIG_TYPE_SAVE)
						{
							if (!isUnLock && !mailData.isLock())
							{
								mailData.setSave(1);
							}
							else if (isUnLock && mailData.isLock())
							{
								mailData.setSave(0);
							}
						}
					}
				}

				if (configType == DBManager.CONFIG_TYPE_READ)
				{
					channel.unreadCount = 0;
					channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
					DBManager.getInstance().updateChannel(channel);
					ChannelManager.getInstance().calulateAllChannelUnreadNum();
				}

			}
		}
		else if (channelId.equals(MailManager.CHANNELID_MESSAGE))
		{
			List<ChatChannel> messageChannelArr = ChannelManager.getInstance().getAllMessageChannel();
			if (messageChannelArr != null && messageChannelArr.size() > 0)
			{
				for (int i = 0; i < messageChannelArr.size(); i++)
				{
					ChatChannel messageChannel = messageChannelArr.get(i);
					if (messageChannel != null)
					{
						messageChannel.markAsRead();
					}
				}
			}

			ChatChannel channel = ChannelManager.getInstance().getMessageChannel();
			if (channel != null)
			{
				channel.unreadCount = 0;
				channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
				DBManager.getInstance().updateChannel(channel);
				ChannelManager.getInstance().calulateAllChannelUnreadNum();
			}

		}

		if (ChatServiceController.hostActivity == null)
			return;
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChannelListFragment() != null)
					{
						ChatServiceController.getChannelListFragment().notifyDataSetChanged();
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void postMailUpdate(String updateData)
	{
		if (updateData.equals(""))
			return;
		ChannelManager.mailUpdateData = updateData;
		try
		{
			MailUpdateData updateDate = JSON.parseObject(updateData, MailUpdateData.class);
			ChannelManager.getInstance().updateMailData(updateDate);

		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}

	}

	public static void postMailDeleteStatus(String mailUid)
	{
		if (mailUid.equals(""))
			return;
		ChannelManager.getInstance().deleteMailFrom2dx(mailUid);
	}

	public static String getLastMailUpdateTime()
	{
		String ret = "";
		long latestModifyTime = ChannelManager.getInstance().getLatestSysMailModifyTime();

		if (latestModifyTime > 0)
		{
			ret = Long.toString(latestModifyTime);
		}
		return ret;
	}

	public static void postMailDealStatus(String mailUid)
	{
		if (mailUid.equals(""))
			return;
		ChannelManager.getInstance().dealMailFrom2dx(mailUid);
	}

	public static void postTranslatedResult(String jsonRet)
	{
		if (StringUtils.isNotEmpty(jsonRet))
		{
			try
			{
				TranslatedByLuaResult result = JSON.parseObject(jsonRet, TranslatedByLuaResult.class);
				if (result != null && StringUtils.isNotEmpty(result.getOriginalMsg())
						&& TranslateManager.getInstance().isInTranslateQueue(result.getOriginalMsg()))
				{
					TranslateManager.getInstance().handleTranslateResult(result);
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}

	public static void postTranslateByLuaStart()
	{
		TranslateManager.isTranslatedByLuaStart = true;
	}

	public static void postUIShow()
	{
		TranslateManager.isUIShow = true;
	}

	public static void setMailNewUIEnable(boolean newUIEnable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "newUIEnable", newUIEnable);
		ChatServiceController.isNewMailUIEnable = newUIEnable;
	}

	public static void setMailSortType(int sortType)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "sortType", sortType);
		ChatServiceController.sortType = sortType;
	}

	public static boolean isStickMsg(String msg)
	{
		return StickManager.getPredefinedEmoj(msg) != null;
	}

	public static void setDefaultTranslateEnable(boolean isEnable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "isEnable", isEnable);
		ChatServiceController.isDefaultTranslateEnable = isEnable;
	}

	public static void setFriendEnable(boolean isEnable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "isEnable", isEnable);
		ChatServiceController.isFriendEnable = isEnable;
	}

	public static void setDetectInfoEnable(boolean isEnable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "isEnable", isEnable);
		ChatServiceController.isDetectInfoEnable = isEnable;
	}

	public static void setStandaloneServerEnable(int keyIndex, boolean isEnable)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "keyIndex", keyIndex, "isEnable", isEnable);
		switch (keyIndex)
		{
			case 1:
				ConfigManager.useWebSocketServer = isEnable;
				break;
			case 2:
				ConfigManager.isRecieveFromWebSocket = isEnable;
				break;
			case 3:
				ConfigManager.isSendFromWebSocket = isEnable;
				break;
		}
	}

	public static void rmDataBaseFile()
	{
		DBManager.getInstance().rmDatabaseFile();
	}

	public static void getDetectMailByMailUid(String mailUid)
	{
		if (StringUtils.isNotEmpty(mailUid))
		{
			MailData mail = DBManager.getInstance().getSysMailByID(mailUid);
			if (mail != null)
			{
				mail.setNeedParseByForce(true);
				MailData mailData = MailManager.getInstance().parseMailDataContent(mail);
				try
				{
					String jsonStr = JSON.toJSONString(mailData);
					MailManager.getInstance().transportMailInfo(jsonStr, true);
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		}
	}

	public static void loadMoreMailFromAndroid(String channelId)
	{
		if (StringUtils.isNotEmpty(channelId))
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
			if (channel != null && channel.mailDataList != null && channel.mailDataList.size() > 0)
			{
				MailData lastMail = channel.mailDataList.get(channel.mailDataList.size() - 1);
				if (lastMail != null)
				{
					ChannelManager.getInstance().loadMoreSysMailFromDB(channel, lastMail.getCreateTime());
				}
			}
		}
	}

	public static void setChannelPopupOpen(String channelId)
	{
		ChannelManager.currentOpenedChannel = channelId;
	}

	public static void postMutiRewardItem(String jsonStr)
	{
		try
		{
			final FlyMutiRewardInfo flyMutiReward = JSON.parseObject(jsonStr, FlyMutiRewardInfo.class);
			if (flyMutiReward != null)
			{
				if (ChatServiceController.hostActivity == null)
					return;
				ChatServiceController.hostActivity.runOnUiThread(new Runnable()
				{
					@Override
					public void run()
					{
						try
						{
							if (ChatServiceController.getChannelListFragment() != null)
							{
								ChatServiceController.getChannelListFragment().showMutiRewardFlyAnimation(flyMutiReward);
							}
						}
						catch (Exception e)
						{
							LogUtil.printException(e);
						}
					}
				});
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	private static boolean handleRedPackageInfo(String uidStr, int status)
	{
		Map<String, MsgItem> map = ChannelManager.getInstance().getUnHandleRedPackageMap();
		boolean hasCounteryRedPackage = false;
		boolean hasAllianceRedPackage = false;
		if (!map.containsKey(uidStr))
			return false;
		MsgItem msgItem = map.get(uidStr);
		if (msgItem != null && msgItem.sendState == MsgItem.UNHANDLE)
		{
			ChatChannel channel = null;
			if (msgItem.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
			{
				channel = ChannelManager.getInstance().getCountryChannel();
				hasCounteryRedPackage = true;
			}
			else if (msgItem.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
			{
				channel = ChannelManager.getInstance().getAllianceChannel();
				hasAllianceRedPackage = true;
			}
			if (channel != null)
			{
				if (msgItem.sendState != status)
				{
					msgItem.sendState = status;
					DBManager.getInstance().updateMessage(msgItem, channel.getChatTable());
				}

				for (int j = 0; j < channel.msgList.size(); j++)
				{
					MsgItem item = channel.msgList.get(j);
					if (item != null && msgItem.attachmentId.equals(item.attachmentId))
					{
						if (item.sendState != status)
						{
							item.sendState = status;
							channel.msgList.set(j, item);
						}
						break;
					}
				}

				if (msgItem.sendState == MsgItem.UNHANDLE && status != MsgItem.UNHANDLE)
					map.remove(uidStr);
			}
		}
		return hasCounteryRedPackage | hasAllianceRedPackage;
	}

	public static void postRedPackageGotUids(String redpackageInfo)
	{
		String[] uidArray = redpackageInfo.split(",");
		boolean hasRedPackageChange = false;
		for (int i = 0; i < uidArray.length; i++)
		{
			if (StringUtils.isNotEmpty(uidArray[i]))
			{
				String[] redPackageInfoArr = uidArray[i].split("\\|");
				if (redPackageInfoArr.length == 2)
				{
					hasRedPackageChange = handleRedPackageInfo(redPackageInfoArr[0], Integer.parseInt(redPackageInfoArr[1]));
				}

			}
		}

//		if (ChatServiceController.hostActivity == null || !hasRedPackageChange)
//			return;
//		notifyDataSetChangedChatFragment();
	}

	public static void notifyDataSetChangedChatFragment()
	{
		ChatServiceController.hostActivity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getChatFragment() != null)
					{
						ChatServiceController.getChatFragment().notifyDataSetChanged(ChatServiceController.getCurrentChannelType(),false);
					}
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void postRedPackageStatus(String redPackageUid, int status)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "redPackageUid", redPackageUid, "status", status);
		MsgItem currentPopupItem = null;// ChatServiceController.getChatFragment().getCurrentRedPackageItem();
		if (ChatServiceController.getChatFragment() != null)
		{
			currentPopupItem = ChatServiceController.getChatFragment().getCurrentRedPackageItem();
		}
		if (StringUtils.isEmpty(redPackageUid) || status <= 0)
		{
			if (ChatServiceController.getChatFragment() != null)
				ChatServiceController.getChatFragment().hideRedPackageConfirm();

			if (currentPopupItem != null)
				ChatServiceController.doHostAction("pickRedPackage", "", "", currentPopupItem.attachmentId, true);
			return;
		}
		if (ChatServiceController.getChatFragment() != null)
		{

			if (currentPopupItem != null)
			{
				if (currentPopupItem.attachmentId.equals(redPackageUid) && currentPopupItem.sendState == MsgItem.UNHANDLE
						&& status != MsgItem.UNHANDLE)
				{
					currentPopupItem.sendState = status;
					ChatServiceController.getChatFragment().showRedPackageConfirm(currentPopupItem);
				}
				else
				{
					ChatServiceController.getChatFragment().hideRedPackageConfirm();
					ChatServiceController.doHostAction("pickRedPackage", "", "", currentPopupItem.attachmentId, true);
				}
			}
		}
		boolean hasChange = handleRedPackageInfo(redPackageUid, status);
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "hasChange", hasChange);
//		if (ChatServiceController.hostActivity == null || !hasChange)
//			return;
//		notifyDataSetChangedChatFragment();
	}

	public static void postRedPackageDuringTime(int time)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "time", time);
		ChatServiceController.red_package_during_time = time;
	}

	public static String getFriendLatestMails(String uids)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "uids", uids);
		if (StringUtils.isEmpty(uids))
			return "";
		String[] uidArr = uids.split("_");
		List<FriendLatestMail> friendMailList = new ArrayList<FriendLatestMail>();
		for (int i = 0; i < uidArr.length; i++)
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_USER, uidArr[i]);
			if (channel != null)
			{
				MsgItem mail = channel.getLatestUserMail();
				if (mail != null)
				{
					String latestMsg = mail.msg;
					if (StringUtils.isNotEmpty(mail.translateMsg))
						latestMsg = mail.translateMsg;
					FriendLatestMail friendMail = new FriendLatestMail(uidArr[i], latestMsg);
					if (friendMail != null)
						friendMailList.add(friendMail);
				}
			}
		}

		try
		{
			String friendMailJson = JSON.toJSONString(friendMailList);
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "friendMailJson", friendMailJson);
			return friendMailJson;
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return "";
	}

	public static String getChatLatestMessage()
	{
		System.out.println("getChatLatestMessage");
		ChatChannel countryChannel = ChannelManager.getInstance().getCountryChannel();
		ChatChannel allianceChannel = ChannelManager.getInstance().getAllianceChannel();
		LatestCountryAllianceChatInfo chatInfo = new LatestCountryAllianceChatInfo();
		if (countryChannel != null)
		{
			MsgItem latestCountryMsg = DBManager.getInstance().getChatLatestMsg(countryChannel.getChatTable());
			if (latestCountryMsg != null)
			{
				System.out.println("getChatLatestMessage latestCountryMsg!=null");
				LatestChatInfo countryInfo = new LatestChatInfo();
				countryInfo.setMsgInfo(latestCountryMsg);
				chatInfo.setLatestCountryChatInfo(countryInfo);
			}
		}
		if (allianceChannel != null)
		{
			System.out.println("getChatLatestMessage allianceChannel!=null");
			MsgItem latestAllianceMsg = DBManager.getInstance().getChatLatestMsg(allianceChannel.getChatTable());
			if (latestAllianceMsg != null)
			{
				LatestChatInfo allianceInfo = new LatestChatInfo();
				allianceInfo.setMsgInfo(latestAllianceMsg);
				chatInfo.setLatestAllianceChatInfo(allianceInfo);
			}
		}

		try
		{
			String lateChatJson = JSON.toJSONString(chatInfo);
			System.out.println("lateChatJson:" + lateChatJson);
			return lateChatJson;
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return "";
	}

	public static void sendChatLatestMessage(ChatChannel channel)
	{
		MsgItem latestMsgItem = DBManager.getInstance().getChatLatestMsg(channel.getChatTable());
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "channel", channel, "channel.getChatTable()",
				channel.getChatTable(), "channel.getChatTable().getTableNameAndCreate()", channel.getChatTable().getTableNameAndCreate(),
				"latestMsgItem", latestMsgItem);
		if (latestMsgItem != null)
		{
			ChatServiceController.getInstance().postLatestChatMessage(latestMsgItem);
		}
	}

	public static void postKingUid(String kingUid)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "kingUid", kingUid);
		ChatServiceController.kingUid = kingUid;
	}

	public static void postBanTime(String banTime)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "banTime", banTime);
		ChatServiceController.banTime = banTime;
	}

	public static void postShieldUids(String shieldUids)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "shieldUids", shieldUids);
		String[] shieldArr = shieldUids.split("_");
		for (int i = 0; i < shieldArr.length; i++)
		{
			if (StringUtils.isNotEmpty(shieldArr[i]))
			{
				UserManager.getInstance().addRestrictUser(shieldArr[i], UserManager.BLOCK_LIST);
			}
		}
	}

	public static void postServerType(int serverType)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "serverType", serverType);
		ChatServiceController.serverType = serverType;
	}

	public static void showAllianceDialog()
	{
		System.out.println("showAllianceDialog");
		ChatServiceController.needShowAllianceDialog = true;
	}

	public static void postAddedMailListMail(String mailUid)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "mailUid", mailUid);
		if (StringUtils.isEmpty(mailUid))
			return;
		MailManager.getInstance().addMailInTransportedList(mailUid);
	}

	public static String getNeighborMail(String mailUid, int type)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "mailUid", mailUid, "type", type);
		if (StringUtils.isEmpty(mailUid) || !(type == 1 || type == 2))
			return "";
		MailData mail = DBManager.getInstance().getSysMailByID(mailUid);
		if (mail != null)
		{
			if (type == 1)
				return MailManager.getInstance().transportNeiberMailData(mail, true, false);
			else if (type == 2)
				return MailManager.getInstance().transportNeiberMailData(mail, false, true);
		}
		return "";
	}

	public static void postSwitch(String switchKey, String switchValue)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "switchKey", switchKey, "switchValue", switchValue);
		if (StringUtils.isEmpty(switchKey))
			return;
		if (switchKey.equals("chat_k10"))
			ChatServiceController.switch_chat_k10 = switchValue;
		else if (switchKey.equals("chat_k11"))
			ChatServiceController.switch_chat_k11 = switchValue;
	}

	public static void postPlayerLevel(int playerLevel)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "playerLevel", playerLevel);
		ChatServiceController.currentLevel = playerLevel;
	}
}
