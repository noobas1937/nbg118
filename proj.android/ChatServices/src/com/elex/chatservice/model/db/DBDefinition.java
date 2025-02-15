package com.elex.chatservice.model.db;

import android.provider.BaseColumns;

public class DBDefinition
{
	public static final String	TABLE_SQLITE_MASTER				= "sqlite_master";
	public static final String	COLUMN_TABLE_VER				= "TableVer";		// 表版本号

	/**
	 * UserID和Type的取值枚举
	 * 								(后缀不同是为了防止id冲突)
	 * 1.玩家（会与消息关联）			UserID:玩家id				Type:2
	 * 2.国家频道						UserID:国家号@country		Type:0
	 * 3.联盟频道						UserID:联盟id@alliance		Type:1
	 * 4.群							UserID:群id@chatroom			Type:3
	 * 5.官方信息（类似公众号）			UserID:频道id@official		Type:4
	 * 					更新公告		UserID:announce@official	Type:4
	 * 					系统邮件		UserID:update@official		Type:4
	 * 					战斗报告		UserID:report@official		Type:4
	 */
	public static final String	CHANNEL_ID_POSTFIX_COUNTRY		= "@country";
	public static final String	CHANNEL_ID_POSTFIX_ALLIANCE		= "@alliance";
	public static final String	CHANNEL_ID_POSTFIX_USER			= "";
	public static final String	CHANNEL_ID_POSTFIX_CHATROOM		= "@chatroom";
	public static final String	CHANNEL_ID_POSTFIX_OFFICIAL		= "@official";
	public static final String	CHANNEL_ID_POSTFIX_MOD			= "@mod";
	public static final int		CHANNEL_TYPE_COUNTRY			= 0;
	public static final int		CHANNEL_TYPE_ALLIANCE			= 1;
	public static final int		CHANNEL_TYPE_USER				= 2;
	public static final int		CHANNEL_TYPE_CHATROOM			= 3;
	public static final int		CHANNEL_TYPE_OFFICIAL			= 4;

	public static String getPostfixForType(int type)
	{
		switch (type)
		{
			case CHANNEL_TYPE_USER:
				return CHANNEL_ID_POSTFIX_USER;
			case CHANNEL_TYPE_COUNTRY:
				return CHANNEL_ID_POSTFIX_COUNTRY;
			case CHANNEL_TYPE_ALLIANCE:
				return CHANNEL_ID_POSTFIX_ALLIANCE;
			case CHANNEL_TYPE_CHATROOM:
				return CHANNEL_ID_POSTFIX_CHATROOM;
			case CHANNEL_TYPE_OFFICIAL:
				return CHANNEL_ID_POSTFIX_OFFICIAL;
			default:
				return null;
		}
	}

	public static String chatTableId2Name(String md5TableId)
	{
		return DBDefinition.TABEL_CHAT + "_" + md5TableId;
	}
	
	public static String mailTableId2Name(String md5TableId)
	{
		return DBDefinition.TABEL_MAIL + "_" + md5TableId;
	}


	/**
	 * User表
	 */
	public static final String	TABEL_USER						= "User";
	public static final String	USER_COLUMN_USER_ID				= "UserID";			// uid，唯一
	public static final String	USER_COLUMN_NICK_NAME			= "NickName";		// 用户名（群名）
	public static final String	USER_COLUMN_ALLIANCE_ID			= "AllianceId";		// 联盟ID
	public static final String	USER_COLUMN_ALLIANCE_NAME		= "AllianceName";	// 联盟名称
	public static final String	USER_COLUMN_ALLIANCE_RANK		= "AllianceRank";	// 联盟等级，只有自己有
	public static final String	USER_COLUMN_SERVER_ID			= "ServerId";		// 国家id，只有自己有
	public static final String	USER_CROSS_FIGHT_SRC_SERVER_ID	= "CrossFightSrcServerId";// 跨服战时的原服id，若为-1表示没有跨服
	public static final String	USER_COLUMN_TYPE				= "Type";			// 用户类型 1:自己 2:
	public static final String	USER_COLUMN_HEAD_PIC			= "HeadPic";		// 系统自带头像
	public static final String	USER_COLUMN_CUSTOM_HEAD_PIC		= "CustomHeadPic";	// 自定义头像
	public static final String	USER_COLUMN_PRIVILEGE			= "Privilege";		// 特殊用户 2:mod 4:tmod 5:smod 3:gm
	public static final String	USER_COLUMN_VIP_LEVEL			= "VipLevel";		// vip等级
	public static final String	USER_COLUMN_VIP_END_TIME		= "VipEndTime";		// vip结束时间，结束后vip信息显示为空
	public static final String	USER_COLUMN_LAST_UPDATE_TIME	= "LastUpdateTime"; // 上次更新时间
	public static final String	USER_COLUMN_LAST_CHAT_TIME		= "LastChatTime";	// 上次聊天时间
	public static final String	USER_COLUMN_SEX					= "Sex";			// 0:未定义 1:男 2:女

