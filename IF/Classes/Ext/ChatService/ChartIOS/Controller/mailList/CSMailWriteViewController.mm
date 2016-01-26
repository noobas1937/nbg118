//
//  CSMailWriteViewController.m
//  IF
//
//  Created by 张彦涛 on 15/11/16.
//
//

#import "CSMailWriteViewController.h"
#import "TopHeadView.h"
#import "NSString+Cocos2dHelper.h"
#import "UIImage+resized.h"
#import "MailWriteEditView.h"
#import "PersonSelectVC.h"
#include "cocos2d.h"
#include "MailController.h"
#import "ChatServiceCocos2dx.h"
#import "ServiceInterface.h"
#import "UserManager.h"
@interface CSMailWriteViewController ()<TopHeadViewDelegate,MailWriteEditViewDelegate,PersonSelectVCDelegate >{
    NSString *uidString ;
    NSString *roomName;
}
@property (strong, nonatomic)  UIView *headView;
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong)  UIView * footView;
@property (nonatomic,strong)MailWriteEditView *mailWriteEditView;
@property (nonatomic,strong)UIButton *sendButton;
@end

@implementation CSMailWriteViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    UIImageView *backimageView =[[UIImageView alloc]initWithFrame:self.view.frame];
    backimageView.image =[UIImage resizedImage:@"cs_mail_write_backImage"];
    backimageView.userInteractionEnabled = YES;
    UITapGestureRecognizer *tapGesturRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapAction)];
    [backimageView addGestureRecognizer:tapGesturRecognizer];
    [self.view insertSubview:backimageView atIndex:0];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
    [self.view addSubview:self.mailWriteEditView];
    [self _initFootView];
//    self.view.backgroundColor =[UIColor colorWithPatternImage:[UIImage resizedImage:@"cs_mail_write_backImage"]];
    [self.headView addSubview: self.topHeaderView];
 
}
-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:kMailChatCreatByOCNative object:nil];
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
            footView.frame = CGRectMake(0, kMainScreenHeight* 0.93, kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            footView.frame = CGRectMake(0, kMainScreenHeight* 0.93, kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _footView  = footView;
    }
    return  _footView;
}

