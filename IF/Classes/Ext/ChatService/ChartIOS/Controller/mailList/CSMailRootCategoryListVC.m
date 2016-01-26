//
//  CSMailRootCategoryListVC.m
//  IF
//
//  Created by 张彦涛 on 15/10/9.
//
//

#import "CSMailRootCategoryListVC.h"
#import "TopHeadView.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
#import "CSMailCategroyListTableCell.h"
#import "UIImage+resized.h"
#import "ChannelManager.h"
#import "CSMailCategoryListModel.h"
#import "CSSystemMailListVC.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "NSArray+SortHelper.h"
#import "CSMailListViewController.h"
#import "CSCommandManager.h"
#import "MonsterMailData.h"
#import "MailDataManager.h"

@interface CSMailRootCategoryListVC ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate, UIViewControllerPreviewingDelegate>

@property (strong, nonatomic)  UIView *headView;
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong) UITableView * mailCategoryListTableView;
@property (nonatomic,strong) NSMutableArray * firstListDataArray;
@property (nonatomic,strong) NSMutableArray *secondListDataArray;


@end

@implementation CSMailRootCategoryListVC

-(instancetype)init{
    self = [super init];
    if (self) {
        [self _initData];
    }
    return self;
}

-(void)_initData{
    [[ChannelManager sharedChannelManager] chechAllUnreadCount];
   
    ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingChannelInfo:Mail_ChannelID_Message];
    if (channel == nil) {
        [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:Mail_ChannelID_Message];
        channel =[[ChannelManager sharedChannelManager] gettingChannelInfo:Mail_ChannelID_Message];
    }
    
    CSMailCategoryListModel *chatCategoryModel = [[CSMailCategoryListModel alloc]init];
    chatCategoryModel.categetyChannel  = channel;
    chatCategoryModel.headImageString   =@"mail_list_main_icon_chat";
    chatCategoryModel.titleString = [NSString stringWithMultilingualWithKey:@"105384"];//105384=信息
    chatCategoryModel.showIndexSequence = 1;
    chatCategoryModel.unReadCount = channel.unreadCount;
    [self.firstListDataArray addObject:chatCategoryModel];

    
    NSArray *channelIdArray =@[Mail_ChannelID_Message,Mail_ChannelID_Alliance,Mail_ChannelID_Fight,Mail_ChannelID_Studio,Mail_ChannelID_System,Mail_ChannelID_Mod,Mail_ChannelID_Resource,Mail_ChannelID_Monster];
    for (NSString *channelIdString  in channelIdArray) {
        ChatChannel *channel =[[ChannelManager sharedChannelManager]gettingChannelInfo:channelIdString];
        if (channel == nil) {
            [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:channelIdString];
            channel =[[ChannelManager sharedChannelManager]gettingChannelInfo:channelIdString];
        }
        CSMailCategoryListModel *tempModel = [[CSMailCategoryListModel alloc]init];
        tempModel.categetyChannel = channel;
        //列表1
        if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_System]){
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"101207"];//系统  101207
            tempModel.categetyChannel.faceImageString = @"mail_list_main_icon_system";
            tempModel.showIndexSequence = 5;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
            tempModel.unReadCount = channel.unreadCount;
            [self.firstListDataArray addObject:tempModel];
        }
        else if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Studio]){
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"103731"];// 列王的纷争游戏工作室  103731
            tempModel.categetyChannel.faceImageString = @"mail_list_main_icon_studio";
            tempModel.showIndexSequence = 4;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
             tempModel.unReadCount = channel.unreadCount;
            [self.firstListDataArray addObject:tempModel];
        }
        else if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Fight]){
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"105519"];//战斗报告 105519
            tempModel.categetyChannel.faceImageString = @"mail_list_main_icon_battle";
            tempModel.showIndexSequence = 3;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
             tempModel.unReadCount = channel.unreadCount;
            [self.firstListDataArray addObject:tempModel];
        }
        else if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Alliance]){
            tempModel.categetyChannel.nameString =  [NSString stringWithMultilingualWithKey:@"140036"];;//联盟 140036
            tempModel.categetyChannel.faceImageString = @"mail_list_main_icon_invite";
            tempModel.showIndexSequence =2;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
             tempModel.unReadCount = channel.unreadCount;
            [self.firstListDataArray addObject:tempModel];
        }
        
        //列表2
        if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Mod]){
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"132000"]; // 132000 = 联系MOD
            tempModel.categetyChannel.faceImageString = @"mail_list_main_icon_mod";
            tempModel.showIndexSequence = 1;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
            tempModel.unReadCount = channel.unreadCount;
            [self.secondListDataArray addObject:tempModel];
        }
        else if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Resource]){
            
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"105516"];//资源采集报告    105516
            tempModel.categetyChannel.faceImageString = @"Resource Gathering Report";
            tempModel.showIndexSequence = 2;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
            tempModel.unReadCount = channel.unreadCount;
            [self.secondListDataArray addObject:tempModel];
            
        }else if ([tempModel.categetyChannel.channelID isEqualToString:Mail_ChannelID_Monster]){
            
            tempModel.categetyChannel.nameString = [NSString stringWithMultilingualWithKey:@"103715"];//怪物报告    103715
            tempModel.categetyChannel.faceImageString = @"Monster Report";
            tempModel.showIndexSequence = 3;
            tempModel.headImageString = channel.faceImageString;
            tempModel.titleString = channel.nameString;
            tempModel.unReadCount = channel.unreadCount;
            [self.secondListDataArray addObject:tempModel];
        }

    }
   
}
-(void)viewDidLoad{
    [super viewDidLoad];
    _firstListDataArray = [NSArray mailCategoryListModelSortingForIndexSeqWithArray:_firstListDataArray];
    _secondListDataArray =[NSArray mailCategoryListModelSortingForIndexSeqWithArray:_secondListDataArray];
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.headView addSubview:self.topHeaderView];
    [self.view addSubview:self.mailCategoryListTableView];
    
    [[NSNotificationCenter defaultCenter]addObserver:self
                                            selector:@selector(notificationForMailChannelChangedWithNotify:)
                                                name:kMailChannelUnReadCountChanged
                                              object:nil];
    
    if( self.traitCollection.forceTouchCapability == UIForceTouchCapabilityAvailable)
    {
        
        [self registerForPreviewingWithDelegate:self sourceView:self.view];
    }

        [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(notificationForChatChannelChangedWithNotify:) name:kChatChannelUnReadCountChanged object:nil];

}
-(void)dealloc{
    self.headView = nil;
    self.topHeaderView = nil;
    self.mailCategoryListTableView = nil;
    [[NSNotificationCenter defaultCenter]removeObserver:self name:kMailChannelUnReadCountChanged object:nil];
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

-(TopHeadView*)topHeaderView{
    if (_topHeaderView ==nil) {
        TopHeadView *view = [TopHeadView  topHeadViewWithRightButtonShowFlag:YES andShowRightButtonType:TopHeadViewRightButtonType_ViewEdit];
        view.titleNameLabel.text = [NSString stringWithMultilingualWithKey:@"115030"];//邮件 115030
        view.frame = self.headView.frame;
//        view.rightButton.hidden = YES;
//        [view.rightButton setImage:[UIImage imageNamed:@"mail_btn_write_mail"] forState:UIControlStateNormal];
//        [view.rightButton setImage:[UIImage imageNamed:@"mail_btn_complete"] forState:UIControlStateSelected];
// 
//        view.rightButton.hidden = NO;
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}


-(UITableView *)mailCategoryListTableView{
    if (_mailCategoryListTableView == nil) {
        UITableView *tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height) style:UITableViewStylePlain];
        
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage resizableImage:@"mail_list_bg"]];
        tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        
