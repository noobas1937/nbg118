package com.elex.chatservice.model;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import org.apache.commons.lang.StringUtils;

import android.content.ContentValues;
import android.database.Cursor;
import android.provider.BaseColumns;

import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.view.ChatActivity;

public final class MsgItem
{
	public final static int	SENDING					= 0;
	public final static int	SEND_FAILED				= 1;
	public final static int	SEND_SUCCESS			= 2;

	public final static int	MSG_TYPE_CHATROOM_TIP	= 100;
	public final static int	MSG_TYPE_MOD			= 200;
	public final static int	MAIL_MOD_PERSON			= 23;

	// 数据库对应
	public int				_id;								// 数据库使用的id
	public int				tableVer;
	public int				sequenceId;
	public String			mailId;								// 用来标识邮件的id
	public String			uid						= "";		// uid，群聊时才会存数据库
	public int				channelType				= -1;		// 频道类型
	public int				createTime				= 0;			// 收到的消息会在C++中初始化此字段，对应后台传回来的createTime
	public int				post					= -1;		// 数据库中名为type：是否为系统信息，“0”表示不是，非“0”表示是
	public String			msg						= "";		// 消息体
	public String			translateMsg			= "";		// 翻译信息
	public String			originalLang			= "";		// 源语言
	public String			translatedLang			= "";		// 翻译后的语言
	public int				sendState				= -1;		// 发送状态，0正在发送，1发送失败，2发送成功
	public String			attachmentId			= "";		// 战报UID，侦察战报UID,装备ID
	public String			media					= "";

	// 运行时属性
	public boolean			isSelfMsg;							// 是否是自己的信息
	public boolean			isNewMsg;							// 是否是新消息
	public String			currentText				= "";
	public boolean			hasTranslated;						// 是否被翻译过
	public boolean			isSendDataShowed		= false;
	public int				lastUpdateTime			= 0;
	public int				sendLocalTime			= 0;		// 本地发送时间戳
	public boolean			isTranslateByGoogle		= false;
	public boolean			isFirstNewMsg			= false;
	public int				firstNewMsgState		= 0;		// 0:不是第一条
																// 1:第一条且新消息数小于等于200条
																// 2:第一条且新消息数超过200条
	public ChatChannel		chatChannel				= null;		// msgItem所属的Channel
	public boolean			isTranslatedByForce		= false;	// 是否强制翻译，点击翻译菜单后置为true

	// 被C++使用
	public String			name;								// 发送者名称
	public String			asn;								// 联盟简称
	public String			vip;								// vip信息
	public String			headPic;							// 系统头像
	public int				gmod;
	public int				headPicVer;							// 自定义头像

	/**
	 * C++创建的对象可能没有默认值赋值，需要补上
	 */
	public void initNullField()
	{
		if (currentText == null)
		{
			currentText = "";
		}
	}
	
	public UserInfo getUser()
	{
		UserInfo user = UserManager.getInstance().getUser(uid);
		if(user == null){
			System.out.println("UserInfo getUser() null:" + " uid = " + uid);
		}
		return user;
	}
	
	// 如果希望获取用户信息之前可以看到基本信息，可以启用isDummy判断
	public String getName()
	{
		return getUser().userName;
//		return getUser().isDummy ? name : getUser().userName;
	}
	public int getSrcServerId()
	{
		return getUser().crossFightSrcServerId;
	}
	public String getASN()
	{
		return getUser().asn;
//		return getUser().isDummy ? asn : getUser().asn;
	}
	public String getVip()
	{
		return getUser().getVipInfo();
//		return getUser().isDummy ? vip : getUser().vipInfo;
	}
	public String getHeadPic()
	{
		return getUser().headPic;
//		return getUser().isDummy ? headPic : getUser().headPic;
	}
	public int getGmod()
	{
		return getUser().mGmod;
//		return getUser().isDummy ? gmod : getUser().mGmod;
	}
	public int getHeadPicVer()
	{
		return getUser().headPicVer;
//		return getUser().isDummy ? headPicVer : getUser().headPicVer;
	}