-(TopHeadView*)topHeaderView{
    if (_topHeaderView ==nil) {
        TopHeadView *view = [TopHeadView  topHeadViewWithRightButtonShowFlag:NO andShowRightButtonType:TopHeadViewRightButtonType_ViewEdit];
        view.titleNameLabel.text = [NSString stringWithMultilingualWithKey:@"115030"];//邮件 115030
        view.frame = self.headView.frame;
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}
-(MailWriteEditView *)mailWriteEditView{
    if (_mailWriteEditView == nil) {
        _mailWriteEditView =[MailWriteEditView mailWriteEditViewWithFrame:CGRectMake(0, self.headView.bottom, kMainScreenWidth, 250)];
        _mailWriteEditView.mailWriteViewDelegate = self;
    }
    return _mailWriteEditView;
}
#pragma mark -
#pragma mark UI set
-(void)_initFootView{
    UIImageView *backImageView =[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height)];
    backImageView.image = [UIImage resizedImage:@"toolbar_bottom_bar"];
    UIImageView *topLineImageView=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height * 0.1)];
    topLineImageView.image =[UIImage resizedImage:@"line_grey02"];
    [self.footView addSubview:backImageView];
    [self.footView addSubview:topLineImageView];
    UIButton *sendMailButton =[UIButton buttonWithType:UIButtonTypeCustom];
    sendMailButton.frame = CGRectMake((self.footView.width *0.5 -(self.footView.width* 0.25/2)), self.footView.height * 0.1, self.footView.width* 0.3, self.footView.height * 0.85);
    [sendMailButton setImage:[UIImage imageNamed:@"cs_mail_write_buttonImage"] forState:UIControlStateNormal];
    [sendMailButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_green"] forState:UIControlStateNormal];
    sendMailButton.imageEdgeInsets = UIEdgeInsetsMake(4, 20, 4, 20);
    [self.footView addSubview:sendMailButton];
    sendMailButton.enabled = NO;
    [sendMailButton addTarget:self action:@selector(sendButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    self.sendButton = sendMailButton;
}
#pragma mark -
#pragma mark action

-(void)tapAction{
    [self.mailWriteEditView.nameTextField resignFirstResponder];
    [self.mailWriteEditView.contentsTextView resignFirstResponder];
}
-(void)sendButtonAction:(UIButton *)sender{
    sender.enabled = NO;
    NSArray *tempArray =[uidString componentsSeparatedByString:@"|"];
    if (tempArray.count == 0) {
        
        //用户输入名字的
            NSString *nameString = self.mailWriteEditView.nameTextField.text;
            NSString *contentsString = self.mailWriteEditView.contentsTextView.text;
           NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",(long)[[NSDate date] timeIntervalSince1970]];
            NSString * allianceUidStr  =@"";
            NSString *mailUid = @"";
            BOOL isflag = YES;
            int type = 2;//单人邮件的
            
            //(std::string toName,  std::string title,  std::string contents,  std::string sendLocalTime,  std::string allianceId,  std::string mailUid,  bool isflag, int type,  std::string targetUid,  std::string thxMail,  bool showTip)
            MailController::getInstance()->sendMailToServer([nameString UTF8String], "", [contentsString UTF8String],[sendLocalTime UTF8String],[allianceUidStr UTF8String],[mailUid UTF8String], isflag,type,[mailUid UTF8String]);
       
        [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(notifyAction:) name:kMailChatCreatByOCNative object:nil];
    }
    else if(tempArray.count == 1){
            //选人界面回来的
        
    }
    else  {
        //聊天室
        CCArray *ccArr =CCArray::create();
        for (NSString *str in tempArray) {
            
            ccArr->addObject(CCString::create([str UTF8String]));
        }
        NSString *nameString = self.mailWriteEditView.nameTextField.text;
        NSString *contentsString = self.mailWriteEditView.contentsTextView.text;
        MailController::getInstance()->createChatRoom([nameString UTF8String], ccArr , [roomName UTF8String], [contentsString UTF8String]);

    }
   
}
#pragma mark -
#pragma mark topHeadViewDelegate
- (void)clickButton {
    [self.navigationController popViewControllerAnimated:YES];
}
#pragma mark -
#pragma mark MailWriteEditViewDelegate
-(void)backViewTouchDownWithMailWriteView:(MailWriteEditView *)vMailWriteView{
    [self tapAction];
}
-(void)mailWriteViewAddButtonActionWith:(MailWriteEditView *)vMailWriteView andWithAddButton:(UIButton *)sender{
   PersonSelectVC *personVC = [[PersonSelectVC alloc]initWithType:PersonSelectVCType_New];
//    personVC.ispushFrom_OC_Native = YES;
    personVC.personVCOpenFrom = PersonVCOpenFrom_OC_MailListAdd;
    personVC.personVCDelegate = self;
    [self.navigationController pushViewController:personVC animated:YES];
}
-(void)mailWriteViewTextViewChangedWithMailWriteView:(MailWriteEditView *)vmailWriteView{
    if  (vmailWriteView.contentsTextView.text.length > 0 &&vmailWriteView.nameTextField.text.length > 0 ){
        self.sendButton.enabled = YES;
    }else{
        self.sendButton.enabled = NO;
    }
   
}
#pragma mark -
#pragma mark  PersonSelectVCDelegate
-(void)personSelectVCCreatChatRoomWithRoomMemberName:(NSString *)vNameString andWithUidString:(NSString *)vUidString andWithRoomName:(NSString *)vRoomNameString{
    self.mailWriteEditView.nameTextField.text = vNameString;
    uidString = vUidString;
    roomName = vRoomNameString;
 
}

#pragma mark -
#pragma mark notifyAction 
-(void)notifyAction:(NSNotification *)vNotify{
    NSDictionary *userInfo =vNotify.userInfo;
    int creatStatus= [[userInfo objectForKey:@"creatStatus"] intValue];
    NSString *nameString = [userInfo objectForKey:@"name"];
    NSString *uidStr = [userInfo objectForKey:@"uid"];
    if (self.mailWriteEditView.nameTextField.text.length > 0 && self.mailWriteEditView.contentsTextView.text.length> 0){
        self.sendButton.enabled = YES;
    }
    if (creatStatus == 1){
        if ([nameString isEqualToString:self.mailWriteEditView.nameTextField.text]){
            ChatServiceCocos2dx::m_channelType = CHANNEL_TYPE_USER;
            //
            [[ServiceInterface serviceInterfaceSharedManager] setMailInfo:uidStr :[UserManager sharedUserManager].currentUser.uid :nameString :2];
            ChatServiceCocos2dx::showChatIOSFrom2dx();
            
        }

    }
    
}
@end
