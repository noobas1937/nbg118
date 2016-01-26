//
//  CSMailListViewController.m
//  IF
//
//  Created by 张彦涛 on 15/8/19.
//
//

#import "CSMailListViewController.h"
#import "ServiceInterface.h"
#import "TopHeadView.h"
#import "ChatChannel.h"
#import "ChannelManager.h"
#import "MailListCell.h"
#import "MailInfoDataModel.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatCellFrame.h"
#import "ChatServiceCocos2dx.h"
#import "ChattingVC.h"
#import "CSMailChattingVC.h"
#import "CSSystemMailListVC.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "UserManager.h"
#import "NSString+Cocos2dHelper.h"
#import "CSMailChattingVC.h"
#import "CSMailBottomOperationToolBarView.h"
#import "CSCommandManager.h"
#import "NSArray+SortHelper.h"

#import "CSMessageModel.h"
#import "CSMessage.h"
@interface CSMailListViewController ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate,MailChattingVCDelegate,MailBottomToolBarDelegate,MailListCellDelegate,SlideCellProtocol>
{
    BOOL topButtonSelected;
    BOOL allMemberSelected;
    
    MailListCell * openCell;
    BOOL isOpenScroCell;
    
}
@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong) UITableView *mailListTableView;
@property (nonatomic,strong)NSMutableArray *dataSourceArray;
@property (nonatomic,strong) CSMailBottomOperationToolBarView * mailBottomToolBarView;

@property (nonatomic,strong) NSMutableArray * selectedArray;
@end

@implementation CSMailListViewController

-(instancetype)initWithChatMailType:(MailListVCChatType)vChatTpye{
    
    self = [super init];
    if  (self){
        self.mailListVCChatType = vChatTpye;
    }
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    
    isOpenScroCell=NO;

    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
    [self.footView addSubview:self.mailBottomToolBarView];
    [self.headView addSubview:self.topHeaderView];
    [self.view addSubview:self.mailListTableView];
    [self gettingChannelList];
    [self.mailListTableView reloadData];
    topButtonSelected = NO;
    allMemberSelected = NO;
    
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(notificationForChatChannelChangedWithNotify:) name:kChatChannelUnReadCountChanged object:nil];
     
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:kChatChannelUnReadCountChanged object:nil];
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
        UIView *footView =[[UIView alloc]init];
          footView.backgroundColor = [UIColor grayColor];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            footView.frame = CGRectMake(0, kMainScreenHeight  , kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            footView.frame = CGRectMake(0, kMainScreenHeight  , kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _footView = footView;
    }
    return _footView;
}
-(TopHeadView*)topHeaderView{
    if (_topHeaderView ==nil) {
        TopHeadView *view = [TopHeadView  topHeadViewWithRightButtonShowFlag:YES andShowRightButtonType:TopHeadViewRightButtonType_ViewEdit];
        view.titleNameLabel.text = [NSString stringWithMultilingualWithKey:@"105384"];//105384=信息
        view.frame = self.headView.frame;
         [view.rightEditButton setImage:[UIImage imageNamed:@"Btn_Mail_Editor"] forState:UIControlStateNormal];
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}
-(UITableView *)mailListTableView{
    if (_mailListTableView == nil) {
        UITableView *tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, self.topHeaderView.bottom, kMainScreenWidth,self.footView.top - self.headView.height ) style:UITableViewStylePlain];
        tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        tableView.delegate = self;
        tableView.dataSource = self;
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"mail_list_bg"]];
         tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        tableView.separatorColor = [UIColor brownColor];
        _mailListTableView = tableView;
    }
    return  _mailListTableView;
}
-(NSMutableArray *)dataSourceArray{
    if (_dataSourceArray == nil) {
        
        _dataSourceArray=[NSMutableArray array];
    }
    return _dataSourceArray;
}
-(CSMailBottomOperationToolBarView *)mailBottomToolBarView{
    if  (_mailBottomToolBarView == nil){
        _mailBottomToolBarView =[CSMailBottomOperationToolBarView mailBottomOperationToolBarViewWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height) ];
    }
    
    _mailBottomToolBarView.mailBottomToolBarDelegate = self;
    [_mailBottomToolBarView settingDeleteButtonShow:YES andgitButtonShow:NO andStarButtonShow:NO];
    return _mailBottomToolBarView;
}
-(NSMutableArray *)selectedArray{
    if  (_selectedArray == nil){
        _selectedArray = [NSMutableArray array];
    }
    return _selectedArray;
}
#pragma mark -
#pragma mark data

