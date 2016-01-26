//
//  CSSystemMailListVC.m
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import "CSSystemMailListVC.h"
#import "TopHeadView.h"
#import "SysMailListCell.h"
#import "ChatServiceController.h"
#import "CSMailBottomOperationToolBarView.h"
#import "CSConvertViewManager.h"
#import "ChannelManager.h"
#import "MailInfoManager.h"
#import "CSCommandManager.h"
#import "NSString+Cocos2dHelper.h"
#import "CSAlertView.h"
#import "ServiceInterface.h"
#import "ChatServiceCocos2dx.h"
#import "MailDataManager.h"
#import "NSArray+SortHelper.h"
#import "MJRefresh.h"
#import "AnimateView.h"
#import "JSONKit.h"
@interface CSSystemMailListVC ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate,MailBottomToolBarDelegate,CSConvertViewManagerDelegate,SysMailListCellDelegate,ZYTAlertViewDelegate,SlideCellProtocol,ChatChannelDelegate>
{
    BOOL allMemberSelected;
    NSIndexPath *deletedIndexPath;
    NSMutableArray  *editTempArray;
    BOOL isCellTouched;
    
    SysMailListCell * openCell;
    BOOL isOpenScroCell;
    
    dispatch_queue_t csSystemMailListVCQuene;
}

@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (nonatomic,assign,getter= isSelectedButtonShowing) BOOL selectedButtonShowing;
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong) CSMailBottomOperationToolBarView * mailBottomToolBarView;
@property (nonatomic,strong) UITableView * sysMailListTableView;
@property (nonatomic,strong) NSMutableArray * dataSourceArr;
@property (nonatomic,strong) NSMutableArray * selectedMailArray;

@end

@implementation CSSystemMailListVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
     csSystemMailListVCQuene = dispatch_queue_create("com.chatServer.csSystemMailListVCQuene", NULL);

    
    isOpenScroCell=NO;
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
    [self.footView addSubview:self.mailBottomToolBarView];
    [self.headView addSubview:self.topHeaderView];
    [self.view addSubview:self.sysMailListTableView];
    self.selectedMailArray= [NSMutableArray array];
    allMemberSelected = NO;
    isCellTouched = NO;
    //    [self lodingRefreshFooter];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(notifyAction:) name:kMailRewardBanchForClaim object:nil];
    
    
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:kMailRewardBanchForClaim object:nil];
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
        view.titleNameLabel.text = self.chatChannel.nameString;
        view.frame = self.headView.frame;
        [view.rightEditButton setImage:[UIImage imageNamed:@"Btn_Mail_Editor"] forState:UIControlStateNormal];
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}
-(UITableView *)sysMailListTableView{
    if (_sysMailListTableView == nil) {
        UITableView *tableView =[[UITableView alloc]initWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, self.footView.top - self.headView.height) style:UITableViewStylePlain];
        
        
        tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"mail_list_bg"]];
        tableView.delegate = self;
        tableView.dataSource = self;
        _sysMailListTableView = tableView;
    }
    return _sysMailListTableView;
}
-(NSMutableArray *)dataSourceArr{
    if (_dataSourceArr == nil) {
        NSMutableArray * mutArray = [NSMutableArray array ];
        _dataSourceArr = mutArray;
    }
    return _dataSourceArr;
}
-(CSMailBottomOperationToolBarView *)mailBottomToolBarView{
    if (_mailBottomToolBarView == nil) {
        _mailBottomToolBarView =[CSMailBottomOperationToolBarView mailBottomOperationToolBarViewWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height)];
        _mailBottomToolBarView.mailBottomToolBarDelegate = self;
        [_mailBottomToolBarView settingDeleteButtonShow:YES andgitButtonShow:YES andStarButtonShow:NO];
    }
    return _mailBottomToolBarView;
}
//-(NSMutableArray *)selectedMailArray{
//    if  (_selectedMailArray == nil){
//        NSMutableArray * mutArray =[NSMutableArray array];
//        _selectedMailArray=mutArray ;
//    }
//    return _selectedMailArray;
//}
#pragma mark -
#pragma mark setter

