//
//  ChatServiceBridge.m
//  IF
//
//  Created by mzc on 15/4/3.
//
//

#import "ServiceInterface.h"
#import "ChatViewController.h"
#import "UserManager.h"
#import "ChatAllianceTableViewController.h"
#import "ChatCountriesTableViewController.h"
#import "MsgMessage.h"
#include "ChatServiceCocos2dx.h"
#import "ChatServiceController.h"
#import "BDKViewController.h"

#import "AppDelegate.h"

#import "AppController.h"
#import "ChannelManager.h"
#import "ChattingVC.h"
#import "LanguageManager.h"
#import "BBSIOSViewController.h"
#import "UITableViewController+Extension.h"
#import "CSConvertViewManager.h"
//屏幕适配
#define ScreenHeight [[UIScreen mainScreen] bounds].size.height
#define ScreenWidth [[UIScreen mainScreen] bounds].size.width

@interface ServiceInterface()
@property (nonatomic,strong) BDKViewController *bdk;

@end


@implementation ServiceInterface

-(void) initView
{
    
    if(!self.mailViewController ){
        self.mailViewController = [[MailViewController alloc]init];
        self.mailViewController.chatMailTableViewType = ChatMailTableVCType_ShowAddMemButton;
        if (self.vc_win) {
            [self.vc_win addSubview:self.mailViewController.view];
        }
    }
    
    if(!self.chatViewController){
        self.chatViewController = [[ChatViewController alloc]init];
        if (self.vc_win) {
            [self.vc_win addSubview:self.chatViewController.view];
        }
    }
    
}

-(void)showChatIOSFrom2dx:(int)chatType withAddState:(OpenChatState)ocs
{
   // 顺序不能换
    if (!ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        [self initView];
    }
    if (chatType == IOS_CHANNEL_TYPE_COUNTRY ||chatType == IOS_CHANNEL_TYPE_ALLIANCE) {
        [self _initCountChatVC:chatType];
    }else if(chatType == IOS_CHANNEL_TYPE_USER || chatType == IOS_CHANNEL_TYPE_CHATROOM){
        [self _initMailVC:chatType];
    }
    if  (!ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
             [self showChatViewIOS];
    }
    
    
 
    
}

-(void)reloadDataChatView
{
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView reloadData];
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView reloadData];
}

-(void)reloadDataMailView
{
    [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView reloadData];
}

-(void)openBBS
{
    if (self.bbsIOSViewController) {
        self.vc_win.hidden = NO;
        [self.vc_win bringSubviewToFront:self.bbsIOSViewController.view];
    }else{
        self.bbsIOSViewController= [[BBSIOSViewController alloc]init];
        if (self.vc_win) {
            self.vc_win.hidden = NO;
            [self.vc_win addSubview:self.bbsIOSViewController.view];
            [self.vc_win bringSubviewToFront:self.bbsIOSViewController.view];
        }
    }
}

-(void)loadingChatVC{
    ChattingVC *chatVC =[[ChattingVC alloc]init];
    self.chatRootWindow.rootViewController = chatVC;
    self.chatRootWindow.hidden = NO;
}

