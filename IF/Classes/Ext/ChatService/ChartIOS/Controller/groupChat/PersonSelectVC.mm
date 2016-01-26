//
//  PersonSelectVC.m
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import "PersonSelectVC.h"
#import "ServiceInterface.h"
#import "SectionHeadView.h"
#import "UserGroup.h"
#import "NSUserInfo.h"
#import "MemberTableCell.h"
#import "UserManager.h"
#import "TopHeadView.h"

#import "TopButtonView.h"

#import "TopUIView.h"
#include "MailController.h"
#import "HeadEditView.h"
#import "CSAlertView.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "ChatServiceController.h"
#import "GameHost.h"
#import "ChannelManager.h"
#import "ChatServiceCocos2dx.h"
#import "SearchEditView.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatChannel.h"

typedef enum :NSUInteger {
    PSVCTopButtonSelected_left,
    PSVCTopButtonSelected_right,
}PSVCTopButtonSelected;


@interface PersonSelectVC ()<UITableViewDataSource,UITableViewDelegate,SectionHeadViewDelegate,MemberTableCellDelegate,TopHeadViewDelegate,UITextFieldDelegate,ZYTAlertViewDelegate,TopButtonViewDelegate,SearchEditViewDelegate>{
    CCArray * groupUidCCArr;
    UIView *headDefaultView;
    
//    NSMutableArray * addUidArr ;
    NSString *addNameString;
//    NSMutableArray *subUidArr;
    NSString *subNameString;
    NSUInteger alertCount;
    PSVCTopButtonSelected psVcTopButtonSelected;
}


@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (strong, nonatomic)  UITableView *tableView;
@property (nonatomic,strong) UIView *nameEditView ;

@property (nonatomic,strong) NSMutableArray *selectedMemArr;
@property (strong,nonatomic)  NSMutableArray * searchSelectedMemArr;
@property (strong,nonatomic)  NSMutableArray * addMemberArr;
@property (strong,nonatomic) NSMutableArray * subMemberArr;
//@property (strong ,nonatomic)NSMutableArray *rightAddMemberArr;
//@property (nonatomic,strong) NSMutableArray * repeatMemberArr;
//@property (strong,nonatomic)NSMutableArray *rightSubMemberArr;
@property (nonatomic,strong)NSArray *membersArray;//联盟所有成员

@property (nonatomic,strong) NSArray  * chatRoomMemberUidArr;//聊天室已经加入成员Uid列表

@property (nonatomic,strong) UserGroup * friendsGroup ;

@property (nonatomic,strong)TopHeadView *topView;

@property (nonatomic,strong)HeadEditView *headEditView;
//@property (strong,nonatomic) SearchEditView * searchEditView;

@end

@implementation PersonSelectVC

-(instancetype)initWithType:(PersonSelectVCType )vType{
    self = [super init];
    if  (self){
        self.selectedMemArr =[NSMutableArray array];
        self.personSelectVCType = vType;
        
        ChatChannel *allChannel = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
        if (allChannel){
            NSString *alliance_allUserUid = allChannel.memberUidString;
            NSArray *uidArr = [alliance_allUserUid componentsSeparatedByString:@"_"];
            NSMutableArray *tempArr = [NSMutableArray arrayWithCapacity:uidArr.count];
            NSUserInfo *selfUser= nil;
            for (NSString *uidString  in uidArr) {
                NSUserInfo *userInfo = [[UserManager sharedUserManager]gettingUser:uidString];
                if  (userInfo == nil){
                    userInfo =[[UserManager sharedUserManager]gettingUserInforWithUid:uidString];
                }
                if ([userInfo.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                    selfUser = userInfo;
                }
                userInfo.chooseState = ChooseState_normal;
                [tempArr addObject:userInfo];
            }
            if(selfUser){
                selfUser.chooseState = ChooseState_selected;
            }
            self.membersArray = [NSArray arrayWithArray:tempArr];
        }
//        [self managerMemberData];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingFriendsList];
         [ self managerFriendMemDataArr: [UserManager sharedUserManager].friends_ordinary ];
    }
    return self;
}
-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
//    [self managerMemberData];
}