-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    _chatChannel.channelDelegate = self;
    if (self.chatChannel.msgList> 0){
        if (self.chatChannel.msgList.count>= 20){
             [self settingFootLoadMoreView];
        }
       
    }else{
        UILabel *showlable = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 100, 50)];
        showlable.text = [NSString stringWithMultilingualWithKey:@"105385"];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            showlable.font = [UIFont systemFontOfSize:22.0];
        }else{
            showlable.font = [UIFont systemFontOfSize:12.0];
        }
        [showlable sizeToFit];
        showlable.center = CGPointMake(self.sysMailListTableView.center.x, self.sysMailListTableView.center.y-50);
        [self.sysMailListTableView addSubview:showlable];
    }
    chatChannel.msgList=  [self sortingDataSource:chatChannel.msgList];
    self.dataSourceArr = chatChannel.msgList  ;
 
    [self.sysMailListTableView reloadData];
}
//删除后再次加载数据
-(void)channelMsgloadMoreWithDeleted{
    NSMutableArray *tempArray  = [[ChannelManager sharedChannelManager]gettingSystemMailArrayWithChannelIDFromDB:self.chatChannel.channelID andGetCount:20];
    if (tempArray.count> 0){
        for (MailInfoDataModel *mode in tempArray) {
            mode.maildata = [MailDataManager mailDataWithMailInfoData:mode];
        }
        tempArray = [self sortingDataSource:tempArray];
        self.chatChannel.msgList = tempArray;
        self.dataSourceArr = self.chatChannel.msgList;
        if (tempArray.count>= 20){
              [self settingFootLoadMoreView];
        }else{
            self.sysMailListTableView.footer = nil;
        }
        [self.sysMailListTableView reloadData];
    }else{
        UILabel *showlable = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 100, 50)];
        showlable.text = [NSString stringWithMultilingualWithKey:@"105385"];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            showlable.font = [UIFont systemFontOfSize:22.0];
        }else{
            showlable.font = [UIFont systemFontOfSize:12.0];
        }
        [showlable sizeToFit];
        showlable.center = CGPointMake(self.sysMailListTableView.center.x, self.sysMailListTableView.center.y-50);
        [self.sysMailListTableView addSubview:showlable];

    }

}
#pragma mark -
#pragma mark action
-(void)updateMailUnReadCount{
    [self.chatChannel channelUnreadCountcalculatedWithSystemMail];
    [self.chatChannel channelUpDateWithSelfChanged];
    [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":self.chatChannel.channelID}];
    ChatServiceCocos2dx::updateUnreadCountOnMastermind();
}
-(void)cancelEditingBar{
    _topHeaderView.rightEditButton.selected = NO;
    [_mailBottomToolBarView settingStartStatus];
    [self mailBottomToolBarShow:NO];
}
-(void)mailBottomToolBarShow:(BOOL)vIsShow{
    
    
    
    if (vIsShow){
        // 显示
        
        self.selectedButtonShowing = YES;
        
        self.footView.top = kMainScreenHeight - self.footView.height;
    }else{
        
        self.selectedButtonShowing = NO;
        self.footView.top = kMainScreenHeight  ;
        
        allMemberSelected = NO;//是否选择了全部选择按钮
        [_mailBottomToolBarView settingStartStatus];
        for (MailInfoDataModel  *mailDate in self.dataSourceArr) {
            mailDate.isSelected = NO;
        }
        [self.selectedMailArray removeAllObjects];
        
    }
    self.sysMailListTableView.height = self.footView.top - self.headView.height;
    [self.sysMailListTableView reloadData];
}
-(void)showAlertOnViewWithTitle:(NSString *)vTitleString
                andWithEditType:(int)vEditType{
    //弹出提示 没有领取奖励
    
    CSAlertView  * alertView = [CSAlertView alertViewWithTitleString:nil];
    alertView.tag = vEditType;
    if (vEditType  == 1){
        alertView.viewType = NOGETREWARD;
        
    }else if (vEditType == 2){
        alertView.viewType = BLOCK;
        
    }else if(vEditType == 3){
        alertView.viewType = BLOCK;
    }
    
    
    [alertView showViewByType];
    alertView.titleType = ZYTAlertViewTitleType_shield;
    [alertView setNameString:vTitleString];
    alertView.alertViewDelegate = self;
    
    UIViewController *recentView = nil;
    UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
    [nav.topViewController.view addSubview:alertView];
    [nav.topViewController.view bringSubviewToFront:alertView];
}


#pragma mark -
#pragma mark 排序
/**
 *  从大到小排序
 *
 *  @param vArray 传入的array
 *
 *  @return 返回的array
 */
- (NSMutableArray *)sortingDataSource:(NSMutableArray *)vArray
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(MailInfoDataModel *obj1, MailInfoDataModel* obj2){
                          
                          
                          if(obj1.creatTime > obj2.creatTime) {
                              //升序
                              return(NSComparisonResult)NSOrderedAscending;
                          }else {
                              //降序
                              return(NSComparisonResult)NSOrderedDescending;
                          }
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}



#pragma mark -
#pragma mark TopHeadView delegate
-(void)clickButton{
    if(self.chatChannel.msgList.count > 0){
        //        MailInfoDataModel *lastModel = [self.chatChannel.msgList lastObject];
        //        [self.chatChannel.msgList removeAllObjects];
        //        [self.chatChannel.msgList addObject:lastModel];
    }
    
    [self.navigationController popToRootViewControllerAnimated:YES];
}

-(void)rightButtonAction:(UIButton *)sender{
    sender.selected = !sender.selected;
    if (sender.selected) {
        [self mailBottomToolBarShow:YES];
    }else{
        [self mailBottomToolBarShow:NO];
    }
}
#pragma mark -
#pragma mark tableView delegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSourceArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    SysMailListCell *cell = [SysMailListCell sysMailListCellWithTableView:tableView];
    cell.selectedButtonShowing = [self isSelectedButtonShowing];
    cell.mailInforDataModel = self.dataSourceArr[indexPath.row];
    cell.sysMailListCellDelegate = self;
    cell.delegate=self;
    //    [cell initView];
    if (allMemberSelected == NO) {
        cell.mailInforDataModel.isSelected = NO;
    }
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return [SysMailListCell rowHeightWithSysMailListCell];
 
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    
    if (isOpenScroCell) {
        [tableView deselectRowAtIndexPath:indexPath animated:NO];
        [openCell closeUpCell:openCell];
        isOpenScroCell=NO;
        
    }else{
        
        
            [tableView deselectRowAtIndexPath:indexPath animated:YES];
            
            //    MailInfoDataModel *tempInfo = (MailInfoDataModel*)self.dataSourceArr[indexPath.row];
            //    [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:tempInfo.maildata];
            
            SysMailListCell *cell = [tableView cellForRowAtIndexPath:indexPath];
            if  ( isCellTouched == YES){
                return;
            }
            
            
            [self cancelEditingBar];
            
            MailInfoDataModel *maildata = (MailInfoDataModel*)self.dataSourceArr[indexPath.row];
            if (!maildata) {
                return ;
            }
            isCellTouched = YES;
            [CSConvertViewManager sharedMailInfoManager].currentIndexPath = indexPath;
            [CSConvertViewManager sharedMailInfoManager].currentmailInfoDataModel = maildata;
            [CSConvertViewManager sharedMailInfoManager].convertViewManagerDelegate = self;
            [[CSConvertViewManager sharedMailInfoManager] currentMailReadingNofityToServer];
            [[CSConvertViewManager sharedMailInfoManager] showCocos2DViewWithMailID:maildata];
            
            //    -(void)showCocos2DViewWithMailID:(MailInfoDataModel *)vMailData;
            //    [[ChatServiceController chatServiceControllerSharedManager].gameHost popUpCocosMailfByData:maildata.maildata];
        
        
    }
    
}