-(void)_initCountChatVC:(int)chatType{
 
        
        if  (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
            BOOL isChatVCPageJump = NO;
            ChattingVC *chatVC;
            if (self.chatRootWindow.rootViewController && [self.chatRootWindow.rootViewController isKindOfClass:[UINavigationController class]]){
                
                UIViewController *vc = [(UINavigationController *)self.chatRootWindow.rootViewController visibleViewController];
                if ([vc isKindOfClass:[ChattingVC class]]){
                     chatVC = vc;
                    isChatVCPageJump = YES;
                }else{
                    chatVC =[[ChattingVC alloc]init];
                }
               
            }else{
               chatVC =[[ChattingVC alloc]init];
            }
            
            
            if  (chatType == IOS_CHANNEL_TYPE_COUNTRY){
                chatVC.chatVCShowTableType = ChatVCShowTableType_left;
            }else{
                chatVC.chatVCShowTableType = ChatVCShowTableType_right;
            }
            
  
            ChatChannel *countryCC = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            chatVC.countryChannel = countryCC;
            self.chatViewController.countriesTableViewController.currentChannel = countryCC;
            if (countryCC.msgList.count <20){
                [countryCC gettingFirstMsg];
            }else{
                
                if ([countryCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                    [countryCC.channelDelegate showNewMsg:countryCC];
                }
            }
            NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
            
            if (allianceId.length> 0) {
                ChatChannel *  allianceCC = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
                chatVC.allianceChannel =allianceCC;
                
                if (allianceCC.msgList.count <20){
                    [allianceCC gettingFirstMsg];
                }else{
                    
                    if ([allianceCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                        [allianceCC.channelDelegate showNewMsg:allianceCC];
                    }
                }
            }
            
            
            if  (isChatVCPageJump){
                //跳转
                self.chatRootWindow.hidden = NO;
                CCDirector::sharedDirector()->setVisitFlag(false);
            }else{
                //新建
                UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:chatVC   ];
                chatVC.navigationController.navigationBar.hidden = YES;
                self.chatRootWindow.rootViewController = nav;
                self.chatRootWindow.hidden = NO;
                CCDirector::sharedDirector()->setVisitFlag(false);
            }
            
            
        }
        else{
            ChatChannel *countryCC = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            self.chatViewController.countriesTableViewController.currentChannel = countryCC;
            if (countryCC.msgList.count <20){
                [countryCC gettingFirstMsg];
            }else{
                //            if ([countryCC refreshingMsgListAtLastStatus]){
                //                [countryCC msgArrayFormatToStartState];
                //            }
                if ([countryCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                    [countryCC.channelDelegate showNewMsg:countryCC];
                }
                
            }
            NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
            if (allianceId.length> 0) {
                ChatChannel *allianceCC = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
                self.chatViewController.allianceTableViewController.currentChannel = allianceCC;
                
                if (allianceCC.msgList.count <20){
                    [allianceCC gettingFirstMsg];
                }else{
                    //                if ([allianceCC refreshingMsgListAtLastStatus]){
                    //                    [allianceCC msgArrayFormatToStartState];
                    //           	 }
                    if ([allianceCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                        [allianceCC.channelDelegate showNewMsg:allianceCC];
                    }
                }
            }
            
            if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                
                [self.chatViewController.topUIView selectASN];
                [self.chatViewController isShowJionAllance];
            }else{
                [self.chatViewController.topUIView selectCoun];
                
                [self.chatViewController.view sendSubviewToBack:self.chatViewController.jionAllanceView];
                [self.chatViewController.view bringSubviewToFront:self.chatViewController.countriesTableViewController.view];
                if(ChatServiceCocos2dx::m_isNoticItemUsed){
                    [self.chatViewController.keyBordView openRadio];
                    ChatServiceCocos2dx::m_isNoticItemUsed = false;
                }else{
                    [self.chatViewController.keyBordView closeRadio];
                }
            }
            
            [self.vc_win bringSubviewToFront:self.chatViewController.view];
            
        }
        
 

}
-(void)_initMailVC:(int)chatType{
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable )
    {
        ChatChannel *cc= nil;
        if  (chatType == IOS_CHANNEL_TYPE_CHATROOM){
            self.m_curChatType = IOS_CHANNEL_TYPE_CHATROOM;
            cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:[UserManager sharedUserManager].currentMail.opponentUid];
            
            [cc gettingFirstMsg];
            DVLog(@"chatChannel.memberUidArray %@",cc.memberUidArray);
            
            
        }else{
            NSString *channelString;
            if  ([UserManager sharedUserManager].currentMail.type == 23 || [UserManager sharedUserManager].currentMail.type == 24){
                //发送mod
                channelString  = [UserManager sharedUserManager].currentMail.opponentUid;
                if ([channelString hasSuffix:@"@mod"]){
                    
                }else{
                    channelString = [channelString stringByAppendingString:@"@mod"];
                }
            }else{
                channelString  = [UserManager sharedUserManager].currentMail.opponentUid;
            }
            [UserManager sharedUserManager].currentMail.opponentUid = channelString;
           cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:channelString];
            if (cc == nil){
                [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:[UserManager sharedUserManager].currentMail.opponentUid];
                cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:[UserManager sharedUserManager].currentMail.opponentUid];
            }
            
            NSString *userUid = [cc.channelID stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
            NSUserInfo *user =  [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:userUid];
            if (user){
                cc.nameString =user.userName ;
            }
            
        }
            CSMailChattingVC *mailChattingVC=[[CSMailChattingVC alloc]init];
            mailChattingVC.currentChatChannel = cc;
            
            UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:mailChattingVC];
            mailChattingVC.navigationController.navigationBarHidden = YES;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nav;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
            CCDirector::sharedDirector()->setVisitFlag(false);
         
        
        
    }
    else{
        self.isInMailDialog = TRUE;
        
        if (chatType == IOS_CHANNEL_TYPE_CHATROOM)
        {
            self.m_curChatType = IOS_CHANNEL_TYPE_CHATROOM;
            ChatChannel *cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:[UserManager sharedUserManager].currentMail.opponentUid];
            
            [cc gettingFirstMsg];
            DVLog(@"chatChannel.memberUidArray %@",cc.memberUidArray);
            
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController setCurrentChatChannel:cc];
            [[ServiceInterface serviceInterfaceSharedManager] settingChatRoomName:cc.customName];
        }else{
            self.m_curChatType = IOS_CHANNEL_TYPE_USER;
            //设置邮件上方标题处。对方人物名称
            self.mailViewController.topUIView.titlePlayerName.text = [UserManager sharedUserManager].currentMail.opponentName;
            
            NSString *channelID = nil;
            if ([UserManager sharedUserManager].currentMail.type == 23 || [UserManager sharedUserManager].currentMail.type == 24) {
                channelID = [UserManager sharedUserManager].currentMail.opponentUid;
                channelID = [channelID stringByAppendingString:@"@mod"];
            }else{
                channelID = [UserManager sharedUserManager].currentMail.opponentUid;
            }
            
            ChannelManager *cm = [ChannelManager sharedChannelManager];
            if (![[cm.channel_map allKeys] containsObject:channelID]) {
                [cm createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:channelID];
            }
            ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:channelID];
            cc.customName = [UserManager sharedUserManager].currentMail.opponentName;
            [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.currentChatChannel = cc;
            
            [cc gettingFirstMsg];
            
        }
        
        
        [self.vc_win bringSubviewToFront:self.mailViewController.view];
    }
}

