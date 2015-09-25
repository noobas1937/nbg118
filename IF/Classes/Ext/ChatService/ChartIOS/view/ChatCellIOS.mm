//
//  ChartCell.m
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import "ChatCellIOS.h"
#import "ChatContentView.h"
#import <QuartzCore/QuartzCore.h>
#import "ServiceInterface.h"
#import "UserManager.h"
#import "BanPopUpController.h"
#import "TipPopUpController.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "MsgMessage.h"
#import "ChatServiceController.h"
#include "ChatServiceCocos2dx.h"
//#import "EGOImageView.h"
#import "ServiceInterface.h"
#import "ZYTGoogleTranslat.h"
#import "UIImageView+EMWebCache.h"
#import "NSAttributedString+Emotion.h"
#import "ZYTAlertView.h"
#import "NSString+Cocos2dHelper.h"
@interface ChatCellIOS()<ChatContentViewDelegate,ZYTAlertViewDelegate>
@property (nonatomic,strong) UIView *icon;
@property (nonatomic,strong) UIImageView *icon_border;
@property (nonatomic,strong) UIImageView *icon_headPic;
@property (nonatomic,strong) UIImageView *icon_gmod;
@property (nonatomic,strong) NSString *contentStr;
@property (nonatomic,strong) UILabel *timeLabel;
@property (nonatomic,strong) UILabel *vipLabel;
@property (nonatomic,strong) UILabel *asnNameLabel;
@property (nonatomic,strong) UILabel *playerNameLabel;
@property (nonatomic,strong) UIView *uiViewTop;
@property (nonatomic,strong) UIView *uiViewDown;
@property (nonatomic,strong) UIView *vip_asn_name_View;
@property (nonatomic,strong) UILabel *userInfoLabel;
@property (nonatomic,assign) CGFloat userInfoLabel_fontSize;
@property (nonatomic,assign) CGFloat time_fontSize;

//item
@property(nonatomic,assign) BOOL     canTranlate;
@property(nonatomic,assign) BOOL     hasTranslated;
@property(nonatomic,assign) BOOL     canJoinAlliance;
@property(nonatomic,assign) BOOL     canSendMail;
@property(nonatomic,assign) BOOL     canBlock;
@property(nonatomic,assign) BOOL     canUnBlock;
@property(nonatomic,assign) BOOL     canBan;
@property(nonatomic,assign) BOOL     canUnBan;
@property(nonatomic,assign) BOOL     canViewBattleReport;
@property(nonatomic,assign) BOOL     canViewDetectReport;
@property(nonatomic,assign) BOOL     canInvite;
//重发按钮
@property (nonatomic,strong) UIButton *resetSendBtn;
-(void)cellAddSubview;
-(void)flashCell;
@end

@implementation ChatCellIOS

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        /**添加边框线*/
//        [[self layer] setBorderWidth:1.0f];
        for (UIView *subView in self.contentView.subviews) {
            [subView removeFromSuperview];
        }
        [self initData];
    
    }
    

    return self;
}



-(void)setCellFrame:(ChatCellFrame *)cellFrame
{
    _cellFrame = cellFrame;
//    [_cellFrame.chatMessage initUserForReceivedMsg];
    __block ChatCellFrame * weakcellFrame = _cellFrame;
    __block ChatCellIOS *weakCell = self;
    [_cellFrame.chatMessage initMsgUserWithBlock:^(NSUserInfo *userInfo) {
        if ([userInfo.uid isEqualToString:weakcellFrame.chatMessage.uid]) {
            weakcellFrame.chatMessage.name = userInfo.userName;
            if (weakcellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
                weakcellFrame.chatMessage.asn = @"";
            }else{
                weakcellFrame.chatMessage.asn = userInfo.asn;
            }
            weakcellFrame.chatMessage.vip = [userInfo gettingVipString];;
            weakcellFrame.chatMessage.headPic = userInfo.headPic;
            weakcellFrame.chatMessage.headPicVer = userInfo.headPicVer;
            [weakcellFrame.chatMessage settingMsgUserNickString];
            NSString *headerUrl = [NSString gettingCustomHeadPic:weakcellFrame.chatMessage.uid  int:weakcellFrame.chatMessage.headPicVer ];
            weakcellFrame.chatMessage.customHeadPicUrl = headerUrl;
            [weakcellFrame  chatCellFrameUpdateUserInfo:weakcellFrame.chatMessage];
 
            [weakCell reloadWithModel];
            
        }
    }];
 
    //设置cell数据
    [self initWithCellData:cellFrame];
    
    //设置cell大小
    [self setUICGRectMake];
    
    //添加cell
    [self cellAddSubview];
    
    
    
    //设置内容背景图
    [self contentBackGround];
    
    //设置重发按钮以及风火轮的显示
    [self isShowActivityIndicator];
    [self isShowResetSendBtn];
    [self settingActivityIndicatorAndResetSendBtnIsShow];

    self.autoresizingMask = UIViewAutoresizingFlexibleTopMargin|UIViewAutoresizingFlexibleRightMargin;
    
    [self setTestBorderWidth];
    
}

-(void)reloadWithModel{
//    if  ([self.chatCellIosDelegate respondsToSelector:@selector(chatCellReloadWithCell:)]){
//        [self.chatCellIosDelegate chatCellReloadWithCell:self];
//    }
    UITableView * tableView ;
    if (self.cellFrame.chatMessage.channelType == 0) {
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
    }else if(self.cellFrame.chatMessage.channelType == 1){
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
    }else if(self.cellFrame.chatMessage.channelType == 2 || self.cellFrame.chatMessage.channelType == 3){
        tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
    }
    NSIndexPath *indexPath = [tableView indexPathForCell:self];
    if (indexPath != nil){
        //设置cell数据
        [self initWithCellData:self.cellFrame];
//        [self.icon_headPic sd_setImageWithURL:[NSURL URLWithString:self.cellFrame.chatMessage.customHeadPicUrl] placeholderImage:[UIImage imageNamed:self.cellFrame.chatMessage.headPic]];
        //设置cell大小
        [self setUICGRectMake];
        
 
        [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];

    }
   
}
/**@brief 不是自己的信息风火轮和重发按钮隐藏*/
-(void) settingActivityIndicatorAndResetSendBtnIsShow
{
    if (!self.cellFrame.chatMessage.isSelfMsg) {
        self.activityIndicator.hidden = YES;
        self.resetSendBtn.hidden = YES;
    }
}

