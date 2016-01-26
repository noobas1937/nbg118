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
#import "CSGoogleTranslat.h"
#import "UIImageView+EMWebCache.h"
#import "NSAttributedString+Emotion.h"
#import "CSAlertView.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatController.h"
#import "ToolController.h"
#import "ChannelManager.h"
#import "CSAlertRedPackageView.h"

@interface ChatCellIOS()<ChatContentViewDelegate,ZYTAlertViewDelegate,CSRedEnvelopeViewDelegate,CSAlertRedPackageViewDelegate>
@property (nonatomic,strong) UIView *icon;
@property (nonatomic,strong) UIImageView *icon_border;
@property (nonatomic,strong) UIImageView *icon_headPic;
@property (nonatomic,strong) UIImageView *icon_gmod;
@property (nonatomic,strong) UIImageView *icon_king;
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

@property (nonatomic,strong)BanPopUpController *banPopUpController;

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
@property(nonatomic,assign) BOOL     canCheckTheEquip;
@property(nonatomic,assign) BOOL     canHead2Report;
@property(nonatomic,assign) BOOL     canContentofReport;
@property(nonatomic,assign) BOOL     canHello;
@property(nonatomic,assign) BOOL     canRallyMsg;
@property(nonatomic,assign) BOOL     canLotteryInfo;
@property(nonatomic,assign) BOOL     canAllianceTask;



/**
 *  点击红包信息弹出红包view
 */
@property(nonatomic,strong) CSAlertRedPackageView *alertRedPackageView;

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
    
    for (UIView *subView in self.uiViewDown.subviews) {
        [subView removeFromSuperview];
    }
    
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
            
            NSString *headerUrl = [NSString gettingCustomHeadPic:weakcellFrame.chatMessage.uid  intValue:weakcellFrame.chatMessage.headPicVer ];
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
 
    [self initWithCellData:self.cellFrame];
    
    [self setUICGRectMake];
    
    if ([self.chatCellIosDelegate respondsToSelector:@selector(chatMailCellDidSelectedWithCell:)]) {
        [self.chatCellIosDelegate chatMailCellDidSelectedWithCell:self];
    }else{
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
            
            [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
            
        }
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
    if ([self.cellFrame.chatMessage isTranlateDisable]) {
        return;
    }
    
    if (self.cellFrame.chatMessage.isSelfMsg ==YES){
        return;
    }
    if (self.cellFrame.chatMessage.translateMsg.length > 0 && self.cellFrame.chatMessage.translatedLang.length > 0) {
        return;
    }
    __block ChatCellIOS *weakSelf = self;
    
    CSGoogleTranslat *  translat =[[CSGoogleTranslat alloc]init];
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
            
            if (self.chatView.chatMessage.post == 12) {
                UIImage *imageUp = [UIImage imageNamed:@"chat_redenvelope_left_bg_pressed"];
                UIImage *imageDown = [UIImage imageNamed:@"chat_redenvelope_left_bg_normal"];
                [self setChatViewFrame:imageUp :imageDown];
            }else if(self.chatView.chatMessage.post == 200){
               UIImage *  imageUp=[UIImage imageNamed:@"chat_to_bg_normal"];
                UIImage * imageDown=[UIImage imageNamed:@"chat_to_bg_pressed"];
                [self setChatViewFrame:imageUp :imageDown];
            }else{
                UIImage *imageUp = [UIImage imageNamed:@"chat_system_left_bg_normal"];
                UIImage *imageDown = [UIImage imageNamed:@"chat_to_bg_pressed"];
                [self setChatViewFrame:imageUp :imageDown];
            }
            
        }
        //喇叭消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post == 6) {
            //            DVLog(@"左边喇叭消息");
            UIImage *imageUp = [UIImage imageNamed:@"horn_bg_left"];
            UIImage *imageDown = [UIImage imageNamed:@"horn_bg_left"];
            
            [self setChatViewFrame:imageUp :imageDown];
        }
        //别人的消息
        if (self.chatView.chatMessage.post == 0 && !self.chatView.chatMessage.isSelfMsg) {
            UIImage *imageUp;
            UIImage *imageDown;
            
            if ([self.chatView.chatMessage.uid intValue]==[[ChatServiceController chatServiceControllerSharedManager].kingUidString intValue]) {
                imageUp=[UIImage imageNamed:@"cs_king_bubble_left_normal"];
                imageDown=[UIImage imageNamed:@"cs_king_bubble_left_pressed"];
                
            }else{
                imageUp=[UIImage imageNamed:@"chat_to_bg_normal"];
                imageDown=[UIImage imageNamed:@"chat_to_bg_pressed"];
            }
            
            [self setChatViewFrame:imageUp :imageDown];
            //cs_king_bubble_left_normal
            //cs_king_bubble_left_pressed
            
            
        }
    }else{                                          //右边的消息
    
        //系统消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post != 6) {
            if (self.chatView.chatMessage.post == 12) {
                UIImage *imageUp = [UIImage imageNamed:@"chat_redenvelope_right_bg_pressed"];
                
                UIImage *imageDown = [UIImage imageNamed:@"chat_redenvelope_right_bg_normal"];
                [self setChatViewFrame:imageUp :imageDown];
            }else if(self.chatView.chatMessage.post == 200){
                UIImage *  imageUp=[UIImage imageNamed:@"chat_from_bg_normal"];
                UIImage * imageDown=[UIImage imageNamed:@"chat_from_bg_pressed"];
                [self setChatViewFrame:imageUp :imageDown];
            }else{
                UIImage *imageUp = [UIImage imageNamed:@"chat_system_right_bg_normal"];
                UIImage *imageDown = [UIImage imageNamed:@"chat_system_right_bg_pressed"];
                [self setChatViewFrame:imageUp :imageDown];
            }
        }
        //喇叭消息
        if (self.chatView.chatMessage.post != 0 && self.chatView.chatMessage.post == 6) {
            DVLog(@"右边喇叭消息");
            
            UIImage *imageUp = [UIImage imageNamed:@"horn_bg_right"];
            UIImage *imageDown = [UIImage imageNamed:@"horn_bg_right"];
            
            
            [self setChatViewFrame:imageUp :imageDown];
        }
        //自己的消息
        if (self.chatView.chatMessage.post == 0 && self.chatView.chatMessage.isSelfMsg) {
            UIImage *imageUp;
            UIImage *imageDown;
            if ([self.chatView.chatMessage.uid intValue]==[[ChatServiceController chatServiceControllerSharedManager].kingUidString intValue]) {
                imageUp=[UIImage imageNamed:@"cs_king_bubble_right_normal"];
                imageDown=[UIImage imageNamed:@"cs_king_bubble_right_pressed"];
               
                
            }else{
                imageUp=[UIImage imageNamed:@"chat_from_bg_normal"];
                imageDown=[UIImage imageNamed:@"chat_from_bg_pressed"];
            }
            
            [self setChatViewFrame:imageUp :imageDown];
            //cs_king_bubble_right_normal
            //cs_king_bubble_right_pressed
        }
    }
}


