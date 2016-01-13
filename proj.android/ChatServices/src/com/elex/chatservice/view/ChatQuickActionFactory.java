package com.elex.chatservice.view;

import net.londatiga.android.ActionItem;
import net.londatiga.android.QuickAction;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserManager;

public class ChatQuickActionFactory
{
	public static final int	ID_JOIN_ALLIANCE			= 1;
	public static final int	ID_COPY						= 2;
	public static final int	ID_SEND_MAIL				= 3;
	public static final int	ID_VIEW_PROFILE				= 4;
	public static final int	ID_BAN						= 5;
	public static final int	ID_UNBAN					= 8;
	public static final int	ID_TRANSLATE				= 6;
	public static final int	ID_ORIGINAL_LANGUAGE		= 7;
	public static final int	ID_VIEW_BATTLE_REPORT		= 9;
	public static final int	ID_INVITE					= 10;
	public static final int	ID_BLOCK					= 11;
	public static final int	ID_UNBLOCK					= 12;
	public static final int	ID_VIEW_DETECT_REPORT		= 13;
	public static final int	ID_REPORT_HEAD_IMG			= 14;
	public static final int	ID_VIEW_EQUIPMENT			= 15;
	public static final int	ID_REPORT_PLAYER_CHAT		= 16;
	public static final int	ID_TRANSLATE_NOT_UNDERSTAND	= 17;
	public static final int	ID_SAY_HELLO				= 18;
	public static final int	ID_VIEW_RALLY_INFO			= 19;
	public static final int	ID_VIEW_LOTTERY_SHARE		= 20;
	public static final int	ID_VIEW_ALLIANCETASK_SHARE	= 21;
	public static final int	ID_VIEW_RED_PACKAGE			= 22;

	public static QuickAction createQuickAction(final Activity activity, MsgItem item)
	{
		boolean canTranlate = (!item.isSystemMessage() || item.isHornMessage()) && !item.isSelfMsg();
		boolean canViewEquip = item.isEquipMessage();
		boolean hasTranslated = item.canShowTranslateMsg();
		boolean canJoinAlliance = item.isInAlliance() && !item.isSelfMsg() && !item.isTipMsg()
				&& UserManager.getInstance().getCurrentUser().allianceId.equals("") && !ChatServiceController.isInMailDialog()
				&& !item.isSystemHornMsg();
		boolean canBlock = !item.isSystemMessage() && !item.isHornMessage() && !item.isSelfMsg() && !item.isTipMsg()
				&& !UserManager.getInstance().isInRestrictList(item.uid, UserManager.BLOCK_LIST);
		boolean canUnBlock = !item.isSystemMessage() && !item.isHornMessage() && !item.isSelfMsg() && !item.isTipMsg()
				&& UserManager.getInstance().isInRestrictList(item.uid, UserManager.BLOCK_LIST);
		boolean canBan = item.isNotInRestrictList() && !item.isSelfMsg() && !item.isTipMsg()
				&& (UserManager.getInstance().getCurrentUser().mGmod >= 1 && UserManager.getInstance().getCurrentUser().mGmod != 11)
				&& !ChatServiceController.isInMailDialog();
		boolean canUnBan = item.isInRestrictList() && !item.isSelfMsg() && !item.isTipMsg()
				&& (UserManager.getInstance().getCurrentUser().mGmod == 3) && !ChatServiceController.isInMailDialog();
		boolean canViewBattleReport = !item.isHornMessage()
				&& (item.isBattleReport() && !UserManager.getInstance().getCurrentUser().allianceId.equals(""))
				&& !ChatServiceController.isInMailDialog();
		boolean canViewDetectReport = !item.isHornMessage()
				&& (item.isDetectReport() && !UserManager.getInstance().getCurrentUser().allianceId.equals(""))
				&& !ChatServiceController.isInMailDialog();
		boolean canInvite = !item.isHornMessage() && item.getASN().equals("")
				&& !UserManager.getInstance().getCurrentUser().allianceId.equals("")
				&& UserManager.getInstance().getCurrentUser().allianceRank >= 3 && !item.isSelfMsg() && !item.isTipMsg()
				&& !ChatServiceController.isInMailDialog();
		boolean canReportHeadImg = !item.isSystemMessage() && !item.isSelfMsg() && item.isCustomHeadImage();
		boolean canReportContent = !item.isSystemMessage() && !item.isSelfMsg();
		boolean canSayHello = item.isAllianceJoinMessage() && !item.isSelfMsg();
		boolean canViewRallyInfo = item.isRallyMessage();
		boolean canViewLotteryShare = item.isLotteryMessage();
		boolean canViewAllianceTaskShare = item.isAllianceTaskMessage() && item.msg.contains(LanguageKeys.TIP_ALLIANCE_TASK_SHARE_1);
		boolean canReportTranslate = false; // canTranlate && hasTranslated &&
											// !item.isSystemMessage();

		boolean canSendMail = false;
		QuickAction quickAction = ChatQuickActionFactory.getQuickAction(activity, QuickAction.HORIZONTAL, canJoinAlliance, canSendMail,
				canViewBattleReport, canViewDetectReport, canTranlate, hasTranslated, canBlock, canUnBlock, canBan, canUnBan, canInvite,
				canReportHeadImg, canViewEquip, canReportContent, canSayHello, canViewRallyInfo, canViewLotteryShare,
				canViewAllianceTaskShare, canReportTranslate);

		return quickAction;
	}