#pragma mark -
#pragma mark MailBottomToolBarDelegate
-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView selectAllButtonActionWithButton:(UIButton *)sender{
    allMemberSelected = sender.selected;
    if (self.selectedMailArray.count> 0){
        [self.selectedMailArray removeAllObjects];
    }
    
    if(sender.selected ){
        
        for (MailInfoDataModel *mailModel in self.dataSourceArr) {
            mailModel.isSelected = YES;
            [self.selectedMailArray addObject:mailModel];
        }
    }else{
        for (MailInfoDataModel *mailModel in self.dataSourceArr) {
            mailModel.isSelected = NO;
            
        }
    }
    [self.sysMailListTableView reloadData];
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView giftButtonActionWithButton:(UIButton *)sender{
    DVLog(@"礼物按钮  selectedArr %@",self.selectedMailArray);
    NSMutableArray *tempArray =[NSMutableArray array];
    for (MailInfoDataModel *model in self.selectedMailArray) {
        if (model.rewardStatus == 0 ){
            [tempArray addObject:model];
        }
        model.rewardStatus = 1;
        model.status = 1;
        /** 这句话只更新数据库的已读状态 */
        [model settingToReadForDB];
    }
    if  (tempArray.count> 0){
        [[CSCommandManager sharedCommandManager]settingRewardStatusWithMailDataArray:self.selectedMailArray];
    }
    [self.chatChannel channelUnreadCountcalculatedWithSystemMail];
    [self.chatChannel channelUpDateWithSelfChanged];
    
    [self cancelEditingBar];
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView starButtonActionWithButton:(UIButton *)sender{
    DVLog(@"标星按钮  selectedArr %@",self.selectedMailArray);
}

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView delectButtonActionWithButton:(UIButton *)sender{
    DVLog(@"删除按钮  selectedArr %@",self.selectedMailArray);
    if(editTempArray == nil){
            editTempArray = [NSMutableArray array];
    }
    for (MailInfoDataModel * mail in self.selectedMailArray) {
        /** 未领取并且未标星 */
        if (mail.rewardStatus != 0 && mail.saveFlag == 0){
            [editTempArray addObject:mail];
        }
    }
    if(editTempArray.count < self.selectedMailArray.count){
        //有未读邮件
        NSString *titleString =[NSString stringWithMultilingualWithKey:@"4100014"];
        titleString = [titleString stringByAppendingString:@"\n"];
        titleString = [titleString stringByAppendingString:[NSString stringWithMultilingualWithKey:@"105374"]];
        [self showAlertOnViewWithTitle:titleString andWithEditType:2];//4100014 == 邮件中存在未领奖...
    }else{
        [self showAlertOnViewWithTitle:[NSString stringWithMultilingualWithKey:@"105374"] andWithEditType:3];//105374=您真的要删除这些邮件吗？
    }
}
#pragma mark -
#pragma mark MailListCellDelegate
-(void)deleteMailFromCocosViewWithManager:(CSConvertViewManager *)vManager{
    if (vManager.currentIndexPath ){
        NSMutableArray *tempArray = [NSMutableArray arrayWithArray:self.chatChannel.msgList];
        [tempArray removeObject:vManager.currentmailInfoDataModel];
        self.chatChannel.msgList = tempArray;
        self.dataSourceArr = self.chatChannel.msgList ;
        [self.sysMailListTableView reloadData];
//        [self.sysMailListTableView deleteRowsAtIndexPaths:@[vManager.currentIndexPath] withRowAnimation:UITableViewRowAnimationBottom];
    }
}
-(void)sysMailListCellSelectedButtonActionWithCell:(SysMailListCell *)vCell andWithButton:(UIButton *)sender{
    
    
    
    if (YES == sender.selected ){
        [self addMember:vCell.mailInforDataModel];
    }else{
        [self subMember:vCell.mailInforDataModel];
    }
    DVLog(@"selectedArr :%@",self.selectedMailArray);
}

#pragma mark 当前已选择人员的array修改
-(void)addMember:(MailInfoDataModel *)vMailModel{
    MailInfoDataModel *tempMailModel= nil;
    for (MailInfoDataModel *selectedMailModel in self.selectedMailArray) {
        if  ([selectedMailModel.mail_ID isEqualToString:vMailModel.mail_ID]){
            tempMailModel = selectedMailModel;
            break;
        }
    }
    if (tempMailModel == nil){
        [self.selectedMailArray addObject:vMailModel];
    }
}
-(void)subMember:(MailInfoDataModel *)vMailModel{
    
    [self.selectedMailArray removeObject:vMailModel];
    
}

#pragma mark -
#pragma mark CSConvertViewManagerDelegate
-(void)serverRefeshDateSucceedWithManager:(CSConvertViewManager *)vManager{
    [self.dataSourceArr setObject:vManager.currentmailInfoDataModel atIndexedSubscript:vManager.currentIndexPath.row];
    NSString *channelIDString = vManager.currentmailInfoDataModel.channelID ;
    
    [self.sysMailListTableView reloadData];
    [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channelIDString}];
}
-(void)settingStarFormServerCallBackWithManager:(CSConvertViewManager *)vManager{
    NSIndexPath *tempIndexPath = vManager.currentIndexPath ;
    if  (tempIndexPath){
        [self.sysMailListTableView reloadRowsAtIndexPaths:@[tempIndexPath] withRowAnimation:UITableViewRowAnimationNone];
    }
}
-(void)cocosCallback_Oc_NativeView{
   
    isCellTouched = NO;
}
#pragma mark -
#pragma mark alertView Delegate
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView{
    if  (vAlertView.tag == 1){
        //        MailInfoDataModel *deleteMailInfoModel = [self.dataSourceArr objectAtIndex:deletedIndexPath.row];
        //        [self.dataSourceArr removeObject:deleteMailInfoModel];
        //
        //        [self.sysMailListTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:deletedIndexPath] withRowAnimation:UITableViewRowAnimationFade]; //数组执行删除 操作
        //        [self.sysMailListTableView  reloadData];
        //        [MailInfoManager mailInfoDataFor_deleteWithMailID:deleteMailInfoModel.mail_ID];
    }else if(vAlertView.tag == 2){
        self.selectedMailArray  = [NSMutableArray arrayWithArray:editTempArray];
        [self.dataSourceArr removeObjectsInArray:self.selectedMailArray];
        [self.chatChannel.msgList removeObjectsInArray:self.selectedMailArray];
        [[CSCommandManager sharedCommandManager]deletingMailDataWithALlMailIDArray:self.selectedMailArray];
        [self.sysMailListTableView reloadData];

        [MailInfoManager mailInfoDataFor_deleteWithmailArray:self.selectedMailArray];
        [self updateMailUnReadCount];
        [self cancelEditingBar];
        

        
        map<string,pair<string,int>> m_detectMailMap;//name uid createTime
        for (MailInfoDataModel *tempModel in editTempArray) {
                    if ([self.chatChannel.channelID isEqualToString:Mail_ChannelID_Fight]){
                        if ([tempModel isGrandCastleInvestigation]){
                            //是否是城堡侦查邮件
                            NSDictionary *contentsdic =[tempModel.contents objectFromJSONString];
                            tempModel.nameText = [[contentsdic objectForKey:@"user"] objectForKey:@"name"];
                               m_detectMailMap.insert(make_pair([tempModel.nameText UTF8String], make_pair([tempModel.mail_ID UTF8String], tempModel.creatTime)));
                          
                        }
                    }
        }
        ChatServiceCocos2dx::deleteDectCityMail(m_detectMailMap);
        [editTempArray removeAllObjects];
        editTempArray = nil;
        
        
    }else if (vAlertView.tag == 3){
        [self.dataSourceArr removeObjectsInArray:self.selectedMailArray];
        editTempArray = [NSMutableArray arrayWithArray:self.selectedMailArray];
        [self.chatChannel.msgList removeObjectsInArray:self.selectedMailArray];
        [[CSCommandManager sharedCommandManager]deletingMailDataWithALlMailIDArray:self.selectedMailArray];
   
        [self.sysMailListTableView reloadData];

        [MailInfoManager mailInfoDataFor_deleteWithmailArray:self.selectedMailArray];
        [self updateMailUnReadCount];
        [self cancelEditingBar];
        
        
        map<string,pair<string,int>> m_detectMailMap;//name uid createTime
        for (MailInfoDataModel *tempModel in editTempArray) {
            if ([self.chatChannel.channelID isEqualToString:Mail_ChannelID_Fight]){
                if ([tempModel isGrandCastleInvestigation]){
                    //是否是城堡侦查邮件
                    NSDictionary *contentsdic =[tempModel.contents objectFromJSONString];
                    tempModel.nameText = [[contentsdic objectForKey:@"user"] objectForKey:@"name"];
                    m_detectMailMap.insert(make_pair([tempModel.nameText UTF8String], make_pair([tempModel.mail_ID UTF8String], tempModel.creatTime)));
                    
                }
            }
        }
        ChatServiceCocos2dx::deleteDectCityMail(m_detectMailMap);
         [editTempArray removeAllObjects];
        editTempArray = nil;
    }
    if (self.chatChannel.msgList.count <20){
        [self channelMsgloadMoreWithDeleted];
    }
    
}
-(void)cancalBtnAction:(CSAlertView *)vAlertView{
    
}
#pragma mark -
#pragma mark cell 左滑事件
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath{
    //打开编辑状态，不允许删除
    
    //selectedButtonShowing
    //return ![self isSelectedButtonShowing];
    return NO;
}
-(BOOL)SysMailopen:(SysMailListCell *)cell
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
-(BOOL)SysMailclose:(SysMailListCell *)cell
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
    return self.selectedButtonShowing;
}
-(void)cellbuttonClick:(UIButton*)button andWithMailCategroyListCell:(UITableViewCell *)vCell
{
    //    NSInteger CurrentselectNum;
    //    if ([[[UIDevice currentDevice] systemVersion] floatValue]<8) {
    //        CurrentselectNum=[self.sysMailListTableView indexPathForCell:([[(SysMailListCell*)[button  superview] superview] superview])].row;
    //    }else{
    //        CurrentselectNum=[self.sysMailListTableView indexPathForCell:([(SysMailListCell*)[button  superview] superview])].row;
    //    }
    
    NSIndexPath * indexPath =[self.sysMailListTableView indexPathForCell:(SysMailListCell *)vCell];

    NSUInteger row = [indexPath row]; //获取当前行
    MailInfoDataModel *deleteMailInfoModel = [self.dataSourceArr objectAtIndex:indexPath.row];
    if(button.tag == 1){
        //标记已读
        //isOpenScroCell=NO;
        [openCell closeUpCell:openCell];//关闭
        if(deleteMailInfoModel.status == 0){
            deleteMailInfoModel.status = 1;
            [[CSCommandManager sharedCommandManager] settingreadSatusWithMailID:deleteMailInfoModel.mail_ID andWithMailTpye:-1];
        }
        
        
        
    }else if(button.tag == 2){
         //删除
        
        if(deleteMailInfoModel.rewardStatus == 0){
            [self.sysMailListTableView reloadData];
            [ self showAlertOnViewWithTitle:[NSString stringWithMultilingualWithKey:@"105512"] andWithEditType:1]; //105512 =请领取奖励后再删除邮件！
            deletedIndexPath = indexPath;
        }else if(deleteMailInfoModel.saveFlag == 1){
            
            [ self showAlertOnViewWithTitle:[NSString stringWithMultilingualWithKey:@"4100013"] andWithEditType:1]; //4100013=邮件已锁定，请解锁后再删除
            deletedIndexPath = indexPath;
        }else{
            
            if ([self.chatChannel.channelID isEqualToString:Mail_ChannelID_Fight]){
                if ([deleteMailInfoModel isGrandCastleInvestigation]){
                    //是否是城堡侦查邮件
                    map<string,pair<string,int>> m_detectMailMap;//name uid createTime

                    NSDictionary *contentsdic =[deleteMailInfoModel.contents objectFromJSONString];
                    deleteMailInfoModel.nameText = [[contentsdic objectForKey:@"user"] objectForKey:@"name"];
                    m_detectMailMap.insert(make_pair([deleteMailInfoModel.nameText UTF8String], make_pair([deleteMailInfoModel.mail_ID UTF8String], deleteMailInfoModel.creatTime)));
                    ChatServiceCocos2dx::deleteDectCityMail(m_detectMailMap);
                }
            }
            isOpenScroCell=NO;
            [self.dataSourceArr removeObject:deleteMailInfoModel];
//            [self.sysMailListTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade]; //数组执行删除 操作
            [self.sysMailListTableView reloadData];
          
            [MailInfoManager mailInfoDataFor_deleteWithMailID:deleteMailInfoModel.mail_ID];
             [self updateMailUnReadCount];
        }
    }
}
/**
 *  加载上拉控件
 */