-(void)gettingChannelList{
    NSMutableArray *mutArray =[NSMutableArray array];
    NSArray *allChannelArray =[[ChannelManager sharedChannelManager].channel_map allValues];
    for (ChatChannel *channel in allChannelArray) {
        if (channel.channelType == IOS_CHANNEL_TYPE_USER || channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
            if ([channel settingMsgListWithLastMsgFromDB]){
                id object =[channel.msgList lastObject];
                if ([object isKindOfClass:[CSMessageModel class]]){
                    channel.contentsString = [object showContentsString];
                    channel.latestTime = [[(CSMessageModel *)object message]createTime] ;
                }else if([object isKindOfClass:[ChatCellFrame class]]){
                    channel.contentsString = [[channel.msgList lastObject] chatMessage].showMsg;
                    channel.latestTime = [[channel.msgList lastObject] chatMessage].createTime;
                }
                
            }
            
            NSString *channelUserUID =channel.channelID;
            if (channel.channelType == IOS_CHANNEL_TYPE_USER){
                if ([channel.channelID hasSuffix:@"@mod"]){
                     channelUserUID  =  [channelUserUID stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
                }
                NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:channelUserUID];
                DVLog(@"%@",[user printAllPropertys]);
                if(user ){
                    NSString *titleString =@"";
                    if (user.asn.length> 0){
                        titleString =[titleString stringByAppendingFormat:@"(%@)",user.asn];
                    }
                    titleString = [titleString stringByAppendingFormat:@"%@",user.userName];
                    channel.nameString = titleString;
                    channel.faceImageString = user.headPic;
                    if (user.headPicVer> 0){
                        channel.faceCustemString = [NSString gettingCustomHeadPic:user.uid intValue:user.headPicVer];
                    }
                }else{
                    channel.nameString = channelUserUID;
                    channel.faceImageString = @"g015";
                }
            }else if (channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
                
            }
            
            /** 这里非mod有问题 */
            if (_mailListVCChatType== MailListVCChatType_Mod){
                if (channel.channelType == IOS_CHANNEL_TYPE_USER){
                    if ([channel.channelID hasSuffix:@"@mod"]){
                        [mutArray addObject:channel];
                    }
                }
            }else{
                if (![channel.channelID hasSuffix:@"@mod"]){
                    [mutArray addObject:channel];
                }
                
            }
            
        }
    }
 
    if(mutArray.count> 0){
        self.dataSourceArray =[NSArray chatChannelSortingDataSource:mutArray];
        
    }else{
        UILabel *showlable = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 100, 50)];
        showlable.text = [NSString stringWithMultilingualWithKey:@"105385"];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
           showlable.font = [UIFont systemFontOfSize:22.0];
        }else{
          showlable.font = [UIFont systemFontOfSize:12.0];
        }
        
        [showlable sizeToFit];
        showlable.center = CGPointMake(self.mailListTableView.center.x, self.mailListTableView.center.y-50);
        [self.mailListTableView addSubview:showlable];

    }
 
}
#pragma mark -
#pragma mark action
-(void)cancelEditingBar{
    _topHeaderView.rightEditButton.selected = NO;
    [_mailBottomToolBarView settingStartStatus];
    [self mailBottomToolBarShow:NO];
    
}