/**设置翻译信息*/
-(void)_settingtrTranslation
{
    
    if (self.cellFrame.chatMessage.isSelfMsg ==YES){
        return;
    }
    if (self.cellFrame.chatMessage.translateMsg.length > 0 && self.cellFrame.chatMessage.translatedLang.length > 0) {
        return;
    }
    __block ChatCellIOS *weakSelf = self;
    
    ZYTGoogleTranslat *  translat =[[ZYTGoogleTranslat alloc]init];
    _cellFrame.chatMessage.translateMsg =  _cellFrame.chatMessage.msg;
    [translat translationingWithSL:@"auto" andWithTL:_cellFrame.chatMessage.localLanString andWithHL:@"en" andWithIE:@"UTF-8" andWithOE:@"UTF-8" andWithQ:_cellFrame.chatMessage.msg andResultBlock:^(ZYTGoogleTranslatHTTPStatus status,NSString *resultString,NSString *orgLangTypeString) {
        
        if (status == ZYTGoogleTranslatHTTPStatus_sucess) {
            DVLog(@"翻译后的语言:%@",resultString);
            DVLog(@"原语言出处 :%@",orgLangTypeString);
            if (resultString.length>0) {
                weakSelf.cellFrame.chatMessage.translateMsg = resultString;
                weakSelf.cellFrame.chatMessage.originalLang = orgLangTypeString;
                weakSelf.cellFrame.chatMessage.hasTranslated = YES;
                [weakSelf.cellFrame.chatMessage packedMsg];
                [weakSelf.cellFrame setChatMessage:weakSelf.cellFrame.chatMessage];
                if  ([weakSelf.chatCellIosDelegate respondsToSelector:@selector(ChatCellIOSTransLateWithCell:)]){
                    [weakSelf.chatCellIosDelegate ChatCellIOSTransLateWithCell:self];
                }
                
            }
        }else{
            //            [weakSelf settingtrTranslation];
        }
        
    }];
    
}

/**
 *  检查当前msg对象是否有翻译
 */
-(void)_chatCellFrameCheckTranslate{
    if (self.cellFrame.chatMessage.isSelfMsg ==NO){
        if (!(self.cellFrame.chatMessage.translateMsg.length > 0 && self.cellFrame.chatMessage.translatedLang.length > 0)) {
            self.cellFrame.chatMessage.translateMsg = @"";
            self.cellFrame.chatMessage.translatedLang = @"";
            [self _settingtrTranslation];
        }
    }
}


-(void) contentBackGround
{
    if(!self.chatView.chatMessage.isSelfMsg)        //左边的消息
    {
        //系统消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post != 6) {
            UIImage *imageUp = [UIImage imageNamed:@"chat_system_left_bg_normal"];
            UIImage *imageDown = [UIImage imageNamed:@"chat_to_bg_pressed"];
            [self setChatViewFrame:imageUp :imageDown];
        }
        //喇叭消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post == 6) {
            NSLog(@"左边喇叭消息");
            UIImage *imageUp = [UIImage imageNamed:@"horn_bg_left"];
            UIImage *imageDown = [UIImage imageNamed:@"horn_bg_left"];
            
            [self setChatViewFrame:imageUp :imageDown];
        }
        //别人的消息
        if (self.chatView.chatMessage.post == 0 && !self.chatView.chatMessage.isSelfMsg) {
            UIImage *imageUp = [UIImage imageNamed:@"chat_to_bg_normal"];
            UIImage *imageDown = [UIImage imageNamed:@"chat_to_bg_pressed"];
            [self setChatViewFrame:imageUp :imageDown];
        }
    }else{                                          //右边的消息
        //系统消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post != 6) {
            UIImage *imageUp = [UIImage imageNamed:@"chat_system_right_bg_normal"];
            UIImage *imageDown = [UIImage imageNamed:@"chat_system_right_bg_pressed"];
            [self setChatViewFrame:imageUp :imageDown];
        }
        //喇叭消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post == 6) {
            NSLog(@"右边喇叭消息");
            
            UIImage *imageUp = [UIImage imageNamed:@"horn_bg_right"];
            UIImage *imageDown = [UIImage imageNamed:@"horn_bg_right"];
            
    
            [self setChatViewFrame:imageUp :imageDown];
        }
        //自己的消息
        if (self.chatView.chatMessage.post == 0 && self.chatView.chatMessage.isSelfMsg) {
            UIImage *imageUp = [UIImage imageNamed:@"chat_right_bg_normal"];
            UIImage *imageDown = [UIImage imageNamed:@"chat_from_bg_pressed"];
            [self setChatViewFrame:imageUp :imageDown];
        }
    }
}

-(void) setChatViewFrame:(UIImage *)imageUp :(UIImage *)imageDown
{
    self.chatView.chatImageUp = [imageUp resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 +1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))];
    self.chatView.chatImageDown = [imageDown resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 + 1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))];
    
    self.chatView.backImageView.image = self.chatView.chatImageUp;
    
    self.chatView.frame= self.cellFrame.chatViewRect;
    
    //测试线
    //[[self.chatView layer] setBorderWidth:1.0f];
    
    CGFloat x = 0;
    
    if(self.cellFrame.chatMessage.isSelfMsg){
        x=5*1.0;
    }else {
        x=5 * 3.0;
    }
//    self.chatView.contentLabel.left =x;
//    self.chatView.contentLabel.top = 5;
    self.chatView.contentLabel.frame = CGRectMake(x,5, self.cellFrame.chatLabelSize.width,self.cellFrame.chatLabelSize.height);
    if(self.chatView.contentLabel.text == nil){
        self.chatView.contentLabel.text =@"";
    }
