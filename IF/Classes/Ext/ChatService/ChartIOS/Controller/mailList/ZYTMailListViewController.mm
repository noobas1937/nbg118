//
//  ZYTMailListViewController.m
//  IF
//
//  Created by 张彦涛 on 15/8/19.
//
//

#import "ZYTMailListViewController.h"
#import "ServiceInterface.h"
#import "TopHeadView.h"
#import "ChatChannel.h"
#import "ChannelManager.h"
#import "MailListCell.h"
#import "MailInfoDataModel.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatCellFrame.h"
#import "ChatServiceCocos2dx.h"
#import "ChattingVC.h"
#import "ZYTMailChattingVC.h"
@interface ZYTMailListViewController ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate>
{
    BOOL topButtonSelected;
}
@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong) UITableView *mailListTableView;
@property (nonatomic,strong)NSMutableArray *dataSourceArray;
@end

@implementation ZYTMailListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
    [self.headView addSubview:self.topHeaderView];
    [self.view addSubview:self.mailListTableView];
    [self gettingChannelList];
    [self.mailListTableView reloadData];
    topButtonSelected = NO;
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
        TopHeadView *view = [TopHeadView  topHeadView];
        view.titleNameLabel.text = @"邮件";
        view.frame = self.headView.frame;
        view.rightButton.hidden = NO;
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}
-(UITableView *)mailListTableView{
    if (_mailListTableView == nil) {
        UITableView *tableView = [[UITableView alloc]initWithFrame:CGRectMake(0, self.topHeaderView.bottom, kMainScreenWidth,self.footView.top - self.headView.height ) style:UITableViewStylePlain];
        tableView.delegate = self;
        tableView.dataSource = self;
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
        tableView.separatorStyle=UITableViewCellSeparatorStyleSingleLine;
        tableView.separatorColor = [UIColor brownColor];
        _mailListTableView = tableView;
    }
    return  _mailListTableView;
}
-(NSMutableArray *)dataSourceArray{
    if (_dataSourceArray == nil) {
        NSMutableArray *mubArray = [NSMutableArray array];
        _dataSourceArray = mubArray;
    }
    return _dataSourceArray;
}
#pragma mark -
#pragma mark data

-(void)gettingChannelList{
    NSArray * channelArray =[[ChannelManager sharedChannelManager].channel_map allValues];
    
    for (ChatChannel *channel  in channelArray) {
        channel.isHasGift = NO;
      
        DVLog(@"%@",[channel printAllPropertys]);
        if (channel.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL) {
             channel.msgList = [channel sortingMailWithCreateTimeDataSource:channel.msgList];
             MailInfoDataModel *maildata =  [channel.msgList lastObject];
            channel.timeString = [NSString stringWithTimeFormatWithCreateTime:maildata.creatTime];
            channel.contentsString = maildata.title;
            channel.isHasGift = maildata.rewardStatus;
            channel.faceImageString = maildata.picString;
            
            if(maildata.tabType == Mail_BigType_System){
                channel.nameString = @"系统";
            }else if ( maildata.tabType == Mail_BigType_Notice){
                channel.nameString = @"公告";
            }else if ( maildata.tabType == Mail_BigType_Studio){
                channel.nameString = @"龙族部落游戏工作室";
            }else if ( maildata.tabType == Mail_BigType_Fight){
                channel.nameString = @"战斗报告";
            }else if ( maildata.tabType == Mail_BigType_Mod){
                channel.nameString = maildata.fromName;
            }
            
        }else if(channel.channelType == IOS_CHANNEL_TYPE_USER){
            channel.msgList =[channel sortingDataForMsgItemWithCreatTimeSource:channel.msgList];
            ChatCellFrame *cellFrame = [channel.msgList lastObject];
            
            channel.contentsString =[NSString stringWithFormat:@"%@:%@",cellFrame.chatMessage.name,cellFrame.chatMessage.showMsg] ;
        }else if(channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
            channel.msgList =[channel sortingDataForMsgItemWithCreatTimeSource:channel.msgList];
            ChatCellFrame *cellFrame = [channel.msgList lastObject];
            if (cellFrame != nil) {
                DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
                channel.contentsString =[NSString stringWithFormat:@"%@:%@",cellFrame.chatMessage.name,cellFrame.chatMessage.showMsg] ;
            }
            
        }
    }
    self.dataSourceArray = (NSMutableArray *)channelArray;
}

#pragma mark -
#pragma mark TopHeadView delegate
-(void)clickButton{
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden= YES;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;

}
-(void)rightButtonAction:(UIButton *)sender{
    sender.selected = !sender.selected;
    
    if (sender.selected) {
        self.footView.top = kMainScreenHeight - self.footView.height;
       
    }else{
        self.footView.top = kMainScreenHeight  ;
    }
    self.mailListTableView.height = self.footView.top - self.headView.height;
    topButtonSelected = sender.selected;
    [self.mailListTableView reloadData];
}
#pragma mark -
#pragma mark 
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSourceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    MailListCell *cell = [MailListCell mailListCellWithTableView:tableView];
    cell.showSelectedButton =topButtonSelected;
    cell.chatChannel = [self.dataSourceArray objectAtIndex:indexPath.row];
//    DVLog(@"%@",indexPath);
//    DLog(@"%@",[cell.chatChannel printAllPropertys]);
    return cell;
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return [MailListCell mailListCellRowHeight];
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
      [tableView deselectRowAtIndexPath:indexPath animated:NO];
    ChatChannel *channel = [self.dataSourceArray objectAtIndex:indexPath.row];
    NSArray * array = channel.msgList;
    ChatCellFrame *cellFrame = [array lastObject];
     DVLog(@"%@",[channel printAllPropertys]);
    for ( ChatCellFrame  *cellFrame in array) {
//        DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
    }
    ZYTMailChattingVC *mailChatVC =[[ZYTMailChattingVC alloc]init];
    mailChatVC.chatChannel = channel;
    [self.navigationController pushViewController:mailChatVC animated:YES];
//    [[ServiceInterface serviceInterfaceSharedManager] setMailInfo:channel.channelID :cellFrame.chatMessage.mailId :channel.nameString :channel.channelType];
//    [[ServiceInterface serviceInterfaceSharedManager] settingChatRoomName:channel.nameString];
//    
//      [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController setCurrentChatChannel:channel];
//    
// 
//    if(!ChatServiceCocos2dx::isChatShowing_fun()){
//     
//
//        ChatServiceCocos2dx::m_channelType=channel.channelType;
//      
//        ChatServiceCocos2dx::m_isNoticItemUsed=false;
////        MailController::getInstance()->setOldOpenMailInfo(m_mailInfo);
//        ChatServiceCocos2dx::showChatIOSFrom2dx();
//    }
   
}
@end
