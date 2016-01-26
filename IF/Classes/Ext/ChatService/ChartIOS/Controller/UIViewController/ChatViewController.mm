//
//  ViewController.m
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import "ChatViewController.h"
#import "ServiceInterface.h"
#import "TopUIView.h"
#import "UIView+FrameMethods.h"
#import "ChatServiceCocos2dx.h"
#import "ChatServiceController.h"
#import "UserManager.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "UITableViewController+Extension.h"
#import "CCSafeNotificationCenter.h"
#import "MailController.h"

#import "ChannelManager.h"
@interface ChatViewController ()<KeyBordVIewDelegate,TopUIViewDelegate>

@property (nonatomic,assign) CGRect allianceTabelViewFrame;
@property (nonatomic,assign) CGRect countriesTabelViewFrame;

-(void) addTopUIView;
-(void) addUITableView;
-(void) addUIKeyBordVIew;

@end

static float topUIViewH = 0;
@implementation ChatViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationController.navigationBarHidden=YES;
    self.view.backgroundColor = [UIColor blackColor];
    
//    [self addTopUIView];
//    [self addUIKeyBordVIew];
//    [self addUITableView];
//    [self addJoinAllance];
//    [self.view sendSubviewToBack:self.countriesTableViewController.view];
//    [self.view sendSubviewToBack:self.allianceTableViewController.view];
//    [self.view bringSubviewToFront:self.topUIView];
//    
    UITapGestureRecognizer *aTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(keyboardHide:)];
    //设置成NO表示当前控件响应后会传播到其他控件上，默认为YES。
    aTapGestureRecognizer.cancelsTouchesInView = NO;
    //将触摸事件添加到TableView
    [self.allianceTableViewController.tableView addGestureRecognizer:aTapGestureRecognizer];
    
    
    UITapGestureRecognizer *cTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(keyboardHide:)];
    //设置成NO表示当前控件响应后会传播到其他控件上，默认为YES。
    cTapGestureRecognizer.cancelsTouchesInView = NO;
    //将触摸事件添加到TableView
    [self.countriesTableViewController.tableView addGestureRecognizer:cTapGestureRecognizer];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification object:nil]; //监听是否触发home键挂起程序.
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification object:nil]; //监听是否重新进入程序程序.
    

 
    
    
}

-(void)dealloc{
   
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
}


-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasShown:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
   
}
-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
   
}

-(void)keyboardWasShown:(NSNotification *)note
{

    int type = ChatServiceCocos2dx::m_channelType;
    
    if (type == IOS_CHANNEL_TYPE_COUNTRY || type == IOS_CHANNEL_TYPE_ALLIANCE) {
        CGSize winSize=[UIScreen mainScreen].bounds.size;
        CGRect keyBoardRect=[note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
        CGFloat deltaY=keyBoardRect.size.height;
        CGFloat buttonViewH = self.topUIView.buttonView.frame.size.height;
        
        
        self.keyBordView.frame = CGRectMake( 0, kMainScreenHeight - keyBoardRect.size.height-kMainScreenHeight * 0.1, kMainScreenWidth, kMainScreenHeight*0.1);
        self.countriesTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height-deltaY) ;
        self.allianceTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height-deltaY) ;
        
        if (type == IOS_CHANNEL_TYPE_COUNTRY) {
            [self tableViewScrollToBottomWithTableView:self.countriesTableViewController.tableView andAnimal:YES];
        }else if(type == IOS_CHANNEL_TYPE_ALLIANCE){
            [self tableViewScrollToBottomWithTableView:self.allianceTableViewController.tableView andAnimal:YES];
        }
    }
}