//    DVLog(@" 对应文本: %@",self.chatView.contentLabel.text);
//    DVLog(@"self.chatView.contentLabel.frame   %@",NSStringFromCGRect(self.chatView.contentLabel.frame));
    
    CGSize winSize=[UIScreen mainScreen].bounds.size;
    
    if (self.cellFrame.endStrLabelSize.width >= self.chatView.contentLabel.width)
    {
        self.chatView.endStrLabel.frame = CGRectMake(self.chatView.contentLabel.left, self.chatView.contentLabel.frame.size.height + winSize.height * 0.008, self.cellFrame.endStrLabelSize.width, self.cellFrame.endStrLabelSize.height);

    }else{
        self.chatView.endStrLabel.frame = CGRectMake(self.chatView.contentLabel.right - self.cellFrame.endStrLabelSize.width, self.chatView.contentLabel.frame.size.height + winSize.height * 0.008, self.cellFrame.endStrLabelSize.width, self.cellFrame.endStrLabelSize.height);

    }
    
    //背景图片宽度 ＝ chatView . 高度 ＝ 通过文字长度计算出来的值 + 聊天结尾信息高度
    self.chatView.backImageView.frame = CGRectMake(0 ,0 , self.chatView.frame.size.width , self.chatView.frame.size.height + self.chatView.endStrLabel.frame.size.height);
    
}

-(void)chatContentViewTapPress:(ChatContentView *)chatView content:(NSString *)content
{
    //[self becomeFirstRespond];
    
    //self.chatView.backImageView.image = self.chatView.chatImageUp;
    //给item设置数据
    [self initItemBool];
    
    UIMenuController *uiMenu = [UIMenuController sharedMenuController];
    
    //item 本地化
    NSString *invitejoin_Item_Str       = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_INVITE];
    NSString *joinAlliance_Item_Str     = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_JOIN];
    NSString *sendMail_Item_Str         = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_SENDMSG];
    NSString *originalItem_Str          = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_ORIGINALLAN];
    NSString *translateItem_Str         = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_TRANSLATE];
    NSString *battleMsgItem_Str         = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_BATTLEREPORT];
    NSString *detectMsgItem_Str         = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_DETECTREPORT];
    NSString *blockItem_Str             = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_SHIELD];
    NSString *unBlockItem_Str           = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_UNSHIELD];
    NSString *banItem_Str               = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_BAN];
    NSString *unBanItem_Str             = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_UNBAN];
    NSString *copyItem_Str              = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_COPY];
    
    //item
    UIMenuItem *invitejoin_Item = [[UIMenuItem alloc] initWithTitle:invitejoin_Item_Str action:@selector(invitejoin_callFun:)];
    UIMenuItem *joinAlliance_Item = [[UIMenuItem alloc] initWithTitle:joinAlliance_Item_Str action:@selector(joinAlliance_callFun:)];
    UIMenuItem *sendMail_Item = [[UIMenuItem alloc] initWithTitle:sendMail_Item_Str action:@selector(sendMail_callFun:)];
    UIMenuItem *originalItem = [[UIMenuItem alloc] initWithTitle:originalItem_Str action:@selector(original_callFun:)];
    UIMenuItem *translateItem = [[UIMenuItem alloc] initWithTitle:translateItem_Str action:@selector(translate_callFun:)];
    UIMenuItem *battleMsgItem = [[UIMenuItem alloc] initWithTitle:battleMsgItem_Str action:@selector(battleMsg_callFun:)];
    UIMenuItem *detectMsgItem = [[UIMenuItem alloc] initWithTitle:detectMsgItem_Str action:@selector(detectMsg_callFun:)];
    UIMenuItem *blockItem = [[UIMenuItem alloc] initWithTitle:blockItem_Str action:@selector(block_callFun:)];
    UIMenuItem *unBlockItem = [[UIMenuItem alloc] initWithTitle:unBlockItem_Str action:@selector(unBlock_callFun:)];
    UIMenuItem *banItem = [[UIMenuItem alloc] initWithTitle:banItem_Str action:@selector(ban_callFun:)];
    UIMenuItem *unBanItem = [[UIMenuItem alloc] initWithTitle:unBanItem_Str action:@selector(unBan_callFun:)];
    UIMenuItem *copyItem = [[UIMenuItem alloc] initWithTitle:copyItem_Str action:@selector(copy_callFun:)];
    
    UIWindow *window = [ServiceInterface serviceInterfaceSharedManager].vc_win;
    if ([window isKeyWindow] == NO)
    {
        [window becomeKeyWindow];
        [window makeKeyAndVisible];
    }
    
    [self becomeFirstResponder];
    
//    查看战报｜翻译｜复制｜发送邮件｜屏蔽
    
    NSMutableArray *itemList = [[NSMutableArray alloc]init];
    
    if(self.canViewBattleReport){
        [itemList addObject:battleMsgItem];
    }
    
    if (self.canViewDetectReport) {
        [itemList addObject:detectMsgItem];
    }
    
    if (self.canTranlate || ChatServiceCocos2dx::m_autoTranslateWithGoogle) {
        if (self.hasTranslated) {
            [itemList addObject:originalItem];
        }else{
            [itemList addObject:translateItem];
        }
    }

    [itemList addObject:copyItem];
    
    if(self.canSendMail){
        [itemList addObject:sendMail_Item];
    }
    
    if(self.canBlock){
        [itemList addObject:blockItem];
    }
    
    if (self.canUnBlock) {
        [itemList addObject:unBlockItem];
    }
    
    if(self.canBan){
        [itemList addObject:banItem];
    }
    
    if(self.canUnBan){
        [itemList addObject:unBanItem];
    }
    
    if (self.canInvite) {
        [itemList addObject:invitejoin_Item];
    }
    
    if (self.canJoinAlliance) {
        [itemList addObject:joinAlliance_Item];
    }
    
    [uiMenu setMenuItems:itemList];
    