	public static final String	CREATE_TABEL_USER				= "CREATE TABLE IF NOT EXISTS " + TABEL_USER + "("
																	+ BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT , "
																	+ COLUMN_TABLE_VER + " INT4 DEFAULT " + DBHelper.CURRENT_DATABASE_VERSION + ", "
																	+ USER_COLUMN_USER_ID + " TEXT NOT NULL UNIQUE ON CONFLICT IGNORE , "
																	+ USER_COLUMN_NICK_NAME + " TEXT , "
																	+ USER_COLUMN_ALLIANCE_ID + " TEXT , "
																	+ USER_COLUMN_ALLIANCE_NAME + " TEXT , "
																	+ USER_COLUMN_ALLIANCE_RANK + " INTEGER DEFAULT -1 , "
																	+ USER_COLUMN_SERVER_ID + " INTEGER DEFAULT -1 , "
																	+ USER_CROSS_FIGHT_SRC_SERVER_ID + " INTEGER DEFAULT -2 , "
																	+ USER_COLUMN_TYPE + " INTEGER DEFAULT 0 , "
																	+ USER_COLUMN_HEAD_PIC + " TEXT , "
																	+ USER_COLUMN_CUSTOM_HEAD_PIC + " INTEGER DEFAULT -1 , "
																	+ USER_COLUMN_PRIVILEGE + " INTEGER DEFAULT -1 , "
																	+ USER_COLUMN_VIP_LEVEL + " INTEGER DEFAULT -1 , "
																	+ USER_COLUMN_VIP_END_TIME + " INTEGER DEFAULT 0 , "
																	// + USER_COLUMN_SEX + " INTEGER DEFAULT 0 , "
																	+ USER_COLUMN_LAST_UPDATE_TIME + " INTEGER DEFAULT 0 , "
																	+ USER_COLUMN_LAST_CHAT_TIME + " INTEGER DEFAULT 0 )";

	/**
	 * Chat表
	 */
	public static final String	TABEL_CHAT						= "Chat";
	public static final String	CHAT_COLUMN_SEQUENCE_ID			= "SequenceID"; 	// 消息的序号
	public static final String	CHAT_COLUMN_MAIL_ID				= "MailID";			// 私信的uid
	public static final String	CHAT_COLUMN_USER_ID				= "UserID";			// uid，群聊中才有
	public static final String	CHAT_COLUMN_CREATE_TIME			= "CreateTime";		// 消息发送的时间，由后台决定
	public static final String	CHAT_COLUMN_LOCAL_SEND_TIME		= "LocalSendTime";	// 消息发送的本地时间，用于第一次返回时判断是否是同一条
	public static final String	CHAT_COLUMN_TYPE				= "Type";			// post：是否为系统信息，“0”表示不是，非“0”表示是
	public static final String	CHAT_COLUMN_CHANNEL_TYPE		= "ChannelType";	// 5种对话type
	public static final String	CHAT_COLUMN_MSG					= "Msg";			// 消息内容
	public static final String	CHAT_COLUMN_TRANSLATION			= "Translation";	// 翻译后的消息内容
	public static final String	CHAT_COLUMN_ORIGINAL_LANGUAGE	= "OriginalLanguage";	// 源语言
	public static final String	CHAT_COLUMN_TRANSLATED_LANGUAGE	= "TranslatedLanguage";	// 翻译后的语言
	public static final String	CHAT_COLUMN_STATUS				= "Status";			// 消息状态 SENDING=0; SEND_FAILED=1; SEND_SUCCESS=2;
	public static final String	CHAT_COLUMN_ATTACHMENT_ID		= "AttachmentId";	// reportUid:战报UID detectReportUid:侦察战报UID
	public static final String	CHAT_COLUMN_MEDIA				= "Media";			// 媒体资源

