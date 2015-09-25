//
//  TopUIView.m
//  气泡
//
//  Created by mzc on 15/3/31.
//  Copyright (c) 2015年 zzy. All rights reserved.
//

#import "TopUIView.h"
#import "ServiceInterface.h"
#import "ChatViewController.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "ChatServiceCocos2dx.h"
#import "ChatServiceController.h"
#import "UserManager.h"

#import"AppController.h"

@interface TopUIView()

@property (strong,nonatomic) UIButton  * addMemberButton;

@end

@implementation TopUIView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(id) init:(int)chatType
{
    self = [super init];
    if (self) {
        self.chatType = chatType;
        [self addTop];
        [self addDown];
        [self viewConstraint];
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
    }
    return self;
}

-(void) viewConstraint
{
    //整体布局 上中下
    
    if (self.chatType == IOS_CHANNEL_TYPE_USER || self.chatType == IOS_CHANNEL_TYPE_BBS) {
        NSDictionary *views = NSDictionaryOfVariableBindings(_topView);
        NSArray *tbt=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_topView]-0-|"
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.huitiaoImage
                                                         attribute:NSLayoutAttributeTop
                                                         relatedBy:NSLayoutRelationLessThanOrEqual
                                                            toItem:self.topView
                                                         attribute:NSLayoutAttributeBottom
                                                        multiplier:1.0
                                                          constant:-4]];//设置子视图的高度是父视图高度的一半
        [self addConstraints:tbt];
    }else{
        NSDictionary *views = NSDictionaryOfVariableBindings(_topView,_buttonView);
        NSArray *tbt=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_topView]-0-[_buttonView(_topView)]"
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.buttonView
                                                         attribute:NSLayoutAttributeTop
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self.topView
                                                         attribute:NSLayoutAttributeBottom
                                                        multiplier:1.0
                                                          constant:-4]];
        
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.huitiaoImage
                                                         attribute:NSLayoutAttributeTop
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeBottom
                                                        multiplier:1.0
                                                          constant:-4]];
        [self addConstraints:tbt];
    }
    
    
}


- (void)addTop
{
    CGSize winSize=[UIScreen mainScreen].bounds.size;
    
    //上布局
    self.topView = [[UIView alloc]init];
    self.topView.backgroundColor = [UIColor clearColor];
    self.topView.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.topView];
    
    NSArray *topView_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_topView]-0-|"
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_topView)];
    
    if (self.chatType == IOS_CHANNEL_TYPE_USER || self.chatType == IOS_CHANNEL_TYPE_BBS) {
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.topView
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:1.0
                                                          constant:0]];//设置子视图的高度是父视图高度的一半
    }else{
        [self addConstraint:[NSLayoutConstraint constraintWithItem:self.topView
                                                         attribute:NSLayoutAttributeHeight
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:self
                                                         attribute:NSLayoutAttributeHeight
                                                        multiplier:0.5
                                                          constant:0]];//设置子视图的高度是父视图高度的一半
    }
    
    
    [self addConstraints:topView_c_H];
    
    //导航栏背景
    self.topBackGround = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"ui_gray_biaotilan.png"]];
    self.topBackGround.translatesAutoresizingMaskIntoConstraints=NO;
    [self.topView addSubview:self.topBackGround];
    
    NSArray *topBackGround_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_topBackGround]-0-|"
                          　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                          　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                          　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_topBackGround)];
    
    NSArray *topBackGround_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_topBackGround]-0-|"
                                　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_topBackGround)];
    
    [self.topView addConstraints:topBackGround_c_H];
    [self.topView addConstraints:topBackGround_c_V];
//
//    
    //添加导航栏标题
    self.titlePlayerName = [[UILabel alloc]init];
    self.titlePlayerName.translatesAutoresizingMaskIntoConstraints=NO;

    if(self.chatType == IOS_CHANNEL_TYPE_USER || self.chatType == IOS_CHANNEL_TYPE_BBS){
        //显示邮件对话。对方人物的名字
        self.titlePlayerName.text = [UserManager sharedUserManager].currentMail.opponentName;
    }else{
        self.titlePlayerName.text = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_CHAT];
    }
    
    self.titlePlayerName.textAlignment = NSTextAlignmentCenter;
    self.titlePlayerName.backgroundColor = [UIColor clearColor];
    self.titlePlayerName.font=[UIFont fontWithName:@"Helvetica-Bold" size:[ServiceInterface serviceInterfaceSharedManager].fontSize];
    self.titlePlayerName.textColor = [UIColor whiteColor];
    [self.topView addSubview:self.titlePlayerName];
    //标题添加约束 垂直居中
    [self.topView addConstraint:[NSLayoutConstraint constraintWithItem:_titlePlayerName attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:self.topView attribute:NSLayoutAttributeCenterY multiplier:1 constant:0]];
    //标题添加约束 垂直居中
    [self.topView addConstraint:[NSLayoutConstraint constraintWithItem:_titlePlayerName attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:self.topView attribute:NSLayoutAttributeCenterX multiplier:1 constant:0]];