-(BOOL)isRoomCreater{
    return [self.chatChannel.roomOwner isEqualToString:[UserManager sharedUserManager].currentUser.uid];
}
-(void)managerMemberData{

    for(NSString *uidString in self.memberInGroupArr){
//        DLog(@" uidString  :%@",uidString);
        
        if ([self isRoomCreater]){
            //我是房主
            
            for (NSUserInfo *tempUser in self.membersArray) {
//                 DLog(@" tempUser uid-:%@   ",tempUser.uid);
//                tempUser.chooseState = ChooseState_normal;//设置默认状态
                if ([tempUser.uid isEqualToString:uidString]) {
                    tempUser.chooseState = ChooseState_pressed;
                }
                if ([tempUser.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                    tempUser.chooseState = ChooseState_selected;
                }
            }
            for (NSUserInfo *tempUser in self.friendsGroup.memberArray) {
//                tempUser.chooseState = ChooseState_normal;//设置默认状态
                if ([tempUser.uid isEqualToString:uidString]) {
                    tempUser.chooseState = ChooseState_pressed;
                }
            }
        }else{
            //我不是房主
            for (NSUserInfo *tempUser in self.membersArray) {
//                tempUser.chooseState = ChooseState_normal;//设置默认状态
                if ([tempUser.uid isEqualToString:uidString]) {
                    tempUser.chooseState = ChooseState_selected;
                }
            }
            for (NSUserInfo *tempUser in self.friendsGroup.memberArray) {
//                tempUser.chooseState = ChooseState_normal;//设置默认状态
                if ([tempUser.uid isEqualToString:uidString]) {
                    tempUser.chooseState = ChooseState_pressed;
                }
            }
         
        }
        
    }
    
    
    //联盟人员分组
    NSMutableArray *rank1=[[NSMutableArray alloc]init];
    NSMutableArray *rank2=[[NSMutableArray alloc]init];
    NSMutableArray *rank3=[[NSMutableArray alloc]init];
    NSMutableArray *rank4=[[NSMutableArray alloc]init];
    NSMutableArray *rank5 =[[NSMutableArray alloc]init];
    NSArray *arr=@[rank5,rank4,rank3,rank2,rank1];
    
    NSMutableArray *newGroupArray = [[NSMutableArray alloc]init];
    for (NSUserInfo *tempInfo in self.membersArray) {
 
        if (tempInfo.allianceRank == 5) {
            [rank5 addObject:tempInfo];
        }else if (tempInfo.allianceRank == 4){
            [rank4 addObject:tempInfo];
        }else if (tempInfo.allianceRank == 3){
            [rank3 addObject:tempInfo];
        }else if ( tempInfo.allianceRank == 2){
            [rank2 addObject:tempInfo];
        }else{
            [rank1 addObject:tempInfo];
        }
    }
    
    for (NSArray *tempArr in arr) {
        if (tempArr.count>0) {
            
            UserGroup *tempGroup =[UserGroup userGroup];
            tempGroup.grade = [(NSUserInfo *)[tempArr objectAtIndex:0]allianceRank];
            tempGroup.open = YES;
            tempGroup.groupNameString= [[UserManager sharedUserManager]getRankLang:tempGroup.grade];
            tempGroup.memberArray = tempArr;
            [newGroupArray addObject:tempGroup];
        }
    }
    if (self.selectedMemArr.count> 0) {
        [self  settingNameEditViewHidden:NO];
        [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count ]];
    }else{
        
    }
    self.memberArr = newGroupArray;
    [self.tableView reloadData];
}

-(void)managerFriendMemDataArr:(NSArray *)vArray{
    NSMutableArray *friendTempArr =[NSMutableArray array];
    for (NSUserInfo *tempUser in vArray) {
        tempUser.chooseState = ChooseState_normal;
        [friendTempArr addObject:tempUser];
    }
    self.friendsGroup = [UserGroup userGroup];
    self.friendsGroup.memberArray = friendTempArr;
    self.friendsGroup.open = YES;
    self.friendsGroup.groupNameString =[NSString stringWithMultilingualWithKey:@"132103"];//   132103=好友列表
}