//        tableView.separatorColor = [UIColor brownColor];
        tableView.dataSource = self;
        tableView.delegate = self;
        _mailCategoryListTableView = tableView;
    }
    return _mailCategoryListTableView;
}
-(NSMutableArray *)secondListDataArray{
    if (_secondListDataArray == nil) {
        _secondListDataArray =[NSMutableArray array];
    }
    return _secondListDataArray;
}
-(NSMutableArray *)firstListDataArray{
    if (_firstListDataArray == nil) {
        _firstListDataArray= [NSMutableArray array];
    }
    return _firstListDataArray;
}
#pragma mark -
#pragma mark TopHeadView delegate
-(void)clickButton{
    [[ServiceInterface serviceInterfaceSharedManager] hideMailListIOS];
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
    
}
-(void)rightButtonAction:(UIButton *)sender{
    sender.selected = !sender.selected;
    DVLog(@"在这里调用打开c++邮件创建的页面");
}

#pragma mark -
#pragma mark tableView Delegate
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return 2;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    if (section == 0) {
         return _firstListDataArray.count;
    }else if(section == 1){
         return _secondListDataArray.count;
    }else{
        return 0;
    }
   
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    CSMailCategroyListTableCell * cell = [CSMailCategroyListTableCell mailCategroyListTableCellWithTableView:tableView];
    if (indexPath.section == 0) {
         cell.mailCategoryListModel = [_firstListDataArray objectAtIndex:indexPath.row];
    }else {
         cell.mailCategoryListModel = [_secondListDataArray objectAtIndex:indexPath.row];
    }
   
    return cell;
}
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:( NSIndexPath *)indexPath{
     [tableView deselectRowAtIndexPath:indexPath animated:YES];
    CSMailCategoryListModel *tempCategoryListModel ;
    if (indexPath.section == 0) {
         tempCategoryListModel= [_firstListDataArray objectAtIndex:indexPath.row];
    }else{
         tempCategoryListModel= [_secondListDataArray objectAtIndex:indexPath.row];
    }

    ChatChannel *channel = tempCategoryListModel.categetyChannel;
    int count =[[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[MailInfoDataModel getTableName] withModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ChannelID ='%@')",channel.channelID]];
    channel.msgList = [[ChannelManager sharedChannelManager]gettingSystemMailArrayWithChannelIDFromDB:channel.channelID andGetCount:count];
 
    
        if ([channel.channelID isEqualToString:Mail_ChannelID_Message] ) {
            CSMailListViewController *mailChatListVC =[[CSMailListViewController alloc]init];
    
            [self.navigationController pushViewController:mailChatListVC animated:YES];

        }else if ([channel.channelID isEqualToString:Mail_ChannelID_Resource] ) {
            for (MailInfoDataModel *mode in channel.msgList) {
                
                if (!mode.maildata) {
                    mode.maildata = [[ResourceMailData alloc] initWithAddMailData:mode];
                }
            }
           
            MailData *data = [channel mergeResourceMailData];
            MailInfoDataModel *mailInfoModel =[channel.msgList lastObject];
            [[CSCommandManager sharedCommandManager]settingReadStatus_Banched_WithMailType:mailInfoModel.type andWithModFlag:NO andWithMailTypes:@""];
            if (data) {
                [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:data];
            }
        }else if([channel.channelID isEqualToString:Mail_ChannelID_Monster]){
            for (MailInfoDataModel *mode in channel.msgList) {
                if (!mode.maildata) {
                    mode.maildata = [[MonsterMailData alloc] initWithAddMailData:mode];
                }
            }
            
            MailData *data = [channel mergeMonsterMailData];
            MailInfoDataModel *mailInfoModel =[channel.msgList lastObject];
            [[CSCommandManager sharedCommandManager]settingReadStatus_Banched_WithMailType:mailInfoModel.type andWithModFlag:NO andWithMailTypes:@""];
            
            if (data) {
                [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:data];
            }
        }else{
            [self parseModeByChannel:(ChatChannel*) channel];
            [self pushCSSystemMailListVCByChannel:channel];
        }
}

-(void) parseModeByChannel:(ChatChannel*) channel
{
    for (MailInfoDataModel *mode in channel.msgList) {
        mode.maildata = [MailDataManager mailDataWithMailInfoData:mode];
    }
}

-(CSSystemMailListVC*) getCSSystemMailListVCByChannel:(ChatChannel*) channel
{
    CSSystemMailListVC *sysMailListVC =[[CSSystemMailListVC alloc]init];
    sysMailListVC.chatChannel = channel;
    
    return sysMailListVC;
}

-(void) pushCSSystemMailListVCByChannel:(ChatChannel*) channel
{
    CSSystemMailListVC *sysMailListVC =[[CSSystemMailListVC alloc]init];
    sysMailListVC.chatChannel = channel;
    [self.navigationController pushViewController:sysMailListVC animated:YES];
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    if (section == 0) {
        return 0;
    }else{
        return 20;
    }
}
- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    if (section == 0) {
        return 0;
    }else{
        return 0;
    }
}
- (nullable UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    UIView  *headerView =[[UIView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, 20)];
    headerView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"mail_list_divider_title_bg"]];
    UILabel *titleLabel=[[UILabel alloc]initWithFrame:CGRectMake(10, 0, 100, 20)];
    titleLabel.font = [UIFont systemFontOfSize:12];
    titleLabel.backgroundColor = [UIColor clearColor];
    titleLabel.text =  [NSString stringWithMultilingualWithKey:@"105568"];//105568=报告
    [headerView addSubview:titleLabel];
    return headerView;
}

