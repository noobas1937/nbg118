//
//  KeyBordVIew.m
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import "KeyBordVIew.h"
#import "NSMsgItem.h"
#import "ChatCellFrame.h"
#import "ServiceInterface.h"
#include "ChatServiceCocos2dx.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "UserManager.h"
#import "MsgMessage.h"
#import "ChatServiceController.h"
#import "UIView+FrameMethods.h"
#import "ChatController.h"
#import "CSAlertView.h"
#import "NSString+Cocos2dHelper.h"
#import "YesNoDialog.h"

@interface KeyBordVIew()<UITextFieldDelegate>
//发送按钮
@property (nonatomic,strong) UIButton *sendBtn;
//接收输入的文笔
@property (nonatomic,strong) NSString *text;
//背景
@property (nonatomic,strong) UIImageView *backImageView;
//tool控件的两张背景图
@property (nonatomic,strong) UIImage *chatBackImage;
@property (nonatomic,strong) UIImage *radioBackImage;
//广播按钮
@property (nonatomic,strong) UIButton *radioBtn;
//广播按钮的两张背景图
@property (nonatomic,strong) UIImage *btn_chatBackImage;
@property (nonatomic,strong) UIImage *btn_radioBackImage;
//输入框的两张背景图
@property (nonatomic,strong) UIImage *textField_chatBackImage;
@property (nonatomic,strong) UIImage *textField_radioBackImage;
//广播条
@property (nonatomic,strong) UIView *radioTiaoView;
//广播条背景
@property (nonatomic,strong) UIImageView *radioTiaoViewBack;
//广播条喇叭图片
@property (nonatomic,strong) UIImageView *radioImage;
//广播条文字
@property (nonatomic,strong) UILabel *noticeLabel;



@end

@implementation KeyBordVIew

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        [self initData];
        
    }
    return self;
}


