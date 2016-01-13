package com.elex.chatservice.model;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import org.apache.commons.lang.StringUtils;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.provider.BaseColumns;
import android.util.Log;
import android.view.View;

import com.alibaba.fastjson.annotation.JSONField;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;

public final class MsgItem
{
	public final static int	SENDING							= 0;
	public final static int	SEND_FAILED						= 1;
	public final static int	SEND_SUCCESS					= 2;

	public final static int	MSG_TYPE_ALLIANCE_JOIN			= 8;
	public final static int	MSG_TYPE_ALLIANCE_RALLY			= 9;
	public final static int	MSG_TYPE_LOTTERY_SHARE			= 10;
	public final static int	MSG_TYPE_ALLIANCETASK_SHARE		= 11;
	public final static int	MSG_TYPE_RED_PACKAGE			= 12;
	/** 增加post时要变更这个值 */
	public final static int	MSG_TYPE_MAX_VALUE				= 12;

	public final static int	MSG_TYPE_CHATROOM_TIP			= 100;
	public final static int	MSG_TYPE_MOD					= 200;
	public final static int	MAIL_MOD_PERSON					= 23;

	public final static int	HANDLED							= 0;
	public final static int	UNHANDLE						= 1;
	public final static int	NONE_MONEY						= 2;
	public final static int	FINISH							= 3;

	public final static int	MSGITEM_TYPE_MESSAGE			= 0;
	public final static int	MSGITEM_TYPE_GIF				= 1;
	public final static int	MSGITEM_TYPE_PIC				= 2;
	public final static int	MSGITEM_TYPE_REDPACKAGE			= 3;
	public final static int	MSGITEM_TYPE_CHATROM_TIP		= 4;
	public final static int	MSGITEM_TYPE_NEW_MESSAGE_TIP	= 5;

	/** 数据库使用的id */
	public int				_id;
	public int				tableVer;
	public int				sequenceId;
	/** 用来标识邮件的id */
	public String			mailId;
	/** uid，群聊时才会存数据库 */
	public String			uid								= "";
	/** 频道类型 */
	public int				channelType						= -1;
	/** 收到的消息会在C++中初始化此字段，对应后台传回来的createTime */
	public int				createTime						= 0;
	/** 数据库中名为type：是否为系统信息，“0”表示不是，非“0”表示是 */
	public int				post							= -1;
	/** 消息体 */
	public String			msg								= "";
	/** 翻译信息 */
	public String			translateMsg					= "";
	/** 源语言 */
	public String			originalLang					= "";
	/** 翻译后的语言 */
	public String			translatedLang					= "";
	/**
	 * 对于自己发的消息,发送状态，0正在发送，1发送失败，2发送成功 红包消息时，表示红包的领取状态,1未领取，0领取过,2被抢光了,3到期了
	 * */
	public int				sendState						= -1;
	/** 战报UID，侦察战报UID,装备ID等 */
	public String			attachmentId					= "";

	public String			media							= "";

	// 运行时属性
	/** 是否是自己的信息 */
	public boolean			isSelfMsg;
	/** 是否是新消息 */
	public boolean			isNewMsg;
	public String			currentText						= "";
	/** 是否被翻译过 */
	public boolean			hasTranslated;
	public boolean			isSendDataShowed				= false;
	public int				lastUpdateTime					= 0;
	/** 本地发送时间戳 */
	public int				sendLocalTime					= 0;
	public boolean			isTranslateByGoogle				= false;
	public boolean			isFirstNewMsg					= false;
	/**
	 * 0:不是第一条 1:第一条且新消息数小于等于200条 2:第一条且新消息数超过200条
	 * */
	public int				firstNewMsgState				= 0;
	/** msgItem所属的Channel */
	public ChatChannel		chatChannel						= null;
	/** 是否强制翻译，点击翻译菜单后置为true，点击原文置为false */
	public boolean			isTranslatedByForce				= false;
	/** 是否做过强制翻译，点击翻译菜单后置为true */
	public boolean			hasTranslatedByForce			= false;
	//是否被强制显示原文
	public boolean			isOriginalLangByForce			= false;