-(void)setMemberInGroupArr:(NSArray *)memberInGroupArr{
//    _memberInGroupArr = memberInGroupArr;
//    if (_memberInGroupArr.count>0) {
//        [self managerMemberData];
//    }
}
//拿取当前聊天室成员UID
-(void)memberUidWithChatRoom{
    NSMutableArray *tempArray =[NSMutableArray array ];
    NSArray *memberTempArr =[self.chatChannel.memberUidString componentsSeparatedByString:@"_"];
    for (NSString *tempUid in memberTempArr) {
        [tempArray addObject:tempUid];
    }
   _memberInGroupArr= tempArray;
    
    self.selectedMemArr = self.memberInGroupArr;//要提前加入已经在聊天室的成员
     [self managerMemberData];
}
-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    if (self.personSelectVCType == PersonSelectVCType_ChangeMember){
        [self memberUidWithChatRoom];
    }else{
        [self managerMemberData];
        [self.selectedMemArr addObject:[UserManager sharedUserManager].currentUser.uid];
    }
    if (self.selectedMemArr.count < 2){
        [self settingNameEditViewHidden:YES];
    }
    alertCount = 0;
   
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(searchUserNotifyAction:) name:kSearchUserWithNameString object:nil];
    [self.view addSubview:self.headView];
    [self.view addSubview: self.footView];
    self.footView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"chuzheng_frame02"]];
    [self.view addSubview: self.nameEditView ];
  
    [self.view addSubview:self.tableView];
    [self.view bringSubviewToFront:self.headView];
    
    TopHeadView *topView= [TopHeadView topHeadViewWithRightButtonShowFlag:NO andShowRightButtonType:TopHeadViewRightButtonType_ChatRoomMember];
    topView.frame = CGRectMake(0, 0, self.headView.width, self.headView.height/2);
    topView.topHeadViewDelegate = self;
    topView.titleNameLabel.text = [NSString stringWithMultilingualWithKey:@"105354"];//105354=多人聊天
    _topView = topView;
    [self.headView addSubview:topView];
    
    //顶部ButtonView
    TopButtonView *topButtonView =[TopButtonView topButtonViewWithLeftButtonName:[NSString stringWithMultilingualWithKey:@"132106"] andWithRightButtonName:[NSString stringWithMultilingualWithKey:@"105602"]];//105602 = 联盟   132106= 好友
    topButtonView.frame = CGRectMake(0, self.headView.height/2, self.headView.width, self.headView.height/2);
    [topButtonView settingLeftButtonSelected:YES];
    psVcTopButtonSelected = PSVCTopButtonSelected_left;
    topButtonView.topButtonViewDelegate = self;
    [self.headView addSubview:topButtonView];
    
    
    [self.nameEditView addSubview:self.headEditView];
//    [self.nameEditView addSubview:self.searchEditView];
//    [self settingSearchViewShow:NO];
    
    
 
    
    
    UIButton *qiutButton=[UIButton buttonWithType:UIButtonTypeCustom];
    NSString *quitNameString = [NSString stringWithMultilingualWithKey:@"105344"];// 105344=退出群组
    NSString *enterNameString =[NSString stringWithMultilingualWithKey:@"101274"];//101274=确定
    [qiutButton setTitle:quitNameString forState:UIControlStateNormal];
    [qiutButton setTitleColor:[UIColor colorWithRed:190/255.0 green:151/255.0 blue:90/255.0 alpha:1] forState:UIControlStateNormal];
    [qiutButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_red"] forState:UIControlStateNormal];
    qiutButton.frame = CGRectMake( (self.footView.width-120*2-40)/2, (self.footView.height-40)/2, 120, 40);
    qiutButton.tag = 1001;
    [qiutButton addTarget:self action:@selector(enterButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    if (self.personSelectVCType == PersonSelectVCType_New){
         qiutButton.enabled = NO;
    }else if(self.personSelectVCType == PersonSelectVCType_ChangeMember ){
        qiutButton.enabled = YES;
    }
    [self.footView addSubview:qiutButton];
    
    UIButton *enterButton=[UIButton buttonWithType:UIButtonTypeCustom];
    [enterButton setTitle:enterNameString forState:UIControlStateNormal];
    [enterButton setTitleColor:[UIColor colorWithRed:190/255.0 green:151/255.0 blue:90/255.0 alpha:1] forState:UIControlStateNormal];
    [enterButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_green"] forState:UIControlStateNormal];
    enterButton.tag = 1002;
    enterButton.frame = CGRectMake( qiutButton.right + 40, (self.footView.height-40)/2, 120, 40);
    [enterButton addTarget:self action:@selector(enterButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.footView addSubview:enterButton];
    
   
    
    if (self.selectedMemArr.count> 1) {
        [self  settingNameEditViewHidden:NO];
        [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count ]];
        
    }
}


#pragma mark -
#pragma mark 设置VC  title 
-(void)settingVCTitleNameWithString:(NSString *)vNameString {
    self.topView.titleNameLabel.text  = vNameString;
}
#pragma mark getter

-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView =[[UIView alloc]init];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.14);
        } else{
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.14);
        }
        _headView = headView;
    }
    return _headView;
}