-(UIButton *)buttonWith:(NSString *)noraml hightLight:(NSString *)hightLight action:(SEL)action
{
    UIButton *btn=[[UIButton alloc]init];
    [btn setBackgroundImage:[UIImage imageNamed:noraml] forState:UIControlStateNormal];
    [btn setBackgroundImage:[UIImage imageNamed:hightLight] forState:UIControlStateHighlighted];
    //本地化 发送
    NSString *sendStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_SEND];
    [btn setTitle:sendStr forState:UIControlStateNormal];
    [btn setTitle:sendStr forState:UIControlStateHighlighted];
    [btn setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor grayColor]forState:UIControlStateDisabled];
    btn.titleLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].fontSize * 0.75];
    [btn addTarget:self action:action forControlEvents:UIControlEventTouchUpInside];
    btn.enabled=false;
    return btn;
}
-(void)initData
{
    self.backgroundColor = [UIColor whiteColor];
    
//    button.translatesAutoresizingMaskIntoConstraints = NO;
//    NSArray *button_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[button]-5-|"
//                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
//                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
//                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(button)];
//    [self addConstraint:[NSLayoutConstraint constraintWithItem:button
//                                                     attribute:NSLayoutAttributeWidth
//                                                     relatedBy:NSLayoutRelationEqual
//                                                        toItem:self.topView
//                                                     attribute:NSLayoutAttributeHeight
//                                                    multiplier:0.9
//                                                      constant:0]];//设置子视图的高度是父视图高度的一半
//    [self.topView addConstraints:button_c_H];
    
    //设置背景
    self.backImageView=[[UIImageView alloc]init];
    self.chatBackImage = [self create9Image:@"chuzheng_frame02" :UIEdgeInsetsMake(0, 5, 0,5)];
    self.radioBackImage = [self create9Image:@"bottom_bg" :UIEdgeInsetsMake(0, 5, 0,5)];
    self.backImageView.image=self.chatBackImage;
    self.backImageView.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.backImageView];
    
    //背景上方条
    
    
    
    //设置切换广播按钮
    self.radioBtn = [[UIButton alloc]init];
    self.btn_chatBackImage = [UIImage imageNamed:@"horn_unchecked"];
    self.btn_radioBackImage = [UIImage imageNamed:@"horn_checked"];
    [self.radioBtn setBackgroundImage:self.btn_chatBackImage forState:UIControlStateNormal];
    [self.radioBtn addTarget:self action:@selector(switchRadioView:) forControlEvents:UIControlEventTouchUpInside];
    self.radioBtn.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.radioBtn];
    self.radioBtn.hidden = YES;
    
    //设置输入框
    self.chatViewTextField = [[UITextField alloc]init];
    self.chatViewTextField.translatesAutoresizingMaskIntoConstraints = NO;
    
    self.chatViewTextField.returnKeyType=UIReturnKeySend;
    self.chatViewTextField.font=[UIFont fontWithName:@"HelveticaNeue" size:[ServiceInterface serviceInterfaceSharedManager].fontSize ];
    self.chatViewTextField.returnKeyType =UIReturnKeyDefault;
    self.chatViewTextField.enablesReturnKeyAutomatically = YES;
    self.textField_chatBackImage = [self create9Image:@"text_field_bg2.png" :UIEdgeInsetsMake(0, 5, 0,5)];
    self.textField_radioBackImage = [self create9Image:@"input" :UIEdgeInsetsMake(0, 25, 0,25)];
    self.chatViewTextField.background = self.textField_chatBackImage;
    self.chatViewTextField.delegate=self;
    [self addSubview:self.chatViewTextField];
    [self.chatViewTextField addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    //输入框左侧加入内边距
    CGRect frame = self.chatViewTextField.frame;
    frame.size.width = 10.0f;
    UIView *leftview = [[UIView alloc] initWithFrame:frame];
    self.chatViewTextField.leftViewMode = UITextFieldViewModeAlways;
    self.chatViewTextField.leftView = leftview;
    
    //设置发送按钮
    self.sendBtn =[self buttonWith:@"chat_bottom_smile_nor.png" hightLight:@"chat_bottom_smile_press.png" action:@selector(sendBtnPress:)];
    self.sendBtn.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.sendBtn];
    
    //加入广播条
    self.radioTiaoView = [[UIView alloc]init];
    self.radioTiaoView.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.radioTiaoView];
    //设置广播条背景
    self.radioTiaoViewBack = [[UIImageView alloc]init];
    self.radioTiaoViewBack.image = [self create9Image:@"horn_text_bg" :UIEdgeInsetsMake(0, 5, 0,5)];
    self.radioTiaoViewBack.translatesAutoresizingMaskIntoConstraints = NO;
    [self.radioTiaoView addSubview:self.radioTiaoViewBack];
    //设置广播条图片
    self.radioImage = [[UIImageView alloc]init];
    self.radioImage.image = [UIImage imageNamed:@"horn_icon"];
    self.radioImage.translatesAutoresizingMaskIntoConstraints = NO;
    [self.radioTiaoView addSubview:self.radioImage];
    //设置广播条文字
    self.noticeLabel = [[UILabel alloc]init];
    self.noticeLabel.translatesAutoresizingMaskIntoConstraints = NO;
    NSString *noticeStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_HORN_TEXT];
    self.noticeLabel.text = noticeStr;
    self.noticeLabel.textColor = [UIColor yellowColor];
    [self.radioTiaoView addSubview:self.noticeLabel];
    
    
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        [self showRadioView];
    }else{
        [self hiddenRadioView];
    }
    
}

//打开广播视图
-(void)switchRadioView:(UIButton *)image
{
    
    if (self.radioTiaoView.hidden == YES) {
        [self openRadio];
    }else{
        [self closeRadio];
    }
    
}

-(void)openRadio
{
    self.radioTiaoView.hidden = NO;
    //切换背景
    self.backImageView.image = self.radioBackImage;
    //切换广播按钮
    [self.radioBtn setBackgroundImage:self.btn_radioBackImage forState:UIControlStateNormal];
    //切换输入框
    [self.chatViewTextField setBackground:self.textField_radioBackImage];
    
}

-(void)closeRadio
{
    self.radioTiaoView.hidden = YES;
    //切换背景
    self.backImageView.image = self.chatBackImage;
    //切换广播按钮
    [self.radioBtn setBackgroundImage:self.btn_chatBackImage forState:UIControlStateNormal];
    //切换输入框
    [self.chatViewTextField setBackground:self.textField_chatBackImage];
}


//显示公告视图
-(void) showRadioView
{
    
    if (!ChatController::getInstance()->getIsNoticeOpen())
        return;
    //显示公告按钮 背景图片重置默认
    self.radioBtn.hidden = NO;
    //重置布局显示公告
    [self updateConstraints];
    //RadioView 视图重置
    [self viewReset];
    
}
//隐藏公告视图
-(void) hiddenRadioView
{
    //隐藏公告按钮和公告条视图
    self.radioBtn.hidden = YES;
    self.radioTiaoView.hidden = YES;
    //重置布局隐藏公告
    [self updateConstraints];
    //RadioView 视图重置
    [self viewReset];
    
    
}

