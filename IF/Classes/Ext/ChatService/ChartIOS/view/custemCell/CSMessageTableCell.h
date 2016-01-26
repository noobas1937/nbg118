//
//  CSMessageTableCell.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSBaseMessageCell.h"
 

@interface CSMessageTableCell : CSBaseMessageCell
/** 时间条 font */
@property (nonatomic)UIFont *timeLabelFont UI_APPEARANCE_SELECTOR;//default [UIFont systemFontOfSize:11];
/** 时间条高度 */
@property (nonatomic)CGFloat timeLabelHeight UI_APPEARANCE_SELECTOR; //default 15;
/** 时间字体颜色 */
@property (nonatomic) UIColor *timeLabelTextColor UI_APPEARANCE_SELECTOR; //default [UIColor grayColor];
/** 时间背景颜色 */
@property (nonatomic) UIColor *timeLabelbackColor UI_APPEARANCE_SELECTOR; //default [UIColor grayColor];
/** 头像 size */
@property (nonatomic) CGFloat avatarSize UI_APPEARANCE_SELECTOR; //default 30;
/** 头像切圆角 */
@property (nonatomic) CGFloat avatarCornerRadius UI_APPEARANCE_SELECTOR; //default 0;
/** 消息名字 font */
@property (nonatomic) UIFont *messageNameFont UI_APPEARANCE_SELECTOR; //default [UIFont systemFontOfSize:12];
/** 消息名字高度  */
@property (nonatomic) CGFloat messageNameHeight UI_APPEARANCE_SELECTOR; //default 15;
/** 消息名字颜色 */
@property (nonatomic) UIColor *messageNameColor UI_APPEARANCE_SELECTOR; //default [UIColor grayColor];
/** 消息翻译font */
@property (nonatomic)UIFont * translatExplainFont UI_APPEARANCE_SELECTOR ;//default 10


@property (nonatomic) BOOL messageNameIsShow UI_APPEARANCE_SELECTOR; //default yes;

-(void)longRecognizerPresed;
-(void)longRecognizerResumeNormal;
@end
