package com.elex.chatservice.model;

import java.util.HashMap;
import java.util.Map;
import org.apache.commons.lang.StringUtils;

public class MailOldUI
{
	// Channel ICON
	public static final String	CHANNEL_ICON_STUDIO							= "mail_pic_flag_2";
	public static final String	CHANNEL_ICON_SYSTEM							= "mail_pic_flag_2";
	public static final String	CHANNEL_ICON_MOD							= "mange_mod";
	public static final String	CHANNEL_ICON_ANNOUNCEMENT					= "mail_pic_flag_2";

	// 新邮件UI邮件ICON
	public static final String	MAIL_NEW_ICON_ANNOUNCEMENT					= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_BATTLE_EXPLORE				= "mail_pic_flag_14";
	public static final String	MAIL_NEW_ICON_BATTLE_CAMP					= "mail_pic_flag_9";
	public static final String	MAIL_NEW_ICON_BATTLE_CAPTURE				= "mail_pic_flag_9";
	public static final String	MAIL_NEW_ICON_BATTLE_CITY_DEFENT_FAILURE	= "mail_pic_flag_4";
	public static final String	MAIL_NEW_ICON_BATTLE_CITY_DEFENT_VICTORY	= "mail_pic_flag_4";
	public static final String	MAIL_NEW_ICON_BATTLE_CITY_FAILURE			= "mail_pic_flag_4_1";
	public static final String	MAIL_NEW_ICON_BATTLE_CITY_VICTORY			= "mail_pic_flag_4_1";
	public static final String	MAIL_NEW_ICON_BATTLE_DETECT					= "mail_pic_flag_8";
	public static final String	MAIL_NEW_ICON_BATTLE_KNIGHT					= "mail_pic_flag_4";
	public static final String	MAIL_NEW_ICON_BATTLE_MONSTER				= "mail_pic_flag_18";
	public static final String	MAIL_NEW_ICON_BATTLE_REPORT					= "mail_pic_flag_4_1";
	public static final String	MAIL_NEW_ICON_INVITE_JOIN_ALLIANCE			= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_INVITE_JOIN					= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_INVITE_KICKEDOUT				= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_INVITE_MOVE					= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_INVITE_REJECTED				= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_STUDIO						= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_ID_BINGING					= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_SYSTEM_OTHER					= "mail_pic_flag_2";
	public static final String	MAIL_NEW_ICON_SYSTEM_VIP					= "mail_pic_flag_2";

	private static MailOldUI	_instance									= null;
	private Map<String, String>	oldUIMap									= null;

	public static MailOldUI getInstance()
	{
		if (_instance == null)
			_instance = new MailOldUI();
		return _instance;
	}

	public String getIconByName(String name)
	{
		if (oldUIMap != null && StringUtils.isNotEmpty(name) && oldUIMap.containsKey(name))
			return oldUIMap.get(name);
		return "";
	}

	private MailOldUI()
	{
		// Channel ICON
		oldUIMap = new HashMap<String, String>();
		oldUIMap.put("CHANNEL_ICON_STUDIO", "mail_pic_flag_2");
		oldUIMap.put("CHANNEL_ICON_SYSTEM", "mail_pic_flag_2");
		oldUIMap.put("CHANNEL_ICON_MOD", "mange_mod");
		oldUIMap.put("CHANNEL_ICON_ANNOUNCEMENT", "mail_pic_flag_2");
		oldUIMap.put("CHANNEL_ICON_RESOURCE", "mail_pic_flag_5");
		oldUIMap.put("CHANNEL_ICON_MONSTER", "mail_pic_flag_18");

		oldUIMap.put("MAIL_ICON_ANNOUNCEMENT", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_BATTLE_EXPLORE", "mail_pic_flag_14");
		oldUIMap.put("MAIL_ICON_BATTLE_CAMP", "mail_pic_flag_9");
		oldUIMap.put("MAIL_ICON_BATTLE_CAPTURE", "mail_pic_flag_9");
		oldUIMap.put("MAIL_ICON_BATTLE_CITY_DEFENT_FAILURE", "mail_pic_flag_4");
		oldUIMap.put("MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY", "mail_pic_flag_4");
		oldUIMap.put("MAIL_ICON_BATTLE_CITY_FAILURE", "mail_pic_flag_4_1");
		oldUIMap.put("MAIL_ICON_BATTLE_CITY_VICTORY", "mail_pic_flag_4_1");
		oldUIMap.put("MAIL_ICON_BATTLE_DETECT", "mail_pic_flag_8");
		oldUIMap.put("MAIL_ICON_BATTLE_KNIGHT", "mail_pic_flag_4");
		oldUIMap.put("MAIL_ICON_BATTLE_REPORT", "mail_pic_flag_4_1");
		oldUIMap.put("MAIL_ICON_INVITE_JOIN_ALLIANCE", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_INVITE_JOIN", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_INVITE_KICKEDOUT", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_INVITE_MOVE", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_INVITE_REJECTED", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_STUDIO", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_ID_BINGING", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_SYSTEM", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_SYSTEM_VIP", "mail_pic_flag_2");
		oldUIMap.put("MAIL_ICON_CHAT_ROOM", "mail_pic_flag_31");
	}
}
