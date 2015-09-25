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
//屏幕适配
#define ScreenHeight [[UIScreen mainScreen] bounds].size.height
#define ScreenWidth [[UIScreen mainScreen] bounds].size.width



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
    
    [self initView];
    
    [self selectOpenView:chatType];
    
    [self showChatViewIOS];
    
    if (chatType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (ocs == OpenChatIOS_Normal) {
            [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController tableViewScrollCurrentLine];
        }else{
            ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
            if (cc) {
                [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController adjustLocation2:cc.lastPosition];
            }
        }
        
    }else if(chatType == IOS_CHANNEL_TYPE_ALLIANCE){
        if (ocs == OpenChatIOS_Normal) {
            [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController tableViewScrollCurrentLine];
        }else{
            ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
            if (cc) {
                [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController adjustLocation2:cc.lastPosition];
            }
        }
        
    }else if(chatType == IOS_CHANNEL_TYPE_USER || chatType == IOS_CHANNEL_TYPE_CHATROOM){
        
        if ([UserManager sharedUserManager].currentUser.allianceId.length >0) {
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.topUIView settingAddMemberButtonShow:YES];
        }else{
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.topUIView settingAddMemberButtonShow:NO];
        }
        
        //是否是普通视图打开
        if (ocs == OpenChatIOS_Normal) {
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController tableViewScrollCurrentLine];
        }else{
            NSString *channelID = [UserManager sharedUserManager].currentMail.opponentUid;
            ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:channelID];
            
            if (cc) {
                [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController adjustLocation2:cc.lastPosition];
            }
        }
    }
    
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
-(void) selectOpenView:(int)chatType
{
    if(chatType == IOS_CHANNEL_TYPE_USER || chatType == IOS_CHANNEL_TYPE_CHATROOM)
    {
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
            
            
            NSString *channelID = [UserManager sharedUserManager].currentMail.opponentUid;
            ChannelManager *cm = [ChannelManager sharedChannelManager];
            if (![[cm.channel_map allKeys] containsObject:channelID]) {
                [cm createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:channelID];
            }
            ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:channelID];
            [cc gettingFirstMsg];
            cc.customName = [UserManager sharedUserManager].currentMail.opponentName;
            [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.currentChatChannel = cc;
        }
        
        
        [self.vc_win bringSubviewToFront:self.mailViewController.view];
        
    }
    else if(chatType == IOS_CHANNEL_TYPE_COUNTRY || chatType == IOS_CHANNEL_TYPE_ALLIANCE)
    {
        if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
            ChatChannel *cc = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
            [cc gettingFirstMsg];
            [self.chatViewController.topUIView selectASN];
            [self.chatViewController isShowJionAllance];
        }else{
            [self.chatViewController.topUIView selectCoun];
            ChatChannel *cc = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            [cc gettingFirstMsg];
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
    NSString *sendLocalTime = [NSString stringWithFormat:@"%d", (NSUInteger)[[NSDate date] timeIntervalSince1970]];
    
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
-(void) initMaiChannelData
{
    //对相同的fromUid第一次打开邮件 需要做滚动到最新行。在加载数据后调用滚动函数
    [[ChatServiceController chatServiceControllerSharedManager].gameHost initChatToHistory_mail] ;
}

-(void) flyHint:(NSString*)icon :(NSString*)titleText :(NSString*)contentText :(CGFloat)time :(CGFloat)dy :(BOOL)useDefaultIcon
{
    BDKViewController *bdk = [[BDKViewController alloc] init:contentText];
    [self.vc_win addSubview:bdk.view];
    
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
    [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
    CCDirector::sharedDirector()->setVisitFlag(true);
}

-(void) showChatViewIOS
{
    [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = NO;
    CCDirector::sharedDirector()->setVisitFlag(false);
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

-(ServiceInterface*)init
{
    self = [super init];
    if (self) {
        self.isInMailDialog = FALSE;
        self.m_curChatType = -1;
        self.rememberPosition = false;
//        self.isLoadCountriesData = TRUE;
//        self.isLoadAllanceData = TRUE;
//        self.isFirstOpenIOS = TRUE;
        self.isSrollNewMsg = TRUE;
        self.isReturnOpengIOS = TRUE;
        [self screenAdaptation];
        
        UIWindow *window=[[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
        self.vc_win = window;
        UIWindow *keywindow= [[[UIApplication sharedApplication ]delegate]window];
        self.vc_win.hidden = YES;
        [keywindow addSubview:self.vc_win];
        
        
        /*zyt*/
        UIWindow *window1  =[[UIWindow alloc]init];
        window1.frame = [UIScreen mainScreen].bounds;
        self.chatRootWindow = window1;
         window1 = nil;
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
        
        self.userInfofontSize = 15.0;
        
        self.endtimeFontSize = 20.0;
        
        self.fontSize = 32.0;
        
        self.userInfoX = 0.10;
        
        self.font2dxSize = 28.0;
        
        self.x = 0.015;
        
        self.contentX = 0.045;
        
        self.sectionFontSize = 22;
        
        
        self.chatLabel_icon_size_height = 30;
        self.chatLabel_icon_size_width = 70;
        self.chatLabel_LINE_HEIGHT = 40;
        self.mail_native_nameLabelSize =27;
        self.mail_native_contentsLabelSize =23;
        self.mail_native_timeLabelSize = 18;
    }else{
        
        self.autoSizeScaleX = 1.0;
        
        self.autoSizeScaleY = 1.0;
        
        self.autoSizeScaleWidth = 1.1;
        
        self.autoSizeScaleHeight = 1.1;
        
        self.chatFontSize = 13.0;
        
         self.chatSystemSize = 12.0;
        self.timefontSize = 12.0;
        
        self.userInfofontSize = 10.0;
        
        self.endtimeFontSize = 10.0;
        
        self.fontSize = 16.0;
        
        self.userInfoX = 0.1;
        
        self.font2dxSize = 12.0;
        
        self.x = 0.015;
        
        self.contentX = 0.09;
        self.sectionFontSize = 14;
        
        self.chatLabel_icon_size_height = 20;
        self.chatLabel_icon_size_width = 40;
        self.chatLabel_LINE_HEIGHT = 20;
        self.mail_native_nameLabelSize =17;
        self.mail_native_contentsLabelSize =14;
        self.mail_native_timeLabelSize = 13;
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

@end

