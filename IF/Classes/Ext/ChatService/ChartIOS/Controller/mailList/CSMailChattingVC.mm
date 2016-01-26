//
//  CSMailChattingVC.m
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import "CSMailChattingVC.h"
#import "TopHeadView.h"
#import "ChatChannel.h"
#import "ChatCellIOS.h"
#import "CSChatSystemCell.h"
#import "ServiceInterface.h"
#import "UserManager.h"
#import "MsgMessage.h"
#import "ChatServiceController.h"
#import "EditBarView.h"
#import "SRRefreshView.h"
#import "PersonSelectVC.h"
#import "CSCommandManager.h"
#import "ChatServiceCocos2dx.h"
#import "NSString+Cocos2dHelper.h"
#import "CSAlertView.h"

#import "CSMessageTableCell.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
#import "CSMessage+Manager.h"
#import "ChatChannel+ChatMessage.h"
#import "CSConvertViewManager.h"
#import "CSChatMenuItemFactory.h"
#import "CSAlertView.h"

#import "BanPopUpController.h"
@interface CSMailChattingVC ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate,EditBarViewDelegate,ChatChannelDelegate,SRRefreshDelegate,ChatCellIOSDelegate,CSMsgTableCellDelegate,ZYTAlertViewDelegate>

{
    CGFloat editBarH;
    int HeadH;
      dispatch_queue_t csMailChattingQuene;
    BOOL isMod;
    
     UILongPressGestureRecognizer *_lpgr;
}
@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (strong,nonatomic) TopHeadView *topHeadView;
@property (nonatomic,strong) EditBarView * editBarView;
@property (strong, nonatomic) SRRefreshView *slimeView;

@property (nonatomic,strong) NSMutableArray * dataSourceArray;
@property (nonatomic,assign) CGFloat offsetY;

/** 当前长恩选中的cell indexpath */
@property (strong, nonatomic) NSIndexPath *menuIndexPath;

@property (strong, nonatomic) UIMenuController *menuController;

@property (nonatomic,assign,getter=isHasNewMsgPushed)BOOL hasNewMsgPushed;
@property (nonatomic,assign,getter=isHasLoadOldMsgPushed)BOOL hasLoadOldMsgPushed;
@property (nonatomic,assign,getter=isRefreshingFlagTure)BOOL refreshingFlag;

@end

@implementation CSMailChattingVC

-(instancetype)init{
    self = [super init];
    if  (self){
        csMailChattingQuene = dispatch_queue_create("com.chatServer.csMailChattingQuene", NULL);
    }
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
 
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
//    [self.footView addSubview:self.inputView];
    if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER){
        
        [[CSCommandManager sharedCommandManager]settingReadStatus_Chat_WithChatFromUid:self.currentChatChannel.channelID andWithModFlag:NO];
    }
    self.currentChatChannel.unreadCount = 0;
     [self.currentChatChannel channelUpDateWithSelfChanged];
    [self settingInputView];
    [self.headView addSubview:self.topHeadView];
    [self.view addSubview:self.mailChatTableView];
    [self.mailChatTableView addSubview:self.slimeView];
    if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
        
        
        NSString *channelUserUID =_currentChatChannel.channelID;
 
            if ([_currentChatChannel.channelID hasSuffix:@"@mod"]){
                channelUserUID  =  [channelUserUID stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
            }
            NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:channelUserUID];
            DVLog(@"%@",[user printAllPropertys]);
            NSString *titleString =@"";
        if(user ){
                titleString = [titleString stringByAppendingFormat:@"%@",user.userName];
            }else{
                titleString  = channelUserUID;
                
            }
          [self setTitleWithString:titleString];

    }else{
        if (self.currentChatChannel.customName) {
            [self setTitleWithString:self.currentChatChannel.customName];
        }else{
            [self setTitleWithString:@"多人聊天"];
        }
        
    }
   
      [ServiceInterface serviceInterfaceSharedManager].mailChatVC = self;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
    [[NSNotificationCenter defaultCenter]postNotificationName:kChatChannelUnReadCountChanged object:nil userInfo:@{@"channelID":self.currentChatChannel.channelID}];
 
}
-(void)deallocAnyWithMailChattingVC{

    self.headView= nil;
    self.footView = nil;
    self.topHeadView = nil;
    self.editBarView = nil;
    self.slimeView = nil;
    self.dataSourceArray = nil;
    self.mailChatTableView = nil;
    self.currentChatChannel.channelDelegate = nil;
    self.currentChatChannel = nil;
    [self.editBarView.inputTextField resignFirstResponder];
    if (self.currentChatChannel.msgList.count> 0){
        ChatCellFrame *lastFrame = [self.currentChatChannel.msgList lastObject];
        [self.currentChatChannel.msgList removeAllObjects ];
        [self.currentChatChannel.msgList addObject:lastFrame];
    }
    if  ([self.editBarView.inputTextField isFirstResponder]){
        [self.editBarView.inputTextField resignFirstResponder];
    }
    self.currentChatChannel.channelDelegate = nil;
        csMailChattingQuene = nil;
}
-(void)dealloc{

    [self deallocAnyWithMailChattingVC];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillChangeFrameNotification object:nil];
 
}


