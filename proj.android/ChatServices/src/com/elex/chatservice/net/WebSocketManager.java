package com.elex.chatservice.net;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TimerTask;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.WsClient;
import org.apache.commons.lang.StringUtils;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.codebutler.android_websockets.WebSocketClient;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.HeadPicUtil.MD5;
import com.elex.chatservice.util.HttpRequestUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.MathUtil;

public class WebSocketManager
{
	private final static String			APP_ID				= "100001";
	public final static String			WS_SERVER_LIST_URL	= "http://184.173.82.76:8080/server/link";

	private static WebSocketManager		instance;

	private WsClient				client;
	private IWebSocketStatusListener	statusListener;
	private int							joinRoomCount		= 0;
	private String						protocol;
	private String						ip;
	private String						port;
	private ScheduledExecutorService	getServerListService;

	public static WebSocketManager getInstance()
	{
		if (instance == null)
		{
			instance = new WebSocketManager();
		}
		return instance;
	}

	private WebSocketManager()
	{
	}

	public void setStatusListener(IWebSocketStatusListener listener)
	{
		statusListener = listener;
	}
	
	/**
	 * 应该只调一次，以后断线会自动触发重连
	 */
	public void connect()
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE);
		if(!isWebSocketEnabled()) return;

        getServerListService = Executors.newSingleThreadScheduledExecutor();
        TimerTask timerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					getServerList();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};
		
		getServerListService.scheduleWithFixedDelay(timerTask, 100, 5000, TimeUnit.MILLISECONDS);
	}
	
	private void getServerList()
	{
		if (statusListener == null)
		{
			statusListener = WebSocketStatusHandler.getInstance();
		}
		
		statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_GET_SERVERLIST));
		
		String timeStr = Integer.toString(TimeManager.getInstance().getCurrentTime());
		String md5 = MathUtil.md5(MathUtil.md5(timeStr.substring(0, 3)) + MathUtil.md5(timeStr.substring(timeStr.length() - 3, timeStr.length())));
		String serverlistJson = HttpRequestUtil.sendGet(WS_SERVER_LIST_URL, "t=" + timeStr + "&s=" + md5);
		onGetServerList(serverlistJson);
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "timeStr", timeStr, "md5", md5, "serverlistJson", serverlistJson);
		
		if(StringUtils.isEmpty(ip) || StringUtils.isEmpty(port))
		{
			statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_GET_SERVERLIST_ERROR));
		}else{
			getServerListService.shutdown();
			connect2ws();
		}
	}

    private static int connectionTime = 0;
	private boolean forceDisconnect = false;
	private void connect2ws()
	{
		connectionTime++;
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "connectionTime", connectionTime);
		
		if(client != null){
			try
			{
				client.closeBlocking();
			}
			catch (InterruptedException e)
			{
				// TODO do something
				e.printStackTrace();
			}
		}
		client = null;
		
		statusListener.onConsoleOutput("Connecting");
		statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_CONNECTING));

		Map<String, String> header = new HashMap<String, String>();
		long time = TimeManager.getInstance().getCurrentTimeMS();
		header.put("APPID", APP_ID);
		header.put("TIME", String.valueOf(TimeManager.getInstance().getCurrentTimeMS()));
		header.put("UID", UserManager.getInstance().getCurrentUserId());
		header.put("SIGN", calcSign(APP_ID, UserManager.getInstance().getCurrentUserId(), time));

        try {
        	// "ws://cokchat-s2.elexapp.com:8088"
			client = new WsClient(protocol + "://" + ip + ":" + port, header, this, statusListener);
        } catch (URISyntaxException e) {
        	// TODO reconnect?
            e.printStackTrace();
        }
        
		client.connect();
	}
	
	public void forceClose()
	{
		forceDisconnect = true;
		client.close();
	}
	
	private int	reconnectRetryCount = RECONNECT_MAX_RETRY;
	private final static int RECONNECT_INTERVAL = 5;
	private final static int RECONNECT_MAX_RETRY = 99;
	private int reconnectCountDown = 0;

	private ScheduledExecutorService reconnectService;
	private TimerTask reconnectTimerTask;
	
	public synchronized void onConnectError()
	{
		synchronized (this)
		{
			if (reconnectCountDown == 0 && reconnectRetryCount >= 0 && !forceDisconnect)
			{
				reconnectCountDown = RECONNECT_INTERVAL + 1;
//				resetClientID();
				startReconnect();
			}
		}
	}
	
	/**
	 * 只会执行一次
	 */
	private synchronized void startReconnect()
	{
		if (reconnectService != null)
		{
			return;
		}

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS);

		reconnectService = Executors.newSingleThreadScheduledExecutor();
		reconnectTimerTask = new TimerTask()
		{
			@Override
			public void run()
			{
				try
				{
					checkReconnect();
				}
				catch (Exception e)
				{
					LogUtil.printException(e);
				}
			}
		};

		reconnectService.scheduleWithFixedDelay(reconnectTimerTask, 1000, 1000, TimeUnit.MILLISECONDS);
	}
	
	private synchronized void checkReconnect()
	{
		synchronized (this)
		{
			if(reconnectCountDown <= 0) return;
			
			reconnectCountDown--;
			if(reconnectCountDown <= 0 && !forceDisconnect){
				statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_RECONNECTING));
				reconnectRetryCount--;
				connect2ws();
			}else{
				statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.WEB_SOCKET_CONNECT_FAIL, reconnectCountDown + ""));
			}
		}
	}

	public static final String bytesToHexString(byte[] bArray)
	{
		StringBuffer sb = new StringBuffer(bArray.length);
		String sTemp;
		for (int i = 0; i < bArray.length; i++)
		{
			sTemp = Integer.toHexString(0xFF & bArray[i]);
			if (sTemp.length() < 2)
				sb.append(0);
			sb.append(sTemp.toUpperCase());
		}
		return sb.toString();
	}

	public void handleConnect()
	{
		setUserInfo();
		joinRoom();
		getNewMsgs();
	}

	private static String calcSign(String appid, String uid, long time)
	{
		return MD5.getMD5Str(MD5.getMD5Str(appid + uid) + time).toLowerCase();
	}

	private final static String	SEND_USER_MSG_COMMAND				= "chat.user";
	private final static String	LOGIN_SUCCESS_COMMAND				= "login.success";
	private final static String	RECIEVE_USER_MSG_COMMAND			= "push.chat.user";
	private final static String	JOIN_ROOM_COMMAND					= "room.join";
	private final static String	LEAVE_ROOM_COMMAND					= "room.leave";
	private final static String	SEND_ROOM_MSG_COMMAND				= "chat.room";
	private final static String	RECIEVE_ROOM_MSG_COMMAND			= "push.chat.room";
	private final static String	SET_USER_INFO_COMMAND				= "user.setInfo";
