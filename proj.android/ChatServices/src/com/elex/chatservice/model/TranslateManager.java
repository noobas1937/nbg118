package com.elex.chatservice.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.TranslateParam;
import com.elex.chatservice.util.TranslateUtil;
import com.elex.chatservice.util.TranslatedByLuaResult;

public class TranslateManager {
	
	private static TranslateManager _instance=null;
	private List<MsgItem> translateQueue=null;
	private Map<String,List<MsgItem>> translateQueueLua=null;
	private Timer timer=null;
	private TimerTask timerTask=null;
	private long tranlateStartTime=0;
	private boolean isTranlating=false;
	public String disableLang="";
	public static boolean isTranslatedByLuaStart = false;
	public static boolean isUIShow = false;
	
	public static TranslateManager getInstance()
	{
		if(_instance == null)
			_instance=new TranslateManager();
		return _instance;
	}
	
	private TranslateManager()
	{
		reset();
	}
	
	public void reset()
	{
		stopTimer();
		translateQueue = new ArrayList<MsgItem>();
		translateQueueLua = new HashMap<String, List<MsgItem>>();
	}
	
	private boolean isNeedTranslateChar(String str)
	{
		if(StringUtils.isNumeric(str))
			return false;
		char[] chars = str.toCharArray();
		if(chars.length==1)
		{
//			System.out.println("isNeedTranslate str:"+str);
			int code=(int)chars[0];
			if(code>=0 && code<=255)
				return false;
		}
		return true;
	}
	
	private boolean isTranslateMsgValid(MsgItem msgItem)
	{
		if(msgItem.translateMsg.equals("") || msgItem.translatedLang==null || 
				(!msgItem.translateMsg.equals("") && msgItem.translatedLang!=null && !msgItem.translatedLang.equals("") 
				&& !msgItem.translatedLang.equals(ConfigManager.getInstance().gameLang)))
			return false;
		return true;
	}
	
	private boolean isOriginalLangValid(MsgItem msgItem)
	{
		if(msgItem.originalLang.equals(ConfigManager.getInstance().gameLang) && msgItem.translateMsg.equals(""))
			return true;
		return false;
	}
	
	public void enterTranlateQueue(MsgItem msgItem)
	{
		if(ConfigManager.autoTranlateMode <= 0)
			return;
		try {
			if(msgItem!=null && !msgItem.isSelfMsg && !msgItem.isNewMsg && !msgItem.isEquipMessage() && !msgItem.msg.equals("") && isNeedTranslateChar(msgItem.msg) 
					&& !isOriginalLangValid(msgItem) && !isTranslateMsgValid(msgItem))
			{
//				System.out.println("历史消息无翻译或译文语言和游戏语言不一致");
				if(translateQueue!=null)
					translateQueue.add(msgItem);
				if(translateQueueLua!=null)
				{
					List<MsgItem> list = null;
					if(translateQueueLua.containsKey(msgItem.msg))
						list = translateQueueLua.get(msgItem.msg);
					if(list == null)
						list = new ArrayList<MsgItem>();
					list.add(msgItem);
					translateQueueLua.put(msgItem.msg, list);
				}
				
				if(timer==null)
					createTimer();
			}
		} catch (Exception e) {
			LogUtil.printException(e);
		}
	}
	
	private boolean canTranslateByLua()
	{
		if(isTranslatedByLuaStart && isUIShow)
			return true;
//		System.out.println("canTranslateByLua");
		isTranslatedByLuaStart = ChatServiceController.getInstance().host.canTransalteByLua();
		return isTranslatedByLuaStart;
	}
	