-(void) selectOpenView:(int)chatType
{
    if(chatType == IOS_CHANNEL_TYPE_USER || chatType == IOS_CHANNEL_TYPE_CHATROOM)
    {
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable )
        {
            NSString *channelString;
            if  ([UserManager sharedUserManager].currentMail.type == 23 || [UserManager sharedUserManager].currentMail.type == 24){
                //发送mod
              channelString  = [UserManager sharedUserManager].currentMail.opponentUid;
                if ([channelString hasSuffix:@"@mod"]){
                    
                }else{
                    channelString = [channelString stringByAppendingString:@"@mod"];
                }
            }else{
                 channelString  = [UserManager sharedUserManager].currentMail.opponentUid;
            }
            
            ChatChannel *cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:channelString];
            if (cc == nil){
                [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:[UserManager sharedUserManager].currentMail.opponentUid];
                cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:[UserManager sharedUserManager].currentMail.opponentUid];
            }
            NSUserInfo *user =  [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:cc.channelID];
            cc.nameString =[UserManager sharedUserManager].currentMail.opponentName; ;
            CSMailChattingVC *mailChattingVC=[[CSMailChattingVC alloc]init];
            mailChattingVC.currentChatChannel = cc;
            
            UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:mailChattingVC];
            mailChattingVC.navigationController.navigationBarHidden = YES;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nav;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
                CCDirector::sharedDirector()->setVisitFlag(false);
            
        }
        else{
            self.isInMailDialog = TRUE;
            
            if (chatType == IOS_CHANNEL_TYPE_CHATROOM)
            {
                self.m_curChatType = IOS_CHANNEL_TYPE_CHATROOM;
                ChatChannel *cc =[[ChannelManager sharedChannelManager].channel_map objectForKey:[UserManager sharedUserManager].currentMail.opponentUid];
                
                [cc gettingFirstMsg];
                DVLog(@"chatChannel.memberUidArray %@",cc.memberUidArray);
                
                [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController setCurrentChatChannel:cc];
                [[ServiceInterface serviceInterfaceSharedManager] settingChatRoomName:cc.customName];
            }else{
                self.m_curChatType = IOS_CHANNEL_TYPE_USER;
                //设置邮件上方标题处。对方人物名称
                self.mailViewController.topUIView.titlePlayerName.text = [UserManager sharedUserManager].currentMail.opponentName;
                
                NSString *channelID = nil;
                if ([UserManager sharedUserManager].currentMail.type == 23 || [UserManager sharedUserManager].currentMail.type == 24) {
                    channelID = [UserManager sharedUserManager].currentMail.opponentUid;
                    channelID = [channelID stringByAppendingString:@"@mod"];
                }else{
                    channelID = [UserManager sharedUserManager].currentMail.opponentUid;
                }
                
                ChannelManager *cm = [ChannelManager sharedChannelManager];
                if (![[cm.channel_map allKeys] containsObject:channelID]) {
                    [cm createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:channelID];
                }
                ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:channelID];
                cc.customName = [UserManager sharedUserManager].currentMail.opponentName;
                [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.currentChatChannel = cc;
                
                [cc gettingFirstMsg];
                
            }
            
            
            [self.vc_win bringSubviewToFront:self.mailViewController.view];
        }
    }
    else if(chatType == IOS_CHANNEL_TYPE_COUNTRY || chatType == IOS_CHANNEL_TYPE_ALLIANCE)
    {
        
        if  (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
            ChattingVC *chatVC =[[ChattingVC alloc]init];
            if  (chatType == IOS_CHANNEL_TYPE_COUNTRY){
                 chatVC.chatVCShowTableType = ChatVCShowTableType_left;
            }else{
                chatVC.chatVCShowTableType = ChatVCShowTableType_right;
            }
           
           
            
            ChatChannel *countryCC = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            chatVC.countryChannel = countryCC;
            self.chatViewController.countriesTableViewController.currentChannel = countryCC;
            if (countryCC.msgList.count <20){
                [countryCC gettingFirstMsg];
            }else{
 
                if ([countryCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                    [countryCC.channelDelegate showNewMsg:countryCC];
                }
            }
            NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
           
            if (allianceId.length> 0) {
               ChatChannel *  allianceCC = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
                 chatVC.allianceChannel =allianceCC;

                if (allianceCC.msgList.count <20){
                    [allianceCC gettingFirstMsg];
                }else{
 
                    if ([allianceCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                        [allianceCC.channelDelegate showNewMsg:allianceCC];
                    }
                }
            }
            


            UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:chatVC   ];
            chatVC.navigationController.navigationBar.hidden = YES;
            self.chatRootWindow.rootViewController = nav;
            self.chatRootWindow.hidden = NO;
             CCDirector::sharedDirector()->setVisitFlag(false);
            
        }
        else{
            ChatChannel *countryCC = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            self.chatViewController.countriesTableViewController.currentChannel = countryCC;
            if (countryCC.msgList.count <20){
                [countryCC gettingFirstMsg];
            }else{
                //            if ([countryCC refreshingMsgListAtLastStatus]){
                //                [countryCC msgArrayFormatToStartState];
                //            }
                if ([countryCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                    [countryCC.channelDelegate showNewMsg:countryCC];
                }
                
            }
            NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
            if (allianceId.length> 0) {
                ChatChannel *allianceCC = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
                self.chatViewController.allianceTableViewController.currentChannel = allianceCC;
                
                if (allianceCC.msgList.count <20){
                    [allianceCC gettingFirstMsg];
                }else{
                    //                if ([allianceCC refreshingMsgListAtLastStatus]){
                    //                    [allianceCC msgArrayFormatToStartState];
                    //           	 }
                    if ([allianceCC.channelDelegate respondsToSelector:@selector(showNewMsg:)]) {
                        [allianceCC.channelDelegate showNewMsg:allianceCC];
                    }
                }
            }
            
            if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                
                [self.chatViewController.topUIView selectASN];
                [self.chatViewController isShowJionAllance];
            }else{
                [self.chatViewController.topUIView selectCoun];
                
                [self.chatViewController.view sendSubviewToBack:self.chatViewController.jionAllanceView];
                [self.chatViewController.view bringSubviewToFront:self.chatViewController.countriesTableViewController.view];
                if(ChatServiceCocos2dx::m_isNoticItemUsed){
                    [self.chatViewController.keyBordView openRadio];
                    ChatServiceCocos2dx::m_isNoticItemUsed = false;
                }else{
                    [self.chatViewController.keyBordView closeRadio];
                }
            }
            
            [self.vc_win bringSubviewToFront:self.chatViewController.view];

        }
        
    }
    
}
/*
-(void)refreshData:(ResponseMsgType) rmt withAdd:(ChatChannel *) cc;
{
    
    if (![ServiceInterface serviceInterfaceSharedManager].mailViewController || ![ServiceInterface serviceInterfaceSharedManager].chatViewController) {
        [self initView];
        [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
    }
    
    ChatCellFrame *ccf = [cc.msgList lastObject];
    
    if (ccf.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        ChatViewController *chatViewVC = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
        [[ServiceInterface serviceInterfaceSharedManager] tabelViewReloadData:chatViewVC.countriesTableViewController withAdd:rmt withAdd:cc];
        if (ccf.chatMessage.isNewMsg) {
            if ([ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.isSrollNewMsg) {
                [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController tableViewScrollCurrentLine];
            }
        }
    }
    
   else  if (ccf.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
        ChatViewController *chatViewVC = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
        [[ServiceInterface serviceInterfaceSharedManager] tabelViewReloadData:chatViewVC.allianceTableViewController withAdd:rmt withAdd:cc];
        if (ccf.chatMessage.isNewMsg){
            if ([ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.isSrollNewMsg) {
                [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController tableViewScrollCurrentLine];
            }
        }
    }
    
   else if (ccf.chatMessage.channelType == IOS_CHANNEL_TYPE_USER || ccf.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM) {
       
        MailViewController *mailVC = [ServiceInterface serviceInterfaceSharedManager].mailViewController;
        [[ServiceInterface serviceInterfaceSharedManager] tabelViewReloadData:mailVC.mailTableTableViewController withAdd:rmt withAdd:cc];
        
        if (ccf.chatMessage.isNewMsg) {
            if ([ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.isSrollNewMsg) {
                [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController tableViewScrollCurrentLine];
            }
        }
        
    }
}
 */