	// TODO 参数过于重复
	private static QuickAction getQuickAction(final Activity activity, int orientation, boolean canJoinAlliance, boolean canSendMail,
			boolean canViewBattleReport, boolean canViewDetectReport, boolean canTranlate, boolean hasTranslated, boolean canBlock,
			boolean canUnBlock, boolean canBan, boolean canUnBan, boolean canInvite, boolean canReportHeadImg, boolean canViewEquip,
			boolean canReportContent, boolean canSayHello, boolean canViewRallyInfo, boolean canViewLotteryShare,
			boolean canViewAllianceTaskShare, boolean canReportTranslate)
	{
		QuickAction quickAction = actuallyCreateQuickAction(activity, orientation, canJoinAlliance, canSendMail, canViewBattleReport,
				canViewDetectReport, canTranlate, hasTranslated, canBlock, canUnBlock, canBan, canUnBan, canInvite, canReportHeadImg,
				canViewEquip, canReportContent, canSayHello, canViewRallyInfo, canViewLotteryShare, canViewAllianceTaskShare,
				canReportTranslate, 0);

		if (orientation == QuickAction.HORIZONTAL && quickAction.isWiderThanScreen())
		{
			quickAction = ChatQuickActionFactory.actuallyCreateQuickAction(activity, QuickAction.VERTICAL, canJoinAlliance, canSendMail,
					canViewBattleReport, canViewDetectReport, canTranlate, hasTranslated, canBlock, canUnBlock, canBan, canUnBan,
					canInvite, canReportHeadImg, canViewEquip, canReportContent, canSayHello, canViewRallyInfo, canViewLotteryShare,
					canViewAllianceTaskShare, canReportTranslate, quickAction.getMaxItemWidth());
		}

		return quickAction;
	}