	public void handleTranslateResult(TranslatedByLuaResult result)
	{
		List<MsgItem> list = translateQueueLua.get(result.getOriginalMsg());
		if(list!=null && list.size()>0)
		{
			for(int i=0; i<list.size(); i++)
			{
				MsgItem msgItem = list.get(i);
				if(msgItem != null)
				{
					if(!msgItem.isTranlateDisable() && !msgItem.isOriginalSameAsTargetLang())
						msgItem.hasTranslated = true;
					else
						msgItem.hasTranslated = false;
					msgItem.translateMsg = result.getTranslatedMsg();
					msgItem.originalLang = result.getOriginalLang();
					ChatServiceController.getInstance().host.callXCApi();
					msgItem.translatedLang=ConfigManager.getInstance().gameLang;
//					System.out.println("lua 翻译消息原文："+msgItem.msg+" 译文:"+msgItem.translateMsg+" msgItem.channelType:"+msgItem.channelType);
					
					ChatChannel channel = null;
					if((msgItem.channelType == DBDefinition.CHANNEL_TYPE_USER || msgItem.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM) && msgItem.chatChannel!=null)
					{
						channel = ChannelManager.getInstance().getChannel(msgItem.channelType,msgItem.chatChannel.channelID);
					}
					else if(msgItem.channelType ==DBDefinition.CHANNEL_TYPE_COUNTRY || msgItem.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
					{
						channel = ChannelManager.getInstance().getChannel(msgItem.channelType);
					}
					if(channel != null){
						DBManager.getInstance().updateMessage(msgItem, channel.getChatTable());
					}
				}
			}
			translateQueueLua.remove(result.getOriginalMsg());
			isTranlating=false;
		}
	}
	
	public boolean isInTranslateQueue(String msg)
	{
		if(translateQueueLua!=null && translateQueueLua.containsKey(msg))
			return true;
		return false;
	}
	
	private void createTimer()
	{
//		System.out.println("createTimer");
		if(ConfigManager.autoTranlateMode <= 0)
			return;
		timer=new Timer();
		timerTask=new TimerTask() {
			
			@Override
			public void run() {
				if(System.currentTimeMillis()-tranlateStartTime>=5000 || !isTranlating)
				{
					if(ConfigManager.autoTranlateMode == 2 && canTranslateByLua() && !translateQueueLua.isEmpty())
					{
						
						Set<String> msgKeySet = translateQueueLua.keySet();
						if(msgKeySet.size()>0)
						{
							String msg = msgKeySet.toArray()[0].toString();
							if(StringUtils.isNotEmpty(msg))
							{
//								System.out.println("lua traslate start msg:"+msg);
								
								if(System.currentTimeMillis()-tranlateStartTime>=5000 && isTranlating)
								{
									translateQueueLua.remove(msg);
									isTranlating = false;
								}
								else
								{
									tranlateStartTime=System.currentTimeMillis();
									isTranlating=true;
									ChatServiceController.getInstance().host.translateMsgByLua(msg, ConfigManager.getInstance().gameLang);
								}
							}
						}
						if(translateQueueLua.isEmpty())
							stopTimer();
					}
					else if(ConfigManager.autoTranlateMode == 1 && !translateQueue.isEmpty())
					{
						final MsgItem msgItem=translateQueue.get(0);
						if(msgItem!=null)
						{
//							System.out.println("开始翻译:"+msgItem.msg);
							tranlateStartTime=System.currentTimeMillis();
							isTranlating=true;
							try {
								String ret= TranslateUtil.translate(msgItem.msg);
								TranslateParam param=JSON.parseObject(ret,TranslateParam.class);
								String translateMsg=param.getTranslatedMsg();
								if(!translateMsg.trim().equals(""))
								{
									if(!msgItem.isTranlateDisable() && !msgItem.isOriginalSameAsTargetLang())
										msgItem.hasTranslated = true;
									else
										msgItem.hasTranslated = false;
									
									msgItem.translateMsg=translateMsg;
									msgItem.originalLang=param.getSrc();
									ChatServiceController.getInstance().host.callXCApi();
									msgItem.translatedLang=ConfigManager.getInstance().gameLang;
//								System.out.println("google 翻译消息原文："+msgItem.msg+" 译文:"+msgItem.translateMsg+" msgItem.channelType:"+msgItem.channelType);
									
									ChatChannel channel = null;
									if((msgItem.channelType == DBDefinition.CHANNEL_TYPE_USER || msgItem.channelType == DBDefinition.CHANNEL_TYPE_CHATROOM) && msgItem.chatChannel!=null)
									{
										channel = ChannelManager.getInstance().getChannel(msgItem.channelType,msgItem.chatChannel.channelID);
									}
									else if(msgItem.channelType ==DBDefinition.CHANNEL_TYPE_COUNTRY || msgItem.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
									{
										channel = ChannelManager.getInstance().getChannel(msgItem.channelType);
									}
									if(channel != null){
//									System.out.println("翻译消息 222");
										DBManager.getInstance().updateMessage(msgItem, channel.getChatTable());
									}
									translateQueue.remove(msgItem);
									isTranlating=false;
								}
							} catch (Exception e) {
								translateQueue.remove(msgItem);
								LogUtil.trackMessage("JSON.parseObject exception on server" + UserManager.getInstance().getCurrentUser().serverId, "", "");
							}
						}
						if(translateQueue.isEmpty())
							stopTimer();
					}
				}
				
			}
		};
		timer.schedule(timerTask, 0, 100);
	}
	
	private void stopTimer()
	{
//		System.out.println("stopTimer");
		if(timerTask!=null)
		{
			timerTask.cancel();
			timerTask=null;
		}
		
		if(timer!=null)
		{
			timer.cancel();
			timer.purge();
			timer=null;
		}
	}
}
