//
//  Cellconfig.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/13.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <UIKit/UIKit.h>

#define cellConfig [Cellconfig sharedCellConfig]

@interface Cellconfig : NSObject

/** resendButton size */
@property (nonatomic) CGFloat resendButtonSize UI_APPEARANCE_SELECTOR; //default 20;
/** 菊花 size */
@property (nonatomic) CGFloat activitySize UI_APPEARANCE_SELECTOR; //default 20;
/** 气泡内容最大宽度 */
@property (nonatomic) CGFloat bubbleMaxWidth UI_APPEARANCE_SELECTOR; //屏幕宽度 * 0.55;
 
/** 名字默认大小 */
@property  (nonatomic)CGFloat nicklabelFontSize UI_APPEARANCE_SELECTOR;
/** 左边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets leftBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 15, 8, 10);
/** 右边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets rightBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 10, 8, 15);
/** 头像边距 */
@property (nonatomic)CGFloat avatarMargin;
/** 时间条 font */
@property (nonatomic,strong)UIFont *timeLabelFont  ;//default [UIFont systemFontOfSize:11];
/** 时间条高度 */
@property (nonatomic)CGFloat timeLabelHeight UI_APPEARANCE_SELECTOR; //default 15;
/** 时间字体颜色 */
@property (nonatomic,strong) UIColor *timeLabelTextColor  ; //default [UIColor grayColor];
/** 时间背景颜色 */
@property (nonatomic,strong) UIColor *timeLabelbackColor  ; //default [UIColor grayColor];
/** 头像 size */
@property (nonatomic) CGFloat avatarSize UI_APPEARANCE_SELECTOR; //default 30;
/** 头像切圆角 */
@property (nonatomic) CGFloat avatarCornerRadius UI_APPEARANCE_SELECTOR; //default 0;
/** 消息名字 font */
@property (nonatomic,strong) UIFont *messageNameFont  ; //default [UIFont systemFontOfSize:12];
/** 消息名字高度  */
@property (nonatomic) CGFloat messageNameHeight  ; //default 15;
/** 消息名字颜色 */
@property (nonatomic,strong) UIColor *messageNameColor  ; //default [UIColor grayColor];
/** 消息文字大小 */
@property(nonatomic)CGFloat  contentsTextSize UI_APPEARANCE_SELECTOR;//default 16
/** 消息文字大小 */
@property(nonatomic,strong)UIFont  *contentsTextFont  ;//default [UIFont systemFontOfSize:16]
/** 消息文字颜色 */
@property (nonatomic,strong)UIColor * contentsTextColor  ;//default [UIColor blackColor];

/** 聊天室系统消息大小 */
@property (nonatomic)UIFont * chatRoomSystemTextSize UI_APPEARANCE_SELECTOR ;//default 12

/** 消息翻译font */
@property (nonatomic,strong)UIFont * translatExplainFont   ;//default 10
@property (nonatomic,strong)UIColor * translatExplainTextColor   ;//default 10

/** 红包宽度 */
@property (nonatomic)CGFloat redPacketBubbleWith;//屏幕宽度 * 0.50;
@property (nonatomic)CGFloat redPacketBubbleHeight;//default 45
/** 红包操作说明字体font */
@property (nonatomic,strong)UIFont *redSubTextFont ;//default 12
/** 红包时间字体font  */
@property (nonatomic,strong)UIFont *redTimeTextFont;//default 11
/** 左边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets redPacketLeftBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 15, 8, 10);
/** 右边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets redPacketRightBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 10, 8, 15);
+ (Cellconfig *)sharedCellConfig;
@end