	private static QuickAction actuallyCreateQuickAction(final Activity activity, int orientation, boolean canJoinAlliance,
			boolean canSendMail, boolean canViewBattleReport, boolean canViewDetectReport, boolean canTranlate, boolean hasTranslated,
			boolean canBlock, boolean canUnBlock, boolean canBan, boolean canUnBan, boolean canInvite, boolean canReportHeadImg,
			boolean canViewEquip, boolean canReportContent, boolean canSayHello, boolean canViewRallyInfo, boolean canViewLotteryShare,
			boolean canViewAllianceTaskShare, boolean canReportTranslate, int maxItemWidth)
	{
		// create QuickAction. Use QuickAction.VERTICAL or
		// QuickAction.HORIZONTAL param to define layout orientation
		final QuickAction quickAction = new QuickAction(activity, orientation);

		if (orientation == QuickAction.VERTICAL && maxItemWidth > 0)
		{
			quickAction.maxItemWidth = maxItemWidth;
		}

		if (ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0)
		{
			quickAction.scaleRatio = ConfigManager.scaleRatio;
		}

		if (canViewEquip)
		{
			ActionItem nextItem = new ActionItem(ID_VIEW_EQUIPMENT, LanguageManager.getLangByKey(LanguageKeys.MENU_VIEW_EQUIPMENT));
			quickAction.addActionItem(nextItem);
		}

		if (canViewBattleReport)
		{
			ActionItem battleMsgItem = new ActionItem(ID_VIEW_BATTLE_REPORT, LanguageManager.getLangByKey(LanguageKeys.MENU_BATTLEREPORT));
			quickAction.addActionItem(battleMsgItem);
		}

		if (canViewDetectReport)
		{
			ActionItem detectMsgItem = new ActionItem(ID_VIEW_DETECT_REPORT, LanguageManager.getLangByKey(LanguageKeys.MENU_DETECTREPORT));
			quickAction.addActionItem(detectMsgItem);
		}

		if (canViewRallyInfo)
		{
			ActionItem rallyMsgItem = new ActionItem(ID_VIEW_RALLY_INFO, LanguageManager.getLangByKey(LanguageKeys.MENU_VIEW_RALLY_INFO));
			quickAction.addActionItem(rallyMsgItem);
		}

		if (canViewLotteryShare)
		{
			ActionItem lotteryShareMsgItem = new ActionItem(ID_VIEW_LOTTERY_SHARE, LanguageManager.getLangByKey(LanguageKeys.MENU_VIEW));
			quickAction.addActionItem(lotteryShareMsgItem);
		}

		if (canViewAllianceTaskShare)
		{
			ActionItem allianceTaskShareMsgItem = new ActionItem(ID_VIEW_ALLIANCETASK_SHARE,
					LanguageManager.getLangByKey(LanguageKeys.MENU_VIEW_TASK));
			quickAction.addActionItem(allianceTaskShareMsgItem);
		}

		if (canTranlate)
		{
			if (hasTranslated)
			{
				ActionItem originalItem = new ActionItem(ID_ORIGINAL_LANGUAGE, LanguageManager.getLangByKey(LanguageKeys.MENU_ORIGINALLAN));
				quickAction.addActionItem(originalItem);
			}
			else
			{
				ActionItem translateItem = new ActionItem(ID_TRANSLATE, LanguageManager.getLangByKey(LanguageKeys.MENU_TRANSLATE));
				quickAction.addActionItem(translateItem);
			}
		}

		if (canReportTranslate)
		{
			ActionItem notUnderstandItem = new ActionItem(ID_TRANSLATE_NOT_UNDERSTAND,
					LanguageManager.getLangByKey(LanguageKeys.MENU_REPORT_CHAT_TRANSLATION));
			quickAction.addActionItem(notUnderstandItem);
		}

		ActionItem prevItem = new ActionItem(ID_COPY, LanguageManager.getLangByKey(LanguageKeys.MENU_COPY));
		quickAction.addActionItem(prevItem);

		if (canSendMail)
		{
			ActionItem searchItem = new ActionItem(ID_SEND_MAIL, LanguageManager.getLangByKey(LanguageKeys.MENU_SENDMSG));
			quickAction.addActionItem(searchItem);
		}

		if (canSayHello)
		{
			String content = LanguageManager.getLangByKey(LanguageKeys.MENU_SAY_HELLO);
			if (StringUtils.isEmpty(content) || content.equals(LanguageKeys.MENU_SAY_HELLO))
				content = "Say Hello";
			ActionItem sayHelloItem = new ActionItem(ID_SAY_HELLO, content);
			quickAction.addActionItem(sayHelloItem);
		}

		if (canBlock)
		{
			ActionItem blockItem = new ActionItem(ID_BLOCK, LanguageManager.getLangByKey(LanguageKeys.MENU_SHIELD));
			quickAction.addActionItem(blockItem);
		}
		if (canUnBlock)
		{
			ActionItem unBlockItem = new ActionItem(ID_UNBLOCK, LanguageManager.getLangByKey(LanguageKeys.MENU_UNSHIELD));
			quickAction.addActionItem(unBlockItem);
		}

		if (canBan)
		{
			ActionItem banItem = new ActionItem(ID_BAN, LanguageManager.getLangByKey(LanguageKeys.MENU_BAN));
			quickAction.addActionItem(banItem);
		}
		if (canUnBan)
		{
			ActionItem unBanItem = new ActionItem(ID_UNBAN, LanguageManager.getLangByKey(LanguageKeys.MENU_UNBAN));
			quickAction.addActionItem(unBanItem);
		}

		if (canInvite)
		{
			ActionItem actionItem = new ActionItem(ID_INVITE, LanguageManager.getLangByKey(LanguageKeys.MENU_INVITE));
			quickAction.addActionItem(actionItem);
		}

		if (canJoinAlliance)
		{
			ActionItem nextItem = new ActionItem(ID_JOIN_ALLIANCE, LanguageManager.getLangByKey(LanguageKeys.MENU_JOIN));
			quickAction.addActionItem(nextItem);
		}
		if (canReportHeadImg)
		{
			ActionItem nextItem = new ActionItem(ID_REPORT_HEAD_IMG, LanguageManager.getLangByKey(LanguageKeys.MENU_REPORT_HEADIMG));
			quickAction.addActionItem(nextItem);
		}
		if (canReportContent)
		{
			ActionItem nextItem = new ActionItem(ID_REPORT_PLAYER_CHAT, LanguageManager.getLangByKey(LanguageKeys.MENU_REPORT_PLAYER_CHAT));
			quickAction.addActionItem(nextItem);
		}

		return quickAction;
	}
}
