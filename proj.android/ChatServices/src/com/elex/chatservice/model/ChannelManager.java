package com.elex.chatservice.model;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.lang.StringUtils;

import android.util.Log;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.db.ChatTable;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.updatedata.MailUpdateData;
import com.elex.chatservice.model.mail.updatedata.UpdateParam;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.SortUtil;
import com.elex.chatservice.view.ChannelListFragment;

public class ChannelManager implements Serializable
{
	private static final long						serialVersionUID		= 3664013024183969534L;

	public static final int							LOAD_MORE_COUNT			= 20;
	public static final int							LOAD_ALL_MORE_MIN_COUNT	= 20;
	public static final int							LOAD_ALL_MORE_MAX_COUNT	= 200;

	public static final int							OPERATION_DELETE_MUTI	= 1;
	public static final int							OPERATION_REWARD_MUTI	= 2;

	private static ChannelManager					instance;
	public boolean									isGetingNewMsg			= false;

	private ConcurrentHashMap<String, ChatChannel>	channelMap;
	public ChatChannel								countryChannel;
	public ChatChannel								allianceChannel;

	public boolean									isFetching;
	public static int								totalUnreadCount		= 0;
	public Map<String, Integer>						parseFrom2dxMap			= new HashMap<String, Integer>();
	public Map<String, Boolean>						parseLocalFinishMap		= new HashMap<String, Boolean>();

	private ArrayList<ChatChannel>					modChannelList			= null;
	private ArrayList<ChatChannel>					messageChannelList		= null;
	private ArrayList<ChatChannel>					loadedModChannelList;
	private ArrayList<ChatChannel>					loadedMessageChannelList;
	public String									latestModChannelMsg		= "";
	public String									latestMessageChannelMsg	= "";
	private ChatChannel								modChannel				= null;
	private ChatChannel								messageChannel			= null;
	public boolean									isInRootChannelList		= false;

	public List<String>								mailDeleteArray			= null;
	public Map<String, UpdateParam>					mailUpdateMap			= null;
	public static boolean							isHandlingChannelInfo	= false;
	private String									latestId_official		= "";
	private long									latestSysMailModifyTime	= 0;
	public static String							currentOpenedChannel	= "";
	private Map<String, MsgItem>					redPackageUnHandleMap	= null;

	public static String							mailUpdateData			= "";
	private String									firstChannelID			= null;

	public static ChannelManager getInstance()
	{
		if (instance == null)
		{
			synchronized (ChannelManager.class)
			{
				if (instance == null)
				{
					instance = new ChannelManager();
					instance.init();
				}
			}
		}
		return instance;
	}

	public static boolean isInited()
	{
		return instance != null;
	}

	private ChannelManager()
	{
	}

	public void reset()
	{
		init();
		getLoadedModChannel().clear();
		getLoadedMessageChannel().clear();
	}

	/**
	 * 从数据库初始化频道，只从db读取一次，新增的需要插入进来
	 */
	private void init()
	{
		System.out.println("ChannelManager.init()");
		channelMap = new ConcurrentHashMap<String, ChatChannel>();
		ArrayList<ChatChannel> dbChannels = DBManager.getInstance().getAllChannel();
		for (int i = 0; i < dbChannels.size(); i++)
		{
			ChatChannel dbChannel = dbChannels.get(i);
			if (!ChatServiceController.isNewMailUIEnable
					|| (ChatServiceController.isNewMailUIEnable && dbChannel.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL
							&& !dbChannel.channelID.equals(MailManager.CHANNELID_MOD) && !dbChannel.channelID
								.equals(MailManager.CHANNELID_MESSAGE)))
			{
				if (dbChannel.channelType == DBDefinition.CHANNEL_TYPE_USER && StringUtils.isEmpty(dbChannel.latestId))
				{
					continue;
				}
				channelMap.put(dbChannel.getChatTable().getChannelName(), dbChannel);
				System.out.println("init channelMap.put key:" + dbChannel.getChatTable().getChannelName());
			}
		}
		getCountryChannel();
		getAllianceChannel();
		if (ChatServiceController.isNewMailUIEnable)
			getNewSystemMailChannel();
		latestId_official = DBManager.getInstance().getSystemMailLatestId();
		latestSysMailModifyTime = DBManager.getInstance().getSystemMailLatestModifyTime();

		redPackageUnHandleMap = new HashMap<String, MsgItem>();
		getUnHandleRedPackage(countryChannel);
		getUnHandleRedPackage(allianceChannel);
	}

	public Map<String, MsgItem> getUnHandleRedPackageMap()
	{
		return redPackageUnHandleMap;
	}

	private void getUnHandleRedPackage(ChatChannel channel)
	{
		if (channel != null)
		{
			List<MsgItem> msgItemArray = DBManager.getInstance().getUnHandleRedPackage(channel.getChatTable());
			if (msgItemArray != null)
			{
				for (int i = 0; i < msgItemArray.size(); i++)
				{
					MsgItem msgItem = msgItemArray.get(i);
					if (msgItem == null)
						continue;
					if (msgItem.sendState < 0)
						msgItem.sendState = MsgItem.UNHANDLE;
					if (msgItem != null && StringUtils.isNotEmpty(msgItem.attachmentId)
							&& !redPackageUnHandleMap.containsKey(msgItem.attachmentId))
					{
						redPackageUnHandleMap.put(msgItem.attachmentId, msgItem);
					}
				}
			}
		}
	}

	public long getLatestSysMailModifyTime()
	{
		return latestSysMailModifyTime;
	}

