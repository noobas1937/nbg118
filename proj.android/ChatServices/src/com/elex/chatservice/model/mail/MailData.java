package com.elex.chatservice.model.mail;

import org.apache.commons.lang.StringUtils;

import android.content.ContentValues;
import android.database.Cursor;

import com.alibaba.fastjson.annotation.JSONField;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelListItem;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailIconName;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.util.LogUtil;

public class MailData extends ChannelListItem {
	// 由JNI传递过来的邮件属性
	private int						type			= -1;
	private int						createTime;
	private int						status;						// 是否未读，大于1则是
	private int						reply;
	private int						rewardStatus;
	private int						itemIdFlag;					// 1需要读语言文件
	private int						save;						// 0未保存,1保存,2删除保存过
	private String					uid				= "";
	private String					title			= "";
	private String					contents		= "";
	private String					fromName		= "";
	private String					fromUid			= "";
	private String					rewardId		= "";
	
	private transient String		version			= "";
	public transient String			pic				= "";

	// 运行时属性
	public transient int			tabType			= -1;
	public transient boolean		isAtk;
	public transient boolean 		hasParseCompex 	= false;
	public transient boolean 		hasParseBy2dx 	= false;
	public transient boolean 		hasParseLocal 	= false;
	public transient String			language		= "";
	public transient int			parseVersion	= -1;
	
	// 显示属性
	public transient String			nameText		= "";
	public transient String			contentText		= "";
	public transient String			mailIcon		= "";
	public transient String			timeText		= "";
	public transient boolean		usePersonalPic	= false;
	public transient ChatChannel	channel			= null;
	public transient String			channelId		= "";

	private static final int		PARSE_VERSION_BASIS		= 1;
	private static final int		CURRENT_PARSE_VERSION	= PARSE_VERSION_BASIS;
	
	public MailData()
	{
	}

	@JSONField(serialize=false)
	public boolean isParseVersionOld()
	{
		return parseVersion < CURRENT_PARSE_VERSION;
	}
	
	public void updateParseVersion()
	{
		parseVersion = CURRENT_PARSE_VERSION;
	}
	
	public void setMailData(MailData mailData)
	{
		type = mailData.type;
		createTime = mailData.createTime;
		status = mailData.status;
		reply = mailData.reply;
		rewardStatus = mailData.rewardStatus;
		itemIdFlag = mailData.itemIdFlag;
		save = mailData.save;
		uid = mailData.uid;
		title = mailData.title;
		contents = mailData.contents;
		fromName = mailData.fromName;
		fromUid = mailData.fromUid;
		rewardId = mailData.rewardId;
		version = mailData.version;
		channelId = mailData.channelId;
	}
	
	public void setMailDealStatus()
	{
	}
	