/**滚动到当前行*/
-(void)tableViewScrollToBottomWithTableView:(UITableView *)vTableView andAnimal:(BOOL)animal;
{
    if (vTableView.contentSize.height > vTableView.frame.size.height)
    {
        CGPoint offset = CGPointMake(0, vTableView.contentSize.height - vTableView.frame.size.height);
        [vTableView setContentOffset:offset animated:animal];
    }else{
        DVLog(@"没有滚动");
    }
}
-(void)keyboardWasHidden:(NSNotification *)note
{
    CGRect keyBoardRect=[note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGFloat deltaY=keyBoardRect.size.height;
    int type = ChatServiceCocos2dx::m_channelType;
 
    self.keyBordView.frame = CGRectMake(0, kMainScreenHeight - kMainScreenHeight * 0.1, kMainScreenWidth, kMainScreenHeight * 0.1);
    //            self.countriesTableViewController.view.transform = CGAffineTransformMakeTranslation(0, 0);
    self.countriesTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height) ;
     self.allianceTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height) ;
}


#pragma mark -
#pragma mark UITopViewDelegate

-(void)topUIViewCancalButtonAction{
    [self.keyBordView.chatViewTextField resignFirstResponder];
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
//    [self.countriesTableViewController.dataSourceArray removeAllObjects];
//    [self.allianceTableViewController.dataSourceArray removeAllObjects];
    [self.countriesTableViewController.tableView endUpdates];
    [self.countriesTableViewController.currentChannel msgArrayFormatToStartState];
    self.countriesTableViewController.currentChannel.channelDelegate = nil;
    [self.allianceTableViewController.tableView endUpdates];
    [self.allianceTableViewController.currentChannel msgArrayFormatToStartState];

    self.allianceTableViewController.currentChannel.channelDelegate = nil;
    
    
    
}
- (void)clickButtonWithTag:(NSInteger)vTag{
    DVLog(@"vTag :%d",vTag);
    if  ([self.keyBordView.chatViewTextField canBecomeFirstResponder]){
        if(vTag == 10001){
          
//            if(self.countriesTableViewController.dataSourceArray.count == 0){
//                 ChatChannel *countryChannel =  [[ChannelManager sharedChannelManager]gettingCountryChannel];
//                [countryChannel gettingFirstMsg];
//            }
        }else if ( vTag == 10002){
//            if(self.allianceTableViewController.dataSourceArray.count == 0){
            
//                ChatChannel *allianceChannel =  [[ChannelManager sharedChannelManager]gettingAllianceChannel];
//                [allianceChannel gettingFirstMsg];
//            }
        }

    }
    
}

//单机背景隐藏软键盘
-(void)keyboardHide:(UITapGestureRecognizer*)tap{
    [self chatVCEndingEdit];
}
-(void)chatVCEndingEdit{
    [self.keyBordView.chatViewTextField resignFirstResponder];

}

-(void)KeyBordView:(KeyBordVIew *)keyBoardView textFiledBegin:(UITextField *)textFiled
{
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    if (type == IOS_CHANNEL_TYPE_COUNTRY) {
        [self.countriesTableViewController tableViewScrollCurrentLine];
    }else if(type == IOS_CHANNEL_TYPE_ALLIANCE){
        [self.allianceTableViewController tableViewScrollCurrentLine];
    }
    
}

- (void)addUITableView
{
    
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    CGFloat x = 0;
    CGFloat y = winSize.height * 0.14;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.77 ;

    //add countriesTableViewController
    self.countriesTableViewController = [[ChatCountriesTableViewController alloc]initWithStyle:UITableViewStylePlain];
    self.countriesTableViewController.tableView.frame=CGRectMake(x,y,width,height);
    self.countriesTabelViewFrame = self.countriesTableViewController.tableView.frame;
    [self.view addSubview:self.countriesTableViewController.tableView];
    
    
    //add allianceTableViewController
    NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
    self.allianceTableViewController = [[ChatAllianceTableViewController alloc]initWithStyle:UITableViewStylePlain];
    self.allianceTableViewController.tableView.frame=CGRectMake(x,y,width,height);
    self.allianceTabelViewFrame = self.allianceTableViewController.tableView.frame;
    [self.view addSubview:self.allianceTableViewController.tableView];
    
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        self.allianceTableViewController.view.hidden = YES;
        self.countriesTableViewController.view.hidden = NO;
    }else if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        self.countriesTableViewController.view.hidden = YES;
        self.allianceTableViewController.view.hidden = NO;
    }
    
}

