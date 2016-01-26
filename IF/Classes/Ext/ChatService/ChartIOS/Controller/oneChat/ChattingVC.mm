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
#import "NSMsgItem.h"
#import "CSAlertView.h"
#import "UserManager.h"
#import "NSString+Cocos2dHelper.h"
#import "CSConvertViewManager.h"
#import "TipPopUpController.h"
#import "CSAlertRedPackageView.h"
#import "CSRedEnvelopeView.h"
#import "ChatServiceCocos2dx.h"


#import "CSMessageModel.h"
#import "CSMessage.h"
#import "CSMessage+Manager.h"
#import "CSMessageTableCell.h"
#import "CSChatMenuItemFactory.h"
#import "QBPopupMenu.h"
#import "CSGoogleTranslat.h"
#import "ChatChannel+ChatMessage.h"
#import "BanPopUpController.h"
@interface ChattingVC ()<UIScrollViewDelegate,UITableViewDataSource,UITableViewDelegate,TopHeadViewDelegate,EditBarViewDelegate,ChatChannelDelegate,TopButtonViewDelegate,ChatCellIOSDelegate,UIScrollViewDelegate,TipPopUpControllerDelegate,ZYTAlertViewDelegate,CSAlertRedPackageViewDelegate,CSRedEnvelopeViewDelegate,CSMsgTableCellDelegate>{

            UILongPressGestureRecognizer *_lpgr;
//    UIMenuItem *_copyMenuItem;
//    UIMenuItem *_deleteMenuItem;
    
    
    CGRect CountryoldBottomFrame;
    CGRect CountryldEditBarFrame;
    
    CGRect AllianceoldBottomFrame;
    CGRect AllianceldEditBarFrame;
    
    CGFloat keyH;
    
    CGFloat editBarH;
    int HeadH;
    UIView *joinView;
    BOOL  isAddJoinAllance;
    BOOL _isGoIntoServer_Dragon_Battle;
}


@property (strong,nonatomic) UIView * headView;
@property (nonatomic,strong) UIView  * buttonView;
@property (strong,nonatomic) UIView * bottomView;
@property (strong,nonatomic) TopHeadView *topHeadView;
@property (strong,nonatomic) TopButtonView *topButtonView;
@property (nonatomic,strong) EditBarView * editBarView;
@property (strong,nonatomic)  UIScrollView *chatScrollView;
@property (nonatomic,strong) ChatTableView * countryTableView;
@property (nonatomic,strong) ChatTableView * allianceTableView;

@property (nonatomic,strong) NSMutableArray * countryDataSourceArray;
@property (nonatomic,strong) NSMutableArray * allianceDataSourceArray;
/** 当前长恩选中的cell indexpath */
@property (strong, nonatomic) NSIndexPath *menuIndexPath;

@property (strong, nonatomic) UIMenuController *menuController;


@property (nonatomic,assign,getter=isCountryHasNewMsgPushed)BOOL countryHasNewMsgPushed;
@property (nonatomic,assign,getter=isCountryHasLoadOldMsgPushed)BOOL countryHasLoadOldMsgPushed;

@property (nonatomic,assign,getter=isAllianceHasNewMsgPushed)BOOL allianceHasNewMsgPushed;
@property (nonatomic,assign,getter=isAllianceHasLoadOldMsgPushed)BOOL allianceHasLoadOldMsgPushed;

@property (nonatomic,assign,getter=isCountryRefreshingTure)BOOL countryRefreshing;
@property (nonatomic,assign,getter=isAllianceRefreshingTure)BOOL allianceRefreshing;
/**
 *  喇叭弹出提示框
 */
@property (nonatomic,strong) TipPopUpController *tipPopUpController;
@property (nonatomic,strong)BanPopUpController *banpopUpOController;
/**
 *  红包视图
 */
@property (nonatomic,strong) CSAlertRedPackageView *alertRedPackageView;

@end
 
static NSString *countryIdentify = @"countryIdentify";
 static NSString *allianceIdentify = @"allianceIdentify";
@implementation ChattingVC

-(void)viewWillAppear:(BOOL)animated{
    
}

-(void)viewWillDisappear:(BOOL)animated{
    
}

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
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notifCloseKeyBoard:) name:KCLOSEKEYBOARD object:nil];
    
    //顶部ButtonView
    TopButtonView *topButtonView =[TopButtonView topButtonViewWithLeftButtonName:[NSString stringWithMultilingualWithKey:@"105300"] andWithRightButtonName:[NSString stringWithMultilingualWithKey:@"105602"]];//105602 = 联盟  105300=国家
    topButtonView.frame = CGRectMake(0, 0, self.buttonView.width, self.buttonView.height );
//    [topButtonView settingLeftButtonSelected:YES];
    topButtonView.topButtonViewDelegate = self;
    self.topButtonView = topButtonView;
    [self.buttonView addSubview: self.topButtonView];
    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
         [self.topButtonView settingLeftButtonSelected:YES];
       // self.chatVCShowTableType = ChatVCShowTableType_left;

         self.editBarView.isCountry=YES;
    }else{
         [self.topButtonView settingRightButtonSelected:YES];
         self.chatVCShowTableType = ChatVCShowTableType_right;
        self.editBarView.isCountry=NO;
        if  (ChatServiceCocos2dx::CS_AllianceTask_Helped   ){
            //联盟宝藏帮助
            NSString *dialgo = [NSString stringWithMultilingualWithKey:@"134073"];//134073		我发现了新的宝藏，希望得到大家的帮助，谢谢
            self.editBarView.oldAllianceText = dialgo;
            self.editBarView.inputTextField.text = dialgo;
            //               [self.editBarView updateEditView];
            //            self.editBarView.inputTextField.text = dialgo;
            ChatServiceCocos2dx::CS_AllianceTask_Helped = false;
             [self.editBarView textViewDidChange:self.editBarView.inputTextField];
        }
    }
    
    
//不能在国家说话
    if (isAddJoinAllance==NO) {
        joinView =[self addJoinAllance];
         [self.chatScrollView addSubview:joinView];
    }else{
        if (joinView) {
            [joinView removeFromSuperview];
        }
    }
   
 
    
    if (_isGoIntoServer_Dragon_Battle==YES) {
         [self.chatScrollView addSubview:[self NoChatInCountryView]];
    }else{
        
    }
   
}
-(void)deallocAny
{
    if ([self.editBarView.inputTextField isFirstResponder]){
        [self.editBarView.inputTextField resignFirstResponder];
    }
    [self.countryChannel msgArrayFormatToStartState];
    [self.allianceChannel msgArrayFormatToStartState];
    self.countryTableView.delegate=nil;
    self.countryTableView.dataSource=nil;
    self.allianceTableView.delegate=nil;
    self.allianceTableView.dataSource=nil;
    
    self.countryChannel.channelDelegate =nil;
    self.countryChannel = nil;
    self.allianceChannel.channelDelegate = nil;
    self.allianceChannel = nil;
}
-(void)dealloc{
    self.countryTableView.delegate=nil;
    self.countryTableView.dataSource=nil;
    self.allianceTableView.delegate=nil;
    self.allianceTableView.dataSource=nil;
    self.countryChannel.channelDelegate =nil;
    self.countryChannel = nil;
    self.allianceChannel.channelDelegate = nil;
    self.allianceChannel = nil;
    self.countryDataSourceArray = nil;
    self.allianceDataSourceArray = nil;
    self.topButtonView = nil;
    self.topHeadView = nil;
    self.editBarView = nil;
    self.chatScrollView = nil;
    self.headView = nil;
    self.buttonView = nil;
    self.bottomView = nil;
 
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillChangeFrameNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:KCLOSEKEYBOARD object:nil];
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
    self.allianceTableView.frame=CGRectMake(kMainScreenWidth, 0, self.allianceTableView.frame.size.width, self.allianceTableView.frame.size.height-changHeight);
    
    
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
        scrollerView.backgroundColor=[UIColor clearColor];
        //scrollerView.clipsToBounds=NO;
        //水平滚动条
        scrollerView.showsHorizontalScrollIndicator =NO;
        _chatScrollView = scrollerView;
      
        //_chatScrollView.backgroundColor = [UIColor clearColor];
    }
    return _chatScrollView;
}