#pragma mark -
#pragma mark Notify Action
-(void)notificationForMailChannelChangedWithNotify:(NSNotification*)vNotify{
    NSDictionary *userInfo =vNotify.userInfo;
    
    DVLog(@"notifyUserInfo :%@",userInfo);
    NSString *channelIdString =[userInfo objectForKey:@"channelID"];
    ChatChannel * channel = [[ChannelManager sharedChannelManager] gettingChannelInfo:channelIdString];
    if([channelIdString isEqualToString:Mail_ChannelID_Alliance]){
        CSMailCategoryListModel *chatCategoryModel = [self.firstListDataArray objectAtIndex:1];
        chatCategoryModel.unReadCount = channel.unreadCount;
        
    }
    else if([channelIdString isEqualToString:Mail_ChannelID_Fight]){
        CSMailCategoryListModel *chatCategoryModel = [self.firstListDataArray objectAtIndex:2];
        chatCategoryModel.unReadCount = channel.unreadCount;
    }
    else if([channelIdString isEqualToString:Mail_ChannelID_Studio]){
        CSMailCategoryListModel *chatCategoryModel = [self.firstListDataArray objectAtIndex:3];
        chatCategoryModel.unReadCount = channel.unreadCount;
    }
    else if([channelIdString isEqualToString:Mail_ChannelID_System]){
        CSMailCategoryListModel *chatCategoryModel = [self.firstListDataArray objectAtIndex:4];
        chatCategoryModel.unReadCount = channel.unreadCount;
    }else{
        for (CSMailCategoryListModel *model in _secondListDataArray) {
            if ([model.categetyChannel.channelID isEqualToString:channelIdString]) {
                model.unReadCount = channel.unreadCount;
            }
        }
    }

    
        [self.mailCategoryListTableView reloadData];
  
    
}