-(void)mailBottomToolBarShow:(BOOL)vIsShow{
    if (vIsShow){
        // 显示
        topButtonSelected = YES;
        self.footView.top = kMainScreenHeight - self.footView.height;
    }else{
        topButtonSelected = NO;
        self.footView.top = kMainScreenHeight  ;
        
        allMemberSelected = NO;//是否选择了全部选择按钮
        [_mailBottomToolBarView settingStartStatus];
        for (ChatChannel   *channel in self.dataSourceArray) {
            channel.selected = NO;
        }
        [self.selectedArray removeAllObjects];
        
    }
    self.mailListTableView.height = self.footView.top - self.headView.height;
    [self.mailListTableView reloadData];
}

#pragma mark -
#pragma mark TopHeadView delegate
-(void)clickButton{
    [self.navigationController popToRootViewControllerAnimated:YES];

}
-(void)rightButtonAction:(UIButton *)sender{
    sender.selected = !sender.selected;
//     topButtonSelected = sender.selected;
    if (sender.selected) {
        
        [self mailBottomToolBarShow:YES];
//        self.footView.top = kMainScreenHeight - self.footView.height;
       
    }else{
        [self mailBottomToolBarShow:NO];
//        self.footView.top = kMainScreenHeight  ;
//        allMemberSelected = NO;
//        [_mailBottomToolBarView settingStartStatus];
//        for (ChatChannel *channel in self.dataSourceArray) {
//            channel.selected = NO;
//        }
//        [self.selectedArray removeAllObjects];
    }
   
}
#pragma mark -
#pragma mark tableView Delegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSourceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    MailListCell *cell = [MailListCell mailListCellWithTableView:tableView];
    cell.showSelectedButton =topButtonSelected;
    cell.chatChannel = [self.dataSourceArray objectAtIndex:indexPath.row];
    cell.mailListCellDelegate = self;
    cell.delegate=self;
//    [cell initView];
    if (allMemberSelected == NO) {
        cell.chatChannel.selected = NO;
    }
//    [cell settingSelectedButtonSelected:allMemberSelected];
//    DVLog(@"%@",indexPath);
//    DLog(@"%@",[cell.chatChannel printAllPropertys]);
    return cell;
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return [MailListCell mailListCellRowHeight];
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    
    if (isOpenScroCell) {
        //cell左滑状态
            [tableView deselectRowAtIndexPath:indexPath animated:NO];
        [openCell closeUpCell:openCell];
        isOpenScroCell=NO;
        
    }else{
        
        [tableView deselectRowAtIndexPath:indexPath animated:YES];
        [self cancelEditingBar];
        
        
        ChatChannel *channel = [self.dataSourceArray objectAtIndex:indexPath.row];
    //    DVLog(@"%@",[channel printAllPropertys]);
        CSMailChattingVC *mailChatVC =[[CSMailChattingVC alloc]init];
        mailChatVC.readStatusBlock= ^(void){
            [self.mailListTableView reloadData];
        };
        mailChatVC.mailChattingVCDelegate = self;
        mailChatVC.currentChatChannel = channel;
    //    [channel gettingFirstMsg];
        
        NSString *currentUidString = channel.channelID;
       if ([channel.channelID hasSuffix:@"@mod"]){
           currentUidString = [currentUidString stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
            [UserManager sharedUserManager].currentMail.type = 23;
       }else{
            [UserManager sharedUserManager].currentMail.type = 0;
       }
        [UserManager sharedUserManager].currentMail.opponentUid = currentUidString;
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            [UserManager sharedUserManager].currentMail.myUid = [[channel.msgList lastObject] messageId];
        }else{
            [UserManager sharedUserManager].currentMail.myUid = [[channel.msgList lastObject] chatMessage].mailId;
        }
      
        [UserManager sharedUserManager].currentMail.opponentName = channel.nameString;
       
        
      
        [self.navigationController pushViewController:mailChatVC animated:YES];
       
        
    }
}