-(void) setPlayerInfo:(int)gmod headPicVer:(int)headPicVer customHeadImageUrl:(NSString*)customHeadImageUrl name:(NSString *)name uidStr:(NSString *)uidStr picStr:(NSString *)picStr vipStr:(NSString *)vipStr
{
    
    [UserManager sharedUserManager].currentUser.mGmod=gmod;
    [UserManager sharedUserManager].currentUser.userName=name;
    [UserManager sharedUserManager].currentUser.uid=uidStr;
    [UserManager sharedUserManager].currentUser.headPic=picStr;
    [UserManager sharedUserManager].currentUser.vip=vipStr;
    [UserManager sharedUserManager].currentUser.headPicVer = headPicVer;
}

-(void) setMailInfo:(NSString *)mailFromUid :(NSString *)mailUid :(NSString *)mailName :(int)mailType
{
    [UserManager sharedUserManager].currentMail.opponentUid = mailFromUid;
    [UserManager sharedUserManager].currentMail.myUid = mailUid;
    [UserManager sharedUserManager].currentMail.opponentName = mailName;
    [UserManager sharedUserManager].currentMail.type = mailType;
}

-(void) setPlayerAllianceInfo:(NSString *)asnStr :(NSString *)allianceIdStr :(int)alliancerank :(BOOL)isFirstJoinAlliance
{
    [UserManager sharedUserManager].currentUser.asn = asnStr;
    [[UserManager sharedUserManager].currentUser settingAllianceId:allianceIdStr] ;
    [UserManager sharedUserManager].currentUser.allianceRank = alliancerank;
    
}