-(UIView *)footView{
    if (_footView == nil) {
        UIView *footView =[[UIView alloc]init];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            footView.frame = CGRectMake(0, kMainScreenHeight -kMainScreenHeight *0.07, kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            footView.frame = CGRectMake(0, kMainScreenHeight -kMainScreenHeight *0.07, kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _footView = footView;
    }
    return _footView;
}
-(UIView *)nameEditView{
    if (_nameEditView == nil) {
        UIView *nameEditView =[[UIView alloc]init];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            nameEditView.frame = CGRectMake(0, self.headView.bottom,  kMainScreenWidth, kMainScreenHeight * 0.14) ;
        }else{
            nameEditView.frame = CGRectMake(0, self.headView.bottom,  kMainScreenWidth, kMainScreenHeight * 0.14) ;
        }
        _nameEditView = nameEditView;
    }
    return _nameEditView;
}

-(UITableView *)tableView{
    if  (_tableView == nil){
//        DVLog(@" _nameEditView  %@",NSStringFromCGRect(self.nameEditView.frame));
//        DVLog(@" _headView  %@",NSStringFromCGRect(self.headView.frame));
//        DVLog(@" _footView  %@",NSStringFromCGRect(self.footView.frame));
        UITableView *tableView =[[UITableView alloc]initWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height - self.footView.height) style:UITableViewStylePlain];
        tableView.delegate = self;
        tableView.dataSource = self;
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(btnClick)];
        [tableView addGestureRecognizer:tap];
        _tableView = tableView;
    }
    return _tableView;
}

-(HeadEditView *)headEditView{
    if (_headEditView == nil    ){
        HeadEditView *headEditView =[HeadEditView headEditView];
        headEditView.frame = CGRectMake(0, 0, self.nameEditView.width, self.nameEditView.height);
        headEditView.editView.delegate = self;
        
        headEditView.editView.text = self.chatChannel.customName;
       _headEditView = headEditView;
    }
    return _headEditView;
}


-(NSMutableArray *)addMemberArr{
    if (_addMemberArr == nil    ){
        NSMutableArray *array =[NSMutableArray array];
        _addMemberArr = array;
    }
    return _addMemberArr;
}
-(NSMutableArray *)subMemberArr{
    if (_subMemberArr == nil    ){
        NSMutableArray *array =[NSMutableArray array];
        _subMemberArr = array;
    }
    return _subMemberArr;
}

//-(NSMutableArray *)rightAddMemberArr{
//    if  (_rightAddMemberArr == nil){
//        _rightAddMemberArr = [NSMutableArray array];
//    }
//    return _rightAddMemberArr;
//}

//-(NSMutableArray *)rightSubMemberArr{
//    if  (_rightSubMemberArr == nil){
//        _rightSubMemberArr =[NSMutableArray array];
//    }
//    return _rightSubMemberArr;
//}
//-(NSMutableArray *)repeatMemberArr{
//    if  (_repeatMemberArr == nil){
//        _repeatMemberArr =[NSMutableArray array];
//    }
//    return _repeatMemberArr;
//}
-(NSMutableArray *)searchSelectedMemArr{
    if  (_searchSelectedMemArr == nil){
        NSMutableArray *array =[NSMutableArray array];
        _searchSelectedMemArr = array;
    }
    return _searchSelectedMemArr;
}


/** 点击关闭键盘  */
-(void)btnClick{
    [self.headEditView.editView resignFirstResponder];
//    [self.searchEditView.searchEditTextField resignFirstResponder];

}