////
    //返回按钮
    UIButton *button = [[UIButton alloc]init];
    button.translatesAutoresizingMaskIntoConstraints = NO;
    [self.topView addSubview:button];
    [button setBackgroundImage:[UIImage imageNamed:@"but_back"] forState:UIControlStateNormal];
    [button addTarget:self action:@selector(backButtonPressed:) forControlEvents: UIControlEventTouchUpInside];
    /**返回按钮约束 上 下 左 ＝ 0 宽度 ＝ 父高度*/
    
    NSArray *button_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[button]"
                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(button)];
    
    NSArray *button_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[button]-0-|"
                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                             　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(button)];
    [self addConstraint:[NSLayoutConstraint constraintWithItem:button
                                                     attribute:NSLayoutAttributeWidth
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:self.topView
                                                     attribute:NSLayoutAttributeHeight
                                                    multiplier:1.0
                                                      constant:0]];//设置子视图的高度是父视图高度的一半
    [self.topView addConstraints:button_c_H];
    [self.topView addConstraints:button_c_V];
    
    //为邮件或者聊天室
   
        
        if (self.chatType == IOS_CHANNEL_TYPE_USER ||self.chatType == IOS_CHANNEL_TYPE_CHATROOM || self.chatType == IOS_CHANNEL_TYPE_BBS) {
            if (ChatServiceCocos2dx::enableChatRoom) {
                UIButton *addMemberButton =[UIButton buttonWithType:UIButtonTypeCustom];
                [addMemberButton setImage:[UIImage imageNamed:@"groupAddMerber"] forState:UIControlStateNormal];
                [addMemberButton setTitle:@"" forState:UIControlStateNormal];
                addMemberButton.translatesAutoresizingMaskIntoConstraints = NO;
                [addMemberButton addTarget:self action:@selector(addGroupMemberButton:) forControlEvents:UIControlEventTouchUpInside];
                [self.topView addSubview:addMemberButton];
                NSArray *addMemberButton_H =[NSLayoutConstraint constraintsWithVisualFormat:@"H:[addMemberButton]-0-|" options:0 metrics:nil views:NSDictionaryOfVariableBindings(addMemberButton)];
                NSArray *addMemberButton_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[addMemberButton]-0-|"
                                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                            　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(addMemberButton)];
                
                [self addConstraint:[NSLayoutConstraint constraintWithItem:addMemberButton
                                                                 attribute:NSLayoutAttributeWidth
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.topView
                                                                 attribute:NSLayoutAttributeHeight
                                                                multiplier:1.0
                                                                  constant:0]];
                [self.topView addConstraints:addMemberButton_H];
                [self.topView addConstraints:addMemberButton_V];
                self.addMemberButton = addMemberButton;
                if([UserManager sharedUserManager].currentUser.allianceId.length >0){
                    self.addMemberButton.hidden = YES;
                }

            }
            
    }
    
    
//
    //装饰条
    self.huitiaoImage = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"ui_huitiao2.png"]];
    self.huitiaoImage.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.huitiaoImage];
    
    NSArray *huitiaoImage_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_huitiaoImage]-0-|"
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                         　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_huitiaoImage)];
    
    [self addConstraint:[NSLayoutConstraint constraintWithItem:self.huitiaoImage
                                                     attribute:NSLayoutAttributeHeight
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:self
                                                     attribute:NSLayoutAttributeHeight
                                                    multiplier:0.2
                                                      constant:0]];//设置子视图的高度是父视图高度的一半
    [self addConstraints:huitiaoImage_c_H];
}