	// 被C++使用
	/** 发送者名称 */
	public String			name;
	/** 联盟简称 */
	public String			asn;
	/** vip信息 */
	public String			vip;
	/** 系统头像 */
	public String			headPic;
	public int				gmod;
	/** 自定义头像 */
	public int				headPicVer;

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

	public MsgItem()
	{

	}

	public UserInfo getUser()
	{
		UserManager.checkUser(uid, "", 0);
		UserInfo user = UserManager.getInstance().getUser(uid);
		return user;
	}

	public String getName()
	{
		return getUser().userName;
	}
	
	public String getLang()
	{
		String lang = originalLang;
		if(StringUtils.isEmpty(lang) && StringUtils.isNotEmpty(getUser().lang))
			lang = getUser().lang;
		return lang;
	}

	public int getSrcServerId()
	{
		return getUser().crossFightSrcServerId;
	}

	public String getASN()
	{
		return getUser().asn;
	}

	public String getVip()
	{
		return getUser().getVipInfo();
	}
	
	public int getVipLevel()
	{
		return getUser().getVipLevel();
	}

	public String getHeadPic()
	{
		return getUser().headPic;
	}

	public int getGmod()
	{
		return getUser().mGmod;
	}

	public int getHeadPicVer()
	{
		return getUser().headPicVer;
	}

	public void initUserForReceivedMsg(String mailOpponentUid, String mailOpponentName)
	{
		if(lastUpdateTime > TimeManager.getInstance().getCurrentTime())
		{
			LogUtil.printVariables(Log.WARN, LogUtil.TAG_MSG, "invalid lastUpdateTime msg:\n" + LogUtil.typeToString(this));
		}
		String fromUid = ChannelManager.getInstance().getModChannelFromUid(mailOpponentUid);
		if (channelType == DBDefinition.CHANNEL_TYPE_USER && StringUtils.isNotEmpty(fromUid) && !fromUid.equals(uid))
		{
			UserManager.checkUser(fromUid, mailOpponentName, lastUpdateTime);
		}
		else
		{
			UserManager.checkUser(uid, "", lastUpdateTime);
		}
	}

	public void initUserForSendedMsg()
	{
		UserManager.getInstance().getCurrentUser();
	}

