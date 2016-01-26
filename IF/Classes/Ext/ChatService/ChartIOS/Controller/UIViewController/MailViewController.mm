//
//  MailViewController.m
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import "MailViewController.h"
#import "TopUIView.h"
#import "UIView+FrameMethods.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "ChatServiceCocos2dx.h"
#import "UserManager.h"
#import "ChannelManager.h"
#import "PersonSelectVC.h"
#import "UITableViewController+Extension.h"
#import "CCSafeNotificationCenter.h"
#import "MailController.h"
#define ISSYSTEMVERSION7 ([[UIDevice currentDevice].systemVersion floatValue] == 7.1 ? YES : NO)
@interface MailViewController ()<KeyBordVIewDelegate,TopUIViewDelegate>

@property (nonatomic,strong) UIView *navBar;
@property (nonatomic,assign) CGRect mailTabelViewFrame;

-(void) addTopUIView;
-(void) addUITableView;
-(void) addUIKeyBordVIew;

@end

@implementation MailViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.navigationBarHidden=YES;
    self.view.backgroundColor = [UIColor blackColor];
    
    [self addTopUIView];
    [self addUITableView];
    [self addUIKeyBordVIew];
    [self.view bringSubviewToFront:self.topUIView];
    
    UITapGestureRecognizer *aTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(keyboardHide:)];
    //设置成NO表示当前控件响应后会传播到其他控件上，默认为YES。
    aTapGestureRecognizer.cancelsTouchesInView = NO;
    //将触摸事件添加到TableView
    [self.mailTableTableViewController.tableView addGestureRecognizer:aTapGestureRecognizer];
    
  


}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasShown:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasHidden:) name:UIKeyboardWillHideNotification object:nil];
//        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(chatMsgPushNotify:) name:kChatMailMsgPushFromServer object:nil];
}
-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIKeyboardWillHideNotification object:nil];
//        [[NSNotificationCenter defaultCenter]removeObserver:self name:kChatMailMsgPushFromServer object:nil];
}
-(void)keyboardWasShown:(NSNotification *)note
{

    int type = ChatServiceCocos2dx::m_channelType;
    
    if (type == IOS_CHANNEL_TYPE_CHATROOM || type == IOS_CHANNEL_TYPE_USER) {
        if ([[ServiceInterface serviceInterfaceSharedManager] isSystemVersion7]) {
            CGSize winSize=[UIScreen mainScreen].bounds.size;
            CGRect keyBoardRect=[note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
            CGFloat deltaY=keyBoardRect.size.height;
            
            DVLog(@"%f",kMainScreenHeight - keyBoardRect.size.height- kMainScreenHeight*0.1);
            
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.15 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                self.keyBordView.frame = CGRectMake( 0, kMainScreenHeight - keyBoardRect.size.height- kMainScreenHeight*0.1, kMainScreenWidth, kMainScreenHeight*0.1);
            });
            
            self.mailTableTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height-deltaY) ;
            [self tableViewScrollToBottomWithTableView:self.mailTableTableViewController.tableView andAnimal:YES];
            [self.mailTableTableViewController tableViewScrollCurrentLine];
        }else{
            CGRect keyBoardRect=[note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
            CGFloat deltaY=keyBoardRect.size.height;
            CGSize winSize=[UIScreen mainScreen].bounds.size;
            
            
            [UIView animateWithDuration:[note.userInfo[UIKeyboardAnimationDurationUserInfoKey] floatValue] animations:^{
                self.keyBordView.transform=CGAffineTransformMakeTranslation(0, -deltaY);
                float tableViewY = self.mailTableTableViewController.tableView.frame.origin.y - winSize.height * 0.01;;
                float tableViewH = self.keyBordView.frame.origin.y - self.topUIView.topView.frame.size.height + winSize.height * 0.02;
                self.mailTableTableViewController.tableView.frame = CGRectMake(0.0f, tableViewY, self.view.frame.size.width, tableViewH);
                
            }];
            [self.mailTableTableViewController tableViewScrollCurrentLine];
        }
    }
}
-(void)keyboardWasHidden:(NSNotification *)note
{
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    if (type == IOS_CHANNEL_TYPE_USER || type == IOS_CHANNEL_TYPE_CHATROOM) {
        if ([[ServiceInterface serviceInterfaceSharedManager] isSystemVersion7]) {
            CGRect keyBoardRect=[note.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
            CGFloat deltaY=keyBoardRect.size.height;
            self.keyBordView.frame = CGRectMake(0, kMainScreenHeight - kMainScreenHeight * 0.1, kMainScreenWidth, kMainScreenHeight * 0.1);
            self.mailTableTableViewController.tableView.frame = CGRectMake(0, self.topUIView.bottom, kMainScreenWidth, kMainScreenHeight - self.keyBordView.height-self.topUIView.height) ;
        }else{
            [UIView animateWithDuration:[note.userInfo[UIKeyboardAnimationDurationUserInfoKey] floatValue] animations:^{
                self.keyBordView.transform = CGAffineTransformIdentity;
                self.mailTableTableViewController.tableView.frame = self.mailTabelViewFrame;
            }];
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

//单机背景隐藏软键盘
-(void)keyboardHide:(UITapGestureRecognizer*)tap{
    [self endEditWithTextField];
    
}

-(void)endEditWithTextField{
    [self.keyBordView.chatViewTextField resignFirstResponder];
}


-(void)KeyBordView:(KeyBordVIew *)keyBoardView textFiledBegin:(UITextField *)textFiled
{
    [self.mailTableTableViewController tableViewScrollCurrentLine];
}

- (void)addUIKeyBordVIew
{
    self.keyBordView=[[KeyBordVIew alloc]init];
    self.keyBordView.delegate=self;
    
    [self.view addSubview:self.keyBordView];
    [self.keyBordView hiddenRadioView];
    
    self.keyBordView.translatesAutoresizingMaskIntoConstraints=NO;
    
    NSDictionary *views = NSDictionaryOfVariableBindings(_keyBordView);
    
    NSArray *keyBordView_c_H=[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[_keyBordView]-0-|"
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
    
    NSArray *keyBordView_c_V=[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_keyBordView]-0-|"
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　options:0
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　metrics:nil
                              　　　　　　　　　　　　　　　　　　　　　　　　　　　　views:views];
    
    [self.view addConstraint:[NSLayoutConstraint constraintWithItem:self.keyBordView
                                                          attribute:NSLayoutAttributeHeight
                                                          relatedBy:NSLayoutRelationEqual
                                                             toItem:self.view
                                                          attribute:NSLayoutAttributeHeight
                                                         multiplier:0.1
                                                           constant:0]];
    
    [self.view addConstraints:keyBordView_c_H];
    [self.view addConstraints:keyBordView_c_V];
    
}

-(void) addTopUIView
{
    self.topUIView=[[TopUIView alloc] init:IOS_CHANNEL_TYPE_USER];
    self.topUIView.topUIViewDelegate = self;
    self.topUIView.translatesAutoresizingMaskIntoConstraints=NO;
    [self.view addSubview:self.topUIView];
    
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
                                                             multiplier:0.07
                                                               constant:0]];

    
    [self.view addConstraints:topUIView_c_H];
    [self.view addConstraints:topUIView_c_V];
}

- (void)addUITableView
{
    
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    CGFloat x = 0;
    CGFloat y = self.view.frame.size.height * 0.07;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.83 ;
   
    //add allianceTableViewController
    self.mailTableTableViewController = [[ChatMailTableTableViewController alloc] initWithStyle:UITableViewStylePlain];
    self.mailTableTableViewController.view.frame = CGRectMake(x, y, width, height);
    self.mailTabelViewFrame = CGRectMake(x, y, width, height);
    [self.view addSubview:self.mailTableTableViewController.tableView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

//- (void)applicationWillResignActive:(NSNotification *)notification
//{
////    [[ChannelManager sharedChannelManager].channel_map removeAllObjects];
//}
//
//- (void)applicationDidBecomeActive:(NSNotification *)notification
//{
//    if ([ServiceInterface serviceInterfaceSharedManager].vc_win.hidden == NO && ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_USER){
//    
//        NSString *ns_fromUid = [UserManager sharedUserManager].currentMail.opponentUid;
//        NSString *ns_fromName = [UserManager sharedUserManager].currentMail.opponentName;
//        
//        [[ChatServiceController chatServiceControllerSharedManager].gameHost openMailDialogViewFirst:ns_fromUid :ns_fromName];
//
//    
//    }
//}

#pragma mark -
#pragma mark TopUIView Delegate
-(void)TopUIViewRightButtonAction{
    [self.keyBordView.chatViewTextField resignFirstResponder];
    PersonSelectVC *personSelectVC;
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_USER) {
        personSelectVC =[[PersonSelectVC alloc]initWithType:PersonSelectVCType_New];
    }else if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_CHATROOM){
        personSelectVC =[[PersonSelectVC alloc]initWithType:PersonSelectVCType_ChangeMember];
    }
    personSelectVC.personVCOpenFrom = PersonVCOpenFrom_CocosAdd;
//    personSelectVC.ispushFrom_OC_Native = YES;
   
    NSString *groupChatKey = [UserManager sharedUserManager].currentMail.opponentUid;
    ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:groupChatKey];
    DVLog(@"%@",chatChannel);
    DVLog(@"当前群聊组的人员UID :%@",chatChannel.memberUidArray);
    personSelectVC.memberInGroupArr = chatChannel.memberUidArray;
    personSelectVC.chatChannel = chatChannel;
    
    UIWindow *keywindow= [[[UIApplication sharedApplication ]delegate]window];
     [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
    [keywindow bringSubviewToFront:[ServiceInterface serviceInterfaceSharedManager].chatRootWindow];
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = personSelectVC;
}
//返回按钮
-(void)topUIViewCancalButtonAction{
    [self.keyBordView.chatViewTextField resignFirstResponder];
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    self.mailTableTableViewController.currentChatChannel.channelDelegate = nil;
    [self.mailTableTableViewController.currentChatChannel.msgList removeAllObjects];
    self.mailTableTableViewController.currentChatChannel = nil;
}


@end