-(ChatTableView *)countryTableView{
    if (_countryTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, self.chatScrollView.height) style:UITableViewStylePlain];
        UIImageView *backImageView =[[UIImageView alloc]init];
        backImageView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"csChatTableBackImage1"]];//csChatTableBackImage1
        tableView.backgroundView =backImageView;
        tableView.separatorStyle=UITableViewCellSeparatorStyleNone;

        
      
        if ( ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            _lpgr = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
            _lpgr.minimumPressDuration = 0.5;
            [tableView addGestureRecognizer:_lpgr];
        }
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
              UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
            [tableView addGestureRecognizer:tap];
        }
        _countryTableView = tableView;
        _countryTableView.dataSource = self;
        _countryTableView.delegate = self;
    }
    return _countryTableView;
}
-(ChatTableView *)allianceTableView{
    if (_allianceTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(self.chatScrollView.width, 0, self.chatScrollView.width, self.chatScrollView.height) style:UITableViewStylePlain];
        UIImageView *backImageView =[[UIImageView alloc]init];
        backImageView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"csChatTableBackImage1"]];//csChatTableBackImage1
        tableView.backgroundView =backImageView;
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            _lpgr = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
            _lpgr.minimumPressDuration = 0.5;
            [tableView addGestureRecognizer:_lpgr];

        }
             if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
                 [tableView addGestureRecognizer:tap];
             }
         
       
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
        if  ([UserManager sharedUserManager].currentUser.allianceId.length> 0){
            isAddJoinAllance = YES;
//            if (joinView) {
//                [joinView removeFromSuperview];
//            }
        }else{
            isAddJoinAllance = NO;
        }
    
   
    
    
    if  (ChatServiceCocos2dx::isGoIntoServer_Dragon_Battle() == true){
        _isGoIntoServer_Dragon_Battle=YES;
    }else{
        _isGoIntoServer_Dragon_Battle=NO;
    }
    
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    if (_chatVCShowTableType ==ChatVCShowTableType_left) {
        
        //设置C++ 的聊天条光点
        
        [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_COUNTRY];
        ChatServiceCocos2dx::refreshChatInfo(IOS_CHANNEL_TYPE_COUNTRY);
        
        int lever = ChatServiceCocos2dx::CS_Country_SpeakingLevelLimit;
        //user等级大于lever返回yes
        if  ([NSString isSelfUserLeverLimitToServerLeverSetter:lever]){
            self.editBarView.inputTextField.editable = YES;
//            NSString *dialogString =  @" ";
//            NSString *dialog  = [NSString stringWithMultilingualWithKey:@"132135" andWithPaddingString1:[NSString stringWithFormat:@"%d",lever]]; //132135:需要等级：{0}
//            dialogString = [dialogString stringByAppendingString:dialog];
//            self.editBarView.countryPlaceStr= dialogString;//@" 测试打印，之后去掉";
            
        }else{
            self.editBarView.inputTextField.editable = NO;
            NSString *dialogString =  @" ";
            NSString *dialog  = [NSString stringWithMultilingualWithKey:@"132135" andWithPaddingString1:[NSString stringWithFormat:@"%d",lever]]; //132135:需要等级：{0}
            dialogString = [dialogString stringByAppendingString:dialog];
             self.editBarView.countryPlaceStr = dialogString;
         
        }
        
        [self.editBarView passchatVCShowTableType:YES];
        CGPoint point = CGPointMake(0, 0);
        [self.chatScrollView setContentOffset:point animated:YES];
        _editBarView.speakerButtonShowing = YES;
        if (isAddJoinAllance==NO) {
           //开
            self.bottomView.hidden=NO;
            self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, winSize.width, winSize.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height) ;
            self.countryTableView.frame=CGRectMake(0, 0, winSize.width, self.chatScrollView.frame.size.height);
        }else{
            
        }
       
        if (_isGoIntoServer_Dragon_Battle== YES) {
            
            self.bottomView.hidden=YES;
            [self.editBarView.inputTextField resignFirstResponder];
             self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, winSize.width, winSize.height) ;
            
        }else{
            
        }
        
        [self.topButtonView settingLeftButtonSelected:YES];
     //[self bottomBarFrameChangeWithEditBarShowSpeakAll:YES];
    }else{
        

        
        [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_ALLIANCE];
       ChatServiceCocos2dx::refreshChatInfo(IOS_CHANNEL_TYPE_ALLIANCE);

        
        [self.editBarView passchatVCShowTableType:NO];
        _editBarView.speakerButtonShowing = NO;
        
        if (isAddJoinAllance==NO) {
           
            self.bottomView.hidden=YES;
            [self.editBarView.inputTextField resignFirstResponder];
            self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, kMainScreenWidth, winSize.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height) ;
            self.countryTableView.frame=CGRectMake(0, 0, kMainScreenWidth, self.chatScrollView.frame.size.height);
           

        }else{
            self.bottomView.hidden=NO;
        }
        
        
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

-(void)tableVIewScrollerTOBottom{
    
    
    if(self.chatVCShowTableType == ChatVCShowTableType_left){
        if (self.countryDataSourceArray.count> 0){
            NSIndexPath *tempIndexPath = [NSIndexPath indexPathForRow:(self.countryDataSourceArray.count -1) inSection:0];
            [self.countryTableView scrollToRowAtIndexPath:  tempIndexPath atScrollPosition:UITableViewScrollPositionBottom animated:YES];
        }
    }else{
        if (self.allianceDataSourceArray.count> 0){
            NSIndexPath *tempIndexPath = [NSIndexPath indexPathForRow:(self.allianceDataSourceArray.count -1) inSection:0];
            [self.allianceTableView scrollToRowAtIndexPath:  tempIndexPath atScrollPosition:UITableViewScrollPositionBottom animated:YES];
        }
    }
}

-(void)settingHeadRefeashWithTableView:(UITableView *)vTableView{
    NSString *headTitleString =[NSString stringWithMultilingualWithKey:@"105327"];//105327=下拉可加载更多
    __weak ChattingVC *tempVC =self;
    MJRefreshNormalHeader *header= [MJRefreshNormalHeader headerWithRefreshingBlock:^{
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.05 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
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
    
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        CSMessageBodyType post = CSMessageBodyType_Text_Normal;
        CSChannelType channelType = CSChannelType_Country;
       
        
        if  (self.chatVCShowTableType == ChatVCShowTableType_left){
            channelType = CSChannelType_Country;
            /** 判断当前用户符合发消息的条件，不符合，弹出提示  */
            if (![self isSendChat]) {
                return ;
            }
             CSMessage *msg = [CSMessage messageWithSelfSendString:vMsgString andWithChannelType: channelType andWithMessageBodyType: post];
            if  (self.editBarView.speakerChangeButton.selected ){
                //喇叭消息
                post = CSMessageBodyType_Text_Loudspeaker;
                msg.post = post;

            }
            msg.sendState = CSMessageSendState_Failure;
            //消息插库
            [self.countryChannel savingMessageForSelfSendToServerWithMessage:msg];
            msg.sendState = CSMessageSendState_Delivering;
            if  (post == CSMessageBodyType_Text_Loudspeaker){
                [self sendRadioWithMessage:msg];
            }else{
                [self.countryChannel chatNewMsgPushed:@[msg]];
                [[ChatServiceController chatServiceControllerSharedManager].gameHost sendTextWithChatMessage:msg andWithSendMsgType:0];

            }
            
        }else{
            channelType = CSChannelType_Alliance;
            CSMessage *msg = [CSMessage messageWithSelfSendString:vMsgString andWithChannelType: channelType andWithMessageBodyType: post];
            msg.sendState = CSMessageSendState_Failure;
            //消息插库
            [self.allianceChannel savingMessageForSelfSendToServerWithMessage:msg];
            msg.sendState = CSMessageSendState_Delivering;
             [self.allianceChannel chatNewMsgPushed:@[msg]];
             [[ChatServiceController chatServiceControllerSharedManager].gameHost sendTextWithChatMessage:msg andWithSendMsgType:2];
        }
        
        
    }else{
        NSMsgItem *msgItem = [self msgItemWithSendString:vMsgString];
        //自己发的消息，先入库
        msgItem.sendState = 1;
        if (self.chatVCShowTableType == ChatVCShowTableType_left) {
            
            if (![self isSendChat]) {
                return ;
            }
            
            if (self.editBarView.speakerChangeButton.selected ) {
                //喇叭消息
                msgItem.post = 6;
            }
            [self.countryChannel savingMsgForSelfSendToServerWithMsgItem:msgItem];
            if (msgItem.post == 6){
                [self sendRadioByMsg:msgItem];
            }else{
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
    
//    self.countryChannel.channelDelegate = nil;
//    self.countryChannel = nil;
//    self.allianceChannel.channelDelegate = nil;
//    self.allianceChannel = nil;
    [self deallocAny];
    
    
        //新版OC原生邮件列表
        id nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
    if  ([[ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController isKindOfClass:[UINavigationController class]]){
        UINavigationController *  nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;

        if ([ nav.viewControllers objectAtIndex:0] == self){
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
            [[ServiceInterface serviceInterfaceSharedManager]hideMailListIOS];
            
        }else{
            [self.navigationController popViewControllerAnimated:YES];
        }
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
        [self removeAlertRedPackageView];
        [self.chatScrollView setContentOffset:point animated:YES];
        
        [self SetCountryAny];
        
        
        
//        int indexCount = self.countryDataSourceArray.count;
//        if (indexCount> 0){
//
//            [self.countryTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
//        }
        
    }else if(sender.tag == 102){
        //右侧按钮
        DVLog(@"右侧按钮");
        
         self.chatVCShowTableType = ChatVCShowTableType_right;
        [self removeAlertRedPackageView];
        CGPoint point = CGPointMake(self.chatScrollView.width, 0);
        [self.chatScrollView setContentOffset:point animated:YES];
       
        [self SetAllianceAny];
        
        
//        int indexCount = self.allianceDataSourceArray.count;
//        if(indexCount > 0   ){
//            [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
//        }
        
    }
    
    self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.chatScrollView.frame.size.height);
    self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.chatScrollView.frame.size.height);
 
    [self editBarViewWithInputTextFieldChanged:self.editBarView];
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
    if (_isGoIntoServer_Dragon_Battle==YES) {
        
    }else{
    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height-keyH-h) ;
    }
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
    
    if (isAddJoinAllance==NO) {
        
    }else{
    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height-keyH) ;
    }

}
#pragma mark -
#pragma mark EditBarView Delegate

-(void)editBarViewTextViewEditBegin:(EditBarView *)sender{
    //打开键盘时，列表滚动到底部
    [self tableVIewScrollerTOBottom];
}
-(void)editBarViewBtnPressedAction:(EditBarView *)sender{
    
   //    [self tableVIewScrollerTOBottom];
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
        
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            static  NSString *identify = @"coutrycell";
            
            CSMessageTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
            CSMessageModel *model =[_countryDataSourceArray objectAtIndex:indexPath.row];
            if (cell == nil) {
                cell =[[CSMessageTableCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify model:model];
                cell.msgTableCellDelegate = self;
                cell.selectionStyle = UITableViewCellSelectionStyleNone;
            }
            cell.messageModel = model;
            return cell;

        }else{
            ChatCellFrame * cellFrame =[self.countryDataSourceArray objectAtIndex:indexPath.row];
            DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
            ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:countryIdentify];
            if (cell == nil) {
                cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:countryIdentify];
            }
            cell.chatCellIosDelegate = self;
            cell.backgroundColor=[UIColor clearColor];
            cell.selectionStyle = UITableViewCellSelectionStyleNone;
            [cell setCellFrame:cellFrame];
            if (cellFrame.chatMessage.post == 12){
                cell.redEnvelopeView.redEnvelopeViewDelegate = self;
            }
            return cell;

        }
        
     }else{
         
         if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
             static  NSString *identify = @"alliancecell";
             
             CSMessageTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
             CSMessageModel *model =[_allianceDataSourceArray objectAtIndex:indexPath.row];
             if (cell == nil) {
                 cell =[[CSMessageTableCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify model:model];
                                 cell.msgTableCellDelegate = self;
                 cell.selectionStyle = UITableViewCellSelectionStyleNone;
             }
             cell.messageModel = model;
             return cell;
             
         }else{
             ChatCellFrame * cellFrame =[self.allianceDataSourceArray objectAtIndex:indexPath.row];
             DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
             ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:allianceIdentify];
             if (cell == nil) {
                 cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:allianceIdentify];
             }
             cell.chatCellIosDelegate = self;
             cell.backgroundColor=[UIColor clearColor];
             cell.selectionStyle = UITableViewCellSelectionStyleNone;
             [cell setCellFrame:cellFrame];
             if (cellFrame.chatMessage.post == 12){
                 cell.redEnvelopeView.redEnvelopeViewDelegate = self;
             }
 
                return cell;
         }
       
         
    }

}



