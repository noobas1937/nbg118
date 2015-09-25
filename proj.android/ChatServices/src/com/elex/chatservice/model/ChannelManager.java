package com.elex.chatservice.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.lang.StringUtils;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.db.ChatTable;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.model.mail.updatedata.MailUpdateData;
import com.elex.chatservice.model.mail.updatedata.UpdateParam;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.SortUtil;
import com.elex.chatservice.view.ChannelListFragment;

public class ChannelManager
{
	public static final int			LOAD_MORE_COUNT	= 20;
	public static final int			LOAD_ALL_MORE_MIN_COUNT	= 20;
	public static final int			LOAD_ALL_MORE_MAX_COUNT	= 200;

	private static ChannelManager	instance;
	public boolean					isGetingNewMsg	= false;

	private ConcurrentHashMap<String, ChatChannel>	channelMap;
	public ChatChannel				countryChannel;
	public ChatChannel				allianceChannel;

	public boolean					isFetching;
	public static int 				totalUnreadCount=0;
	public Map<String,Integer> parseFrom2dxMap = new HashMap<String, Integer>();
	public Map<String,Boolean> parseLocalFinishMap = new HashMap<String, Boolean>(); 
	
	private List<ChatChannel> modChannelList = null;
	private List<ChatChannel> messageChannelList = null;
	public String latestModChannelMsg = "";
	public String latestMessageChannelMsg = "";
	private ChatChannel modChannel = null;
	private ChatChannel messageChannel = null;
	public boolean isInRootChannelList = false;
	
	public List<String> mailDeleteArray = null;
	public Map<String,UpdateParam> mailUpdateMap = null;
	public static boolean startHandleChannelInfo = false;
	private String latestId_official = "";
	private long latestSysMailModifyTime = 0;
	