#pragma mark -
#pragma mark bottomButton Action
- (void)enterButtonAction:(UIButton *)sender {
    [_headEditView.editView resignFirstResponder];
    
    if (sender.tag == 1001) {
        //退出联盟
        CSAlertView *alertView =[CSAlertView alertViewWithTitleString:nil];
        alertView.titleType = ZYTAlertViewTitleType_quit;
        NSString *title = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_QUIT_CHATROOM];
        alertView.nameString = title;
        alertView.alertViewDelegate  = self;
        [self.view addSubview:alertView];
        [self.view bringSubviewToFront:alertView];
        alertCount =alertCount +1;

    }else if (sender.tag == 1002){
        //确定
        if(_headEditView.editView.text.length >0   ){
            if (![_headEditView.editView.text isEqualToString:self.chatChannel.customName]) {
                DVLog(@"名字不一样了");
                
                CSAlertView *alertView =[CSAlertView alertViewWithTitleString:nil];
                alertView.titleType = ZYTAlertViewTitleType_reName;
                alertView.nameString = _headEditView.editView.text;
                alertView.alertViewDelegate  = self;
                [self.view addSubview:alertView];
                [self.view bringSubviewToFront:alertView];
                alertCount =alertCount +1;
            }
        }
        if (self.personSelectVCType == PersonSelectVCType_New) {
            self.roomMemberName = @"";
            self.roomMemberUid = @"";
            
            for (NSString *tempUserUid in self.selectedMemArr) {
                NSUserInfo *user = [[UserManager sharedUserManager]gettingUser:tempUserUid];
                if  (user == nil){
                    user =[[UserManager sharedUserManager]gettingUserInforWithUid:tempUserUid];
                }
                
                if (self.roomMemberName.length >0) {
                    self.roomMemberName =   [self.roomMemberName stringByAppendingString:@"、"];
                }
                if(user.userName.length > 0){
                    self.roomMemberName =   [self.roomMemberName stringByAppendingString:user.userName];
                }else{
                    self.roomMemberName =   [self.roomMemberName stringByAppendingString:user.uid];
                }
                
                
                
                if (self.roomMemberUid.length >0) {
                    self.roomMemberUid = [self.roomMemberUid stringByAppendingString:@"|"];
                }
                
                self.roomMemberUid = [self.roomMemberUid stringByAppendingString:tempUserUid];
                
            }
            DVLog(@"%@",self.roomMemberName);
            
            CSAlertView *alertView =[CSAlertView alertViewWithTitleString:nil];
            alertView.titleType = ZYTAlertViewTitleType_creat;
            alertView.nameString = _roomMemberName;
            alertView.alertViewDelegate  = self;
            [self.view addSubview:alertView];
            [self.view bringSubviewToFront:alertView];
            alertCount = 1;
            
        }
        else if (self.personSelectVCType == PersonSelectVCType_ChangeMember){
            for (NSUserInfo *user in self.searchSelectedMemArr) {
                [self.addMemberArr addObject:user];
            }
            if (self.addMemberArr.count>0) {
 
                addNameString = @"";
                for (NSString   *tempUserUid in self.addMemberArr) {
                    NSUserInfo *user = [[UserManager sharedUserManager]gettingUser:tempUserUid];
                    if  (user == nil){
                        user =[[UserManager sharedUserManager]gettingUserInforWithUid:tempUserUid];
                    }

                    
                    if (addNameString.length >0 ) {
                        addNameString =[addNameString stringByAppendingString:@"|"];
                    }
                    addNameString =[addNameString stringByAppendingString:user.userName];
                }
                CSAlertView *alertView =[CSAlertView alertViewWithTitleString:nil];
                alertView.titleType = ZYTAlertViewTitleType_add;
                alertView.nameString = addNameString;
                alertView.alertViewDelegate  = self;
                [self.view addSubview:alertView];
                [self.view bringSubviewToFront:alertView];
                alertCount = alertCount +1;
            }
            
            
            if (self.subMemberArr.count>0) {
 
                subNameString = @"";
                for (NSString   *tempUserUid in self.subMemberArr) {
                    NSUserInfo *user = [[UserManager sharedUserManager]gettingUser:tempUserUid];
                    if  (user == nil){
                        user =[[UserManager sharedUserManager]gettingUserInforWithUid:tempUserUid];
                    }
                    if (subNameString.length >0 ) {
                        subNameString =[subNameString stringByAppendingString:@"|"];
                    }
                    subNameString =[subNameString stringByAppendingString:user.userName];
                }
                CSAlertView *alertView =[CSAlertView alertViewWithTitleString:nil];
                alertView.titleType = ZYTAlertViewTitleType_sub;
                alertView.nameString = subNameString;
                alertView.alertViewDelegate  = self;
                [self.view addSubview:alertView];
                [self.view bringSubviewToFront:alertView];
                alertCount = alertCount +1;
            }
            
        }

    }
    
    
}

