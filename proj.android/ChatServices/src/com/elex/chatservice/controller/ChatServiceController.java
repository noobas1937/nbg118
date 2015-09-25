package com.elex.chatservice.controller;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import android.app.Activity;
import android.content.Intent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.elex.chatservice.R;
import com.elex.chatservice.host.DummyHost;
import com.elex.chatservice.host.GameHost;
import com.elex.chatservice.host.IHost;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.TranslateManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.view.ChannelListActivity;
import com.elex.chatservice.view.ChannelListFragment;
import com.elex.chatservice.view.ChatActivity;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.MainListFragment;
import com.elex.chatservice.view.MemberSelectorFragment;
import com.elex.chatservice.view.SysMailListFragment;
import com.elex.chatservice.view.actionbar.MyActionBarActivity;

public class ChatServiceController
{
	public static Activity					hostActivity;					// IF或wrapper的activity，原生未打开时依然存在
	private static Class<?>					hostClass;
	private static MyActionBarActivity		currentActivity;
	public IHost							host;

	public static boolean					isNativeShowing		= false;	// 仅在IF.onResume中重置为false，主要被IF使用
	public static boolean					isNativeStarting	= false;	// 主要被原生activity使用

	// C++传入的参数
	public static boolean 					isContactMod		= false;
	public static boolean					isHornItemUsed		= false;	// 是否使用喇叭道具
	public static boolean					isCreateChatRoom	= false;
	private static int						currentChatType		= -1;		// 刚进入时由C++设置，在java中可修改，退出后会再给C++

	public static boolean					isNewMailListEnable	= false;	// 是否使用新的邮件列表
	public static boolean					isNewMailUIEnable	= false;
	public static int						serverId;
	public static int						crossFightSrcServerId;
	public static boolean					isReturningToGame	= false;	// 仅在打开原生activity时重置为false，在IF.onResume中重置false的话，会导致无法记忆二级邮件列表
	
	private static ChatServiceController	instance;
	
	public static ChatServiceController getInstance()
	{
		if (instance == null)
		{
			instance = new ChatServiceController();
		}
		return instance;
	}

