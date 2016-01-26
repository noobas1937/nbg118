//
//  ChatCellFrame.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
@interface ChatCellFrame : NSObject
@property (nonatomic,assign) CGRect timeLabelRect;
@property (nonatomic,assign) CGRect uiTopViewRect;
@property (nonatomic,assign) CGRect uiDownViewRect;
@property (nonatomic,assign) CGRect iconRect;
@property (nonatomic,assign) CGRect activityIndicatorRect;
@property (nonatomic,assign) CGRect resetSendBtnRect;
@property (nonatomic,assign) CGRect headPicRect;
@property (nonatomic,assign) CGRect borderRect;
@property (nonatomic,assign) CGRect gmodRect;
@property (nonatomic,assign) CGRect chatViewRect;
@property (assign,nonatomic) CGSize  chatSystemSize;
@property (nonatomic,assign) CGSize chatLabelSize;
@property (nonatomic,assign) CGSize endStrLabelSize;
@property (nonatomic,assign) CGSize chatLabelSizeTest;
@property (nonatomic,assign) CGRect vip_asn_name_Rect;
@property (nonatomic,strong) NSMsgItem *chatMessage;
@property (nonatomic,assign) CGFloat cellHeight; //cell高度
@property (nonatomic,assign) CGRect redEnvelopeRect;
-(void) isShowUiTopView:(NSMutableArray*) cellFrameArray;
@property (nonatomic,assign) BOOL isShowUiTopView_BOOL;
-(void)initWithData:(NSMsgItem*) chatMessage ;
-(instancetype)init:(NSMsgItem *)chatMessage;


-(void) settingtrTranslation;
-(void)chatCellFrameCheckTranslate;

-(void)chatCellFrameUpdateUserInfo:(NSMsgItem *)vMsgItem;
@end