-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    

    if  (tableView==self.countryTableView){
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            CGFloat height =[(CSMessageModel *)_countryDataSourceArray[indexPath.row] cellHeight];
            return height;

        }else{
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

        }
        
        
    }else{
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            CGFloat height =[(CSMessageModel *)_allianceDataSourceArray[indexPath.row] cellHeight];
            return height;
            
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
 
}
-(UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section{
    UIView *view =[[UIView alloc]initWithFrame:CGRectMake(0, 0, tableView.width ,tableView.height)];
    view.backgroundColor = [UIColor clearColor];
    return view;
}
- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    return 10;
}
#pragma mark -
#pragma mark UIScrollView 代理方法

/**  当scrollView正在滚动就会调用 */
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    [self.menuController setMenuVisible:NO];
    [self  resumeOldLongPress];
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
-(void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView
{
   
}
-(void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset
{
   
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    if (isAddJoinAllance==NO&&scrollView==self.chatScrollView) {
        if (_isGoIntoServer_Dragon_Battle==YES&&isAddJoinAllance==NO) {
            return;
        }
        if (targetContentOffset->x==winSize.width) {
                self.bottomView.hidden=YES;
            self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, kMainScreenWidth, winSize.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height) ;
            
            }else{
              
                self.bottomView.hidden=NO;
                self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height) ;
                self.countryTableView.frame=CGRectMake(0, 0, self.view.frame.size.width, self.chatScrollView.frame.size.height);
            }

        
    }
    
    if (_isGoIntoServer_Dragon_Battle==YES&&scrollView==self.chatScrollView) {
        if (_isGoIntoServer_Dragon_Battle==YES&&isAddJoinAllance==NO) {
            return;
        }
        if (targetContentOffset->x==0) {
            self.bottomView.hidden=YES;
            self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, kMainScreenWidth, winSize.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height) ;
            
        }else{
            
            self.bottomView.hidden=NO;
            self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.view.frame.size.height-self.topHeadView.frame.size.height-self.buttonView.frame.size.height-self.bottomView.frame.size.height) ;
            self.allianceTableView.frame=CGRectMake(kMainScreenWidth, 0, self.view.frame.size.width, self.chatScrollView.frame.size.height);
        }

        
        
    }
}
-(void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    if (scrollView == self.chatScrollView) {
        if (_isGoIntoServer_Dragon_Battle==YES&&isAddJoinAllance==NO) {
            return;
        }
        if (_isGoIntoServer_Dragon_Battle==YES) {
           
            if (self.chatVCShowTableType==ChatVCShowTableType_right&&scrollView.contentOffset.x==kMainScreenWidth) {
                self.bottomView.hidden=YES;
                [self.editBarView.inputTextField resignFirstResponder];
                self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, kMainScreenHeight-self.topHeadView.frame.size.height-self.buttonView.frame.size.height) ;
                self.allianceTableView.frame=CGRectMake(kMainScreenWidth, 0, self.view.frame.size.width, self.chatScrollView.frame.size.height);
            }
        }
        if (isAddJoinAllance==NO) {
            
            if (self.chatVCShowTableType==ChatVCShowTableType_left&&scrollView.contentOffset.x==0) {
                self.bottomView.hidden=YES;
                [self.editBarView.inputTextField resignFirstResponder];
                self.chatScrollView.frame= CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, kMainScreenHeight-self.topHeadView.frame.size.height-self.buttonView.frame.size.height) ;
                self.countryTableView.frame=CGRectMake(0, 0, self.view.frame.size.width, self.chatScrollView.frame.size.height);
            }
        }

    }else if([scrollView isKindOfClass:[UITableView class]]){
         [self keyBoardClosed];
    }
}
/** 开始拖拽的时候调用 */

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    
    if (scrollView == self.chatScrollView) {
        //左右拖踹聊天表
        if (scrollView.contentOffset.x < 10) {
            [self.topButtonView settingLeftButtonSelected:YES];
            if (self.chatVCShowTableType==ChatVCShowTableType_right) {
                self.chatVCShowTableType = ChatVCShowTableType_left;
                [self SetCountryAny];

//                //设置C++ 的聊天条光点
//                [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_COUNTRY];
//                ChatServiceCocos2dx::refreshChatInfo(IOS_CHANNEL_TYPE_COUNTRY);

            }
            
            
        } else {
            [self.topButtonView settingRightButtonSelected: YES];
            if (self.chatVCShowTableType==ChatVCShowTableType_left) {
                self.chatVCShowTableType = ChatVCShowTableType_right;
                [self SetAllianceAny];

//                [[ChatServiceController chatServiceControllerSharedManager].gameHost updateChatInfo:IOS_CHANNEL_TYPE_ALLIANCE];
//                ChatServiceCocos2dx::refreshChatInfo(IOS_CHANNEL_TYPE_ALLIANCE);

            }
            
        }
        
        
        self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.chatScrollView.frame.size.height);
        self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.chatScrollView.frame.size.height);
        
        [self editBarViewWithInputTextFieldChanged:self.editBarView];
        if  ([self.editBarView.inputTextField isFirstResponder]){
            [self tableVIewScrollerTOBottom];
        }
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
    
   
//    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
//        self.countryDataSourceArray = [self.countryChannel.msgList mutableCopy];
//        [self.countryTableView reloadData];
//
//    }else{
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
        