-(void) setChatViewFrame:(UIImage *)imageUp :(UIImage *)imageDown
{
    
    //50, 40, 10, 20
    if (self.cellFrame.chatMessage.post == 12) {
        
        
         if ([self.chatView.chatMessage.uid intValue]==[[ChatServiceController chatServiceControllerSharedManager].kingUidString intValue]&&self.chatView.chatMessage.post == 0) {
                   
             self.redEnvelopeView.upImage = [imageUp resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 +1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 -1), floorf(imageUp.size.width/2))resizingMode:UIImageResizingModeStretch];
             
             self.redEnvelopeView.downImage = [imageDown resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 + 1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))resizingMode:UIImageResizingModeStretch];
         }else{
             self.redEnvelopeView.upImage = [imageUp resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 +1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 -1), floorf(imageUp.size.width/2))];
             
             self.redEnvelopeView.downImage = [imageDown resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 + 1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))];
         }
       
        
        UIImage *tempImage = self.redEnvelopeView.downImage;
        self.redEnvelopeView.backgroundImage.image = self.redEnvelopeView.downImage;
    }else{
        if ([self.chatView.chatMessage.uid intValue]==[[ChatServiceController chatServiceControllerSharedManager].kingUidString intValue]&&self.chatView.chatMessage.post == 0) {
            self.chatView.chatImageUp = [imageUp resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 +1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 -1), floorf(imageUp.size.width/2))resizingMode:UIImageResizingModeStretch];
            self.chatView.chatImageDown = [imageDown resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 + 1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))resizingMode:UIImageResizingModeStretch];

        }else{
            
            self.chatView.chatImageUp = [imageUp resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 +1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 -1), floorf(imageUp.size.width/2))];
            self.chatView.chatImageDown = [imageDown resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(imageUp.size.height/2 + 1), floorf(imageUp.size.width/2), floorf(imageUp.size.height/2 - 1), floorf(imageUp.size.width/2))];

        }
        
        
        self.chatView.backImageView.image = self.chatView.chatImageUp;
        
        self.chatView.frame= self.cellFrame.chatViewRect;
        
        //测试线
        //[[self.chatView layer] setBorderWidth:1.0f];
        
        CGFloat x = 0;
        
        if(self.cellFrame.chatMessage.isSelfMsg){
            x=5*1.6;
        }else {
            x=5*3.3;
        }
        
        self.chatView.contentLabel.frame = CGRectMake(x,5, self.cellFrame.chatLabelSize.width,self.cellFrame.chatLabelSize.height);
        if(self.chatView.contentLabel.text == nil){
            self.chatView.contentLabel.text =@"";
        }
        
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
    NSString *unBanItem_Str             = [NSString stringWithMultilingualWithKey:@"105208"];
    NSString *copyItem_Str              = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_COPY];
    NSString *checkTheEquip_Str         = [NSString stringWithMultilingualWithKey:@"111665"];
    NSString *head2Report_Str           = [NSString stringWithMultilingualWithKey:@"105777"];
    NSString *contentofReportStr          = [NSString stringWithMultilingualWithKey:@"105392"];

    /**
     *  打招呼
     */
    NSString *helloStr = [NSString stringWithMultilingualWithKey:@"132121"];
    /**
     *  集结信息
     */
    NSString *rallyMsgStr = [NSString stringWithMultilingualWithKey:@"132119"]; //集结信息
    
    NSString *lotteryInfoStr = [NSString stringWithMultilingualWithKey:@"115018"]; //查看
    
    NSString *receiveTasksStr = [NSString stringWithMultilingualWithKey:@"134058"]; //接取任务
    
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
    UIMenuItem *equipItem = [[UIMenuItem alloc] initWithTitle:checkTheEquip_Str action:@selector(CheckTheEquip_callFun:)];
    UIMenuItem *head2ReportItem = [[UIMenuItem alloc] initWithTitle:head2Report_Str action:@selector(head2Report_callFun:)];
    
    UIMenuItem *contentofReportItem = [[UIMenuItem alloc] initWithTitle:contentofReportStr action:@selector(contentofReport_callfun:)];
    
    UIMenuItem *helloItem = [[UIMenuItem alloc] initWithTitle:helloStr action:@selector(hello_callfun:)];
    
    UIMenuItem *rallyMsgItem = [[UIMenuItem alloc] initWithTitle:rallyMsgStr action:@selector(rallyMsg_fun:)];
    
    UIMenuItem *lotteryInfoItem = [[UIMenuItem alloc] initWithTitle:lotteryInfoStr action:@selector(lotteryInfo_fun:)];
     UIMenuItem *receiveTasksItem = [[UIMenuItem alloc] initWithTitle:receiveTasksStr action:@selector(receiveTasks_fun:)];
 
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        
            UIWindow *window = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
            if ([window isKeyWindow] == NO)
            {
                [window becomeKeyWindow];
                [window makeKeyAndVisible];
            }
            
        
    }else{
        if(ChatServiceCocos2dx::Mail_OC_Native_Enable && (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER ||self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM )){
            UIWindow *window = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
            if ([window isKeyWindow] == NO)
            {
                [window becomeKeyWindow];
                [window makeKeyAndVisible];
            }
            
        }else{
            UIWindow *window = [ServiceInterface serviceInterfaceSharedManager].vc_win;
            if ([window isKeyWindow] == NO)
            {
                [window becomeKeyWindow];
                [window makeKeyAndVisible];
            }
            
        }

    }
    
 
    