	public static void init(Activity a, boolean isDummyHost)
	{
		hostActivity = a;
		hostClass = a.getClass();
		System.out.println("hostClass: " + hostClass.getName());
		
//		ServiceInterface.setPlayerAllianceInfo("", "", -1,true);
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

	public boolean isInDummyHost()
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

	private static int	oldSendTime	= 0;	// 上一次发送时间
	private static boolean isSendIntervalValid()
	{
		boolean isValid = true;
		int sendTime = TimeManager.getInstance().getCurrentTime();
		if ((sendTime - oldSendTime) < ConfigManager.sendInterval)
		{
			Toast.makeText(currentActivity, LanguageManager.getLangByKey(LanguageKeys.TIP_SENDMSG_WARN), Toast.LENGTH_SHORT).show();
			isValid = false;
		}
		return isValid;
	}

	// 发送消息
	public static void sendMsg(final String messageText,final boolean isHornMsg,boolean usePoint)
	{
		// 极少情况会出现 chatActivity == null 或 chatActivity.chatFragment == null
		if (ChatServiceController.getCurrentChannelType() < 0 || !isSendIntervalValid() || getChatFragment() == null) return;

		getChatFragment().clearInput();

		int sendLocalTime=TimeManager.getInstance().getCurrentTime();
		int post=isHornMsg?6:0;
		
		// 创建消息对象，加入正在发送列表
		MsgItem msgItem = new MsgItem(UserManager.getInstance().getCurrentUser().uid, true, true,ChatServiceController.getCurrentChannelType(),
				post, messageText, sendLocalTime);
		msgItem.sendState = MsgItem.SENDING;
		msgItem.createTime = sendLocalTime;
		msgItem.initUserForSendedMsg();

		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if(channel == null){
			LogUtil.trackMessage("sendMsg() channel is null: currentChatType=" + ChatServiceController.getCurrentChannelType()
					+ " fromUid=" + UserManager.getInstance().getCurrentMail().opponentUid, "", "");
			return;
		}
		
		channel.sendingMsgList.add(msgItem);

		// 此时插入的数据只包括uid、msg、sendLocalTime、sendState、post、channelType
//		System.out.println("msg:" + LogUtil.typeToString(msgItem));
//		DBManager.getInstance().insertMessages(new MsgItem[]{msgItem}, channel.getChatTable());
		
		// 加入model，更新视图
		try
		{
			channel.addDummyMsg(msgItem);
//			System.out.println("add假msg: " + msgItem + " index:" + channel.msgList.indexOf(msgItem));
			getChatFragment().notifyDataSetChanged(ChatServiceController.getCurrentChannelType());
			// 发送后的行为（跳到最后一行）
			getChatFragment().afterSendMsgShowed(ChatServiceController.getCurrentChannelType());
			// 实际发给后台
			sendMsg2Server(channel, messageText, isHornMsg, usePoint, sendLocalTime);
			oldSendTime=TimeManager.getInstance().getCurrentTime();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private static void sendMsg2Server(ChatChannel channel, String messageText,boolean isHornMessage,boolean usePoint,int sendLocalTime)
	{
//		System.out.println("sendMsg2Server sendLocalTime:"+sendLocalTime+"  ChatActivity.currentChatType:"+ChatActivity.currentChatType);
		
		if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
//			System.out.println("roomID"+UserManager.getInstance().getCurrentMail().opponentUid);
			getInstance().host.sendChatRoomMsg(UserManager.getInstance().getCurrentMail().opponentUid, messageText, Integer.toString(sendLocalTime));
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
			String targetUid = "";
			targetUid = fromUid;
			
			int type = ChatServiceController.isContactMod ? MsgItem.MAIL_MOD_PERSON : UserManager.getInstance().getCurrentMail().type;
			System.out.println("发送邮件 type = " + type);
			
			getInstance().host.sendMailMsg(toName, "", messageText, allianceUid, 
											UserManager.getInstance().getCurrentMail().mailUid,
											UserManager.getInstance().getCurrentMail().isCurChannelFirstVisit, 
											type, 
											Integer.toString(sendLocalTime),targetUid);
		}
		else if (ChatServiceController.getCurrentChannelType() == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			if(!isHornMessage)
			{
				getInstance().host.sendChatMessage(messageText, DBDefinition.CHANNEL_TYPE_COUNTRY, sendLocalTime);
			}
			else
			{
				getInstance().host.sendHornMessage(messageText, usePoint,sendLocalTime);
				if(!usePoint)
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
			getInstance().host.sendChatMessage(messageText, DBDefinition.CHANNEL_TYPE_ALLIANCE, sendLocalTime);
		}
	}
	
	// 重发消息
	public static void resendMsg(MsgItem msgItem,boolean isHornMsg,boolean usePoint)
	{
		if(!isSendIntervalValid())
			return;
		// 显示转圈
		msgItem.sendState = MsgItem.SENDING;
		final MsgItem item = msgItem;

		getChatFragment().notifyDataSetChanged(item.channelType);

		ChatChannel channel = ChannelManager.getInstance().getChannel(ChatServiceController.getCurrentChannelType());
		if(channel != null){
			sendMsg2Server(channel,msgItem.msg,isHornMsg,usePoint,msgItem.sendLocalTime);
		}
	}

	// 与上一条消息的时间判断，让当前时间不能早于它（本地时间可能比服务器时间慢）
	// 服务器返回的时间不会覆盖这个时间
	// 如果本地时间快慢都会有问题（慢了时间会与旧的一样，快了会与后来的他人消息顺序错乱）
	// 应该改成用服务器时间（进入ChatActivity时的服务器时间 + delta）
	public static String getTime(int index)
	{
		String lastTime = "";
		int  lastTimeUTC = 0;
		ArrayList<MsgItem> msgList=ChannelManager.getInstance().getCurMsgListByIndex(index);
		if (msgList!=null && msgList.size()>0)
		{
			MsgItem item = msgList.get(msgList.size() - 1);
			if (item != null){
				lastTime = item.getSendTime();
				lastTimeUTC = item.createTime;
			}
		}

		Date lastDate = new Date((long) lastTimeUTC * 1000);
		Date curDate = new Date(TimeManager.getInstance().getCurrentTimeMS());

		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd  HH:mm");
		String time = formatter.format(curDate);
		if (lastTimeUTC > 0 && lastDate != null && lastDate.after(curDate)){
			time = lastTime;
		}

		return time;
	}
	
	public static void doHostAction(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup)
	{
		doHostAction(action, uid, name, attachmentId, returnToChatAfterPopup, false);
	}

	public static void doHostAction(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup, boolean reverseAnimation)
	{
		getInstance().host.setActionAfterResume(action, uid, name, attachmentId, returnToChatAfterPopup);
		// 可能出异常（极少）：NullPointerException
		// Attempt to invoke virtual method 'void com.elex.chatservice.view.ChatActivity.exitChatActivity()' on a null object reference
		try{
//			chatActivity.exitActivity();
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
					if(noTitle){
						activity.requestWindowFeature(Window.FEATURE_NO_TITLE);// 去掉标题栏
					}else{
//						activity.requestWindowFeature(Window.FEATURE_OPTIONS_PANEL);
//						activity.requestWindowFeature(Window.FEATURE_ACTION_BAR);// 去掉标题栏
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
		if (getChatFragment() == null) return;
		
		ChatServiceController.getCurrentActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (ChatServiceController.getCurrentActivity() != null)
					{
						if(getChatFragment() != null)
						{
							getChatFragment().notifyDataSetChanged(getChatFragment().getCurrentChannelView().channelType);
						}
						else if(getMemberSelectorFragment() != null)
						{
							getMemberSelectorFragment().notifyDataSetChanged();
						}
						else if(getChannelListFragment()!=null)
						{
							getChannelListFragment().notifyDataSetChanged();
						}
					}
				} catch (Exception e) {
					LogUtil.printException(e);
				}
			}
		});
	}

	public static void showGameActivity(Activity a) {
		showGameActivity(a, false);
	}
	public static void showGameActivity(Activity a, boolean reverseAnimation) {
		System.out.println("showGameActivity()");
		isReturningToGame = true;
		ServiceInterface.clearActivityStack();
		
		Intent intent = new Intent(a, hostClass);
		// Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
		a.startActivity(intent);
		if(!reverseAnimation){
			a.overridePendingTransition(R.anim.in_from_left, R.anim.out_to_right);
		}else{
			a.overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left_fast);
		}
	}