-(void)settingFootLoadMoreView{
    
    // 105328=松开载入更多
    if (self.sysMailListTableView.footer == nil){
        NSString *footTitleString =[NSString stringWithMultilingualWithKey:@"105328"];
        __weak CSSystemMailListVC * tempSelf = self;
        MJRefreshAutoNormalFooter *footer = [MJRefreshAutoNormalFooter footerWithRefreshingBlock:^{
            
            [tempSelf loadMoreMailData];
        }];
        [footer setTitle:footTitleString forState:MJRefreshStatePulling];
        [footer setTitle:footTitleString forState:MJRefreshStateIdle];
        [footer setTitle:footTitleString forState:MJRefreshStateRefreshing];
        self.sysMailListTableView.footer = footer;
    }

}
-(void)loadMoreMailData{
    __weak ChatChannel *channel = self.chatChannel;
    dispatch_async(csSystemMailListVCQuene, ^{
        NSArray *tempArray  = [[ChannelManager sharedChannelManager]gettingOldSystemMailArrayFromDBWithChannelID:channel.channelID andWithBeforeMail:[channel.msgList lastObject]];
        if (tempArray.count> 0 && tempArray){
            NSMutableArray *mutArr =[NSMutableArray array];
            for (MailInfoDataModel *mailModel in tempArray) {
                mailModel.maildata = [MailDataManager mailDataWithMailInfoData:mailModel];
                [mutArr addObject:mailModel];
            }
            [channel.msgList addObjectsFromArray:mutArr];
            channel.msgList  = [NSArray mailInfoDataSortingDataSource:channel.msgList ];
            dispatch_async(dispatch_get_main_queue(), ^{
                // 主线程刷新视图
                self.dataSourceArr = channel.msgList;
                [self.sysMailListTableView reloadData];
                [ self.sysMailListTableView.footer endRefreshing];
            });
            
        }else{
            dispatch_async(dispatch_get_main_queue(), ^{
                [ self.sysMailListTableView.footer endRefreshing];
                
                self.sysMailListTableView.footer= nil;
                [self.sysMailListTableView reloadData];
            });
        }
        
    });
    
}