//    查看战报｜翻译｜复制｜发送邮件｜屏蔽
    
    NSMutableArray *itemList = [[NSMutableArray alloc]init];
    
    if ([self.cellFrame.chatMessage isMatchingVersion]) {
        if(self.canViewBattleReport){
            [itemList addObject:battleMsgItem];
        }
        
        if (self.canViewDetectReport) {
            [itemList addObject:detectMsgItem];
        }
        
        if (self.canTranlate) {
            if (![self.cellFrame.chatMessage isOriginalSameAsTargetLang]) {
                if (self.hasTranslated) {
                    [itemList addObject:originalItem];
                }else{
                    [itemList addObject:translateItem];
                }
            }
        }
        
        if (self.cellFrame.chatMessage.post == 0) {
            [itemList addObject:copyItem];
        }
        
        if(self.canSendMail){
            /**
             *  根据需求普通消息去除发送邮件发送邮件item
             */
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
        
        if (self.canCheckTheEquip) {
            [itemList addObject:equipItem];
        }
        
        if (self.canHead2Report) {
            [itemList addObject:head2ReportItem];
        }
        
        if (self.canContentofReport){
            [itemList addObject:contentofReportItem];
        }
        
        if (self.canHello) {
            [itemList addObject:helloItem];
        }
        
        if (self.canRallyMsg) {
            [itemList addObject:rallyMsgItem];
        }
        
        if (self.canLotteryInfo) {
            [itemList addObject:lotteryInfoItem];
        }
        
        if (self.canAllianceTask){
            [itemList addObject:receiveTasksItem];
        }
    }
    
    [uiMenu setMenuItems:itemList];
    
//    NSArray *array = [NSArray arrayWithObjects:invitejoin_Item, joinAlliance_Item, sendMail_Item, originalItem, translateItem, battleMsgItem, detectMsgItem, blockItem, unBlockItem, banItem, unBanItem, copyItem, nil];
//    
//    [uiMenu setMenuItems:array];
     [self becomeFirstResponder];
    
    
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
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        if ([self.chatCellIosDelegate respondsToSelector:@selector(invitejoinByMsg:)]){
            [self.chatCellIosDelegate invitejoinByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost invitejoin:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
    }
}

//加入联盟
- (void)joinAlliance_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        if ([self.chatCellIosDelegate respondsToSelector:@selector(joinAllianceByMsg:)]){
            [self.chatCellIosDelegate joinAllianceByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost joinAlliance:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
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
 
        DVLog(@"源语言");
        self.cellFrame.chatMessage.showTransLate = YES;
        self.cellFrame.chatMessage.hasTranslated = FALSE;
        
        self.cellFrame.chatMessage.isNewMsg = FALSE;
        [self.cellFrame.chatMessage settingShowMsg:nil];
        [self.cellFrame setChatMessage:self.cellFrame.chatMessage];
        
        //邮件代理
        if ([self.chatCellIosDelegate respondsToSelector:@selector(chatMailCellDidSelectedWithCell:)]) {
            [self.chatCellIosDelegate chatMailCellDidSelectedWithCell:self];
        }else{
        //国家联盟
            if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
                if ([self.chatCellIosDelegate respondsToSelector:@selector(originalMsgByMsg:)]){
                    [self.chatCellIosDelegate originalMsgByMsg:self ];
                }
            }else{
                UITableView * tableView = nil;
                
                if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
                    tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
                }else if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                    tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
                }else if(self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM){
                    tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
                }
                
                NSIndexPath *indexPath = [tableView indexPathForCell:self];
                
                [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
            }
        }
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
            NSString *localLanString = [NSString stringWithUTF8String:LocalController::shared()->getLanguageFileName().c_str()] ;
            [self.cellFrame.chatMessage settingShowMsg:localLanString];
            [self.cellFrame setChatMessage:self.cellFrame.chatMessage];
            
            /**
             *  区分新版旧版
             *
             */
            
            //邮件cell
            if([self.chatCellIosDelegate respondsToSelector:@selector(chatMailCellDidSelectedWithCell:)]){
                [self.chatCellIosDelegate chatMailCellDidSelectedWithCell:self];
            }else{
                //国家联盟cell
                if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
                    if ([self.chatCellIosDelegate respondsToSelector:@selector(tranlateMsgByMsg:)]) {
                        [self.chatCellIosDelegate tranlateMsgByMsg:self ];
                    }
                }else{
                    UITableView * tableView = nil;
                    if (self.cellFrame.chatMessage.channelType == 0) {
                        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
                    }else if(self.cellFrame.chatMessage.channelType == 1){
                        tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
                    }else if(self.cellFrame.chatMessage.channelType == 2 || self.cellFrame.chatMessage.channelType == 3){
                        
                        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
                            tableView = [ServiceInterface serviceInterfaceSharedManager].mailChatVC.mailChatTableView;
                        }else{
                            tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
                        }
                    }
                    NSIndexPath *indexPath = [tableView indexPathForCell:self];
                    
                    [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];

                }
            }
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
            CSGoogleTranslat *translat =[[CSGoogleTranslat alloc]init];
            [translat translationingWithText:self.cellFrame.chatMessage.msg andWihtSL:@"auto" andWithTL:localLanString andResultBlock:^(ZYTGoogleTranslatHTTPStatus status, NSString *resultString, NSString *orgLangTypeString) {
                if (status == ZYTGoogleTranslatHTTPStatus_sucess){
                    DVLog(@"%@",resultString);
                    if (resultString.length>0) {
                        self.cellFrame.chatMessage.hasTranslated = TRUE;
                        tempCellFrame.chatMessage.translateMsg = resultString;
                        
                        ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingChannelInfo:tempCellFrame.chatMessage.channelIDString];
                        
                        [channel msgUpdateWithMsgItem:tempCellFrame.chatMessage andWithPropertyName:@[@"translateMsg"]];
                        
                        
                        tempCellFrame.chatMessage.originalLang = orgLangTypeString;
                        tempCellFrame.chatMessage.translatedLang = localLanString;
                        //                weakSelf.cellFrame.chatMessage.showMsg = [weakSelf.cellFrame.chatMessage gettingShowMsg];
                        //
                        //                [weakSelf.cellFrame initWithData:weakSelf.cellFrame.chatMessage];
                        [self.cellFrame.chatMessage settingShowMsg:localLanString];
                        [weakSelf.cellFrame setChatMessage:tempCellFrame.chatMessage];
                        
                        /**
                         *  区分新旧版邮件执行的控制器
                         *
 
                         */
                        if([self.chatCellIosDelegate respondsToSelector:@selector(chatMailCellDidSelectedWithCell:)]){
                            [self.chatCellIosDelegate chatMailCellDidSelectedWithCell:self];
                        }else{
                            UITableView * tableView ;
                            if (weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
                                tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView;
                            }else if(weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                                tableView = [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView;
                            }else if(weakSelf.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_USER){
                                if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
                                    tableView = [ServiceInterface serviceInterfaceSharedManager].mailChatVC.mailChatTableView;
                                }else{
                                    tableView = [ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView;
                                }
                            }
                            NSIndexPath *indexPath = [tableView indexPathForCell:weakSelf];
                            
                            [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:indexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
                        }
                    }
                }
            }];
        }
}