	public static String mailUpdateData = "";
	public static ChannelManager getInstance()
	{
		if (instance == null)
		{
			synchronized (ChannelManager.class)
			{
				if(instance == null)
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
	}

	private void init()
	{
		System.out.println("ChannelManager.init()");

//		LogUtil.printStack();
		channelMap = new ConcurrentHashMap<String, ChatChannel>();
		
		// 从数据库初始化频道，只从db读取一次，新增的需要插入进来
		ArrayList<ChatChannel> dbChannels = DBManager.getInstance().getAllChannel();
		for (int i = 0; i < dbChannels.size(); i++)
		{
			ChatChannel dbChannel = dbChannels.get(i);
			if(!ChatServiceController.isNewMailUIEnable || (ChatServiceController.isNewMailUIEnable && dbChannel.channelType!=DBDefinition.CHANNEL_TYPE_OFFICIAL 
					&& !dbChannel.channelID.equals(MailManager.CHANNELID_MOD) && !dbChannel.channelID.equals(MailManager.CHANNELID_MESSAGE)))
			{
				channelMap.put(dbChannel.getChatTable().getChannelName(), dbChannel);
				System.out.println("channelMap.put key:" + dbChannel.getChatTable().getChannelName());
			}
		}
		getCountryChannel();
		getAllianceChannel();
		if(ChatServiceController.isNewMailUIEnable)
			getNewSystemMailChannel();
		latestId_official = DBManager.getInstance().getSystemMailLatestId();
		latestSysMailModifyTime = DBManager.getInstance().getSystemMailLatestModifyTime();
	}
	
	public long getLatestSysMailModifyTime()
	{
		return latestSysMailModifyTime;
	}
	
	private void getNewSystemMailChannel()
	{
		String[] channelIdArray = {
				MailManager.CHANNELID_FIGHT ,
				MailManager.CHANNELID_ALLIANCE ,
				MailManager.CHANNELID_STUDIO ,
				MailManager.CHANNELID_RESOURCE ,
				MailManager.CHANNELID_MONSTER ,
				MailManager.CHANNELID_SYSTEM
				};
		for(int i=0 ;i<channelIdArray.length;i++)
		{
			if(StringUtils.isNotEmpty(channelIdArray[i]))
			{
				System.out.println("getNewSystemMailChannel:"+channelIdArray[i]);
				ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_OFFICIAL,channelIdArray[i]);
				ChatChannel channel = getChannel(chatTable);
				if(channel!=null)
				{
					channel.latestId = channel.getLatestId();
					channel.latestTime = channel.getLatestTime();
				}
			}
		}
	}
	
	public void removeChannelFromMap(String key)
	{
		if(channelMap!=null && channelMap.containsKey(key))
			channelMap.remove(key);
	}
	
	private void initMsgChannel(List<ChatChannel> channelList, String channelId)
	{
		System.out.println("initMsgChannel:"+channelId);
		ChatChannel parentChannel = null;
		if(channelId.equals(MailManager.CHANNELID_MOD))
			parentChannel = modChannel;
		else if(channelId.equals(MailManager.CHANNELID_MESSAGE))
			parentChannel = messageChannel;
		
		if(channelList!=null && channelList.size()>0)
		{
			if(parentChannel == null)
			{
				parentChannel = new ChatChannel();
				parentChannel.channelType = DBDefinition.CHANNEL_TYPE_USER;
				parentChannel.channelID = channelId;
			}
			int latestCreateTime = 0;
			String latestId = "";
			int unreadCount = 0;
			ChatChannel latestChannel = null;
			for(int i=0;i<channelList.size();i++)
			{
				ChatChannel channel = channelList.get(i);
				if(channel!=null)
				{
					if(channel.unreadCount>0)
						unreadCount ++;
					if(channel.latestTime>latestCreateTime)
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
			if(latestChannel!=null && latestChannel.msgList!=null && latestChannel.msgList.size()>0)
			{
				MsgItem mail = latestChannel.msgList.get(latestChannel.msgList.size() - 1);
				if(mail!=null)
				{
					if(channelId.equals(MailManager.CHANNELID_MOD))
						ChannelManager.getInstance().latestModChannelMsg = mail.msg;
					else if(channelId.equals(MailManager.CHANNELID_MESSAGE))
						ChannelManager.getInstance().latestMessageChannelMsg = mail.msg;
				}
			}
		}else if(ChatServiceController.isNewMailUIEnable && parentChannel == null) {
			parentChannel = new ChatChannel();
			parentChannel.channelType = DBDefinition.CHANNEL_TYPE_USER;
			parentChannel.channelID = channelId;
			parentChannel.unreadCount = 0;
		}
		
		if(channelId.equals(MailManager.CHANNELID_MOD))
			modChannel = parentChannel;
		else if(channelId.equals(MailManager.CHANNELID_MESSAGE))
			messageChannel = parentChannel;
	}
	
	public synchronized ArrayList<ChatChannel> getAllMailChannel()
	{
//		System.out.println("getAllMailChannel");
		ArrayList<ChatChannel> channelList=new ArrayList<ChatChannel>();
		if(channelMap!=null)
		{
			Set<String> keySet = channelMap.keySet();
			if(keySet!=null)
			{
				boolean modChannelInit = false;
				boolean messageChannelInit = false;
				modChannelList = new ArrayList<ChatChannel>();
				if(ChatServiceController.isNewMailUIEnable)
					messageChannelList = new ArrayList<ChatChannel>();
				for(String key:keySet)
				{
					System.out.println("getAllMailChannel key:"+key);
					ChatChannel chatChannel=channelMap.get(key);
					if(chatChannel!=null && chatChannel.channelType!=DBDefinition.CHANNEL_TYPE_COUNTRY 
							&& chatChannel.channelType!=DBDefinition.CHANNEL_TYPE_ALLIANCE && !chatChannel.hasNoItemInChannel()) 
					{
						if(chatChannel.isModChannel())
						{
							modChannelList.add(chatChannel);
						}
						else if(chatChannel.isMessageChannel() && ChatServiceController.isNewMailUIEnable)
						{
							messageChannelList.add(chatChannel);
						}
						else
						{
							if(!chatChannel.channelID.equals(MailManager.CHANNELID_MOD) && !chatChannel.channelID.equals(MailManager.CHANNELID_MESSAGE))
								channelList.add(chatChannel);
						}
					}
				}
				initMsgChannel(modChannelList, MailManager.CHANNELID_MOD);
				if(modChannel!=null && !modChannelInit)
				{
					channelList.add(modChannel);
					modChannelInit = true;
				}
				
				if(ChatServiceController.isNewMailUIEnable)
				{
					initMsgChannel(messageChannelList, MailManager.CHANNELID_MESSAGE);
					if(messageChannel!=null && !messageChannelInit)
					{
						channelList.add(messageChannel);
						messageChannelInit = true;
					}
				}
				
			}
		}
		return channelList;
	}
	
	public List<ChatChannel> getAllModChannel()
	{
		return modChannelList;
	}
	
	public List<ChatChannel> getAllMessageChannel()
	{
		return messageChannelList;
	}
	
	public ChatChannel  getModChannel()
	{
		return modChannel;
	}
	
	public ChatChannel  getMessageChannel()
	{
		return messageChannel;
	}
	
	public Map<String,ChatChannel> getAllSysMailChannelMap()
	{
		System.out.println("getAllSysMailChannelMap");
		Map<String,ChatChannel> systemMailMap=new HashMap<String, ChatChannel>();
		if(channelMap!=null)
		{
			Set<String> keySet = channelMap.keySet();
			if(keySet!=null)
			{
				for(String key:keySet)
				{
					System.out.println("getAllSysMailChannelMap key:"+key);
					ChatChannel chatChannel=channelMap.get(key);
					if(chatChannel!=null && chatChannel.channelType==DBDefinition.CHANNEL_TYPE_OFFICIAL &&
							chatChannel.mailDataList!=null && chatChannel.mailDataList.size()>0)
					{
						for(int i=0;i<chatChannel.mailDataList.size();i++)
						{
							MailData mail=chatChannel.mailDataList.get(i);
							if(mail!=null)
							{
								System.out.println("getAllSysMailChannelMap mail.getUid():"+mail.getUid());
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
		System.out.println("getAllSysMailChannel");
		List<ChatChannel> channelList=new ArrayList<ChatChannel>();
		if(channelMap!=null)
		{
			Set<String> keySet = channelMap.keySet();
			if(keySet!=null)
			{
				for(String key:keySet)
				{
//					System.out.println("getAllMailChannel key:"+key);
					ChatChannel chatChannel=channelMap.get(key);
					if(chatChannel!=null && chatChannel.channelType==DBDefinition.CHANNEL_TYPE_OFFICIAL)
					{
						channelList.add(chatChannel);
					}
				}
			}
		}
		return channelList;
	}
	
	// 总是存在
	public ChatChannel getCountryChannel()
	{
//		System.out.println("getCountryChannel() channelID:" + UserManager.getInstance().getCurrentUser().serverId);
		// 被init()调用时，极少情况UserManager.getInstance().getCurrentUser()可能为null
		if(UserManager.getInstance().getCurrentUser() == null) return null;
		
		ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_COUNTRY, Integer.toString(UserManager.getInstance().getCurrentUser().serverId));
		countryChannel = this.getChannel(chatTable);
		return countryChannel;
	}
	
	/**
	 * 可能有多个联盟或国家频道，需要动态取当前对应的频道
	 * @return 可能为null
	 */
	public ChatChannel getAllianceChannel()
	{
//		System.out.println("getAllianceChannel() channelID:" + UserManager.getInstance().getCurrentUser().allianceId);
		if(UserManager.getInstance().isCurrentUserInAlliance()){
			ChatTable chatTable = ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_ALLIANCE, UserManager.getInstance().getCurrentUser().allianceId);
			allianceChannel = this.getChannel(chatTable);
		}else{
			allianceChannel = null;
		}
		return allianceChannel;
	}
	
	private ChatChannel initChannel(int channelType, String channelID)
	{
		System.out.println("initChannel() channelType:" + channelType + " channelID:" + channelID);
		ChatChannel channel = new ChatChannel();
		channel.channelType = channelType;
		if(channelID != null){
			channel.channelID = channelID;
		}
		if(ChatServiceController.isNewMailUIEnable && (channel.channelID.equals(MailManager.CHANNELID_MOD) || channel.channelID.equals(MailManager.CHANNELID_MESSAGE) || channel.channelID.equals(MailManager.CHANNELID_NOTICE) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP)))
			return channel;
		channelMap.put(channel.getChatTable().getChannelName(), channel);
		System.out.println("initChannel channelMap.put key:" + channel.getChatTable().getChannelName());
		
		return channel;
	}
	
	public ChatChannel addDummyChannel(int channelType, String channelID)
	{
		return initChannel(channelType, channelID);
	}
	
	private void removeChannel(String key)
	{
		if(StringUtils.isNotEmpty(key)){
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
		Iterator<String> it = channelMap.keySet().iterator();
		int latestCreateTime_official = 0;
		
		String latestChannelId_official = "";

		int latestCreateTime_user = 0;
		String latestId_user = "";
		String latestChannelId_user = "";
		while (it.hasNext())
		{
			String key = it.next();
			ChatChannel channel = channelMap.get(key);
			
			if(!channel.containCurrentUser()){
				continue;
			}
			if (channel.channelType != DBDefinition.CHANNEL_TYPE_USER && channel.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
			{
				if(StringUtils.isNotEmpty(result))
					result += ",";
				result += channel.channelID + "|" + channel.dbMaxSeqId + "|" + channel.channelType;
				channel.prevDBMaxSeqId = channel.dbMaxSeqId;
					
			}
			else if (channel.channelType == DBDefinition.CHANNEL_TYPE_USER && !channel.channelID.equals(MailManager.CHANNELID_MOD) && !channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
			{
				if (StringUtils.isNotEmpty(channel.latestId) && channel.latestTime > latestCreateTime_user)
				{
					latestCreateTime_user = channel.latestTime;
					latestId_user = channel.latestId;
					latestChannelId_user = channel.channelID;
				}
			}
		}
		
		if(ChatServiceController.isNewMailListEnable)
		{
			if(StringUtils.isNotEmpty(result))
				result += ",";
			
			if (StringUtils.isNotEmpty(latestId_official))
			{
				result += "0|" + latestId_official + "|" + DBDefinition.CHANNEL_TYPE_OFFICIAL;
			}
			else
			{
				result += "0|0|" + DBDefinition.CHANNEL_TYPE_OFFICIAL;
			}
			
			if(StringUtils.isNotEmpty(result))
				result += ",";
			
			if(latestCreateTime_user!=0 && StringUtils.isNotEmpty(latestId_user))
			{
				result += latestChannelId_user + "|" + latestId_user + "|"+DBDefinition.CHANNEL_TYPE_USER;
			}
			else
			{
				result += "0|0|"+DBDefinition.CHANNEL_TYPE_USER;
			}
		}

		System.out.println("=========getChannelInfo(): " + result + "=========");
		return result;
	}

	/**
	 * 收取消息的channel信息返回值处理
	 * 此前消息已收到，缓存和db中的channel都更新了
	 * 格式："channelType|id|firstSeqId|lastSeqId,channelType|id|firstSeqId|lastSeqId"
	 */
	public void handleChannelInfo(String channelInfo)
	{
		System.out.println("=========handleChannelInfo(): " + channelInfo + "=========");
		if(StringUtils.isEmpty(channelInfo)) return;
		startHandleChannelInfo = true;
		String[] channels = channelInfo.split(",");
		List<String> channelIdArray=new ArrayList<String>();
		for (int i = 0; i < channels.length; i++)
		{
			String[] fields = channels[i].split("\\|");
//			if(channelType == DBDefinition.CHANNEL_TYPE_USER || channelType == DBDefinition.CHANNEL_TYPE_OFFICIAL)
//				continue;
			String channelId = fields[1];
			int channelType = Integer.parseInt(fields[0]);
			if(channelId == "null"){
				if(channelType == DBDefinition.CHANNEL_TYPE_COUNTRY){
					channelId = UserManager.getInstance().getCurrentUser().serverId + "";
				}else if(channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE){
					channelId = UserManager.getInstance().getCurrentUser().allianceId;
				}
			}
			ChatTable chatTable = new ChatTable(channelId, channelType);
			System.out.println("channelName:" + chatTable.getChannelName() + " channel:" + channelMap.get(chatTable.getChannelName()) + "channelMap.size:" + channelMap.size());
			ChatChannel channel = ChannelManager.getInstance().getChannel(chatTable);
            if(channel!=null)
            {
            	channel.hasLoadingAllNew = false;
                //			System.out.println(LogUtil.typeToString(channel));
            	if(channelType != DBDefinition.CHANNEL_TYPE_USER && channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
                {
            		channel.serverMaxSeqId = Integer.parseInt(fields[3]);
            		channel.serverMinSeqId = Integer.parseInt(fields[2]);
            		//			System.out.println("channel.msgList.size():" + channel.msgList.size());
					System.out.println("channel.channelType:" + channel.channelType
							+ " serverMaxSeqId:" + channel.serverMaxSeqId
							+ " serverMinSeqId:" + channel.serverMinSeqId
							+ " prevDBMaxSeqId:" + channel.prevDBMaxSeqId
							+ " msgList.size():" + channel.msgList.size());
            		if(channel.prevDBMaxSeqId == channel.serverMaxSeqId && channel.prevDBMaxSeqId > 0 && channel.msgList.size()==0){
            			int dbMinSeqId = (channel.dbMaxSeqId - LOAD_MORE_COUNT + 1) > 0 ? (channel.dbMaxSeqId - LOAD_MORE_COUNT + 1) : 1;
	            		this.loadMoreFromDB(dbMinSeqId, channel.prevDBMaxSeqId, channel);
	            		DBManager.getInstance().updateChannel(channel);
            		}
                }
            	else
            	{
					int minCreateTime = Integer.parseInt(fields[2]);
					System.out.println("handleChannelInfo minCreateTime:" + minCreateTime);
					loadMoreMailFromDB(channel, minCreateTime);
            	}
            	channelIdArray.add(channel.getChatTable().getChannelName());
            }
		}
		
		Set<String> keySet = channelMap.keySet();
		for (String key : keySet)
		{
			if (!key.equals("") && !channelIdArray.contains(key) && channelMap.get(key) != null)
			{
				System.out.println("loadMoreMailDataFromDB key:" + key);
				loadMoreMailFromDB(channelMap.get(key), -1);
			}
		}
		

		if (ChatServiceController.getChatFragment() != null)
		{
			ChatServiceController.getChatFragment().refreshHasMoreData();
		}
		
		calulateAllChannelUnreadNum();
		
		System.out.println("=========handleChannelInfo() end");
	}
	
	public void loadDBData()
	{
		if(startHandleChannelInfo)
			return;
		System.out.println("loadDBData");
		List<String> channelIdArray=new ArrayList<String>();
		if(getCountryChannel()!=null && getCountryChannel().dbMaxSeqId > 0 && getCountryChannel().msgList.size()==0){
			System.out.println("loadDBData 1");
			int dbMinSeqId = (getCountryChannel().dbMaxSeqId - LOAD_MORE_COUNT + 1) > 0 ? (getCountryChannel().dbMaxSeqId - LOAD_MORE_COUNT + 1) : 1;
    		this.loadMoreFromDB(dbMinSeqId, getCountryChannel().dbMaxSeqId, getCountryChannel());
    		channelIdArray.add(getCountryChannel().getChatTable().getChannelName());
		}
		if(getAllianceChannel()!=null && getAllianceChannel().dbMaxSeqId > 0 && getAllianceChannel().msgList.size()==0){
			System.out.println("loadDBData 2");
			int dbMinSeqId = (getAllianceChannel().dbMaxSeqId - LOAD_MORE_COUNT + 1) > 0 ? (getAllianceChannel().dbMaxSeqId - LOAD_MORE_COUNT + 1) : 1;
    		this.loadMoreFromDB(dbMinSeqId, getAllianceChannel().dbMaxSeqId, getAllianceChannel());
    		channelIdArray.add(getAllianceChannel().getChatTable().getChannelName());
		}
		Set<String> keySet = channelMap.keySet();
		for (String key : keySet)
		{
			if (!key.equals("") && !channelIdArray.contains(key) && channelMap.get(key) != null)
			{
				System.out.println("loadMoreMailDataFromDB key:" + key);
				loadMoreMailFromDB(channelMap.get(key), -1);
			}
		}

		if (ChatServiceController.getChatFragment() != null)
		{
			ChatServiceController.getChatFragment().refreshHasMoreData();
		}
		
		calulateAllChannelUnreadNum();
	}

	public void loadMoreFromDB(int minSeqId, int maxSeqId, ChatChannel channel)
	{
		System.out.println("loadMoreFromDB()");
//		LogUtil.printStack();
		if(channel.channelType!=DBDefinition.CHANNEL_TYPE_OFFICIAL || channel.channelType!=DBDefinition.CHANNEL_TYPE_USER)
		{
			List<MsgItem> dbItems = DBManager.getInstance().getChatMsgBySection(channel.getChatTable(), maxSeqId, minSeqId);
			MsgItem[] dbItemsArray = (MsgItem[]) dbItems.toArray(new MsgItem[0]);
			System.out.println("dbItemsArray:" + dbItemsArray.length);
			if (dbItemsArray.length <= 0) return;
			ServiceInterface.handleMessage(dbItemsArray, channel.channelID,channel.customName);
		}
	}
	
	public void loadMoreMailFromDB(ChatChannel channel, int minCreateTime)
	{
		System.out.println("loadMoreMailFromDB()");
		if(channel == null) {
			return;
		}
		if(channel.channelType==DBDefinition.CHANNEL_TYPE_OFFICIAL && ChatServiceController.isNewMailListEnable)
		{
			loadMoreSysMailFromDB(channel, null);
//			List<MailData> dbMailItems=DBManager.getInstance().getSysMailByTime(channel.getChatTable(), minCreateTime);
//			System.out.println("CHANNEL_TYPE_OFFICIAL dbMailItems length:" + dbMailItems.size());
//			if (dbMailItems.size() <= 0) return;
//			for(int i=0;i<dbMailItems.size();i++)
//			{
//				MailData mailData=dbMailItems.get(i);
//				if(mailData!=null)
//				{
//					ServiceInterface.parseMailData(mailData);
//				}
//			}
		}
		else if(channel.channelType==DBDefinition.CHANNEL_TYPE_USER || channel.channelType==DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			List<MsgItem> dbItems = DBManager.getInstance().getUserMailByTime(channel.getChatTable(), minCreateTime, 20);
			if(dbItems!=null)
			{
				MsgItem[] dbItemsArray = (MsgItem[]) dbItems.toArray(new MsgItem[0]);
				System.out.println("loadMoreMailFromDB minCreateTime dbItemsArray:" + dbItemsArray.length);
				if (dbItemsArray.length <= 0) return;
				
				ServiceInterface.handleMessage(dbItemsArray, channel.channelID,channel.customName);
			}
		}
	}
	
	public int getSysMailDBCount(ChatChannel channel)
	{
		List<MailData> dbMailItems=DBManager.getInstance().getSysMailByTime(channel.getChatTable(), -1);
		return dbMailItems.size();
	}
	

	public void loadMoreSysMailFromDB(ChatChannel channel, String mailId)
	{
		ArrayList<ChannelListItem> allItems = new ArrayList<ChannelListItem>();
		allItems.addAll(DBManager.getInstance().getSysMailByTime(channel.getChatTable(), -1));
		if(isNeedCalculateUnreadCount(channel.channelID))
			channel.calculateUnreadCount(allItems);
		System.out.println("loadMoreSysMailFromDB allItems length:" + allItems.size());
		SortUtil.getInstance().refreshListOrder(allItems);
		int index = -1;
		if(mailId != null){
			for (int i = 0; i < allItems.size(); i++)
			{
				MailData item = (MailData) allItems.get(i);
				if(item.getUid().equals(mailId)){
					index = i + 1;
				}
			}
		}else{
			index = 0;
		}
		
		List<ChannelListItem> moreItems = new ArrayList<ChannelListItem>();
		if(index >= 0){
			int end = (index + 20) > allItems.size() ? allItems.size() : (index + 20);
//			System.out.println("loadMoreSysMailFromDB index:" + index + " end:" + end);
			moreItems = allItems.subList(index, end);
		}
		
		System.out.println("loadMoreSysMailFromDB moreItems length:" + moreItems.size());
//		for (int i = 0; i < moreItems.size(); i++)
//		{
//			MailData item = (MailData) moreItems.get(i);
//			System.out.println("    " + i + ":" + item.getUid() + " " + item.getCreateTime());
//		}
		if (moreItems.size() <= 0){
			if(ChatServiceController.getSysMailListFragment() != null)
			{
				ChatServiceController.getSysMailListFragment().onLoadMoreComplete();
			}
			return;
		}
		
		final List<ChannelListItem> fMoreItems = moreItems;
		Runnable run = new Runnable()
		{
			@Override
			public void run()
			{
				for (int i = 0; i < fMoreItems.size(); i++)
				{
					final MailData mailData = (MailData) fMoreItems.get(i);
					if (mailData != null)
					{
						ServiceInterface.parseMailData(mailData, true);
					}
				}
				if(ChatServiceController.getSysMailListFragment() != null)
				{
					ChatServiceController.getSysMailListFragment().onLoadMoreComplete();
				}
			}
		};
		Thread thread = new Thread(run);
		thread.start();
	} 

	/**
	 * 现在加载的maxId是视图中最小seqId，会被手动加载历史消息改变，而minId是maxId-200，可能需要变为初始化时的serverMaxId
	 */
	public void loadAllNew(ChatChannel channel)
	{
		System.out.println("loadAllNew() channel: " + channel.getChatTable().getChannelName() + " minId:"
				+ channel.getNewMsgMinSeqId() + " maxId:" + channel.getNewMsgMaxSeqId());
		int minSeqId = channel.getNewMsgCount() < LOAD_ALL_MORE_MAX_COUNT ? channel.getNewMsgMinSeqId()
															: (channel.getNewMsgMaxSeqId() - LOAD_ALL_MORE_MAX_COUNT + 1);
		channel.firstNewMsgSeqId = minSeqId;
		ChatServiceController.getInstance().host.getMsgBySeqId(	minSeqId,
																channel.getNewMsgMaxSeqId(),
																channel.channelType,
																channel.channelID);
	}

	public static int channelType2tab(int channelType)
	{
		if(channelType<=DBDefinition.CHANNEL_TYPE_ALLIANCE)
			return channelType;
		else if(channelType==DBDefinition.CHANNEL_TYPE_CHATROOM || channelType==DBDefinition.CHANNEL_TYPE_USER)
			return 2;
		return 0;
	}

	/**
	 * 仅当视图存在时才返回，找不到不会创建
	 */
	public ChatChannel getChannelByViewIndex(int index)
	{
//		System.out.println("getChannelByViewIndex: index = " + index + " chatChannel = " + ChannelManager.getInstance().getChannelView(index).chatChannel);
		ChatChannel result = null;
		if(ChatServiceController.getChatFragment() != null){
			result = ChatServiceController.getChatFragment().getChannelView(index).chatChannel;
		}
		if(result == null){
//			System.out.println("getChannelByViewIndex null: index = " + index + " chatChannel = " + ChannelManager.getInstance().getChannelView(index).chatChannel);
//			LogUtil.printStack();
		}
		return result;
	}

	/**
	 * 用于精确知道channelType和channelId的情况
	 * 如果找不到会创建
	 */
	public ChatChannel getChannel(ChatTable chatTable)
	{
		ChatChannel channel = channelMap.get(chatTable.getChannelName());
		// 仅对邮件、聊天室才可能不存在
		// TODO 不应该有不存在的情况，因为有初始化?
		if (channel == null)
		{
			System.out.println("getChannel null: key = " + chatTable.getChannelName());
			channel = DBManager.getInstance().getChannel(chatTable);
			if(channel == null){
				System.out.println("getChannel channel == null");
				if(chatTable.channelType == DBDefinition.CHANNEL_TYPE_COUNTRY){
					channel = initChannel(DBDefinition.CHANNEL_TYPE_COUNTRY, Integer.toString(UserManager.getInstance().getCurrentUser().serverId));
				}
				else if(chatTable.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE){
					// 没加入联盟的话，获取不到ChatChannel
					if(UserManager.getInstance().isCurrentUserInAlliance())
						channel = initChannel(DBDefinition.CHANNEL_TYPE_ALLIANCE, UserManager.getInstance().getCurrentUser().allianceId);
				}
				else{
					channel = initChannel(chatTable.channelType, chatTable.channelID);
					ChannelListFragment.onChannelAdd();
				}
				if(channel != null){
					channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
					DBManager.getInstance().insertChannel(channel);
				}
			}else{
				System.out.println("getChannel channel != null");
				if(ChatServiceController.isNewMailUIEnable && (channel.channelID.equals(MailManager.CHANNELID_NOTICE) || channel.channelID.equals(MailManager.CHANNELID_RESOURCE_HELP)))
					return channel;
				System.out.println("getChannel channel != null "+chatTable.getChannelName());
				channelMap.put(chatTable.getChannelName(), channel);
			}
		}
		return channel;
	}

	public ChatChannel getChannel(int channelType)
	{
		String channelId="";
		if(channelType==DBDefinition.CHANNEL_TYPE_COUNTRY){
			channelId=UserManager.getInstance().getCurrentUser().serverId + "";
		}
		else if(channelType==DBDefinition.CHANNEL_TYPE_ALLIANCE){
			channelId=UserManager.getInstance().getCurrentUser().allianceId;
		}
		else{
			channelId=UserManager.getInstance().getCurrentMail().opponentUid;
			if(ChatServiceController.isContactMod && !UserManager.getInstance().getCurrentMail().opponentUid.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
				channelId+=DBDefinition.CHANNEL_ID_POSTFIX_MOD;
		}

		System.out.println("getChannel channelType:" + channelType + " channelId:" + channelId);
		return ChannelManager.getInstance().getChannel(channelType, channelId);
	}
	
	
	/**
	 * 用于fromUid(channelId)不一定存在的情况
	 * @return null(如果channelType不为国家或联盟，且channelId为空时)
	 */
	public ChatChannel getChannel(int channelType, String fromUid)
	{
//		System.out.println("getChannel channelType:"+channelType+" fromUid:"+fromUid);

		ChatChannel chatChannel = null;
		if(channelType == DBDefinition.CHANNEL_TYPE_COUNTRY){
			chatChannel = getCountryChannel();
		}else if(channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE){
			chatChannel = getAllianceChannel();
		}else if(!StringUtils.isEmpty(fromUid)){
			ChatTable chatTable = ChatTable.createChatTable(channelType, fromUid);
			chatChannel = getChannel(chatTable);
		}else{
			LogUtil.trackMessage("ChatChannel.getChannel return null, channelType="+channelType+" fromUid="+fromUid, "", "");
		}

		return chatChannel;
	}
	
	// 针对聊天室
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

	public void setChannelMemberArray(String groupId, String uidStr,String roomName)
	{
		System.out.println("setChannelMemberArray groupId:"+groupId);
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if(!uidStr.equals(""))
		{
			channel.memberUidArray.clear();
			String[] uidArr = uidStr.split("_");
			for (int i = 0; i < uidArr.length; i++)
			{
				System.out.println("setChannelMemberArray uidArr[i]:" + uidArr[i]);
				if (!uidArr[i].equals("")) channel.memberUidArray.add(uidArr[i]);
			}
			channel.isMemberUidChanged = true;
		}
		if(!roomName.equals(""))
			channel.customName = roomName;
		DBManager.getInstance().updateChannel(channel);
	}
	
	public void updateChannelMemberArray(String groupId, String uidStr,boolean isAdd)
	{
		if (uidStr.equals("")) return;
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if(channel.memberUidArray==null)
		{
			if(isAdd)
				channel.memberUidArray=new ArrayList<String>();
			else
				return;
		}
		System.out.println("updateChannelMemberArray groupId:"+groupId + "isAdd:"+isAdd);
		String[] uidArr = uidStr.split("_");
		for (int i = 0; i < uidArr.length; i++)
		{
			System.out.println("updateChannelMemberArray uidArr[i]:" + uidArr[i]);
			if (!uidArr[i].equals("")){
				if(isAdd && !channel.memberUidArray.contains(uidArr[i])){
					channel.memberUidArray.add(uidArr[i]);
					channel.isMemberUidChanged = true;
				}else if(!isAdd && channel.memberUidArray.contains(uidArr[i])){
					channel.memberUidArray.remove(uidArr[i]);
					channel.isMemberUidChanged = true;
				}
			}
		}
		DBManager.getInstance().updateChannel(channel);
	}
	
	public void setChatRoomFounder(String groupId,String founderUid)
	{
		if (founderUid.equals("")) return;
		ChatChannel channel = getChannel(ChatTable.createChatTable(DBDefinition.CHANNEL_TYPE_CHATROOM, groupId));
		if(channel!=null)
			channel.roomOwner=founderUid;
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

	// 针对视图
	public ArrayList<MsgItem> getCurMsgListByIndex(int index)
	{
		return getChannelByViewIndex(index).msgList;
	}

//	public ArrayList<SendingMsgItem> getSendingMsgListByIndex(int index)
//	{
//		return getChannelByViewIndex(index).sendingMsgList;
//	}

	public void setNoMoreDataFlag(int index, boolean flag)
	{
		if(getChannelByViewIndex(index) != null){
			getChannelByViewIndex(index).noMoreDataFlag = flag;
		}
	}

	public boolean getNoMoreDataFlag(int index)
	{
		if(getChannelByViewIndex(index) == null) return false;
		
		return getChannelByViewIndex(index).noMoreDataFlag;
	}

	// 针对mail
	
	// 以前国家或联盟只取一个数据，需要这个flag，现在不需要了
	// 只需针对mail，请求过的话，就不用再传邮件数据
	public void setHasRequestDataBeforeFlag(int channelType, String fromUid, boolean flag)
	{
		if (channelType != DBDefinition.CHANNEL_TYPE_USER || (channelType == DBDefinition.CHANNEL_TYPE_USER && fromUid.equals("")))
		{
			return;
		}
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_USER, fromUid);
		if(channel != null){
			channel.hasRequestDataBefore = flag;
		}
	}

	public boolean getHasRequestMailDataBefore(String fromUid)
	{
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_USER, fromUid);
		if(channel != null){
			return channel.hasRequestDataBefore;
		}else{
			return false;
		}
	}
	
	public void removeAllMailMsgByUid(String fromUid)
	{
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_USER, fromUid);
		if(channel != null){
			channel.msgList.clear();
			setHasRequestDataBeforeFlag(DBDefinition.CHANNEL_TYPE_USER, fromUid, false);
		}
		
	}
	
//	public void clearAllMailData()
//	{
//		if(channelMap!=null)
//		{
//			Set<String> keySet=channelMap.keySet();
//			for(String key:keySet)
//			{
//				if(channelMap.get(key).channelType == DBDefinition.CHANNEL_TYPE_USER)
//				{
//					channelMap.get(key).resetChannelMsgData();
//				}
//			}
//		}
//	}
	
	public void deleteChatroomChannel(ChatTable chatTable)
	{
		removeChannel(chatTable.getChannelName());
		
		DBManager.getInstance().deleteChannel(chatTable);
	}
	

	public void deleteChannel(ChatChannel channel)
	{
		if(channel==null || channel.channelType==DBDefinition.CHANNEL_TYPE_COUNTRY || channel.channelType==DBDefinition.CHANNEL_TYPE_ALLIANCE)
			return;
		if(!ChatServiceController.isNewMailUIEnable || 
				(!ChatServiceController.isNewMailUIEnable && 
						(channel.channelType==DBDefinition.CHANNEL_TYPE_USER && !channel.channelID.equals(MailManager.CHANNELID_MESSAGE) && !channel.channelID.equals(MailManager.CHANNELID_MOD)) 
						|| channel.channelType==DBDefinition.CHANNEL_TYPE_CHATROOM))
		{
			ChannelManager.getInstance().removeChannelFromMap(channel.getChatTable().getChannelName());
		}
		
		if(channel.channelType==DBDefinition.CHANNEL_TYPE_USER || channel.channelType==DBDefinition.CHANNEL_TYPE_CHATROOM)
		{
			if(channel.channelID.equals(MailManager.CHANNELID_MOD))
			{
				List<ChatChannel> modChannelArr = getAllModChannel();
				if(modChannelArr!=null && modChannelArr.size()>0)
				{
					for(int i=0; i<modChannelArr.size();i++)
					{
						ChatChannel modChannel = modChannelArr.get(i);
						if(modChannel!=null)
						{
							DBManager.getInstance().deleteChannel(modChannel.getChatTable());
						}
					}
				}
			}
			else if(channel.channelID.equals(MailManager.CHANNELID_MESSAGE))
			{
				List<ChatChannel> messageChannelArr = getAllMessageChannel();
				if(messageChannelArr!=null && messageChannelArr.size()>0)
				{
					for(int i=0; i<messageChannelArr.size();i++)
					{
						ChatChannel messageChannel = messageChannelArr.get(i);
						if(messageChannel!=null)
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
		else if(channel.channelType==DBDefinition.CHANNEL_TYPE_OFFICIAL)
		{
			DBManager.getInstance().deleteSysMailChannel(channel.getChatTable());
		}
		calulateAllChannelUnreadNum();
	}
	
	public void deleteSysMailFromChannel(ChatChannel channel,String mailId)
	{
		if(channel==null)
			return;
		if(channel.mailDataList!=null && channel.mailDataList.size()>0)
		{
			for(int i=0;i<channel.mailDataList.size();i++)
			{
				MailData mailData=channel.mailDataList.get(i);
				if(mailData!=null && mailData.getUid().equals(mailId))
				{
					System.out.println("deleteSysMailFromChannel mailId:"+mailId);
					channel.mailDataList.remove(mailData);
					if(channel.mailDataList.size()==0)
						deleteChannel(channel);
					else
					{
						if(mailData.isUnread())
						{
							channel.unreadCount--;
							ChannelListFragment.onChannelRefresh();
						}
						channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
						DBManager.getInstance().updateChannel(channel);
					}
					break;
				}
			}
		}
		DBManager.getInstance().deleteSysMail(channel, mailId);
		calulateAllChannelUnreadNum();
	}
	
	public void updateSysMailFromChannel(ChatChannel channel,String mailId,UpdateParam updateParam)
	{
		if(channel==null)
			return;
		if(channel.mailDataList!=null && channel.mailDataList.size()>0)
		{
			System.out.println("updateSysMailFromChannel mailDataList ok");
			for(int i=0;i<channel.mailDataList.size();i++)
			{
				MailData mailData=channel.mailDataList.get(i);
				if(mailData!=null && mailData.getUid().equals(mailId))
				{
					System.out.println("updateSysMailFromChannel mailId:"+mailId+ " title:"+mailData.getTitle());
					System.out.println("mailData.getStatus():"+mailData.getStatus()+ " updateParam.getStatus():"+updateParam.getStatus()+
							"mailData.getSave():"+mailData.getSave()+ " updateParam.getSaveFlag():"+updateParam.getSaveFlag()+
							"mailData.getRewardStatus():"+mailData.getRewardStatus()+ " updateParam.getRewardStatus():"+updateParam.getRewardStatus());
					if(mailData.getStatus()!=updateParam.getStatus() || mailData.getSave()!=updateParam.getSaveFlag() 
							|| mailData.getRewardStatus()!=updateParam.getRewardStatus())
					{
						if(mailData.getSave()!=updateParam.getSaveFlag())
							mailData.setSave(updateParam.getSaveFlag());
						if(mailData.isUnread() && updateParam.getStatus()==1)
						{
							mailData.setStatus(updateParam.getStatus());
							channel.unreadCount--;
						}
						if(mailData.getRewardStatus()==0 && updateParam.getRewardStatus()==1)
							mailData.setRewardStatus(1);
						DBManager.getInstance().updateMail(mailData);
						channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
						DBManager.getInstance().updateChannel(channel);
					}
					break;
				}
			}
		}
	}
	
	public void deleteMailFrom2dx(String mailUid)
	{
		if(StringUtils.isNotEmpty(mailUid))
		{
			Map<String,ChatChannel> mailChannelMap=getAllSysMailChannelMap();
			if(mailChannelMap!=null && mailChannelMap.containsKey(mailUid))
			{
				ChatChannel channel=mailChannelMap.get(mailUid);
				if(channel!=null)
					ChannelManager.getInstance().deleteSysMailFromChannel(channel,mailUid);
			}
		}
	}
	
	public void dealMailFrom2dx(String mailUid)
	{
		if(StringUtils.isNotEmpty(mailUid))
		{
			Map<String,ChatChannel> mailChannelMap=getAllSysMailChannelMap();
			if(mailChannelMap!=null && mailChannelMap.containsKey(mailUid))
			{
				ChatChannel channel=mailChannelMap.get(mailUid);
				if(channel!=null && channel.mailDataList!=null)
				{
					for(int i=0;i<channel.mailDataList.size();i++)
					{
						MailData mailData=channel.mailDataList.get(i);
						if(mailData!=null && mailUid.equals(mailData.getUid()))
						{
							if(mailData.getType() == MailManager.MAIL_ALLIANCEAPPLY || mailData.getType() == MailManager.MAIL_ALLIANCEINVITE
									|| mailData.getType() == MailManager.MAIL_INVITE_TELEPORT)
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
		if(mail==null || StringUtils.isEmpty(mail.getUid()))
			return;
		ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL, mail.getChannelId());
		if(channel == null)
			return;
		boolean isDeleted = false;
		if(mailDeleteArray!=null && mailDeleteArray.contains(mail.getUid()))
		{
			deleteSysMailFromChannel(channel,mail.getUid());
			mailDeleteArray.remove(mail.getUid());
			isDeleted = true;
		}
		
		if(mailUpdateMap!=null)
		{
			if(mailUpdateMap.containsKey(mail.getUid()))
			{
				if(isDeleted)
				{
					mailUpdateMap.remove(mail.getUid());
				}
				else
				{
					UpdateParam update = mailUpdateMap.get(mail.getUid());
					if(update!=null)
					{
						ChannelManager.getInstance().updateSysMailFromChannel(channel,mail.getUid(),update);
					}
				}
			}
		}
		calulateAllChannelUnreadNum();
	}
	
	
	public void updateMailData(MailUpdateData mailUpdateData)
	{
		mailDeleteArray = mailUpdateData.getDelete();
		List<UpdateParam> updateMailArr=mailUpdateData.getUpdate();
//		
//		mailUpdateMap = new HashMap<String, UpdateParam>();
//		if(updateMailArr!=null && updateMailArr.size()>0)
//		{
//			for(int i=0;i<updateMailArr.size();i++)
//			{
//				UpdateParam update = updateMailArr.get(i);
//				if(update!=null && StringUtils.isNotEmpty(update.getUid()))
//				{
//					mailUpdateMap.put(update.getUid(), update);
//				}
//			}
//		}
		
//		Map<String,ChatChannel> mailChannelMap=getAllSysMailChannelMap();
		
		if(mailDeleteArray!=null && mailDeleteArray.size()>0)
		{
			for(int j=0;j<mailDeleteArray.size();j++)
			{
				String mailUid=mailDeleteArray.get(j);
				if(StringUtils.isEmpty(mailUid))
					continue;
				MailData mailData = DBManager.getInstance().getSysMailByID(mailUid);
				if(mailData!=null)
				{
					DBManager.getInstance().deleteSysMailFromDB(mailUid);
					ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL,mailData.getChannelId());
					if(channel!=null)
					{
						if(StringUtils.isEmpty(channel.latestId) || channel.latestId.equals(mailUid))
						{
							channel.latestId = channel.getLatestId();
							channel.latestModifyTime = TimeManager.getInstance().getCurrentTimeMS();
							DBManager.getInstance().updateChannel(channel);
						}
					}
				}
//				ChatChannel channel=mailChannelMap.get(mailUid);
//				if(channel!=null && !mailUid.equals(""))
//					ChannelManager.getInstance().deleteSysMailFromChannel(channel,mailUid);
			}
		}
		
		if(updateMailArr!=null && updateMailArr.size()>0)
		{
			for(int j=0;j<updateMailArr.size();j++)
			{
				UpdateParam updateParam=updateMailArr.get(j);
				String mailUid=updateParam.getUid();
				if(StringUtils.isEmpty(mailUid))
					continue;
				MailData mailData = DBManager.getInstance().getSysMailByID(mailUid);
				if(mailData!=null)
				{
					if(mailData.getStatus()!=updateParam.getStatus() || mailData.getSave()!=updateParam.getSaveFlag() 
							|| mailData.getRewardStatus()!=updateParam.getRewardStatus())
					{
						
						if(mailData.getSave()!=updateParam.getSaveFlag())
							mailData.setSave(updateParam.getSaveFlag());
						if(mailData.isUnread() && updateParam.getStatus()==1)
						{
							mailData.setStatus(updateParam.getStatus());
						}
						if(mailData.getRewardStatus()==0 && updateParam.getRewardStatus()==1)
							mailData.setRewardStatus(1);
						DBManager.getInstance().updateMail(mailData);
						ChatChannel channel = getChannel(DBDefinition.CHANNEL_TYPE_OFFICIAL,mailData.getChannelId());
						if(channel!=null)
						{
							channel.latestModifyTime=TimeManager.getInstance().getCurrentTimeMS();
							DBManager.getInstance().updateChannel(channel);
						}
						
					}
				}
////				System.out.println("updateMailData update mail:"+mailUid);
//				ChatChannel channel=mailChannelMap.get(mailUid);
////				if(channel==null) System.out.println("updateMailData channel==null");
//				if(channel!=null && !mailUid.equals(""))
//				{
//					ChannelManager.getInstance().updateSysMailFromChannel(channel,mailUid,updateParam);
//				}
			}
			calulateAllChannelUnreadNum();
		}
	}
	
	public void calulateAllChannelUnreadNum()
	{
		List<ChatChannel> channelList=getAllMailChannel();
		int oldTotalUnreadCount=totalUnreadCount;
		totalUnreadCount=0;
		for(int i=0;i<channelList.size();i++)
		{
			ChatChannel channel=channelList.get(i);
			if(channel!=null)
				totalUnreadCount+=channel.unreadCount;
		}
		
		if(oldTotalUnreadCount!=totalUnreadCount)
			ChatServiceController.getInstance().host.postUnreadMailNum(totalUnreadCount);
	}
	
	
	public boolean isNeedCalculateUnreadCount(String channelId)
	{
		if(StringUtils.isNotEmpty(channelId))
		{
			if(ChatServiceController.isNewMailUIEnable)
			{
				if((channelId.equals(MailManager.CHANNELID_FIGHT) || 
						channelId.equals(MailManager.CHANNELID_ALLIANCE) || 
						channelId.equals(MailManager.CHANNELID_STUDIO) || 
						channelId.equals(MailManager.CHANNELID_RESOURCE) ||
						channelId.equals(MailManager.CHANNELID_MONSTER) ||
						channelId.equals(MailManager.CHANNELID_SYSTEM)))
					return true;
			}
			else
			{
				if(channelId.equals(MailManager.CHANNELID_RESOURCE) || 
						channelId.equals(MailManager.CHANNELID_STUDIO) ||
						channelId.equals(MailManager.CHANNELID_RESOURCE_HELP) || 
						channelId.equals(MailManager.CHANNELID_FIGHT) ||
						channelId.equals(MailManager.CHANNELID_MONSTER) || 
						channelId.equals(MailManager.CHANNELID_SYSTEM))
					return true;
			}
		}
		return false;
	}
	
	public String getModChannelFromUid(String channelId)
	{
		String fromUid = channelId;
		if(StringUtils.isNotEmpty(fromUid) && fromUid.endsWith(DBDefinition.CHANNEL_ID_POSTFIX_MOD))
		{
			fromUid = fromUid.substring(0, fromUid.indexOf(DBDefinition.CHANNEL_ID_POSTFIX_MOD));
		}
		return fromUid;
	}
}