//    NSArray *array = [NSArray arrayWithObjects:invitejoin_Item, joinAlliance_Item, sendMail_Item, originalItem, translateItem, battleMsgItem, detectMsgItem, blockItem, unBlockItem, banItem, unBanItem, copyItem, nil];
//    
//    [uiMenu setMenuItems:array];
    
    CGRect rect = CGRectMake(0,0,self.chatView.frame.size.width,self.chatView.frame.size.height);
    
    [uiMenu setTargetRect:rect inView:self.chatView];
    [uiMenu setMenuVisible:YES animated:YES];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(menuShow:) name:UIMenuControllerWillShowMenuNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(menuHide:) name:UIMenuControllerWillHideMenuNotification object:nil];
    self.contentStr=content;
    
}

//邀请加入联盟
- (void)invitejoin_callFun:(id)sender
{
    NSLog(@"邀请加入联盟");
    [[ChatServiceController chatServiceControllerSharedManager].gameHost invitejoin:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
}

//加入联盟
- (void)joinAlliance_callFun:(id)sender
{
    NSLog(@"加入联盟");
    [[ChatServiceController chatServiceControllerSharedManager].gameHost joinAlliance:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
}

//发送邮件
- (void)sendMail_callFun:(id)sender
{
    NSString *ns_fromUid = self.cellFrame.chatMessage.uid;
    NSString *ns_fromName = self.cellFrame.chatMessage.name;
    [[ChatServiceController chatServiceControllerSharedManager].gameHost openMailDialogViewFirst:ns_fromUid :ns_fromName];
}

//源语言
- (void)original_callFun:(id)sender
{
    
    NSLog(@"源语言");
    self.cellFrame.chatMessage.showTransLate = YES;
    self.cellFrame.chatMessage.hasTranslated = FALSE;
    
    self.cellFrame.chatMessage.isNewMsg = FALSE;
    
    UITableView * tableView = nil;
    
    if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
    }else if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
    }else if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM){
        tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
    }
    
//    self.cellFrame.chatMessage.showMsg = [self.cellFrame.chatMessage gettingShowMsg];
    [self.cellFrame.chatMessage settingShowMsg:nil];
    [self.cellFrame setChatMessage:self.cellFrame.chatMessage];
    
    NSIndexPath *indexPath = [tableView indexPathForCell:self];
    
    [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
}

//翻译
- (void)translate_callFun:(id)sender
{
    DVLog(@"翻译");
    
     self.cellFrame.chatMessage.showTransLate = NO;
    if (self.cellFrame.chatMessage.translateMsg.length>0) {
        DVLog(@"%@",[self.cellFrame.chatMessage printAllPropertys]);
        DVLog(@"服务器带了翻译文本");
        self.cellFrame.chatMessage.hasTranslated = TRUE;
        self.cellFrame.chatMessage.isNewMsg = FALSE;
        
        UITableView * tableView = nil;
        
        if (self.cellFrame.chatMessage.channelType == 0) {
            tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
        }else if(self.cellFrame.chatMessage.channelType == 1){
            tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
        }else if(self.cellFrame.chatMessage.channelType == 2 || self.cellFrame.chatMessage.channelType == 3){
            tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
        }
        
//        self.cellFrame.chatMessage.showMsg = [self.cellFrame.chatMessage gettingShowMsg];
         NSString *localLanString = [NSString stringWithUTF8String:LocalController::shared()->getLanguageFileName().c_str()] ;
        [self.cellFrame.chatMessage settingShowMsg:localLanString];
        [self.cellFrame setChatMessage:self.cellFrame.chatMessage];
        
        NSIndexPath *indexPath = [tableView indexPathForCell:self];
        
        [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];

    }
    else{
        
        if (!ChatServiceCocos2dx::m_autoTranslateWithGoogle){
            return ;
        }
        
          DVLog(@"服务器未带翻译文本");
        self.cellFrame.chatMessage.hasTranslated = FALSE;
        
        self.cellFrame.chatMessage.isNewMsg = FALSE;
        /*
         
         https://translate.google.com/translate_a/single?client=t
         &sl=auto&tl=zh-CN&hl=en&dt=t&ie=UTF-8&oe=UTF-8&q=nihao
         
         sl: 源语言， auto 为 google 自动检测
         tl: 翻译目标语言
         hl: 这个参数没用，google 界面语言
         dt=t 写死
         ie: 输入编码
         oe:输出编码
         q: 需要翻译的字符串
         */
        __block ChatCellFrame *tempCellFrame = self.cellFrame;
        __block ChatCellIOS *weakSelf = self;
        NSString *localLanString = [NSString stringWithUTF8String:LocalController::shared()->getLanguageFileName().c_str()] ;
        DVLog(@"%@",localLanString);
        ZYTGoogleTranslat *translat =[[ZYTGoogleTranslat alloc]init];
        [translat translationingWithText:self.cellFrame.chatMessage.msg andWihtSL:@"auto" andWithTL:localLanString andResultBlock:^(ZYTGoogleTranslatHTTPStatus status, NSString *resultString, NSString *orgLangTypeString) {
        if (status == ZYTGoogleTranslatHTTPStatus_sucess){
            DVLog(@"%@",resultString);
            if (resultString.length>0) {
                self.cellFrame.chatMessage.hasTranslated = TRUE;
                tempCellFrame.chatMessage.translateMsg = resultString;
                tempCellFrame.chatMessage.originalLang = orgLangTypeString;
                tempCellFrame.chatMessage.translatedLang = localLanString;
                //                weakSelf.cellFrame.chatMessage.showMsg = [weakSelf.cellFrame.chatMessage gettingShowMsg];
                //
                //                [weakSelf.cellFrame initWithData:weakSelf.cellFrame.chatMessage];
                [self.cellFrame.chatMessage settingShowMsg:localLanString];
                [weakSelf.cellFrame setChatMessage:tempCellFrame.chatMessage];
                UITableView * tableView ;
                if (weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
                    tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
                }else if(weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                    tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
                }else if(weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER){
                    tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
                }
                NSIndexPath *indexPath = [tableView indexPathForCell:weakSelf];
                
                [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
                
            }
        }
            
        }];
 
    }
    
    

}

//查看战报
- (void)battleMsg_callFun:(id)sender
{
    NSLog(@"查看战报");
    [[ChatServiceController chatServiceControllerSharedManager].gameHost battleMsg:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.attachmentId];

}

//侦查战报
- (void)detectMsg_callFun:(id)sender
{
    NSLog(@"侦查战报");
    [[ChatServiceController chatServiceControllerSharedManager].gameHost detectMsg:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.attachmentId];
}

//屏蔽
- (void)block_callFun:(id)sender
{
    NSLog(@"禁言");
    
 
    NSString * tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_SHIELD_PLAYER :self.cellFrame.chatMessage.name];
    ZYTAlertView  * alertView = [ZYTAlertView alertViewWithTitleString:nil];
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    alertView.alertViewDelegate = self;
    UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    
    
    
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    
//    tipPopUpController.tipType = BLOCKTYPE;
    
    [recentView.view addSubview:alertView];
    
    
    
}

