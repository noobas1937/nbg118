package com.elex.chatservice.view;

import net.londatiga.android.ActionItem;
import net.londatiga.android.QuickAction;
import android.app.Activity;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserManager;

public class ChatQuickActionFactory {
	//action id
	public static final int ID_JOIN_ALLIANCE   = 1;
	public static final int ID_COPY     = 2;
	public static final int ID_SEND_MAIL = 3;
	public static final int ID_VIEW_PROFILE   = 4;
	public static final int ID_BAN  = 5;	
	public static final int ID_UNBAN  = 8;	
	public static final int ID_TRANSLATE  = 6;
	public static final int ID_ORIGINAL_LANGUAGE  = 7;
	public static final int ID_VIEW_BATTLE_REPORT  = 9;
	public static final int ID_INVITE  = 10;
	public static final int ID_BLOCK  = 11;
	public static final int ID_UNBLOCK  = 12;
	public static final int ID_VIEW_DETECT_REPORT  = 13;
	public static final int ID_REPORT_HEAD_IMG  = 14;
	public static final int ID_VIEW_EQUIPMENT  = 15;
//	public static final int ID_OK     = 6;
	
//	private static Activity activity;
	
	public static QuickAction createQuickAction(final Activity activity, MsgItem item)
	{
		boolean canTranlate=!item.isTranlateDisable();
		boolean canViewEquip = item.isEquipMessage();
		boolean hasTranslated=item.hasTranslated && (canTranlate || item.isTranslatedByForce);
		boolean canJoinAlliance = item.isInAlliance() && !item.isSelfMsg&& !item.isTipMsg() && UserManager.getInstance().getCurrentUser().allianceId.equals("") && !ChatServiceController.isInMailDialog() && !item.isSystemHornMsg();// && !UserManager.getInstance().getCurrentUser().allianceId.equals(item.asn)
		boolean canSendMail = !item.isSelfMsg && !item.isTipMsg() && !ChatServiceController.isInMailDialog() && !item.isSystemHornMsg();
		boolean canBlock =!item.isHornMessage() && !item.isSelfMsg && !item.isTipMsg()&& !UserManager.getInstance().isInRestrictList(item.getName(),UserManager.BLOCK_LIST);
		boolean canUnBlock =!item.isHornMessage() && !item.isSelfMsg && !item.isTipMsg()&& UserManager.getInstance().isInRestrictList(item.getName(),UserManager.BLOCK_LIST);
		boolean canBan =!item.isHornMessage() && !item.isSelfMsg && !item.isTipMsg()&& !UserManager.getInstance().isInRestrictList(item.getName(),UserManager.BAN_LIST)&& (UserManager.getInstance().getCurrentUser().mGmod>1) && !ChatServiceController.isInMailDialog();
		boolean canUnBan = !item.isHornMessage() &&!item.isSelfMsg && !item.isTipMsg()&& UserManager.getInstance().isInRestrictList(item.getName(),UserManager.BAN_LIST) && (UserManager.getInstance().getCurrentUser().mGmod==3) && !ChatServiceController.isInMailDialog();
		boolean canViewBattleReport=!item.isHornMessage() &&(item.isBattleReport() && !UserManager.getInstance().getCurrentUser().allianceId.equals("")) && !ChatServiceController.isInMailDialog();
		boolean canViewDetectReport=!item.isHornMessage() &&(item.isDetectReport()&& !UserManager.getInstance().getCurrentUser().allianceId.equals("")) && !ChatServiceController.isInMailDialog();
		boolean canInvite=!item.isHornMessage() &&item.getASN().equals("") &&!UserManager.getInstance().getCurrentUser().allianceId.equals("") && UserManager.getInstance().getCurrentUser().allianceRank>=3 && !item.isSelfMsg && !item.isTipMsg() && !ChatServiceController.isInMailDialog();
		boolean canReportHeadImg = item.isCustomHeadImage();
		QuickAction quickAction = ChatQuickActionFactory.getQuickAction(activity, QuickAction.HORIZONTAL, canJoinAlliance, canSendMail, canViewBattleReport,canViewDetectReport, canTranlate, hasTranslated, canBlock, canUnBlock,canBan,canUnBan,canInvite,canReportHeadImg,canViewEquip);
		return quickAction;
	}
	private static QuickAction getQuickAction(final Activity activity, 
												int orientation, 
												boolean canJoinAlliance, 
												boolean canSendMail, 
												boolean canViewBattleReport, 
												boolean canViewDetectReport,
												boolean canTranlate, 
												boolean hasTranslated,
												boolean canBlock,
												boolean canUnBlock,
												boolean canBan,
												boolean canUnBan,
												boolean canInvite,
												boolean canReportHeadImg,
												boolean canViewEquip)
	{
//		ChatQuickActionFactory.activity = activity;
		
		QuickAction quickAction = actuallyCreateQuickAction(activity, 
															orientation, 
															canJoinAlliance, 
															canSendMail, 
															canViewBattleReport,
															canViewDetectReport,
															canTranlate, 
															hasTranslated, 
															canBlock, 
															canUnBlock,
															canBan,
															canUnBan,
															canInvite,
															canReportHeadImg,
															canViewEquip,
															0);

		if(orientation == QuickAction.HORIZONTAL && quickAction.isWiderThanScreen())
		{
			quickAction = ChatQuickActionFactory.actuallyCreateQuickAction(activity, 
																		QuickAction.VERTICAL, 
																		canJoinAlliance, 
																		canSendMail, 
																		canViewBattleReport, 
																		canViewDetectReport,
																		canTranlate, 
																		hasTranslated, 
																		canBlock, 
																		canUnBlock,
																		canBan,
																		canUnBan,
																		canInvite,
																		canReportHeadImg,
																		canViewEquip,
																		quickAction.getMaxItemWidth());
		}
        
		return quickAction;
	}
	
