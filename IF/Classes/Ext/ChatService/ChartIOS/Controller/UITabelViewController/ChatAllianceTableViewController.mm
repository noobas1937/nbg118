//
//  ChatAllianceTableViewController.m
//  IF
//
//  Created by mzc on 15/4/21.
//
//

#import "ChatAllianceTableViewController.h"
#import "MJRefresh.h"
#import "ChatCellIOS.h"
#import "ChatServiceController.h"
#import "MsgMessage.h"
#import "ServiceInterface.h"
#import "ChatContentView.h"
#import "ChannelManager.h"
#import "UserManager.h"
#import "UITableViewController+Extension.h"
#import "UITableView+FDTemplateLayoutCell.h"
#import "ChatChannel.h"
#import "SRRefreshView.h"

@interface ChatAllianceTableViewController ()<UITableViewDataSource,UITableViewDelegate,UIScrollViewDelegate,ChatChannelDelegate,SRRefreshDelegate>
@property (nonatomic,assign) CGFloat offsetY;
@property (nonatomic,assign,getter=isHasNewMsgPushed)BOOL hasNewMsgPushed;
@property (nonatomic,assign,getter=isHasLoadOldMsgPushed)BOOL hasLoadOldMsgPushed;
@property (nonatomic,assign,getter=isRefreshingFlagTure)BOOL refreshingFlag;

@property (strong, nonatomic) SRRefreshView *slimeView;

@end

static NSString *const allianceCell=@"alliance";

@implementation ChatAllianceTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
 
    self.hasNewMsgPushed = NO;
    self.hasLoadOldMsgPushed = NO;
    self.dataSourceArray = [[NSMutableArray alloc]init];
 
//    [cc.msgList removeAllObjects];
//    [cc gettingFirstMsg];
    
    [self.tableView registerClass:[ChatCellIOS class] forCellReuseIdentifier:allianceCell];
    self.tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
    self.tableView.allowsSelection = NO;
    self.tableView.backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    self.tableView.dataSource=self;
    self.tableView.delegate=self;

    [self initwithData];
    [self.tableView addSubview:self.slimeView];
    self.refreshingFlag = NO;
    
    [self.tableView reloadData];
    [self tableViewScrollCurrentLine];
    
}

-(void)setCurrentChannel:(ChatChannel *)currentChannel{
    _currentChannel = currentChannel;
    _currentChannel.channelDelegate = self;
}
- (void)viewDidAppear:(BOOL)animated{
//    [self tableViewScrollCurrentLine];
}