//解除屏蔽
- (void)unBlock_callFun:(id)sender
{
    [[ChatServiceController chatServiceControllerSharedManager].gameHost unBlock:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
    [[UserManager sharedUserManager] removeRestrictUser:self.cellFrame.chatMessage.name :1];
}

//禁言
- (void)ban_callFun:(id)sender
{
    
    BanPopUpController *banPopUpController = [[BanPopUpController alloc] initWithNibName:@"BanPopUpController" bundle:nil];
    
    [banPopUpController setInitData:self.cellFrame.chatMessage];
    
    UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    [recentView.view addSubview:banPopUpController.view];
    
}

//解除禁言
- (void)unBan_callFun:(id)sender
{
    NSLog(@"解除禁言");
    [[ChatServiceController chatServiceControllerSharedManager].gameHost unBan:self.cellFrame.chatMessage.uid];
}

//copy
- (void)copy_callFun:(id)sender {
    NSLog(@"copy_callFun");
    NSString *contentStr;
    if(self.cellFrame.chatMessage.hasTranslated){
        contentStr = self.cellFrame.chatMessage.translateMsg;
    }else{
        contentStr = self.cellFrame.chatMessage.msg;
    }
    [[UIPasteboard generalPasteboard]setString:contentStr];
    
    
}

/**重新发送*/
-(void) resetChatPressed:(id)sender
{
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost resetSend:self];
}

//YES item成为第一响应者
- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    
    //邀请加入联盟
    if(action == @selector(invitejoin_callFun:) || action == @selector(joinAlliance_callFun:) || action == @selector(sendMail_callFun:) || action == @selector(original_callFun:) || action == @selector(translate_callFun:) || action == @selector(battleMsg_callFun:) || action == @selector(detectMsg_callFun:) || action == @selector(block_callFun:) || action == @selector(unBlock_callFun:) || action == @selector(ban_callFun:) || action == @selector(unBan_callFun:) || action == @selector(copy_callFun:)){
            return YES;
    }
    
    return [super canPerformAction:action withSender:sender];
}

-(void)menuShow:(UIMenuController *)menu
{

    
    
}

-(void)menuHide:(UIMenuController *)menu
{
//    self.chatView=nil;
    [self resignFirstResponder];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void)initData
{
    
    
    self.time_fontSize = [ServiceInterface serviceInterfaceSharedManager].timefontSize;
    
    self.uiViewTop=[[UIView alloc]init];
    
    self.uiViewDown=[[UIView alloc]init];
    
    self.chatView=[[ChatContentView alloc]init];
    self.chatView.contentLabel.linkTapHandler = ^(KZLinkType linkType, NSString *string, NSRange range){
        if (linkType == KZLinkTypeURL) {
            DVLog(@"点击的链接为：%@",string);
        } else if (linkType == KZLinkTypePhoneNumber) {
            DVLog(@"点击的电话号码为：%@",string);
        } else if (linkType == KZLinkTypeCoordinate){
            DVLog(@"坐标:%@",string);
            [self clickLableWithString:string];
        }else if(linkType == KZLinkTypeUserHandle){
            //用户昵称  eg: @kingzwt
        }else if(linkType == KZLinkTypeUserHandle){
            //内容标签  eg: #hello
        }
    };



    self.timeLabel=[[UILabel alloc]init];
    
    self.vip_asn_name_View = [[UIView alloc]init];
    
    self.vipLabel=[[UILabel alloc]init];

    self.asnNameLabel=[[UILabel alloc]init];
    
    self.playerNameLabel=[[UILabel alloc]init];
    
    int type = ChatServiceCocos2dx::m_channelType;
    
        self.userInfoLabel = [[UILabel alloc]init];
        self.userInfoLabel_fontSize = [ServiceInterface serviceInterfaceSharedManager].userInfofontSize;
    
    
    
    //icon
    
    self.icon=[[UIView alloc]init];
    
    self.icon_border=[[UIImageView alloc]init];
    
    self.icon_headPic=[[UIImageView alloc]init];
    
    self.icon_gmod=[[UIImageView alloc]init];
    
    //风火轮
    self.activityIndicator = [[UIActivityIndicatorView alloc]
                              initWithActivityIndicatorStyle:
                              UIActivityIndicatorViewStyleWhite];
    //重发按钮
    
    self.resetSendBtn = [[UIButton alloc]init];
    self.activityIndicator.center = self.chatView.center;
    self.resetSendBtn.frame = self.activityIndicator.frame;
    self.backgroundColor = [UIColor clearColor];
    
    
    [self.uiViewTop addSubview:self.timeLabel];
     [self.contentView addSubview:self.uiViewTop];
    [self.contentView addSubview:self.uiViewDown];
    [self.uiViewDown addSubview:self.userInfoLabel];
    
    //icon add
    [self.uiViewDown addSubview:self.icon];
    [self.icon addSubview:self.icon_border];
    [self.icon addSubview:self.icon_headPic];
    [self.icon addSubview:self.icon_gmod];

}

