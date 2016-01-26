//
//  ChatCountriesTableViewController.h
//  IF
//
//  Created by mzc on 15/4/21.
//
//

#import <UIKit/UIKit.h>
#import "ChatCellFrame.h"
@class ChatChannel;
@interface ChatCountriesTableViewController : UITableViewController
@property (nonatomic,strong)NSMutableArray *dataSourceArray;
@property (nonatomic,strong)ChatChannel *currentChannel ;
-(void)refreshDisplay:(ChatCellFrame*)cellFrame;

/**判断最新单条数据是否滚动到最新消息*/
@property (nonatomic,assign) BOOL isSrollNewMsg;

/***停止下拉刷新*/
-(void) endRefreshing;

-(CGFloat) gettingOffsetY;

@end