//查看战报
- (void)battleMsg_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(lookBattleMsgByMsg:)]){
            [self.chatCellIosDelegate lookBattleMsgByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost battleMsg:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.attachmentId];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}

//侦查战报
- (void)detectMsg_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(lookDetectMsgByMsg:)]) {
            [self.chatCellIosDelegate lookDetectMsgByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost detectMsg:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.attachmentId];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}

//屏蔽
- (void)block_callFun:(id)sender
{
    DVLog(@"禁言");
    
    NSString * tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_SHIELD_PLAYER :self.cellFrame.chatMessage.name];
    CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
    alertView.viewType = BLOCK;
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    alertView.alertViewDelegate = self;
    
    if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
            if ([self.chatCellIosDelegate respondsToSelector:@selector(alertViewByView:)]) {
                [self.chatCellIosDelegate alertViewByView:alertView];
            }
        }else{
            UIViewController *recentView = nil;
            recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
            
            while (recentView.parentViewController != nil) {
                recentView = recentView.parentViewController;
            }
            
            [recentView.view addSubview:alertView];
        }
    }else{
        [self alertUI2NewMailByView:alertView];
    }
    
}

//解除屏蔽
- (void)unBlock_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(unBlockPlayerByMsg:)]){
            [self.chatCellIosDelegate unBlockPlayerByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBlock:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
        [[UserManager sharedUserManager] removeRestrictUser:self.cellFrame.chatMessage.name :1];
    }
}

//禁言
- (void)ban_callFun:(id)sender
{
    self.banPopUpController = [[BanPopUpController alloc] initWithNibName:@"BanPopUpController" bundle:nil];

    [self.banPopUpController setInitData:self.cellFrame.chatMessage];
    
    if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY)
    {
        if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
            if ([self.chatCellIosDelegate respondsToSelector:@selector(alertBanPlayerViewByView:)]) {
                [self.chatCellIosDelegate alertBanPlayerViewByView:self.banPopUpController.view];
            }
        }else{
            [[ServiceInterface serviceInterfaceSharedManager].vc_win addSubview:self.banPopUpController.view];
        }
    }else{
        UIViewController *recentView = nil;
        
        UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
        [nav.topViewController.view addSubview:self.banPopUpController.view];
        [nav.topViewController.view bringSubviewToFront:self.banPopUpController.view];
    }
}

//解除禁言
- (void)unBan_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(unBanPlayerByMsg:)]) {
            [self.chatCellIosDelegate unBanPlayerByMsg:self.cellFrame.chatMessage];
        }
    }else{
        DVLog(@"解除禁言");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBan:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
    }
}

//copy
- (void)copy_callFun:(id)sender {
    DVLog(@"copy_callFun");
    NSString *contentStr;
    contentStr = self.cellFrame.chatMessage.showMsg;
    [[UIPasteboard generalPasteboard]setString:contentStr];
}

/**
 *  查看装备回调
 *
 *  @param sender
 */
- (void)CheckTheEquip_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(lookEquipmentByMsg:)]){
            [self.chatCellIosDelegate lookEquipmentByMsg:self.cellFrame.chatMessage];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewEquipment:self.cellFrame.chatMessage.attachmentId];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}

/**
 *  举报头像
 *
 *  @param sender
 */
-(void)head2Report_callFun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(reportPlayerHeadByMsg:)]) {
            [self.chatCellIosDelegate reportPlayerHeadByMsg:self.cellFrame.chatMessage];
        }
    }else{
        NSString *uid = self.cellFrame.chatMessage.uid;
        if (self.cellFrame.chatMessage.uid.length > 0) {
            if ([[UserManager sharedUserManager] isUid2ReportList:uid]) {
                NSString *content = [NSString stringWithMultilingualWithKey:@"105782"];
                [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :content :0.0 :0.0 :true];
            }else{
                [[UserManager sharedUserManager] addReportList:uid];
                [[ChatServiceController chatServiceControllerSharedManager].gameHost reportCustomHeadPic:self.cellFrame.chatMessage.uid];
            }
        }
    }
}