-(void)settingInputView{
    self.editBarView = [[EditBarView alloc]initWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height)];
    self.editBarView.editBarViewDelegate = self;
    self.editBarView.speakerButtonShowing = NO;
    
    self.editBarView.isCountry=NO;


    [self.footView addSubview:self.editBarView];
    [self.footView bringSubviewToFront:self.editBarView];
}
#pragma mark -
#pragma mark getter

-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView =[[UIView alloc]init];
        headView.backgroundColor = [UIColor grayColor];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _headView = headView;
    }
    return _headView;
}
-(UIView *)footView{
    if (_footView == nil) {
        
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            editBarH=65;
            HeadH=35;
        }else{
            editBarH=56.8;
            HeadH=25;
        }

        UIView *footView =[[UIView alloc]initWithFrame:CGRectMake(0, kMainScreenHeight - editBarH, kMainScreenWidth, editBarH)];
        footView.backgroundColor =[UIColor redColor];
        _footView = footView;
    }
    return _footView;
}

-(TopHeadView *)topHeadView{
    if (_topHeadView == nil) {
        TopHeadView *topView = [TopHeadView topHeadViewWithRightButtonShowFlag:YES andShowRightButtonType:TopHeadViewRightButtonType_ChatRoomMember];
        topView.frame = self.headView.frame;
     
        topView.topHeadViewDelegate = self;
        _topHeadView = topView;
    }
    return  _topHeadView;
}

-(UITableView *)mailChatTableView{
    if (_mailChatTableView == nil) {
        UITableView *tableView = [[UITableView alloc]initWithFrame:CGRectMake( 0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height-self.footView.height) style:UITableViewStylePlain];
        tableView.delegate = self;
        tableView.dataSource = self;
        UIImageView *backImageView =[[UIImageView alloc]init];
        backImageView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"csChatTableBackImage1"]];//csChatTableBackImage1
        tableView.backgroundView =backImageView;
        tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
        
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
        [tableView addGestureRecognizer:tap];
        _lpgr = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
        _lpgr.minimumPressDuration = 0.5;
        [tableView addGestureRecognizer:_lpgr];
        
        _mailChatTableView = tableView;
    }
    return _mailChatTableView;
}
-(NSMutableArray *)dataSourceArray{
    if (_dataSourceArray == nil) {
        NSMutableArray *array = [NSMutableArray array ];
        _dataSourceArray = array;
    }
    return  _dataSourceArray;
}
- (SRRefreshView *)slimeView
{
    if (_slimeView == nil) {
        _slimeView = [[SRRefreshView alloc] init];
        _slimeView.delegate = self;
        _slimeView.upInset = 0;
        _slimeView.slimeMissWhenGoingBack = YES;
        _slimeView.slime.bodyColor = [UIColor grayColor];
        _slimeView.slime.skinColor = [UIColor grayColor];
        _slimeView.slime.lineWith = 1;
        _slimeView.slime.shadowBlur = 4;
        _slimeView.slime.shadowColor = [UIColor grayColor];
    }
    
    return _slimeView;
}