- (nullable UIViewController *)previewingContext:(id <UIViewControllerPreviewing>)previewingContext viewControllerForLocation:(CGPoint)point
{

    CGPoint location = [self.mailCategoryListTableView convertPoint:point fromView:self.view];
    
    NSIndexPath * indexPath = [self.mailCategoryListTableView indexPathForRowAtPoint:location];
    
    if (indexPath.row == 0)
    {
        return nil;
    }
    
    CSMailCategoryListModel *tempCategoryListModel ;
    if (indexPath.section == 0) {
        tempCategoryListModel= [_firstListDataArray objectAtIndex:indexPath.row];
    }else{
        tempCategoryListModel= [_secondListDataArray objectAtIndex:indexPath.row];
    }
    
    ChatChannel *channel = tempCategoryListModel.categetyChannel;
    
    channel.msgList = [[ChannelManager sharedChannelManager]gettingSystemMailArrayWithChannelIDFromDB:tempCategoryListModel.categetyChannel.channelID andGetCount:100];
    
    
    if ([channel.channelID isEqualToString:Mail_ChannelID_Message] ) {
        CSMailListViewController *mailChatListVC =[[CSMailListViewController alloc]init];
        
        return mailChatListVC;
        
    }else if ([channel.channelID isEqualToString:Mail_ChannelID_Resource] ) {
//        for (MailInfoDataModel *mode in channel.msgList) {
//            
//            if (!mode.maildata) {
//                mode.maildata = [[ResourceMailData alloc] initWithAddMailData:mode];
//            }
//        }
//        
//        MailData *data = [channel mergeResourceMailData];
//        MailInfoDataModel *mailInfoModel =[channel.msgList lastObject];
//        [[CSCommandManager sharedCommandManager]settingReadStatus_Banched_WithMailType:mailInfoModel.type andWithModFlag:NO andWithMailTypes:@""];
//        if (data) {
//            [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:data];
//        }
    }else if([channel.channelID isEqualToString:Mail_ChannelID_Monster]){
//        for (MailInfoDataModel *mode in channel.msgList) {
//            if (!mode.maildata) {
//                mode.maildata = [[MonsterMailData alloc] initWithAddMailData:mode];
//            }
//        }
//        
//        MailData *data = [channel mergeMonsterMailData];
//        MailInfoDataModel *mailInfoModel =[channel.msgList lastObject];
//        [[CSCommandManager sharedCommandManager]settingReadStatus_Banched_WithMailType:mailInfoModel.type andWithModFlag:NO andWithMailTypes:@""];
//        
//        if (data) {
//            [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:data];
//        }
    }else{
//        [self parseShowContentsByChannel:channel];
//        [self pushCSSystemMailListVCByChannel:channel];
        
        [self parseModeByChannel:(ChatChannel*) channel];
        
        CSSystemMailListVC *sysMailListVC = [self getCSSystemMailListVCByChannel:channel];
        
        return sysMailListVC;
    }
    
    
    
    return nil;
}


- (void)previewingContext:(id <UIViewControllerPreviewing>)previewingContext commitViewController:(UIViewController *)viewControllerToCommit
{
    
    [self showViewController:viewControllerToCommit sender:self];
}

-(void)notificationForChatChannelChangedWithNotify:(NSNotification*)vNotify{
    NSDictionary *userInfo =vNotify.userInfo;
    
    DVLog(@"notifyUserInfo :%@",userInfo);
    NSString *channelIdString =[userInfo objectForKey:@"channelID"];
    [[ChannelManager sharedChannelManager]chatMailCHannelUnreadManager];
    CSMailCategoryListModel *chatCategoryModel = [self.firstListDataArray objectAtIndex:0];
    chatCategoryModel.unReadCount = chatCategoryModel.categetyChannel.unreadCount;
      [self.mailCategoryListTableView reloadData];
}

@end