	public static boolean isInChatRoom()
	{
		return currentChatType==DBDefinition.CHANNEL_TYPE_CHATROOM;
	}

	public static boolean isInUserMail()
	{
		return (currentChatType==DBDefinition.CHANNEL_TYPE_USER);
	}

	public static boolean isInMailDialog()
	{
		return (currentChatType==DBDefinition.CHANNEL_TYPE_USER || currentChatType==DBDefinition.CHANNEL_TYPE_CHATROOM);
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
	
//	public static ChannelListFragment secondaryListFragment;
//	public static ChannelListFragment getSecondaryListFragment()
//	{
//		if (ChatServiceController.getCurrentActivity() != null && ChatServiceController.getCurrentActivity().fragment != null
//				&& secondaryListFragment != null)
//		{
//			return secondaryListFragment;
//		}
//		return null;
//	}
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
	
	
	public static boolean isInTheSameChannel(String channelId)
	{
		if(getChatFragment() != null){
			return getChatFragment().getCurrentChannel().chatChannel.channelID.equals(channelId);
		}
		return false;
	}
	
	// crossFightSrcServerId = -1 表示没有跨服， >=0表示现在处于跨服状态
	public static boolean isInCrossFightServer()
	{
		return crossFightSrcServerId > 0;
	}
	
	public static boolean isParseEnable()
	{
		if(getChannelListFragment() != null || getChatFragment() != null ||getMemberSelectorFragment() != null)
			return true;
		return false;
	}
}
