package com.elex.chatservice.model;

import android.content.Context;
import android.view.WindowManager;

/**
 * 全局配置
 */
public class ConfigManager
{
	private static ConfigManager	instance;

	/** 只对当前用户有意义，不存数据库 */
	public boolean					scaleFontandUI			= true;
	public boolean					enableChatInputField	= false;
	public boolean					isFirstJoinAlliance		= true;
	/** 用于论坛判定默认显示语言，尚未使用，与游戏进程相关 */
	public String					gameLang				= "zh-CN";

	/** 后台可配置，会被复写 */
	public static int				sendInterval			= 1000;
	/** 自定义头像开关 */
	public static boolean			enableCustomHeadImg		= true;
	public static int				autoTranlateMode		= 0;
	public static boolean			enableChatHorn			= true;
	/** 喇叭消息最大输入长度 */
	public static int				maxHornInputLength		= 140;
	public static boolean			isFirstUserHorn			= true;
	public static boolean			isFirstUserCornForHorn	= true;
	public static boolean			isHornBtnEnable			= false;
	public static boolean			useWebSocketServer		= false;
	public static boolean			isRecieveFromWebSocket	= true;
	public static boolean			isSendFromWebSocket		= true;

	private ConfigManager()
	{
	}

	public static ConfigManager getInstance()
	{
		if (instance == null)
		{
			instance = new ConfigManager();
		}
		return instance;
	}

	private static boolean		calcSizeCompleted	= false;
	public static double		scaleRatio			= 0;
	public static double		scaleRatioButton	= 0;
	private final static double	designWidth			= 640;
	private final static double	designHeight		= 852;

	public static void calcScale(Context context)
	{
		if (calcSizeCompleted)
			return;

		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		double screenWidth = windowManager.getDefaultDisplay().getWidth();
		double screenHeight = windowManager.getDefaultDisplay().getHeight();
		double scaleX = screenWidth / designWidth;
		double scaleY = screenHeight / designHeight;
		double minScale = Math.min(scaleX, scaleY);
		scaleRatio = minScale;
		scaleRatioButton = minScale;
		// 在大屏上字体可能会偏大，可能需要用dp计算才行,先加个修正因子
		scaleRatio = scaleRatio > (1 / 0.84390234277028) ? scaleRatio * 0.84390234277028 : scaleRatio;
		if (scaleRatio > 1)
		{
			// 小米pad是1.873170518056575
			scaleRatio = 1 + (scaleRatio - 1) * 0.5;
		}
		else
		{
			// 小屏幕不要缩放，否则太小（华为 U8800Pro 800x480）
			scaleRatio = 1 - (1 - scaleRatio) * 0.5;
		}
		calcSizeCompleted = true;
	}
}
