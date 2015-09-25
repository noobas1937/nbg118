//
//  ChatCell.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>
 
#import "ChatContentView.h"
@class ChatCellIOS;
 
#import "ChatCellFrame.h"
#define TESTBORDERWIDTH 0.5

@protocol ChatCellIOSDelegate <NSObject>
-(void)ChatCellIOSTransLateWithCell:(ChatCellIOS *)vChatCellIOS;
 
@end
@interface ChatCellIOS : UITableViewCell <UITextViewDelegate>
@property (nonatomic,strong) ChatCellFrame *cellFrame;
@property (nonatomic,strong) UIMenuController *uiMenu;
@property (nonatomic,strong) UIActivityIndicatorView *activityIndicator;
@property (nonatomic,strong) ChatContentView *chatView;
@property (nonatomic,assign) id <ChatContentViewDelegate> contentViewDelegate;
@property (nonatomic,assign)id<ChatCellIOSDelegate>chatCellIosDelegate;
-(void) initData;
-(void) setUICGRectMake;
-(void) setTestBorderWidth;
-(void) exitResetSend;

//item bool
-(void) initItemBool;


@end