//设置先上屏用户数据
-(NSMsgItem *) createChatMessage:(NSString *)msg
{
    //获取用户数据信息
    NSUserInfo *userInfo = [UserManager sharedUserManager].currentUser;
    
    NSString *ns_uid =userInfo.uid;
    bool isNewMsg = TRUE;
    int channelType = ChatServiceCocos2dx::m_channelType;
    bool isSelfMsg = TRUE;
    int post = 0;
    NSString *sendLocalTime = [NSString stringWithFormat:@"%ld", (long)[[NSDate date] timeIntervalSince1970]];
    DVLog(@" sendLocalTime   %@",sendLocalTime);
    NSMsgItem *mi = [[NSMsgItem alloc] initSend:ns_uid isNewMsg:isNewMsg isSelf:isSelfMsg channelType:channelType post:post msgStr:msg sendLocalTime:sendLocalTime];
    
    mi.vip = userInfo.vip;
    mi.asn = userInfo.asn;
    mi.name = userInfo.userName;
    mi.headPic = userInfo.headPic;
    mi.headPicVer = userInfo.headPicVer;
    mi.translateMsg = msg;
    [mi packedMsg];

    return mi;
}

///**第一次打开聊天频道显示历史数据20条*/
//-(void) firstOpenChat
//{
//    if ([UserManager sharedUserManager].currentUser.allianceId.length != 0 && ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
//        [[ChatServiceController chatServiceControllerSharedManager].gameHost requestAllanceData];
//    }else{
//        [[ChatServiceController chatServiceControllerSharedManager].gameHost requestCountriesData];
//        
//    }
//}
/**第一次打开邮件显示历史数据20条*/
//-(void) initMaiChannelData
//{
//    //对相同的fromUid第一次打开邮件 需要做滚动到最新行。在加载数据后调用滚动函数
//    [[ChatServiceController chatServiceControllerSharedManager].gameHost initChatToHistory_mail] ;
//}

-(void) flyHint:(NSString*)icon :(NSString*)titleText :(NSString*)contentText :(CGFloat)time :(CGFloat)dy :(BOOL)useDefaultIcon
{
    if (self.bdk) {
        [self.bdk removeFromParentViewController];
        [self.bdk.view removeFromSuperview];
    }
    self.bdk = [[BDKViewController alloc] init:contentText];
    if ([ServiceInterface serviceInterfaceSharedManager].chatRootWindow && [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden == NO) {
        UIViewController *recentView = nil;
        
        UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
        [nav.topViewController.view addSubview:self.bdk.view];
        [nav.topViewController.view bringSubviewToFront:self.bdk.view];
    }else{
        [self.vc_win addSubview:self.bdk.view];
    }
}

-(void) initIsLoadVariable
{
//    self.isLoadAllanceData = TRUE;
//    self.isLoadCountriesData = TRUE;
//    self.isFirstOpenIOS = TRUE;
    self.isSrollNewMsg = TRUE;
}

-(void) hideChatViewIOS
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
        [[NSNotificationCenter defaultCenter]postNotificationName:KCLOSEKEYBOARD object:nil userInfo:nil];
    }else{
        [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
        CCDirector::sharedDirector()->setVisitFlag(true);
    }
}