	public void initUserForReceivedMsg(String mailOpponentUid, String mailOpponentName)
	{
		checkUser(uid, "", lastUpdateTime);
		
		// 有时一个私信session里只有我自己说的话，但也需要取到对方的信息（这样channel才会有标题），所以加以下判断
		String fromUid = ChannelManager.getInstance().getModChannelFromUid(mailOpponentUid);
		if(channelType == DBDefinition.CHANNEL_TYPE_USER && StringUtils.isNotEmpty(fromUid) && !fromUid.equals(uid))
		{
			checkUser(fromUid, mailOpponentName, 0);
		}
	}
	
	private void checkUser(String uid, String name, int updateTime)
	{
		UserInfo user = UserManager.getInstance().getUser(uid);
		
		boolean isOld = false;
		if(user != null) isOld = updateTime > 0 ? updateTime > user.lastUpdateTime : false;
//		System.out.println("checkUser() initUserForReceivedMsg: " + uid + " " + name + " " + updateTime + " " + isOld);
//		if(user != null)	System.out.println("user.lastUpdateTime: " + user.lastUpdateTime + " !user.isValid():" + !user.isValid());
		
		if (user == null || !user.isValid() || (isOld && !user.uid.equals(UserManager.getInstance().getCurrentUserId())))
		{
			if (user == null){
				user = new UserInfo(uid);
				if(StringUtils.isNotEmpty(name)) user.userName = name;
				UserManager.getInstance().addUser(user);
			}
			
			// 从后台取userInfo
			ArrayList<String> uids = new ArrayList<String>();
			uids.add(uid);
			UserManager.getInstance().getMultiUserInfo(uids);
		}
	}

	public void initUserForSendedMsg()
	{
		UserManager.getInstance().getCurrentUser();
	}

	// 用于从数据库获取消息
	public MsgItem(Cursor c)
	{
		try{
			_id = c.getInt(c.getColumnIndex(BaseColumns._ID));
			tableVer = c.getInt(c.getColumnIndex(DBDefinition.COLUMN_TABLE_VER));
			sequenceId = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_SEQUENCE_ID));
			uid = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_USER_ID));
			mailId = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_MAIL_ID));
			createTime = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_CREATE_TIME));
			sendLocalTime = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_LOCAL_SEND_TIME));
			post = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_TYPE));
			channelType = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_CHANNEL_TYPE));
			msg = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_MSG));
			translateMsg = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_TRANSLATION));
			originalLang = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_ORIGINAL_LANGUAGE));
			translatedLang = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_TRANSLATED_LANGUAGE));
			sendState = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_STATUS));
			attachmentId = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_ATTACHMENT_ID));
			media = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_MEDIA));
			UserManager.getInstance().getUser(uid);