//    }
    

    
 
}
-(void)showOldMsg:(ChatChannel *)vChannel{
    
//    [self endRefreshedWithTableView:self.countryTableView];
//    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
//        self.countryDataSourceArray = [self.countryChannel.msgList mutableCopy];
//        [self.countryTableView reloadData];
//        
//    }else{
   
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
//    }
    
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
            int oldCount  = 0;
           oldCount = self.allianceDataSourceArray.count;
            self.allianceDataSourceArray = [self.allianceChannel.msgList mutableCopy];
            int indexCount = self.countryDataSourceArray.count - oldCount;
            [self.allianceTableView reloadData];
            if (self.allianceDataSourceArray.count > 0){
                if (tempIndexPath.row > oldCount-4){
                    [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                }
//                if (oldCount == 0) {
//                    [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
//                }else{
//                    if  (indexCount > 0){
//                        if  (tempIndexPath.row > oldCount-4){
//                            [self.allianceTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.allianceDataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
//                        }
//                    }
//                }
            }
        }
        
        
        });
    if (vChannelType == IOS_CHANNEL_TYPE_COUNTRY) {
        if (self.countryDataSourceArray.count>0) {
           
            if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                CSMessageModel *chatMsgModel = [self.countryDataSourceArray lastObject];
                if  ([chatMsgModel.message.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                      [self tableVIewScrollerTOBottom];
                }
            }else{
                 ChatCellFrame * cellFrame =[self.countryDataSourceArray lastObject];
                if ([cellFrame.chatMessage.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                    [self tableVIewScrollerTOBottom];
                }
            }
            
          
        }
       
        self.countryHasNewMsgPushed = NO;
    }else{
        if (self.allianceDataSourceArray.count>0) {
            if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                CSMessageModel *chatMsgModel = [self.allianceDataSourceArray lastObject];
                if  ([chatMsgModel.message.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                    [self tableVIewScrollerTOBottom];
                }
            }else{
                ChatCellFrame * cellFrame =[self.allianceDataSourceArray lastObject];
                if ([cellFrame.chatMessage.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                    [self tableVIewScrollerTOBottom];
                }
            }
            
            
           
         }
        
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
     jionAllanceView.frame = CGRectMake(self.chatScrollView.width, 0, self.chatScrollView.width, winSize.height);
    
    
    UIImageView *jionAllanceBack = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    jionAllanceBack.frame=CGRectMake(0, 0, jionAllanceView.frame.size.width, jionAllanceView.frame.size.height);
    [jionAllanceView addSubview:jionAllanceBack];
    
    UILabel *tipJionAllanceLabel = [[UILabel alloc]init];
    tipJionAllanceLabel.lineBreakMode = NSLineBreakByWordWrapping;
    tipJionAllanceLabel.numberOfLines = 0;
    tipJionAllanceLabel.backgroundColor = [UIColor clearColor];
    CGFloat fontSize ;
     if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
 
         fontSize=22;
 
     }else{
         fontSize=15;
     }
    
    tipJionAllanceLabel.font = [UIFont systemFontOfSize:fontSize];
    tipJionAllanceLabel.textAlignment=NSTextAlignmentCenter;
    tipJionAllanceLabel.text = [NSString stringWithMultilingualWithKey:@"E100068"];//E100068=您未加入联盟，暂时无法使用联盟聊天频道
    CGFloat tipJionAllanceLabel_x =  jionAllanceView.frame.size.width / 4;
    CGFloat tipJionAllanceLabel_y =  jionAllanceView.frame.size.height / 6;
    CGFloat tipJionAllanceLabel_width = self.view.frame.size.width * 0.6;
    CGFloat tipJionAllanceLabel_height =  jionAllanceView.frame.size.height * 0.2;
    tipJionAllanceLabel.frame = CGRectMake(0, tipJionAllanceLabel_y, jionAllanceBack.frame.size.width, tipJionAllanceLabel_height);
    
    [jionAllanceBack addSubview:tipJionAllanceLabel];

    
    UIButton *jionAllanceBtn = [[UIButton alloc]init];
    [jionAllanceBtn setBackgroundImage:[UIImage imageNamed:@"chat_bottom_smile_nor.png"] forState:UIControlStateNormal];
    NSString *jionStr = [NSString stringWithMultilingualWithKey:@"115020"];//115020=//加入
    [jionAllanceBtn setTitle:jionStr forState:UIControlStateNormal];
    [jionAllanceBtn setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
    
    [jionAllanceBtn addTarget:self action:@selector(jionFun:) forControlEvents:UIControlEventTouchUpInside];
    
    CGFloat jionAllanceBtn_x =  jionAllanceBack.frame.size.width * 0.3;
    CGFloat jionAllanceBtn_y =  jionAllanceBack.frame.size.height * 0.4;
    CGFloat jionAllanceBtn_width = jionAllanceBack.frame.size.width * 0.4;
    CGFloat jionAllanceBtn_height = jionAllanceBack.frame.size.height * 0.1;
    
    jionAllanceBtn.frame = CGRectMake(jionAllanceBtn_x, jionAllanceBtn_y, jionAllanceBtn_width, jionAllanceBtn_height);
    [jionAllanceBack addSubview:jionAllanceBtn];
    jionAllanceBack.userInteractionEnabled = YES;
    return jionAllanceView;
    
}
-(UIView *) NoChatInCountryView
{
    UIView *  NoChatInCountryView = [[UIView alloc] init];
    CGSize winSize = [ UIScreen mainScreen ].bounds.size;
    
    CGFloat x = 0;
    CGFloat y = winSize.height * 0.14;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.86;
    NoChatInCountryView.frame = CGRectMake(0, 0, self.chatScrollView.width, winSize.height);
    
    
    UIImageView *jionAllanceBack = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    jionAllanceBack.frame=CGRectMake(0, 0, NoChatInCountryView.frame.size.width, NoChatInCountryView.frame.size.height);
    [NoChatInCountryView addSubview:jionAllanceBack];
    
    UILabel *tipJionAllanceLabel = [[UILabel alloc]init];
    tipJionAllanceLabel.lineBreakMode = NSLineBreakByWordWrapping;
    tipJionAllanceLabel.numberOfLines = 0;
    tipJionAllanceLabel.backgroundColor = [UIColor clearColor];
    CGFloat fontSize ;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        
        fontSize=23;
        
    }else{
        fontSize=15;
    }
    
    tipJionAllanceLabel.font = [UIFont systemFontOfSize:fontSize];
    tipJionAllanceLabel.textAlignment=NSTextAlignmentCenter;
    tipJionAllanceLabel.text = [NSString stringWithMultilingualWithKey:@"132134"];//E100068=您未加入联盟，暂时无法使用联盟聊天频道
    CGFloat tipJionAllanceLabel_x =  NoChatInCountryView.frame.size.width / 4;
    CGFloat tipJionAllanceLabel_y =  NoChatInCountryView.frame.size.height / 5;
    CGFloat tipJionAllanceLabel_width = self.view.frame.size.width * 0.6;
    CGFloat tipJionAllanceLabel_height =  NoChatInCountryView.frame.size.height * 0.2;
    tipJionAllanceLabel.frame = CGRectMake(0, tipJionAllanceLabel_y, jionAllanceBack.frame.size.width, tipJionAllanceLabel_height);
    
    [jionAllanceBack addSubview:tipJionAllanceLabel];
    
    
    return NoChatInCountryView;
    
}

-(void)jionFun:(UIButton *)vSender{
    NSString *uid = [UserManager sharedUserManager].currentUser.uid;
    NSString *name = [UserManager sharedUserManager].currentUser.userName;
    [[ChatServiceController chatServiceControllerSharedManager].gameHost joinAllianceBtnClick];
}

#pragma mark -
#pragma mark CSMsgTableCellDelegate
-(void)resumeOldLongPress{
    if (self.chatVCShowTableType == ChatVCShowTableType_left){
        CSMessageTableCell *tempCell =[self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
        if (tempCell) {
            [tempCell longRecognizerResumeNormal];
        }

    }else{
        CSMessageTableCell *tempCell =[self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
        if (tempCell) {
            [tempCell longRecognizerResumeNormal];
        }

    }
    
}

//长按响应
- (void)handleLongPress:(UILongPressGestureRecognizer *)recognizer
{
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        UIWindow *window = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
        if ([window isKeyWindow] == NO)
        {
            [window becomeKeyWindow];
            [window makeKeyAndVisible];
        }
        
            if (recognizer.state == UIGestureRecognizerStateBegan && [self.countryDataSourceArray count] > 0)
            {
                //去掉之前的选中
                [self resumeOldLongPress];
//                [self.menuController dismiss];
 
                
                if  (self.chatVCShowTableType == ChatVCShowTableType_left){
                    CGPoint location = [recognizer locationInView:self.countryTableView];
                    NSIndexPath * indexPath = [self.countryTableView indexPathForRowAtPoint:location];
                    CSMessageTableCell *cell = (CSMessageTableCell *)[self.countryTableView cellForRowAtIndexPath:indexPath];
//                    [cell becomeFirstResponder];
//                    [cell longRecognizerPresed];
                    _menuIndexPath = indexPath;
                    [self _showMenuViewController:cell  andIndexPath:indexPath messageType:cell.messageModel.messageBodyType];

                }else{
                    CGPoint location = [recognizer locationInView:self.allianceTableView];
                    NSIndexPath * indexPath = [self.allianceTableView indexPathForRowAtPoint:location];
                    CSMessageTableCell *cell = (CSMessageTableCell *)[self.allianceTableView cellForRowAtIndexPath:indexPath];
//                    [cell becomeFirstResponder];
//                    [cell longRecognizerPresed];
                    _menuIndexPath = indexPath;
                    [self _showMenuViewController: cell  andIndexPath:indexPath messageType:cell.messageModel.messageBodyType];

                }
                
              
          }
            
        
    }
}


/** 点击头像 */
-(void)msgCellAvatarViewSelected:(CSBaseMessageCell *)vCell{
    CSMessageModel *tempModel = vCell.messageModel;
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showPlayerInfo" :tempModel.message.uid :tempModel.currentUser.userName :@"" :@"" :0 :TRUE ];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:tempModel.message.channelType];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 点击重发 */
-(void)msgcellResendButtonSelected:(CSBaseMessageCell *)vCell{
    CSMessageModel *reSendModel = vCell.messageModel;
    if (reSendModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
        //喇叭消息
         [self sendRadioWithMessage:reSendModel.message];
    }else{
        if (reSendModel.message.channelType == CSChannelType_Country){
             [[ChatServiceController chatServiceControllerSharedManager].gameHost sendTextWithChatMessage:reSendModel.message  andWithSendMsgType:0];
        }else{
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendTextWithChatMessage:reSendModel.message andWithSendMsgType:2];
        }
    }
}
/** 点击bubble */
-(void)msgcellBubbleViewSelected:(CSBaseMessageCell *)vCell{
    CSMessageModel *messageModel = vCell.messageModel;
    if (messageModel.messageBodyType ==CSMessageBodyType_RedPackge) {
        CSMessage *mi = vCell.messageModel.message;
        NSArray *tempArray = [mi.attachmentId componentsSeparatedByString:@"|"];
        NSString *redPacketKey = @"";
        if  (tempArray.count> 0){
            redPacketKey = tempArray[0];
        }
        if ([mi.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
            //        NSArray * arr = [mi.attachmentId componentsSeparatedByString:@"|"];
            //        if ([arr count] == 2) {
            NSArray *id_serverArr = [redPacketKey componentsSeparatedByString:@"_"];
            NSString *redPacketsId = (NSString*)id_serverArr[0];
            NSString *serverId = (NSString*)id_serverArr[1];
            
            if (redPacketsId.length > 0 && serverId.length > 0) {
                [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
                [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
                [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
            }
            //        }
            
        }else{
            //0-以领取  1- 未领取 2-红包被抢光 3-红包到期
            CSMessage *tempMsgItem = [[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict objectForKey:redPacketKey];
            if  (tempMsgItem ){
                mi.attachmentId = tempMsgItem.attachmentId;
                if (self.chatVCShowTableType == ChatVCShowTableType_left){
                    [self.countryChannel redPacketMsgUpdateStautsWithMessage:mi];
                }else{
                    [self.allianceChannel redPacketMsgUpdateStautsWithMessage:mi];
                }
            }else{
                [[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict setObject:mi forKey:redPacketKey];
            }
            if ( [mi gettingRedPackageStatusWithMessage] == 1 || [mi gettingRedPackageStatusWithMessage] == 2 ||[mi gettingRedPackageStatusWithMessage ] == 3) {
                //未领取  被抢光 红包过期
                [self keyBoardClosed];
                
                
                
                CGSize winSize=[UIScreen mainScreen].bounds.size;
                float x = winSize.width * 0.08;
                float y = winSize.height * 0.05;
                [self removeAlertRedPackageView];
                self.alertRedPackageView = [[CSAlertRedPackageView alloc] initWithFrame:CGRectMake(x,y,winSize.width * 0.9,winSize.height * 0.9) andWithMessageModel:vCell.messageModel];
                self.alertRedPackageView.alertRedPackageViewDelegate = self;
                [self.view addSubview:self.alertRedPackageView];
                
                
            }else if ( [mi gettingRedPackageStatusWithMessage] == 0){
                //已领取
                NSArray *id_serverArr = [redPacketKey componentsSeparatedByString:@"_"];
                NSString *redPacketsId = (NSString*)id_serverArr[0];
                NSString *serverId = (NSString*)id_serverArr[1];
                
                if (redPacketsId.length > 0 && serverId.length > 0) {
                    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
                    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
                    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
                }
                
            }
        }

        
    }
    else if( messageModel.messageBodyType == CSMessageBodyType_Text_EquipShare){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewEquipment: messageModel.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
    else if ( messageModel.messageBodyType == CSMessageBodyType_Text_FightReport){
       
            [[ChatServiceController chatServiceControllerSharedManager].gameHost battleMsg:messageModel.message.uid :messageModel.message.attachmentId];
            [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
   }
    else if ( messageModel.messageBodyType == CSMessageBodyType_Text_InvestigateReport){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost detectMsg:messageModel.message.uid :messageModel.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
    else if ( messageModel.messageBodyType == CSMessageBodyType_Text_AllianceConciles){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewRallyInfoByStr:messageModel.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
    else if ( messageModel.messageBodyType == CSMessageBodyType_Text_TurntableShare){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewLotteryInfoById:messageModel.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];

    }
    else if ( messageModel.messageBodyType == CSMessageBodyType_Text_AllianceTask){
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"allianceTask" :@"" :@"" :@"" :@"" :0 :TRUE];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:messageModel.message.channelType];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
    
}
/** bubble子视图响应回调 */
-(void)msgcellTextBubbleViewTouchedWithCell:(CSBaseMessageCell *)vCell andWithTouchInfo:(NSDictionary *)vInfoDic{
    NSString *typeString = [[vInfoDic allKeys]lastObject];
    NSString *touchString = [vInfoDic objectForKey:typeString];
    if  ([typeString isEqualToString:kCSTouch_Link]){
         DVLog(@"url回调:%@",touchString);
    }else if([typeString isEqualToString:kCSTouch_Point]){
        DVLog(@"坐标回调:%@",touchString);
        [self onClickPointByPoint:touchString];
    }else if([typeString isEqualToString:kCSTouch_EquipName]){
         DVLog(@"装备分享回调:%@",touchString);
    }else if([typeString isEqualToString:kCSTouch_AllianceTask]){
         DVLog(@"联盟任务回调:%@",touchString);
    }
}
#pragma mark -
#pragma mark CELL个功能代理
//邀请加入联盟
-(void) invitejoinByMsg:(NSMsgItem*) mi{
    DVLog(@"invitejoinByMsgMode");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost invitejoin:mi.uid :mi.name];
    }
}
//加入联盟
-(void)joinAllianceByMsg:(NSMsgItem *)mi{
    DVLog(@"joinAllianceByMsgMode");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost joinAlliance:mi.uid :mi.name];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}
// 查看战报
-(void)lookBattleMsgByMsg:(NSMsgItem *)mi{
    DVLog(@"lookBattleMsgByMsgMode");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost battleMsg:mi.uid :mi.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}
//侦查战报
-(void)lookDetectMsgByMsg:(NSMsgItem *)mi{
    DVLog(@"lookDetectMsgByMsgMode");
    if (mi && mi.uid.length > 0 && mi.attachmentId.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost detectMsg:mi.uid :mi.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}

/**
 *  屏蔽玩家
 *
 *  @param mi 向后台发送命令需要的数据
 */
-(void)blockPlayerByMsg:(NSMsgItem *)mi{
    DVLog(@"blockPlayerByMsgMode");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost block:mi.uid :mi.name];
    }
}

-(void)unBlockPlayerByMsg:(NSMsgItem *)mi{
    DVLog(@"unBlockPlayerByMsgMode");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBlock:mi.uid :mi.name];
        [[UserManager sharedUserManager] removeRestrictUser:mi.name :1];
    }
}
//解除禁言玩家
-(void)unBanPlayerByMsg:(NSMsgItem *)mi{
    DVLog(@"unBanPlayerByMsgMode");
    if (mi && mi.uid.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBan:mi.uid :mi.name];
    }
}
//查看装备
-(void)lookEquipmentByMsg:(NSMsgItem *)mi{
    DVLog(@"lookEquipmentByMsgMode");
    if (mi && mi.attachmentId.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewEquipment:mi.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}
//举报头像
-(void)reportPlayerHeadByMsg:(NSMsgItem *)mi{
    DVLog(@"reportPlayerHeadByMsgMode");
    if (mi && mi.uid.length > 0) {
        if ([[UserManager sharedUserManager] isUid2ReportList:mi.uid]) {
            NSString *content = [NSString stringWithMultilingualWithKey:@"105782"];
            [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :content :0.0 :0.0 :true];
        }else{
            [[UserManager sharedUserManager] addReportList:mi.uid];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost reportCustomHeadPic:mi.uid];
        }
    }
}

-(void)sayHelloByMsg:(NSMsgItem *)mi{
    DVLog(@"hello_callfun");
    int boundary = [mi.helloStrs count];
    NSString *helloStr =  mi.helloStrs[arc4random() % boundary];
    if (helloStr.length > 0) {
        self.editBarView.inputTextField.text = helloStr;
    }
}
//联盟集结
-(void)rallyArmyByMsg:(NSMsgItem *)mi{
    DVLog(@"rallyArmyByMsgModee");
    if (mi && mi.attachmentId.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewRallyInfoByStr:mi.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}
//轮盘分享
-(void)lotteryShareByMsg:(NSMsgItem *)mi{
    DVLog(@"lotteryShareByMsgModee");
    if (mi && mi.attachmentId.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost viewLotteryInfoById:mi.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}
//联盟任务
-(void)acceptTasksByMsg:(NSMsgItem *)mi{
    DVLog(@"acceptTasksByMsge");
    if (mi && mi.channelType >= 0) {
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"allianceTask" :@"" :@"" :@"" :@"" :0 :TRUE];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
}

-(void)resetSendMsgByMsg:(ChatCellIOS *)vCell{
    DVLog(@"resetSendMsgByMsgMode");
    if(vCell.cellFrame.chatMessage.post == 6 && vCell.cellFrame.chatMessage.channelType == 0){
        [self sendRadioByMsg:vCell.cellFrame.chatMessage];
    }else{
        vCell.cellFrame.chatMessage.sendState = 0;
        [self refreshCellByCell:vCell];
        if (self.chatVCShowTableType == ChatVCShowTableType_left) {
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendText:vCell.cellFrame.chatMessage :0];
        }else if (self.chatVCShowTableType == ChatVCShowTableType_right) {
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendText:vCell.cellFrame.chatMessage :2];
        }
    }
}

-(void)originalMsgByMsg:(ChatCellIOS *)vCell{
    DVLog(@"originalMsgByMsg");
 
    if  (vCell.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        NSIndexPath  *currentIndexPath =[self.countryTableView indexPathForCell:vCell];
        if(currentIndexPath){
            [self.countryTableView reloadRowsAtIndexPaths:@[currentIndexPath] withRowAnimation:UITableViewRowAnimationNone];
        }
    }else{
        NSIndexPath  *currentIndexPath =[self.allianceTableView indexPathForCell:vCell];
        if(currentIndexPath){
            [self.allianceTableView reloadRowsAtIndexPaths:@[currentIndexPath] withRowAnimation:UITableViewRowAnimationNone];
        }
    }
}

-(void)tranlateMsgByMsg:(ChatCellIOS *)vCell{
    DVLog(@"tranlateMsgByMsg");
 
    if  (vCell.cellFrame.chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        NSIndexPath  *currentIndexPath =[self.countryTableView indexPathForCell:vCell];
        if(currentIndexPath){
            [self.countryTableView reloadRowsAtIndexPaths:@[currentIndexPath] withRowAnimation:UITableViewRowAnimationNone];
        }
    }else{
        NSIndexPath  *currentIndexPath =[self.allianceTableView indexPathForCell:vCell];
        if(currentIndexPath){
            [self.allianceTableView reloadRowsAtIndexPaths:@[currentIndexPath] withRowAnimation:UITableViewRowAnimationNone];
        }
    }
}
/** 查看领主 */
-(void)onClickHeadByMsg:(NSMsgItem *)mi{
    if ([self.editBarView.inputTextField isFirstResponder]){
        [self.editBarView.inputTextField resignFirstResponder];
    }
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showPlayerInfo" :mi.uid :mi.name :@"" :@"" :0 :TRUE ];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 点击坐标 */
-(void)onClickPointByPoint:(NSString *)pointStr{
    NSString *tempPointStr = [pointStr stringByReplacingOccurrencesOfString:@"：" withString:@":"];
    NSArray *poslist=[tempPointStr componentsSeparatedByString:@":"];
    NSString *posX = [poslist objectAtIndex:0];
    NSString *posY = [poslist objectAtIndex:1];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost skipPos:posX :posY];
    [[CSConvertViewManager sharedMailInfoManager] closeMailVC];
}

//禁言
-(void)alertBanPlayerViewByView:(UIView *)view{
    DVLog(@"alertBanPlayerViewByView");
    if (view) {
        [self.view addSubview:view];
    }
}

-(void)alertViewByView:(CSAlertView *)alertView{
    [self.view addSubview:alertView];
}

-(void)clickEnterButton2AlertViewByType:(ViewType)type addWithMsg:(NSMsgItem *)mi{
    DVLog(@"clickEnterButton2AlertViewByType");
    if (mi && mi.uid.length > 0 && mi.name.length > 0) {
        switch (type) {
            case BLOCK:
                [self blockPlayerByMsg:mi];
                break;
            case CONTENTOFTHEREPORT:
                [[ChatServiceController chatServiceControllerSharedManager].gameHost contentofReportWithUid:mi.uid withContent:mi.msg];
                break;
            default:
                break;
        }
    }
}

/**
 *  发送喇叭弹出提示框
 *
 *  @param type 弹出提示框类型
 *  @param msg  弹出提示框显示数据
 */
-(void) popUpConsumptionTip:(TipType) type msg:(NSMsgItem*)msg
{
    if (self.tipPopUpController){
        [self.tipPopUpController removeFromParentViewController];
        [self.tipPopUpController.view removeFromSuperview];
    }
    self.tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
    self.tipPopUpController.chatMessage = msg;
    self.tipPopUpController.tipType = type;
    self.tipPopUpController.tipPopUpControllerDelegate = self;
    [self.view addSubview:self.tipPopUpController.view];
}
-(void) popUpConsumptionTip:(TipType) type andWithMessage:(CSMessage *)vMsg{
    if (self.tipPopUpController){
        [self.tipPopUpController removeFromParentViewController];
        [self.tipPopUpController.view removeFromSuperview];
        self.tipPopUpController = nil;
    }
    TipPopUpController *tipTpo = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
    tipTpo.tipType = type;
    tipTpo.tipPopUpControllerDelegate = self;
    tipTpo.message = vMsg;
    tipTpo.view.tag = 1001;
    self.tipPopUpController = tipTpo;
    [self.view addSubview:tipTpo.view];
}


/**
 *  重新发送喇叭弹出提示框
 *
 *  @param type 弹出提示框类型
 *  @param msg  弹出提示框显示数据
 */
-(void) popUpConsumptionTip:(TipType) type msg:(NSMsgItem*)msg addWithCell:(ChatCellIOS*)vCell
{
    if (!self.tipPopUpController){
        [self.tipPopUpController removeFromParentViewController];
        [self.tipPopUpController.view removeFromSuperview];
    }
    self.tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
    self.tipPopUpController.chatMessage = msg;
    self.tipPopUpController.tipType = type;
    self.tipPopUpController.cell = vCell;
    self.tipPopUpController.tipPopUpControllerDelegate = self;
    [self.view addSubview:self.tipPopUpController.view];
}

/**
 *  发送喇叭使用道具类型
 *
 *  @param flag true 使用金币 false 使用喇叭
 *  @param mi 发送信息
 */
-(void) sendRadioConsumptionType:(BOOL)flag msg:(NSMsgItem*)mi
{
    mi.sendState = 0;
    [self.countryChannel chatNewMsgPushed:@[mi]];
    NSString *sendLoaclTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    //给服务器发送喇叭消息
    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:mi.msg :200011 :flag :sendLoaclTime];
}
-(void) sendRadioConsumptionType:(BOOL)flag andWithMessage:(CSMessage  *)mi
{
    mi.sendState = CSMessageSendState_Delivering;
    [self.countryChannel chatNewMsgPushed:@[mi]];
    NSString *sendLoaclTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    //给服务器发送喇叭消息
    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:mi.msg :200011 :flag :sendLoaclTime];
}
/**
 *  发送喇叭使用道具类型
 *
 *  @param flag true 使用金币 false 使用喇叭
 *  @param mi 发送信息
 */
-(void) resendRadioConsumptionType:(BOOL)flag msg:(NSMsgItem*)mi addWithCell:(ChatCellIOS*) vCell
{
    [self refreshCellByCell:vCell];
    NSString *sendLoaclTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    //给服务器发送喇叭消息
    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:mi.msg :200011 :flag :sendLoaclTime];
}