	// 用于从数据库获取消息
	public MailData(Cursor c) {
		try
		{
			type = c.getInt(c.getColumnIndex(DBDefinition.MAIL_TYPE));
			createTime = c.getInt(c.getColumnIndex(DBDefinition.MAIL_CREATE_TIME));
			status = c.getInt(c.getColumnIndex(DBDefinition.MAIL_STATUS));
			reply = c.getInt(c.getColumnIndex(DBDefinition.MAIL_REPLY));
			rewardStatus = c.getInt(c.getColumnIndex(DBDefinition.MAIL_REWARD_STATUS));
			itemIdFlag = c.getInt(c.getColumnIndex(DBDefinition.MAIL_ITEM_ID_FLAG));
			save = c.getInt(c.getColumnIndex(DBDefinition.MAIL_SAVE_FLAG));
			uid = c.getString(c.getColumnIndex(DBDefinition.MAIL_ID));
			channelId = c.getString(c.getColumnIndex(DBDefinition.MAIL_CHANNEL_ID));
			title = c.getString(c.getColumnIndex(DBDefinition.MAIL_TITLE));
			contents = c.getString(c.getColumnIndex(DBDefinition.MAIL_CONTENTS));
			fromName = c.getString(c.getColumnIndex(DBDefinition.MAIL_FROM_NAME));
			fromUid = c.getString(c.getColumnIndex(DBDefinition.MAIL_FROM_USER_ID));
			rewardId = c.getString(c.getColumnIndex(DBDefinition.MAIL_REWARD_ID));
			if (type == MailManager.MAIL_SYSUPDATE) version = fromUid;
			nameText = c.getString(c.getColumnIndex(DBDefinition.MAIL_TITLE_TEXT));
			contentText = c.getString(c.getColumnIndex(DBDefinition.MAIL_SUMMARY));
			language = c.getString(c.getColumnIndex(DBDefinition.MAIL_LANGUAGE));
			parseVersion = c.getInt(c.getColumnIndex(DBDefinition.PARSE_VERSION));
			hasParseCompex = isComplexMail() && !needParseContent() && !isParseVersionOld();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	@JSONField(serialize=false)
	public ContentValues getContentValues() {
		ContentValues cv = new ContentValues();
		cv.put(DBDefinition.COLUMN_TABLE_VER, DBHelper.CURRENT_DATABASE_VERSION);
		cv.put(DBDefinition.MAIL_TYPE, type);
		cv.put(DBDefinition.MAIL_CREATE_TIME, createTime);
		cv.put(DBDefinition.MAIL_STATUS, status);
		cv.put(DBDefinition.MAIL_REPLY, reply);
		cv.put(DBDefinition.MAIL_REWARD_STATUS, rewardStatus);
		cv.put(DBDefinition.MAIL_ITEM_ID_FLAG, itemIdFlag);
		cv.put(DBDefinition.MAIL_SAVE_FLAG, save);
		cv.put(DBDefinition.MAIL_ID, uid);
		cv.put(DBDefinition.MAIL_CHANNEL_ID, channelId);
		cv.put(DBDefinition.MAIL_TITLE, title);
		cv.put(DBDefinition.MAIL_CONTENTS, contents);
		cv.put(DBDefinition.MAIL_FROM_NAME, fromName);
		cv.put(DBDefinition.MAIL_FROM_USER_ID, fromUid);
		cv.put(DBDefinition.MAIL_REWARD_ID, rewardId);
		cv.put(DBDefinition.MAIL_TITLE_TEXT, nameText);
		cv.put(DBDefinition.MAIL_SUMMARY, contentText);
		cv.put(DBDefinition.MAIL_LANGUAGE, language);
		cv.put(DBDefinition.PARSE_VERSION, parseVersion);
		
		return cv;
	}
	
	public boolean canDelete()
	{
		boolean ret=true;
		if((!rewardId.equals("") && rewardStatus==0) || save==1)
			ret=false;
		return ret;
	}
	
	public boolean hasNotGetReward()
	{
		boolean ret=true;
		if((!rewardId.equals("") && rewardStatus==0))
			ret=false;
		return ret;
	}
	
	public void parseContents()
	{
		if(type==MailManager.MAIL_SYSUPDATE)
			version=fromUid;
		parseMailTypeTab();
//		System.out.println("parseMailTypeTab");
		parseMailCellIcon();
//		System.out.println("parseMailCellIcon");
		parseMailName();
//		System.out.println("parseName");
		
		timeText = TimeManager.getReadableTime(createTime);
		language = ConfigManager.getInstance().gameLang;
		

		if(!needComplexParse())
		{
			parseContentText();
			hasParseLocal = true;
		}
	}
	
	public void parseMailTypeTab()
	{
		if(type>=0 && type<MailManager.MAIL_TYPE_COUNT)
		{
			if(type==MailManager.MAIL_BATTLE_REPORT||type==MailManager.MAIL_DETECT||
					type==MailManager.MAIL_DETECT_REPORT||type==MailManager.MAIL_ENCAMP||type==MailManager.MAIL_WORLD_BOSS){
		        tabType = MailManager.MAILTAB_FIGHT;
		    }
			else if(type==MailManager.ALL_SERVICE){
		        tabType = MailManager.MAILTAB_STUDIO;
		    }
			else if(type== MailManager.MAIL_FRESHER||type==MailManager.MAIL_SYSNOTICE||type==MailManager.MAIL_SYSUPDATE){
		        tabType = MailManager.MAILTAB_NOTICE;
		    }
			else if(type <=MailManager.MAIL_USER||type==MailManager.MAIL_Alliance_ALL||type==MailManager.CHAT_ROOM){
		        tabType = MailManager.MAILTAB_USER;
		    }
			else if(type == MailManager.MAIL_MOD_SEND || type == MailManager.MAIL_MOD_PERSONAL){
		        if(UserManager.getInstance().getCurrentUser().mGmod  == 2 || UserManager.getInstance().getCurrentUser().mGmod == 5){
		            tabType = MailManager.MAILTAB_MOD;
		        }else{
		            tabType = MailManager.MAILTAB_USER;
		        }
		    }else{
		        tabType = MailManager.MAILTAB_SYSTEM;
		    }
		}
//		System.out.println("parseMailTypeTab type:" + type + " tabType:" + tabType);
	}
	
	private void parseMailCellIcon() {
		switch (type)
		{
			case MailManager.MAIL_SYSTEM:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM);
				break;
			case MailManager.MAIL_BATTLE_REPORT:
			{
				if (isAtk)
				{
					mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_CITY_VICTORY);
				}
				else
				{
					mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY);
				}
				break;
			}
			case MailManager.MAIL_RESOURCE:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.CHANNEL_ICON_RESOURCE);
				break;
			case MailManager.MAIL_DETECT:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_DETECT);
				break;
			case MailManager.MAIL_DETECT_REPORT:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_DETECT);
				break;
			case MailManager.MAIL_ENCAMP:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_CAMP);
				break;
			case MailManager.MAIL_FRESHER:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM);
				break;
			case MailManager.MAIL_WOUNDED:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_DIGONG:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.ALL_SERVICE:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM);
				break;
			case MailManager.WORLD_NEW_EXPLORE:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_EXPLORE);
				break;
			case MailManager.MAIL_SYSNOTICE:
				if(StringUtils.isNotEmpty(title) && title.equals("114020"))
					mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM_VIP);
				else
					mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM);
				break;
			case MailManager.MAIL_SYSUPDATE:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_ANNOUNCEMENT);
				break;
			case MailManager.MAIL_ALLIANCEINVITE:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_ATTACKMONSTER:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.CHANNEL_ICON_MONSTER);
				break;
			case MailManager.WORLD_MONSTER_SPECIAL:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_Alliance_ALL:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_INVITE_TELEPORT:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_ALLIANCE_KICKOUT:
				mailIcon = pic;
				usePersonalPic = true;
				break;
			case MailManager.MAIL_WORLD_BOSS:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_REPORT);
				break;
			case MailManager.CHAT_ROOM:
				mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_CHAT_ROOM);
				break;
			default:
				break;
		}
		
		if(isWorldBossKillRewardMail())
			mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_BATTLE_REPORT);
		if (mailIcon==null || mailIcon.equals("")) {
			mailIcon = MailManager.getInstance().getMailIconByName(MailIconName.MAIL_ICON_SYSTEM);
		}
	}
	
	public void parseMailName()
	{
//		System.out.println("refreshStatus 1");
		nameText=fromName;
		
	    if(type==MailManager.MAIL_RESOURCE_HELP){
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_RESOURCEHELP);
	    }
	    else if(type == MailManager.MAIL_MOD_SEND || type == MailManager.MAIL_MOD_PERSONAL){
	        nameText = "[MOD]" +fromName;
	    }
	    else if(type==MailManager.MAIL_ALLIANCEINVITE){
	        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_INVITE);
	    }
	    else if(type == MailManager.MAIL_ALLIANCE_KICKOUT){
	        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_QUITALLIANCE);
	    }
	    else if(type == MailManager.MAIL_GIFT){
	    	if(UserManager.getInstance().getUser(fromUid)!=null && !UserManager.getInstance().getUser(fromUid).asn.equals("")){
	    		nameText="("+UserManager.getInstance().getUser(fromUid).asn+")"+fromName;
	        }else{
	        	nameText=fromName;
	        }
	    }
	    else if(type == MailManager.MAIL_WORLD_BOSS || isWorldBossKillRewardMail())
	    {
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_WORLDBOSS);
	    }
	    else if(type==MailManager.MAIL_RESOURCE)
	    {
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_NAME_RESOURCE);
	    }
	    else if(type == MailManager.MAIL_FRESHER || type == MailManager.MAIL_SYSTEM){
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.NPC_NAME);
	    }
	    else if(type == MailManager.MAIL_REFUSE_ALL_APPLY){
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_115464);
	    }
	    else if(type == MailManager.MAIL_ATTACKMONSTER)
	    {
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_103715);
	    }
	    else if(type == MailManager.MAIL_DETECT)
		{
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.MENU_DETECTREPORT);
	    	if(itemIdFlag==0 && StringUtils.isNotEmpty(title))
	    	{
	    		if(title.equals("1"))
	    		{
	    			nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105523);
	    		}
	    		else
	    		{
	    			nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105567);
	    		}
	    	}
		}
	    
	    
	    if(nameText==null ||nameText.equals(""))
	    {
	    	if(tabType == MailManager.MAILTAB_FIGHT){
		        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_FIGHT); 
		    }else if(tabType == MailManager.MAILTAB_MOD){
		        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_MOD);
		    }else if(tabType == MailManager.MAILTAB_STUDIO){
		        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_STUDIO);
		    }
		    else if(tabType ==MailManager.MAILTAB_SYSTEM){
		    	nameText = LanguageManager.getLangByKey(LanguageKeys.TIP_SYSTEM);
		    }
		    else if(tabType ==MailManager.MAILTAB_NOTICE){
		        nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_NOTICE);
		    }
	    }
	    if(nameText.equals("3000001") || nameText.equals("3000002"))
	    	nameText = LanguageManager.getLangByKey(LanguageKeys.NPC_NAME);
	   
	}
	
	public void parseContentText()
	{
//		System.out.println("parseTitleText type:"+type+"  title:"+title);
		if(title!=null && !title.equals(""))
			contentText = title.replaceAll("\n", " ");
		
		if(title.equals("105734"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_CASTLEMOVE);
		else if(title.equals("138067"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_BATTLE_TITLE);
		else if(title.equals("114010"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_NEWPLAYER_MOVECASTAL);
		else if(title.equals("115295"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_INVITE_MOVECASTAL);
		else if(title.equals("114012"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_APPLY);
		else if(title.equals("105718"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_ENEMY_KILL);
		else if(title.equals("133017"))
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_REWARD);
		else if(StringUtils.isNumeric(title))
		{
			contentText=LanguageManager.getLangByKey(title);
		}
		
		if(itemIdFlag == 1)
		{
			String contentArr[]=contents.split("\\|");
			int num = contentArr.length;
			
			System.out.println("contents:"+contents+" str size:"+num);
			switch (num)
			{
				case 5:
					if (contentArr[0].equals("115336"))
					{// 领地使用默认名字
						System.out.println("str[3]" + contentArr[3]);
						System.out.println("str[4]" + contentArr[4]);
						String name = LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE, contentArr[4]);
						contentText = LanguageManager.getLangByKey(contentArr[3], name);
					}
					break;
				case 6:
				{
					String name = "";
					if (contentArr[1].equals("") || contentArr[2].equals(""))
					{
						name = LanguageManager.getLangByKey(LanguageKeys.MAIL_ALLIANCE_CASTLE, contentArr[5]);
					}
					else
					{
						name = contentArr[1];
					}
					contentText = LanguageManager.getLangByKey(contentArr[4], name);
				}
				break;
				default:
					break;
			}
		}
		
		
		if(type == MailManager.MAIL_DONATE)
		{
	        String str[]=contents.split("\\|");
	        if(str.length>4)
	        {
	        	String userName=str[1];
	        	if(!str[2].equals(""))
	        	{
	        		userName="("+str[2]+")"+userName;
	        	}
	        	contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_GIFT,userName);
	        }
		}
		else if(type == MailManager.MAIL_DIGONG || type == MailManager.WORLD_NEW_EXPLORE)
		{
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TABNAME_WORLDEXPLORE);
		}
		else if(type == MailManager.MAIL_WOUNDED)
		{
			contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_WOUNDED);
		}
		else if(type == MailManager.MAIL_DETECT)
		{
			if(itemIdFlag==1)
			{
	            if(!contents.equals("") && contents.length()>6)
	            {
	            	String content=contents.substring(6);
	            	if(content.equals("105554"))
	            	{
	            		contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105523);
	            	}
	            }
	            
			}
			else
			{
			    if(title.equals("1"))
			    {
			    	contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105523);
			    }
			    else
			    {
			    	contentText=LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105567);
			    }
			    
			    String content[] = contents.split("\\|");
                int num = content.length;
                System.out.println("contents num:"+num);
                switch (num) {
                    case 1:
                    	contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105524,contents);
                        break;
                    case 3:{
                    	String name = content[0];
                    	String type = content[1];
                        if(type.equals("1")){
                        	contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_137429,name);
                        }else if (type.equals("2")){
                        	contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_137431,name);
                        }else if (type.equals("3")){
                        	contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_137430,name);
                        }else{
                        	contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_105524,name);
                        }
                        break;
                    }
                    default:
                        break;
                }
			    
			}
		}
		else if(type == MailManager.MAIL_GIFT){
	    	contentText = contents;
	    }
		if(contentText.length()>50)
		{
			contentText = contentText.substring(0, 50);
			contentText = contentText + "...";
		}
		
	}
	
	public boolean needParseFrom2dx()
	{
		return (!hasParseBy2dx && isComplexMail());
	}
	
	public boolean needComplexParse()
	{
		return (!hasParseCompex && isComplexMail());
	}
	
	@JSONField(serialize=false)
	public boolean isComplexMail()
	{
		if(type == MailManager.MAIL_ATTACKMONSTER || type == MailManager.MAIL_BATTLE_REPORT 
				|| type == MailManager.MAIL_RESOURCE ||type == MailManager.MAIL_RESOURCE_HELP 
				|| type == MailManager.MAIL_DETECT_REPORT || type == MailManager.MAIL_ENCAMP 
				|| type == MailManager.WORLD_NEW_EXPLORE)
			return true;
		return false;
	}
	
	public boolean needParseContent()
	{
		if(StringUtils.isNotEmpty(nameText) && StringUtils.isNotEmpty(contentText) && !StringUtils.isNumeric(nameText) 
				&& !StringUtils.isNumeric(contentText) && StringUtils.isNotEmpty(language) && 
				language.equals(ConfigManager.getInstance().gameLang))
		{
			return false;
		}
		return true;
	}
	
	@JSONField(serialize = false)
	public String getChannelId()
	{
		if(ChatServiceController.isNewMailUIEnable)
			return getNewChannelId();
		else
			return getOldChannelId();
	}
	
	@JSONField(serialize = false)
	public String getOldChannelId()
	{
		String channelId = "";
		if(tabType<0)
			parseMailTypeTab();
		switch (tabType) {
			case MailManager.MAILTAB_SYSTEM:
				channelId = MailManager.CHANNELID_SYSTEM;
				break;
			case MailManager.MAILTAB_NOTICE:
				channelId = MailManager.CHANNELID_NOTICE;
				break;
			case MailManager.MAILTAB_STUDIO:
				channelId = MailManager.CHANNELID_STUDIO;
				break;
			case MailManager.MAILTAB_FIGHT:
				channelId = MailManager.CHANNELID_FIGHT;
				break;
			case MailManager.MAILTAB_MOD:
				channelId = MailManager.CHANNELID_MOD;
				break;
			default:
				break;
		}
		
		if(type == MailManager.MAIL_RESOURCE)
			channelId = MailManager.CHANNELID_RESOURCE;
		else if(type == MailManager.MAIL_ATTACKMONSTER)
			channelId = MailManager.CHANNELID_MONSTER;
		else if(type == MailManager.MAIL_RESOURCE_HELP)
			channelId = MailManager.CHANNELID_RESOURCE_HELP;
		return channelId;
	}
	
	@JSONField(serialize = false)
	public String getNewChannelId()
	{
		String channelId = "";
		if(type == MailManager.MAIL_SYSTEM)
		{
			if(title.equals("114111") 
					|| title.equals("105726")
					|| title.equals("105727")
					|| title.equals("105728")
					|| title.equals("105729")
					|| title.equals("105730"))
			{
				channelId = MailManager.CHANNELID_ALLIANCE;
			}
			else if(isWorldBossKillRewardMail())
			{
				channelId = MailManager.CHANNELID_FIGHT;
			}
			else
			{
				channelId = MailManager.CHANNELID_SYSTEM;
			}
		}
		else
		{
			switch(type)
			{
				case MailManager.MAIL_BATTLE_REPORT:
				case MailManager.MAIL_DETECT:
				case MailManager.MAIL_DETECT_REPORT:
				case MailManager.MAIL_ENCAMP:
				case MailManager.MAIL_WORLD_BOSS:
				case MailManager.WORLD_NEW_EXPLORE:
				case MailManager.WORLD_MONSTER_SPECIAL:
					channelId = MailManager.CHANNELID_FIGHT;
					break;
				case MailManager.MAIL_ALLIANCEINVITE:
				case MailManager.MAIL_Alliance_ALL:
				case MailManager.MAIL_ALLIANCEAPPLY:
				case MailManager.MAIL_ALLIANCE_KICKOUT:
				case MailManager.MAIL_INVITE_TELEPORT:
				case MailManager.MAIL_REFUSE_ALL_APPLY:
				case MailManager.MAIL_RESOURCE_HELP:
				case MailManager.MAIL_DONATE:
					channelId = MailManager.CHANNELID_ALLIANCE;
					break;
				case MailManager.ALL_SERVICE:
				case MailManager.MAIL_SYSUPDATE:
					channelId = MailManager.CHANNELID_STUDIO;
					break;
				case MailManager.MAIL_GIFT:
				case MailManager.MAIL_SYSNOTICE:
				case MailManager.MAIL_SERVICE:
				case MailManager.MAIL_FRESHER:
				case MailManager.MAIL_WOUNDED:
					channelId = MailManager.CHANNELID_SYSTEM;
					break;
				case MailManager.MAIL_ATTACKMONSTER:
					channelId = MailManager.CHANNELID_MONSTER;
					break;
				case MailManager.MAIL_RESOURCE:
					channelId = MailManager.CHANNELID_RESOURCE;
					break;
			}
		}
		
		return channelId;
	}
	
	@Override
	public String toString() {
		return "[MailData]: uid = "+uid+" type = " + type+" title:"+title;
	}
	
	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getCreateTime() {
		return createTime;
	}

	public void setCreateTime(int createTime) {
		this.createTime = createTime;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}

	@JSONField(serialize=false)
	public boolean isUnread() {
		return status == 0;
	}

	public int getReply() {
		return reply;
	}

	public void setReply(int reply) {
		this.reply = reply;
	}
	
	public boolean hasReward()
	{
		return !rewardId.equals("") && rewardStatus == 0;
	}
	@JSONField (serialize = false)
	public boolean isLock()
	{
		return getSave() == 1;
	}

	public int getRewardStatus() {
		return rewardStatus;
	}

	public void setRewardStatus(int rewardStatus) {
		this.rewardStatus = rewardStatus;
	}

	public int getItemIdFlag() {
		return itemIdFlag;
	}

	public void setItemIdFlag(int itemIdFlag) {
		this.itemIdFlag = itemIdFlag;
	}

	public int getSave() {
		return save;
	}

	public void setSave(int save) {
		this.save = save;
	}

	public String getUid() {
		return uid;
	}

	public void setUid(String uid) {
		this.uid = uid;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getContents() {
		return contents;
	}

	public void setContents(String contents) {
		this.contents = contents;
	}

	public String getFromName() {
		return fromName;
	}

	public void setFromName(String fromName) {
		this.fromName = fromName;
	}

	public String getFromUid() {
		return fromUid;
	}

	public void setFromUid(String fromUid) {
		this.fromUid = fromUid;
	}

	public String getRewardId() {
		return rewardId;
	}

	public void setRewardId(String rewardId) {
		this.rewardId = rewardId;
	}

	@JSONField(serialize = false)
	public String getVersion() {
		return version;
	}

	@JSONField(serialize = false)
	public void setVersion(String version) {
		this.version = version;
	}

	@JSONField(serialize = false)
	public int getChannelTime()
	{
		return getCreateTime();
	}
	
	@JSONField(serialize = false)
	public boolean isWorldBossKillRewardMail()
	{
		if(itemIdFlag == 1 && StringUtils.isNotEmpty(title) && title.equals("137460"))
			return true;
		return false;
	}
}