-(void)initWithCellData:(ChatCellFrame *)cellFrame
{
    //cell data
    _cellFrame=cellFrame;
    
    //ChatContentView
    self.chatView.chatMessage=self.cellFrame.chatMessage;
    //time
    if (self.cellFrame.chatMessage.sendLocalTime > 0) {
        
        NSString *time = [[self.cellFrame.chatMessage nsDataSwitchNSString:self.cellFrame.chatMessage.createTime] substringToIndex:10];
        self.timeLabel.text=time;
        self.timeLabel.font=[UIFont systemFontOfSize:self.time_fontSize];
        self.timeLabel.textAlignment = NSTextAlignmentCenter;
        self.timeLabel.textColor = [UIColor whiteColor];
        self.timeLabel.clipsToBounds = YES;
        self.timeLabel.layer.cornerRadius = 2;
    }
    if(self.cellFrame.isShowUiTopView_BOOL){
        self.timeLabel.hidden = NO;
    }else{
        self.timeLabel.hidden = YES;
    }
    
    //ios聊天结尾信息
    self.chatView.endStrLabel.text = self.cellFrame.chatMessage.endMsg;

    
    //设置用户信息 判断是否是邮件，如果不是邮件显示用户信息
//    int type = ChatServiceCocos2dx::m_channelType;
    int type = self.cellFrame.chatMessage.channelType;
    if (type == IOS_CHANNEL_TYPE_USER ) {
        self.userInfoLabel.hidden = YES;
        
    }else{
        self.userInfoLabel.hidden = NO;
        self.userInfoLabel.font = [UIFont systemFontOfSize:self.userInfoLabel_fontSize];
        if(self.cellFrame.chatMessage.vip.length == 0 || [self.cellFrame.chatMessage.vip isEqualToString:@""] || !self.cellFrame.chatMessage.vip){
            self.userInfoLabel.text = self.cellFrame.chatMessage.showUserInfoLabel;
        }else{
            NSMutableAttributedString *str = [[NSMutableAttributedString alloc] initWithString:self.cellFrame.chatMessage.showUserInfoLabel];
            int length = self.cellFrame.chatMessage.vip.length;
            if ([self.cellFrame.chatMessage.showUserInfoLabel hasPrefix:@"VIP"]) {
                [str addAttribute:NSForegroundColorAttributeName value:[UIColor yellowColor] range:NSMakeRange(0,length)];
                self.userInfoLabel.attributedText = str;
            }else{
                self.userInfoLabel.text = self.cellFrame.chatMessage.showUserInfoLabel;
            }
        }
    }

    //icon image
    self.icon_border.image = [UIImage imageNamed:@"icon_kuang.png"];
    
    self.icon_headPic.image = [UIImage imageNamed:self.cellFrame.chatMessage.headPic];
    
    //是否是自定义头像
    int senderPicVer = self.cellFrame.chatMessage.headPicVer;
    if ([[self.cellFrame.chatMessage gettingUserInfo] isCustomHeadImage]){
        //自定义头像
        
        [self.icon_headPic sd_setImageWithURL:[NSURL URLWithString:self.cellFrame.chatMessage.customHeadPicUrl] placeholderImage:[UIImage imageNamed:self.cellFrame.chatMessage.headPic]];
        
//        UIImage *image = [self.cellFrame.chatMessage gettingCustomHeadPicImg];
//        if(image){
//            self.icon_headPic.image = image;
//        }else{
//            //自定义头像的地址如果为空 从网络下载
//            if (self.cellFrame.chatMessage.customHeadPicUrl.length > 0) {
//                self.icon_headPic.placeholderImage = [UIImage imageNamed:self.cellFrame.chatMessage.headPic];
//                self.icon_headPic.imageURL = [NSURL URLWithString:self.cellFrame.chatMessage.customHeadPicUrl];
//                [[ChatServiceController chatServiceControllerSharedManager] downHeadImage:self.cellFrame.chatMessage.customHeadPicUrl];
//            }else{
//                [self settingDefaultPlayerHeadPicd];
//            }
//        }
    }else{
        [self settingDefaultPlayerHeadPicd];
    }
    
    self.icon_gmod.image = [UIImage imageNamed:self.cellFrame.chatMessage.gmodStr];
    
    //头像点击查看玩家信息
    [self.icon_headPic setUserInteractionEnabled:YES];
    [self.icon_headPic addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(showPlayerInfo:)]];
    
//    self.chatView.contentLabel.text = self.cellFrame.chatMessage.showMsg;
//    DVLog(@" 对应文本: %@",self.cellFrame.chatMessage.showMsg);
    
    CGFloat fs = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    
    //    CGSize labelSize = [_chatMessage.showMsg textSizeWithFont:[UIFont systemFontOfSize:fs] constrainedToSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
    UIFont *font = [UIFont systemFontOfSize:fs];
    NSDictionary *attributes = @{NSFontAttributeName: font};
    NSAttributedString *attributedString = [NSAttributedString emotionAttributedStringFrom:self.cellFrame.chatMessage.showMsg attributes:attributes];
//    [self.chatView.contentLabel setTextAndIcon:self.cellFrame.chatMessage.showMsg];
    self.chatView.contentLabel.attributedText = attributedString;
//    self.chatView.contentLabel.frame = CGRectMake(0, 0, self.cellFrame.chatLabelSize.width, self.cellFrame.chatLabelSize.height);
//    [self.chatView.contentLabel sizeToFit];
    //重发按钮配置
    [self.resetSendBtn setBackgroundImage:[UIImage imageNamed:@"sendfailed"] forState:UIControlStateNormal];
    [self.resetSendBtn addTarget:self action:@selector(resetChatPressed:) forControlEvents: UIControlEventTouchUpInside];
    self.resetSendBtn.hidden = YES;


}

//设置玩家默认头像
-(void) settingDefaultPlayerHeadPicd
{
    self.icon_headPic.image = [UIImage imageNamed:self.cellFrame.chatMessage.headPic];
}

