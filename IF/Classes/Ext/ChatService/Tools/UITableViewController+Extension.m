//
//  UITableViewController+Extension.m
//  IF
//
//  Created by 马纵驰 on 15/8/3.
//
//

#import "UITableViewController+Extension.h"
#import "UserManager.h"
#import "ChannelManager.h"
#import "ChatChannel.h"
#import "MJRefresh.h"
#import "ServiceInterface.h"

@implementation UITableViewController (Extension)

-(void)tableViewScrollCurrentLine
{
    if (self.tableView.contentSize.height > self.tableView.frame.size.height)
    {
        CGPoint offset = CGPointMake(0, self.tableView.contentSize.height - self.tableView.frame.size.height);
        [self.tableView setContentOffset:offset animated:FALSE];
    }else{
        DLog(@"没有滚动");
    }
}

-(void)adjustLocation:(CGFloat)offsetY
{
    CGFloat scrollViewY = self.tableView.contentSize.height;
    
    if (offsetY < scrollViewY) {
        CGPoint offset = CGPointMake(0,scrollViewY - offsetY - [[UIScreen mainScreen] bounds].size.height * 0.08);
        [self.tableView setContentOffset:offset animated:FALSE];
    }
}

-(void)adjustLocation2:(CGPoint)offsetPoint
{
    if (offsetPoint.y > self.tableView.bounds.size.height) {
        [self.tableView setContentOffset:offsetPoint animated:FALSE];
    }
}

-(void) beginRefreshing
{
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
        [self.tableView.header endRefreshing];
        [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = TRUE;
    });
}

@end