#pragma mark -
#pragma mark setter
-(void)setTitleWithString:(NSString *)titleString{
    self.topHeadView.titleNameLabel.text = titleString;
}
-(void)setCurrentChatChannel:(ChatChannel *)currentChatChannel{
    _currentChatChannel = currentChatChannel;
    _currentChatChannel.channelDelegate  = self;
    
   
    if ([_currentChatChannel.channelID hasSuffix:@"@mod"]){
        isMod = YES;
    }else{
        isMod = NO;
    }
    [_currentChatChannel gettingFirstMsg];
//    self.dataSourceArray = [_currentChatChannel.msgList mutableCopy];
}
-(void)keyBoardClosed{
    [self.editBarView.inputTextField resignFirstResponder];
 
}



#pragma mark -
#pragma mark tableView 
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
 
    return self.dataSourceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *identify = @"mailIdentify";
    if (self.dataSourceArray == nil){
        return nil;
    }
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        CSMessageModel *msgModel = [self.dataSourceArray objectAtIndex:indexPath.row];
        DVLog(@"%@",[msgModel printAllPropertys]);
        if (msgModel.messageBodyType == CSMessageBodyType_Text_ChatRoomSystemMsg){
            //聊天室系统消息
            CSChatSystemCell *cell =[CSChatSystemCell chatSystemCellWithTableView:tableView];
            [cell setMsgModel:msgModel];
            
            cell.backgroundColor=[UIColor clearColor];
            
            return cell;

        }else{
            static  NSString *identify = @"mailMsgcell";
            
            CSMessageTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
           
            if (cell == nil) {
                cell =[[CSMessageTableCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify model:msgModel];
                cell.msgTableCellDelegate = self;
                cell.selectionStyle = UITableViewCellSelectionStyleNone;
            }
            cell.messageModel = msgModel;
            return cell;

        }
    }else{
        ChatCellFrame * cellFrame =[self.dataSourceArray objectAtIndex:indexPath.row];
        
        DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
        
        
        if (cellFrame.chatMessage.post == 100) {
            
            CSChatSystemCell *cell =[CSChatSystemCell chatSystemCellWithTableView:tableView];
            [cell setChatCellFrame:cellFrame];
            
            cell.backgroundColor=[UIColor clearColor];
            
            return cell;
            
        }else{
            
            ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:identify];
            cell.selectionStyle = UITableViewCellSelectionStyleGray;
            if (cell == nil) {
                cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];
            }
            cell.chatCellIosDelegate = self;
            cell.backgroundColor=[UIColor clearColor];
            cell.selectionStyle = UITableViewCellSelectionStyleNone;
            [cell setCellFrame:cellFrame];
            return cell;
            
        }

    }
    
    
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
   
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        CSMessageModel *model =[self.dataSourceArray objectAtIndex:indexPath.row];
        return model.cellHeight;
    }else{
        ChatCellFrame *cellFrame = self.dataSourceArray[indexPath.row];
        
        CGFloat totalHeight = 0;
        if (cellFrame.chatMessage.post == 100) {
            return cellFrame.chatSystemSize.height+10;
        }else if (cellFrame.isShowUiTopView_BOOL) {
            totalHeight = cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
        }else{
            totalHeight = cellFrame.cellHeight;
        }
        
        return totalHeight;
    }

}

#pragma mark -
#pragma mark action