#pragma mark -
#pragma mark 组操作
-(void) commitCreateRoom
{
    if (self.personSelectVCType == PersonSelectVCType_New) {
        
        NSString *roomName=@"";
        if  (_headEditView.editView.text.length>0){
            roomName = _headEditView.editView.text;
        }else{
            roomName = [NSString stringWithMultilingualWithKey:@"105354"]; //     105354 = 多人聊天

        }
        if (  self.personVCOpenFrom == PersonVCOpenFrom_OC_OneMailChat ) {
            //从单人邮件打开的多人聊天创建
            NSArray *tempArray =[self.roomMemberUid componentsSeparatedByString:@"|"];
            CCArray *ccArr =CCArray::create();
            for (NSString *str in tempArray) {
                
                ccArr->addObject(CCString::create([str UTF8String]));
            }
            MailController::getInstance()->createChatRoom([self.roomMemberName UTF8String], ccArr , [roomName UTF8String], "");
        }else if( self.personVCOpenFrom == PersonVCOpenFrom_CocosAdd){
            //从C++页面打开的多人聊天创建
            [[ChatServiceController chatServiceControllerSharedManager].gameHost selectChatRoomMember:roomName :self.roomMemberName :self.roomMemberUid ];

        }else if(self.personVCOpenFrom == PersonVCOpenFrom_OC_MailListAdd){
            if ([self.personVCDelegate isKindOfClass:[UIViewController class]] && [self.personVCDelegate respondsToSelector:@selector(personSelectVCCreatChatRoomWithRoomMemberName:andWithUidString:andWithRoomName:)]){
                [self.personVCDelegate personSelectVCCreatChatRoomWithRoomMemberName:self.roomMemberName andWithUidString:self.roomMemberUid andWithRoomName:roomName];
            }
        }
           [self clickButton];
    }

}

-(void)commitAddRoomMember{
     if (self.personSelectVCType == PersonSelectVCType_ChangeMember ){
        if (self.addMemberArr.count>0){
            [[ChatServiceController chatServiceControllerSharedManager].gameHost addGroupChatMemberWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid andWithMemberUidArray:self.addMemberArr andWithMemberName:addNameString];
        }
        [self clickButton];
    }

}
-(void)commitSubRoomMember{
    if (self.personSelectVCType == PersonSelectVCType_ChangeMember ){
        if (self.subMemberArr.count>0) {
            [[ChatServiceController chatServiceControllerSharedManager].gameHost subGroupChatMemberWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid andWithMemberUidArray:self.subMemberArr andWithMemberName:subNameString ];
        }
        [self clickButton];
    }
}
-(void)commitRenameMember{
    [[ChatServiceController chatServiceControllerSharedManager].gameHost  reNameGroupChatTitleWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid andWithGroupName:_headEditView.editView.text];
    [self clickButton];
}
-(void)commitQuitGroup{
    [[ChatServiceController chatServiceControllerSharedManager].gameHost  quitGroupChatWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid];
    [self clickButton];
}
#pragma mark -
#pragma mark tableView delegate
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    if (psVcTopButtonSelected == PSVCTopButtonSelected_right) {
        return self.memberArr.count;
    }else{
        return 1;
    }
    
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    
    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
        UserGroup *tempGroup = self.friendsGroup;
        if (tempGroup.isOpen) {
            return tempGroup.memberArray.count;
        }else{
            return  0;
        }
    }else{
        UserGroup *tempGroup = [self.memberArr objectAtIndex:section];
        if (tempGroup.isOpen) {
            return tempGroup.memberArray.count;
        }else{
            return  0;
        }
    }
    
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    MemberTableCell *cell =  [MemberTableCell cellWithTableView:tableView];
    cell.selectionStyle= UITableViewCellSelectionStyleNone;
    cell.memberCellDelegate = self;
    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
         cell.userInfo = [self.friendsGroup.memberArray objectAtIndex:indexPath.row];
    }else{
         cell.userInfo = [[[self.memberArr objectAtIndex:indexPath.section] memberArray] objectAtIndex:indexPath.row];
    }
   
    return cell;
    
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{


    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return 80;
    } else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        return 60;
    }else{
        return 60;
    }
   
}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
 
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return 50;
    } else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        return 30;
    }else{
        return 30;
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    return 0.25;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    
    if (psVcTopButtonSelected == PSVCTopButtonSelected_right) {
        SectionHeadView *headerView =[SectionHeadView sectionHeadViewWithTableView:tableView];
        headerView.sectionHeadViewDelegate = self;
        headerView.userGroup = [self.memberArr objectAtIndex:section];
        headerView.backgroundColor = [UIColor clearColor];
        return headerView;
    }else{
        SectionHeadView *headerView =[SectionHeadView sectionHeadViewWithTableView:tableView];
        headerView.sectionHeadViewDelegate = self;
        headerView.userGroup = self.friendsGroup;
        return headerView;
    }
    
}

