//
//  CSChatMenuItemFactory.m
//  IF
//
//  Created by 张彦涛 on 15/12/23.
//
//

#import "CSChatMenuItemFactory.h"
#import "CSMessageModel.h"
#import "CSMessage.h"
#import "UserManager.h"
#import "NSUserInfo.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
@implementation CustemMenuItem



@end

@implementation CSChatMenuItemFactory




+(NSMutableArray *)addMenuItemWithChatMsgModel:(CSMessageModel *)vChatMsgModel andWithTarget:(UIViewController*)vc andWithSelect:(SEL)vSel{
    NSMutableArray *menuArray =[NSMutableArray   array ];
    if (!vChatMsgModel.isSelfSender) {
        if ((vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal || vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker )&& vChatMsgModel.isTranslatLabelShow == NO){
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"105326"];
            //显示译文按钮
           CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105326=翻译
//            CustemMenuItem *tempMenuItem =   [CustemMenuItem itemWithTitle:@"翻译" target:vc action:vSel];
            
            tempMenuItem.menuItemType = CSMenuItemType_translate;
            [menuArray addObject:tempMenuItem];
            
        }else if ((vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal || vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker )&& vChatMsgModel.isTranslatLabelShow == YES ){
            //显示原文按钮
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"105322"];
             CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105322=原文
//            CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"原文" target:vc  action:vSel];
            tempMenuItem.menuItemType = CSMenuItemType_showOriginal;
            [menuArray addObject:tempMenuItem];
        }
        

        if ((CSChannelType )vChatMsgModel.message.channelType == CSChannelType_Country || (CSChannelType )vChatMsgModel.message.channelType == CSChannelType_Alliance){
            if ( (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal || vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker) && ([UserManager sharedUserManager].currentUser.mGmod >= 1 && [UserManager sharedUserManager].currentUser.mGmod <= 9) && ![[UserManager sharedUserManager] isInRestrictList:vChatMsgModel.message.uid :um_BAN_LIST]) {
                //禁言
                NSString *titleString = [NSString stringWithMultilingualWithKey:@"105207"];
                CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105207=禁言
                //            CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"禁言" target:vc  action:vSel];
                tempMenuItem.menuItemType = CSMenuItemType_banned;
                [menuArray addObject:tempMenuItem];
            }else if((vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal ||vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker) && ([UserManager sharedUserManager].currentUser.mGmod >= 1 && [UserManager sharedUserManager].currentUser.mGmod <= 9) && [[UserManager sharedUserManager] isInRestrictList:vChatMsgModel.message.uid :um_BAN_LIST]){
                NSString *titleString = [NSString stringWithMultilingualWithKey:@"105208"];
                CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105208=解禁
                //            CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"禁言" target:vc  action:vSel];
                tempMenuItem.menuItemType = CSMenuItemType_closedBanned;
                [menuArray addObject:tempMenuItem];
            }

        }
        
        
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal && ![[UserManager sharedUserManager] isInRestrictList:vChatMsgModel.message.uid :um_BLOCK_LIST]) {
            //屏蔽
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"105312"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105312=屏蔽
            tempMenuItem.menuItemType = CSMenuItemType_block;
            [menuArray addObject:tempMenuItem];
            
        }else if(vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal && [[UserManager sharedUserManager] isInRestrictList:vChatMsgModel.message.uid :um_BLOCK_LIST]){
            //解除屏蔽
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"105315"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105315=解除屏蔽
            tempMenuItem.menuItemType = CSMenuItemType_closedBlock;
            [menuArray addObject:tempMenuItem];
        }
        

        
        NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:vChatMsgModel.message.uid];
        if (user ) {
            if (user.headPicVer> 0) {
                if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal || vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker) {
                    //举报头像
                    NSString *titleString = [NSString stringWithMultilingualWithKey:@"105777"];
                     CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105777=举报头像
//                    CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"举报头像" target:vc  action:vSel];
                    tempMenuItem.menuItemType = CSMenuItemType_informHead;
                    [menuArray addObject:tempMenuItem];
                }

            }
        }
        
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal || vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker) {
            //消息举报
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"105392"];
             CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105392=举报消息
//            CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"举报消息" target:vc  action:vSel];
            tempMenuItem.menuItemType =CSMenuItemType_informMsg;
            [menuArray addObject:tempMenuItem];
        }
    }
    
    if(vChatMsgModel.messageBodyType == CSMessageBodyType_Text_Normal ){
        //复制
        NSString *titleString = [NSString stringWithMultilingualWithKey:@"105050"];
        CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//105050=复制
        
        tempMenuItem.menuItemType = CSMenuItemType_copy;
        [menuArray addObject:tempMenuItem];
        
    }
    
    
    if ((CSChannelType )vChatMsgModel.message.channelType == CSChannelType_Country || (CSChannelType )vChatMsgModel.message.channelType == CSChannelType_Alliance){
    
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_EquipShare) {
            //装备分享
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"111665"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//111665=查看属性
            //        CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"装备分享" target:vc  action:vSel];
            tempMenuItem.menuItemType = CSMenuItemType_equipShare;
            [menuArray addObject:tempMenuItem];
            
        }
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_FightReport || vChatMsgModel.messageBodyType ==CSMessageBodyType_Text_InvestigateReport) {
            //查看战斗战报   侦查战报
             NSString *titleString = [NSString stringWithMultilingualWithKey:@"115281"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//115281=查看战报
            //        CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"查看战报" target:vc  action:vSel];
            tempMenuItem.menuItemType = CSMenuItemType_fightReport;
            [menuArray addObject:tempMenuItem];
            
        }
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_TurntableShare) {
            //查看转盘分享
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"115018"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//115018 =查看
            //        CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"查看转盘分享" target:vc  action:vSel];
            tempMenuItem.menuItemType = CSMenuItemType_rotaryTableShare;
            [menuArray addObject:tempMenuItem];
        }
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_AllianceTask) {
            //查看联盟任务
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"134058"];
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];//134058=接取任务
            //        CustemMenuItem *tempMenuItem = [CustemMenuItem itemWithTitle:@"查看联盟任务" target:vc  action:vSel];
            tempMenuItem.menuItemType = CSMenuItemType_allianceTask;
            [menuArray addObject:tempMenuItem];
        }
        if (vChatMsgModel.messageBodyType == CSMessageBodyType_Text_AllianceConciles) {
            //联盟集结
            NSString *titleString = [NSString stringWithMultilingualWithKey:@"132119"];//132119=查看集结信息
            CustemMenuItem *tempMenuItem =  [[CustemMenuItem alloc]initWithTitle:titleString action:vSel];
 
            tempMenuItem.menuItemType = CSMenuItemType_AllianceConciles;
            [menuArray addObject:tempMenuItem];
        }

    }
    
    return menuArray;
    
}
@end