-(void)sendMsg:(NSString *)vMsgString{
    
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
         CSMessageBodyType post = CSMessageBodyType_Text_Normal;
        if ([self.currentChatChannel.channelID hasSuffix:@"@mod"]){
            post = CSMessageBodyType_Text_ModMsg;
        }
       
        CSChannelType channelType = (CSChannelType )self.currentChatChannel.channelType;
        CSMessage *msg = [CSMessage messageWithSelfSendString:vMsgString andWithChannelType: channelType andWithMessageBodyType: post];
 
        msg.sendState = CSMessageSendState_Failure;
        //消息插库
        [self.currentChatChannel savingMessageForSelfSendToServerWithMessage:msg];
        msg.sendState = CSMessageSendState_Delivering;
     
        [self.currentChatChannel chatNewMsgPushed:@[msg]];
        
    
        if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
            DVLog(@"邮件发送信息 == ");
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendMailWithMessage:msg];
        }else{
            DVLog(@"聊天室发送信息 == ");
            NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",msg.sendLocalTime];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendChatRoomMsg:msg.msg :self.currentChatChannel.channelID sendLocalTime:sendLocalTime];
        }

        
    }else{
        
        NSMsgItem *msgItem = [self msgItemWithSendString:vMsgString];
        if ([self.currentChatChannel.channelID hasSuffix:@"@mod"]){
            msgItem.post = 200;
        }
        //自己发的消息，先入库
        msgItem.sendState = 1;
        [self.currentChatChannel savingMsgItemToDBWithMsgItem :msgItem];
        msgItem.sendState = 0;
        
        //通知channel，让channel当做新消息处理
        [self.currentChatChannel chatNewMsgPushed:@[msgItem]];
        
        if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
            DVLog(@"邮件发送信息 == ");
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendMail:msgItem];
        }else{
            DVLog(@"聊天室发送信息 == ");
            NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",msgItem.sendLocalTime];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendChatRoomMsg:msgItem.msg :self.currentChatChannel.channelID sendLocalTime:sendLocalTime];
        }

    }
   
}
-(NSMsgItem *)msgItemWithSendString:(NSString *)vMsgString{
    NSMsgItem *chatMessage = [[ServiceInterface serviceInterfaceSharedManager] createChatMessage:vMsgString];
     chatMessage.channelType = self.currentChatChannel.channelType;
    return chatMessage;
}
-(ChatCellFrame *)cellFrameWithMsgItem:(NSMsgItem *)vMsgItem{
    
    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:vMsgItem];
    return cellFrame;
}


#pragma mark -
#pragma mark topHeadView  delegate
-(void)clickButton{

    
    dispatch_async(csMailChattingQuene, ^{
        [self.currentChatChannel channelMarkToReadStatus];
        ChatServiceCocos2dx::updateUnreadCountOnMastermind();

    });
    
    [self deallocAnyWithMailChattingVC];
    self.currentChatChannel = nil;
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
        UINavigationController *nav = (UINavigationController *)[ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
        if ([nav.viewControllers objectAtIndex:0] == self){
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = YES;
               CCDirector::sharedDirector()->setVisitFlag(true);
        }else{
            [self.navigationController popViewControllerAnimated:YES];

        }
       
    }else{
         [self.navigationController popViewControllerAnimated:YES];
    }
 

}

-(void)rightButtonAction:(UIButton *)sender{
    PersonSelectVC *personSelectVC;
    [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingFriendsList];
    if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
        //单人邮件
        personSelectVC =  [[PersonSelectVC alloc]initWithType:PersonSelectVCType_New];
    }else{
        //聊天室
        personSelectVC =  [[PersonSelectVC alloc]initWithType:PersonSelectVCType_ChangeMember];
    }