-(void) viewReset
{
    self.radioTiaoView.hidden = YES;
    self.backImageView.image=self.chatBackImage;
    [self.radioBtn setBackgroundImage:self.btn_chatBackImage forState:UIControlStateNormal];
    [self.chatViewTextField setBackground:self.textField_chatBackImage];
}


-(void)sendBtnPress:(UIButton *)image
{
    //组装用户信息
    
    NSMsgItem *chatMessage = [[ServiceInterface serviceInterfaceSharedManager] createChatMessage:self.chatViewTextField.text];
    if (chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (![self isSendChat]) {
            return ;
        }
    }
    
    if(self.radioTiaoView.hidden == NO){
        chatMessage.post = 6;
    }
    
    //先上屏
    [[ChatServiceController chatServiceControllerSharedManager].gameHost directlySendMsg:chatMessage];
    
    //输入框置空
    self.chatViewTextField.text = @"";
    self.sendBtn.enabled = NO;

}

-(BOOL) isChatRestrict
{
    
    BOOL ret = false;
    
    NSUserInfo *user = [UserManager sharedUserManager].currentUser;
    NSString *uid = [UserManager sharedUserManager].currentUser.uid;
    if(uid.length >= 3)
    {
        NSString *uidPostfix = [uid substringFromIndex:uid.length - 3];
        if([uidPostfix isPureInt])
        {
            int serverId = [uidPostfix intValue];
            uidPostfix = [NSString stringWithFormat:@"%d",serverId];
            if(user!=nil && user.userName.length > 0)
            {
                if ([user.userName hasPrefix:@"Empire"] && [user.userName hasSuffix:uidPostfix])
                    return true;
                else
                    return false;
            }
        }
        
    }
    
    return ret;
}

-(void) banSendMsgTip
{
    NSString * tipStr = [NSString stringWithMultilingualWithKey:@"132130"];
    CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
    alertView.viewType = CHANGENAME;
    [alertView showViewByType];
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    
    UIViewController *recentView = nil;
    
    recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    
    [recentView.view addSubview:alertView];
    
}

-(BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    if([self.delegate respondsToSelector:@selector(KeyBordView:textFiledBegin:)]){
        [self.delegate KeyBordView:self textFiledBegin:self.chatViewTextField];
    }
    return YES;
}

//回车功能
-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    return YES;
}


-(void)textFieldDidChange:(id)sender
{
    if (self.chatViewTextField.text.length !=0 && ![self.chatViewTextField.text isEqualToString:@""]) {
    
        int ascll = [self.chatViewTextField.text characterAtIndex:0];
        
        if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE && [[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]){
            self.sendBtn.enabled = NO;
        }else{
            //汉字
            if( ascll > 0x4e00 && ascll < 0x9fff)
            {
                self.sendBtn.enabled = YES;
            }
            //应为
            else
            {
                self.sendBtn.enabled = YES;
            }
            
            self.text = self.chatViewTextField.text;
        }
    }else{
        //没有输入
        self.sendBtn.enabled = NO;
    }
}

-(UIImage *) create9Image:(NSString *) imageName :(UIEdgeInsets) edge
{
    UIImage* img=[UIImage imageNamed:imageName];//原图
    UIImage *image9 = [img resizableImageWithCapInsets:edge resizingMode:UIImageResizingModeStretch];
    return image9;
}

-(void)updateStatus
{
    if (self.chatViewTextField.text.length != 0 && ![self.chatViewTextField.text isEqualToString:@""]) {
        if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE && [[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]){
            self.sendBtn.enabled = NO;
        }else{
            self.sendBtn.enabled = YES;
        }
    }
}
    