-(void) showChatViewIOS
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        [[CSConvertViewManager sharedMailInfoManager] chatWindowShow];
    }else{
        [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = NO;
        CCDirector::sharedDirector()->setVisitFlag(false);
    }
}

-(void) hideMailListIOS
{
    self.chatRootWindow.hidden = YES;
    CCDirector::sharedDirector()->setVisitFlag(true);
}

-(void) showMailListIOS
{
    self.chatRootWindow.hidden = NO;
    CCDirector::sharedDirector()->setVisitFlag(false);
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

-(instancetype)init
{
    self = [super init];
    if (self) {
        self.isChangeLanguage = FALSE;
        self.isInMailDialog = FALSE;
        self.m_curChatType = -1;
        self.rememberPosition = false;
//        self.isLoadCountriesData = TRUE;
//        self.isLoadAllanceData = TRUE;
//        self.isFirstOpenIOS = TRUE;
        self.isSrollNewMsg = TRUE;
        self.isReturnOpengIOS = TRUE;
        self.cs_CountryChat_OC_Native_New = ChatServiceCocos2dx::CS_CountryChat_OC_Native_New;
        [self screenAdaptation];
        self.csChatMsgQueue = dispatch_queue_create("com.chatServer.ServiceInterface", NULL);
         self.csGetUserInfoQueue = dispatch_queue_create("com.chatServer.ServiceInterface.csGetUserInfoQueue", NULL);
        UIWindow *window=[[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
        self.vc_win = window;
        UIWindow *keywindow= [[[UIApplication sharedApplication ]delegate]window];
        self.vc_win.hidden = YES;
        [keywindow addSubview:self.vc_win];
        
        
        /*zyt*/
        UIWindow *window1  =[[UIWindow alloc]init];
        window1.frame = [UIScreen mainScreen].bounds;
//            [window1 becomeKeyWindow];
            [window1 makeKeyAndVisible];
        self.chatRootWindow = window1;
        
        self.chatRootWindow.hidden = YES;
        
        [keywindow addSubview:self.chatRootWindow];
        
        ChatServiceCocos2dx::notifyChangeLanguage();

        self.isFirstDeductRadioCount = TRUE;
        self.isFirstDeductRadioMoney = TRUE;
    }
    return self;
}


+ (instancetype)serviceInterfaceSharedManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        serviceInterface = [[ServiceInterface alloc] init];
    });
    return serviceInterface;
}

+ (id)allocWithZone:(NSZone *)zone
{

    //调用dispatch_once保证在多线程中也只被实例化一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        serviceInterface = [super allocWithZone:zone];
    });
    return serviceInterface;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

-(void) setM_curChatType:(int)m_curChatType
{
    _m_curChatType = m_curChatType;
    ChatServiceCocos2dx::m_channelType = m_curChatType;
}

//屏幕适配比例
- (void)screenAdaptation{
  
    
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
        
        self.autoSizeScaleX = 1.0;
        
        self.autoSizeScaleY = 1.0;
        
        self.autoSizeScaleWidth = 1.0;
        
        self.autoSizeScaleHeight = 1.0;
        self.chatSystemSize = 25.0;
        self.chatFontSize = 30.0;
        
        self.timefontSize = 25.0;
        
        self.redPackageTimeSize = 15.0;
        
        self.userInfofontSize = 16.0;
        
        self.endtimeFontSize = 20.0;
        
        self.fontSize = 32.0;
        
        self.redEnvelopeFont = 26.0;
        
        self.redEnvelopeFont2 = 20.0;
        
        self.userInfoX = 0.10;
        
        self.font2dxSize = 28.0;
        
        self.x = 0.015;
        
        self.contentX = 0.045;
        
        self.sectionFontSize = 22;
        
        
        self.chatLabel_icon_size_height = 30;
        self.chatLabel_icon_size_width = 70;
        self.chatLabel_icon_size_height_1 = 30;
        self.chatLabel_icon_size_width_1 = 30;
        self.chatLabel_LINE_HEIGHT = 40;
        self.mail_native_nameLabelSize =23;
        self.mail_native_contentsLabelSize =22;
        self.mail_native_timeLabelSize = 22;
        self.redPackageTimePosY = 0.2;
        
    }else{
        
        self.autoSizeScaleX = 1.0;
        
        self.autoSizeScaleY = 1.0;
        
        self.autoSizeScaleWidth = 1.1;
        
        self.autoSizeScaleHeight = 1.1;
        
        self.chatFontSize = 13.0;
        
        self.chatSystemSize = 12.0;
        
        self.timefontSize = 12.0;
        
        self.redPackageTimeSize = 12.0;
        
        self.userInfofontSize = 11.0;
        
        self.endtimeFontSize = 10.0;
        
        self.fontSize = 16.0;
        
        self.userInfoX = 0.1;
        
        self.font2dxSize = 12.0;
        
        self.redEnvelopeFont = 10.5;
        
        self.redEnvelopeFont2 = 9.0;
        
        self.x = 0.015;
        
        self.contentX = 0.09;
        self.sectionFontSize = 14;
        
        self.chatLabel_icon_size_height = 20;
        self.chatLabel_icon_size_width = 40;
        self.chatLabel_icon_size_height_1 = 20;
        self.chatLabel_icon_size_width_1 = 20;
        self.chatLabel_LINE_HEIGHT = 20;
        self.mail_native_nameLabelSize =13;
        self.mail_native_contentsLabelSize =12;
        self.mail_native_timeLabelSize = 13;
        self.redPackageTimePosY = 0.25;
    }
    
}