-(void) refreshCellByCell:(ChatCellIOS*) vCell{
    if (self.chatVCShowTableType == ChatVCShowTableType_left) {
        NSIndexPath *tempIndexPath =[self.countryTableView indexPathForCell:vCell];
        [self.countryTableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:tempIndexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
    }else if (self.chatVCShowTableType == ChatVCShowTableType_right) {
        NSIndexPath *tempIndexPath =[self.allianceTableView indexPathForCell:vCell];
        [self.allianceTableView reloadRowsAtIndexPaths:[NSArray arrayWithObjects:tempIndexPath,nil] withRowAnimation:UITableViewRowAnimationNone];
    }
}
/**
 *  发送喇叭
 *
 *  @param mi
 */
-(void) sendRadioByMsg:(NSMsgItem*) mi{
    BOOL radioCountFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpRadioConsumptionTip];
    BOOL radioMoneyFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpMoneyConsumptionTip];
    BOOL firstDeductCount = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount;
    BOOL firstDeductMoney = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney;
    if (radioCountFlag && !firstDeductCount){
        [self sendRadioConsumptionType:FALSE msg:mi];
    }else if (radioMoneyFlag && !firstDeductMoney){
        [self sendRadioConsumptionType:TRUE msg:mi];
    }else if (radioCountFlag && firstDeductCount) {
        [self keyBoardClosed];
        [self popUpConsumptionTip:RADIOTYPE msg:mi];
    }else{
        [self keyBoardClosed];
        [self popUpConsumptionTip:NOHORN msg:mi];
    }
}
-(void) sendRadioWithMessage:(CSMessage*) mi{
    // 判断喇叭道具数量是否大于0个
    BOOL radioCountFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpRadioConsumptionTip];
    //判断使用喇叭金币是否足够
    BOOL radioMoneyFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpMoneyConsumptionTip];
   //发送喇叭是否是第一次扣除道具
    BOOL firstDeductCount = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount;
    //发送喇叭是否是第一次扣除金币
    BOOL firstDeductMoney = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney;
    if (radioCountFlag && !firstDeductCount){
        [self sendRadioConsumptionType:FALSE andWithMessage:mi];
    }
    else  if (radioMoneyFlag && !firstDeductMoney){
       [self sendRadioConsumptionType:TRUE andWithMessage:mi];
    }
        else if (radioCountFlag && firstDeductCount) {
        [self keyBoardClosed];
        [self popUpConsumptionTip:RADIOTYPE andWithMessage:mi];
    }
    else{
        [self keyBoardClosed];
          [self popUpConsumptionTip:NOHORN andWithMessage:mi];
        
    }
}
/**
 *  重新发送喇叭
 *
 *  @param mi
 *  @param vCell 需要刷新cell
 */
