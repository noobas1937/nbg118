//
//  ChatMailTableTableViewController.m
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import "ChatAllianceTableViewController.h"
#import "MJRefresh.h"
#import "ChatCellIOS.h"
#import "ChatServiceController.h"
#import "MsgMessage.h"
#import "ServiceInterface.h"
#import "UserManager.h"
#import "ZYTChatSystemCell.h"
#import "ChannelManager.h"
#import "PersonSelectVC.h"
#include "ChatServiceCocos2dx.h"
#import "MJRefresh.h"
#import "UITableViewController+Extension.h"
#import "UITableView+FDTemplateLayoutCell.h"

@interface ChatMailTableTableViewController ()<UITableViewDataSource,UITableViewDelegate>
@property (nonatomic,assign) CGFloat offsetY;
@end

static NSString *const mailCell=@"mail";

@implementation ChatMailTableTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupRefresh];
    [self.tableView registerClass:[ChatCellIOS class] forCellReuseIdentifier:mailCell];
    self.tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
    self.tableView.allowsSelection = NO;
    self.tableView.backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    self.tableView.dataSource=self;
    self.tableView.delegate=self;
    self.dataSourceArray = [NSMutableArray array];
}

- (void)viewDidAppear:(BOOL)animated{
//    [self tableViewScrollCurrentLine];
}

-(void)setCurrentChatChannel:(ChatChannel *)currentChatChannel{
    _currentChatChannel = currentChatChannel;
   
    if (_currentChatChannel.msgList > 0) {
         _dataSourceArray = [_currentChatChannel.msgList mutableCopy];
        [self.tableView reloadData];
    }
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{

    return  self.dataSourceArray.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{

    ChatCellFrame * cellFrame =[self.dataSourceArray objectAtIndex:indexPath.row];
    
    if (cellFrame.chatMessage.post == 100) {

        ZYTChatSystemCell *cell =[ZYTChatSystemCell chatSystemCellWithTableView:tableView];
        [cell setChatCellFrame:cellFrame];
       
        cell.backgroundColor=[UIColor clearColor];
   
        return cell;
        
    }else{
        ChatCellIOS *cell=[tableView dequeueReusableCellWithIdentifier:mailCell forIndexPath:indexPath];
        
        for (UIView *subView in cell.contentView.subviews)
        {
            [subView removeFromSuperview];
        }
      
        cell.backgroundColor=[UIColor clearColor];
        
        [cell setCellFrame:cellFrame];
        return cell;
        
    }
    
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
//    ChatCellFrame *cellFrame = [self gettingMailCellFrames][indexPath.row];
//    
//    if (cellFrame.chatMessage.post == 100) {
//        return cellFrame.chatSystemSize.height+10;
//    }else{
//        if (cellFrame.isShowUiTopView_BOOL) {
//            return [self gettingCellHight:cellFrame];
//        }
//        cellFrame.isShowUiTopView_BOOL = [cellFrame isShowUiTopView:[self gettingMailCellFrames]];
//        if (cellFrame.isShowUiTopView_BOOL) {
//            return [self gettingCellHight:cellFrame];
//        }
//        return cellFrame.cellHeight;
//        
//    }
    
    return [tableView fd_heightForCellWithIdentifier:mailCell cacheByIndexPath:indexPath configuration:^(ChatCellIOS *cell) {
        // 配置 cell 的数据源，和 "cellForRow" 干的事一致，比如：
        cell.cellFrame = self.dataSourceArray[indexPath.row];
    }];
    
}

/**------------------------------------------------------自定义函数------------------------------------------------------*/
- (void)setupRefresh
{
    
    MJRefreshNormalHeader *header = [MJRefreshNormalHeader headerWithRefreshingTarget:self refreshingAction:@selector(headerRereshing)];
    
    // 隐藏时间
    header.lastUpdatedTimeLabel.hidden = YES;
    
    // 隐藏状态
    header.stateLabel.hidden = YES;
    
    // 设置header
    self.tableView.header = header;
}

-(void)endRefreshing
{
    [self.tableView.header endRefreshing];
}

//下拉刷新
- (void)headerRereshing
{
    
    if (self.currentChatChannel.channelType == IOS_CHANNEL_TYPE_USER) {
        [self.tableView.header endRefreshing];
        return;
    }
    
    self.offsetY = self.tableView.contentSize.height;
    [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
    
    BOOL flg = [self.currentChatChannel gettingOldMsg];
    
    if (flg) {
        [self beginRefreshing];
    }else{
        [self endRefreshing];
    }

}

-(instancetype) init
{
    self = [super init];
    if (self) {
        self.isSrollNewMsg = TRUE;
        self.fromUid = [self gettingFromUid];
    }
    return self;
}

/**类函数 刷新显示 先上屏reload   0*/
-(void)refreshDisplay:(ChatCellFrame *)cellFrame :(NSString *)fromUid {

    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:fromUid];
    [cc.msgList addObject:cellFrame];
    ChatServiceCocos2dx::settingGroupChatMailVCChatChannel();
    [self tableViewScrollCurrentLine];

}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    
    
    CGPoint pt = [scrollView contentOffset];
        
    //获取是否滚动的最大边界值
    CGFloat boundaryValueMax= scrollView.contentSize.height - self.view.frame.size.height * 0.6 - self.view.frame.size.height;
        
    if (pt.y > boundaryValueMax && boundaryValueMax > 0) {
        self.isSrollNewMsg = TRUE;
    }else{
        self.isSrollNewMsg = FALSE;
    }
    
}

-(NSString*) gettingFromUid
{
    return [UserManager sharedUserManager].currentMail.opponentUid;
}

-(CGFloat) gettingCellHight:(ChatCellFrame*) cellFrame
{
    return cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
}

-(NSMutableArray*) gettingMailCellFrames
{
    return self.dataSourceArray;
}

-(CGFloat) gettingOffsetY
{
    return self.offsetY;
}


@end