	private void getNewSystemMailChannel()
	{
		String[] channelIdArray = {
				MailManager.CHANNELID_FIGHT,
				MailManager.CHANNELID_ALLIANCE,
				MailManager.CHANNELID_STUDIO,
				MailManager.CHANNELID_RESOURCE,
				MailManager.CHANNELID_MONSTER,
				MailManager.CHANNELID_KNIGHT,
				MailManager.CHANNELID_SYSTEM,
				MailManager.CHANNELID_EVENT };
		for (int i = 0; i < channelIdArray.length; i++)
		{
			if (StringUtils.isNotEmpty(channelIdArray[i]))
			{
				System.out.println("getNewSystemMailChannel:" + channelIdArray[i]);
				ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelIdArray[i]);
				ChatChannel channel = getChannel(chatTable);
				if (channel != null)
				{
					channel.latestId = channel.getLatestId();
					channel.latestTime = channel.getLatestTime();
				}
			}
		}
		if(firstChannelID == null)
			parseFirstChannelID();
	}

	public void removeChannelFromMap(String key)
	{
		if (channelMap != null && channelMap.containsKey(key))
			channelMap.remove(key);
	}

	private void initMsgChannel(ArrayList<ChatChannel> channelList, String channelId)
	{
		SortUtil.getInstance().refreshListOrder(channelList, ChatChannel.class);
		ChatChannel parentChannel = null;
		if (channelId.equals(MailManager.CHANNELID_MOD))
			parentChannel = modChannel;
		else if (channelId.equals(MailManager.CHANNELID_MESSAGE))
			parentChannel = messageChannel;

		if (channelList != null && channelList.size() > 0)
		{
			if (parentChannel == null)
			{
				parentChannel = new ChatChannel();
				parentChannel.channelType = DBDefinition.CHANNEL_TYPE_USER;
				parentChannel.channelID = channelId;
			}
			long latestCreateTime = 0;
			String latestId = "";
			int unreadCount = 0;
			ChatChannel latestChannel = null;
			for (int i = 0; i < channelList.size(); i++)
			{
				ChatChannel channel = channelList.get(i);
				if (channel != null)
				{
					if (channel.unreadCount > 0)
						unreadCount++;
					if (channel.latestTime >= latestCreateTime)
					{
						latestCreateTime = channel.latestTime;
						latestId = channel.latestId;
						latestChannel = channel;
					}

				}
			}
			parentChannel.latestId = latestId;
			parentChannel.latestTime = latestCreateTime;
			parentChannel.unreadCount = unreadCount;
			if (latestChannel != null && latestChannel.msgList != null && latestChannel.msgList.size() > 0)
			{
				MsgItem mail = latestChannel.msgList.get(latestChannel.msgList.size() - 1);
				if (mail != null)
				{
					if (channelId.equals(MailManager.CHANNELID_MOD))
						ChannelManager.getInstance().latestModChannelMsg = mail.msg;
					else if (channelId.equals(MailManager.CHANNELID_MESSAGE))
						ChannelManager.getInstance().latestMessageChannelMsg = mail.msg;
				}
			}
		}
		else if (ChatServiceController.isNewMailUIEnable && parentChannel == null)
		{
			parentChannel = new ChatChannel();
			parentChannel.channelType = DBDefinition.CHANNEL_TYPE_USER;
			parentChannel.channelID = channelId;
			parentChannel.unreadCount = 0;
		}

		if (channelId.equals(MailManager.CHANNELID_MOD))
			modChannel = parentChannel;
		else if (channelId.equals(MailManager.CHANNELID_MESSAGE))
			messageChannel = parentChannel;
	}

	public synchronized ArrayList<ChannelListItem> getAllMailChannel()
	{
		ArrayList<ChannelListItem> channelList = new ArrayList<ChannelListItem>();
		if (channelMap != null)
		{
			Set<String> keySet = channelMap.keySet();
			if (keySet != null)
			{
				boolean modChannelInit = false;
				boolean messageChannelInit = false;
				if (modChannelList != null)
					modChannelList.clear();
				else
					modChannelList = new ArrayList<ChatChannel>();
				if (ChatServiceController.isNewMailUIEnable)
				{
					if (messageChannelList != null)
						messageChannelList.clear();
					else
						messageChannelList = new ArrayList<ChatChannel>();
				}
				for (String key : keySet)
				{
					ChatChannel chatChannel = channelMap.get(key);
					if (chatChannel != null && chatChannel.channelType != DBDefinition.CHANNEL_TYPE_COUNTRY
							&& chatChannel.channelType != DBDefinition.CHANNEL_TYPE_ALLIANCE && !chatChannel.hasNoItemInChannel())// &&
																																	// !chatChannel.hasInitLoaded()
					{
						if (chatChannel.isModChannel())
						{
							modChannelList.add(chatChannel);
						}
						else if (chatChannel.isMessageChannel() && ChatServiceController.isNewMailUIEnable)
						{
							messageChannelList.add(chatChannel);
						}
						else
						{
							if (!chatChannel.channelID.equals(MailManager.CHANNELID_MOD)
									&& !chatChannel.channelID.equals(MailManager.CHANNELID_MESSAGE))
								channelList.add(chatChannel);
						}
					}
				}
				initMsgChannel(modChannelList, MailManager.CHANNELID_MOD);
				if (modChannel != null && !modChannelInit && needModChannel())
				{
					channelList.add(modChannel);
					if(firstChannelID!=null && !firstChannelID.equals(MailManager.CHANNELID_MOD))
						parseFirstChannelID();
					modChannelInit = true;
				}

				if (ChatServiceController.isNewMailUIEnable)
				{
					initMsgChannel(messageChannelList, MailManager.CHANNELID_MESSAGE);
					if (messageChannel != null && !messageChannelInit)
					{
						channelList.add(messageChannel);
						messageChannelInit = true;
					}
				}
			}
		}
		return channelList;
	}

	public String getFirstChannelID()
	{
		if(firstChannelID == null)
			parseFirstChannelID();
		return firstChannelID;
	}
	
	public void parseFirstChannelID()
	{
		firstChannelID = "";
		if (needModChannel())
			firstChannelID = MailManager.CHANNELID_MOD;
		else
		{
			ChatChannel monsterChannel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, MailManager.CHANNELID_MONSTER);
			if (monsterChannel != null && !monsterChannel.hasNoItemInChannel())
				firstChannelID = MailManager.CHANNELID_MONSTER;
			else
			{
				ChatChannel knightChannel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, MailManager.CHANNELID_KNIGHT);
				if (knightChannel != null && !knightChannel.hasNoItemInChannel())
					firstChannelID = MailManager.CHANNELID_KNIGHT;
				else
				{
					ChatChannel resourceChannel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, MailManager.CHANNELID_RESOURCE);
					if (resourceChannel != null && !resourceChannel.hasNoItemInChannel())
						firstChannelID = MailManager.CHANNELID_RESOURCE;
				}
			}
		}
	}

	public List<ChatChannel> getAllModChannel()
	{
		return modChannelList;
	}

	public List<ChatChannel> getAllMessageChannel()
	{
		return messageChannelList;
	}

	public List<ChatChannel> getLoadedModChannel()
	{
		if (loadedModChannelList == null)
		{
			loadedModChannelList = new ArrayList<ChatChannel>();
		}
		return loadedModChannelList;
	}

	public List<ChatChannel> getLoadedMessageChannel()
	{
		if (loadedMessageChannelList == null)
		{
			loadedMessageChannelList = new ArrayList<ChatChannel>();
		}
		return loadedMessageChannelList;
	}

	private void removeFromLoadedChannel(ChatChannel channel)
	{
		if (getLoadedModChannel().indexOf(channel) >= 0)
		{
			getLoadedModChannel().remove(channel);
		}
		if (getLoadedMessageChannel().indexOf(channel) >= 0)
		{
			getLoadedMessageChannel().remove(channel);
		}
	}

	public void addToLoadedChannel(ChatChannel channel)
	{
		List<ChatChannel> loadedChannels = getLoadedMsgChannels(channel);
		if (loadedChannels != null)
		{
			if (!isChannelInList(channel, loadedChannels))
			{
				loadedChannels.add(channel);
				if (getModChannel() != null)
					getModChannel().refreshRenderData();
				if (getMessageChannel() != null)
					getMessageChannel().refreshRenderData();
			}
		}
	}

	public static <T> boolean isChannelInList(ChatChannel channel, List<T> channels)
	{
		if (channel == null || channels == null)
			return false;

		if (channels.contains(channel))
		{
			return true;
		}
		for (int i = 0; i < channels.size(); i++)
		{
			if (channels.get(i) != null && channels.get(i) instanceof ChatChannel
					&& ((ChatChannel) channels.get(i)).channelID.equals(channel.channelID))
			{
				return true;
			}
		}

		return false;
	}

	private List<ChatChannel> getLoadedMsgChannels(ChatChannel channel)
	{
		if (channel != null)
		{
			if (channel.isModChannel())
			{
				return ChannelManager.getInstance().getLoadedModChannel();
			}
			else if (channel.isMessageChannel())
			{
				return ChannelManager.getInstance().getLoadedMessageChannel();
			}
		}
		return null;
	}

	public ChatChannel getModChannel()
	{
		return modChannel;
	}

	public ChatChannel getMessageChannel()
	{
		return messageChannel;
	}

	public Map<String, ChatChannel> getAllSysMailChannelMap()
	{
		Map<String, ChatChannel> systemMailMap = new HashMap<String, ChatChannel>();
		if (channelMap != null)
		{
			Set<String> keySet = channelMap.keySet();
			if (keySet != null)
			{
				for (String key : keySet)
				{
					ChatChannel chatChannel = channelMap.get(key);
					if (chatChannel != null && chatChannel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL
							&& chatChannel.mailDataList != null && chatChannel.mailDataList.size() > 0)
					{
						for (int i = 0; i < chatChannel.mailDataList.size(); i++)
						{
							MailData mail = chatChannel.mailDataList.get(i);
							if (mail != null)
							{
								systemMailMap.put(mail.getUid(), chatChannel);
							}
						}
					}
				}
			}
		}
		return systemMailMap;
	}

	public List<ChatChannel> getAllSysMailChannel()
	{
		List<ChatChannel> channelList = new ArrayList<ChatChannel>();
		if (channelMap != null)
		{
			Set<String> keySet = channelMap.keySet();
			if (keySet != null)
			{
				for (String key : keySet)
				{
					ChatChannel chatChannel = channelMap.get(key);
					if (chatChannel != null && chatChannel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
					{
						channelList.add(chatChannel);
					}
				}
			}
		}
		return channelList;
	}

	public ChatChannel getCountryChannel()
	{
		if (UserManager.getInstance().getCurrentUser() == null)
			return null;

		ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_COUNTRY,
				Integer.toString(UserManager.getInstance().getCurrentUser().serverId));
		countryChannel = this.getChannel(chatTable);
		return countryChannel;
	}

	/**
	 * 可能有多个联盟或国家频道，需要动态取当前对应的频道
	 */
	public ChatChannel getAllianceChannel()
	{
		if (UserManager.getInstance().isCurrentUserInAlliance())
		{
			ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_ALLIANCE,
					UserManager.getInstance().getCurrentUser().allianceId);
			allianceChannel = this.getChannel(chatTable);
		}
		else
		{
			allianceChannel = null;
		}
		return allianceChannel;
	}

	private ChatChannel initChannel(int channelType, String channelID)
	{
		ChatChannel channel = new ChatChannel();
		channel.channelType = channelType;
		if (channelID != null)
		{
			channel.channelID = channelID;
		}
		if (ChatServiceController.isNewMailUIEnable
				&& (channel.channelID.equals(MailManager.CHANNELID_MOD) || channel.channelID.equals(MailManager.CHANNELID_MESSAGE)
						|| channel.channelID.equals(MailManager.CHANNELID_NOTICE) || channel.channelID
							.equals(MailManager.CHANNELID_RESOURCE_HELP)))
			return channel;
		channelMap.put(channel.getChatTable().getChannelName(), channel);

		if(channel.isDialogChannel())
			parseFirstChannelID();
		return channel;
	}

	public ChatChannel addDummyChannel(int channelType, String channelID)
	{
		return initChannel(channelType, channelID);
	}

	private void removeChannel(String key)
	{
		if (StringUtils.isNotEmpty(key))
		{
			try
			{
				channelMap.remove(key);
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}

	/**
	 * 收取消息接口需要的参数
	 * 格式："id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
	 */
	public String getChannelInfo()
	{
		String result = "";
		simulateReturnChannelInfo = "";
		newServerChannels = new ArrayList<ChatChannel>();
		long latestCreateTime_user = 0;
		String latestId_user = "";
		String latestChannelId_user = "";

		Iterator<String> it = channelMap.keySet().iterator();
		while (it.hasNext())
		{
			String key = it.next();
			ChatChannel channel = channelMap.get(key);

			if (!channel.containCurrentUser())
			{
				continue;
			}
			if (channel.channelType != DBDefinition.CHANNEL_TYPE_USER && channel.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
			{
				if (!WebSocketManager.isRecieveFromWebSocket(channel.channelType))
				{
					result = addCommaToParam(result);
					simulateReturnChannelInfo = addCommaToParam(simulateReturnChannelInfo);
					result += channel.channelID + "|" + channel.dbMaxSeqId + "|" + channel.channelType;
					simulateReturnChannelInfo += channel.channelType + "|" + channel.channelID + "|" + (channel.dbMaxSeqId - 1) + "|"
							+ channel.dbMaxSeqId;
					channel.prevDBMaxSeqId = channel.dbMaxSeqId;
				}
				else
				{
					newServerChannels.add(channel);
				}
			}
			else if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER && !channel.channelID.equals(MailManager.CHANNELID_MOD)
					&& !channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
			{
				if (StringUtils.isNotEmpty(channel.latestId) && channel.latestTime > latestCreateTime_user)
				{
					latestCreateTime_user = channel.latestTime;
					latestId_user = channel.latestId;
					latestChannelId_user = channel.channelID;
				}
			}
		}

		if (ChatServiceController.isNewMailListEnable)
		{
			result = addCommaToParam(result);

			if (StringUtils.isNotEmpty(latestId_official))
			{
				result += "0|" + latestId_official + "|" + DBDefinition.CHANNEL_TYPE_OFFICIAL;
			}
			else
			{
				result += "0|0|" + DBDefinition.CHANNEL_TYPE_OFFICIAL;
			}

			result = addCommaToParam(result);

			if (latestCreateTime_user != 0 && StringUtils.isNotEmpty(latestId_user))
			{
				result += latestChannelId_user + "|" + latestId_user + "|" + DBDefinition.CHANNEL_TYPE_USER;
			}
			else
			{
				result += "0|0|" + DBDefinition.CHANNEL_TYPE_USER;
			}
		}

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "result", result);
		return result;
	}

	private String addCommaToParam(String param)
	{
		if (StringUtils.isNotEmpty(param))
		{
			param += ",";
		}
		return param;
	}

	private String	simulateReturnChannelInfo	= "";

	public String getSimulateReturnChannelInfo()
	{
		ServiceInterface.getChannelInfo();

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "simulateReturnChannelInfo", simulateReturnChannelInfo);

		return simulateReturnChannelInfo;
	}

	private ArrayList<ChatChannel>	newServerChannels;

	public ArrayList<ChatChannel> getNewServerChannels()
	{
		return newServerChannels;
	}

	/**
	 * 收取消息的channel信息返回值处理 此前消息已收到，缓存和db中的channel都更新了 格式：
	 * "channelType|id|firstSeqId|lastSeqId,channelType|id|firstSeqId|lastSeqId"
	 */
	public void handleChannelInfo(String channelInfo)
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_CORE, "channelInfo", channelInfo);

		if (StringUtils.isEmpty(channelInfo))
			return;
		isHandlingChannelInfo = true;
		String[] channels = channelInfo.split(",");
		List<String> channelIdArray = new ArrayList<String>();
		for (int i = 0; i < channels.length; i++)
		{
			String[] fields = channels[i].split("\\|");
			String channelId = fields[1];
			int channelType = Integer.parseInt(fields[0]);
			if (channelId.equals("null"))
			{
				if (channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
				{
					channelId = UserManager.getInstance().getCurrentUser().serverId + "";
				}
				else if (channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
				{
					channelId = UserManager.getInstance().getCurrentUser().allianceId;
				}
			}
			ChatTable chatTable = new ChatTable(channelId, channelType);
			System.out.println("channelName:" + chatTable.getChannelName() + " channel:" + channelMap.get(chatTable.getChannelName())
					+ "channelMap.size:" + channelMap.size());
			ChatChannel channel = ChannelManager.getInstance().getChannel(chatTable);
			if (channel != null)
			{
				channel.hasLoadingAllNew = false;
				if (channelType != DBDefinition.CHANNEL_TYPE_USER && channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
				{
					if (!WebSocketManager.isRecieveFromWebSocket(channelType))
					{
						channel.serverMaxSeqId = Integer.parseInt(fields[3]);
						channel.serverMinSeqId = Integer.parseInt(fields[2]);
						if (channel.prevDBMaxSeqId > 0 && !channel.hasInitLoaded())
						{
							channel.loadMoreMsg();
							DBManager.getInstance().updateChannel(channel);
						}
					}
				}
				else
				{
					preProcessSysMailChannel(channel);
				}
				channelIdArray.add(channel.getChatTable().getChannelName());
			}
		}

		Set<String> keySet = channelMap.keySet();
		for (String key : keySet)
		{
			if (!key.equals("") && !channelIdArray.contains(key) && channelMap.get(key) != null)
			{
				preProcessSysMailChannel(channelMap.get(key));
			}
		}

		if (ChatServiceController.getChatFragment() != null)
		{
			ChatServiceController.getChatFragment().refreshHasMoreData();
		}

		isHandlingChannelInfo = false;
		calulateAllChannelUnreadNum();
	}

	private void preProcessSysMailChannel(ChatChannel channel)
	{
		if (channel.channelID.equals(MailManager.CHANNELID_MONSTER) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE)
				|| channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP) || channel.channelID.equals(MailManager.CHANNELID_KNIGHT))
		{
			loadMoreSysMailFromDB(channel, -1);
		}
		calcUnreadCount(channel);
	}

	/**
	 * TODO channel与forceUseTime重复了，条件判断冗余，参数也有点过长。可以拆开，或者修改调用方式
	 */
	public void loadMoreMsgFromDB(ChatChannel channel, int minSeqId, int maxSeqId, int minCreateTime, boolean forceUseTime)
	{
		if (channel == null)
			return;

		if (forceUseTime || channel.channelType == DBDefinition.CHANNEL_TYPE_USER)
		{
			List<MsgItem> dbItems = DBManager.getInstance().getMsgsByTime(channel.getChatTable(), minCreateTime, LOAD_MORE_COUNT);
			if (dbItems != null)
			{
				MsgItem[] dbItemsArray = (MsgItem[]) dbItems.toArray(new MsgItem[0]);
				if (dbItemsArray.length <= 0)
					return;

				ServiceInterface.handleMessage(dbItemsArray, channel.channelID, channel.customName, false, false);
			}
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE
				|| channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			List<MsgItem> dbItems = DBManager.getInstance().getChatMsgBySection(channel.getChatTable(), maxSeqId, minSeqId);
			MsgItem[] dbItemsArray = (MsgItem[]) dbItems.toArray(new MsgItem[0]);
			if (dbItemsArray.length <= 0)
				return;
			ServiceInterface.handleMessage(dbItemsArray, channel.channelID, channel.customName, false, false);
		}
	}

	public int getSysMailDBCount(ChatChannel channel)
	{
		return DBManager.getInstance().getSysMailDBCountByTime(channel.getChatTable(), -1);
	}

	private void calcUnreadCount(ChatChannel channel)
	{
		if (channel == null || channel.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			return;
		}
		if (isNeedCalculateUnreadCount(channel.channelID))
		{
			channel.setUnreadCount(DBManager.getInstance().getUnreadCountOfSysMail(channel.getChatTable()));
		}
	}

	public void loadMoreSysMailFromDB(ChatChannel channel, int lastMailCreateTime)
	{
		if (channel == null || channel.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			return;
		}

		final List<MailData> moreItems = DBManager.getInstance().getSysMailByTime(channel.getChatTable(), lastMailCreateTime, 20);
		if (moreItems.size() <= 0)
		{
			if (ChatServiceController.getSysMailListFragment() != null)
			{
				ChatServiceController.getSysMailListFragment().onLoadMoreComplete();
			}
			return;
		}

		Runnable run = new Runnable()
		{
			@Override
			public void run()
			{
				if (moreItems.size() > 0)
				{
					final List<MailData> mailDataArr = new ArrayList<MailData>();
					for (int i = 0; i < moreItems.size(); i++)
					{
						MailData mailData = moreItems.get(i);
						if (mailData != null)
						{
							MailData mail = ServiceInterface.parseMailData(mailData, true);
							if (mail != null)
							{
								// System.out.println("mailUid:"+mail.getUid());
								mailDataArr.add(mail);
							}
						}
					}

					if (mailDataArr.size() > 0)
					{
						ChatServiceController.hostActivity.runOnUiThread(new Runnable()
						{
							@Override
							public void run()
							{
								try
								{
									ChatChannel channel = null;
									String channelId = "";
									for (int i = 0; i < mailDataArr.size(); i++)
									{
										MailData mail = mailDataArr.get(i);
										if (mail != null)
										{
											if (StringUtils.isEmpty(channelId))
												channelId = mail.getChannelId();
											channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL,
													mail.getChannelId());
											if (channel != null)
											{
												channel.addNewMailData(mail);
											}
										}
									}

									if (channel != null && channel.mailDataList != null)
										SortUtil.getInstance().refreshListOrder(channel.mailDataList, MailData.class);

									if (StringUtils.isNotEmpty(currentOpenedChannel) && currentOpenedChannel.equals(channelId))
										postNotifyPopup(channelId);

									if (ChatServiceController.getSysMailListFragment() != null)
									{
										ChatServiceController.getSysMailListFragment().onLoadMoreComplete();
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
			}
		};
		Thread thread = new Thread(run);
		thread.start();
	}

	public void postNotifyPopup(String channelId)
	{
		if (channelId.equals(MailManager.CHANNELID_MONSTER) || channelId.equals(MailManager.CHANNELID_RESOURCE)
				|| channelId.equals(MailManager.CHANNELID_KNIGHT))
		{
			ChatChannel channel = ChannelManager.getInstance().getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, channelId);
			MailData mail = null;
			if (channel.channelID.equals(MailManager.CHANNELID_MONSTER))
				mail = channel.getMonsterMailData();
			else if (channel.channelID.equals(MailManager.CHANNELID_RESOURCE))
				mail = channel.getResourceMailData();
			else if (channel.channelID.equals(MailManager.CHANNELID_KNIGHT))
				mail = channel.getKnightMailData();

			if (mail != null)
			{
				try
				{
					String jsonStr = JSON.toJSONString(mail);
					MailManager.getInstance().transportMailInfo(jsonStr, false);
//					JniController.getInstance().excuteJNIVoidMethod("postNotifyMailPopup", null);

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
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * 现在加载的maxId是视图中最小seqId，会被手动加载历史消息改变，而minId是maxId-200，
	 * 可能需要变为初始化时的serverMaxId
	 */
	public void loadAllNew(ChatChannel channel)
	{
		int minSeqId = channel.getNewMsgCount() < LOAD_ALL_MORE_MAX_COUNT ? channel.getNewMsgMinSeqId() : (channel.getNewMsgMaxSeqId()
				- LOAD_ALL_MORE_MAX_COUNT + 1);
		channel.firstNewMsgSeqId = minSeqId;
		JniController.getInstance().excuteJNIVoidMethod(
				"getMsgBySeqId",
				new Object[] {
						Integer.valueOf(minSeqId),
						Integer.valueOf(channel.getNewMsgMaxSeqId()),
						Integer.valueOf(channel.channelType),
						channel.channelID });
	}

	public static int channelType2tab(int channelType)
	{
		if (channelType <= DBDefinition.CHANNEL_TYPE_ALLIANCE)
			return channelType;
		else if (channelType == DBDefinition.CHANNEL_TYPE_CHATROOM || channelType == DBDefinition.CHANNEL_TYPE_USER)
			return 2;
		return 0;
	}

	/**
	 * 仅当视图存在时才返回，找不到不会创建
	 */
	public ChatChannel getChannelByViewIndex(int index)
	{
		ChatChannel result = null;
		if (ChatServiceController.getChatFragment() != null)
		{
			result = ChatServiceController.getChatFragment().getChannelView(index).chatChannel;
		}
		return result;
	}

	/**
	 * 用于精确知道channelType和channelId的情况 如果找不到会创建
	 */
	public ChatChannel getChannel(ChatTable chatTable)
	{
		ChatChannel channel = channelMap.get(chatTable.getChannelName());
		if (channel == null)
		{
			channel = DBManager.getInstance().getChannel(chatTable);
			if (channel == null)
			{
				if (chatTable.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
				{
					channel = initChannel(DBDefinition.CHANNEL_TYPE_COUNTRY,
							Integer.toString(UserManager.getInstance().getCurrentUser().serverId));
				}
				else if (chatTable.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
				{
					if (UserManager.getInstance().isCurrentUserInAlliance())
						channel = initChannel(DBDefinition.CHANNEL_TYPE_ALLIANCE, UserManager.getInstance().getCurrentUser().allianceId);
				}
				else
				{
					channel = initChannel(chatTable.channelType, chatTable.channelID);
					ChannelListFragment.onChannelAdd();
				}
				if (channel != null)
				{
					channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
					DBManager.getInstance().insertChannel(channel);
				}
			}
			else
			{
				if (ChatServiceController.isNewMailUIEnable
						&& (channel.channelID.equals(MailManager.CHANNELID_NOTICE) || channel.channelID
								.equals(MailManager.CHANNELID_RESOURCE_HELP)))
					return channel;
				channelMap.put(chatTable.getChannelName(), channel);
				if(channel.isDialogChannel() || channel.channelID.equals(MailManager.CHANNELID_MOD))
					parseFirstChannelID();
			}
		}
		return channel;
	}

	public ChatChannel getChannel(int channelType)
	{
		String channelId = "";
		if (channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			channelId = UserManager.getInstance().getCurrentUser().serverId + "";
		}
		else if (channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			channelId = UserManager.getInstance().getCurrentUser().allianceId;
		}
		else
		{
			channelId = UserManager.getInstance().getCurrentMail().opponentUid;
			if (ChatServiceController.isContactMod
					&& !UserManager.getInstance().getCurrentMail().opponentUid.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
				channelId += DBDefinition.CHANNEL_ID_POSTFIX_MOD;
		}

		return ChannelManager.getInstance().getChannel(channelType, channelId);
	}

	/**
	 * 用于fromUid(channelId)不一定存在的情况
	 * 
	 * @return null(如果channelType不为国家或联盟，且channelId为空时)
	 */
	public ChatChannel getChannel(int channelType, String fromUid)
	{
		ChatChannel chatChannel = null;
		if (channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
		{
			chatChannel = getCountryChannel();
		}
		else if (channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
		{
			chatChannel = getAllianceChannel();
		}
		else if (!StringUtils.isEmpty(fromUid))
		{
			ChatTable chatTable = ChatTable.createChatTable(channelType, fromUid);
			chatChannel = getChannel(chatTable);
		}
		else
		{
			LogUtil.trackMessage("ChatChannel.getChannel return null, channelType=" + channelType + " fromUid=" + fromUid, "", "");
		}

		return chatChannel;
	}

	public void setIsMemberFlag(String groupId, boolean flag)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (channel != null)
		{
			channel.setMember(flag);
			DBManager.getInstance().updateChannel(channel);
		}
	}

	public boolean getIsMemberFlag(String groupId)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (channel != null)
		{
			return channel.isMember();
		}
		return false;
	}

	public String getChatRoomMemberStr(String groupId)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (channel != null)
		{
			return ChatChannel.getMembersString(channel.memberUidArray);
		}
		return "";
	}

	public void setChannelMemberArray(String groupId, String uidStr, String roomName)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (!uidStr.equals(""))
		{
			channel.memberUidArray.clear();
			String[] uidArr = uidStr.split("_");
			for (int i = 0; i < uidArr.length; i++)
			{
				if (!uidArr[i].equals(""))
					channel.memberUidArray.add(uidArr[i]);
			}
			channel.isMemberUidChanged = true;
		}
		if (!roomName.equals(""))
			channel.customName = roomName;
		DBManager.getInstance().updateChannel(channel);
	}

	public void updateChannelMemberArray(String groupId, String uidStr, boolean isAdd)
	{
		if (uidStr.equals(""))
			return;
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (channel.memberUidArray == null)
		{
			if (isAdd)
				channel.memberUidArray = new ArrayList<String>();
			else
				return;
		}
		String[] uidArr = uidStr.split("_");
		for (int i = 0; i < uidArr.length; i++)
		{
			if (!uidArr[i].equals(""))
			{
				if (isAdd && !channel.memberUidArray.contains(uidArr[i]))
				{
					channel.memberUidArray.add(uidArr[i]);
					channel.isMemberUidChanged = true;
				}
				else if (!isAdd && channel.memberUidArray.contains(uidArr[i]))
				{
					channel.memberUidArray.remove(uidArr[i]);
					channel.isMemberUidChanged = true;
				}
			}
		}
		DBManager.getInstance().updateChannel(channel);
	}

	public void setChatRoomFounder(String groupId, String founderUid)
	{
		if (founderUid.equals(""))
			return;
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if (channel != null)
			channel.roomOwner = founderUid;
		DBManager.getInstance().updateChannel(channel);
	}

	public ArrayList<String> getChatRoomMemberArrayByKey(String groupId)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		ArrayList<String> memberUidArray = channel.memberUidArray;
		if (memberUidArray != null && !UserManager.getInstance().getCurrentUser().uid.equals("")
				&& !memberUidArray.contains(UserManager.getInstance().getCurrentUser().uid))
		{
			memberUidArray.add(UserManager.getInstance().getCurrentUser().uid);
		}
		return memberUidArray;
	}

	public String getChatRoomFounderByKey(String groupId)
	{
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		return channel.roomOwner;
	}

	public ArrayList<MsgItem> getCurMsgListByIndex(int index)
	{
		return getChannelByViewIndex(index).msgList;
	}

	public void setNoMoreDataFlag(int index, boolean flag)
	{
		if (getChannelByViewIndex(index) != null)
		{
			getChannelByViewIndex(index).noMoreDataFlag = flag;
		}
	}

	public boolean getNoMoreDataFlag(int index)
	{
		if (getChannelByViewIndex(index) == null)
			return false;

		return getChannelByViewIndex(index).noMoreDataFlag;
	}

	public void setHasRequestDataBeforeFlag(int channelType, String fromUid, boolean flag)
	{
		if (channelType != DBDefinition.CHANNEL_TYPE_USER || (channelType == DBDefinition.CHANNEL_TYPE_USER && fromUid.equals("")))
		{
			return;
		}
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_USER, fromUid);
		if (channel != null)
		{
			channel.hasRequestDataBefore = flag;
		}
	}

	public void removeAllMailMsgByUid(final String fromUid)
	{
		final ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_USER, fromUid);
		if (channel != null)
		{
			ChatServiceController.hostActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					try
					{
						channel.msgList.clear();
						setHasRequestDataBeforeFlag(DBDefinition.CHANNEL_TYPE_USER, fromUid, false);
						channel.getChannelView().getMessagesAdapter().notifyDataSetChanged();
					}
					catch (Exception e)
					{
						LogUtil.printException(e);
					}
				}
			});
		}

	}

	public void deleteChatroomChannel(ChatTable chatTable)
	{
		removeChannel(chatTable.getChannelName());

		DBManager.getInstance().deleteChannel(chatTable);
	}

	public void deleteChannel(ChatChannel channel)
	{
		if (channel == null || channel.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY
				|| channel.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
			return;

		if (!ChatServiceController.isNewMailUIEnable
				|| (ChatServiceController.isNewMailUIEnable
						&& (channel.channelType == DBDefinition.CHANNEL_TYPE_USER
								&& !channel.channelID.equals(MailManager.CHANNELID_MESSAGE) && !channel.channelID
									.equals(MailManager.CHANNELID_MOD)) || channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM || (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL && channel
						.isDialogChannel())))
		{
			ChannelManager.getInstance().removeChannelFromMap(channel.getChatTable().getChannelName());
		}

		if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER || channel.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			if (channel.channelID.equals(MailManager.CHANNELID_MOD))
			{
				List<ChatChannel> modChannelArr = getAllModChannel();
				if (modChannelArr != null && modChannelArr.size() > 0)
				{
					for (int i = 0; i < modChannelArr.size(); i++)
					{
						ChatChannel modChannel = modChannelArr.get(i);
						if (modChannel != null)
						{
							DBManager.getInstance().deleteChannel(modChannel.getChatTable());
						}
					}
				}
				parseFirstChannelID();
			}
			else if (channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
			{
				List<ChatChannel> messageChannelArr = getAllMessageChannel();
				if (messageChannelArr != null && messageChannelArr.size() > 0)
				{
					for (int i = 0; i < messageChannelArr.size(); i++)
					{
						ChatChannel messageChannel = messageChannelArr.get(i);
						if (messageChannel != null)
						{
							DBManager.getInstance().deleteChannel(messageChannel.getChatTable());
						}
					}
				}
			}
			else
			{
				DBManager.getInstance().deleteChannel(channel.getChatTable());
			}
		}
		else if (channel.channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			channel.unreadCount = 0;
			if (channel.isDialogChannel())
			{
				DBManager.getInstance().deleteDialogMailChannel(channel.getChatTable());
				parseFirstChannelID();
			}
			else
				DBManager.getInstance().deleteSysMailChannel(channel.getChatTable());
		}

		removeFromLoadedChannel(channel);

		calulateAllChannelUnreadNum();
	}

	public void deleteSysMailFromChannel(ChatChannel channel, String mailId, boolean isDeleteMuti)
	{
		if (channel == null)
			return;
		boolean hasDetectMail = false;
		if (channel.mailDataList != null && channel.mailDataList.size() > 0)
		{
			for (int i = 0; i < channel.mailDataList.size(); i++)
			{
				MailData mailData = channel.mailDataList.get(i);
				if (mailData != null && mailData.getUid().equals(mailId))
				{
					if (mailData.getType() == MailManager.MAIL_DETECT_REPORT && !isDeleteMuti)
						hasDetectMail = true;
					channel.mailDataList.remove(mailData);
					channel.mailUidList.remove(mailData.getUid());
					if (channel.mailDataList.size() == 0 && channel.isDialogChannel())
					{
						deleteChannel(channel);
					}
					else
					{
						if (mailData.isUnread())
						{
							channel.unreadCount--;
							ChannelListFragment.onChannelRefresh();
						}
						channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
						DBManager.getInstance().updateChannel(channel);
					}
					break;
				}
			}
		}
		DBManager.getInstance().deleteSysMail(channel, mailId);
		if (hasDetectMail)
			DBManager.getInstance().getDetectMailInfo();
		if (!isDeleteMuti)
		{
			calulateAllChannelUnreadNum();
			channel.querySysMailCountFromDB();
		}

		if (channel.mailDataList != null && channel.mailDataList.size() == 0 && DBManager.getInstance().hasMailDataInDB(channel.channelID))
		{
			loadMoreSysMailFromDB(channel, -1);
		}
	}

	public void updateSysMailFromChannel(ChatChannel channel, String mailId, UpdateParam updateParam)
	{
		if (channel == null)
			return;
		if (channel.mailDataList != null && channel.mailDataList.size() > 0)
		{
			for (int i = 0; i < channel.mailDataList.size(); i++)
			{
				MailData mailData = channel.mailDataList.get(i);
				if (mailData != null && mailData.getUid().equals(mailId))
				{
					if (mailData.getStatus() != updateParam.getStatus() || mailData.getSave() != updateParam.getSaveFlag()
							|| mailData.getRewardStatus() != updateParam.getRewardStatus())
					{
						if (mailData.getSave() != updateParam.getSaveFlag())
							mailData.setSave(updateParam.getSaveFlag());
						if (mailData.isUnread() && updateParam.getStatus() == 1)
						{
							mailData.setStatus(updateParam.getStatus());
							channel.unreadCount--;
						}
						if (mailData.getRewardStatus() == 0 && updateParam.getRewardStatus() == 1)
							mailData.setRewardStatus(1);
						DBManager.getInstance().updateMail(mailData);
						channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
						DBManager.getInstance().updateChannel(channel);
					}
					break;
				}
			}
		}
	}

	public void deleteMailFrom2dx(String mailUid)
	{
		if (StringUtils.isNotEmpty(mailUid))
		{
			Map<String, ChatChannel> mailChannelMap = getAllSysMailChannelMap();
			if (mailChannelMap != null && mailChannelMap.containsKey(mailUid))
			{
				ChatChannel channel = mailChannelMap.get(mailUid);
				if (channel != null)
					ChannelManager.getInstance().deleteSysMailFromChannel(channel, mailUid, false);
			}
		}
	}

	public void dealMailFrom2dx(String mailUid)
	{
		if (StringUtils.isNotEmpty(mailUid))
		{
			Map<String, ChatChannel> mailChannelMap = getAllSysMailChannelMap();
			if (mailChannelMap != null && mailChannelMap.containsKey(mailUid))
			{
				ChatChannel channel = mailChannelMap.get(mailUid);
				if (channel != null && channel.mailDataList != null)
				{
					for (int i = 0; i < channel.mailDataList.size(); i++)
					{
						MailData mailData = channel.mailDataList.get(i);
						if (mailData != null && mailUid.equals(mailData.getUid()))
						{
							if (mailData.getType() == MailManager.MAIL_ALLIANCEAPPLY
									|| mailData.getType() == MailManager.MAIL_ALLIANCEINVITE
									|| mailData.getType() == MailManager.MAIL_INVITE_TELEPORT
									|| mailData.getType() == MailManager.MAIL_ALLIANCE_PACKAGE)
							{
								mailData.setMailDealStatus();
								DBManager.getInstance().updateMail(mailData);
							}
							break;
						}
					}
				}
			}
		}
	}

	public void handleUpdateAndDelete(MailData mail)
	{
		if (mail == null || StringUtils.isEmpty(mail.getUid()))
			return;
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
		if (channel == null)
			return;
		boolean isDeleted = false;
		if (mailDeleteArray != null && mailDeleteArray.contains(mail.getUid()))
		{
			deleteSysMailFromChannel(channel, mail.getUid(), false);
			mailDeleteArray.remove(mail.getUid());
			isDeleted = true;
		}

		if (mailUpdateMap != null)
		{
			if (mailUpdateMap.containsKey(mail.getUid()))
			{
				if (isDeleted)
				{
					mailUpdateMap.remove(mail.getUid());
				}
				else
				{
					UpdateParam update = mailUpdateMap.get(mail.getUid());
					if (update != null)
					{
						ChannelManager.getInstance().updateSysMailFromChannel(channel, mail.getUid(), update);
					}
				}
			}
		}
		calulateAllChannelUnreadNum();
	}

	public void updateMailData(MailUpdateData mailUpdateData)
	{
		mailDeleteArray = mailUpdateData.getDelete();
		List<UpdateParam> updateMailArr = mailUpdateData.getUpdate();

		if (mailDeleteArray != null && mailDeleteArray.size() > 0)
		{
			for (int j = 0; j < mailDeleteArray.size(); j++)
			{
				String mailUid = mailDeleteArray.get(j);
				if (StringUtils.isEmpty(mailUid))
					continue;
				MailData mailData = DBManager.getInstance().getSysMailByID(mailUid);
				if (mailData != null)
				{
					DBManager.getInstance().deleteSysMailFromDB(mailUid);
					ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mailData.getChannelId());
					if (channel != null)
					{
						if (StringUtils.isEmpty(channel.latestId) || channel.latestId.equals(mailUid))
						{
							channel.latestId = channel.getLatestId();
							channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
							DBManager.getInstance().updateChannel(channel);
						}
					}
				}
			}
		}

		if (updateMailArr != null && updateMailArr.size() > 0)
		{
			for (int j = 0; j < updateMailArr.size(); j++)
			{
				UpdateParam updateParam = updateMailArr.get(j);
				String mailUid = updateParam.getUid();
				if (StringUtils.isEmpty(mailUid))
					continue;
				MailData mailData = DBManager.getInstance().getSysMailByID(mailUid);
				if (mailData != null)
				{
					if (mailData.getStatus() != updateParam.getStatus() || mailData.getSave() != updateParam.getSaveFlag()
							|| mailData.getRewardStatus() != updateParam.getRewardStatus())
					{

						if (mailData.getSave() != updateParam.getSaveFlag())
							mailData.setSave(updateParam.getSaveFlag());
						if (mailData.isUnread() && updateParam.getStatus() == 1)
						{
							mailData.setStatus(updateParam.getStatus());
						}
						if (mailData.getRewardStatus() == 0 && updateParam.getRewardStatus() == 1)
							mailData.setRewardStatus(1);
						DBManager.getInstance().updateMail(mailData);
						ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mailData.getChannelId());
						if (channel != null)
						{
							channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
							DBManager.getInstance().updateChannel(channel);
						}

					}
				}
			}
			calulateAllChannelUnreadNum();
		}
	}

	int	calulateAllChannelCount;

	public void calulateAllChannelUnreadNum()
	{
		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "isHandlingChannelInfo", isHandlingChannelInfo);
		if (isHandlingChannelInfo)
			return;
		calulateAllChannelCount++;
		List<ChannelListItem> channelList = null;
		if (ChatServiceController.getMainListFragment() != null
				&& ChatServiceController.getMainListFragment().getMainChannelAdapter() != null
				&& ChatServiceController.getMainListFragment().getMainChannelAdapter().list != null)
		{
			channelList = ChatServiceController.getMainListFragment().getMainChannelAdapter().list;
		}
		else
		{
			System.out.println("calulateAllChannelUnreadNum 2");
			channelList = getAllMailChannel();
		}

		int oldTotalUnreadCount = totalUnreadCount;
		totalUnreadCount = 0;
		for (int i = 0; i < channelList.size(); i++)
		{
			try
			{
				if (channelList.get(i) != null && channelList.get(i) instanceof ChatChannel)
				{
					ChatChannel channel = (ChatChannel) channelList.get(i);
					totalUnreadCount += channel.unreadCount;
				}
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}

		LogUtil.printVariablesWithFuctionName(Log.INFO, LogUtil.TAG_DEBUG, "totalUnreadCount", totalUnreadCount, "oldTotalUnreadCount",
				oldTotalUnreadCount);
		if (oldTotalUnreadCount != totalUnreadCount)
		{
			JniController.getInstance().excuteJNIVoidMethod("postUnreadMailNum", new Object[] { Integer.valueOf(totalUnreadCount) });
		}
	}

	public boolean isNeedCalculateUnreadCount(String channelId)
	{
		if (StringUtils.isNotEmpty(channelId))
		{
			if (ChatServiceController.isNewMailUIEnable)
			{
				if ((channelId.equals(MailManager.CHANNELID_FIGHT) || channelId.equals(MailManager.CHANNELID_ALLIANCE)
						|| channelId.equals(MailManager.CHANNELID_STUDIO) || channelId.equals(MailManager.CHANNELID_RESOURCE)
						|| channelId.equals(MailManager.CHANNELID_MONSTER) || channelId.equals(MailManager.CHANNELID_KNIGHT)
						|| channelId.equals(MailManager.CHANNELID_SYSTEM) || channelId.equals(MailManager.CHANNELID_EVENT)))
					return true;
			}
			else
			{
				if (channelId.equals(MailManager.CHANNELID_RESOURCE) || channelId.equals(MailManager.CHANNELID_STUDIO)
						|| channelId.equals(MailManager.CHANNELID_RESOURCE_HELP) || channelId.equals(MailManager.CHANNELID_FIGHT)
						|| channelId.equals(MailManager.CHANNELID_MONSTER) || channelId.equals(MailManager.CHANNELID_KNIGHT)
						|| channelId.equals(MailManager.CHANNELID_SYSTEM) || channelId.equals(MailManager.CHANNELID_EVENT))
					return true;
			}
		}
		return false;
	}

	public String getModChannelFromUid(String channelId)
	{
		String fromUid = channelId;
		if (StringUtils.isNotEmpty(fromUid) && fromUid.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
		{
			fromUid = fromUid.substring(0, fromUid.indexOf(DBDefinition.CHANNEL_ID_POSTFIX_MOD));
		}
		return fromUid;
	}

	public void serialize()
	{
		try
		{
			ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(getSerializeDataPath()));
			oos.writeObject(this);
			oos.flush();
			oos.close();
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}

	private static String getSerializeDataPath()
	{
		return DBHelper.getDBDirectoryPath(ChatServiceController.hostActivity, true) + "channelManager.dat";
	}

	public static void deserialize()
	{
		ObjectInputStream oin = null;
		try
		{
			oin = new ObjectInputStream(new FileInputStream(getSerializeDataPath()));
		}
		catch (FileNotFoundException e1)
		{
			e1.printStackTrace();
		}
		catch (IOException e1)
		{
			e1.printStackTrace();
		}
		ChannelManager manager = null;
		try
		{
			manager = (ChannelManager) oin.readObject();
			instance = manager;
		}
		catch (ClassNotFoundException e)
		{
			e.printStackTrace();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}

	public boolean needModChannel()
	{
		return (UserManager.getInstance().getCurrentUser().mGmod == 2 || UserManager.getInstance().getCurrentUser().mGmod == 5)
				|| (modChannelList!=null && modChannelList.size() > 0);
	}
}
