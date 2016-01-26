//
//  KeyBordVIew.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>

@class KeyBordVIew;

@protocol KeyBordVIewDelegate <NSObject>
-(void)KeyBordView:(KeyBordVIew *)keyBoardView textFiledBegin:(UITextField *)textFiled;
@end

@interface KeyBordVIew : UIView
@property (nonatomic,strong) UITextField *chatViewTextField;
@property (nonatomic,assign) id<KeyBordVIewDelegate>delegate;
/**隐藏公告视图*/
-(void) hiddenRadioView;
/**显示公告视图*/
-(void) showRadioView;
/**刷新自己的状态*/
-(void) updateStatus;

-(void) openRadio;

-(void) closeRadio;

@end