#pragma mark -
#pragma mark topButtonView delegate
-(void)topButtonViewBtnPressedAction:(UIButton *)sender{
    if (sender.tag == 101){
        //左侧按钮
        DVLog(@"左侧按钮");
        psVcTopButtonSelected = PSVCTopButtonSelected_left;
        //计算坐标
//        [self settingNameEditViewHidden:NO];
        //控制子视图显示
//        [self settingSearchViewShow:YES];
      
    }else if(sender.tag == 102){
        
        //右侧按钮
        DVLog(@"右侧按钮");
        psVcTopButtonSelected = PSVCTopButtonSelected_right ;
        
        
//        if (self.selectedMemArr.count>0) {
//            [self settingNameEditViewHidden:NO];
//        }else{
//            [self settingNameEditViewHidden:YES];
//        }
//        
//        [self settingSearchViewShow:NO];
        
    }
    if (self.selectedMemArr.count>1) {
        [self settingNameEditViewHidden:NO];
    }else{
        [self settingNameEditViewHidden:YES];
    }
    
    [self.tableView reloadData];
}


#pragma mark -
#pragma mark CSAlertViewDelegate
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView{
    alertCount = alertCount -1;
    if (vAlertView.titleType == ZYTAlertViewTitleType_creat) {
        [self commitCreateRoom ];
        
    }else if(vAlertView.titleType == ZYTAlertViewTitleType_add){
        [self commitAddRoomMember];
    }else if(vAlertView.titleType == ZYTAlertViewTitleType_sub){
        [self commitSubRoomMember];
    }else if (vAlertView.titleType == ZYTAlertViewTitleType_reName){
        [self commitRenameMember];
    }else if(vAlertView.titleType == ZYTAlertViewTitleType_quit){
        [self commitQuitGroup];
    }
    
}
-(void)cancalBtnAction:(CSAlertView *)vAlertView{
    if (vAlertView.titleType == ZYTAlertViewTitleType_reName) {
        self.headEditView.editView.text =self.chatChannel.customName;
    }
    alertCount = alertCount -1;
}
#pragma mark -
#pragma mark TopHeadViewDelegate
- (void)clickButton {
    if (alertCount != 0) {
        return;
    }
   
    
    if(ChatServiceCocos2dx::Mail_OC_Native_Enable){
          //新版OC原生邮件列表
        if  ([[ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController isKindOfClass:[UINavigationController class]])
        {
        
            UINavigationController *nav = [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController;
            if ([nav.viewControllers objectAtIndex:0] == self){
                [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden= YES;
                [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
            }else{
                [self.navigationController popViewControllerAnimated:YES];
            }

        }else {
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden= YES;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;

        }
        
      
    }else{
        [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden= YES;
        [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
    }
    
}

#pragma mark -
#pragma mark notify Action
-(void)searchUserNotifyAction:(NSNotification *)vNotify{
    DVLog(@"%@",vNotify.userInfo);
    NSArray *userArray =[vNotify.userInfo objectForKey:@"users"];
//    [self managerSearchMemberData:userArray];
}
#pragma mark -
#pragma mark HeadEditViewDelegate
- (void)editViewEditWithtextField:(UITextField *)vTextField{
    DLog(@"vTextField.text :%@",vTextField.text);
}


- (void)textFieldDidBeginEditing:(UITextField *)textField{
    
}

- (void)textFieldDidEndEditing:(UITextField *)textField{
    DVLog(@"%@",textField.text);
}
#pragma mark -
#pragma mark SectionHeadView Delegate
- (void)clickHeadView
{
    [self.tableView reloadData];
}
#pragma mark -
#pragma mark  MemberTableCellDelegate
- (void)clickButtonActionWithCell:(MemberTableCell *)vCell
                andSelectedButton:(UIButton *)vSender{
    NSIndexPath *tempIndexPath =[self.tableView indexPathForCell:vCell];
    NSUserInfo *tempUserInfo =vCell.userInfo;
    if (tempUserInfo.chooseState == ChooseState_normal) {
        vSender.enabled = YES;
        vSender.selected = YES;
        tempUserInfo.chooseState = ChooseState_pressed;
  
        [self groupMemberAddWithUserInfo:tempUserInfo];
 
    }else if (tempUserInfo.chooseState == ChooseState_pressed){
        vSender.enabled = YES;
        vSender.selected = NO;
        tempUserInfo.chooseState = ChooseState_normal;
        [self groupMemberRemoveWithUserInfo:tempUserInfo];
    }else {
        
    }
    
}
//修改userinfo模型的选择状态
-(void)changeWithAnotherListArrayForUserChooseStateWithUid:(NSString *)vUid
                                           andWithAddOrSub:(int )vAddOrSubFlag{
    if (vAddOrSubFlag == 1) {
        //加
        if (psVcTopButtonSelected == PSVCTopButtonSelected_right){
            for (NSUserInfo *user in self.membersArray) {
                if ([user.uid isEqualToString:vUid]){
                    user.chooseState = ChooseState_pressed;
                }
            }
        }else{
            for (NSUserInfo *user in self.friendsGroup.memberArray) {
                if ([user.uid isEqualToString:vUid]){
                    user.chooseState = ChooseState_pressed;
                }
            }
        }
        
    }else if ( vAddOrSubFlag == 0){
        //减
        if (psVcTopButtonSelected == PSVCTopButtonSelected_right){
            for (NSUserInfo *user in self.membersArray) {
                if ([user.uid isEqualToString:vUid]){
                    user.chooseState = ChooseState_normal;
                }
            }
        }else{
            for (NSUserInfo *user in self.friendsGroup.memberArray) {
                if ([user.uid isEqualToString:vUid]){
                    user.chooseState = ChooseState_normal;
                }
            }
        }
    }
    

}


-(void)groupMemberAddWithUserInfo:(NSUserInfo*)vUserInfo{
    [self changeWithAnotherListArrayForUserChooseStateWithUid:vUserInfo.uid andWithAddOrSub:1];
    
    NSString *tempUidStr = nil;
    for (NSString *uid in self.subMemberArr) {
        if  ([uid isEqualToString:vUserInfo.uid]){
            tempUidStr = vUserInfo.uid;
        }
    }
    if (tempUidStr){
        [self.subMemberArr removeObject:tempUidStr];
    }else{
        [self.addMemberArr addObject:vUserInfo.uid];
        [self.selectedMemArr addObject:vUserInfo.uid];
    }
 
    
    
    if (self.selectedMemArr.count >1) {
        if  (self.personSelectVCType == PersonSelectVCType_New){
            [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count  ]];
            [self settingNameEditViewHidden:NO];
        }else{
            if ([self isRoomCreater]){
                [self settingNameEditViewHidden:NO];
                [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count  ]];
            }else{
                [self settingNameEditViewHidden:YES];
                [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count ]];
            }
        }
       
    }
 
        [self.tableView reloadData];

}
-(void)groupMemberRemoveWithUserInfo:(NSUserInfo *)vUserInfo{
        [self changeWithAnotherListArrayForUserChooseStateWithUid:vUserInfo.uid andWithAddOrSub:0];
 
    NSString *tempUid = nil;
 
    for (NSString *str in self.addMemberArr) {
        if  ([vUserInfo.uid isEqualToString:str]){
            tempUid = str;
            break;
        }
    }
    if  (tempUid == nil){
        [self.subMemberArr addObject:vUserInfo.uid];
    }else{
        [self.addMemberArr removeObject:tempUid];
    }

     tempUid = nil;
    for (NSString *str in self.selectedMemArr) {
        if  ([vUserInfo.uid isEqualToString:str]){
            tempUid = str;
            break;
        }
    }
    [self.selectedMemArr removeObject:tempUid];
 
 
    if (self.selectedMemArr.count == 1) {
 
        [self settingNameEditViewHidden:YES];
        [self.tableView reloadData];
        [self settingVCTitleNameWithString:[NSString stringWithMultilingualWithKey:@"105354"]];//105354=多人聊天]
        
    }else if(self.selectedMemArr.count > 1){
        
        [self settingNameEditViewHidden:NO];
        [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count ]];
        
    }
     [self.tableView reloadData];
}
-(void)settingNameEditViewHidden:(BOOL)isHidden{
    if(isHidden){
        //设置隐藏
        self.tableView.top = self.headView.bottom;
        self.tableView.height = kMainScreenHeight - self.headView.height - self.footView.height;
        

    }else{
        //设置显示
        self.tableView.top = self.nameEditView.bottom;
        self.tableView.height = kMainScreenHeight - self.headView.height - self.footView.height - self.nameEditView.height;

    }
}


@end