- (void)addUIKeyBordVIew
{
    //add keyBordView
    self.keyBordView = [[KeyBordVIew alloc]initWithFrame:CGRectMake(0, kMainScreenHeight - kMainScreenHeight * 0.1, kMainScreenWidth, kMainScreenHeight * 0.1)];
    self.keyBordView.delegate=self;
//    self.keyBordView.frame = CGRectMake(0, kMainScreenHeight - kMainScreenHeight * 0.1, kMainScreenWidth, kMainScreenHeight * 0.1);
    DVLog(@" self.keyBordView.frame   %@", NSStringFromCGRect( self.keyBordView.frame));
    [self.view addSubview:self.keyBordView];
    
//    self.keyBordView.translatesAutoresizingMaskIntoConstraints=NO;
    
//    NSDictionary *views = NSDictionaryOfVariableBindings(_keyBordView);
    
//    NSArray *keyBordView_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_keyBordView]-0-|"
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:NSDictionaryOfVariableBindings(_keyBordView)];
//    
//    NSArray *keyBordView_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_keyBordView]-0-|"
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
//                           　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
//    
//    [self.view addConstraint:[NSLayoutConstraint constraintWithItem:self.keyBordView
//                                                     attribute:NSLayoutAttributeHeight
//                                                     relatedBy:NSLayoutRelationEqual
//                                                        toItem:self.view
//                                                     attribute:NSLayoutAttributeHeight
//                                                    multiplier:0.1
//                                                      constant:0]];
//    
//    [self.view addConstraints:keyBordView_c_H];
//    [self.view addConstraints:keyBordView_c_V];
//    
    
}


-(void) addTopUIView
{
    
    self.topUIView=[[TopUIView alloc]init:0];
    self.topUIView.topUIViewDelegate = self;
    self.topUIView.translatesAutoresizingMaskIntoConstraints=NO;
    [self.view addSubview:self.topUIView];
    self.topUIView.backgroundColor = [UIColor blackColor];
    
//    self.topUIView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight* 0.14);
    
    NSDictionary *views = NSDictionaryOfVariableBindings(_topUIView);
    
    NSArray *topUIView_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_topUIView]-0-|"
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
    
    NSArray *topUIView_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[_topUIView]"
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
    
    [self.view addConstraint:[NSLayoutConstraint constraintWithItem:self.topUIView
                                                     attribute:NSLayoutAttributeHeight
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:self.view
                                                     attribute:NSLayoutAttributeHeight
                                                    multiplier:0.14
                                                      constant:0]];
    
    [self.view addConstraints:topUIView_c_H];
    [self.view addConstraints:topUIView_c_V];
//
    NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
    if ([allianceId isEqualToString:@""]) {
        self.keyBordView.hidden = YES;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}


- (void)applicationWillResignActive:(NSNotification *)notification

{
    
    NSDate *datenow = [NSDate date];
    
    [ServiceInterface serviceInterfaceSharedManager].screenLockTime = (long)[datenow timeIntervalSince1970];
    
    [ServiceInterface serviceInterfaceSharedManager].isLockScreen = YES;
    
//    [self.allianceTableViewController.tableView reloadData];
//    [self.countriesTableViewController.tableView reloadData];

}