/**
 *  举报内容
 *
 *  @param sender <#sender description#>
 */
-(void)contentofReport_callfun:(id)sender
{
    
    NSString * tipStr = [NSString stringWithMultilingualWithKey:@"105393"];
    CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
    alertView.viewType = CONTENTOFTHEREPORT;
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    alertView.alertViewDelegate = self;
    
    if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
            if ([self.chatCellIosDelegate respondsToSelector:@selector(alertViewByView:)]){
                [self.chatCellIosDelegate alertViewByView:alertView];
            }
        }else{
            UIViewController *recentView = nil;
            
            if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
                recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
                while (recentView.parentViewController != nil) {
                    recentView = recentView.parentViewController;
                }
                
                [recentView.view addSubview:alertView];
            }else{
                [self alertUI2NewMailByView:alertView];
            }
        }
    }else{
        [self alertUI2NewMailByView:alertView];
    }
}
/**
 *  打招呼
 *
 *  @param sender <#sender description#>
 */
-(void)hello_callfun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(sayHelloByMsg:)]) {
            [self.chatCellIosDelegate sayHelloByMsg:self.cellFrame.chatMessage];
        }
    }else{
        int boundary = [self.cellFrame.chatMessage.helloStrs count];
        NSString *helloStr =  self.cellFrame.chatMessage.helloStrs[arc4random() % boundary];
        if ([self.chatCellIosDelegate respondsToSelector:@selector(chatcellIosSaiHello:andSayString:)]){
            [self.chatCellIosDelegate chatcellIosSaiHello:self andSayString:helloStr];
        }
        if (helloStr.length > 0) {
            [ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView.chatViewTextField.text = helloStr;
        }
    }
}

/**
 *  集结回调
 *
 *  @param sender <#sender description#>
 */
-(void)rallyMsg_fun:(id)sender
{
    
    DVLog(@"")
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(rallyArmyByMsg:)]) {
            [self.chatCellIosDelegate rallyArmyByMsg:self.cellFrame.chatMessage];
        }
    }else{
        DVLog(@"rallyMsg_fun");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewRallyInfoByStr:self.cellFrame.chatMessage.attachmentId];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}
//轮盘分享
-(void)lotteryInfo_fun:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(lotteryShareByMsg:)]){
            [self.chatCellIosDelegate lotteryShareByMsg:self.cellFrame.chatMessage];
        }
    }else{
        DVLog(@"lotteryInfo_fun");
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewLotteryInfoById:self.cellFrame.chatMessage.attachmentId];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}

//接收任务
-(void)receiveTasks_fun:(id)sender
{
    
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
        if ([self.chatCellIosDelegate respondsToSelector:@selector(acceptTasksByMsg:)]) {
            [self.chatCellIosDelegate acceptTasksByMsg:self.cellFrame.chatMessage];
        }
    }else{
        DVLog(@"receiveTasks_fun");
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"allianceTask" :@"" :@"" :@"" :@"" :0 :TRUE];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:self.cellFrame.chatMessage.channelType];
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    }
}


/**重新发送*/
-(void) resetChatPressed:(id)sender
{
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        if ([self.chatCellIosDelegate respondsToSelector:@selector(resetSendMsgByMsg:)]) {
            [self.chatCellIosDelegate resetSendMsgByMsg:self];
        }
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost resetSend:self];
    }
}

