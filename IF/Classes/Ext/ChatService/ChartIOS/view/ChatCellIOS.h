//
//  ChatCell.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "ChatContentView.h"
#import "CSRedEnvelopeView.h"
#import "CSAlertView.h"
@class ChatCellIOS;
 
#import "ChatCellFrame.h"
#define TESTBORDERWIDTH 0.5

@protocol ChatCellIOSDelegate <NSObject>
-(void)ChatCellIOSTransLateWithCell:(ChatCellIOS *)vChatCellIOS;
-(void)chatcellIosSaiHello:(ChatCellIOS *)vChatCell andSayString:(NSString *)vString;
-(void)chatMailCellDidSelectedWithCell:(ChatCellIOS *)vChatMailCell ;
/**
 *  点击cell关闭新版邮件的键盘
 */
-(void) chatNewMailCellDidCloseKey;
/**
 *  邀请加入联盟
 *
 *  @param mi
 */
-(void) invitejoinByMsg:(NSMsgItem*) mi;
/**
 *  加入联盟
 *
 *  @param mi 
 */
-(void) joinAllianceByMsg:(NSMsgItem*) mi;
/**
 *  查看战报
 *
 *  @param mi
 */
-(void) lookBattleMsgByMsg:(NSMsgItem*) mi;
/**
 *  侦查战报
 *
 *  @param mi
 */
-(void) lookDetectMsgByMsg:(NSMsgItem*) mi;
/**
 *  解除屏蔽玩家
 *
 *  @param mi
 */
-(void) unBlockPlayerByMsg:(NSMsgItem*) mi;
/**
 *  解除禁言玩家
 *
 *  @param mi
 */
-(void) unBanPlayerByMsg:(NSMsgItem*) mi;
/**
 *  查看装备
 *
 *  @param mi
 */
-(void) lookEquipmentByMsg:(NSMsgItem*) mi;
/**
 *  举报玩家头像
 *
 *  @param mi
 */
-(void) reportPlayerHeadByMsg:(NSMsgItem*) mi;
/**
 *  打招呼
 *
 *  @param mi 
 */
-(void) sayHelloByMsg:(NSMsgItem*) mi;
/**
 *  查看集结信息
 *
 *  @param mi 
 */
-(void) rallyArmyByMsg:(NSMsgItem*) mi;
/**
 *  轮盘分享
 *
 *  @param mi
 */
-(void) lotteryShareByMsg:(NSMsgItem*) mi;
/**
 *  接受联盟任务
 *
 *  @param mi
 */
-(void) acceptTasksByMsg:(NSMsgItem*) mi;
/**
 *  从新发送信息
 *
 *  @param mi 
 */
-(void) resetSendMsgByMsg:(ChatCellIOS*) vCell;
/**
 *  弹出视图
 */
-(void) alertViewByView:(CSAlertView *)alertView;
/**
 *  弹出禁言玩家视图
 *
 *  @param view 弹出视图
 */
-(void) alertBanPlayerViewByView:(UIView *)view;
/**
 *  CSAlertView 点击事件
 *
 *  @param type CSAlertView视图类型 枚举值
 *  @param mi   数据
 */
-(void) clickEnterButton2AlertViewByType:(ViewType) type addWithMsg:(NSMsgItem*)mi;
/**
 *  翻译回调
 *
 *  @param mi 信息数据源
 */
-(void) tranlateMsgByMsg:(ChatCellIOS*)vCell;
/**
 *  原文回调
 *
 *  @param mi 信息数据源
 */
-(void) originalMsgByMsg:(ChatCellIOS*)vCell;
/**
 *  点击头像显示人物信息
 *
 *  @param mi 调用cmd需要的参数
 */
-(void) onClickHeadByMsg:(NSMsgItem*)mi;
/**
 *  点击坐标跳转世界地图
 *
 *  @param mi 调用C++函数需要的参数
 */
-(void) onClickPointByPoint:(NSString*)pointStr;

@end
@interface ChatCellIOS : UITableViewCell  
@property (nonatomic,strong) ChatCellFrame *cellFrame;
@property (nonatomic,strong) UIMenuController *uiMenu;
@property (nonatomic,strong) UIActivityIndicatorView *activityIndicator;
@property (nonatomic,strong) ChatContentView *chatView;
@property (nonatomic,strong) CSRedEnvelopeView *redEnvelopeView;
@property (nonatomic,assign) id <ChatContentViewDelegate> contentViewDelegate;
@property (nonatomic,assign)id<ChatCellIOSDelegate>chatCellIosDelegate;
-(void) initData;
-(void) setUICGRectMake;
-(void) setTestBorderWidth;
-(void) exitResetSend;

//item bool
-(void) initItemBool;


@end



