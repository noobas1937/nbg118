package com.elex.chatservice.controller;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.R;
import com.elex.chatservice.host.DummyHost;
import com.elex.chatservice.host.GameHost;
import com.elex.chatservice.host.IHost;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.LatestChatInfo;
import com.elex.chatservice.model.LatestCountryAllianceChatInfo;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;
import com.elex.chatservice.view.ChatActivity;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.MainListFragment;
import com.elex.chatservice.view.MemberSelectorFragment;
import com.elex.chatservice.view.MsgMailListFragment;
import com.elex.chatservice.view.SysMailListFragment;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class ChatServiceController
{
	public static Activity					hostActivity;																	// IF或wrapper的activity，原生未打开时依然存在
	private static Class<?>					hostClass;
	private static MyActionBarActivity		currentActivity;
	public IHost							host;

	public static boolean					isNativeShowing					= false;										// 仅在IF.onResume中重置为false，主要被IF使用
	public static boolean					isNativeStarting				= false;										// 主要被原生activity使用

	// C++传入的参数
	public static boolean					isContactMod					= false;
	public static boolean					isHornItemUsed					= false;										// 是否使用喇叭道具
	public static boolean					isCreateChatRoom				= false;
	private static int						currentChatType					= -1;											// 刚进入时由C++设置，在java中可修改，退出后会再给C++

	public static boolean					isNewMailListEnable				= false;										// 是否使用新的邮件列表
	public static boolean					isNewMailUIEnable				= true;
	public static int						serverId;
	/** crossFightSrcServerId = -1 表示没有跨服， >=0表示现在处于跨服状态 */
	public static int						crossFightSrcServerId;
	public static boolean					isReturningToGame				= false;										// 仅在打开原生activity时重置为false，在IF.onResume中重置false的话，会导致无法记忆二级邮件列表
	public static int						sortType						= -1;
	public static boolean					isDefaultTranslateEnable		= true;										// 默认翻译开关
	public static boolean					isFriendEnable					= true;										// 好友功能开关
	public static boolean					isDetectInfoEnable				= false;										// 侦察战报更新开关

	public static long						oldReportContentTime			= 0;
	public static long						REPORT_CONTENT_TIME_INTERVAL	= 30000;
	public static int						red_package_during_time			= 24;											// 红包到期时间
	public static String					kingUid							= "";											// 国王的UID
	public static String					banTime							= "1|2|3|4";									// 国王的UID
	public static boolean					isListViewFling					= false;
	public static int						serverType						= -1;
	public static boolean					needShowAllianceDialog			= false;										// 需要在联盟聊天输入框显示特定的dialog
	public static String					switch_chat_k10					= "cn_uc,cn1,cn_mihy,cn_wdj,cn_ewan,cn_anzhi";
	public static String					switch_chat_k11					= "5|6";
	public static int						currentLevel					= 1;
	public static int						sendTimeTextHeight				= 0;

	private static ChatServiceController	instance;

	public static ChatServiceController getInstance()
	{
		if (instance == null)
		{
			instance = new ChatServiceController();
		}
		return instance;
	}

	public static boolean isInDragonSencen()
	{
		return serverType == 3;
	}

	public static void init(Activity a, boolean isDummyHost)
	{
		hostActivity = a;
		hostClass = a.getClass();
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "hostClass", hostClass.getName());

		if (!isDummyHost)
		{
			getInstance().host = new GameHost();
		}
		else
		{
			getInstance().host = new DummyHost();
		}
	}

	public void reset()
	{
		UserManager.getInstance().reset();
		TranslateManager.getInstance().reset();
		ChannelManager.getInstance().reset();
	}

	public static void setCurrentActivity(MyActionBarActivity a)
	{
		currentActivity = a;
	}

	public static MyActionBarActivity getCurrentActivity()
	{
		return currentActivity;
	}

	public static boolean	isRunning	= false;

	public static boolean isAppInForeGround()
	{
		if (currentActivity != null)
		{
			return isRunning;
		}
		return false;
	}

	// TODO 删除此函数
	public boolean isInDummyHost()
	{
		return false;// host instanceof DummyHost;
	}

	public boolean isUsingDummyHost()
	{
		return host instanceof DummyHost;
	}

	public static void setCurrentChannelType(int type)
	{
		currentChatType = type;
	}

	public static int getCurrentChannelType()
	{
		return currentChatType;
	}

	private static long	oldSendTime	= 0;	// 上一次发送时间

	private static boolean isSendIntervalValid()
	{
		boolean isValid = true;
		long sendTime = System.currentTimeMillis();
		if ((sendTime - oldSendTime) < ConfigManager.sendInterval)
		{
			Toast.makeText(currentActivity, LanguageManager.getLangByKey(LanguageKeys.TIP_SENDMSG_WARN), Toast.LENGTH_SHORT).show();
			isValid = false;
		}
		return isValid;
	}

	public static boolean isChatRestrict()
	{
		boolean result = false;
		if (currentChatType == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			UserInfo user = UserManager.getInstance().getCurrentUser();
			String uid = UserManager.getInstance().getCurrentUserId();
			if (StringUtils.isNotEmpty(uid) && uid.length() >= 3)
			{
				String uidPostfix = uid.substring(uid.length() - 3, uid.length());
				if (StringUtils.isNumeric(uidPostfix))
				{
					int serverId = Integer.parseInt(uidPostfix);
					uidPostfix = "" + serverId;
					if (user != null && StringUtils.isNotEmpty(user.userName))
					{
						if (user.userName.startsWith("Empire") && user.userName.endsWith(uidPostfix))
							return true;
						else
							return false;
					}
				}

			}
		}
		return result;
	}

	// 发送消息
	public static void sendMsg(final String messageText, final boolean isHornMsg, boolean usePoint)
	{
		// 极少情况会出现 chatActivity == null 或 chatActivity.chatFragment == null
		if (ChatServiceController.getCurrentChannelType() < 0 || !isSendIntervalValid() || getChatFragment() == null)
			return;
		if (isChatRestrict())
		{
			MenuController.showChatRestrictConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_CHAT_RESTRICT));
			return;
		}

		getChatFragment().clearInput();

		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if (channel == null)
		{
			LogUtil.trackMessage("sendMsg() channel is null: currentChatType=" + ChatServiceController.getCurrentChannelType()
					+ " fromUid=" + UserManager.getInstance().getCurrentMail().opponentUid, "", "");
			return;
		}

		int sendLocalTime = TimeManager.getInstance().getCurrentTime();
		int post = isHornMsg ? 6 : 0;
		// 创建消息对象，加入正在发送列表
		MsgItem msgItem = new MsgItem(UserManager.getInstance().getCurrentUser().uid, true, true,
				ChatServiceController.getCurrentChannelType(), post, messageText, sendLocalTime);
		msgItem.sendState = MsgItem.SENDING;
		msgItem.createTime = sendLocalTime;
		msgItem.initUserForSendedMsg();
		if (channel.msgList != null && channel.msgList.size() > 0 && currentChatType != DBDefinition.CHANNEL_TYPE_USER)
		{
			MsgItem lastItem = channel.msgList.get(channel.msgList.size() - 1);
			if (lastItem != null)
				msgItem.sequenceId = lastItem.sequenceId + 1;
		}

		// 此时插入的数据只包括uid、msg、sendLocalTime、sendState、post、channelType
		channel.sendingMsgList.add(msgItem);

		// 加入model，更新视图
		try
		{
			channel.addDummyMsg(msgItem);
			channel.getTimeNeedShowMsgIndex();
			// getChatFragment().notifyDataSetChanged(ChatServiceController.getCurrentChannelType());
			// 发送后的行为（跳到最后一行）
			getChatFragment().afterSendMsgShowed(ChatServiceController.getCurrentChannelType());
			// 实际发给后台
			if (!WebSocketManager.isSendFromWebSocket(channel.channelType))
			{
				sendMsg2Server(channel, messageText, isHornMsg, usePoint, sendLocalTime);
			}
			else
			{
				sendMsg2WSServer(channel, messageText, isHornMsg, usePoint, sendLocalTime);
			}
			oldSendTime = System.currentTimeMillis();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private static void sendMsg2WSServer(ChatChannel channel, String messageText, boolean isHornMessage, boolean usePoint, int sendLocalTime)
	{
		if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			if (isHornMessage)
			{
			}
			WebSocketManager.getInstance().sendRoomMsg(messageText, sendLocalTime, channel);
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			WebSocketManager.getInstance().sendRoomMsg(messageText, sendLocalTime, channel);
		}
	}

	private static void sendMsg2Server(ChatChannel channel, String messageText, boolean isHornMessage, boolean usePoint, int sendLocalTime)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_MSG, "channelType", ChatServiceController.getCurrentChannelType(),
				"messageText", messageText);
		if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			JniController.getInstance().excuteJNIVoidMethod("sendChatRoomMsg",
					new Object[] { UserManager.getInstance().getCurrentMail().opponentUid, messageText, Integer.toString(sendLocalTime) });
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_USER)
		{
			String toName = "";
			String allianceUid = "";
			String fromUid = ChannelManager.getInstance().getModChannelFromUid(UserManager.getInstance().getCurrentMail().opponentUid);
			if (fromUid.equals(UserManager.getInstance().getCurrentUser().uid))
			{
				toName = LanguageManager.getLangByKey(LanguageKeys.TIP_ALLIANCE);
				allianceUid = UserManager.getInstance().getCurrentUser().allianceId;
			}
			else
			{
				toName = UserManager.getInstance().getCurrentMail().opponentName;
			}
			String targetUid = fromUid;

			int type = ChatServiceController.isContactMod ? MsgItem.MAIL_MOD_PERSON : UserManager.getInstance().getCurrentMail().type;

			JniController.getInstance().excuteJNIVoidMethod(
					"sendMailMsg",
					new Object[] {
							toName,
							"",
							messageText,
							allianceUid,
							UserManager.getInstance().getCurrentMail().mailUid,
							Boolean.valueOf(UserManager.getInstance().getCurrentMail().isCurChannelFirstVisit),
							Integer.valueOf(type),
							Integer.toString(sendLocalTime),
							targetUid });
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			if (!isHornMessage)
			{
				JniController.getInstance().excuteJNIVoidMethod("sendChatMessage",
						new Object[] { messageText, Integer.valueOf(DBDefinition.CHANNEL_TYPE_COUNTRY), Integer.toString(sendLocalTime) });
			}
			else
			{
				JniController.getInstance().excuteJNIVoidMethod("sendHornMessage",
						new Object[] { messageText, Boolean.valueOf(usePoint), Integer.toString(sendLocalTime) });

				if (!usePoint)
				{
					ConfigManager.isFirstUserHorn = false;
				}
				else
				{
					ConfigManager.isFirstUserCornForHorn = false;
				}
			}
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			JniController.getInstance().excuteJNIVoidMethod("sendChatMessage",
					new Object[] { messageText, Integer.valueOf(DBDefinition.CHANNEL_TYPE_ALLIANCE), Integer.toString(sendLocalTime) });
		}
	}

	// 重发消息
	public static void resendMsg(MsgItem msgItem, boolean isHornMsg, boolean usePoint)
	{
		if (!isSendIntervalValid())
			return;
		if (isChatRestrict())
		{
			MenuController.showChatRestrictConfirm(LanguageManager.getLangByKey(LanguageKeys.TIP_CHAT_RESTRICT));
			return;
		}
		// 显示转圈
		msgItem.sendState = MsgItem.SENDING;
		final MsgItem item = msgItem;

		if (getChatFragment() != null)
			getChatFragment().notifyDataSetChanged(item.channelType,false);

		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if (channel != null)
		{
			if (!WebSocketManager.isSendFromWebSocket(channel.channelType))
			{
				sendMsg2Server(channel, msgItem.msg, isHornMsg, usePoint, msgItem.sendLocalTime);
			}
			else
			{
				sendMsg2WSServer(channel, msgItem.msg, isHornMsg, usePoint, msgItem.sendLocalTime);
			}
		}
	}

	/**
	 * 与上一条消息的时间判断，让当前时间不能早于它（本地时间可能比服务器时间慢）
	 * <P>
	 * 服务器返回的时间不会覆盖这个时间
	 * <P>
	 * 如果本地时间快慢都会有问题（慢了时间会与旧的一样，快了会与后来的他人消息顺序错乱）
	 * <P>
	 * 应该改成用服务器时间（进入ChatActivity时的服务器时间 + delta）
	 * <P>
	 */
	public static String getTime(int index)
	{
		String lastTime = "";
		int lastTimeUTC = 0;
		ArrayList<MsgItem> msgList = ChannelManager.getInstance().getCurMsgListByIndex(index);
		if (msgList != null && msgList.size() > 0)
		{
			MsgItem item = msgList.get(msgList.size() - 1);
			if (item != null)
			{
				lastTime = item.getSendTime();
				lastTimeUTC = item.createTime;
			}
		}

		Date lastDate = new Date((long) lastTimeUTC * 1000);
		Date curDate = new Date(TimeManager.getInstance().getCurrentTimeMS());

		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd  HH:mm");
		String time = formatter.format(curDate);
		if (lastTimeUTC > 0 && lastDate != null && lastDate.after(curDate))
		{
			time = lastTime;
		}

		return time;
	}

	public static void doHostAction(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup)
	{
		doHostAction(action, uid, name, attachmentId, returnToChatAfterPopup, false);
	}

	public static void doHostAction(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup,
			boolean reverseAnimation)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW, "action", action, "returnToChat", returnToChatAfterPopup);

		JniController.getInstance().excuteJNIVoidMethod("setActionAfterResume",
				new Object[] { action, uid, name, attachmentId, Boolean.valueOf(returnToChatAfterPopup) });

		try
		{
			ChatServiceController.showGameActivity(ChatServiceController.getCurrentActivity(), reverseAnimation);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public static void toggleFullScreen(final boolean fullscreen, final boolean noTitle, final Activity activity)
	{
		activity.runOnUiThread(new Runnable()
		{
			public void run()
			{
				try
				{
					// TODO 删除noTitle参数
					if (noTitle)
					{
						activity.requestWindowFeature(Window.FEATURE_NO_TITLE);// 去掉标题栏
					}
					else
					{
						// activity.requestWindowFeature(Window.FEATURE_OPTIONS_PANEL);
						// activity.requestWindowFeature(Window.FEATURE_ACTION_BAR);//
						// 去掉标题栏
					}
					if (fullscreen)
					{
						activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
					}
					else
					{
						activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
					}
				}
				catch (Exception e)
				{
				}
			}
		});
	}

	// 重发消息
	public void notifyCurrentDataSetChanged()
	{
		if (getChatFragment() == null)
			return;

		ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					if (ChatServiceController.getCurrentActivity() != null)
					{
						if (getChatFragment() != null)
						{
							getChatFragment().notifyDataSetChanged(getChatFragment().getCurrentChannelView().channelType,false);
						}
						else if (getMemberSelectorFragment() != null)
						{
							getMemberSelectorFragment().notifyDataSetChanged();
						}
						else if (getChannelListFragment() != null)
						{
							getChannelListFragment().notifyDataSetChanged();
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

	public static void showGameActivity(Activity a)
	{
		showGameActivity(a, false);
	}

	public static void showGameActivity(Activity a, boolean reverseAnimation)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_VIEW);
		isReturningToGame = true;
		ServiceInterface.clearActivityStack();

		Intent intent = new Intent(a, hostClass);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
		a.startActivity(intent);
		if (!reverseAnimation)
		{
			a.overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
		}
		else
		{
			a.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left_fast);
		}
	}

	public static boolean isInChatRoom()
	{
		return currentChatType == DBDefinition.CHANNEL_TYPE_CHATROOM;
	}

	public static boolean isInUserMail()
	{
		return (currentChatType == DBDefinition.CHANNEL_TYPE_USER);
	}

	public static boolean isInMailDialog()
	{
		return (currentChatType == DBDefinition.CHANNEL_TYPE_USER || currentChatType == DBDefinition.CHANNEL_TYPE_CHATROOM);
	}

	public static MemberSelectorFragment getMemberSelectorFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof MemberSelectorFragment)
		{
			return (MemberSelectorFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static ChatActivity getChatActivity()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity() instanceof ChatActivity)
		{
			return (ChatActivity) ChatServiceController.getCurrentActivity();
		}
		return null;
	}

	public static ChatFragment getChatFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof ChatFragment)
		{
			return (ChatFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static ChannelListActivity getChannelListActivity()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity() instanceof ChannelListActivity)
		{
			return (ChannelListActivity) ChatServiceController.getCurrentActivity();
		}
		return null;
	}

	public static ChannelListFragment getChannelListFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof ChannelListFragment)
		{
			return (ChannelListFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static SysMailListFragment getSysMailListFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof SysMailListFragment)
		{
			return (SysMailListFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static MainListFragment getMainListFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof MainListFragment)
		{
			return (MainListFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static MsgMailListFragment getMsgListFragment()
	{
		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
				&& ChatServiceController.getCurrentActivity().fragment instanceof MsgMailListFragment)
		{
			return (MsgMailListFragment) ChatServiceController.getCurrentActivity().fragment;
		}
		return null;
	}

	public static boolean isInTheSameChannel(String channelId)
	{
		if (getChatFragment() != null && getChatFragment().getCurrentChannel() != null
				&& getChatFragment().getCurrentChannel().chatChannel != null
				&& StringUtils.isNotEmpty(getChatFragment().getCurrentChannel().chatChannel.channelID))
		{
			return getChatFragment().getCurrentChannel().chatChannel.channelID.equals(channelId);
		}
		return false;
	}

	public static boolean isInCrossFightServer()
	{
		return crossFightSrcServerId > 0;
	}

	public static boolean isParseEnable()
	{
		if (getChannelListFragment() != null || getChatFragment() != null || getMemberSelectorFragment() != null)
			return true;
		return false;
	}

	public static boolean isInnerVersion()
	{
		return ChatServiceController.hostActivity.getPackageName().equals("com.clash.of.kings.inner");
	}

	public static boolean isBetaVersion()
	{
		return ChatServiceController.hostActivity.getPackageName().equals("com.hcg.cok.beta");
	}

	public static int getChatRestrictLevel()
	{
		int level = 5;
		String channelName = MailManager.getInstance().getPublishChannelName();
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "channelName", channelName);
		if (StringUtils.isEmpty(channelName) || StringUtils.isNotEmpty(ChatServiceController.switch_chat_k11))
			return level;
		String[] switchArr = ChatServiceController.switch_chat_k11.split("\\|");
		if (switchArr.length != 2)
			return level;
		if (ChatServiceController.switch_chat_k10.contains(channelName))
		{
			if (StringUtils.isNumeric(switchArr[1]))
				level = Integer.parseInt(switchArr[1]);
		}
		else
		{
			if (StringUtils.isNumeric(switchArr[0]))
				level = Integer.parseInt(switchArr[0]);
		}
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "level", level);
		return level;
	}

	public static boolean isChatRestrictForLevel()
	{
		if (currentChatType == DBDefinition.CHANNEL_TYPE_COUNTRY)
			return currentLevel < getChatRestrictLevel();
		return false;
	}

	public void postLatestChatMessage(MsgItem msgItem)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "msgItem", msgItem);
		if (msgItem == null)
			return;

		LatestCountryAllianceChatInfo chatInfo = new LatestCountryAllianceChatInfo();
		LatestChatInfo latestChatInfo = new LatestChatInfo();
		latestChatInfo.setMsgInfo(msgItem);
		if (msgItem.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
			chatInfo.setLatestCountryChatInfo(latestChatInfo);
		else if (msgItem.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
			chatInfo.setLatestAllianceChatInfo(latestChatInfo);

		try
		{
			String lateChatMessage = JSON.toJSONString(chatInfo);
			LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "lateChatMessage", lateChatMessage);
			JniController.getInstance().excuteJNIVoidMethod("postChatLatestInfo", new Object[] { lateChatMessage });
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public boolean isDifferentDate(MsgItem item, List<MsgItem> items)
	{
		if (item == null || items == null)
			return true;
		int index = items.indexOf(item);
		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if (channel != null && channel.getMsgIndexArrayForTimeShow() != null && channel.getMsgIndexArrayForTimeShow().size() > 0)
		{
			if (channel.getMsgIndexArrayForTimeShow().contains(Integer.valueOf(index)))
				return true;
		}
		else
		{
			if (index == 0)
			{
				return true;
			}
			else if (index > 0 && items.get(index - 1) != null)
			{
				return !item.getSendTimeYMD().equals(items.get(index - 1).getSendTimeYMD());
			}
		}

		return false;
	}
}
