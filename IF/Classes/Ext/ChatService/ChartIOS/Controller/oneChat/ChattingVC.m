//
//  ChattingVC.m
//  IF
//
//  Created by 张彦涛 on 15/6/30.
//
//

#import "ChattingVC.h"
#import "ChatTableView.h"
#import "ServiceInterface.h"
#import "ChatCellIOS.h"
#import "TopHeadView.h"
#import "EditBarView.h"
#import "UITableView+FDTemplateLayoutCell.h"
#import "NSString+Cocos2dHelper.h"
#import "TopButtonView.h"
#import "ChatServiceController.h"
#import "MJRefresh.h"

@interface ChattingVC ()<UIScrollViewDelegate,UITableViewDataSource,UITableViewDelegate,TopHeadViewDelegate,EditBarViewDelegate,ChatChannelDelegate,TopButtonViewDelegate,UIScrollViewDelegate>{

    
    CGRect CountryoldBottomFrame;
    CGRect CountryldEditBarFrame;
    
    CGRect AllianceoldBottomFrame;
    CGRect AllianceldEditBarFrame;
    
    CGFloat keyH;
    
    CGFloat editBarH;
    int HeadH;
    
}


@property (strong,nonatomic) UIView * headView;
@property (nonatomic,strong) UIView  * buttonView;
@property (strong,nonatomic) UIView * bottomView;
@property (strong,nonatomic) TopHeadView *topHeadView;
@property (strong,nonatomic)    TopButtonView *topButtonView;
@property (nonatomic,strong) EditBarView * editBarView;
@property (strong,nonatomic)  UIScrollView *chatScrollView;
@property (nonatomic,strong) ChatTableView * countryTableView;
@property (nonatomic,strong) ChatTableView * allianceTableView;

@property (nonatomic,strong) NSMutableArray * countryDataSourceArray;
@property (nonatomic,strong) NSMutableArray * allianceDataSourceArray;


@property (nonatomic,assign,getter=isCountryHasNewMsgPushed)BOOL countryHasNewMsgPushed;
@property (nonatomic,assign,getter=isCountryHasLoadOldMsgPushed)BOOL countryHasLoadOldMsgPushed;

@property (nonatomic,assign,getter=isAllianceHasNewMsgPushed)BOOL allianceHasNewMsgPushed;
@property (nonatomic,assign,getter=isAllianceHasLoadOldMsgPushed)BOOL allianceHasLoadOldMsgPushed;

@property (nonatomic,assign,getter=isCountryRefreshingTure)BOOL countryRefreshing;
@property (nonatomic,assign,getter=isAllianceRefreshingTure)BOOL allianceRefreshing;

@end
 
static NSString *countryIdentify = @"countryIdentify";
 static NSString *allianceIdentify = @"allianceIdentify";
@implementation ChattingVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    [self.view addSubview:self.headView];
    
    
    
   
    [self.headView addSubview:self.topHeadView];
    [self settingInputView];
    [self.view addSubview:self.buttonView];
    [self.view addSubview:self.bottomView];
    [self.view insertSubview:self.chatScrollView atIndex:0] ;
    [self.chatScrollView addSubview:self.countryTableView];
    [self.chatScrollView addSubview:self.allianceTableView];
    [self settingHeadRefeashWithTableView:self.countryTableView];
    [self settingHeadRefeashWithTableView:self.allianceTableView];
    
    AllianceoldBottomFrame=self.bottomView.frame;
    AllianceldEditBarFrame=self.editBarView.frame;
    CountryoldBottomFrame=self.bottomView.frame;
    CountryldEditBarFrame=self.editBarView.frame;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
    //顶部ButtonView
    TopButtonView *topButtonView =[TopButtonView topButtonViewWithLeftButtonName:[NSString stringWithMultilingualWithKey:@"105300"] andWithRightButtonName:[NSString stringWithMultilingualWithKey:@"105602"]];//105602 = 联盟  105300=国家
    topButtonView.frame = CGRectMake(0, 0, self.buttonView.width, self.buttonView.height );
//    [topButtonView settingLeftButtonSelected:YES];
    topButtonView.topButtonViewDelegate = self;
    self.topButtonView = topButtonView;
    [self.buttonView addSubview: self.topButtonView];
    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
         [self.topButtonView settingLeftButtonSelected:YES];
         self.editBarView.isCountry=YES;
    }else{
         [self.topButtonView settingRightButtonSelected:YES];
         self.chatVCShowTableType = ChatVCShowTableType_right;
        self.editBarView.isCountry=NO;
    }
    
    
//    [self.chatScrollView addSubview:[self addJoinAllance]];

}
-(void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillChangeFrameNotification object:nil];
    
}

-(void)settingInputView{
 
    [self.bottomView addSubview:self.editBarView];
    [self.bottomView bringSubviewToFront:self.editBarView];
}