-(void) updateConstraints
{
    CGSize winSize=[UIScreen mainScreen].bounds.size;
    //显示广播组件
    if(self.radioBtn.hidden == NO){
        [self resetConstraints];
        //背景图添加约束
        NSArray *backImage_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_backImageView]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backImageView)];
        NSArray *backImage_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_backImageView]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backImageView)];
        [self addConstraints:backImage_c_H];
        [self addConstraints:backImage_c_V];
        
        //广播按钮添加约束
        NSArray *radioBtn_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_radioBtn]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_radioBtn)];
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioBtn
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.93
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioBtn
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.95
                                                          constant:0]];
        
        [self addConstraints:radioBtn_c_V];
        
        //输入框添加约束
        
        CGFloat width = self.chatViewTextField.bounds.size.width - self.radioBtn.bounds.size.width;
        
        NSArray *chatViewTextField_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_chatViewTextField]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_chatViewTextField)];
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.chatViewTextField
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeWidth
                                                        multiplier:0.61
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.chatViewTextField
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.9
                                                          constant:0]];
        
        [self addConstraints:chatViewTextField_c_V];
        
        //发送按钮添加约束
        
        NSDictionary *views = NSDictionaryOfVariableBindings(_radioBtn,_chatViewTextField,_sendBtn);
        NSArray *sendBtn_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_radioBtn]-0-[_chatViewTextField]-0-[_sendBtn]-0-|" options:0 metrics:nil views:views];
        NSArray *sendBtn_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_sendBtn]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_sendBtn)];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.sendBtn
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeWidth
                                                        multiplier:0.23
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.sendBtn
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.9
                                                          constant:0]];
        
        [self addConstraints:sendBtn_c_H];
        [self addConstraints:sendBtn_c_V];
        
        
        //广播条view约束
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioTiaoView
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeWidth
                                                        multiplier:1.0
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioTiaoView
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.6
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioTiaoView
                                                         attribute:NSLayoutAttributeBottom
                                                         relatedBy:NSLayoutRelationGreaterThanOrEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeTop
                                                        multiplier:1.0
                                                          constant:1]];
        
        //广播条背景图约束
        NSArray *radioTiaoViewBack_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_radioTiaoViewBack]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_radioTiaoViewBack)];
        NSArray *radioTiaoViewBack_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_radioTiaoViewBack]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_radioTiaoViewBack)];
        [self.radioTiaoView addConstraints:radioTiaoViewBack_c_H];
        [self.radioTiaoView addConstraints:radioTiaoViewBack_c_V];
        
        //广播喇叭图片约束
        NSArray *radioImage_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_radioImage]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_radioImage)];
        [self.radioTiaoView addConstraint:[NSLayoutConstraint constraintWithItem:self.radioImage
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self.radioTiaoView
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:1.0
                                                          constant:0]];
        
        [self.radioTiaoView addConstraints:radioImage_c_V];
        
        //广播条文字约束
        
        NSDictionary *noticeLabel_views = NSDictionaryOfVariableBindings(_radioImage,_noticeLabel);
        
        NSArray *noticeLabel_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-[_radioImage]-0-[_noticeLabel]-|" options:0 metrics:nil views:noticeLabel_views];
        
        [self.radioTiaoView addConstraint:[NSLayoutConstraint constraintWithItem:self.noticeLabel attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:self.radioTiaoView attribute:NSLayoutAttributeCenterY multiplier:1 constant:0]];
        
        [self.radioTiaoView addConstraints:noticeLabel_c_H];
    }else{
        //背景图添加约束
        NSArray *backImage_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_backImageView]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backImageView)];
        NSArray *backImage_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_backImageView]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_backImageView)];
        [self addConstraints:backImage_c_H];
        [self addConstraints:backImage_c_V];
        
        //广播按钮添加约束
        NSArray *radioBtn_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_radioBtn]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_radioBtn)];
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioBtn
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.95
                                                          constant:0]];//设置子视图的高度是父视图高度的一半
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.radioBtn
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.95
                                                          constant:0]];//设置子视图的高度是父视图高度的一半
        
        [self addConstraints:radioBtn_c_V];
        
        //输入框添加约束
        NSArray *chatViewTextField_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_chatViewTextField]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_chatViewTextField)];
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.chatViewTextField
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeWidth
                                                        multiplier:0.75
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.chatViewTextField
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.9
                                                          constant:0]];
        
        [self addConstraints:chatViewTextField_c_V];
        
        //发送按钮添加约束
        NSDictionary *views = NSDictionaryOfVariableBindings(_chatViewTextField,_sendBtn);
        NSArray *sendBtn_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_chatViewTextField]-0-[_sendBtn]-0-|" options:0 metrics:nil views:views];
        NSArray *sendBtn_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_sendBtn]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(_sendBtn)];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.sendBtn
                                                         attribute:NSLayoutAttributeWidth
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeWidth
                                                        multiplier:0.23
                                                          constant:0]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.sendBtn
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.9
                                                          constant:0]];
        
        [self addConstraints:sendBtn_c_H];
        [self addConstraints:sendBtn_c_V];
        
    }
    
    [super updateConstraints];
}

//重置约束
-(void)resetConstraints{
    [self removeConstraints:self.constraints];
}

-(BOOL) isSendChat{
    DVLog(@"isSendChat");
    //判断是否改名
    if([self isChatRestrict]){
        [self.chatViewTextField resignFirstResponder];
        [self banSendMsgTip];
        return FALSE;
    }
    
    return TRUE;
}

@end