#pragma mark -
#pragma mark notify
-(void)notifyAction:(NSNotification *)vNotify{
    NSDictionary *dic = vNotify.userInfo;
    DVLog(@"%@",dic);
    if ([[dic objectForKey:@"rewardStatus"] intValue] == 0){
        AnimateView  * animateview;
        animateview=[[AnimateView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight)];
        animateview.backgroundColor=[UIColor clearColor];
        [self.view addSubview:animateview];
        [self.view bringSubviewToFront:animateview];
        [animateview initView:[dic objectForKey:@"rewardDic"]];
        [animateview StartAnimate];
        __weak AnimateView * tempVie= animateview;
        animateview.finishAnimated=^()
        {
            [tempVie removeFromSuperview];
        };
    }else{
        
    }
}
#pragma mark -
#pragma mark ChatChannelDelegate
-(void)systemMail_pushNewMsgWith:(ChatChannel *)vChannel andMsg:(MailInfoDataModel *)vMailInfoModel{
    if ([vChannel.channelID isEqualToString:self.chatChannel.channelID]){
         dispatch_async(csSystemMailListVCQuene, ^{
            MailData *maildata = [MailDataManager mailDataWithMailInfoData:vMailInfoModel];
            vMailInfoModel.maildata = maildata;
            NSMutableArray *tempArray =[NSMutableArray array ];
            [tempArray addObject:vMailInfoModel];
            [tempArray addObjectsFromArray:self.chatChannel.msgList ];
            tempArray=  [self sortingDataSource:tempArray];
            self.chatChannel.msgList = tempArray;
            self.dataSourceArr = self.chatChannel.msgList  ;
             dispatch_async(dispatch_get_main_queue(), ^{
                   [self.sysMailListTableView reloadData];
             });
         
         });
 
    }
}
@end