	public static final String	CHAT_TABLE_NAME_PLACEHOLDER		= TABEL_CHAT + "_" + "%chat_id%";
	public static final String	CREATE_TABLE_CHAT				= "CREATE TABLE IF NOT EXISTS " + CHAT_TABLE_NAME_PLACEHOLDER + "("
																	+ BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT , "
																	+ COLUMN_TABLE_VER + " INT4 DEFAULT " + DBHelper.CURRENT_DATABASE_VERSION + ", "
																	+ CHAT_COLUMN_SEQUENCE_ID + " INTEGER DEFAULT -1 , "
																	+ CHAT_COLUMN_MAIL_ID + " TEXT , "
																	+ CHAT_COLUMN_USER_ID + " TEXT NOT NULL , "	// 系统发的消息也有uid，注意空字符串也符合Not Null
																	+ CHAT_COLUMN_CHANNEL_TYPE + " INTEGER CHECK(" + CHAT_COLUMN_CHANNEL_TYPE + " >= 0 ) , "
																	+ CHAT_COLUMN_CREATE_TIME + " INTEGER DEFAULT 0 , "
																	+ CHAT_COLUMN_LOCAL_SEND_TIME + " INTEGER DEFAULT 0 , "
																	+ CHAT_COLUMN_TYPE + " INTEGER DEFAULT -1 , "
																	+ CHAT_COLUMN_MSG + " TEXT , "
																	+ CHAT_COLUMN_TRANSLATION + " TEXT , "
																	+ CHAT_COLUMN_ORIGINAL_LANGUAGE + " TEXT , "
																	+ CHAT_COLUMN_TRANSLATED_LANGUAGE + " TEXT , "
																	+ CHAT_COLUMN_STATUS + " INTEGER DEFAULT -1 , "
																	+ CHAT_COLUMN_ATTACHMENT_ID + " TEXT , "
																	+ CHAT_COLUMN_MEDIA + " TEXT )";

	/**
	 * Channel表(聊天频道属性)，条目与User表一一对应
	 */
	public static final String	TABEL_CHANNEL					= "Channel";
	public static final String	CHANNEL_CHANNEL_ID				= "ChannelID";		// 与User的uid字段内容一样
	public static final String	CHANNEL_MIN_SEQUENCE_ID			= "MinSequenceID";	// 用于频道最新消息序号
	public static final String	CHANNEL_MAX_SEQUENCE_ID			= "MaxSequenceID";	// 用于频道最新消息序号
	public static final String	CHANNEL_TYPE					= "ChannelType";	// 5种对话type
	public static final String	CHANNEL_CHATROOM_MEMBERS		= "ChatRoomMembers";// 用于聊天室成员列表
	public static final String	CHANNEL_CHATROOM_OWNER			= "ChatRoomOwner";	// 聊天室房主
	public static final String	CHANNEL_IS_MEMBER				= "IsMember";		// 当前玩家是否属于聊天室成员
	public static final String	CHANNEL_CUSTOM_NAME				= "CustomName";		// 聊天室自定义名称
	public static final String	CHANNEL_UNREAD_COUNT			= "UnreadCount";	// 未读数目
	public static final String  CHANNEL_LATEST_ID				= "LatestId";		// 最近邮件的ID（邮件专用）
	public static final String	CHANNEL_LATEST_TIME				= "LatestTime";		// 最近消息时间
	public static final String	CHANNEL_LATEST_MODIFY_TIME		= "LatestModifyTime";// 最近修改时间，仅针对系统邮件
	public static final String  CHANNEL_SETTINGS				= "Settings";		// 置顶、免打扰、保存到通讯录、聊天背景、是否显示昵称等开关型属性，key1:0|key2:1形式（无顺序，没有就是默认值），或者用0|1|0形式