-(void) setUICGRectMake
{
    //uiViewTop CGRectMake
    self.uiViewTop.frame=self.cellFrame.uiTopViewRect;
    //uiViewDown CGRectMake
    self.uiViewDown.frame=self.cellFrame.uiDownViewRect;
    
  
    
    //time CGRectMake
    self.timeLabel.frame=self.cellFrame.timeLabelRect;
    
    //设置用户信息大小
    int type = self.cellFrame.chatMessage.channelType;
    if (type != IOS_CHANNEL_TYPE_USER) {
        self.userInfoLabel.frame = self.cellFrame.vip_asn_name_Rect;
    }
//      self.userInfoLabel.backgroundColor = [UIColor redColor];
    self.chatView.contentLabel.frame.size = self.cellFrame.chatLabelSize;
    self.contentViewDelegate=self;
//    self.chatView.contentLabel.tqDelegage = self;
    self.chatView.chatContentViewdelegate = self;
    
    
    //icon CGRectMake
    self.icon.frame=self.cellFrame.iconRect;
    self.icon_border.frame=self.cellFrame.borderRect;
    self.icon_headPic.frame=self.cellFrame.headPicRect;
    self.icon_gmod.frame=self.cellFrame.gmodRect;
    
    //icon backgroundColor
    self.icon.backgroundColor = [UIColor clearColor];
    self.icon_border.backgroundColor = [UIColor clearColor];
    self.icon_headPic.backgroundColor = [UIColor clearColor];
    self.icon_gmod.backgroundColor = [UIColor clearColor];
    
    //风火轮大小
    self.activityIndicator.frame = self.cellFrame.activityIndicatorRect;
    //重发按钮大小
    self.resetSendBtn.frame = self.cellFrame.activityIndicatorRect;
    
//    NSArray *resetSendBtn_H=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_resetSendBtn]-0-[_chatView]"
//                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
//                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
//                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:self.uiViewDown];
//    
//    //宽
//    
//    [self addConstraint:[NSLayoutConstraint constraintWithItem:self.huitiaoImage
//                                                     attribute:NSLayoutAttributeTop
//                                                     relatedBy:NSLayoutRelationLessThanOrEqual
//                                                        toItem:self.topView
//                                                     attribute:NSLayoutAttributeBottom
//                                                    multiplier:1.0
//                                                      constant:-4]];//设置子视图的高度是父视图高度的一半
//    [self addConstraints:tbt];

}

-(void) cellAddSubview
{
    //根据当前cell的time ＝＝ 上一个cell的time 等来显示时间
    if (self.cellFrame.isShowUiTopView_BOOL ) {
        [self.contentView addSubview:self.uiViewTop];
        self.timeLabel.center = CGPointMake(self.uiViewTop.bounds.size.width/2,self.uiViewTop.bounds.size.height/2);
        self.timeLabel.backgroundColor = [UIColor colorWithRed:146.0/255.0 green:126.0/255.0 blue:102.0/255.0 alpha:1.0];
        [self.uiViewTop addSubview:self.timeLabel];
    }
    
    [self.contentView addSubview:self.uiViewDown];
    
    
    //vip_asn_name_View add
//    [self.uiViewDown addSubview:self.vip_asn_name_View];
//    [self.vip_asn_name_View addSubview:self.vipLabel];
//    [self.vip_asn_name_View addSubview:self.asnNameLabel];
//    [self.vip_asn_name_View addSubview:self.playerNameLabel];
    //添加用户信息
    [self.uiViewDown addSubview:self.userInfoLabel];
    int type = self.cellFrame.chatMessage.channelType;
 
    if (type != IOS_CHANNEL_TYPE_USER) {
   
        [self.uiViewDown addSubview:self.userInfoLabel];
 
    }
    
    //icon add
    [self.uiViewDown addSubview:self.icon];
    [self.icon addSubview:self.icon_border];
    [self.icon addSubview:self.icon_headPic];
    [self.icon addSubview:self.icon_gmod];
    
    //添加风火轮
    [self.uiViewDown addSubview:self.activityIndicator];
    
    //添加重发按钮
    [self.uiViewDown addSubview:self.resetSendBtn];
    
    [self.uiViewDown addSubview:self.chatView];
}

-(void) setTestBorderWidth
{
//    //设置边框
//    self.uiViewTop.layer.borderWidth = TESTBORDERWIDTH;
//    self.uiViewDown.layer.borderWidth = TESTBORDERWIDTH;
//    
//    self.uiViewTop.layer.borderColor = [[UIColor redColor] CGColor];
//    self.uiViewDown.layer.borderColor = [[UIColor yellowColor] CGColor];
//    
//    self.uiViewTop.layer.borderWidth = 3.0;
    
    
//    self.timeLabel.layer.borderWidth = TESTBORDERWIDTH;
//    self.vipLabel.layer.borderWidth = TESTBORDERWIDTH;
//    self.asnNameLabel.layer.borderWidth = TESTBORDERWIDTH;
//    self.playerNameLabel.layer.borderWidth = TESTBORDERWIDTH;
//    self.icon.layer.borderWidth = TESTBORDERWIDTH;
//    self.icon_border.layer.borderWidth = TESTBORDERWIDTH;
//    self.icon_headPic.layer.borderWidth = TESTBORDERWIDTH;
//    self.icon_gmod.layer.borderWidth = TESTBORDERWIDTH;
//    self.chatView.layer.borderWidth = TESTBORDERWIDTH;
//    self.vip_asn_name_View.layer.borderWidth = TESTBORDERWIDTH;
    
//    //设置颜色
//    self.uiViewTop.backgroundColor = [UIColor whiteColor];
//    self.uiViewDown.backgroundColor = [UIColor blackColor];
//    
//    self.timeLabel.backgroundColor = [UIColor whiteColor];
//    
//    self.vipLabel.backgroundColor = [UIColor whiteColor];
//    self.asnNameLabel.backgroundColor = [UIColor whiteColor];
//    self.playerNameLabel.backgroundColor = [UIColor whiteColor];
//    self.chatView.backgroundColor = [UIColor whiteColor];
//    
//    self.timeLabel.textColor=[UIColor whiteColor];
//    
//    self.vip_asn_name_View.backgroundColor = [UIColor whiteColor];
//
//    self.uiViewDown.layer.borderWidth = TESTBORDERWIDTH;
//    self.chatView.layer.borderWidth = TESTBORDERWIDTH;
    
    
}


