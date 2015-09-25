//
//  ChattingVC.m
//  IF
//
//  Created by 张彦涛 on 15/6/30.
//
//

#import "ChattingVC.h"
#import "ChatTableView.h"
#import "ServiceInterface.h"
#import "ChatCellIOS.h"
#import "ZYTChatSystemCell.h"
#import "UITableView+FDTemplateLayoutCell.h"
@interface ChattingVC ()<UIScrollViewDelegate,UITableViewDataSource,UITableViewDelegate>


@property (strong,nonatomic) UIView * headView;
@property (strong,nonatomic) UIView * bottomView;
@property (strong,nonatomic)UIScrollView *chatScrollView;
@property (nonatomic,strong) ChatTableView * countryTableView;
@property (nonatomic,strong) ChatTableView * allianceTableView;

@property (nonatomic,strong) NSMutableArray * dataSourceArray;

@end
 

@implementation ChattingVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.view addSubview:self.headView];
    UIButton *button =[UIButton buttonWithType:UIButtonTypeCustom];
    [button setTitle:@"返回" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonAction) forControlEvents:UIControlEventTouchUpInside];
    button.frame = CGRectMake(0, 0, 60, 40);
    [self.headView addSubview:button];
    
    [self.view addSubview:self.bottomView];
    [self.view addSubview:self.chatScrollView];
    [self.chatScrollView addSubview:self.countryTableView];
    [self.chatScrollView addSubview:self.allianceTableView];
  
}


-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    self.dataSourceArray = chatChannel.msgList;
    [self.countryTableView reloadData];
}
#pragma mark -
#pragma mark getter
-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight *0.07)];
        headView.backgroundColor = [UIColor grayColor];
        _headView = headView;
    }
    return _headView;
}
-(UIView *)bottomView{
    if (_bottomView == nil) {
        UIView *bottomView =[[UIView alloc]initWithFrame:CGRectMake(0, kMainScreenHeight - kMainScreenHeight*0.1, kMainScreenWidth, kMainScreenHeight * 0.1)];
        bottomView.backgroundColor =[UIColor grayColor];
        _bottomView = bottomView;
    }
    return _bottomView;
}

-(UIScrollView *)chatScrollView{
    if (_chatScrollView == nil) {
        UIScrollView *scrollerView =[[UIScrollView alloc]initWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height - self.bottomView.height)];
        scrollerView.contentSize = CGSizeMake(kMainScreenWidth* 2,0);
        scrollerView.pagingEnabled = YES;
        //水平滚动条
        scrollerView.showsHorizontalScrollIndicator =NO;
        _chatScrollView = scrollerView;
        _chatScrollView.backgroundColor = [UIColor redColor];
    }
    return _chatScrollView;
}

-(ChatTableView *)countryTableView{
    if (_countryTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(0, 0, self.chatScrollView.width, self.chatScrollView.height) style:UITableViewStylePlain];
        tableView.backgroundColor = [UIColor purpleColor];
        _countryTableView = tableView;
        _countryTableView.dataSource = self;
        _countryTableView.delegate = self;
    }
    return _countryTableView;
}
-(ChatTableView *)allianceTableView{
    if (_allianceTableView == nil) {
        ChatTableView *tableView =[[ChatTableView alloc]initWithFrame:CGRectMake(self.chatScrollView.width, 0, self.chatScrollView.width, self.chatScrollView.height) style:UITableViewStylePlain];
         tableView.backgroundColor = [UIColor orangeColor];
        _allianceTableView = tableView;
    }
    return _allianceTableView;
}
-(void)buttonAction{
//    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
//    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = YES;
    [self.navigationController popViewControllerAnimated:YES];
}
-(NSMutableArray *)dataSourceArray{
    if (_dataSourceArray == nil) {
        NSMutableArray *array = [NSMutableArray array ];
        _dataSourceArray = array;
    }
    return _dataSourceArray;
}

#pragma mark -
#pragma mark tableView delegate


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSourceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
 
    
    static NSString *identify = @"mailIdentify";
    ChatCellFrame * cellFrame =[self.dataSourceArray objectAtIndex:indexPath.row];
    
    DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
    if (cellFrame.chatMessage.post == 100) {
        
        ZYTChatSystemCell *cell =[ZYTChatSystemCell chatSystemCellWithTableView:tableView];
        [cell setChatCellFrame:cellFrame];
        
        cell.backgroundColor=[UIColor clearColor];
        
        return cell;
        
    }else{
        
        ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:identify];
        
        
        if (cell == nil) {
            cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];
        }
        
        //       cell=[tableView dequeueReusableCellWithIdentifier:mailCell forIndexPath:indexPath];
        
        //        for (UIView *subView in cell.contentView.subviews)
        //        {
        //            [subView removeFromSuperview];
        //        }
        //
        cell.backgroundColor=[UIColor clearColor];
        
        [cell setCellFrame:cellFrame];
        return cell;
        
    }
}



-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 100;
}

#pragma mark -
#pragma mark UIScrollView 代理方法

/**  当scrollView正在滚动就会调用 */
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
 
}

/** 开始拖拽的时候调用 */
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
 
}


/**  停止拖拽的时候调用 */
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
 
}
@end