-(void) resendSendByMsg:(NSMsgItem*) mi addWithCell:(ChatCellIOS*)vCell{
    BOOL radioCountFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpRadioConsumptionTip];
    BOOL radioMoneyFlag = [[ChatServiceController chatServiceControllerSharedManager].gameHost isPopUpMoneyConsumptionTip];
    BOOL firstDeductCount = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount;
    BOOL firstDeductMoney = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney;
    if (radioCountFlag && !firstDeductCount){
        [self resendRadioConsumptionType:FALSE msg:mi addWithCell:vCell];
    }else if (radioMoneyFlag && !firstDeductMoney){
        [self resendRadioConsumptionType:TRUE msg:mi addWithCell:vCell];
    }else if (radioCountFlag && firstDeductCount) {
        [self keyBoardClosed];
        [self popUpConsumptionTip:RADIOTYPE msg:mi addWithCell:vCell];
    }else{
        [self keyBoardClosed];
        [self popUpConsumptionTip:NOHORN msg:mi addWithCell:vCell];
    }
}


#pragma mark -
#pragma mark TipPopUpControllerDelegate
-(void)onClickButtonSendRadio:(NSMsgItem *)mi{
    DVLog(@"resendRadioByMsg");
    if (mi) {
        [self sendRadioConsumptionType:FALSE msg:mi];
        [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount = FALSE;
    }
}
//zyt
-(void) onClickButtonSendRadioWithMessage:(CSMessage *)vMsg andWithTiPView:(TipPopUpController *)vTipPopView{
    if (vTipPopView.view.tag == 1001){
        [self sendRadioConsumptionType:FALSE andWithMessage:vMsg];
        [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount = FALSE;
    }
}
-(void)onClickButtonSendRadio2:(NSMsgItem *)mi{
    DVLog(@"resendRadioByMsg");
    if (mi) {
        [self sendRadioConsumptionType:TRUE msg:mi];
        [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney = FALSE;
    }
}
//zyt
-(void) onClickButtonSendRadio2WithMessage:(CSMessage *)vMsg andWithTiPView:(TipPopUpController *)vTipPopView{
    if (vTipPopView.view.tag == 1001){
        [self sendRadioConsumptionType:TRUE andWithMessage:vMsg];
        [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney = FALSE;
    }
}
-(void)resendRadioByMsg:(ChatCellIOS *)vCell{
    DVLog(@"resendRadioByMsg");
    if (vCell) {
        vCell.cellFrame.chatMessage.sendState = 0;
        
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendRadio:vCell.cellFrame.chatMessage];
    }
}

#pragma mark -
#pragma mark 私有函数
/**
 *  当前用户发言是否有限制
 *
 *  @return
 */
-(BOOL) isPlayerNameRestrict
{
    BOOL ret = false;

    NSUserInfo *user = [UserManager sharedUserManager].currentUser;
    return [user isHaveNameRestrict];
}
/**
 *  进制发送信息时 弹出改名提示框
 */
-(void) banSendMsgTip
{
    NSString * tipStr = [NSString stringWithMultilingualWithKey:@"132130"];
    if (tipStr.length > 0) {
        CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
        alertView.viewType = CHANGENAME;
        [alertView showViewByType];
        alertView.titleType = ZYTAlertViewTitleType_shield;
        [alertView setNameString:tipStr];
        alertView.alertViewDelegate = self;
        [self.view addSubview:alertView];
    }
}

-(BOOL) isSendChat{
    DVLog(@"isSendChat");
    //判断是否改名
    if([self isPlayerNameRestrict]){
        [self keyBoardClosed];
        [self banSendMsgTip];
        return FALSE;
    }    
    return TRUE;
}

/**
 *  弹出Cocos改名UI
 */
#pragma mark -
#pragma mark 改名VIEW代理
-(void)popUpChangeName{
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost changeNickName];
}

-(void)addAlertRedPackageByView:(CSAlertRedPackageView *)alertRedPackageView{
    alertRedPackageView.alertRedPackageViewDelegate = self;
    [self.view addSubview:alertRedPackageView];
}


#pragma mark -
#pragma mark  红包视图代理

-(void)openRedPackageByMsg:(NSMsgItem *)msgItem{
    DVLog(@"chatcellios::openRedPackageByMsg");

    NSArray *arr = [msgItem.attachmentId componentsSeparatedByString:@"|"];
    NSArray *arr2 = [arr[0] componentsSeparatedByString:@"_"];

    if ([arr2 count] == 2) {
        NSString *attachmentId = arr2[0];
        NSString *serverId = arr2[1];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingRedPackageStatusById:attachmentId withServerId:serverId ];
    }
}
-(void)redPackageOpenedWithAttachmentID:(NSString *)vAttachMentIDString andWithPackageView:(CSAlertRedPackageView *)vPackageView{
    NSArray *arr = [vAttachMentIDString componentsSeparatedByString:@"|"];
    NSArray *arr2 = [arr[0] componentsSeparatedByString:@"_"];
    
    if ([arr2 count] == 2) {
        NSString *attachmentId = arr2[0];
        NSString *serverId = arr2[1];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingRedPackageStatusById:attachmentId withServerId:serverId ];
    }

}
//-(void)lookRedPackageByMsg:(NSMsgItem *)msgItem{
//    DVLog(@"chatcellios::lookRedPackageByMsg");
//    if (msgItem){
//        NSArray * arr = [msgItem.attachmentId componentsSeparatedByString:@"|"];
//        if ([arr count] == 2) {
//            NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
//            NSString *redPacketsId = (NSString*)id_serverArr[0];
//            NSString *serverId = (NSString*)id_serverArr[1];
//            if (redPacketsId.length > 0 && serverId.length > 0) {
//                [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
//                [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
//                [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:msgItem.channelType];
//            }
//        }
//        [self removeAlertRedPackageView];
//    }
//}