-(void) selectAsnORCountries
{
    //判断win是否打开。
    if(self.vc_win){
        TopUIView *top = self.chatViewController.topUIView;
        ChatAllianceTableViewController *middle_asn = self.chatViewController.allianceTableViewController;
        ChatCountriesTableViewController *middle_coun = self.chatViewController.countriesTableViewController;
        KeyBordVIew *down = self.chatViewController.keyBordView;
        if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY) {
            //上
            [top selectCoun];
            //中
            middle_coun.view.hidden = NO;
            middle_asn.view.hidden = YES;
            //下
            [down showRadioView];
            [down updateStatus];
        }else{
            //上
            [top selectASN];
            //中
            middle_coun.view.hidden = YES;
            middle_asn.view.hidden = NO;
            //下
            [down hiddenRadioView];
            [down updateStatus];
        }
        
    }
}

-(void)updateChatCellFrame:(NSString *)uid
{
    
    NSArray *arrayVal = [[ChannelManager sharedChannelManager].channel_map allValues];
    if (!arrayVal) {
        return;
    }
//    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        //加入耗时操作
        for(int xx = 0  ;xx<arrayVal.count;xx++){
//        for (ChatChannel *cc in arrayVal) {
            ChatChannel *cc =arrayVal[xx];
            for(int i = 0; i < [cc.msgList count] ; i++)
            {
                ChatCellFrame *ccf = cc.msgList[i];
                
                if([ccf.chatMessage.uid isEqualToString:uid]){
                    [ccf.chatMessage updateMsg];
                }
                [ccf  chatCellFrameUpdateUserInfo:ccf.chatMessage];
            }
        }
    
//        dispatch_sync(dispatch_get_main_queue(), ^{
//            [self reloadDataAll];
//        });
        
//    });

}

-(void)reloadDataAll
{
    if ([ServiceInterface serviceInterfaceSharedManager].chatViewController &&
        [ServiceInterface serviceInterfaceSharedManager].mailViewController) {
        
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView reloadData];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView reloadData];
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView reloadData];
    }
}

-(void)settingChatRoomName:(NSString *)roomName
{
    NSString *ccId = [UserManager sharedUserManager].currentMail.opponentUid;
    ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:ccId];
    cc.customName = roomName;
    TopUIView *topUIView =  [ServiceInterface serviceInterfaceSharedManager].mailViewController.topUIView;
    topUIView.titlePlayerName.text = roomName;
}