- (void)addDown
{
    
    if (self.chatType == IOS_CHANNEL_TYPE_ALLIANCE || self.chatType == IOS_CHANNEL_TYPE_COUNTRY) {
        //addDownView
        self.buttonView=[[UIView alloc]init];
        self.buttonView.backgroundColor=[UIColor blackColor];
        self.buttonView.translatesAutoresizingMaskIntoConstraints = NO;
        [self addSubview:self.buttonView];
        
        //addButton     联盟
        self.allianceButton = [UIButton buttonWithType:UIButtonTypeCustom];
        self.allianceButton.translatesAutoresizingMaskIntoConstraints = NO;
        self.allianceButton.tag = 10002;
        [self.buttonView addSubview:self.allianceButton];
        [self setUIButton:self.allianceButton type:IOS_CHANNEL_TYPE_ALLIANCE];
        //addButton     国家
        self.countriesButton = [UIButton buttonWithType:UIButtonTypeCustom];
        self.countriesButton.translatesAutoresizingMaskIntoConstraints = NO;
        self.countriesButton.tag = 10001;
        [self setUIButton:self.countriesButton type:IOS_CHANNEL_TYPE_COUNTRY];
        [self.buttonView addSubview:self.countriesButton];
        NSArray *buttonView_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_buttonView]-0-|"
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_buttonView)];
        
        
        [self addConstraints:buttonView_c_H];
        
        
        NSDictionary *views = NSDictionaryOfVariableBindings(_allianceButton,_countriesButton);
        
        NSArray *asn_Button_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:[_allianceButton]-0-|"
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                 　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
        
        NSArray *coun_Button_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_countriesButton(_allianceButton)]"
                                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                  　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
        
        NSArray *allianceButtonButton_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_allianceButton]-0-|"
                                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_allianceButton)];
        NSArray *countriesButton_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_countriesButton]-0-|"
                                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                                      　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_countriesButton)];
        
        [self.buttonView addConstraint:[NSLayoutConstraint constraintWithItem:self.allianceButton
                                                                    attribute:NSLayoutAttributeWidth
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:self.buttonView
                                                                    attribute:NSLayoutAttributeWidth
                                                                   multiplier:0.49
                                                                     constant:0]];
        
        
        [self.buttonView addConstraints:asn_Button_c_H];
        [self.buttonView addConstraints:coun_Button_c_H];
        [self.buttonView addConstraints:allianceButtonButton_c_V];
        [self.buttonView addConstraints:countriesButton_c_V];
    }
    
    
}

-(void) setUIButton:(UIButton *)button type:(int)buttonType
{
    
    // 设置aligment 属性
    button.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    button.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
    button.titleLabel.textAlignment = NSTextAlignmentCenter;
    
    //设置title自适应对齐
    button.titleLabel.lineBreakMode = NSLineBreakByWordWrapping;
    
    // 添加 action
    if(buttonType == IOS_CHANNEL_TYPE_COUNTRY){
        
        [button addTarget:self action:@selector(countriesButtonPressed:) forControlEvents: UIControlEventTouchUpInside];
    }else{
        [button addTarget:self action:@selector(allianceButtonPressed:) forControlEvents: UIControlEventTouchUpInside];
    }
    
    //本地化 国家联盟
    NSString *countriesStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_COUNTRY];
    NSString *allianceStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_ALLIANCE];
    //字体
    button.titleLabel.font = [UIFont boldSystemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].fontSize];
    //设置title
    
    if (ChatServiceCocos2dx::m_channelType== IOS_CHANNEL_TYPE_COUNTRY) {
        if (buttonType == IOS_CHANNEL_TYPE_COUNTRY) {
            [button setTitle:countriesStr forState:UIControlStateNormal];
            [button setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
        }else{
            [button setTitle:allianceStr forState:UIControlStateNormal];
            [button setTitleColor:[UIColor colorWithRed:255 green:255 blue:0 alpha:0.3] forState:UIControlStateNormal];
        }
    }else{
        if (buttonType == IOS_CHANNEL_TYPE_COUNTRY) {
            [button setTitle:countriesStr forState:UIControlStateNormal];
            [button setTitleColor:[UIColor colorWithRed:255 green:255 blue:0 alpha:0.3] forState:UIControlStateNormal];
        }else{
            [button setTitle:allianceStr forState:UIControlStateNormal];
            [button setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
        }
    }
    
    
    //设置背景
    
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY){
        if (buttonType == IOS_CHANNEL_TYPE_COUNTRY) {
            [button setBackgroundImage:[UIImage imageNamed:@"btn_qieye_01"] forState:UIControlStateNormal];
        }else{
            [button setBackgroundImage:[UIImage imageNamed:@"btn_qieye_02"] forState:UIControlStateNormal];
        }
    }else{
        if (buttonType == IOS_CHANNEL_TYPE_COUNTRY) {
            [button setBackgroundImage:[UIImage imageNamed:@"btn_qieye_02"] forState:UIControlStateNormal];
        }else{
            [button setBackgroundImage:[UIImage imageNamed:@"btn_qieye_01"] forState:UIControlStateNormal];
        }
    }
    
}