	/**
	 * 用于从数据库获取消息
	 */
	public MsgItem(Cursor c)
	{
		try
		{
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
			if (isRedPackageMessage() && sendState < 0)
			{
				sendState = UNHANDLE;
			}
			attachmentId = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_ATTACHMENT_ID));
			media = c.getString(c.getColumnIndex(DBDefinition.CHAT_COLUMN_MEDIA));
			UserManager.getInstance().getUser(uid);
			isSelfMsg = uid.equals(UserManager.getInstance().getCurrentUserId());
			isNewMsg = false;
			if (TranslateManager.getInstance().hasTranslated(this))
				this.hasTranslated = true;
			else
				this.hasTranslated = false;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

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
		if (isRedPackageMessage() && sendState < 0)
			sendState = UNHANDLE;
		cv.put(DBDefinition.CHAT_COLUMN_STATUS, sendState);
		cv.put(DBDefinition.CHAT_COLUMN_ATTACHMENT_ID, attachmentId);
		cv.put(DBDefinition.CHAT_COLUMN_MEDIA, media);
		return cv;
	}

	/**
	 * 用于发送消息
	 */
	public MsgItem(String uidStr, boolean isNewMsg, boolean isSelf, int channelType, int post, String msgStr, int sendLocalTime)
	{
		this.uid = uidStr;
		this.isNewMsg = isNewMsg;
		this.isSelfMsg = isSelf && (post != 100);
		this.channelType = channelType;
		this.post = post;
		this.msg = msgStr;
		if (TranslateManager.getInstance().hasTranslated(this))
			this.hasTranslated = true;
		else
			this.hasTranslated = false;
		this.sendLocalTime = sendLocalTime;
	}

	/**
	 * 用于wrapper假消息
	 */
	public MsgItem(int seqId, boolean isNewMsg, boolean isSelf, int channelType, int post, String uidStr, String msgStr,
			String translateMsgStr, String originalLangStr, int sendLocalTime)
	{
		this.sequenceId = seqId;
		this.isNewMsg = isNewMsg;
		this.isSelfMsg = isSelf && (post != 100);
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
		if (TranslateManager.getInstance().hasTranslated(this))
		{
			this.hasTranslated = true;
		}
		else
		{
			this.hasTranslated = false;
		}

		if (isSystemHornMsg())
		{
			this.headPic = "guide_player_icon";
		}
	}

	public boolean isEqualTo(MsgItem msgItem)
	{
		if (this.isSelfMsg == msgItem.isSelfMsg && this.msg.equals(msgItem.msg))
			return true;
		return false;
	}

	public boolean isSelfMsg()
	{
		isSelfMsg = StringUtils.isNotEmpty(uid) && StringUtils.isNotEmpty(UserManager.getInstance().getCurrentUserId())
				&& uid.equals(UserManager.getInstance().getCurrentUserId()) && post != MSG_TYPE_CHATROOM_TIP;
		return isSelfMsg;
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
		if (StringUtils.isNotEmpty(originalLang) && StringUtils.isNotEmpty(TranslateManager.getInstance().disableLang))
		{
			boolean isContainsOriginLang = false;
			if (originalLang.contains(","))
			{
				String langStr[] = originalLang.split(",");
				for (int i = 0; i < langStr.length; i++)
				{
					if (!langStr[i].equals("") && isContainsLang(TranslateManager.getInstance().disableLang, langStr[i]))
					{
						isContainsOriginLang = true;
						break;
					}
				}
			}
			else
			{
				isContainsOriginLang = isContainsLang(TranslateManager.getInstance().disableLang, originalLang);
			}

			if (isContainsOriginLang)
				return true;
		}
		return false;
	}

	private boolean isContainsLang(String disableLang, String lang)
	{
		boolean ret = false;
		if (StringUtils.isNotEmpty(disableLang) && StringUtils.isNotEmpty(originalLang))
		{
			if (disableLang.contains(lang))
				ret = true;
			else
			{
				if (((disableLang.contains("zh-CN") || disableLang.contains("zh_CN") || disableLang.contains("zh-Hans")) && TranslateManager
						.getInstance().isZh_CN(lang))
						|| ((disableLang.contains("zh-TW") || disableLang.contains("zh_TW") || disableLang.contains("zh-Hant")) && TranslateManager
								.getInstance().isZh_TW(lang)))
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

	/**
	 * 是否是聊天室的提示消息,显示在中间
	 */
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

	public boolean isAllianceJoinMessage()
	{
		return post == MSG_TYPE_ALLIANCE_JOIN;
	}

	public boolean isRallyMessage()
	{
		return post == MSG_TYPE_ALLIANCE_RALLY;
	}

	public boolean isLotteryMessage()
	{
		return post == MSG_TYPE_LOTTERY_SHARE;
	}

	/**
	 * 判断是否是系统消息
	 */
	public boolean isSystemMessage()
	{
		return post > 0 && !isTipMsg() && !isModMsg();
	}

	public boolean isAllianceTaskMessage()
	{
		return post == MSG_TYPE_ALLIANCETASK_SHARE;
	}

	public boolean isRedPackageMessage()
	{
		return post == MSG_TYPE_RED_PACKAGE;
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

	public String getSendTimeToShow()
	{
		if (TimeManager.getInstance().isToday(createTime))
		{
			return getSendTimeHM();
		}
		return getSendTime();
	}

	public boolean hasTranslation()
	{
		return StringUtils.isNotEmpty(translateMsg) && !translateMsg.startsWith("{\"code\":{");
	}

	public boolean canShowTranslateMsg()
	{
		return TranslateManager.getInstance().isTranslateMsgValid(this)
				&& (!isTranlateDisable() || isTranslatedByForce)
				&& !isOriginalSameAsTargetLang()
				&& !isOriginalLangByForce ;
	}

	public boolean isOriginalSameAsTargetLang()
	{
		boolean isSame = false;
		if (StringUtils.isNotEmpty(originalLang)
				&& StringUtils.isNotEmpty(ConfigManager.getInstance().gameLang)
				&& (ConfigManager.getInstance().gameLang.equals(originalLang) || TranslateManager.getInstance().isSameZhLang(originalLang,
						ConfigManager.getInstance().gameLang)))
			isSame = true;
		return isSame;
	}

	public boolean isVersionInvalid()
	{
		if (post > MSG_TYPE_MAX_VALUE && !isTipMsg() && !isModMsg() && post != MAIL_MOD_PERSON)
			return true;
		return false;
	}

	public ChatChannel getChatChannel()
	{
		ChatChannel channel = null;
		if (channelType == DBDefinition.CHANNEL_TYPE_COUNTRY)
			channel = ChannelManager.getInstance().getCountryChannel();
		else if (channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE)
			channel = ChannelManager.getInstance().getAllianceChannel();
		return channel;
	}

	public void handleRedPackageFinishState()
	{
		if (!isRedPackageMessage())
			return;
		if (sendState == UNHANDLE && isRedPackageFinish())
		{
			sendState = FINISH;
			ChatChannel channel = getChatChannel();
			if (channel != null)
				DBManager.getInstance().updateMessage(this, channel.getChatTable());
		}
	}

	public boolean isRedPackageFinish()
	{
		if (!isRedPackageMessage())
			return false;
		if (createTime + ChatServiceController.red_package_during_time * 60 * 60 < TimeManager.getInstance().getCurrentTime())
			return true;
		return false;
	}

	public boolean isKingMsg()
	{
		if (StringUtils.isNotEmpty(uid) && StringUtils.isNotEmpty(ChatServiceController.kingUid)
				&& ChatServiceController.kingUid.equals(uid))
			return true;
		return false;
	}

	public int getMsgItemType(Context context)
	{
		if (firstNewMsgState == 1 || firstNewMsgState == 2)
		{
			return MSGITEM_TYPE_NEW_MESSAGE_TIP;
		}
		else
		{
			String replacedEmoj = StickManager.getPredefinedEmoj(msg);
			if (replacedEmoj != null)
			{
				return getPicType(context, replacedEmoj);
			}
			else
			{
				if (isRedPackageMessage())
					return MSGITEM_TYPE_REDPACKAGE;
				else
					return getMessageType();
			}
		}
	}

	public int getMessageType()
	{
		if (isTipMsg())
			return MSGITEM_TYPE_CHATROM_TIP;
		else
			return MSGITEM_TYPE_MESSAGE;
	}

	public int getPicType(Context context, String fileName)
	{
		if (fileName == null)
			return -1;
		int id = ResUtil.getId(context, "drawable", fileName);
		if (id == 0)
			return -1;
		if (context.getString(id).endsWith(".gif"))
		{
			return MSGITEM_TYPE_GIF;
		}
		else
		{
			return MSGITEM_TYPE_PIC;
		}
	}
	
	public boolean isNotInRestrictList()
	{
		return (!isSystemMessage() && !UserManager.getInstance().isInRestrictList(uid, UserManager.BAN_LIST)) || 
				(isHornMessage() && !UserManager.getInstance().isInRestrictList(uid, UserManager.BAN_NOTICE_LIST));
	}

	public boolean isInRestrictList()
	{
		return (!isSystemMessage() && UserManager.getInstance().isInRestrictList(uid, UserManager.BAN_LIST)) || 
				(isHornMessage() && UserManager.getInstance().isInRestrictList(uid, UserManager.BAN_NOTICE_LIST));
	}
}