-(void)initwithData
{
    self.isSrollNewMsg = TRUE;
    
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



/**下拉刷新请求数据*/
-(BOOL) requestActionHistoryMsg
{

    return  [self.currentChannel gettingOldMsg];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
 
    return _dataSourceArray.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    ChatCellIOS *cell=[tableView dequeueReusableCellWithIdentifier:allianceCell forIndexPath:indexPath];
    cell.backgroundColor=[UIColor clearColor];
    ChatCellFrame * cellFrame = [self gettingAllianceCellFrames][indexPath.row];
    if (cellFrame.chatMessage.post == 12) {
        [cellFrame.chatMessage changeRedPackageStatus];
    }
    [cell setCellFrame:cellFrame];
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
//    ChatCellFrame *cellFrame = [self gettingAllianceCellFrames][indexPath.row];
//    
//    if (cellFrame.isShowUiTopView_BOOL) {
//        return cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
//    }
//    
//    cellFrame.isShowUiTopView_BOOL = [cellFrame isShowUiTopView:[self gettingAllianceCellFrames]];
//    if (cellFrame.isShowUiTopView_BOOL) {
//        return cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
//    }
//    return cellFrame.cellHeight;
    
    return [tableView fd_heightForCellWithIdentifier:allianceCell cacheByIndexPath:indexPath configuration:^(ChatCellIOS *cell) {
        // 配置 cell 的数据源，和 "cellForRow" 干的事一致，比如：
        cell.cellFrame = self.dataSourceArray[indexPath.row];
    }];
}

/** 刷新显示*/
-(void)refreshDisplay:(ChatCellFrame*)cellFrame{
    
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
//    [cc.msgList addObject:cellFrame];
    [_dataSourceArray addObject:cellFrame];
    [self.tableView reloadData];
    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
    //    [self setDataSourceArray:cc.msgList];
//    [self tableViewScrollCurrentLine];

}


// /**滚动到第一行自动刷新*/
//- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
//    
//    CGPoint pt = [scrollView contentOffset];
//        
//    //获取是否滚动的最大边界值
//    CGFloat boundaryValueMax= scrollView.contentSize.height - self.view.frame.size.height * 0.6 - self.view.frame.size.height;
//        
//    if (pt.y > boundaryValueMax && boundaryValueMax > 0) {
//        self.isSrollNewMsg = TRUE;
//    }else{
//        self.isSrollNewMsg = FALSE;
//    }
//    
//}

/**给先上屏的数据覆盖从服务器返回的SequenceId*/
-(void) coverSequenceId:(NSMsgItem *)chatMessage
{
    for(ChatCellFrame *cellframe in [self gettingAllianceCellFrames])
    {
        if (cellframe.chatMessage.sendLocalTime == chatMessage.sendLocalTime)
        {
            cellframe.chatMessage.sequenceId = chatMessage.sequenceId;
        }
    }
}

-(NSMutableArray*) gettingAllianceCellFrames
{
    return _dataSourceArray;
}

-(void) setDataSourceArray:(NSMutableArray *)dataSourceArray
{
    if (dataSourceArray.count > 0) {
//        [_dataSourceArray removeAllObjects];
        _dataSourceArray = dataSourceArray;
//        [self.tableView reloadData];
    }
 
}

-(CGFloat) gettingOffsetY
{
    return self.offsetY;
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
#pragma mark 赋值刷新
-(void)reloadNewMsg{
        dispatch_async(dispatch_get_main_queue(), ^{
            NSIndexPath *tempIndexPath = [self.tableView.indexPathsForVisibleRows lastObject];
            
            int oldCount = self.dataSourceArray.count;
            self.dataSourceArray = [self.currentChannel.msgList mutableCopy];
            int indexCount = self.dataSourceArray.count - oldCount;
            [self.tableView reloadData];
            if (self.dataSourceArray.count > 0){
                if (oldCount == 0) {
                    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                }else{
                    if  (indexCount > 0){
                        if  (tempIndexPath.row > oldCount-4){
                            [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                        }
                    }
                    if (indexCount  == 0) {
                        [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
                    }
                }
            }
            self.hasNewMsgPushed = NO;
        });
    
}
-(void)reloadOldMsg{
    
    dispatch_async(dispatch_get_main_queue(), ^{
        NSArray *indexPathArray =[self.tableView indexPathsForVisibleRows];
        NSIndexPath *tempIndexPath ;
        if (indexPathArray.count> 0) {
            tempIndexPath =  [[self.tableView indexPathsForVisibleRows] objectAtIndex:0];
        }
      
        int oldCount = self.dataSourceArray.count;
        self.dataSourceArray = [self.currentChannel.msgList mutableCopy];
        int indexCount = self.dataSourceArray.count - oldCount;
        [self.tableView reloadData];
        if (self.dataSourceArray.count> 0) {
            if (tempIndexPath && tempIndexPath.row <= 3) {
                //判断新数据来之前，数据表是否在顶端
                if(self.dataSourceArray.count >= indexCount  ){
                    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
                }
            }else{
                if (self.dataSourceArray.count>(tempIndexPath.row +indexCount)) {
                    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:(tempIndexPath.row +indexCount) inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
                }
            }
        }
        self.hasLoadOldMsgPushed = NO;
    });
}
#pragma mark -
#pragma mark ChatChannel Delegate

-(void)showNewMsg:(ChatChannel *)vChannle{
    if ( self.tableView.dragging== NO && self.tableView.decelerating == NO) {
        [self reloadNewMsg];
        self.hasNewMsgPushed = NO;
    }else{
        self.hasNewMsgPushed = YES;
    }
}
-(void)showOldMsg:(ChatChannel *)vChannel{
    [self tableViewEndRefreshed];
    if ( self.tableView.dragging== NO && self.tableView.decelerating == NO) {
        [self reloadOldMsg];
        self.hasLoadOldMsgPushed = NO;
    }else{
        self.hasLoadOldMsgPushed = YES;
    }
}

-(void)showscreenLockedMsg:(ChatChannel *)vChannel{
    if ( self.tableView.dragging== NO && self.tableView.decelerating == NO) {
        [self reloadOldMsg];
        self.hasLoadOldMsgPushed = NO;
    }else{
        self.hasLoadOldMsgPushed = YES;
    }
}
#pragma mark -
#pragma mark SRRefreshDelegate

- (void)slimeRefreshStartRefresh:(SRRefreshView*)refreshView{
    if (self.currentChannel.msgList.count== 0){
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
        [self.currentChannel gettingFirstMsg];
        [_slimeView endRefresh];
    }
}
-(BOOL)tableViewBeginRefreshed{
    
    self.offsetY = self.tableView.contentSize.height;
  return   [self requestActionHistoryMsg];
    
}
-(void)tableViewEndRefreshed{
    self.refreshingFlag = NO;
    [_slimeView endRefresh];
}
@end