-(void) allianceButtonPressed:(UIButton *)sender
{
    ChatServiceCocos2dx::m_channelType = IOS_CHANNEL_TYPE_ALLIANCE;
   
    if ([self.topUIViewDelegate respondsToSelector:@selector(clickButtonWithTag:)]){
        [self.topUIViewDelegate clickButtonWithTag:sender.tag];
    }
    [self selectASN];
}

-(void) countriesButtonPressed:(UIButton *)sender
{
    ChatServiceCocos2dx::m_channelType = IOS_CHANNEL_TYPE_COUNTRY;
    if ([self.topUIViewDelegate respondsToSelector:@selector(clickButtonWithTag:)]){
        [self.topUIViewDelegate clickButtonWithTag:sender.tag];
    }
    [self selectCoun];
}

-(void) backButtonPressed:(id)sender
{
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    if (self.chatType == IOS_CHANNEL_TYPE_BBS) {
        NSString *url = [ServiceInterface serviceInterfaceSharedManager].bbsIOSViewController.bbsView.request.URL.absoluteString;
        [ServiceInterface serviceInterfaceSharedManager].bbsIOSViewController.targetUrl = url;
    }
    
    //邮件界面隐藏时。更新c++邮件状态
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_USER || ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_CHATROOM) {
        [[ChatServiceController chatServiceControllerSharedManager] updateMailList];
    }
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:ChatServiceCocos2dx::m_channelType];
    
    if ([[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""]){
        [self selectCoun];
    }
    
    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    
    
}

-(void)addGroupMemberButton:(UIButton *)sender{
    DVLog(@"管理按钮点击");
    if ([self.topUIViewDelegate respondsToSelector:@selector(TopUIViewRightButtonAction)]) {
        [self.topUIViewDelegate TopUIViewRightButtonAction];
    }
}

-(void) selectASN
{
    ChatServiceCocos2dx::m_channelType = IOS_CHANNEL_TYPE_ALLIANCE;
    [self.allianceButton setBackgroundImage:[UIImage imageNamed:@"btn_qieye_01"] forState:UIControlStateNormal];
    [self.countriesButton setBackgroundImage:[UIImage imageNamed:@"btn_qieye_02"] forState:UIControlStateNormal];
    [self.allianceButton setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
    [self.countriesButton setTitleColor:[UIColor colorWithRed:255 green:255 blue:0 alpha:0.3] forState:UIControlStateNormal];
    
    if ([ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView.hidden == YES) {
        
        [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView.hidden = YES;
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView hiddenRadioView];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_ALLIANCE];
        
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController isShowJionAllance];
        
        
    }
    [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView.hidden = NO;
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView updateStatus];
}

-(void) selectCoun
{
    ChatServiceCocos2dx::m_channelType = IOS_CHANNEL_TYPE_COUNTRY;
    [self.countriesButton setBackgroundImage:[UIImage imageNamed:@"btn_qieye_01"] forState:UIControlStateNormal];
    [self.allianceButton setBackgroundImage:[UIImage imageNamed:@"btn_qieye_02"] forState:UIControlStateNormal];
    [self.countriesButton setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
    [self.allianceButton setTitleColor:[UIColor colorWithRed:255 green:255 blue:0 alpha:0.3] forState:UIControlStateNormal];
    
    if ([ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView.hidden == YES) {
        
//        if([ServiceInterface serviceInterfaceSharedManager].isLoadCountriesData){
//            //从本地加载还是从服务器请求
//            
//            [[ChatServiceController chatServiceControllerSharedManager].gameHost requestCountriesData];
//        }
        
        [ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView.hidden = YES;
        
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView showRadioView];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_COUNTRY];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController hiddenJionAllanceView];
    }
    [ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView.hidden = NO;
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView updateStatus];
}



-(void)settingAddMemberButtonShow:(BOOL)vShowBool{
    self.addMemberButton.hidden = !vShowBool;
}

@end