- (void)applicationDidBecomeActive:(NSNotification *)notification
{
    
    NSDate *datenow = [NSDate date];
        
    long time = (long)[datenow timeIntervalSince1970]-[ServiceInterface serviceInterfaceSharedManager].screenLockTime;
        
    DVLog(@"%ld",time);
        
    if (time > 60)
    {
        //判断时间超过60秒隐藏界面
        [ServiceInterface serviceInterfaceSharedManager].screenLockTime = 0.0;
            
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
            
        [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    }
    
    [[ServiceInterface serviceInterfaceSharedManager] initIsLoadVariable];
    
    [ServiceInterface serviceInterfaceSharedManager].isLockScreen = NO;
}

-(void) addJoinAllance
{
    self.jionAllanceView = [[UIView alloc] init];
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    CGFloat x = 0;
    CGFloat y = winSize.height * 0.14;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.86;
    self.jionAllanceView.frame = CGRectMake(x,y,width,height);
    
    UIImageView *jionAllanceBack = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    jionAllanceBack.frame = self.jionAllanceView.bounds;
    
    UILabel *tipJionAllanceLabel = [[UILabel alloc]init];
    tipJionAllanceLabel.lineBreakMode = NSLineBreakByWordWrapping;
    tipJionAllanceLabel.numberOfLines = 0;
    tipJionAllanceLabel.backgroundColor = [UIColor clearColor];
    CGFloat fontSize = [ServiceInterface serviceInterfaceSharedManager].fontSize;
    tipJionAllanceLabel.font = [UIFont systemFontOfSize:fontSize];
    tipJionAllanceLabel.text = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_JOIN_ALLIANCE];
    CGFloat tipJionAllanceLabel_x = self.jionAllanceView.frame.size.width / 4;
    CGFloat tipJionAllanceLabel_y = self.jionAllanceView.frame.size.height / 5;
    CGFloat tipJionAllanceLabel_width = self.view.frame.size.width * 0.6;
    CGFloat tipJionAllanceLabel_height = self.jionAllanceView.frame.size.height * 0.2;
    tipJionAllanceLabel.frame = CGRectMake(tipJionAllanceLabel_x, tipJionAllanceLabel_y, tipJionAllanceLabel_width, tipJionAllanceLabel_height);
    
    UIButton *jionAllanceBtn = [[UIButton alloc]init];
    [jionAllanceBtn setBackgroundImage:[UIImage imageNamed:@"chat_bottom_smile_nor.png"] forState:UIControlStateNormal];
    NSString *jionStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_JOIN];
    [jionAllanceBtn setTitle:jionStr forState:UIControlStateNormal];
    [jionAllanceBtn setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
    
    [jionAllanceBtn addTarget:self action:@selector(jionFun:) forControlEvents:UIControlEventTouchUpInside];
    
    CGFloat jionAllanceBtn_x = self.jionAllanceView.frame.size.width * 0.3;
    CGFloat jionAllanceBtn_y = self.jionAllanceView.frame.size.height * 0.5;
    CGFloat jionAllanceBtn_width = self.view.frame.size.width * 0.45;
    CGFloat jionAllanceBtn_height = self.jionAllanceView.frame.size.height * 0.1;

    jionAllanceBtn.frame = CGRectMake(jionAllanceBtn_x, jionAllanceBtn_y, jionAllanceBtn_width, jionAllanceBtn_height);
    
    [self.jionAllanceView addSubview:jionAllanceBack];
    [self.jionAllanceView addSubview:tipJionAllanceLabel];
    [self.jionAllanceView addSubview:jionAllanceBtn];
    [self.view addSubview:self.jionAllanceView];
    
}

-(void)jionFun:(UIButton *)image
{
    NSString *uid = [UserManager sharedUserManager].currentUser.uid;
    NSString *name = [UserManager sharedUserManager].currentUser.userName;
    [[ChatServiceController chatServiceControllerSharedManager].gameHost joinAllianceBtnClick];
}

-(void) isShowJionAllance{
    if([[UserManager sharedUserManager].currentUser.allianceId isEqualToString:@""] || [UserManager sharedUserManager].currentUser.allianceId == nil ){
        [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
        [self.view bringSubviewToFront:self.jionAllanceView];
    }else{
        [self.view sendSubviewToBack:self.jionAllanceView];
        self.allianceTableViewController.view.hidden = NO;
        [self.view bringSubviewToFront:self.allianceTableViewController.view];
    }
}

-(void) hiddenJionAllanceView
{
    [self.view sendSubviewToBack:self.jionAllanceView];
}

-(void)openFriendsView{
    [[ChatServiceController chatServiceControllerSharedManager].gameHost openFriendsView];
}

@end