#pragma mark -
#pragma mark getter
-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight *0.07)];
        headView.backgroundColor = [UIColor grayColor];
        _headView = headView;
    }
    return _headView;
}
-(UIView *)buttonView{
    if (_buttonView == nil) {
        UIView *buttonView =[[UIView alloc]initWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight *0.07)];
        buttonView.backgroundColor =[UIColor grayColor];
        _buttonView  = buttonView;
    }
    return _buttonView;
}
-(UIView *)bottomView{
    if (_bottomView == nil) {
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            editBarH=65;
            HeadH=35;
        }else{
            editBarH=56.8;
            HeadH=25;
        }
        UIView *bottomView =[[UIView alloc]initWithFrame:CGRectMake(0, kMainScreenHeight - editBarH, kMainScreenWidth, editBarH)];
        bottomView.backgroundColor =[UIColor redColor];
        _bottomView = bottomView;
    
    }
    return _bottomView;
}
-(TopHeadView *)topHeadView{
    if (_topHeadView == nil) {
        TopHeadView *topView = [TopHeadView topHeadViewWithRightButtonShowFlag:NO andShowRightButtonType:TopHeadViewRightButtonType_ChatRoomMember];
        topView.frame = self.headView.frame;
        topView.topHeadViewDelegate = self;
        topView.titleNameLabel.text =[NSString stringWithMultilingualWithKey:@"105316"];//105316=聊天
        _topHeadView = topView;
    }
    return  _topHeadView;
}

-(void)passChangTextViewHeight:(CGFloat)changHeight
{
    
    
    self.bottomView.frame=CGRectMake(0, self.bottomView.frame.origin.y-changHeight, kMainScreenWidth, self.bottomView.frame.size.height+changHeight);
    self.editBarView.frame=CGRectMake(0, 0, kMainScreenWidth, self.editBarView.frame.size.height+changHeight);
    
    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.chatScrollView.frame.size.height-changHeight) ;
    self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.self.countryTableView.size.height-changHeight);
    self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.allianceTableView.frame.size.height-changHeight);
    
    
    if (changHeight>0) {
        [self.countryTableView setContentOffset:CGPointMake(self.countryTableView.contentOffset.x, self.countryTableView.contentOffset.y+changHeight) animated:NO];
        [self.allianceTableView setContentOffset:CGPointMake(self.allianceTableView.contentOffset.x, self.allianceTableView.contentOffset.y+changHeight) animated:NO];
    }
    
    
}
-(EditBarView *)editBarView{
    if (_editBarView == nil) {
        _editBarView = [[EditBarView alloc]initWithFrame:CGRectMake(0, 0, self.bottomView.width, self.bottomView.height)];
        _editBarView.backgroundColor=[UIColor orangeColor];
        self.editBarView.editBarViewDelegate = self;
       
    }
    return _editBarView;
}
-(UIScrollView *)chatScrollView{
    if (_chatScrollView == nil) {
        CGRect scrollViewFrame = CGRectMake(0, self.buttonView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height-self.bottomView.height - self.buttonView.height);
        UIScrollView *scrollerView =[[UIScrollView alloc]initWithFrame:scrollViewFrame];
        scrollerView.contentSize = CGSizeMake(scrollViewFrame.size.width * 2,0);
        scrollerView.pagingEnabled = YES;
        scrollerView.delegate = self;
        scrollerView.bounces=NO;
        //水平滚动条
        scrollerView.showsHorizontalScrollIndicator =NO;
        _chatScrollView = scrollerView;
      
        _chatScrollView.backgroundColor = [UIColor purpleColor];
    }
    return _chatScrollView;
}

-(ChatTableView *)countryTableView{
    if (_countryTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(0, 0, self.chatScrollView.width, self.chatScrollView.height) style:UITableViewStylePlain];
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
        tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
        
 
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
        [tableView addGestureRecognizer:tap];
        _countryTableView = tableView;
        _countryTableView.dataSource = self;
        _countryTableView.delegate = self;
    }
    return _countryTableView;
}
-(ChatTableView *)allianceTableView{
    if (_allianceTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(self.chatScrollView.width, 0, self.chatScrollView.width, self.chatScrollView.height) style:UITableViewStylePlain];
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
        
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
        [tableView addGestureRecognizer:tap];
        tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
        _allianceTableView = tableView;
        _allianceTableView.dataSource = self;
        _allianceTableView.delegate = self;
    }
    return _allianceTableView;
}
-(NSMutableArray *)countryDataSourceArray{
    if (_countryDataSourceArray == nil) {
        _countryDataSourceArray =[NSMutableArray array ];
    }
    return _countryDataSourceArray;
}

-(NSMutableArray *)allianceDataSourceArray{
    if (_allianceDataSourceArray ==nil) {
        _allianceDataSourceArray =[NSMutableArray array ];
    }
    return _allianceDataSourceArray;
}