-(void)redPackageLookedWithAttachMentID:(NSString *)vAttachMentID andWithMsgChannelType:(int)vType{
    NSArray * arr = [vAttachMentID componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
        NSString *redPacketsId = (NSString*)id_serverArr[0];
        NSString *serverId = (NSString*)id_serverArr[1];
        if (redPacketsId.length > 0 && serverId.length > 0) {
            [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:vType];
        }
    }
        [self removeAlertRedPackageView];
}
-(void) popUpCocosRedPackageByMsg:(NSMsgItem *)msgItem{
    NSArray * arr = [msgItem.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
        NSString *redPacketsId = (NSString*)id_serverArr[0];
        NSString *serverId = (NSString*)id_serverArr[1];
        
        if (redPacketsId.length > 0 && serverId.length > 0) {
            [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"getRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:msgItem.channelType];
        }
    }
    [self removeAlertRedPackageView];
}
-(void)redPackagePopupCocosWithMessageModel:(CSMessageModel *)vMessageModel{
    NSArray * arr = [vMessageModel.message.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSArray *id_serverArr = [arr[0] componentsSeparatedByString:@"_"];
        NSString *redPacketsId = (NSString*)id_serverArr[0];
        NSString *serverId = (NSString*)id_serverArr[1];
        
        if (redPacketsId.length > 0 && serverId.length > 0) {
            [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"getRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:vMessageModel.message.channelType];
        }
    }
    [self removeAlertRedPackageView];
}
//红包
-(void)createRedPackageViewByMsg:(NSMsgItem *)mi{
    
    NSArray *tempArray = [mi.attachmentId componentsSeparatedByString:@"|"];
    NSString *redPacketKey = @"";
    if  (tempArray.count> 0){
        redPacketKey = tempArray[0];
    }
    if ([mi.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
//        NSArray * arr = [mi.attachmentId componentsSeparatedByString:@"|"];
//        if ([arr count] == 2) {
            NSArray *id_serverArr = [redPacketKey componentsSeparatedByString:@"_"];
            NSString *redPacketsId = (NSString*)id_serverArr[0];
            NSString *serverId = (NSString*)id_serverArr[1];
            
            if (redPacketsId.length > 0 && serverId.length > 0) {
//                [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
                [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
                [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
                [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
            }
//        }

    }else{
          //0-以领取  1- 未领取 2-红包被抢光 3-红包到期
        NSMsgItem *tempMsgItem = [[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict objectForKey:redPacketKey];
        if  (tempMsgItem ){
            mi.attachmentId = tempMsgItem.attachmentId;
            if (self.chatVCShowTableType == ChatVCShowTableType_left){
                [self.countryChannel redPacketMsgSaveToDBWithMsgItem:mi];
            }else{
                [self.allianceChannel redPacketMsgSaveToDBWithMsgItem:mi];
            }
        }else{
             [[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict setObject:mi forKey:redPacketKey];
        }
        if ( [mi gettingRedPackageStatus] == 1 || [mi gettingRedPackageStatus] == 2 ||[mi gettingRedPackageStatus ] == 3) {
            //未领取  被抢光 红包过期
            [self keyBoardClosed];
            
            
           
            CGSize winSize=[UIScreen mainScreen].bounds.size;
            float x = winSize.width * 0.08;
            float y = winSize.height * 0.05;
            [self removeAlertRedPackageView];
            self.alertRedPackageView = [[CSAlertRedPackageView alloc] initWithFrame:CGRectMake(x,y,winSize.width * 0.9,winSize.height * 0.9) AndWith:mi];
            self.alertRedPackageView.alertRedPackageViewDelegate = self;
            [self.view addSubview:self.alertRedPackageView];
            
            
        }else if ( [mi gettingRedPackageStatus] == 0){
            //已领取
            NSArray *id_serverArr = [redPacketKey componentsSeparatedByString:@"_"];
            NSString *redPacketsId = (NSString*)id_serverArr[0];
            NSString *serverId = (NSString*)id_serverArr[1];
            
            if (redPacketsId.length > 0 && serverId.length > 0) {
//                [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
                [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewRedPackage" :redPacketsId :serverId:@"" :@"" :0 :TRUE ];
                [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.channelType];
                [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
            }
 
         }
    }
    
 
}

-(void) removeAlertRedPackageView{
    if (self.alertRedPackageView) {
        [self.alertRedPackageView removeFromSuperview];
        self.alertRedPackageView = nil;
    }
}

#pragma mark -
#pragma mark 关闭键盘通知
- (void)notifCloseKeyBoard:(NSNotification *)notification{
    [self keyBoardClosed];
}

#pragma mark -
#pragma mark menu Item 弹出

- (BOOL)canBecomeFirstResponder{
    return YES;
}

-(BOOL)canPerformAction:(SEL)action withSender:(id)sender{
    if (action == @selector(menuItemAction:) ||
        action == @selector(menuItemShowOrginal:) ||
        action == @selector(menuItemTranslate:) ||
        action == @selector(menuItemRotaryTableShare:) ||
        action == @selector(menuItemEquipShare:) ||
        action == @selector(menuImteAllianceTask:) ||
        action == @selector(menuItemBanned:) ||
        action == @selector(menuItemClosedBanned:) ||
        action == @selector(menuItemBlock:) ||
        action == @selector(menuItemCloseBlock:) ||
        action == @selector(menuItemInformHead:) ||
        action == @selector(menuItemInformMsg:) ||
        action == @selector(menuItemCopyed:) ||
        action == @selector(menuItemBattleReport:) ||
         action == @selector(menuItemAllianceConciles:)) {
        return YES;
    }
    return [super canPerformAction:action withSender:sender];
}

- (void)_showMenuViewController:(UIView *)showInView
                   andIndexPath:(NSIndexPath *)indexPath
                    messageType:(CSMessageBodyType)messageType
{
 
    CSMessageModel *model = nil;
    if (self.chatVCShowTableType == ChatVCShowTableType_left){
        model = [self.countryDataSourceArray objectAtIndex:indexPath.row];
    }else{
        model = [self.allianceDataSourceArray objectAtIndex:indexPath.row];
    }
    NSArray *tempArray = [CSChatMenuItemFactory addMenuItemWithChatMsgModel:model andWithTarget:self andWithSelect:@selector(menuItemAction:)];
    for (CustemMenuItem *tempItem  in tempArray) {
        if  (tempItem.menuItemType == CSMenuItemType_showOriginal){
            DLog(@"原文Item");
              tempItem.action = @selector(menuItemShowOrginal:);
        }else if (tempItem.menuItemType == CSMenuItemType_translate){
            DLog(@"翻译Item");
              tempItem.action = @selector(menuItemTranslate:);
        }else if (tempItem.menuItemType == CSMenuItemType_rotaryTableShare){
            DLog(@"转盘分享Item");
            tempItem.action = @selector(menuItemRotaryTableShare:);
        }else if(tempItem.menuItemType == CSMenuItemType_equipShare){
            DLog(@"装备分享Item");
            tempItem.action = @selector(menuItemEquipShare:);
        }else if(tempItem.menuItemType == CSMenuItemType_allianceTask){
            DLog(@"联盟任务分享Item");
            tempItem.action = @selector(menuImteAllianceTask:);
        }else if(tempItem.menuItemType == CSMenuItemType_banned){
            DLog(@"禁言 Item");
            tempItem.action = @selector(menuItemBanned:);
        }else if(tempItem.menuItemType == CSMenuItemType_closedBanned){
            DLog(@" 取消禁言Item");
            tempItem.action = @selector(menuItemClosedBanned:);
        }else if (tempItem.menuItemType == CSMenuItemType_block){
            DLog(@"屏蔽Item");
            tempItem.action = @selector(menuItemBlock:);
        }else if (tempItem.menuItemType == CSMenuItemType_closedBlock){
            DLog(@"取消屏蔽Item");
            tempItem.action = @selector(menuItemCloseBlock:);
        }else if (tempItem.menuItemType == CSMenuItemType_informHead){
            DLog(@"举报头像Item");
            tempItem.action = @selector(menuItemInformHead:);
        }else if (tempItem.menuItemType == CSMenuItemType_informMsg){
            DLog(@"举报消息Item");
            tempItem.action = @selector(menuItemInformMsg:);
        }else if (tempItem.menuItemType == CSMenuItemType_copy){
            DLog(@"复制Item");
            tempItem.action = @selector(menuItemCopyed:);
        }else if(tempItem.menuItemType == CSMenuItemType_fightReport){
            DLog(@"战报Item");
            tempItem.action= @selector(menuItemBattleReport:);
        }else if(tempItem.menuItemType == CSMenuItemType_AllianceConciles){
            DLog(@"联盟集结 Item");
            tempItem.action = @selector(menuItemAllianceConciles:);
        }
      
    }
  
    if (tempArray.count> 0){
        
  
        if (self.menuController == nil){
            self.menuController = [UIMenuController sharedMenuController];
            
        }
        CSMessageTableCell *cell = (CSMessageTableCell *)showInView;
        [cell becomeFirstResponder];
        [cell longRecognizerPresed];
        self.menuController.menuItems = tempArray;
        [self.menuController setTargetRect:cell.bubbleView.frame inView:cell.bubbleView.superview];
        [self.menuController setMenuVisible:YES animated:YES];
   
        
//        CGRect cellRect ;
//        float y = 0;
//        if  (self.chatVCShowTableType == ChatVCShowTableType_left){
//          cellRect =  [self.countryTableView rectForRowAtIndexPath:_menuIndexPath];
//           y = cellRect.origin.y-self.countryTableView.contentOffset.y;
//        }else{
//           cellRect = [self.allianceTableView rectForRowAtIndexPath:_menuIndexPath];
//           y  = cellRect.origin.y-self.allianceTableView.contentOffset.y;
//        }
//       
//        
//
// 
//
//        [self.menuController showInView:self.view atPoint:CGPointMake(cell.bubbleView.center.x, y+self.buttonView.height+self.headView.height + cell.cellView.bodyView.top+cell.bubbleView.top) ];
    

    }else{
        DVLog(@"当前cell没有可点击的item操作");
    }
}

-(void)menuItemAction:(CustemMenuItem *)vSender{
//    CSMessageTableCell *pressedCell =nil ;
//    if  (self.chatVCShowTableType == ChatVCShowTableType_left ){
//        pressedCell = [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
//    }else{
//        pressedCell = [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
//    }
//    if  (vSender.menuItemType == CSMenuItemType_showOriginal){
//        //显示原文
//        [self chatTableCellShowOrginal:pressedCell];
//    }else if(vSender.menuItemType == CSMenuItemType_translate){
//        [self chatTableCellTranslate:pressedCell];
//    }
 
    
//    DVLog(@"menuTitle:%@ \nmenutype :%d ",vSender.title,vSender.menuItemType);
//     DVLog(@" nmenutype :%d ", vSender.menuItemType);
}
#pragma mark -
#pragma mark MenuItem Action
/** 显示原文 */
-(void)menuItemShowOrginal:(id)sender{
    
   
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        
        CSMessageTableCell *vCell = [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
         vCell.messageModel.isTranslatLabelShow = NO;
         [vCell.messageModel updateMessageModelWithMsg];
     
        [self.countryTableView reloadRowAtIndexPath:_menuIndexPath  withRowAnimation:UITableViewRowAnimationFade];
    }else{
        CSMessageTableCell *vCell = [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
        vCell.messageModel.isTranslatLabelShow = NO;
        [vCell.messageModel updateMessageModelWithMsg];
        
        [self.allianceTableView reloadRowAtIndexPath:_menuIndexPath  withRowAnimation:UITableViewRowAnimationFade];
    }
}
/** 显示翻译 */
-(void)menuItemTranslate:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
      vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
         vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
 
    
    
    if (vPressedCell.messageModel.message.translateMsg.length> 0){
        vPressedCell.messageModel.isTranslatLabelShow = YES;
        [vPressedCell.messageModel updateMessageModelWithMsg];
        if  (self.chatVCShowTableType == ChatVCShowTableType_left){
         
            [self.countryTableView reloadRowAtIndexPath:_menuIndexPath  withRowAnimation:UITableViewRowAnimationFade];
        }else{
            NSIndexPath * tempIndexPath = [self.allianceTableView indexPathForCell:vPressedCell];
            [self.allianceTableView reloadRowAtIndexPath:tempIndexPath  withRowAnimation:UITableViewRowAnimationFade];
        }

    }else{
        __block  ChattingVC *weakSelf = self;
        __block CSMessageTableCell *weakCell = vPressedCell;
        CSGoogleTranslat *  translat =[[CSGoogleTranslat alloc]init];
        NSString *originalString = vPressedCell.messageModel.message.msg;
        
        NSString *localLanString = [NSString stringWithUTF8String: LocalController::shared()->getLanguageFileName().c_str()] ;
        DVLog(@"%@",localLanString);
        
        [translat translationingWithText:originalString andWihtSL:@"auto" andWithTL:localLanString andResultBlock:^(ZYTGoogleTranslatHTTPStatus status, NSString *resultString, NSString *orgLangTypeString) {
            if (status == ZYTGoogleTranslatHTTPStatus_sucess){
                DVLog(@"%@",resultString);
                weakCell.messageModel.message.translateMsg = resultString;
                weakCell.messageModel.message.originalLang = orgLangTypeString;
                if  (weakSelf.chatVCShowTableType == ChatVCShowTableType_left){
                    NSIndexPath * tempIndexPath = [weakSelf.countryTableView indexPathForCell:vPressedCell];
                    [weakSelf.countryTableView reloadRowAtIndexPath:tempIndexPath  withRowAnimation:UITableViewRowAnimationFade];
                }else{
                    NSIndexPath * tempIndexPath = [weakSelf.allianceTableView indexPathForCell:vPressedCell];
                    [weakSelf.allianceTableView reloadRowAtIndexPath:tempIndexPath  withRowAnimation:UITableViewRowAnimationFade];
                }

            }
        }];

 

    }
}
/** 转盘分享 */
-(void)menuItemRotaryTableShare:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }

    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewLotteryInfoById:vPressedCell.messageModel.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 装备分享 */
-(void)menuItemEquipShare:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewEquipment:vPressedCell.messageModel.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 联盟任务 */
-(void)menuImteAllianceTask:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    DVLog(@"AllianceTask");
    if (mi && mi.message.channelType >= 0) {
//        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"allianceTask" :@"" :@"" :@"" :@"" :0 :TRUE];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.message.channelType];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
  

}



/** 禁言 */
-(void)menuItemBanned:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
     BanPopUpController *banPopUp  = [[BanPopUpController alloc] initWithNibName:@"BanPopUpController" bundle:nil];
    
    [banPopUp settingInitDataWithMessageModel:mi];
    self.banpopUpOController = banPopUp;
    [self.view addSubview:banPopUp.view];
    [self.view bringSubviewToFront:banPopUp.view];
    
}
/** 解除禁言 */
-(void)menuItemClosedBanned:(id)sender{
    DVLog(@"menuItemClosedBanned");
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;

    if (mi && mi.message.uid.length > 0) {
        NSString *nameString = mi.currentUser.userName;
        if  (nameString.length == 0){
            nameString = mi.message.uid;
        }
        if (mi.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
            [[ChatServiceController chatServiceControllerSharedManager].gameHost closeBanPlayNoticeWithUserUid:mi.message.uid];
        }else{
             [[ChatServiceController chatServiceControllerSharedManager].gameHost unBan:mi.message.uid :mi.currentUser.userName];
        }
       
    }
}
/** 屏蔽 */
-(void)menuItemBlock:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    NSString *nameString = mi.currentUser.userName;
    if  (nameString.length == 0){
        nameString = mi.message.uid;
    }
        [[ChatServiceController chatServiceControllerSharedManager].gameHost block:mi.message.uid :nameString];
    
}
/** 解除屏蔽 */
-(void)menuItemCloseBlock:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    NSString *nameString = mi.currentUser.userName;
    if  (nameString.length == 0){
        nameString = mi.message.uid;
    }
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBlock:mi.message.uid :nameString];
        [[UserManager sharedUserManager] removeRestrictUser:mi.message.uid :1];
   
}
/** 举报头像 */
-(void)menuItemInformHead:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    if (mi && mi.message.uid> 0) {
        if ([[UserManager sharedUserManager] isUid2ReportList:mi.message.uid]) {
            NSString *content = [NSString stringWithMultilingualWithKey:@"105782"];//105782=您已经举报过该领主的头像了
            [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :content :0.0 :0.0 :true];
        }else{
            [[UserManager sharedUserManager] addReportList:mi.message.uid];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost reportCustomHeadPic:mi.message.uid];
        }
    }

}
/** 举报消息 */
-(void)menuItemInformMsg:(id)sender{

    NSString * tipStr = [NSString stringWithMultilingualWithKey:@"105393"];
    CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
    alertView.tag = 101;
    alertView.viewType = CONTENTOFTHEREPORT;
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    alertView.alertViewDelegate = self;
    [self.view addSubview:alertView];
    [self.view bringSubviewToFront:alertView];
}
/** 复制消息 */
-(void)menuItemCopyed:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    NSString *copyString = nil;
    if(mi.isTranslatLabelShow){
        copyString = mi.message.translateMsg;
    }else{
        copyString= mi.message.msg;
    }
    
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setString:copyString];
}
/** 战报 */
-(void)menuItemBattleReport:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;
    if (mi.messageBodyType == CSMessageBodyType_Text_FightReport){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost battleMsg:mi.message.uid :mi.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }else if (mi.messageBodyType == CSMessageBodyType_Text_InvestigateReport){
        [[ChatServiceController chatServiceControllerSharedManager].gameHost detectMsg:mi.message.uid :mi.message.attachmentId];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];

    }

}

/** 联盟集结 */
-(void)menuItemAllianceConciles:(id)sender{
    CSMessageTableCell *vPressedCell =nil;
    if  (self.chatVCShowTableType == ChatVCShowTableType_left){
        vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
    }else{
        vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
    }
    CSMessageModel *mi = vPressedCell.messageModel;

    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewRallyInfoByStr:mi.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];

}

#pragma mark -
#pragma mark CSAlertView Delegate
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView{
    if (vAlertView.tag == 101){
        CSMessageTableCell *vPressedCell =nil;
        if  (self.chatVCShowTableType == ChatVCShowTableType_left){
            vPressedCell =  [self.countryTableView cellForRowAtIndexPath:_menuIndexPath];
        }else{
            vPressedCell =  [self.allianceTableView cellForRowAtIndexPath:_menuIndexPath];
        }
        CSMessageModel *mi = vPressedCell.messageModel;
        [[ChatServiceController chatServiceControllerSharedManager].gameHost contentofReportWithUid:mi.message.uid withContent:mi.message.msg];
        
    }
}

@end