	private static QuickAction actuallyCreateQuickAction(final Activity activity, 
														int orientation, 
														boolean canJoinAlliance, 
														boolean canSendMail, 
														boolean canViewBattleReport, 
														boolean canViewDetectReport,
														boolean canTranlate, 
														boolean hasTranslated,
														boolean canBlock,
														boolean canUnBlock,
														boolean canBan,
														boolean canUnBan,
														boolean canInvite,
														boolean canReportHeadImg,
														boolean canViewEquip,
														int maxItemWidth)
	{
		//create QuickAction. Use QuickAction.VERTICAL or QuickAction.HORIZONTAL param to define layout 
        //orientation
		final QuickAction quickAction = new QuickAction(activity, orientation);
		
		if(orientation == QuickAction.VERTICAL && maxItemWidth > 0)
		{
			quickAction.maxItemWidth = maxItemWidth;
		}
		
		if(ConfigManager.getInstance().scaleFontandUI && ConfigManager.scaleRatio > 0){
			quickAction.scaleRatio = ConfigManager.scaleRatio;
		}
		
		if(canViewEquip)
		{
			ActionItem nextItem 	= new ActionItem(ID_VIEW_EQUIPMENT, LanguageManager.getLangByKey(LanguageKeys.MENU_VIEW_EQUIPMENT));
	        quickAction.addActionItem(nextItem);
		}
		
		if(canViewBattleReport)
        {
        	ActionItem battleMsgItem 	= new ActionItem(ID_VIEW_BATTLE_REPORT, LanguageManager.getLangByKey(LanguageKeys.MENU_BATTLEREPORT));
        	quickAction.addActionItem(battleMsgItem);
        }
        
        if(canViewDetectReport)
        {
        	ActionItem detectMsgItem 	= new ActionItem(ID_VIEW_DETECT_REPORT, LanguageManager.getLangByKey(LanguageKeys.MENU_DETECTREPORT));
        	quickAction.addActionItem(detectMsgItem);
        }
        
//      if(canTranlate)
//      {	
      	if(hasTranslated){
          	ActionItem originalItem=new ActionItem(ID_ORIGINAL_LANGUAGE, LanguageManager.getLangByKey(LanguageKeys.MENU_ORIGINALLAN));
          	quickAction.addActionItem(originalItem);
          }
      	else
      	{
      		ActionItem translateItem=new ActionItem(ID_TRANSLATE, LanguageManager.getLangByKey(LanguageKeys.MENU_TRANSLATE));
      		quickAction.addActionItem(translateItem);
      	}
//      }
      	
      	ActionItem prevItem 	= new ActionItem(ID_COPY, LanguageManager.getLangByKey(LanguageKeys.MENU_COPY));
		quickAction.addActionItem(prevItem);
		
		if(canSendMail){
	        ActionItem searchItem 	= new ActionItem(ID_SEND_MAIL, LanguageManager.getLangByKey(LanguageKeys.MENU_SENDMSG));
	        quickAction.addActionItem(searchItem);
		}
		
		if(canBlock){
			ActionItem blockItem 	= new ActionItem(ID_BLOCK, LanguageManager.getLangByKey(LanguageKeys.MENU_SHIELD));
		    quickAction.addActionItem(blockItem);
		}
	    if(canUnBlock){
	    	ActionItem unBlockItem 	= new ActionItem(ID_UNBLOCK, LanguageManager.getLangByKey(LanguageKeys.MENU_UNSHIELD));
		    quickAction.addActionItem(unBlockItem);
		}
		
		if(canBan){
	        ActionItem banItem 	= new ActionItem(ID_BAN, LanguageManager.getLangByKey(LanguageKeys.MENU_BAN));
	        quickAction.addActionItem(banItem);
        }
        if(canUnBan){
	        ActionItem unBanItem 	= new ActionItem(ID_UNBAN, LanguageManager.getLangByKey(LanguageKeys.MENU_UNBAN));
	        quickAction.addActionItem(unBanItem);
        }
		
		if(canInvite)
		{
			ActionItem actionItem=new ActionItem(ID_INVITE,LanguageManager.getLangByKey(LanguageKeys.MENU_INVITE));
			quickAction.addActionItem(actionItem);
		}
	    
		if(canJoinAlliance){
			ActionItem nextItem 	= new ActionItem(ID_JOIN_ALLIANCE, LanguageManager.getLangByKey(LanguageKeys.MENU_JOIN));
	        quickAction.addActionItem(nextItem);
		}
		if(canReportHeadImg){
			ActionItem nextItem 	= new ActionItem(ID_REPORT_HEAD_IMG, LanguageManager.getLangByKey(LanguageKeys.MENU_REPORT_HEADIMG));
	        quickAction.addActionItem(nextItem);
		}
		
//        ActionItem infoItem 	= new ActionItem(ID_VIEW_PROFILE, ChatServiceController.getLangByKey(LanguageKeys.MENU_VIEWPLAYER));
//        quickAction.addActionItem(infoItem);
        
		quickAction.setOnDismissListener(new QuickAction.OnDismissListener() {			
			@Override
			public void onDismiss() {
//				Toast.makeText(activity.getApplicationContext(), "Dismissed", Toast.LENGTH_SHORT).show();
			}
		});
		
		return quickAction;
	}
}
