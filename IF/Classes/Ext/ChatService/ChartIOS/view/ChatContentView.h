//
//  ChatContentView.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>
 
#import "NSMsgItem.h"
//#import "ChatLabel.h"
#import "KZLinkLabel.h"
@class ChatContentView,ChatMessage;

@protocol ChatContentViewDelegate <NSObject>
-(void)chatContentViewTapPress:(ChatContentView *)chatView content:(NSString *)content;
@end

@interface ChatContentView : UIView
@property (nonatomic,strong) UIImageView *backImageView;
@property (nonatomic,strong) UIImage *chatImageDown;
@property (nonatomic,strong) UIImage *chatImageUp;
@property (nonatomic,strong) KZLinkLabel *contentLabel;
@property (nonatomic,strong) UILabel *endStrLabel;
@property (nonatomic,strong) NSMsgItem *chatMessage;
//-(void)setBackGroundImageViewImage:(ChatContentView *)chatView left:(NSString *)left right:(NSString *)right state:(int)state;
@property (nonatomic,assign) id <ChatContentViewDelegate> chatContentViewdelegate;
@end