//    personSelectVC.ispushFrom_OC_Native = YES;
    personSelectVC.personVCOpenFrom = PersonVCOpenFrom_OC_OneMailChat;
    personSelectVC.chatChannel = self.currentChatChannel;
    [self.navigationController pushViewController:personSelectVC animated:YES];
    
}
#pragma mark -
#pragma mark EditBarView Delegate
-(void)editBarViewBtnPressedAction:(EditBarView *)sender{
    //组装用户信息
    
//    NSMsgItem *chatMessage = [[ServiceInterface serviceInterfaceSharedManager] createChatMessage:sender.inputTextField.text];
    
    [self sendMsg:sender.inputTextField.text];
    
    
    //输入框置空
    sender.inputTextField.text = @"";

    /**
     *  输入框逻辑 需要补充
     */
    sender.senderButton.enabled = NO;
    [self.editBarView textViewDidChange:self.editBarView.inputTextField];
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
#pragma mark - UIKeyboardNotification
-(void)passChangTextViewHeight:(CGFloat)changHeight
{
    
    self.footView.frame=CGRectMake(0, self.footView.frame.origin.y-changHeight, kMainScreenWidth, self.footView.frame.size.height+changHeight);
    self.editBarView.frame=CGRectMake(0, 0, kMainScreenWidth, self.editBarView.frame.size.height+changHeight);
     self.mailChatTableView.height = self.footView.top-self.headView.height;
    
//    self.chatScrollView.frame=CGRectMake(self.chatScrollView.origin.x, self.chatScrollView.origin.y, self.chatScrollView.frame.size.width, self.chatScrollView.frame.size.height-changHeight) ;
//    self.countryTableView.frame=CGRectMake(0, 0, self.countryTableView.frame.size.width, self.self.countryTableView.size.height-changHeight);
//    self.allianceTableView.frame=CGRectMake(self.view.frame.size.width, 0, self.allianceTableView.frame.size.width, self.allianceTableView.frame.size.height-changHeight);
    
    
//    if (changHeight>0) {
//        [self.countryTableView setContentOffset:CGPointMake(self.countryTableView.contentOffset.x, self.countryTableView.contentOffset.y+changHeight) animated:NO];
//        [self.allianceTableView setContentOffset:CGPointMake(self.allianceTableView.contentOffset.x, self.allianceTableView.contentOffset.y+changHeight) animated:NO];
//    }

}
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
        
            if (endFrame.origin.y<beginFrame.origin.y) {
                if (self.dataSourceArray.count> 1){
                     [self.mailChatTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                }

            }
       
    };
    
    [UIView animateWithDuration:duration delay:0.0f options:(curve << 16 | UIViewAnimationOptionBeginFromCurrentState) animations:animations completion:completion];
}

- (void)willShowKeyboardFromFrame:(CGRect)beginFrame toFrame:(CGRect)toFrame
{
 
    self.footView.bottom = toFrame.origin.y;
    self.mailChatTableView.height = self.footView.top-self.headView.height;
    [self.view bringSubviewToFront:self.footView];
   
 
}
#pragma mark -
#pragma mark UIScrollView Delegate

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    if (self.isHasNewMsgPushed) {
        [self reloadNewMsg];
    }else if (self.isHasLoadOldMsgPushed){
        [self reloadOldMsg];
    }
}
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (_slimeView) {
        [_slimeView scrollViewDidScroll];
    }
}

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    if (_slimeView) {
        [_slimeView scrollViewDidEndDraging];
    }
}
#pragma mark -
#pragma mark SRRefreshDelegate

- (void)slimeRefreshStartRefresh:(SRRefreshView*)refreshView{
    if (self.dataSourceArray.count== 0){
        //没有邮件消息，不让下拉刷新
        self.refreshingFlag = YES;
       
    }else{
        self.refreshingFlag = NO;
    }
    if (self.isRefreshingFlagTure == NO) {
        self.refreshingFlag = ![self tableViewBeginRefreshed];
        if (self.isRefreshingFlagTure == YES) {
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
                if(self.isRefreshingFlagTure == YES ){
                    [self tableViewEndRefreshed];
                }
                
            });
        }else{
            [self tableViewEndRefreshed];
        }
        
    }else{

        [self.currentChatChannel gettingFirstMsg];
        [_slimeView endRefresh];
    }
}
-(BOOL)tableViewBeginRefreshed{
    
    self.offsetY = self.mailChatTableView.contentSize.height;
    return [self.currentChatChannel gettingOldMsg];
    
}
-(void)tableViewEndRefreshed{
    self.refreshingFlag = NO;
    [_slimeView endRefresh];
}
#pragma mark -
#pragma mark ChatChannel Delegate

-(void)showNewMsg:(ChatChannel *)vChannle{

    if ( self.mailChatTableView.dragging== NO && self.mailChatTableView.decelerating == NO) {
        [self reloadNewMsg];
        self.hasNewMsgPushed = NO;
    }else{
        self.hasNewMsgPushed = YES;
    }
    
    if ([self.mailChattingVCDelegate respondsToSelector:@selector(mailChattingVCHasNewMsgCallBackWithChatChannel:)]) {
        [self.mailChattingVCDelegate mailChattingVCHasNewMsgCallBackWithChatChannel:vChannle];
    }
}
-(void)showOldMsg:(ChatChannel *)vChannel{
    [self tableViewEndRefreshed];
    if ( self.mailChatTableView.dragging== NO && self.mailChatTableView.decelerating == NO) {
        [self reloadOldMsg];
        self.hasLoadOldMsgPushed = NO;
    }else{
        self.hasLoadOldMsgPushed = YES;
    }
}

