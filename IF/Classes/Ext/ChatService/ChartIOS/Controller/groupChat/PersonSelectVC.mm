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
#import "ZYTAlertView.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "ChatServiceController.h"
#import "GameHost.h"
#import "ChannelManager.h"
#import "ChatServiceCocos2dx.h"
#import "SearchEditView.h"


typedef enum :NSUInteger {
    PSVCTopButtonSelected_left,
    PSVCTopButtonSelected_right,
}PSVCTopButtonSelected;


@interface PersonSelectVC ()<UITableViewDataSource,UITableViewDelegate,SectionHeadViewDelegate,MemberTableCellDelegate,TopHeadViewDelegate,UITextFieldDelegate,ZYTAlertViewDelegate,TopButtonViewDelegate,SearchEditViewDelegate>{
    CCArray * groupUidCCArr;
    UIView *headDefaultView;
    
    NSMutableArray * addUidArr ;
    NSString *addNameString;
    NSMutableArray *subUidArr;
    NSString *subNameString;
    NSUInteger alertCount;
    PSVCTopButtonSelected psVcTopButtonSelected;
}


@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (strong, nonatomic)  UITableView *tableView;
@property (nonatomic,strong) UIView *nameEditView ;

@property (nonatomic,strong)NSMutableArray *selectedMemArr;
@property (strong,nonatomic)  NSMutableArray * searchSelectedMemArr;
@property (strong,nonatomic)  NSMutableArray * addMemberArr;
@property (strong,nonatomic) NSMutableArray * subMemberArr;
@property (nonatomic,strong)NSArray *membersArray;

@property (nonatomic,strong)TopHeadView *topView;

@property (nonatomic,strong)HeadEditView *headEditView;
@property (strong,nonatomic) SearchEditView * searchEditView;

@end

@implementation PersonSelectVC

-(instancetype)initWithType:(PersonSelectVCType )vType{
    self = [super init];
    if  (self){
        self.selectedMemArr =[NSMutableArray array];
        self.personSelectVCType = vType;
        
        NSArray *di = [(NSDictionary *)[UserManager sharedUserManager].allianceMemberInfoMap allValues];
        
        DVLog(@"%d",[di count]);
        
        self.membersArray =  [NSMutableArray arrayWithArray: [(NSDictionary *)[UserManager sharedUserManager].allianceMemberInfoMap allValues]];
        [self managerMemberData];
    }
    return self;
}
-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    [self managerMemberData];
}