//YES item成为第一响应者
- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    
    //邀请加入联盟
    if(action == @selector(invitejoin_callFun:) || action == @selector(joinAlliance_callFun:) || action == @selector(sendMail_callFun:) || action == @selector(original_callFun:) || action == @selector(translate_callFun:) || action == @selector(battleMsg_callFun:) || action == @selector(detectMsg_callFun:) || action == @selector(block_callFun:) || action == @selector(unBlock_callFun:) || action == @selector(ban_callFun:) || action == @selector(unBan_callFun:) || action == @selector(copy_callFun:) || action == @selector(CheckTheEquip_callFun:) || action == @selector(head2Report_callFun:) || action == @selector(contentofReport_callfun:) || action == @selector(hello_callfun:) || action == @selector(rallyMsg_fun:) || action == @selector(lotteryInfo_fun:) || action == @selector(receiveTasks_fun:)){
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
    
    int type = self.cellFrame.chatMessage.channelType;
    
    self.userInfoLabel = [[UILabel alloc]init];
    
    UIColor *color= [UIColor colorWithRed:72/255.0 green:44/255.0 blue:19/255.0 alpha:1];
    
    self.userInfoLabel.textColor = color;
    
    self.userInfoLabel_fontSize = [ServiceInterface serviceInterfaceSharedManager].userInfofontSize;
    
    
    
    //icon
    
    self.icon=[[UIView alloc]init];
    
    self.icon_border=[[UIImageView alloc]init];
    
    self.icon_headPic=[[UIImageView alloc]init];
    
    self.icon_gmod=[[UIImageView alloc]init];
    
    self.icon_king=[[UIImageView alloc]init];
    self.icon_king.backgroundColor=[UIColor clearColor];
    self.icon_king.hidden=YES;
    
    //风火轮
    self.activityIndicator = [[UIActivityIndicatorView alloc]
                              initWithActivityIndicatorStyle:
                              UIActivityIndicatorViewStyleWhite];
    //重发按钮
    
    self.resetSendBtn = [[UIButton alloc]init];
    self.activityIndicator.frame = self.cellFrame.activityIndicatorRect;
    self.resetSendBtn.frame = self.cellFrame.resetSendBtnRect;
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
    if (self.cellFrame.chatMessage.createTime > 0) {
        
        NSString *time = [[self.cellFrame.chatMessage nsDataSwitchNSString:self.cellFrame.chatMessage.createTime] substringToIndex:10];
        self.timeLabel.text=time;
        self.timeLabel.font=[UIFont systemFontOfSize:self.time_fontSize];
        self.timeLabel.textAlignment = NSTextAlignmentCenter;
        
        UIColor *color= [UIColor colorWithRed:87/255.0 green:77/255.0 blue:66/255.0 alpha:1];
        
        self.timeLabel.textColor = color;
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
    
    if ([self.chatView.chatMessage.uid intValue]==[[ChatServiceController chatServiceControllerSharedManager].kingUidString intValue] && self.chatView.chatMessage.post == 0 ) {
        self.icon_border.image = [UIImage imageNamed:@"cs_king_headImageBack"];
        self.icon_king.hidden=NO;
        self.chatView.contentLabel.textColor=[UIColor colorWithRed:149/255.0 green:138/255.0 blue:120/255.0 alpha:1 ];
    }else{
        self.icon_border.image = [UIImage imageNamed:@"icon_kuang.png"];
        self.icon_king.hidden=YES;
        self.chatView.contentLabel.textColor=[UIColor blackColor];

    }
    
    self.icon_king.image=[UIImage imageNamed:@"cs_king_head_diadema"];
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
    
//    if([self.cellFrame.chatMessage isModMsg])
//    {
//        self.icon_gmod.image = [UIImage imageNamed:@"mod.png"];
//    }else{
        self.icon_gmod.image = [UIImage imageNamed:[self.cellFrame.chatMessage gettingGmodStr]];
//    }
    
    
    
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
    
    if ([self.cellFrame.chatMessage isMatchingVersion]) {
        if (self.cellFrame.chatMessage.post == 7 || self.cellFrame.chatMessage.post == 11) {
            [self coloring2EquipStr:attributedString];
        }else{
            self.chatView.contentLabel.attributedText = attributedString;
        }
    }else{
        self.chatView.contentLabel.attributedText = attributedString;
    }
    
    
    
//    self.chatView.contentLabel.frame = CGRectMake(0, 0, self.cellFrame.chatLabelSize.width, self.cellFrame.chatLabelSize.height);
//    [self.chatView.contentLabel sizeToFit];
    //重发按钮配置
    [self.resetSendBtn setBackgroundImage:[UIImage imageNamed:@"sendfailed"] forState:UIControlStateNormal];
    [self.resetSendBtn addTarget:self action:@selector(resetChatPressed:) forControlEvents: UIControlEventTouchUpInside];
    self.resetSendBtn.hidden = YES;


}
/**
 *  装备分享 设置字体颜色
 *
 *  @param attributedString <#attributedString description#>
 */
-(void) coloring2EquipStr :(NSAttributedString *)attributedString
{
    NSString *colorStr = [NSString stringWithMultilingualWithKey:self.cellFrame.chatMessage.changeColor2String];
    
    NSArray *colorRGB = [self.cellFrame.chatMessage.fontColorStr componentsSeparatedByString:@"|"];
    
    if (colorRGB.count == 3) {
        float r = ((NSString*)colorRGB[0]).floatValue;
        
        float g = ((NSString*)colorRGB[1]).floatValue;
        
        float b = ((NSString*)colorRGB[2]).floatValue;
        
        
        UIColor *color= [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1];
        
        NSMutableAttributedString *mutableAttributeString = [[NSMutableAttributedString alloc] initWithAttributedString:attributedString];
        
        NSString *anotherString=[mutableAttributeString string];
        
        NSRange range = [anotherString rangeOfString:colorStr];
        
        [mutableAttributeString addAttribute:NSForegroundColorAttributeName value:color range:NSMakeRange(range.location,range.length)];
        
        self.chatView.contentLabel.attributedText = attributedString;
        
        [self.chatView.contentLabel settingNSMutableAttributedString:mutableAttributeString :range];
    }

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
    
    self.icon_king.frame=CGRectMake(self.icon_border.origin.x+self.icon_border.frame.size.width/6, self.icon_border.origin.y- (self.icon_border.frame.size.width*2/3)*39/72, self.icon_border.frame.size.width*2/3, (self.icon_border.frame.size.width*2/3)*39/72);
    self.icon_gmod.frame=self.cellFrame.gmodRect;
    
    //icon backgroundColor
    self.icon.backgroundColor = [UIColor clearColor];
    self.icon_border.backgroundColor = [UIColor clearColor];
    self.icon_headPic.backgroundColor = [UIColor clearColor];
    self.icon_gmod.backgroundColor = [UIColor clearColor];
    
    //风火轮大小
    self.activityIndicator.frame = self.cellFrame.activityIndicatorRect;
    //重发按钮大小
    self.resetSendBtn.frame = self.cellFrame.resetSendBtnRect;

}

-(void) cellAddSubview
{
    //根据当前cell的time ＝＝ 上一个cell的time 等来显示时间
    if (self.cellFrame.isShowUiTopView_BOOL ) {
        [self.contentView addSubview:self.uiViewTop];
        self.timeLabel.center = CGPointMake(self.uiViewTop.bounds.size.width/2,self.uiViewTop.bounds.size.height/2);
//        self.timeLabel.backgroundColor = [UIColor colorWithRed:146.0/255.0 green:126.0/255.0 blue:102.0/255.0 alpha:1.0];
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
    [self.icon addSubview:self.icon_king];
    [self.icon addSubview:self.icon_gmod];
    
    //添加风火轮
    [self.uiViewDown addSubview:self.activityIndicator];
    
    //添加重发按钮
    [self.uiViewDown addSubview:self.resetSendBtn];
    
    if (self.cellFrame.chatMessage.post == 12) {
        self.redEnvelopeView = [[CSRedEnvelopeView alloc] initWithFrame:self.cellFrame.redEnvelopeRect addWithIsSelf:self.cellFrame.chatMessage];
        self.redEnvelopeView.redEnvelopeViewDelegate = self;
        [self.uiViewDown addSubview:self.redEnvelopeView];
    }else{
        
        [self.uiViewDown addSubview:self.chatView];
    }
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
    
    int type = self.cellFrame.chatMessage.channelType;
    
    self.canTranlate = !self.cellFrame.chatMessage.isSelfMsg && (self.cellFrame.chatMessage.post != 4 && self.cellFrame.chatMessage.post != 5 &&self.cellFrame.chatMessage.post != 7 && self.cellFrame.chatMessage.post != 9 && self.cellFrame.chatMessage.post != 10 && self.cellFrame.chatMessage.post != 11);
    
    self.hasTranslated=self.cellFrame.chatMessage.hasTranslated;
    
    self.canJoinAlliance = [self.cellFrame.chatMessage isInAlliance] && !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""] && type != IOS_CHANNEL_TYPE_USER && self.cellFrame.chatMessage.post == 0;
    
    self.canSendMail = !self.cellFrame.chatMessage.isSelfMsg && type != IOS_CHANNEL_TYPE_USER && self.cellFrame.chatMessage.post != 0 && FALSE;
    
    self.canBlock = !self.cellFrame.chatMessage.isSelfMsg && ![[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.uid :um_BLOCK_LIST] && self.cellFrame.chatMessage.post == 0;
    
    self.canUnBlock = !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.uid :um_BLOCK_LIST] && self.cellFrame.chatMessage.post == 0;
    
    self.canBan = !self.cellFrame.chatMessage.isSelfMsg && ![[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.uid :um_BAN_LIST] && ([UserManager sharedUserManager].currentUser.mGmod > 1 && [UserManager sharedUserManager].currentUser.mGmod != 11) && self.cellFrame.chatMessage.post == 0;
    
    self.canUnBan = !self.cellFrame.chatMessage.isSelfMsg && [[UserManager sharedUserManager] isInRestrictList:self.cellFrame.chatMessage.uid :um_BAN_LIST] && ([UserManager sharedUserManager].currentUser.mGmod > 1 && [UserManager sharedUserManager].currentUser.mGmod != 11) && self.cellFrame.chatMessage.post == 0;
    
    self.canViewBattleReport = (([self.cellFrame.chatMessage isBattleReport] && ![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]) && type != IOS_CHANNEL_TYPE_USER ) && (self.cellFrame.chatMessage.post == 4 || self.cellFrame.chatMessage.post == 5);
    
    self.canViewDetectReport = (([self.cellFrame.chatMessage isDetectReport] && ![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]) && type != IOS_CHANNEL_TYPE_USER) && (self.cellFrame.chatMessage.post == 4 || self.cellFrame.chatMessage.post == 5);
    
    self.canInvite=[self.cellFrame.chatMessage.asn isEqualToString:@""] &&![[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""] && [UserManager sharedUserManager].currentUser.allianceRank >=3 && !self.cellFrame.chatMessage.isSelfMsg && type != IOS_CHANNEL_TYPE_USER && type != IOS_CHANNEL_TYPE_ALLIANCE && self.cellFrame.chatMessage.post == 0;
    
    self.canCheckTheEquip = [self.cellFrame.chatMessage ischeckTheEquip] && [UserManager sharedUserManager].currentUser.allianceId.length > 0 && self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE && self.cellFrame.chatMessage.post == 7;
    
    self.canHead2Report = [[self.cellFrame.chatMessage gettingUserInfo] isCustomHeadImage] && !self.cellFrame.chatMessage.isSelfMsg && self.cellFrame.chatMessage.post == 0;
    
    self.canContentofReport = !self.cellFrame.chatMessage.isSelfMsg && self.cellFrame.chatMessage.post == 0;
    
    self.canHello = !self.cellFrame.chatMessage.isSelfMsg && self.cellFrame.chatMessage.post == 8;
    
    self.canRallyMsg = self.cellFrame.chatMessage.post == 9;
    
    self.canLotteryInfo = self.cellFrame.chatMessage.post == 10;
    
    self.canAllianceTask = self.cellFrame.chatMessage.post == 11 && [self.cellFrame.chatMessage isAcceptTask] && !self.cellFrame.chatMessage.isSelfMsg;
}

/**
 * TODO 修改为 (post > 0 && post <= MSG_TYPE_ALLIANCETASK_SHARE)，以避免item.isSystemMessage() && !item.isHornMessage()这种写法
 */
-(BOOL) isSystemMessage
{
    return self.cellFrame.chatMessage.post > 0 && self.cellFrame.chatMessage.post<=11 ;
}

-(BOOL) isHornMessage
{
    return self.cellFrame.chatMessage.post == 6;
}

-(BOOL) isTipMsg
{
    return self.cellFrame.chatMessage.post == 100;
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
        
        if ([self.chatCellIosDelegate respondsToSelector:@selector(chatMailCellDidSelectedWithCell:)])
        {
            [self.chatCellIosDelegate chatMailCellDidSelectedWithCell:self];
        }else{
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.tableView reloadData];
        }
    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM){
        DVLog(@"聊天室重发");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost directlySendMsg:self.cellFrame.chatMessage];
    }else if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL){
        
    }
}