-(void)showscreenLockedMsg:(ChatChannel *)vChannel{
    if ( self.mailChatTableView.dragging== NO && self.mailChatTableView.decelerating == NO) {
        [self reloadOldMsg];
        self.hasLoadOldMsgPushed = NO;
    }else{
        self.hasLoadOldMsgPushed = YES;
    }
}
#pragma mark -
#pragma mark 滚动停止后，来消息处理
-(void)reloadNewMsg{
    dispatch_async(dispatch_get_main_queue(), ^{
        //更新UI操作
        NSIndexPath *tempIndexPath = [self.mailChatTableView.indexPathsForVisibleRows lastObject];
        
        NSInteger oldCount = self.dataSourceArray.count;
        self.dataSourceArray = [self.currentChatChannel.msgList mutableCopy];
        NSInteger indexCount = self.dataSourceArray.count - oldCount;
        [self.mailChatTableView reloadData];
        if (self.dataSourceArray.count > 0){
            if (oldCount == 0) {
                [self.mailChatTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
            }else{
                if  (indexCount > 0){
                    if  (tempIndexPath.row > oldCount-4){
                        [self.mailChatTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                    }
                }
            }
        }
        
    });
    self.hasNewMsgPushed = NO;
}
-(void)reloadOldMsg{
    
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        NSIndexPath *tempIndexPath =  [[self.mailChatTableView indexPathsForVisibleRows] objectAtIndex:0];
        
        //更新UI操作
        
        NSInteger oldCount = self.dataSourceArray.count;
        self.dataSourceArray = [self.currentChatChannel.msgList mutableCopy];
        NSInteger indexCount = self.dataSourceArray.count - oldCount;
        [self.mailChatTableView reloadData];
        if (tempIndexPath.row<= 3) {
            CGPoint pt =self.mailChatTableView.contentOffset;
            CGFloat boundaryValueMax= self.mailChatTableView.contentSize.height - self.mailChatTableView.frame.size.height * 0.6 - self.mailChatTableView.frame.size.height;
            if(indexCount > 0   ){
                [self.mailChatTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
                
            }
        }
        
    });
    
    
}


#pragma mark -
#pragma mark ChatCellIOSDelegate 
-(void)chatMailCellDidSelectedWithCell:(ChatCellIOS *)vChatMailCell {
    DVLog(@"%@",vChatMailCell);
    NSIndexPath  *currentIndexPath =[self.mailChatTableView indexPathForCell:vChatMailCell];
    if(currentIndexPath){
        [self.mailChatTableView reloadRowsAtIndexPaths:@[currentIndexPath] withRowAnimation:UITableViewRowAnimationNone];
    }
    
}

-(void)chatNewMailCellDidCloseKey
{
    DVLog(@"关闭新版邮件键盘");
    [self.editBarView.inputTextField resignFirstResponder];
}


#pragma mark -
#pragma mark CSMsgTableCellDelegate
-(void)resumeOldLongPress{
 
        CSMessageTableCell *tempCell =[self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
        if (tempCell) {
            [tempCell longRecognizerResumeNormal];
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
        
        if (recognizer.state == UIGestureRecognizerStateBegan && [self.dataSourceArray count] > 0)
        {
            //去掉之前的选中
            [self resumeOldLongPress];
 
           
                CGPoint location = [recognizer locationInView:self.mailChatTableView];
                NSIndexPath * indexPath = [self.mailChatTableView indexPathForRowAtPoint:location];
                CSMessageTableCell *cell = (CSMessageTableCell *)[self.mailChatTableView cellForRowAtIndexPath:indexPath];
                //                    [cell becomeFirstResponder];
                //                    [cell longRecognizerPresed];
                _menuIndexPath = indexPath;
                [self _showMenuViewController:cell  andIndexPath:indexPath messageType:cell.messageModel.messageBodyType];
            
            
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
    
 
    CSMessage *msg = vCell.messageModel.message;
    msg.sendState = CSMessageSendState_Delivering;
 
    [self.currentChatChannel chatNewMsgPushed:@[msg]];
    
    
    if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
        DVLog(@"邮件发送信息 == ");
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendMailWithMessage:msg];
    }else{
        DVLog(@"聊天室发送信息 == ");
        NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",msg.sendLocalTime];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendChatRoomMsg:msg.msg :self.currentChatChannel.channelID sendLocalTime:sendLocalTime];
    }
    
    
}
/** 点击bubble */
-(void)msgcellBubbleViewSelected:(CSBaseMessageCell *)vCell{
    if (vCell.messageModel.messageBodyType == CSMessageBodyType_RedPackge){
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
                
                    [self.currentChatChannel redPacketMsgUpdateStautsWithMessage:mi];
            }else{
                [[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict setObject:mi forKey:redPacketKey];
            }
            if ( [mi gettingRedPackageStatusWithMessage] == 1 || [mi gettingRedPackageStatusWithMessage] == 2 ||[mi gettingRedPackageStatusWithMessage ] == 3) {
                //未领取  被抢光 红包过期
                [self keyBoardClosed];
                
                
                
                CGSize winSize=[UIScreen mainScreen].bounds.size;
                float x = winSize.width * 0.08;
                float y = winSize.height * 0.05;
//                [self removeAlertRedPackageView];
//                self.alertRedPackageView = [[CSAlertRedPackageView alloc] initWithFrame:CGRectMake(x,y,winSize.width * 0.9,winSize.height * 0.9) andWithMessageModel:vCell.messageModel];
//                self.alertRedPackageView.alertRedPackageViewDelegate = self;
//                [self.view addSubview:self.alertRedPackageView];
                
                
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
    CSMessageModel *model  = [self.dataSourceArray objectAtIndex:indexPath.row];
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

    }else{
        DVLog(@"当前cell没有可点击的item操作");
    }
}

#pragma mark -
#pragma mark MenuItem Action

/** 点击坐标 */
-(void)onClickPointByPoint:(NSString *)pointStr{
    NSString *tempPointStr = [pointStr stringByReplacingOccurrencesOfString:@"：" withString:@":"];
    NSArray *poslist=[tempPointStr componentsSeparatedByString:@":"];
    NSString *posX = [poslist objectAtIndex:0];
    NSString *posY = [poslist objectAtIndex:1];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost skipPos:posX :posY];
    [[CSConvertViewManager sharedMailInfoManager] closeMailVC];
}

-(void)menuItemAction:(CustemMenuItem *)vSender{
 
}
/** 显示原文 */
-(void)menuItemShowOrginal:(id)sender{
   
        CSMessageTableCell *vCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
        vCell.messageModel.isTranslatLabelShow = NO;
        [vCell.messageModel updateMessageModelWithMsg];
        
        [self.mailChatTableView reloadRowAtIndexPath:_menuIndexPath  withRowAnimation:UITableViewRowAnimationFade];
}
/** 显示翻译 */
-(void)menuItemTranslate:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];

    if (vPressedCell.messageModel.message.translateMsg.length> 0){
        vPressedCell.messageModel.isTranslatLabelShow = YES;
        [vPressedCell.messageModel updateMessageModelWithMsg];
       [self.mailChatTableView reloadRowAtIndexPath:_menuIndexPath  withRowAnimation:UITableViewRowAnimationFade];
        
    }else{
        __block  CSMailChattingVC *weakSelf = self;
        __block CSMessageTableCell *weakCell = vPressedCell;
//        CSGoogleTranslat *  translat =[[CSGoogleTranslat alloc]init];
//        NSString *originalString = vPressedCell.messageModel.message.msg;
//        
//        NSString *localLanString = [NSString stringWithUTF8String: LocalController::shared()->getLanguageFileName().c_str()] ;
//        DVLog(@"%@",localLanString);
//        
//        [translat translationingWithText:originalString andWihtSL:@"auto" andWithTL:localLanString andResultBlock:^(ZYTGoogleTranslatHTTPStatus status, NSString *resultString, NSString *orgLangTypeString) {
//            if (status == ZYTGoogleTranslatHTTPStatus_sucess){
//                DVLog(@"%@",resultString);
//                weakCell.messageModel.message.translateMsg = resultString;
//                weakCell.messageModel.message.originalLang = orgLangTypeString;
//                if  (weakSelf.chatVCShowTableType == ChatVCShowTableType_left){
//                    NSIndexPath * tempIndexPath = [weakSelf.countryTableView indexPathForCell:vPressedCell];
//                    [weakSelf.countryTableView reloadRowAtIndexPath:tempIndexPath  withRowAnimation:UITableViewRowAnimationFade];
//                }else{
//                    NSIndexPath * tempIndexPath = [weakSelf.allianceTableView indexPathForCell:vPressedCell];
//                    [weakSelf.allianceTableView reloadRowAtIndexPath:tempIndexPath  withRowAnimation:UITableViewRowAnimationFade];
//                }
//                
//            }
//        }];
        
        
        
    }
}
/** 转盘分享 */
-(void)menuItemRotaryTableShare:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewLotteryInfoById:vPressedCell.messageModel.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 装备分享 */
-(void)menuItemEquipShare:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewEquipment:vPressedCell.messageModel.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
}
/** 联盟任务 */
-(void)menuImteAllianceTask:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    CSMessageModel *mi = vPressedCell.messageModel;
    DVLog(@"AllianceTask");
    if (mi && mi.message.channelType >= 0) {
        [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"allianceTask" :@"" :@"" :@"" :@"" :0 :TRUE];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:mi.message.channelType];
        [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    }
    
}
/** 禁言 */
-(void)menuItemBanned:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    CSMessageModel *mi = vPressedCell.messageModel;
    BanPopUpController *banPopUpController = [[BanPopUpController alloc] initWithNibName:@"BanPopUpController" bundle:nil];
    
    [banPopUpController settingInitDataWithMessageModel:mi];
    [self.view addSubview:banPopUpController.view];
    [self.view bringSubviewToFront:banPopUpController.view];
}
/** 解除禁言 */
-(void)menuItemClosedBanned:(id)sender{
    DVLog(@"menuItemClosedBanned");
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    CSMessageModel *mi = vPressedCell.messageModel;
    
    if (mi && mi.message.uid.length > 0) {
        NSString *nameString = mi.currentUser.userName;
        if  (nameString.length == 0){
            nameString = mi.message.uid;
        }
        [[ChatServiceController chatServiceControllerSharedManager].gameHost unBan:mi.message.uid :mi.currentUser.userName];
    }
}
/** 屏蔽 */
-(void)menuItemBlock:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    CSMessageModel *mi = vPressedCell.messageModel;
    NSString *nameString = mi.currentUser.userName;
    if  (nameString.length == 0){
        nameString = mi.message.uid;
    }
    [[ChatServiceController chatServiceControllerSharedManager].gameHost block:mi.message.uid :nameString];
    
}
/** 解除屏蔽 */
-(void)menuItemCloseBlock:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
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
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
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
    alertView.tag = 102;
    alertView.viewType = CONTENTOFTHEREPORT;
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:tipStr];
    alertView.alertViewDelegate = self;
    [self.view addSubview:alertView];
    [self.view bringSubviewToFront:alertView];
}
/** 复制消息 */
-(void)menuItemCopyed:(id)sender{
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
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
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
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
    CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
    CSMessageModel *mi = vPressedCell.messageModel;
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost viewRallyInfoByStr:mi.message.attachmentId];
    [[CSConvertViewManager sharedMailInfoManager] chatWindowHidden];
    
}

#pragma mark -
#pragma mark CSAlertView Delegate
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView{
    if (vAlertView.tag == 102){
        CSMessageTableCell *vPressedCell = [self.mailChatTableView cellForRowAtIndexPath:_menuIndexPath];
        CSMessageModel *mi = vPressedCell.messageModel;
        [[ChatServiceController chatServiceControllerSharedManager].gameHost contentofReportWithUid:mi.message.uid withContent:mi.message.msg];
        
    }
}

@end