-(void)initItemBool
{
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    self.canTranlate= ![self.cellFrame.chatMessage.translateMsg isEqualToString:@""];
    self.hasTranslated=self.cellFrame.chatMessage.hasTranslated;
    self.canJoinAlliance = [self.cellFrame.chatMessage isInAlliance] && !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""] && type != IOS_CHANNEL_TYPE_USER;
    self.canSendMail = !self.cellFrame.chatMessage.isSelfMsg && type != IOS_CHANNEL_TYPE_USER;
    self.canBlock = !self.cellFrame.chatMessage.isSelfMsg && ![[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.name :um_BLOCK_LIST];
    
    self.canUnBlock = !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.name :um_BLOCK_LIST];
    self.canBan = !self.cellFrame.chatMessage.isSelfMsg && ![[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.name :um_BAN_LIST]&& ([UserManager sharedUserManager].currentUser.mGmod>1) && type != IOS_CHANNEL_TYPE_USER;
    self.canUnBan = !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.name :um_BAN_LIST] && ([UserManager sharedUserManager].currentUser.mGmod==3) && type != IOS_CHANNEL_TYPE_USER;
    self.canViewBattleReport = ([self.cellFrame.chatMessage isBattleReport] && ![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]) && type != IOS_CHANNEL_TYPE_USER;
    self.canViewDetectReport = ([self.cellFrame.chatMessage isDetectReport] && ![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]) && type != IOS_CHANNEL_TYPE_USER;
    self.canInvite=[self.cellFrame.chatMessage.asn isEqualToString:@""] &&![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""] && [UserManager sharedUserManager].currentUser.allianceRank >=3 && !self.cellFrame.chatMessage.isSelfMsg && type != IOS_CHANNEL_TYPE_USER;
}

-(void)flashCell
{
    UITableView * tableView;
    
    if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
    }else if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
    }else{
        
    }
    
    NSIndexPath * path = [tableView indexPathForCell:self];
    NSArray *indexArray=[NSArray  arrayWithObject:path];
    [tableView  reloadRowsAtIndexPaths:indexArray withRowAnimation:UITableViewRowAnimationAutomatic];
}

-(void)isShowActivityIndicator
{
    if(self.cellFrame.chatMessage.isSelfMsg){
        
        if(self.cellFrame.chatMessage.sendState == 0){
            self.activityIndicator.hidesWhenStopped = NO;
            self.resetSendBtn.hidden = YES;
            [self.activityIndicator startAnimating];
            
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                if(self.cellFrame.chatMessage.sendState == 0){
                    self.cellFrame.chatMessage.sendState = 1;
                    [self.activityIndicator stopAnimating];
                    self.activityIndicator.hidesWhenStopped = YES;
                    self.resetSendBtn.hidden = NO;
                }
            });
        }else if(self.cellFrame.chatMessage.sendState == 2){
            //发送完成
            [self.activityIndicator stopAnimating];
            self.activityIndicator.hidesWhenStopped = YES;
            self.resetSendBtn.hidden = YES;
        }
    }
}

-(void)isShowResetSendBtn
{
    if(self.cellFrame.chatMessage.isSelfMsg){
        if(self.cellFrame.chatMessage.sendState == 1){
            self.activityIndicator.hidesWhenStopped = YES;
            self.resetSendBtn.hidden = NO;
        }
    }
}

//执行重新发送
-(void) exitResetSend
{
    /**状态重置 重发*/
    self.cellFrame.chatMessage.sendState = 0;
    
    
    if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        DVLog(@"国家重发");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendText:self.cellFrame.chatMessage :0];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView reloadData];

    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        DVLog(@"联盟重发");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendText:self.cellFrame.chatMessage :2];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView reloadData];
    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER){
        DVLog(@"邮件重发");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendMail:self.cellFrame.chatMessage];
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView reloadData];
    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM){
        DVLog(@"聊天室重发");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost directlySendMsg:self.cellFrame.chatMessage];
    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL){
        
    }
    

}

-(void)showPlayerInfo:(UITapGestureRecognizer *)gestureRecognizer
{
    
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
        
    UIView *viewClicked=[gestureRecognizer view];
    if (viewClicked== self.icon_headPic) {
       [[ChatServiceController chatServiceControllerSharedManager].gameHost showPlayerInfo:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
    }
}

CG_INLINE CGRect

CGRectMake1(CGFloat x, CGFloat y, CGFloat width, CGFloat height)

{
    
    ServiceInterface *serviceInterface = [ServiceInterface serviceInterfaceSharedManager];
    
    CGRect rect;
    
    rect.origin.x = x * serviceInterface.autoSizeScaleX; rect.origin.y = y * serviceInterface.autoSizeScaleY;
    
    rect.size.width = width * serviceInterface.autoSizeScaleWidth; rect.size.height = height * serviceInterface.autoSizeScaleHeight;
    
    return rect;
    
}

 

-(void)clickLableWithString:(NSString *)vString{
//    DVLog(@"%@",vString);
    
    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    int type = self.cellFrame.chatMessage.channelType;
    if (type == IOS_CHANNEL_TYPE_CHATROOM || type == IOS_CHANNEL_TYPE_USER) {
        [[ChatServiceController chatServiceControllerSharedManager] updateMailList];
    }
    [[ChatServiceController chatServiceControllerSharedManager].gameHost close2MailList];
     vString = [vString stringByReplacingOccurrencesOfString:@"：" withString:@":"];
    NSArray *poslist=[vString componentsSeparatedByString:@":"];
    NSString *posX = [poslist objectAtIndex:0];
    NSString *posY = [poslist objectAtIndex:1];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost skipPos:posX :posY];
}

- (CGSize)sizeThatFits:(CGSize)size
{
    CGFloat totalHeight = 0;

    if (self.cellFrame.isShowUiTopView_BOOL) {
        totalHeight = self.cellFrame.cellHeight + self.cellFrame.uiTopViewRect.size.height;
    }else{
        totalHeight = self.cellFrame.cellHeight;
    }

    return CGSizeMake(size.width, totalHeight);
}
#pragma mark -
#pragma mark zytAlertViewDelegate
- (void)clickEnterButtonWithZYTAlertView:(ZYTAlertView *)vAlertView{
     [[ChatServiceController chatServiceControllerSharedManager].gameHost block:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
}
@end