#pragma mark -
#pragma mark setter
-(void)setChatVCShowTableType:(ChatVCShowTableType)chatVCShowTableType{
    _chatVCShowTableType = chatVCShowTableType;
    
    if (_chatVCShowTableType ==ChatVCShowTableType_left) {
        CGPoint point = CGPointMake(0, 0);
        [self.chatScrollView setContentOffset:point animated:YES];
        _editBarView.speakerButtonShowing = YES;
        [self.topButtonView settingLeftButtonSelected:YES];
     //[self bottomBarFrameChangeWithEditBarShowSpeakAll:YES];
    }else{
        _editBarView.speakerButtonShowing = NO;
        [self.topButtonView settingRightButtonSelected:YES];
        CGPoint point = CGPointMake(self.chatScrollView.width, 0);
        [self.chatScrollView setContentOffset:point animated:YES];
        //[self bottomBarFrameChangeWithEditBarShowSpeakAll:NO];
        
    
    }
}


-(void)setCountryChannel:(ChatChannel *)countryChannel{
    _countryChannel = countryChannel;
    _countryChannel.channelDelegate = self;
}
-(void)setAllianceChannel:(ChatChannel *)allianceChannel{
    _allianceChannel = allianceChannel;
    _allianceChannel.channelDelegate = self;
}
#pragma mark -
#pragma mark action

-(void)settingHeadRefeashWithTableView:(UITableView *)vTableView{
    NSString *headTitleString =[NSString stringWithMultilingualWithKey:@"105327"];//105327=下拉可加载更多
    __weak ChattingVC *tempVC =self;
    MJRefreshNormalHeader *header= [MJRefreshNormalHeader headerWithRefreshingBlock:^{
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [tempVC loadMoreOldMsg];
 
        });
    }];
    header.automaticallyChangeAlpha = YES;
    header.lastUpdatedTimeLabel.hidden = YES;
    [header setTitle:headTitleString forState:MJRefreshStatePulling];
    [header setTitle:headTitleString forState:MJRefreshStateIdle];
    [header setTitle:headTitleString forState:MJRefreshStateRefreshing];
    vTableView.header = header;

}
-(void)sendMsg:(NSString *)vMsgString{
    NSMsgItem *msgItem = [self msgItemWithSendString:vMsgString];
    //自己发的消息，先入库
    msgItem.sendState = 1;
    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
        if (self.editBarView.speakerChangeButton.selected ) {
            //喇叭消息
            msgItem.sendState = 0;
            
        }else{
            [self.countryChannel savingMsgForSelfSendToServerWithMsgItem:msgItem];
            msgItem.sendState = 0;
            [self.countryChannel chatNewMsgPushed:@[msgItem]];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost  sendText:msgItem :0];

        }
        
    }else{
        [self.allianceChannel savingMsgForSelfSendToServerWithMsgItem:msgItem];
         msgItem.sendState = 0;
         [self.allianceChannel chatNewMsgPushed:@[msgItem]];
          [[ChatServiceController chatServiceControllerSharedManager].gameHost  sendText:msgItem :2];
    }
 
    
}
-(NSMsgItem *)msgItemWithSendString:(NSString *)vMsgString{
    NSMsgItem *chatMessage = [[ServiceInterface serviceInterfaceSharedManager] createChatMessage:vMsgString];
    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
        chatMessage.channelType = self.countryChannel.channelType;
    }else{
         chatMessage.channelType = self.allianceChannel.channelType;
    }
    
    return chatMessage;
}




-(void)keyBoardClosed{
    [self.editBarView.inputTextField resignFirstResponder];
    
}

-(void)bottomBarFrameChangeWithEditBarShowSpeakAll:(BOOL)vShowSpeakAllYes{
    int  editBarViewBottom = self.bottomView.bottom;
    if (  vShowSpeakAllYes == YES) {
        
       
        if (self.editBarView.speakerChangeButton.selected) {
              self.bottomView.height = self.editBarView.bodyHeght + self.editBarView.headHeight;
        }else{
              self.bottomView.height = self.editBarView.bodyHeght;
        }
      
        self.bottomView.top = editBarViewBottom - self.bottomView.height;
        
 
        self.editBarView.height = self.bottomView.height;
        self.chatScrollView.height = self.bottomView.top-self.buttonView.bottom;
        self.countryTableView.height = self.chatScrollView.height;
    }else{
//        if (self.editBarView.speakerChangeButton.selected) {
              self.bottomView.height = self.editBarView.bodyHeght  ;
              self.bottomView.top = editBarViewBottom - self.bottomView.height;
 
            self.editBarView.height = self.bottomView.height;
            self.chatScrollView.height = self.bottomView.top-self.buttonView.bottom;
            self.countryTableView.height = self.chatScrollView.height;

//        }
        
    }

}