-(void)tabelViewReloadData:(UITableViewController *)tvc withAdd:(ResponseMsgType)rmt withAdd:(ChatChannel *)cc
{
    if ([tvc isKindOfClass:[ChatAllianceTableViewController class]]) {
        ChatAllianceTableViewController *allianceVC = (ChatAllianceTableViewController *)tvc;
        allianceVC.dataSourceArray = cc.msgList;
        
        if (rmt == ResponseMsgTypeActionMSG) {
            [allianceVC endRefreshing];
            [allianceVC adjustLocation:[allianceVC gettingOffsetY]];
            [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
        }else{
            [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = TRUE;
        }
        
    }else if ([tvc isKindOfClass:[ChatCountriesTableViewController class]]){
        ChatCountriesTableViewController *countriesVC = (ChatCountriesTableViewController *)tvc;
        countriesVC.dataSourceArray = cc.msgList;
        
        if (rmt == ResponseMsgTypeActionMSG) {
            [countriesVC endRefreshing];
            [countriesVC adjustLocation:[countriesVC gettingOffsetY]];
            [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
        }else{
            [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = TRUE;
        }
        
    }else if ([tvc isKindOfClass:[ChatMailTableTableViewController class]]){
        if ([cc.channelID isEqualToString:[UserManager sharedUserManager].currentMail.opponentUid]){
            ChatMailTableTableViewController *mailVC = (ChatMailTableTableViewController *)tvc;
            ChatServiceCocos2dx::settingGroupChatMailVCChatChannel();
            
            if (rmt == ResponseMsgTypeActionMSG) {
                [mailVC endRefreshing];
                [mailVC adjustLocation:[mailVC gettingOffsetY]];
                [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
            }else{
                [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = TRUE;
            }
        }
    }
}

-(BOOL) isSystemVersion7
{
    return [[UIDevice currentDevice].systemVersion isEqualToString:@"7.1"];
}
/** 关闭原生邮件列表，对象彻底释放 */
-(void)closed_The_OC_Native_mailVC{
    CCDirector::sharedDirector()->setVisitFlag(true);
    self.chatRootWindow.rootViewController = nil;
    self.chatRootWindow.hidden = YES;
}

-(void)openMail_OC_Native_Categary_ListWith3DTouch{
    CCDirector::sharedDirector()->setVisitFlag(false);
    self.chatRootWindow.rootViewController = nil;
    ChatServiceCocos2dx::creatingMailListWith_OC_Native();
}
-(void)packageMsgCommand{
    NSString *commandStr =@"";
    if  (!ChatServiceCocos2dx::DB_MsgItem_switch){
        //GlobalData::shared()->playerInfo.getAllianceId().c_str();
        NSString *allianceID = [NSString stringWithCString:GlobalData::shared()->playerInfo.getAllianceId().c_str() encoding:4];
        if(allianceID.length> 0){
            commandStr = [commandStr stringByAppendingFormat:@"0|0|0,%@|0|1,0|0|2,0|0|4",allianceID];
        }else{
            commandStr = @"0|0|0,0|0|2,0|0|4";
        }
    }else{
        //数据库开关打开
        NSString *allianceID = [NSString stringWithCString:GlobalData::shared()->playerInfo.getAllianceId().c_str() encoding:4];
        ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingCountryChannel];
        NSMsgItem *msg = nil;
        if  (chatChannel ){
            msg =  [chatChannel gettingLastMsg];
        }
        if(  msg == nil){
            if(allianceID.length> 0){
                commandStr = [commandStr stringByAppendingFormat:@"0|0|0,%@|0|1,0|0|2",allianceID];
            }else{
                commandStr = @"0|0|0";
                
            }
        }else{
            commandStr = [commandStr stringByAppendingFormat:@"%d|%d|0",[UserManager sharedUserManager].currentUser.serverId,msg.sequenceId];
            if(allianceID.length> 0){
                ChatChannel *allChannel =  [[ChannelManager sharedChannelManager] gettingAllianceChannel];
                NSMsgItem *allianLastMsg =  [allChannel gettingLastMsg];
                if(allianLastMsg){
                    commandStr = [commandStr stringByAppendingString:@","];
                    commandStr = [commandStr stringByAppendingFormat:@"%@|%d|1",allianceID,allianLastMsg.sequenceId];
                }else{
                    commandStr = [commandStr stringByAppendingString:@","];
                    commandStr = [commandStr stringByAppendingFormat:@"%@|0|1",allianceID];
                }
            }
            ChatChannel *mailChannel =   [[ChannelManager sharedChannelManager] gettingMailChannelWithLastChanged];
            
            NSMsgItem *mailMsg = nil;
            if (mailChannel){
                mailMsg=  [mailChannel gettingLastMsg];
                if  (mailMsg){
                    commandStr = [commandStr stringByAppendingString:@","];
                    commandStr = [commandStr stringByAppendingFormat:@"0|%@|2",mailMsg.mailId];
                }else{
                    commandStr = [commandStr stringByAppendingString:@","];
                    commandStr =[commandStr stringByAppendingString:@"0|0|2"];
                }
            }else{
                commandStr = [commandStr stringByAppendingString:@","];
                commandStr =[commandStr stringByAppendingString:@"0|0|2"];
            }
            
            NSString * roomStrig   =  [[ChannelManager sharedChannelManager] stringForChatRoomWithAllRoomChannel];
            commandStr = [commandStr stringByAppendingString:@","];
            commandStr = [commandStr stringByAppendingString:roomStrig];
            
        }
        
    }
    self.msgCommandString = commandStr;
}


@end