#pragma mark -
#pragma mark MailChattingVCDelegate
-(void)mailChattingVCHasNewMsgCallBackWithChatChannel:(ChatChannel *)opendChannel{
    ChatChannel *tempChannel = opendChannel;
//    for (ChatChannel *channel in self.dataSourceArray) {
//        if  ([channel.channelID isEqualToString:opendChannel.channelID]){
//            tempChannel = channel;
//            break;
//        }
//    }
    if  (tempChannel){
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New) {
             tempChannel.contentsString = [[tempChannel.msgList lastObject] showContentsString];
        }else{
             tempChannel.contentsString = [[tempChannel.msgList lastObject] chatMessage].showMsg;
        }
        
        
//        if (tempChannel.channelType == IOS_CHANNEL_TYPE_USER){
//            NSString *channelUserUID = @"";
//            if ([tempChannel.channelID hasSuffix:@"@mod"]){
//                channelUserUID  =  [channelUserUID stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
//            }
//            NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:channelUserUID];
//            DVLog(@"%@",[user printAllPropertys]);
//            if(user ){
//                tempChannel.nameString = user.userName;
//                tempChannel.faceImageString = user.headPic;
//                if (user.headPicVer> 0){
//                    tempChannel.faceCustemString = [NSString gettingCustomHeadPic:user.uid intValue:user.headPicVer];
//                }
//            }else{
//                tempChannel.nameString = channelUserUID;
//                tempChannel.faceImageString = @"g015";
//            }
//        }else if (tempChannel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
//            
//        }
        [self.mailListTableView reloadData];
    }
}
#pragma mark -
#pragma mark MailBottomToolBarDelegate
-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView selectAllButtonActionWithButton:(UIButton *)sender{
    allMemberSelected = !allMemberSelected;
     [self.selectedArray removeAllObjects];
    if(sender.selected ){
        
        for (ChatChannel *channel in self.dataSourceArray) {
            channel.selected = YES;
            [self.selectedArray addObject:channel];
        }
    }else{
        for (ChatChannel *channel in self.dataSourceArray) {
            channel.selected = NO;
            
        }
    }
    [self.mailListTableView reloadData];
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView giftButtonActionWithButton:(UIButton *)sender{
    DVLog(@"礼物按钮  selectedArr %@",self.selectedArray);
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView starButtonActionWithButton:(UIButton *)sender{
    DVLog(@"标星按钮  selectedArr %@",self.selectedArray);
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView delectButtonActionWithButton:(UIButton *)sender{
    DVLog(@"删除按钮  selectedArr %@",self.selectedArray);
    [self.dataSourceArray removeObjectsInArray:self.selectedArray];
    [self.mailListTableView reloadData];
    [[CSCommandManager sharedCommandManager]deletingChannelsWithChannelIDArray:self.selectedArray];
    [[ChannelManager sharedChannelManager]deleteChannelWithChannelArray:self.selectedArray];
    [self cancelEditingBar];
    
}
#pragma mark -
#pragma mark Notify Action
-(void)notificationForChatChannelChangedWithNotify:(NSNotification*)vNotify{
    NSDictionary *userInfo =vNotify.userInfo;
    
    DVLog(@"notifyUserInfo :%@",userInfo);

    NSString *channelIdString =[userInfo objectForKey:@"channelID"];
    if (channelIdString.length > 0){
        ChatChannel *tempChannel = nil;
        for (ChatChannel *channel in _dataSourceArray) {
            if ([channel.channelID isEqualToString:channelIdString]){
                tempChannel = channel;
                break;
            }
        }
        if (tempChannel == nil){
            //新聊天
            tempChannel =[[ChannelManager sharedChannelManager]gettingChannelInfo:channelIdString];
            if (tempChannel ){
                if ([tempChannel settingMsgListWithLastMsgFromDB]){
                    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                         tempChannel.contentsString = [(CSMessageModel *)[tempChannel.msgList lastObject] showContentsString];
                    }else{
                         tempChannel.contentsString = [[tempChannel.msgList lastObject] chatMessage].showMsg;
                    }
                   
                }
                
                if (tempChannel.channelType == IOS_CHANNEL_TYPE_USER){
                    NSString *userUID =[tempChannel.channelID stringByReplacingOccurrencesOfString:@"@mod" withString:@""];
                    NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:userUID];
               
                    if(user ){
                        tempChannel.nameString = user.userName;
                        tempChannel.faceImageString = user.headPic;
                        if (user.headPicVer> 0){
                            tempChannel.faceCustemString = [NSString gettingCustomHeadPic:user.uid intValue:user.headPicVer];
                        }
                    }else{
                        tempChannel.nameString = tempChannel.channelID;
                        tempChannel.faceImageString = @"g015";
                    }
                }

                [_dataSourceArray addObject:tempChannel];
            }
        }
    }
    self.dataSourceArray =[NSArray chatChannelSortingDataSource:self.dataSourceArray];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.mailListTableView reloadData];
    });
   
}
#pragma mark -
#pragma mark MailListCellDelegate
-(void)mailListCellSelectedButtonActionWithCell:(MailListCell *)vCell andWithButton:(UIButton *)sender{
    if (YES == vCell.chatChannel.selected ){
        [self addMember:vCell.chatChannel];
    }else{
        [self subMember:vCell.chatChannel];
    }
    DVLog(@"selectedArr :%@",self.selectedArray);
}