#pragma mark -
#pragma mark MJRefeash 调用方法
-(void)loadMoreOldMsg{
    if (self.chatVCShowTableType ==ChatVCShowTableType_left) {
        //国家
        if (_countryChannel.msgList.count == 0){
            self.countryRefreshing = YES;
        }else{
            self.countryRefreshing = NO;
        }
        if (self.isCountryRefreshingTure == NO) {
            self.countryRefreshing = ![self beginFreshedWithTableView:_countryTableView];
            if (self.isCountryRefreshingTure == YES) {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                    // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
                    if(self.isCountryRefreshingTure == YES ){
                        [self endRefreshedWithTableView:_countryTableView];
                    }
                });
            }else{
                [self endRefreshedWithTableView:_countryTableView];
            }
        }else{
            [self.countryChannel gettingFirstMsg];
           [self endRefreshedWithTableView:_countryTableView];
        }
        
    }else{
        //联盟
 
        if (_allianceChannel.msgList.count == 0){
            self.allianceRefreshing = YES;
        }else{
            self.allianceRefreshing = NO;
        }
        if (self.isAllianceRefreshingTure == NO) {
            self.allianceRefreshing = ![self beginFreshedWithTableView:_allianceTableView];
            if ([self isAllianceRefreshingTure] == YES) {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                    // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
                    if([self isAllianceRefreshingTure] == YES ){
                        [self endRefreshedWithTableView:_allianceTableView];
                    }
                });
            }else{
                [self endRefreshedWithTableView:_allianceTableView];
            }
        }else{
            [self.allianceChannel gettingFirstMsg];
            [self endRefreshedWithTableView:_allianceTableView];
        }

    }
}

#pragma mark -
#pragma mark topHeadView  delegate

-(void)clickButton{
    [self.countryChannel msgArrayFormatToStartState];
    [self.allianceChannel msgArrayFormatToStartState];
    self.countryChannel.channelDelegate = nil;
    self.countryChannel = nil;
    self.allianceChannel.channelDelegate = nil;
    self.allianceChannel = nil;
    
    
        //新版OC原生邮件列表
        UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
        if ([nav.viewControllers objectAtIndex:0] == self){
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
            [[ServiceInterface serviceInterfaceSharedManager]hideMailListIOS];
            
        }else{
            [self.navigationController popViewControllerAnimated:YES];
        }
        
       
 }
#pragma mark -
#pragma mark topButtonView delegate
-(void)topButtonViewBtnPressedAction:(UIButton *)sender{
   
    if (sender.tag == 101){
        //左侧按钮
        DVLog(@"左侧按钮");
        CGPoint point = CGPointMake(0, 0);
        self.chatVCShowTableType = ChatVCShowTableType_left;
        [self.chatScrollView setContentOffset:point animated:YES];
        [self SetCountryAny];
        
    }else if(sender.tag == 102){
        //右侧按钮
        DVLog(@"右侧按钮");
        self.chatVCShowTableType = ChatVCShowTableType_right;
        CGPoint point = CGPointMake(self.chatScrollView.width, 0);
        [self.chatScrollView setContentOffset:point animated:YES];
        [self SetAllianceAny];
    }
    
    self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.chatScrollView.frame.size.height);
    self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.chatScrollView.frame.size.height);
 
}
-(void)SetCountryAny
{
    //保存
    AllianceldEditBarFrame=self.editBarView.frame;
    AllianceoldBottomFrame=self.bottomView.frame;
    
    if (keyH>0) {
        //当前键盘出来
        self.editBarView.frame=CGRectMake(CountryldEditBarFrame.origin.x,CountryldEditBarFrame.origin.y,CountryldEditBarFrame.size.width,CountryldEditBarFrame.size.height);
        self.bottomView.frame=CGRectMake(CountryoldBottomFrame.origin.x,self.view.frame.size.height-keyH-CountryoldBottomFrame.size.height,CountryoldBottomFrame.size.width,CountryoldBottomFrame.size.height);
        
    }else{
        
        self.editBarView.frame=CGRectMake(CountryldEditBarFrame.origin.x,CountryldEditBarFrame.origin.y,CountryldEditBarFrame.size.width,CountryldEditBarFrame.size.height);
        self.bottomView.frame=CGRectMake(CountryoldBottomFrame.origin.x,self.view.frame.size.height-CountryoldBottomFrame.size.height,CountryoldBottomFrame.size.width,CountryoldBottomFrame.size.height);
    }
    int h;
    if (self.editBarView.speakerChangeButton.selected) {
        h=HeadH;
    }else{
        h=0;
    }
    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height-keyH-h) ;
}
-(void)SetAllianceAny
{
    CountryldEditBarFrame=self.editBarView.frame;
    CountryoldBottomFrame=self.bottomView.frame;
    
    if (keyH>0) {
        
        self.editBarView.frame=CGRectMake(AllianceldEditBarFrame.origin.x,AllianceldEditBarFrame.origin.y,AllianceldEditBarFrame.size.width,AllianceldEditBarFrame.size.height);
        self.bottomView.frame=CGRectMake(AllianceoldBottomFrame.origin.x,self.view.frame.size.height-keyH-AllianceoldBottomFrame.size.height,AllianceoldBottomFrame.size.width,AllianceoldBottomFrame.size.height);
        
    }else{
        
        self.editBarView.frame=CGRectMake(AllianceldEditBarFrame.origin.x,AllianceldEditBarFrame.origin.y,AllianceldEditBarFrame.size.width,AllianceldEditBarFrame.size.height);
        self.bottomView.frame=CGRectMake(AllianceoldBottomFrame.origin.x,self.view.frame.size.height-AllianceoldBottomFrame.size.height,AllianceoldBottomFrame.size.width,AllianceoldBottomFrame.size.height);
    }
    

    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height-keyH) ;

}
#pragma mark -
#pragma mark EditBarView Delegate
-(void)editBarViewBtnPressedAction:(EditBarView *)sender{
    [self sendMsg:sender.inputTextField.text];
    //输入框置空
    sender.inputTextField.text = @"";
    [self.editBarView textViewDidChange:self.editBarView.inputTextField];
    
    
    
    
    /**
     *  输入框逻辑 需要补充
     */
    sender.senderButton.enabled = NO;
}