-(void)showPlayerInfo:(UITapGestureRecognizer *)gestureRecognizer
{
    /**
     *  点击头像
     */
    
    int channel = self.cellFrame.chatMessage.channelType;
    
    if (channel == IOS_CHANNEL_TYPE_COUNTRY || channel == IOS_CHANNEL_TYPE_ALLIANCE) {
        if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
            UIView *viewClicked=[gestureRecognizer view];
            if (viewClicked== self.icon_headPic) {
                if ([self.chatCellIosDelegate respondsToSelector:@selector(onClickHeadByMsg:)]){
                    [self.chatCellIosDelegate onClickHeadByMsg:self.cellFrame.chatMessage];
                }
            }
            return ;
        }else{
            [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView.chatViewTextField resignFirstResponder];
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        }
    }else{
        
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            if ([self.chatCellIosDelegate respondsToSelector:@selector(chatNewMailCellDidCloseKey)]) {
                [self.chatCellIosDelegate chatNewMailCellDidCloseKey];
                [[ServiceInterface serviceInterfaceSharedManager] hideMailListIOS];
            }
        }else{
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController.keyBordView.chatViewTextField resignFirstResponder];
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        }
    }
    [self popUpCocosPlayerInfo:gestureRecognizer];
}

-(void) popUpCocosPlayerInfo:(UITapGestureRecognizer *)gestureRecognizer
{
    UIView *viewClicked=[gestureRecognizer view];
    if (viewClicked== self.icon_headPic) {
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showPlayerInfo" :self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name :@"" :@"" :0 :TRUE ];
    }
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:self.cellFrame.chatMessage.channelType];
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
    
    if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
            if ([self.chatCellIosDelegate respondsToSelector:@selector(onClickPointByPoint:)]) {
                [self.chatCellIosDelegate onClickPointByPoint:vString];
            }
            return ;
        }else{
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        }
    }else{
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
            [[ServiceInterface serviceInterfaceSharedManager] closed_The_OC_Native_mailVC];
        }else{
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        }
    }
    
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

    if (self.cellFrame.chatMessage.post == 100) {
        return CGSizeMake(size.width, self.cellFrame.chatSystemSize.height+10);
    }else{
        if (self.cellFrame.isShowUiTopView_BOOL) {
            totalHeight = self.cellFrame.cellHeight + self.cellFrame.uiTopViewRect.size.height;
        }else{
            totalHeight = self.cellFrame.cellHeight;
        }
        
        return CGSizeMake(size.width, totalHeight);
    }
    
}
#pragma mark -
#pragma mark zytAlertViewDelegate
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView{
    
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        if ([self.chatCellIosDelegate respondsToSelector:@selector(clickEnterButton2AlertViewByType:addWithMsg:)]) {
            DVLog(@"clickEnterButtonWithZYTAlertView")
            [self.chatCellIosDelegate clickEnterButton2AlertViewByType:vAlertView.viewType addWithMsg:self.cellFrame.chatMessage];
        }
    }else{
        switch (vAlertView.viewType) {
            case BLOCK:
                [[ChatServiceController chatServiceControllerSharedManager].gameHost block:self.cellFrame.chatMessage.uid :self.cellFrame.chatMessage.name];
                break;
            case CONTENTOFTHEREPORT:
                [[ChatServiceController chatServiceControllerSharedManager].gameHost contentofReportWithUid:self.cellFrame.chatMessage.uid withContent:self.cellFrame.chatMessage.msg];
                break;
            default:
                break;
        }
    }
}
/**
 *  弹出ios对话框 挂载到新版邮件根控制器上
 *
 *  @param alertView <#alertView description#>
 */
