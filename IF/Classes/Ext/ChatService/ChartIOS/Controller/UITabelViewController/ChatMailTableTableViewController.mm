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
#import "CSChatSystemCell.h"
#import "ChannelManager.h"
 
#include "ChatServiceCocos2dx.h"
#import "MJRefresh.h"
#import "UITableViewController+Extension.h"
#import "UITableView+FDTemplateLayoutCell.h"

#import "SRRefreshView.h"
@interface ChatMailTableTableViewController ()<UITableViewDataSource,UITableViewDelegate,ChatChannelDelegate,SRRefreshDelegate>
@property (nonatomic,assign) CGFloat offsetY;
@property (nonatomic,assign,getter=isHasNewMsgPushed)BOOL hasNewMsgPushed;
@property (nonatomic,assign,getter=isHasLoadOldMsgPushed)BOOL hasLoadOldMsgPushed;
@property (nonatomic,assign,getter=isRefreshingFlagTure)BOOL refreshingFlag;

@property (strong, nonatomic) SRRefreshView *slimeView;
@end

static NSString *const mailCell=@"mail";

@implementation ChatMailTableTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
 
    [self.tableView registerClass:[ChatCellIOS class] forCellReuseIdentifier:mailCell];
    self.tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
    self.tableView.allowsSelection = NO;
    self.tableView.backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    self.tableView.dataSource=self;
    self.tableView.delegate=self;
    self.dataSourceArray = [NSMutableArray array];
    
    [self.tableView addSubview:self.slimeView];
    self.refreshingFlag = NO;
}

- (void)viewDidAppear:(BOOL)animated{
//    [self tableViewScrollCurrentLine];
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




-(void)setCurrentChatChannel:(ChatChannel *)currentChatChannel{
    _currentChatChannel = currentChatChannel;
    _currentChatChannel.channelDelegate  = self;
 
}

-(void)setDataSourceArray:(NSMutableArray *)dataSourceArray{
    [_dataSourceArray removeAllObjects ];
    _dataSourceArray = [dataSourceArray mutableCopy];
//    [self.tableView reloadData];
//    [self tableViewScrollCurrentLine];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{

    return  self.dataSourceArray.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{

    ChatCellFrame * cellFrame =[self.dataSourceArray objectAtIndex:indexPath.row];
    
    if (cellFrame.chatMessage.post == 100) {

        CSChatSystemCell *cell =[CSChatSystemCell chatSystemCellWithTableView:tableView];
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


-(void)endRefreshing
{
    [self.tableView.header endRefreshing];
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
//    [cc.msgList addObject:cellFrame];
    [_dataSourceArray addObject:cellFrame];
    [self.tableView reloadData];
    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:self.dataSourceArray.count-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
//    ChatServiceCocos2dx::settingGroupChatMailVCChatChannel();
//    [self tableViewScrollCurrentLine];

}

//- (void)scrollViewDidScroll:(UIScrollView *)scrollView {


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
#pragma mark 滚动停止后，来消息处理
-(void)reloadNewMsg{
    dispatch_async(dispatch_get_main_queue(), ^{
        //更新UI操作
        NSIndexPath *tempIndexPath = [self.tableView.indexPathsForVisibleRows lastObject];
        
        int oldCount = self.dataSourceArray.count;
        self.dataSourceArray = [self.currentChatChannel.msgList mutableCopy];
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
            }
        }

    });
    self.hasNewMsgPushed = NO;
}
-(void)reloadOldMsg{
    
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        NSIndexPath *tempIndexPath =  [[self.tableView indexPathsForVisibleRows] objectAtIndex:0];
        
        //更新UI操作
        
        int oldCount = self.dataSourceArray.count;
        self.dataSourceArray = [self.currentChatChannel.msgList mutableCopy];
        int indexCount = self.dataSourceArray.count - oldCount;
        [self.tableView reloadData];
        if (tempIndexPath.row<= 3) {
            CGPoint pt =self.tableView.contentOffset;
            CGFloat boundaryValueMax= self.tableView.contentSize.height - self.tableView.frame.size.height * 0.6 - self.tableView.frame.size.height;
            if(indexCount > 0   ){
                [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:indexCount inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
            }
        }

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
         [_slimeView endRefresh];
    }
}
-(BOOL)tableViewBeginRefreshed{
 
    self.offsetY = self.tableView.contentSize.height;
   return [self.currentChatChannel gettingOldMsg];
    
}
-(void)tableViewEndRefreshed{
    self.refreshingFlag = NO;
    [_slimeView endRefresh];
}
@end