//			System.out.println("msg:"+msg+"  MsgItem uid:"+uid+"   getCurrentUserId:"+UserManager.getInstance().getCurrentUserId());
			isSelfMsg = uid.equals(UserManager.getInstance().getCurrentUserId());
			isNewMsg = false;
			if (hasTranslation() && !isTranlateDisable() && !isOriginalSameAsTargetLang())
				this.hasTranslated = true;
			else
				this.hasTranslated = false;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	/**
	 * TODO 是否应该包括table_ver
	 * 它是用来表示表的版本，不是记录的版本
	 * 它有默认值，问题是alter表不会改变该字段的默认值，但用CURRENT_DATABASE_VERSION也不妥
	 */
	public ContentValues getContentValues()
	{
		ContentValues cv = new ContentValues();
		cv.put(DBDefinition.COLUMN_TABLE_VER, DBHelper.CURRENT_DATABASE_VERSION);
		cv.put(DBDefinition.CHAT_COLUMN_SEQUENCE_ID, sequenceId);
		cv.put(DBDefinition.CHAT_COLUMN_USER_ID, uid);
		cv.put(DBDefinition.CHAT_COLUMN_MAIL_ID, mailId);
		cv.put(DBDefinition.CHAT_COLUMN_CREATE_TIME, createTime);
		cv.put(DBDefinition.CHAT_COLUMN_LOCAL_SEND_TIME, sendLocalTime);
		cv.put(DBDefinition.CHAT_COLUMN_TYPE, post);
		cv.put(DBDefinition.CHAT_COLUMN_CHANNEL_TYPE, channelType);
		cv.put(DBDefinition.CHAT_COLUMN_MSG, msg);
		cv.put(DBDefinition.CHAT_COLUMN_TRANSLATION, translateMsg);
		cv.put(DBDefinition.CHAT_COLUMN_ORIGINAL_LANGUAGE, originalLang);
		cv.put(DBDefinition.CHAT_COLUMN_TRANSLATED_LANGUAGE, translatedLang);
		cv.put(DBDefinition.CHAT_COLUMN_STATUS, sendState);
		cv.put(DBDefinition.CHAT_COLUMN_ATTACHMENT_ID, attachmentId);
		cv.put(DBDefinition.CHAT_COLUMN_MEDIA, media);
		return cv;
	}

	// 用于发送消息
	public MsgItem(String uidStr,
					boolean isNewMsg,
		  			boolean isSelf,
		  			int channelType,
		  			int post,
		  			String msgStr,
		  			int sendLocalTime)
	{
		this.uid = uidStr;
		this.isNewMsg = isNewMsg;
		this.isSelfMsg = isSelf && (post!=100);
		this.channelType = channelType;
		this.post = post;
		this.msg = msgStr;
		if (hasTranslation() && !isTranlateDisable())
			this.hasTranslated = true;
		else
			this.hasTranslated = false;
		this.sendLocalTime = sendLocalTime;
	}
	
	// 用于wrapper假消息
	public MsgItem(int seqId,
					boolean isNewMsg,
		  			boolean isSelf,
		  			int channelType,
		  			int post,
		  			String uidStr,
		  			String msgStr,
		  			String translateMsgStr,
		  			String originalLangStr,
		  			int sendLocalTime)
	{
		this.sequenceId = seqId;
		this.isNewMsg = isNewMsg;
		this.isSelfMsg = isSelf && (post!=100);
		this.channelType = channelType;
		this.msg = msgStr;
		this.uid = uidStr;
		this.post = post;
		this.translateMsg = translateMsgStr;
		this.originalLang = originalLangStr;
		this.sendLocalTime = sendLocalTime;

		setExternalInfo();
	}

  public void setExternalInfo()
  {
	  if(hasTranslation() && !isTranlateDisable())
	  {
		  this.hasTranslated=true;
	  }
	  else
	  {
		  this.hasTranslated=false;
	  }
	  
	  if(isSystemHornMsg())
	  {
		  this.headPic ="guide_player_icon";
	  }
  }

	public boolean isEqualTo(MsgItem msgItem)
	{
		if (this.isSelfMsg == msgItem.isSelfMsg && this.msg.equals(msgItem.msg)) return true;
		return false;
	}
	
	public boolean isSameLang()
	{
		if(StringUtils.isNotEmpty(originalLang) && originalLang.equals(translatedLang))
			return true;
		return false;
	}

	public boolean isInAlliance()
	{
		return !getASN().equals("");
	}

	public boolean isSystemHornMsg()
	{
		return (isHornMessage() && uid.equals("3000002"));
	}
	
	public boolean isTranlateDisable()
	{
		if(ConfigManager.autoTranlateMode <= 0 && translateMsg.equals(""))
			return true;
//		System.out.println("isTranlateDisable disableLang:"+TranslateManager.getInstance().disableLang+"  msg:"+msg+"  translateMsg:"+translateMsg+"  originalLang:"+originalLang);
		boolean isContainsOriginLang=false;	
		if(originalLang.contains(","))
		{
			String langStr[]=originalLang.split(",");
			for(int i=0;i<langStr.length;i++)
			{
				if(!langStr[i].equals("") && isContainsLang(TranslateManager.getInstance().disableLang,langStr[i]))
				{
					isContainsOriginLang=true;
					break;
				}
			}
		}
		else
		{
			isContainsOriginLang=isContainsLang(TranslateManager.getInstance().disableLang,originalLang);
		}
		
		if(!originalLang.equals("") && !TranslateManager.getInstance().disableLang.equals("") && isContainsOriginLang )
			return true;

		return false;
	}
	
	private boolean isContainsLang(String disableLang,String lang)
	{
		boolean ret = false;
		if(StringUtils.isNotEmpty(disableLang) && StringUtils.isNotEmpty(originalLang))
		{
			if(disableLang.contains(lang))
				ret = true;
			else
			{
				if(((disableLang.contains("zh-CN") || disableLang.contains("zh_CN") || disableLang.contains("zh-Hans")) && isZh_CN(lang))
						|| ((disableLang.contains("zh-TW") || disableLang.contains("zh_TW") || disableLang.contains("zh-Hant")) && isZh_TW(lang)))
						ret = true;
			}
		}
		return ret;
	}

	public boolean isCustomHeadImage()
	{
		try
		{
			return getUser().getHeadPicVer() > 0 && getUser().getHeadPicVer() < 1000000 && !getUser().getCustomHeadPic().equals("");
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
			return false;
		}
	}

	//是否是提示消息,显示在中间
	public boolean isTipMsg()
	{
		return post == MSG_TYPE_CHATROOM_TIP;
	}
	
	public boolean isModMsg()
	{
		return post == MSG_TYPE_MOD;
	}
	
	public String getAllianceLabel()
	{
		if (isInAlliance())
		{
			return "(" + getASN() + ") ";
		}
		else
		{
			return "";
		}
	}

	public String getVipLabel()
	{
		return getVip() + " ";
	}

	public boolean isBattleReport()
	{
		return post == 4;
	}

	public boolean isDetectReport()
	{
		return post == 5;
	}

	public boolean isAnnounceInvite()
	{
		return post == 3;
	}

	public boolean isHornMessage()
	{
		return post == 6;
	}
	
	public boolean isEquipMessage()
	{
		return post == 7;
	}

	/**
	 * TODO 修改为 (post > 0 && post <= 5)，以避免item.isSystemMessage() && !item.isHornMessage()这种写法
	 */
	public boolean isSystemMessage()
	{
		return post > 0 && post<=7 ;
	}

	private Date getSendUtcDate()
	{
		int t = createTime > 0 ? createTime : sendLocalTime;
		Date date = new Date((long) t * 1000);
		return date;
	}

	public String getSendTime()
	{
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.getDefault());
		return formatter.format(getSendUtcDate());
	}

	public String getSendTimeYMD()
	{
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
		return formatter.format(getSendUtcDate());
	}

	public String getSendTimeHM()
	{
		SimpleDateFormat formatter = new SimpleDateFormat("HH:mm", Locale.getDefault());
		return formatter.format(getSendUtcDate());
	}

	public boolean hasTranslation()
	{
		return StringUtils.isNotEmpty(translateMsg);
	}
	
	public boolean canShowTranslateMsg()
	{
		return hasTranslation() && hasTranslated && !isOriginalSameAsTargetLang() && (!isTranlateDisable() || isTranslatedByForce);
	}
	
	private boolean isZh_CN(String lang)
	{
		if(StringUtils.isNotEmpty(lang) && lang.equals("zh-CN") || lang.equals("zh_CN") || lang.equals("zh-Hans"))
			return true;
		return false;
	}
	
	private boolean isZh_TW(String lang)
	{
		if(StringUtils.isNotEmpty(lang) && lang.equals("zh-TW") || lang.equals("zh_TW") || lang.equals("zh-Hant"))
			return true;
		return false;
	}
	
	private boolean isSameZhLang(String targetLang)
	{
		if(StringUtils.isNotEmpty(originalLang) && StringUtils.isNotEmpty(targetLang) && ((isZh_CN(originalLang) && isZh_CN(targetLang)) || (isZh_TW(originalLang) && isZh_TW(targetLang))))
				return true;
		return false;
	}
	
	public boolean isOriginalSameAsTargetLang()
	{
		boolean isSame=false;
		if(StringUtils.isNotEmpty(originalLang) && StringUtils.isNotEmpty(ConfigManager.getInstance().gameLang) && (ConfigManager.getInstance().gameLang.equals(originalLang) || isSameZhLang(ConfigManager.getInstance().gameLang)))
			isSame=true;
		return isSame;
	}
}