-(void)editBarViewWithInputTextFieldChanged:(EditBarView *)sender{
    DVLog(@"%@",sender.inputTextField.text);
    
    if (sender.inputTextField.text.length > 0) {
        
        int ascll = [sender.inputTextField.text characterAtIndex:0];
        //汉字
        if( ascll > 0x4e00 && ascll < 0x9fff)
        {
            sender.senderButton.enabled = YES;
        }
        //应为
        else
        {
            sender.senderButton.enabled = YES;
        }
   
    }else{
        //没有输入
        sender.senderButton.enabled = NO;
    }
}
-(void)editBarViewSpeakAllButtonAction:(EditBarView *)vSender{
 
    if (vSender.speakerChangeButton.selected) {
        
        self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.chatScrollView.frame.size.height-HeadH) ;
        self.countryTableView.frame=CGRectMake(self.countryTableView.origin.x, self.countryTableView.origin.y ,self.countryTableView.frame.size.width, self.countryTableView.frame.size.height-HeadH);
        self.allianceTableView.frame=CGRectMake(self.allianceTableView.origin.x, self.allianceTableView.origin.y ,self.allianceTableView.frame.size.width, self.allianceTableView.frame.size.height-HeadH);
        
        [self.countryTableView setContentOffset:CGPointMake(self.countryTableView.contentOffset.x, self.countryTableView.contentOffset.y+HeadH) animated:NO];
         [self.allianceTableView setContentOffset:CGPointMake(self.allianceTableView.contentOffset.x, self.allianceTableView.contentOffset.y+HeadH) animated:NO];
        
    }else{
        
        self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.chatScrollView.frame.size.height+HeadH) ;
        self.countryTableView.frame=CGRectMake(self.countryTableView.origin.x, self.countryTableView.origin.y ,self.countryTableView.frame.size.width, self.countryTableView.frame.size.height+HeadH);
        self.allianceTableView.frame=CGRectMake(self.allianceTableView.origin.x, self.allianceTableView.origin.y ,self.allianceTableView.frame.size.width, self.allianceTableView.frame.size.height+HeadH);

    }
   
    
    //[self bottomBarFrameChangeWithEditBarShowSpeakAll:YES];
}

#pragma mark - UIKeyboardNotification

