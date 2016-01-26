//
//  ChatMailTableTableViewController.h
//  IF
//
//  Created by mzc on 15/4/23.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"
#import "ChatCellFrame.h"

#import "ChatChannel.h"


@interface ChatMailTableTableViewController : UITableViewController

//当前邮件频道
@property (nonatomic,strong) NSString *fromUid;
@property (strong,nonatomic) ChatChannel * currentChatChannel;
@property (strong,nonatomic) NSMutableArray * dataSourceArray;

/**下拉刷新*/
-(void) refreshDisplay:(ChatCellFrame*)cellFrame :(NSString*)fromUid;

/**判断最新单条数据是否滚动到最新消息*/
@property (nonatomic,assign) BOOL isSrollNewMsg;

/***停止下拉刷新*/
-(void) endRefreshing;

-(CGFloat) gettingOffsetY;

@end