#pragma mark -
#pragma mark 当前已选择人员的array修改
-(void)addMember:(ChatChannel *)vChannel{
    ChatChannel *tempChannel= nil;
    for (ChatChannel *selectedChannel in self.selectedArray) {
        if  ([selectedChannel.channelID isEqualToString:vChannel.channelID]){
            tempChannel = selectedChannel;
            break;
        }
    }
    if (tempChannel == nil){
        [self.selectedArray addObject:vChannel];
    }
}
-(void)subMember:(ChatChannel *)vChannel{
//    ChatChannel *tempChannel= nil;
//    for (ChatChannel *selectedChannel in self.selectedArray) {
//        if  ([selectedChannel.channelID isEqualToString:vChannel.channelID]){
//            tempChannel = selectedChannel;
//            break;
//        }
//    }
//    if (vChannel ){
        [self.selectedArray removeObject:vChannel];
//    }
}
//
//- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
//    //打开编辑状态，不允许删除
//    return !topButtonSelected;
//}
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
    //打开编辑状态，不允许删除
    
    //selectedButtonShowing
    //return ![self isSelectedButtonShowing];
    return NO;
}
-(BOOL)Mailopen:(MailListCell *)cell
{
    if (isOpenScroCell) {
        isOpenScroCell=NO;
        [openCell closeUpCell:openCell];
        return NO;
    }else{
        isOpenScroCell=YES;
        openCell=cell;
        return YES;
    }
}
-(BOOL)Mailclose:(MailListCell *)cell
{
    if (isOpenScroCell) {
        isOpenScroCell=NO;
        [openCell closeUpCell:openCell];
        return YES;
    }else{
        isOpenScroCell=NO;
        return NO;
    }
}
-(BOOL)isSelectedButtonShow
{
    return topButtonSelected;
}
-(void)cellbuttonClick:(UIButton*)button andWithMailCategroyListCell:(MailListCell *)vCell
{
    NSIndexPath * indexPath =[self.mailListTableView indexPathForCell: vCell];
    ChatChannel *channel = vCell.chatChannel;
    NSUInteger row = [indexPath row];
    if(button.tag == 1){
        //标记已读
        isOpenScroCell=NO;
        [openCell closeUpCell:openCell];//关闭
        channel.unreadCount = 0;
        [self.mailListTableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        if (channel.channelType == IOS_CHANNEL_TYPE_USER){
            
            [[CSCommandManager sharedCommandManager]settingReadStatus_Chat_WithChatFromUid:channel.channelID andWithModFlag:NO];
        }
         [channel  channelMarkToReadStatus];

    }else if(button.tag == 2){
        isOpenScroCell=NO;
   
        [self.dataSourceArray removeObject:channel];
        [self.mailListTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
        [[ChannelManager sharedChannelManager]deletingChatChannel:channel];
        [[CSCommandManager sharedCommandManager]deletingChatChannelWithChannelID:channel.channelID];
 
    }
}

@end