- (void)keyboardWillChangeFrame:(NSNotification *)notification
{
    NSDictionary *userInfo = notification.userInfo;
    CGRect endFrame = [userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGRect beginFrame = [userInfo[UIKeyboardFrameBeginUserInfoKey] CGRectValue];
    CGFloat duration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    UIViewAnimationCurve curve =  (UIViewAnimationCurve)[userInfo[UIKeyboardAnimationCurveUserInfoKey] longValue];
    
    void(^animations)() = ^{
        [self willShowKeyboardFromFrame:beginFrame toFrame:endFrame];
    };
    
    void(^completion)(BOOL) = ^(BOOL finished){
        if (self.chatVCShowTableType == ChatVCShowTableType_left) {
            if (self.countryDataSourceArray.count> 1){
                [self.countryTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.countryDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
            }
        }else{
            if (self.allianceDataSourceArray.count> 1){
                [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
            }

        }
  
        
    };
    
    [UIView animateWithDuration:duration delay:0.0f options:(curve << 16 | UIViewAnimationOptionBeginFromCurrentState) animations:animations completion:completion];
}

- (void)willShowKeyboardFromFrame:(CGRect)beginFrame toFrame:(CGRect)toFrame
{
    
    keyH=self.view.frame.size.height-toFrame.origin.y;
    
    self.bottomView.bottom = toFrame.origin.y;
    

    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
        if (self.editBarView.speakerChangeButton.selected) {
            self.chatScrollView.height =self.bottomView.top-self.buttonView.height-self.topHeadView.height-HeadH;
        }else{
            self.chatScrollView.height =self.bottomView.top-self.buttonView.height-self.topHeadView.height;
        
        }
    }else{
        self.chatScrollView.height =self.bottomView.top-self.buttonView.height-self.topHeadView.height;
    }
    
    
    self.countryTableView.height =  self.chatScrollView.height;
    self.allianceTableView.height =  self.chatScrollView.height ;
    [self.view bringSubviewToFront:self.bottomView];

   
}

#pragma mark -
#pragma mark tableView delegate


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    if (tableView ==self.countryTableView ) {
         return self.countryDataSourceArray.count;
    }else{
         return self.allianceDataSourceArray.count;
    }
   
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
 
    if (tableView == self.countryTableView) {
        
          ChatCellFrame * cellFrame =[self.countryDataSourceArray objectAtIndex:indexPath.row];
            DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
         ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:countryIdentify];
        if (cell == nil) {
            cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:countryIdentify];
        }
        cell.backgroundColor=[UIColor clearColor];
          cell.selectionStyle = UITableViewCellSelectionStyleNone;
        [cell setCellFrame:cellFrame];
        return cell;
    }else{
       
          ChatCellFrame * cellFrame =[self.allianceDataSourceArray objectAtIndex:indexPath.row];
            DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
         ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:allianceIdentify];
        if (cell == nil) {
            cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:allianceIdentify];
        }
        cell.backgroundColor=[UIColor clearColor];
          cell.selectionStyle = UITableViewCellSelectionStyleNone;
        [cell setCellFrame:cellFrame];
        return cell;
    }

}



-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    

    if  (tableView==self.countryTableView){
        
        CGFloat totalHeight = 0;
        ChatCellFrame *cellFrame =[self.countryDataSourceArray objectAtIndex:indexPath.row];
        if (cellFrame.chatMessage.post == 100) {
            totalHeight = cellFrame.chatSystemSize.height+10 ;
        }else{
            if (cellFrame.isShowUiTopView_BOOL) {
                totalHeight =cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
            }else{
                totalHeight = cellFrame.cellHeight;
            }
            
           
        }
         return totalHeight ;
 
    }else{
        CGFloat totalHeight = 0;
        ChatCellFrame *cellFrame =[self.allianceDataSourceArray objectAtIndex:indexPath.row];
        if (cellFrame.chatMessage.post == 100) {
            totalHeight = cellFrame.chatSystemSize.height+10 ;
        }else{
            if (cellFrame.isShowUiTopView_BOOL) {
                totalHeight =cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
            }else{
                totalHeight = cellFrame.cellHeight;
            }
            
            
        }
        return totalHeight ;
        
    }
 
}

#pragma mark -
#pragma mark UIScrollView 代理方法

/**  当scrollView正在滚动就会调用 */
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (scrollView == self.chatScrollView) {
        
    }else{
//        if (scrollView.contentOffset.y < _oldOffset) {
//            
//            [self keyBoardClosed];
//            
//            
//        }
//        
//        _oldOffset = scrollView.contentOffset.y;
    }
}

/** 开始拖拽的时候调用 */
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    if (scrollView == self.chatScrollView) {
        
    }else{
        
    }
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    
    if (scrollView == self.chatScrollView) {
        //左右拖踹聊天表
        if (scrollView.contentOffset.x < 10) {
            [self.topButtonView settingLeftButtonSelected:YES];
            self.chatVCShowTableType = ChatVCShowTableType_left;
            [self SetCountryAny];
        } else {
            [self.topButtonView settingRightButtonSelected: YES];
            self.chatVCShowTableType = ChatVCShowTableType_right;
            [self SetAllianceAny];
        }
        self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.chatScrollView.frame.size.height);
        self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.chatScrollView.frame.size.height);

    }else if([scrollView isKindOfClass:[UITableView class]]){
        if (self.isCountryHasNewMsgPushed) {
             [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_COUNTRY];
        }
        if (self.isCountryHasLoadOldMsgPushed) {
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_COUNTRY];
        }
        
        if (self.isAllianceHasNewMsgPushed){
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_ALLIANCE];
        }
        if (self.isAllianceHasLoadOldMsgPushed) {
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_ALLIANCE];
        }
    }

}

/**  停止拖拽的时候调用 */
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    if (scrollView == self.chatScrollView) {
        
    }else{
        
    }
}

#pragma mark -
#pragma mark 下拉刷新
-(BOOL)beginFreshedWithTableView:(UITableView *)vTableView {
    if (vTableView == self.countryTableView) {
        return  [self.countryChannel gettingOldMsg];
    }else{
        return [self.allianceChannel gettingOldMsg];
    }
 
}
-(void)endRefreshedWithTableView:(UITableView *)vTableView{
    if (vTableView == self.countryTableView) {
         [_countryTableView.header endRefreshing];
    }else{
         [_allianceTableView.header endRefreshing];
    }
}
#pragma mark -
#pragma mark ChatChannel Delegate