	public static final String	CREATE_TABEL_CHANNEL			= "CREATE TABLE IF NOT EXISTS " + TABEL_CHANNEL + "("
																	+ BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT , "
																	+ COLUMN_TABLE_VER + " INT4 DEFAULT " + DBHelper.CURRENT_DATABASE_VERSION + ", "
																	+ CHANNEL_TYPE + " INTEGER CHECK(" + CHANNEL_TYPE + " >= 0) , " // INTEGER加NOT NULL没用，INTEGER可以输入text
																	+ CHANNEL_CHANNEL_ID + " TEXT NOT NULL UNIQUE ON CONFLICT IGNORE , "
																	+ CHANNEL_MIN_SEQUENCE_ID + " INTEGER DEFAULT -1 , "
																	+ CHANNEL_MAX_SEQUENCE_ID + " INTEGER DEFAULT -1 , "
																	+ CHANNEL_CHATROOM_MEMBERS + " TEXT , "
																	+ CHANNEL_CHATROOM_OWNER + " TEXT , "
																	+ CHANNEL_IS_MEMBER + " INTEGER DEFAULT -1 , "
																	+ CHANNEL_CUSTOM_NAME + " TEXT , "
																	+ CHANNEL_UNREAD_COUNT + " INTEGER DEFAULT 0 , "
																	+ CHANNEL_LATEST_ID + " TEXT , "
																	+ CHANNEL_LATEST_TIME + " INTEGER DEFAULT -1 , "
																	+ CHANNEL_LATEST_MODIFY_TIME + " INTEGER DEFAULT -1 , "
																	+ CHANNEL_SETTINGS + " TEXT ) ";

	/**
	 * Mail表，存储系统邮件，都是发给当前玩家的
	 */
	public static final String	TABEL_MAIL						= "Mail";
	public static final String	MAIL_ID							= "ID";				// 邮件唯一ID
	public static final String	MAIL_CHANNEL_ID					= "ChannelID";		// 邮件的channelId
	public static final String	MAIL_FROM_USER_ID				= "FromUser";
	public static final String	MAIL_FROM_NAME					= "FromName";
	public static final String	MAIL_TITLE						= "Title";
	public static final String	MAIL_CONTENTS					= "Contents";
	public static final String	MAIL_REWARD_ID					= "RewardId";
	public static final String	MAIL_ITEM_ID_FLAG				= "ItemIdFlag";
	public static final String	MAIL_STATUS						= "Status";
	public static final String	MAIL_TYPE						= "Type";
	public static final String	MAIL_REWARD_STATUS				= "RewardStatus";
	public static final String	MAIL_SAVE_FLAG					= "SaveFlag";
	public static final String	MAIL_CREATE_TIME				= "CreateTime";
	public static final String	MAIL_REPLY						= "Reply";
	public static final String	MAIL_TRANSLATION				= "Translation";
	public static final String	MAIL_TITLE_TEXT					= "TitleText";
	public static final String	MAIL_SUMMARY					= "Summary";
	public static final String	MAIL_LANGUAGE					= "Language";
	public static final String	PARSE_VERSION					= "ParseVersion";
	
	public static final String	CREATE_TABEL_MAIL			= "CREATE TABLE IF NOT EXISTS " + TABEL_MAIL + "("
																	+ BaseColumns._ID + " INTEGER PRIMARY KEY AUTOINCREMENT , "
																	+ COLUMN_TABLE_VER + " INT4 DEFAULT " + DBHelper.CURRENT_DATABASE_VERSION + ", "
																	+ MAIL_ID + " TEXT NOT NULL UNIQUE ON CONFLICT IGNORE , "
																	+ MAIL_CHANNEL_ID + " TEXT NOT NULL, "
																	+ MAIL_FROM_USER_ID + " TEXT , "
																	+ MAIL_FROM_NAME + " TEXT NOT NULL , "
																	+ MAIL_TITLE + " TEXT NOT NULL , "
																	+ MAIL_CONTENTS + " TEXT NOT NULL , "
																	+ MAIL_REWARD_ID + " TEXT , "
																	+ MAIL_ITEM_ID_FLAG + " INTEGER DEFAULT -1 , "
																	+ MAIL_STATUS + " INTEGER DEFAULT -1 , "
																	+ MAIL_TYPE + " INTEGER DEFAULT -1 , "
																	+ MAIL_REWARD_STATUS + " INTEGER DEFAULT -1 , "
																	+ MAIL_SAVE_FLAG + " INTEGER DEFAULT -1 , "
																	+ MAIL_CREATE_TIME + " INTEGER DEFAULT -1 , "
																	+ MAIL_REPLY + " INTEGER DEFAULT -1 , "
																	+ MAIL_TRANSLATION + " TEXT , "
																	+ MAIL_TITLE_TEXT + " TEXT , "
																	+ MAIL_SUMMARY + " TEXT , "
																	+ MAIL_LANGUAGE + " TEXT , "
																	+ PARSE_VERSION + " INTEGER DEFAULT -1 )";
}
