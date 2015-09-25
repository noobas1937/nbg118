//
//  ChatCountriesTableViewController.m
//  IF
//
//  Created by mzc on 15/4/21.
//
//

#import "ChatCountriesTableViewController.h"
#import "MJRefresh.h"
#import "ChatCellIOS.h"
#import "ChatServiceController.h"
#import "MsgMessage.h"
#import "ChatServiceCocos2dx.h"
#import "ServiceInterface.h"
#import "ChannelManager.h"
#import "MJRefresh.h"
#import "UITableViewController+Extension.h"
#import "UITableView+FDTemplateLayoutCell.h"

@interface ChatCountriesTableViewController ()<UITableViewDataSource,UITableViewDelegate>
@property (nonatomic,assign) CGFloat offsetY;
@end

static NSString *const countriesCell=@"countries";

@implementation ChatCountriesTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initwithData];
    
    self.dataSourceArray = [[NSMutableArray alloc]init];
    
    [self.tableView registerClass:[ChatCellIOS class] forCellReuseIdentifier:countriesCell];
    self.tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
    self.tableView.allowsSelection = NO;
    self.tableView.backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
    self.tableView.dataSource=self;
    self.tableView.delegate=self;
    self.tableView.hidden = YES;
    
    [self setupRefresh];
    [self.tableView reloadData];
//    [self tableViewScrollCurrentLine];
    
}

- (void)viewDidAppear:(BOOL)animated{
//    [self tableViewScrollCurrentLine];
}

-(void)initwithData
{
    self.isSrollNewMsg = TRUE;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

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
    
    self.offsetY = self.tableView.contentSize.height;
    
    [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
    
    BOOL flg = [self requestActionHistoryMsg];
    
    if (flg) {
        [self beginRefreshing];
    }else{
        [self endRefreshing];
    }
}

/**下拉刷新请求数据*/
-(BOOL) requestActionHistoryMsg
{
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
    return [cc gettingOldMsg];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self gettingCountriesCellFrames].count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ChatCellIOS *cell=[tableView dequeueReusableCellWithIdentifier:countriesCell forIndexPath:indexPath];
    
    cell.backgroundColor=[UIColor clearColor];
    
    ChatCellFrame *cellFrame = [self gettingCountriesCellFrames][indexPath.row];
    
    [cell setCellFrame:cellFrame];
    
    return cell;
    
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return [tableView fd_heightForCellWithIdentifier:countriesCell cacheByIndexPath:indexPath configuration:^(ChatCellIOS *cell) {
        // 配置 cell 的数据源，和 "cellForRow" 干的事一致，比如：
        
        cell.cellFrame = self.dataSourceArray[indexPath.row];
    }];
}


/**类函数 刷新显示*/
-(void)refreshDisplay:(ChatCellFrame*)cellFrame{
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
    [cc.msgList addObject:cellFrame];
    self.dataSourceArray = [cc.msgList mutableCopy];
    [self tableViewScrollCurrentLine];
}

/**滚动到第一行自动刷新*/
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

/**给先上屏的数据覆盖从服务器返回的SequenceId*/
-(void) coverSequenceId:(NSMsgItem *)chatMessage
{
    for(ChatCellFrame *cellframe in [self gettingCountriesCellFrames])
    {
        if (cellframe.chatMessage.createTime == chatMessage.createTime)
        {
            cellframe.chatMessage.sequenceId = chatMessage.sequenceId;
        }
    }
}

-(NSMutableArray*) gettingCountriesCellFrames
{
    return self.dataSourceArray;
}

-(void) setDataSourceArray:(NSMutableArray *)dataSourceArray
{
    if (dataSourceArray.count > 0) {
        _dataSourceArray = [[ChannelManager sharedChannelManager] gettingCountryChannel].msgList;
        //[dataSourceArray mutableCopy];
//        [self.tableView reloadData];
    }
}

-(CGFloat) gettingOffsetY
{
    return self.offsetY;
}

@end