-(void)showNewMsg:(ChatChannel *)vChannle{
    if (vChannle.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if ( self.countryTableView.dragging== NO && self.countryTableView.decelerating == NO) {
            [self reloadNewMsgWithChannelType:IOS_CHANNEL_TYPE_COUNTRY];
            self.countryHasNewMsgPushed = NO;
        }else{
            self.countryHasNewMsgPushed = YES;
        }
    }else{
        if ( self.allianceTableView.dragging== NO && self.allianceTableView.decelerating == NO) {
            [self reloadNewMsgWithChannelType:IOS_CHANNEL_TYPE_ALLIANCE];
            self.allianceHasNewMsgPushed = NO;
        }else{
            self.allianceHasNewMsgPushed = YES;
        }
    }
    

    
 
}
-(void)showOldMsg:(ChatChannel *)vChannel{
    if (vChannel.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
//        [self endRefreshedWithTableView:self.countryTableView];
        if (self.countryTableView.dragging == NO  && self.countryTableView.decelerating == NO){
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_COUNTRY];
            self.countryHasLoadOldMsgPushed = NO;
        }
        else{
             self.countryHasLoadOldMsgPushed = YES;
        }
    }else{
//        [self endRefreshedWithTableView:self.allianceTableView];
        if (self.allianceTableView.dragging == NO  && self.allianceTableView.decelerating == NO){
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_ALLIANCE];
            self.allianceHasLoadOldMsgPushed = NO;
        }
        else{
            self.allianceHasLoadOldMsgPushed = YES;
        }
    }
    
    
}