-(void) alertUI2NewMailByView:(CSAlertView*) alertView
{
    UIViewController *recentView = nil;
    
    UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
    [nav.topViewController.view addSubview:alertView];
    [nav.topViewController.view bringSubviewToFront:alertView];
}

-(void)onClickRedPackageMsg
{
    if (self.cellFrame.chatMessage.isSelfMsg || [self.cellFrame.chatMessage gettingRedPackageStatus] == 0){
        [self lookRedPackage];
    }else{
        UIViewController *recentView = nil;
        if (self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
            CGSize winSize=[UIScreen mainScreen].bounds.size;
            
            float x = winSize.width * 0.08;
            float y = winSize.height * 0.05;
            if (self.alertRedPackageView) {
                [self.alertRedPackageView removeFromSuperview];
            }
            self.alertRedPackageView = [[CSAlertRedPackageView alloc] initWithFrame:CGRectMake(x,y,winSize.width * 0.9 ,winSize.height * 0.9) AndWith:self.cellFrame.chatMessage];
//            self.alertRedPackageView.center = CGPointMake(x,y);
//            self.alertRedPackageView.bounds = CGRectMake(0.0f, 0.0f, winSize.width * 0.5, winSize.width * 0.5);
            self.alertRedPackageView.alertRedPackageViewDelegate = self;
            recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
            
            while (recentView.parentViewController != nil) {
                recentView = recentView.parentViewController;
            }
            
            [recentView.view addSubview:self.alertRedPackageView];
            
        }
    }
}

-(void)lookRedPackageByMsg:(NSMsgItem *)msgItem{
    DVLog(@"chatcellios::lookRedPackageByMsg");
    [self lookRedPackage];
}

-(void)openRedPackageByMsg:(NSMsgItem *)msgItem{
    DVLog(@"chatcellios::openRedPackageByMsg");
    
    NSArray *arr = [msgItem.attachmentId componentsSeparatedByString:@"|"];
    NSArray *arr2 = [arr[0] componentsSeparatedByString:@"_"];
    
    if ([arr2 count] == 2) {
        NSString *attachmentId = arr2[0];
        NSString *serverId = arr2[1];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingRedPackageStatusById:attachmentId withServerId:serverId ];
    }

}

-(void) lookRedPackage{
    NSArray * arr = [self.cellFrame.chatMessage.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
        NSString *redPacketsId = (NSString*)id_serverArr[0];
        NSString *serverId = (NSString*)id_serverArr[1];
        
        if (redPacketsId.length > 0 && serverId.length > 0) {
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:self.cellFrame.chatMessage.channelType];
        }
    }
    [self.alertRedPackageView removeFromSuperview];
    self.alertRedPackageView = nil;
}

-(void) popUpCocosRedPackageByMsg:(NSMsgItem *)msgItem{
    NSArray * arr = [self.cellFrame.chatMessage.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
        NSString *redPacketsId = (NSString*)id_serverArr[0];
        NSString *serverId = (NSString*)id_serverArr[1];
        
        if (redPacketsId.length > 0 && serverId.length > 0) {
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"getRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:self.cellFrame.chatMessage.channelType];
        }
    }
    [self.alertRedPackageView removeFromSuperview];
    self.alertRedPackageView = nil;
}
@end