//	private final static String	GET_NEW_MSGS_COMMAND				= "history.rooms";
	private final static String	GET_NEW_MSGS_BY_TIME_COMMAND		= "history.roomsv2";
//	private final static String	GET_HISTORY_MSGS_COMMAND			= "history.room";
	private final static String	GET_HISTORY_MSGS_BY_TIME_COMMAND	= "history.roomv2";
	private final static String	ANOTHER_LOGIN_COMMAND				= "another.login";

	public void setUserInfo()
	{
		sendCommand(SET_USER_INFO_COMMAND, "info", getUserInfo());
	}
	
	private JSONObject getUserInfo()
	{
		try
		{
			JSONObject params = new JSONObject();
			params.put("uid", UserManager.getInstance().getCurrentUserId());
			params.put("lang", ConfigManager.getInstance().gameLang);
			return params;
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
		return null;
	}

	public void sendUserMsg()
	{
		sendCommand(SEND_USER_MSG_COMMAND, "uid", UserManager.getInstance().getCurrentUserId(), "msg", "test msg");
	}
	
	public void joinRoom()
	{
		joinRoomCount++;
		sendCommand(JOIN_ROOM_COMMAND, "roomId", getCountryRoomId(), "group", "country");
		if(UserManager.getInstance().isCurrentUserInAlliance()){
			sendCommand(JOIN_ROOM_COMMAND, "roomId", getAllianceRoomId(), "group", "alliance");
		}
	}

	public void leaveAllianceRoom()
	{
		if(UserManager.getInstance().isCurrentUserInAlliance()){
			sendCommand(LEAVE_ROOM_COMMAND, "roomId", getAllianceRoomId());
		}
	}

	public void sendRoomMsg(String messageText, int sendLocalTime, ChatChannel channel)
	{
		String roomId = channel.isCountryChannel() ? getCountryRoomId() : getAllianceRoomId();
		sendCommand(SEND_ROOM_MSG_COMMAND, "roomId", roomId, "msg", messageText, "sendTime", sendLocalTime);
	}
	
	/**
	 * 后台返回包括db最后时间在内（可能有时间一样的新消息）的所有新数据（或至多N条数据）
	 */
	public void getNewMsgs()
	{
		sendCommand(GET_NEW_MSGS_BY_TIME_COMMAND, "rooms", getRoomsParams());
	}
	
	private JSONObject getRoomsParams()
	{
		JSONObject params = null;
		try
		{
			params = new JSONObject();
			ArrayList<ChatChannel> channels = ChannelManager.getInstance().getNewServerChannels();
			for (int i = 0; i < channels.size(); i++)
			{
				ChatChannel channel = channels.get(i);
				if(channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY){
					LogUtil.printVariablesWithFuctionName(Log.VERBOSE, LogUtil.TAG_WS_SEND, "latestTime", TimeManager.getTimeInMS(channel.getLatestTime()));
					statusListener.onConsoleOutput("latestTime = " + channel.getLatestTime());
					statusListener.onConsoleOutput("latestTime = " + TimeManager.getTimeInMS(channel.getLatestTime()));
					params.put(getCountryRoomId(), TimeManager.getTimeInMS(channel.getLatestTime()));
				}else if(channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE){
					params.put(getAllianceRoomId(), TimeManager.getTimeInMS(channel.getLatestTime()));
				}
			}
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
		return params;
	}
	
	private void getHistoryMsgs(String roomId, int startSeqId, int endSeqId)
	{
		sendCommand(GET_HISTORY_MSGS_BY_TIME_COMMAND, "roomId", roomId, "start", startSeqId, "end", endSeqId);
	}
	
	public static boolean isWebSocketEnabled()
	{
		// || ChatServiceController.isBetaVersion()
		return ConfigManager.useWebSocketServer;// && (ChatServiceController.isInnerVersion() || ChatServiceController.getInstance().isUsingDummyHost());
	}
	
	public static boolean isRecieveFromWebSocket(int channelType)
	{
		return isWebSocketEnabled() && ConfigManager.isRecieveFromWebSocket
				&& isSupportedType(channelType);
	}
	
	public static boolean isSendFromWebSocket(int channelType)
	{
		return isWebSocketEnabled() && (ConfigManager.isSendFromWebSocket || ChatServiceController.getInstance().isUsingDummyHost())
				&& isSupportedType(channelType);
	}
	
	public static boolean isSupportedType(int channelType)
	{
		return channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE;
	}
	
	public boolean isConnected()
	{
		return client != null && client.isOpen();
	}

	public void sendCommand(String command, Object... args)
	{
		if(!isConnected()) return;
		
		statusListener.onConsoleOutput("send: " + command);
		try
		{
			JSONObject params = new JSONObject();
			for (int i = 0; i < args.length; i += 2)
			{
				if ((i + 1) < args.length)
				{
					params.put((String) args[i], args[i + 1]);
				}
			}
			actualSendCommand(command, params);
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
	}

	private void actualSendCommand(String command, JSONObject params) throws JSONException
	{
		if(!isConnected()) return;
		
		JSONObject jsonobj = new JSONObject();
		jsonobj.put("cmd", command);
		if (params.has("sendTime"))
		{
			jsonobj.put("sendTime", params.getInt("sendTime"));
			params.remove("sendTime");
		}
		else
		{
			long time = TimeManager.getInstance().getCurrentTimeMS();
			jsonobj.put("sendTime", time);
		}

		jsonobj.put("params", params);

		String output = String.format("%s: %s", command, jsonobj.toString());
		LogUtil.printVariables(Log.INFO, LogUtil.TAG_WS_SEND, output);
		statusListener.onConsoleOutput(output);
		
		client.send(jsonobj.toString());
	}

	public void handleMessage(String message)
	{
		if (message.equals("heartbeat"))
		{
			LogUtil.printVariables(Log.INFO, LogUtil.TAG_WS_RECIEVE, "heartbeat");
			return;
		}
		
		try
		{
			JSONObject json = new JSONObject(message);
			String command = json.getString("cmd");
			
			String output = String.format("%s: %s", command, message);
			LogUtil.printVariables(Log.INFO, LogUtil.TAG_WS_RECIEVE, output);
//			statusListener.onConsoleOutput(output);

			if(client.isMyMessage(json)){
				return;
			}
			
			if (json.has("data")) // 由服务端主动推送的数据
			{
				statusListener.onConsoleOutput("push: " + command);
				onRecieveMessage(message);
			}
			else
			// 客户端发送命令时，服务端处理完命令推送的数据
			{
				if (json.has("result"))
				{
					statusListener.onConsoleOutput("send success: " + command);
					onCommandSuccess(message);
				}
				else if (json.has("error")) // 发生错误
				{
//					ServiceInterface.flyHint(null, "", command + " error: " + message, 0, 0, false);
					statusListener.onConsoleOutput("send error: " + command);
				}
			}
		}
		catch (JSONException e)
		{
			LogUtil.printVariables(Log.INFO, LogUtil.TAG_WS_RECIEVE, "JSONException: " + message);
			LogUtil.printException(e);
		}
	}

	private void onCommandSuccess(String message)
	{
		try
		{
			JSONObject json = new JSONObject(message);
			String command = json.getString("cmd");
			JSONObject result = json.getJSONObject("result");
			
			if (command.equals(GET_NEW_MSGS_BY_TIME_COMMAND))
			{
				onGetNewMsg(result);
			}
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
	}

	private void onGetServerList(String serverlist)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_WS_STATUS, "serverlist", serverlist);
		statusListener.onConsoleOutput("onGetServerList:" + serverlist);
		
		try
		{
			JSONObject json = new JSONObject(serverlist);

			if (json.opt("message") != null && json.getString("message").equals("success"))
			{
				JSONObject data = json.getJSONObject("data");
				protocol = data.getString("protocol");
				ip = data.getString("ip");
				port = data.getString("port");
			}
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
	}
	
	private void onRecieveMessage(String message)
	{
		try
		{
			JSONObject json = new JSONObject(message);
			String command = json.getString("cmd");
			JSONObject data = json.getJSONObject("data");

			if (command.equals(RECIEVE_USER_MSG_COMMAND))
			{

			}
			else if (command.equals(RECIEVE_ROOM_MSG_COMMAND))
			{
				onRecieveRoomMessage(data);
			}else if (command.equals(ANOTHER_LOGIN_COMMAND))
			{
				// 同一个uid在不同地方登陆会这样，发生这种情况游戏应该就不让登陆了
				statusListener.onStatus(LanguageManager.getLangByKey(LanguageKeys.ANOTHER_LOGIN));
				forceClose();
			}else if (command.equals(LOGIN_SUCCESS_COMMAND))
			{
				client.setClientID(json);
			}
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
	}

	/**
	 * {"cmd":"push.chat.room","serverTime":1447749281156,
	 * "data":{"appId":"100001"
	 * ,"seqId":2,"sender":"909504798000489","roomId":"country_1",
	 * "msg":"vvvb","sendTime":1447749243505,"serverTime":1447749281155} }
	 */
	private void onRecieveRoomMessage(JSONObject data)
	{
		try
		{
			MsgItem item = parseMsgItem(data);
			if (item != null)
			{
				MsgItem[] dbItemsArray = { item };
				String customName = "";
				ChatChannel channel = getChannel(data.getString("group"));
				ServiceInterface.handleMessage(dbItemsArray, channel.channelID, customName, false, true);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	private MsgItem parseMsgItem(JSONObject msg)
	{
		try
		{
			MsgItem item = new MsgItem();
			
			// 除了从db获取，都为true
			item.isNewMsg = true;
			
			item.sequenceId = -1;
//			if (msg.opt("seqId") != null)
//			{
//				item.sequenceId = msg.getInt("seqId");
//			}
			
			item.uid = msg.getString("sender");
			item.msg = msg.getString("msg");
			try
			{
				// sendTime可能为字符串或long，不会出错，但预防性加个try
				if (msg.opt("sendTime") != null && StringUtils.isNotEmpty(msg.getString("sendTime")))
				{
					item.sendLocalTime = TimeManager.getTimeInS(msg.getLong("sendTime"));
				}
			}
			catch (JSONException e)
			{
				e.printStackTrace();
			}
			item.createTime = TimeManager.getTimeInS(msg.getLong("serverTime"));
			item.channelType = group2channelType(msg.getString("group"));
			item.sendState = 2;
			
			if (msg.opt("originalLang") != null)
			{
				item.originalLang = msg.getString("originalLang");
			}
			if (msg.opt("translationMsg") != null)
			{
				item.translateMsg = msg.getString("translationMsg");
			}
			
			item.post = 0;
			item.mailId = "";
//			
//			if(msg.optJSONObject("senderInfo") != null){
//				JSONObject senderInfo = msg.getJSONObject("senderInfo");
//			}
			JSONObject extra = null;
			if(msg.optJSONObject("extra") != null){
				extra = msg.getJSONObject("extra");

				if (extra.opt("post") != null)
				{
					item.post = extra.getInt("post");
				}
				else
				{
					item.post = 0;
				}
				
				if (extra.opt("reportUid") != null)
				{
					item.attachmentId = extra.getString("reportUid");
				}
				if (extra.opt("allianceId") != null)
				{
					String allianceId = extra.getString("allianceId");
				}
				if (extra.opt("dialog") != null)
				{
					String dialog = extra.getString("dialog");
				}
				if (extra.opt("inviteAlliance") != null)
				{
					String inviteAlliance = extra.getString("inviteAlliance");
				}
				if (extra.optJSONArray("msgarr") != null)
				{
					JSONArray msgarr = extra.getJSONArray("msgarr");
				}
				if (extra.opt("reportDef") != null)
				{
					String reportDef = extra.getString("reportDef");
				}
				if (extra.opt("reportAtt") != null)
				{
					String reportAtt = extra.getString("reportAtt");
				}
			}
			
			return item;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		return null;
	}
	
	private ChatChannel getChannel(String group)
	{
		return ChannelManager.getInstance().getChannel(group2channelType(group));
	}
	
	private void onGetNewMsg(JSONObject result)
	{
		try
		{
			JSONArray rooms = result.getJSONArray("rooms");
			for (int i = 0; i < rooms.length(); i++)
			{
				JSONObject room = rooms.getJSONObject(i);
				String roomId = room.getString("roomId"); 
				long firstMsgTime = room.getLong("firstMsgTime");
				long lastMsgTime = room.getLong("lastMsgTime");
				int firstSeqId = room.getInt("firstSeqId");
				int lastSeqId = room.getInt("lastSeqId");
				
				ChatChannel channel = getChannel(room.getString("group"));
				channel.serverMaxTime = lastMsgTime;
				channel.serverMinTime = firstMsgTime;
				
				JSONArray msgs = room.getJSONArray("msgs");
				
				channel.wsNewMsgCount = msgs.length();
				
				if(msgs.length() == 0)
				{
					channel.loadMoreMsg();
					continue;
				}
				
				MsgItem[] msgArr = new MsgItem[msgs.length()];
				MsgItem firstMsg = null;
				for (int j = 0; j < msgs.length(); j++)
				{
					JSONObject msg = msgs.getJSONObject(j);
					MsgItem item = parseMsgItem(msg);
					if (item != null)
					{
						msgArr[j] = item;

						if (firstMsg == null || item.createTime < firstMsg.createTime)
						{
							firstMsg = item;
						}
					}
				}

				if (channel.wsNewMsgCount < ChannelManager.LOAD_ALL_MORE_MAX_COUNT)
				{
					firstMsg.firstNewMsgState = 1;
				}
				else
				{
					firstMsg.firstNewMsgState = 2;
				}
				
				ServiceInterface.handleMessage(msgArr, roomId2channelId(roomId), "", true, true);
			}
		}
		catch (JSONException e)
		{
			LogUtil.printException(e);
		}
	}
	
	private static String roomId2channelId(String roomId)
	{
		return roomId.substring(roomId.lastIndexOf("_") + 1);
	}
	
	private static int group2channelType(String group)
	{
		return group.equals("country") ? DBDefinition.CHANNEL_TYPE_COUNTRY : DBDefinition.CHANNEL_TYPE_ALLIANCE;
	}

	public void handleDisconnect()
	{
//		ServiceInterface.flyHint(null, "", "disconnect", 0, 0, false);
	}
	
	/**
	 * id格式：<p>
	 * country_1<p>
	 * alliance_1_c79be2b653224cb4b1aeb5138ad15118<p>
	 * 
	 * test_country_1<p>
	 * test_alliance_1_c79be2b653224cb4b1aeb5138ad15118<p>
	 * 
	 * beta_country_107<p>
	 * beta_alliance_107_c79be2b653224cb4b1aeb5138ad15118<p>
	 */
	private static String getCountryRoomId()
	{
		return getRoomIdPrefix() + "country_" + UserManager.getInstance().getCurrentUser().serverId;
	}

	private static String getAllianceRoomId()
	{
		return getRoomIdPrefix() + "alliance_" + UserManager.getInstance().getCurrentUser().serverId + "_" + UserManager.getInstance().getCurrentUser().allianceId;
	}
	
	private static String getRoomIdPrefix()
	{
		if (ChatServiceController.isInnerVersion() || ChatServiceController.getInstance().isUsingDummyHost())
		{
			return "test_";
		}
		else if (ChatServiceController.isBetaVersion())
		{
			return "beta_";
		}
		else
		{
			return "";
		}
	}
}