-(void)showscreenLockedMsg:(ChatChannel *)vChannel{
    if (vChannel.channelType == IOS_CHANNEL_TYPE_COUNTRY) {
        
        if (self.countryTableView.dragging == NO  && self.countryTableView.decelerating == NO){
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_COUNTRY];
            self.countryHasLoadOldMsgPushed = NO;
        }
        else{
            self.countryHasLoadOldMsgPushed = YES;
        }
    }else{
     
        if (self.allianceTableView.dragging == NO  && self.allianceTableView.decelerating == NO){
            [self reloadOldMsgWithChannelType:IOS_CHANNEL_TYPE_ALLIANCE];
            self.allianceHasLoadOldMsgPushed = NO;
        }
        else{
            self.allianceHasLoadOldMsgPushed = YES;
        }
    }
}
#pragma mark -
#pragma mark 滚动停止后，来消息处理
-(void)reloadNewMsgWithChannelType:(int )vChannelType{
    dispatch_async(dispatch_get_main_queue(), ^{
        //更新UI操作
        if (vChannelType == IOS_CHANNEL_TYPE_COUNTRY) {
            NSIndexPath *tempIndexPath = [self.countryTableView.indexPathsForVisibleRows lastObject];
            
            int oldCount = self.countryDataSourceArray.count;
            self.countryDataSourceArray = [self.countryChannel.msgList mutableCopy];
            int indexCount = self.countryDataSourceArray.count - oldCount;
            [self.countryTableView reloadData];
            if (self.countryDataSourceArray.count > 0){
                if (oldCount == 0) {
                    [self.countryTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.countryDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                }else{
                    if  (indexCount > 0){
                        if  (tempIndexPath.row > oldCount-4){
                            [self.countryTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.countryDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                        }
                    }
                }
            }
        }else{
            NSIndexPath *tempIndexPath = [self.allianceTableView.indexPathsForVisibleRows lastObject];
            
            int oldCount = self.allianceDataSourceArray.count;
            self.allianceDataSourceArray = [self.allianceChannel.msgList mutableCopy];
            int indexCount = self.countryDataSourceArray.count - oldCount;
            [self.allianceTableView reloadData];
            if (self.allianceDataSourceArray.count > 0){
                if (oldCount == 0) {
                    [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                }else{
                    if  (indexCount > 0){
                        if  (tempIndexPath.row > oldCount-4){
                            [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                        }
                    }
                }
            }
        }
        
        
        });
    if (vChannelType == IOS_CHANNEL_TYPE_COUNTRY) {
        self.countryHasNewMsgPushed = NO;
    }else{
        self.allianceHasNewMsgPushed = NO;
    }
    
}
-(void)reloadOldMsgWithChannelType:(int )vChannelType{
    
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        if (vChannelType == IOS_CHANNEL_TYPE_COUNTRY) {
            NSArray *tempIndexPathArray =[self.countryTableView indexPathsForVisibleRows];
            NSIndexPath *tempIndexPath = nil;
            if (tempIndexPathArray.count> 0) {
               tempIndexPath=  [tempIndexPathArray objectAtIndex:0];
            }
            //更新UI操作
            int oldCount = self.countryDataSourceArray.count;
            self.countryDataSourceArray = [self.countryChannel.msgList mutableCopy];
            int indexCount = self.countryDataSourceArray.count - oldCount;
            [self endRefreshedWithTableView:self.countryTableView];
            [self.countryTableView reloadData];
            if (tempIndexPath.row<= 3) {
                CGPoint pt =self.countryTableView.contentOffset;
                CGFloat boundaryValueMax= self.countryTableView.contentSize.height - self.countryTableView.frame.size.height * 0.6 - self.countryTableView.frame.size.height;
                if(indexCount > 0   ){
                    [self.countryTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount-1 inSection:0] atScrollPosition: UITableViewScrollPositionTop animated:NO];
                }
            }
            
            
        }else{
            NSArray *tempIndexPathArray =[self.allianceTableView indexPathsForVisibleRows];
            NSIndexPath *tempIndexPath = nil;
            if (tempIndexPathArray.count> 0) {
                tempIndexPath=  [tempIndexPathArray objectAtIndex:0];
            }
            //更新UI操作
            int oldCount = self.allianceDataSourceArray.count;
            self.allianceDataSourceArray = [self.allianceChannel.msgList mutableCopy];
            int indexCount = self.allianceDataSourceArray.count - oldCount;
            [self endRefreshedWithTableView:self.allianceTableView];
            [self.allianceTableView reloadData];
            if (tempIndexPath.row<= 3) {
                CGPoint pt =self.allianceTableView.contentOffset;
                CGFloat boundaryValueMax= self.allianceTableView.contentSize.height - self.allianceTableView.frame.size.height * 0.6 - self.allianceTableView.frame.size.height;
                if(indexCount > 0   ){
                    [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount-1 inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
                }
            }
        }
        

        
    });
    
    
}

#pragma mark -
#pragma mark 加入联盟视图

-(UIView *) addJoinAllance
{
    UIView *  jionAllanceView = [[UIView alloc] init];
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    CGFloat x = 0;
    CGFloat y = winSize.height * 0.14;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.86;
     jionAllanceView.frame = CGRectMake(self.chatScrollView.width, 0, self.chatScrollView.width, self.chatScrollView.height);
    
    UIImageView *jionAllanceBack = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    jionAllanceBack.frame =  jionAllanceView.bounds;
    
    UILabel *tipJionAllanceLabel = [[UILabel alloc]init];
    tipJionAllanceLabel.lineBreakMode = NSLineBreakByWordWrapping;
    tipJionAllanceLabel.numberOfLines = 0;
    tipJionAllanceLabel.backgroundColor = [UIColor clearColor];
    CGFloat fontSize = [ServiceInterface serviceInterfaceSharedManager].fontSize;
    tipJionAllanceLabel.font = [UIFont systemFontOfSize:fontSize];
    tipJionAllanceLabel.text = [NSString stringWithMultilingualWithKey:@"E100068"];//E100068=您未加入联盟，暂时无法使用联盟聊天频道
    CGFloat tipJionAllanceLabel_x =  jionAllanceView.frame.size.width / 4;
    CGFloat tipJionAllanceLabel_y =  jionAllanceView.frame.size.height / 5;
    CGFloat tipJionAllanceLabel_width = self.view.frame.size.width * 0.6;
    CGFloat tipJionAllanceLabel_height =  jionAllanceView.frame.size.height * 0.2;
    tipJionAllanceLabel.frame = CGRectMake(tipJionAllanceLabel_x, tipJionAllanceLabel_y, tipJionAllanceLabel_width, tipJionAllanceLabel_height);
    
    UIButton *jionAllanceBtn = [[UIButton alloc]init];
    [jionAllanceBtn setBackgroundImage:[UIImage imageNamed:@"chat_bottom_smile_nor.png"] forState:UIControlStateNormal];
    NSString *jionStr = [NSString stringWithMultilingualWithKey:@"115020"];//115020=//加入
    [jionAllanceBtn setTitle:jionStr forState:UIControlStateNormal];
    [jionAllanceBtn setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
    
    [jionAllanceBtn addTarget:self action:@selector(jionFun:) forControlEvents:UIControlEventTouchUpInside];
    
    CGFloat jionAllanceBtn_x =  jionAllanceView.frame.size.width * 0.3;
    CGFloat jionAllanceBtn_y =  jionAllanceView.frame.size.height * 0.5;
    CGFloat jionAllanceBtn_width = self.view.frame.size.width * 0.45;
    CGFloat jionAllanceBtn_height = jionAllanceView.frame.size.height * 0.1;
    
    jionAllanceBtn.frame = CGRectMake(jionAllanceBtn_x, jionAllanceBtn_y, jionAllanceBtn_width, jionAllanceBtn_height);
    
    [ jionAllanceView addSubview:jionAllanceBack];
    [ jionAllanceView addSubview:tipJionAllanceLabel];
    [ jionAllanceView addSubview:jionAllanceBtn];
    return jionAllanceView;
    
}

-(void)jionFun:(UIButton *)vSender{
    
}


@end