-(BOOL)isRoomCreater{
    return [self.chatChannel.roomOwner isEqualToString:[UserManager sharedUserManager].currentUser.uid];
}
-(void)managerMemberData{
    NSMutableArray *rank1=[[NSMutableArray alloc]init];
    NSMutableArray *rank2=[[NSMutableArray alloc]init];
    NSMutableArray *rank3=[[NSMutableArray alloc]init];
    NSMutableArray *rank4=[[NSMutableArray alloc]init];
    NSMutableArray *rank5 =[[NSMutableArray alloc]init];
    NSArray *arr=@[rank5,rank4,rank3,rank2,rank1];
    
    NSMutableArray *newGroupArray = [[NSMutableArray alloc]init];
    [self.selectedMemArr removeAllObjects];
    for (NSUserInfo *tempInfo in self.membersArray) {
        tempInfo.chooseState = ChooseState_normal;
//        DLog(@"self.chatChannel.roomOwner %@",self.chatChannel.roomOwner);
//        DLog(@"[UserManager sharedUserManager].currentUser.uid %@",[UserManager sharedUserManager].currentUser.uid);
//        DLog(@"tempInfo uid  %@",tempInfo.uid);

        if ([self isRoomCreater]) {
        //我是房主
            if ([tempInfo.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                tempInfo.chooseState = ChooseState_selected;
            }else{
                for (NSString * userUidString in self.memberInGroupArr) {
                    if  ([userUidString isEqualToString:tempInfo.uid]){
                        tempInfo.chooseState = ChooseState_pressed;
                        [self.selectedMemArr addObject:tempInfo];
                    }
                }
            }
        }else{
            for (NSString * userUidString in self.memberInGroupArr) {
                if  ([userUidString isEqualToString:tempInfo.uid]){
                    tempInfo.chooseState = ChooseState_selected;
                    [self.selectedMemArr addObject:tempInfo];
                }
            }

        }
        
        
        if  (self.personSelectVCType == PersonSelectVCType_New){
            if ([tempInfo.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                tempInfo.chooseState = ChooseState_selected;
            }
        }
        
        if (tempInfo.allianceRank == 1) {
            [rank1 addObject:tempInfo];
        }else if (tempInfo.allianceRank == 2){
            [rank2 addObject:tempInfo];
        }else if (tempInfo.allianceRank == 3){
            [rank3 addObject:tempInfo];
        }else if ( tempInfo.allianceRank == 4){
            [rank4 addObject:tempInfo];
        }else{
            [rank5 addObject:tempInfo];
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
        [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count+1 ]];
    }else{
        
    }
    self.memberArr = newGroupArray;
    [self.tableView reloadData];
}


-(void)managerSearchMemberData:(NSArray *)vArray{
    NSMutableArray *existingArray =[NSMutableArray array];
    NSMutableArray *searchAllUserArray =[NSMutableArray array];
    NSArray *arr=@[existingArray,self.searchSelectedMemArr,searchAllUserArray];
    for ( NSUserInfo *tempUserInfo in vArray) {
        tempUserInfo.chooseState = ChooseState_normal;
        if ([tempUserInfo.uid  isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
            //是否是我自己的uid
            tempUserInfo.chooseState = ChooseState_selected;
            [existingArray addObject:tempUserInfo];
        }else{
            for (NSUserInfo *user in self.selectedMemArr) {
                if ([user.uid isEqualToString:tempUserInfo.uid]) {
                //搜索的用户已经在我们的联盟中
                   
                    tempUserInfo.chooseState = ChooseState_selected;
                    
                    [existingArray addObject:tempUserInfo];
                    
                }
            }
            
//            for (NSUserInfo *user in self.searchSelectedMemArr){
//                 if ( user ==tempUserInfo ) {
//                     user.chooseState =  ChooseState_pressed;
//                 }
//            }
            for (int x= 0; x<self.searchSelectedMemArr.count; x++) {
                NSUserInfo *user =[self.searchSelectedMemArr objectAtIndex:x];
                if ([user.uid isEqualToString:tempUserInfo.uid]) {
                    //搜索的用户已经添加到带加入数组中
                    tempUserInfo.chooseState = ChooseState_pressed;
                    [self.searchSelectedMemArr setObject:tempUserInfo atIndexedSubscript:x ];
                
                }
            }
        }
        [searchAllUserArray addObject:tempUserInfo];
    }
    
    NSMutableArray *seachGroupArr = [NSMutableArray array];
    for (NSArray *tempArray  in arr ) {
       
            UserGroup *tempGroup =[UserGroup userGroup];
            tempGroup.open = YES;
            if(tempArray == existingArray){
                tempGroup.grade = 1;
                 tempGroup.groupNameString=[LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_SELECTED_MEMBER];
            }else if (tempArray == searchAllUserArray){
                tempGroup.grade = 3;
                 tempGroup.groupNameString=[LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_SEARCH_RESULT];
            }else if (tempArray == self.searchSelectedMemArr){
                tempGroup.grade = 2;
                 tempGroup.groupNameString=[LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_SEARCH_ThisCheck];
            }
           
            tempGroup.memberArray = tempArray;
            [seachGroupArr addObject:tempGroup];
      
    }
    self.searchMemberArray = seachGroupArr;
    [self.tableView reloadData];
}
-(void)setMemberInGroupArr:(NSArray *)memberInGroupArr{
    _memberInGroupArr = memberInGroupArr;
    if (_memberInGroupArr.count>0) {
        [self managerMemberData];
    }
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    alertCount = 0;

    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(searchUserNotifyAction:) name:kSearchUserWithNameString object:nil];
    [self.view addSubview:self.headView];
    [self.view addSubview: self.footView];
    self.footView.backgroundColor =[UIColor colorWithPatternImage:[UIImage imageNamed:@"chuzheng_frame02"]];
    [self.view addSubview: self.nameEditView ];
  
    [self.view addSubview:self.tableView];
    [self.view bringSubviewToFront:self.headView];
    
    TopHeadView *topView= [TopHeadView topHeadView];
    topView.frame = CGRectMake(0, 0, self.headView.width, self.headView.height/2);
    topView.topHeadViewDelegate = self;
    topView.titleNameLabel.text = @"创建多人会话";
    _topView = topView;
    [self.headView addSubview:topView];
    
    //顶部ButtonView
    TopButtonView *topButtonView =[TopButtonView topButtonViewWithLeftButtonName:[LanguageKeys lkShared].BTN_ALLIANCE andWithRightButtonName:[LanguageKeys lkShared].BTN_SEARCH];
    topButtonView.frame = CGRectMake(0, self.headView.height/2, self.headView.width, self.headView.height/2);
    [topButtonView settingLeftButtonSelected:YES];
    psVcTopButtonSelected = PSVCTopButtonSelected_left;
    topButtonView.topButtonViewDelegate = self;
    [self.headView addSubview:topButtonView];
    
    
    [self.nameEditView addSubview:self.headEditView];
    [self.nameEditView addSubview:self.searchEditView];
    [self settingSearchViewShow:NO];
    
    
    UIButton *qiutButton=[UIButton buttonWithType:UIButtonTypeCustom];
    NSString *quitNameString = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_QUIT_CHATROOM];
    NSString *enterNameString =[LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM];
    [qiutButton setTitle:quitNameString forState:UIControlStateNormal];
    [qiutButton setTitleColor:[UIColor colorWithRed:190/255.0 green:151/255.0 blue:90/255.0 alpha:1] forState:UIControlStateNormal];
    [qiutButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_red"] forState:UIControlStateNormal];
    qiutButton.frame = CGRectMake( (self.footView.width-120*2-40)/2, (self.footView.height-40)/2, 120, 40);
    qiutButton.tag = 1001;
    [qiutButton addTarget:self action:@selector(enterButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.footView addSubview:qiutButton];
    
    UIButton *enterButton=[UIButton buttonWithType:UIButtonTypeCustom];
    [enterButton setTitle:enterNameString forState:UIControlStateNormal];
    [enterButton setTitleColor:[UIColor colorWithRed:190/255.0 green:151/255.0 blue:90/255.0 alpha:1] forState:UIControlStateNormal];
    [enterButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_green"] forState:UIControlStateNormal];
    enterButton.tag = 1002;
    enterButton.frame = CGRectMake( qiutButton.right + 40, (self.footView.height-40)/2, 120, 40);
    [enterButton addTarget:self action:@selector(enterButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.footView addSubview:enterButton];
    
   
    
    if (self.selectedMemArr.count> 0) {
        [self  settingNameEditViewHidden:NO];
        [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count+1 ]];
        
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
            nameEditView.frame = CGRectMake(0, self.headView.bottom,  kMainScreenWidth, 120) ;
        }else{
            nameEditView.frame = CGRectMake(0, self.headView.bottom,  kMainScreenWidth, 80) ;
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
-(SearchEditView *)searchEditView{
    if  (_searchEditView == nil){
        SearchEditView *searchEditView =[SearchEditView searchEditView];
        searchEditView.frame = CGRectMake(0,0,self.nameEditView.width,  self.nameEditView.height) ;
        searchEditView.searchEditTextField.delegate = self;
        searchEditView.seachEditViewDelegate = self;
        _searchEditView = searchEditView;
    }
    return _searchEditView;
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
-(NSMutableArray *)searchSelectedMemArr{
    if  (_searchSelectedMemArr == nil){
        NSMutableArray *array =[NSMutableArray array];
        _searchSelectedMemArr = array;
    }
    return _searchSelectedMemArr;
}

#pragma mark -
#pragma mark 人数计算
//计算已经加入的人
-(void)calculationJoinedMem{

    //数目大于0 就行，因为还有创建者自己

    for (int  x = 0 ;x<self.memberArr.count ;x++){
        for (NSUserInfo *tempUserInfor  in [(UserGroup *)[self.memberArr objectAtIndex:x] memberArray ]) {
            if(tempUserInfor.chooseState == ChooseState_pressed){
                [self.selectedMemArr addObject:tempUserInfor];
            }
        }
    }
    
   
}
/** 点击关闭键盘  */
-(void)btnClick{
    [self.headEditView.editView resignFirstResponder];
    [self.searchEditView.searchEditTextField resignFirstResponder];

}

#pragma mark -
#pragma mark bottomButton Action
- (void)enterButtonAction:(UIButton *)sender {
    [_headEditView.editView resignFirstResponder];
    
    if (sender.tag == 1001) {
        //退出联盟
        ZYTAlertView *alertView =[ZYTAlertView alertViewWithTitleString:nil];
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
                
                ZYTAlertView *alertView =[ZYTAlertView alertViewWithTitleString:nil];
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
            
            for (NSUserInfo *tempUserInfo in self.selectedMemArr) {
                if (self.roomMemberName.length >0) {
                    self.roomMemberName =   [self.roomMemberName stringByAppendingString:@"、"];
                }
                
                self.roomMemberName =   [self.roomMemberName stringByAppendingString:tempUserInfo.userName];
                
                
                if (self.roomMemberUid.length >0) {
                    self.roomMemberUid = [self.roomMemberUid stringByAppendingString:@"|"];
                }
                
                self.roomMemberUid = [self.roomMemberUid stringByAppendingString:tempUserInfo.uid];
                
            }
            DVLog(@"%@",self.roomMemberName);
            
            ZYTAlertView *alertView =[ZYTAlertView alertViewWithTitleString:nil];
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
                if (addUidArr == nil){
                    addUidArr = [NSMutableArray array];
                }
                addNameString = @"";
                for (NSUserInfo  *tempUserInfor in self.addMemberArr) {
                    [addUidArr addObject:tempUserInfor.uid];
                    
                    if (addNameString.length >0 ) {
                        addNameString =[addNameString stringByAppendingString:@"|"];
                    }
                    addNameString =[addNameString stringByAppendingString:tempUserInfor.userName];
                }
                ZYTAlertView *alertView =[ZYTAlertView alertViewWithTitleString:nil];
                alertView.titleType = ZYTAlertViewTitleType_add;
                alertView.nameString = addNameString;
                alertView.alertViewDelegate  = self;
                [self.view addSubview:alertView];
                [self.view bringSubviewToFront:alertView];
                alertCount = alertCount +1;
            }
            
            
            if (self.subMemberArr.count>0) {
                if (subUidArr == nil) {
                    subUidArr =[NSMutableArray array ];
                }
                subNameString = @"";
                for (NSUserInfo  *tempUserInfor in self.subMemberArr) {
                    [subUidArr addObject:tempUserInfor.uid];
                    if (subNameString.length >0 ) {
                        subNameString =[subNameString stringByAppendingString:@"|"];
                    }
                    subNameString =[subNameString stringByAppendingString:tempUserInfor.userName];
                }
                ZYTAlertView *alertView =[ZYTAlertView alertViewWithTitleString:nil];
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
            roomName = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_CHATROOM];
        }
        [[ChatServiceController chatServiceControllerSharedManager].gameHost selectChatRoomMember:roomName :self.roomMemberName :self.roomMemberUid ];
        [self clickButton];
    }

}

-(void)commitAddRoomMember{
     if (self.personSelectVCType == PersonSelectVCType_ChangeMember ){
        if (self.addMemberArr.count>0){
            [[ChatServiceController chatServiceControllerSharedManager].gameHost addGroupChatMemberWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid andWithMemberUidArray:addUidArr andWithMemberName:addNameString];
        }
        [self clickButton];
    }

}
-(void)commitSubRoomMember{
    if (self.personSelectVCType == PersonSelectVCType_ChangeMember ){
        if (self.subMemberArr.count>0) {
            [[ChatServiceController chatServiceControllerSharedManager].gameHost subGroupChatMemberWithGroupID:[UserManager sharedUserManager].currentMail.opponentUid andWithMemberUidArray:subUidArr andWithMemberName:subNameString ];
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
    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
        return self.memberArr.count;
    }else{
        return self.searchMemberArray.count;
    }
    
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    

    
    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
        UserGroup *tempGroup = [self.memberArr objectAtIndex:section];
        if (tempGroup.isOpen) {
            return tempGroup.memberArray.count;
        }else{
            return  0;
        }
    }else{
        UserGroup *tempGroup = [self.searchMemberArray objectAtIndex:section];
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
         cell.userInfo = [[[self.memberArr objectAtIndex:indexPath.section] memberArray] objectAtIndex:indexPath.row];
    }else{
         cell.userInfo = [[[self.searchMemberArray objectAtIndex:indexPath.section] memberArray] objectAtIndex:indexPath.row];
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
    
    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
        SectionHeadView *headerView =[SectionHeadView sectionHeadViewWithTableView:tableView];
        headerView.sectionHeadViewDelegate = self;
        headerView.userGroup = [self.memberArr objectAtIndex:section];
        headerView.backgroundColor = [UIColor clearColor];
        return headerView;
    }else{
        SectionHeadView *headerView =[SectionHeadView sectionHeadViewWithTableView:tableView];
        headerView.sectionHeadViewDelegate = self;
        headerView.userGroup = [self.searchMemberArray objectAtIndex:section];
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
        

        if (self.selectedMemArr.count>0) {
            [self settingNameEditViewHidden:NO];
        }else{
            [self settingNameEditViewHidden:YES];
        }
        
        [self settingSearchViewShow:NO];
      
    }else if(sender.tag == 102){
        //右侧按钮
        DVLog(@"右侧按钮");
        psVcTopButtonSelected = PSVCTopButtonSelected_right;
        //计算坐标
        [self settingNameEditViewHidden:NO];
        //控制子视图显示
        [self settingSearchViewShow:YES];
  
      
    }
    [self.tableView reloadData];
}

#pragma mark -
#pragma mark SearchEditView Delegate
-(void)searchEditViewBtnPressedAction:(UIButton *)sender{
    [self.searchEditView.searchEditTextField resignFirstResponder];
    DVLog(@"搜索确定按钮点击 %@",sender);
    NSString * searchString =self.searchEditView.searchEditTextField.text;
    if (searchString.length >0) {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingUsersWithSearchString:searchString];
    }
    
//    for (UserGroup *tempgroup in self.searchMemberArray) {
//        //临时选择的人
//        if  (tempgroup.grade == 2){
//            if  (tempgroup.memberArray.count>0){
//                self.searchSelectedMemArr = tempgroup.memberArray;
//            }
//        }
//    }
}
#pragma mark -
#pragma mark ZYTAlertViewDelegate
- (void)clickEnterButtonWithZYTAlertView:(ZYTAlertView *)vAlertView{
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
-(void)cancalBtnAction:(ZYTAlertView *)vAlertView{
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
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden= YES;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
}

#pragma mark -
#pragma mark notify Action
-(void)searchUserNotifyAction:(NSNotification *)vNotify{
    DVLog(@"%@",vNotify.userInfo);
    NSArray *userArray =[vNotify.userInfo objectForKey:@"users"];
    [self managerSearchMemberData:userArray];
}
#pragma mark -
#pragma mark HeadViewDelegate
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


-(void)groupMemberAddWithUserInfo:(NSUserInfo*)vUserInfo{
      if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
          [self.selectedMemArr addObject:vUserInfo];
          [self.addMemberArr addObject:vUserInfo];
          if (self.selectedMemArr.count >0) {
              if ([self isRoomCreater]){
                   [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count +1]];
              }else{
                  [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count ]];
              }
             
              
              [self.tableView reloadData];
              
              [self settingNameEditViewHidden:NO];
          }
          DVLog(@"%@",self.selectedMemArr);
      }else{
         
          for (UserGroup *tempgroup in self.searchMemberArray) {
              if  (tempgroup.grade == 2){
                  [tempgroup.memberArray addObject:vUserInfo];
                  if ([self isRoomCreater]){
                      [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count +1+tempgroup.memberArray.count]];
                  }else{
                      [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count +tempgroup.memberArray.count]];
                  }
                  
              }
          }
          [self.tableView reloadData];
      }

}
-(void)groupMemberRemoveWithUserInfo:(NSUserInfo *)vUserInfo{

    if (psVcTopButtonSelected == PSVCTopButtonSelected_left) {
        [self.selectedMemArr removeObject:vUserInfo];
        [self.addMemberArr removeObject:vUserInfo];
        [self.subMemberArr addObject:vUserInfo];
        if (self.selectedMemArr.count == 0) {
            //        self.tableView.tableHeaderView = headDefaultView;
            
            //            self.nameEditView.height = 0;
            
            [self settingNameEditViewHidden:YES];
            [self.tableView reloadData];
            [self settingVCTitleNameWithString:@"创建多人会话"];
            
        }else{
            
            [self settingNameEditViewHidden:NO];
            [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count +1]];
            
        }
        DVLog(@"%@",self.selectedMemArr);

    }else{
        
        for (UserGroup *tempgroup in self.searchMemberArray) {
            if  (tempgroup.grade == 2){
                [tempgroup.memberArray removeObject:vUserInfo];
                if(tempgroup.memberArray.count + self.selectedMemArr.count  == 0){
                  
                 
                    [self settingVCTitleNameWithString:@"创建多人会话"];
                }else{
                   
                    [self settingVCTitleNameWithString:[NSString stringWithFormat:@"Chat Member (%d)",self.selectedMemArr.count +1+tempgroup.memberArray.count]];
                }
            }
        }
        [self.tableView reloadData];
    }
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
-(void)settingSearchViewShow:(BOOL)vShow{
    self.searchEditView.hidden = !vShow;
    self.headEditView.hidden = vShow;
}

@end
