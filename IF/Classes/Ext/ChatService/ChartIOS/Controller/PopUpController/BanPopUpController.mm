//
//  BanPopUpController.m
//  IF
//
//  Created by mzc on 15/4/17.
//
//

#import "BanPopUpController.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatServiceController.h"
#import "ServiceInterface.h"
#import "CSMessageModel.h"
#import "CSMessage.h"
#import "ChatServiceCocos2dx.h"
#import "NSString+Cocos2dHelper.h"
#import "UserManager.h"
@interface BanPopUpController (){

    int banTime1 ;
    int banTime2 ;
    int banTime3 ;
    int banTime4 ;
}

@property (nonatomic, strong) IBOutlet UIButton *yesBtn;
@property (nonatomic, strong) IBOutlet UIButton *noBtn;
@property (nonatomic, strong) IBOutlet UILabel *titleStr;
@property (nonatomic, strong) IBOutlet NSMsgItem *chatMessage;
@property (nonatomic, strong) IBOutlet UIButton *oneTimeBtn;
@property (nonatomic, strong) IBOutlet UIButton *fourTimeBtn;
@property (nonatomic, strong) IBOutlet UIButton *eightTimeBtn;
@property (nonatomic, strong) IBOutlet UIButton *twentyFourTimeBtn;

@property (nonatomic, strong) IBOutlet UILabel *oneTimeLabel;
@property (nonatomic, strong) IBOutlet UILabel *fourTimeLabel;
@property (nonatomic, strong) IBOutlet UILabel *eightTimeLabel;
@property (nonatomic, strong) IBOutlet UILabel *twentyFourTimeLabel;
@property (retain, nonatomic) IBOutlet UIImageView *backgroundImage;

@property(nonatomic,strong)CSMessageModel *messageModel;



@property (nonatomic,assign) int banTime;

@end

@implementation BanPopUpController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString *banTimeString = [ChatServiceController chatServiceControllerSharedManager].banTimeString;
    if (banTimeString) {
        NSArray *banTimeArr = [banTimeString componentsSeparatedByString:@"|"];
        if (banTimeArr.count == 4) {
            banTime1 = [banTimeArr[0] intValue];
            banTime2 = [banTimeArr[1] intValue];
            banTime3 = [banTimeArr[2] intValue];
            banTime4 = [banTimeArr[3] intValue];
        }
        else{
            banTime1 = 1;
            banTime2 = 4;
            banTime3 = 8;
            banTime4 = 24;
        }
    }else{
        banTime1 = 1;
        banTime2 = 4;
        banTime3 = 8;
        banTime4 = 24;
    }
    
    // Do any additional setup after loading the view from its nib.
    
    self.view.frame = [ UIScreen mainScreen ].bounds;
    
    CGFloat chatFontSize = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    UIFont *uf = [UIFont systemFontOfSize:chatFontSize];
    
    NSString *countriesStr =nil;
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        NSString *nameString = @"";
        NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:self.messageModel.message.uid];
        if (user){
            nameString = user.userName;
        }else{
            nameString =self.messageModel.message.uid;
        }
        if (nameString.length == 0){
            nameString =self.messageModel.message.uid ;
        }
         countriesStr =  [NSString stringWithMultilingualWithKey:@"105210" andWithPaddingString1:nameString];
    }else{
      countriesStr =  [NSString stringWithMultilingualWithKey:@"105210" andWithPaddingString1:self.chatMessage.name];
    }
    
    //本地化
    //105210=是否禁言：{0}？
    
//    NSString *countriesStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_BAN :self.chatMessage.name];
    NSString *yes = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM];
    NSString *no = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CANCEL];
    
    NSString *time = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_TIME];
    
    //9宫格背景图
    UIImage *image = self.backgroundImage.image;
    
    self.backgroundImage.image = [image resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(image.size.height/2), floorf(image.size.width/2), floorf(image.size.height/2), floorf(image.size.width/2))];
    
    self.titleStr.text = countriesStr;//[NSString initWithFormat:@"%@,%@", self.titleStr, self.chatMessage.name];
    [self.yesBtn setTitle:yes forState:UIControlStateNormal];
    [self.noBtn setTitle:no forState:UIControlStateNormal];
    
    self.oneTimeLabel.text = [NSString stringWithFormat:@"%d %@",banTime1,time];// [@"1 " stringByAppendingString:time];
    self.fourTimeLabel.text =[NSString stringWithFormat:@"%d %@",banTime2,time];// [@"2 " stringByAppendingString:time];
    self.eightTimeLabel.text =[NSString stringWithFormat:@"%d %@",banTime3,time];// [@"3 " stringByAppendingString:time];
    self.twentyFourTimeLabel.text =[NSString stringWithFormat:@"%d %@",banTime4,time];// [@"4 " stringByAppendingString:time];
    
    self.yesBtn.titleLabel.font = uf;
    self.noBtn.titleLabel.font = uf;
    self.titleStr.font = uf;
    self.oneTimeLabel.font = uf;
    self.fourTimeLabel.font = uf;
    self.eightTimeLabel.font = uf;
    self.twentyFourTimeLabel.font = uf;
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)oneTime:(UIButton *)sender {
    self.banTime = 0;
    
    [self.oneTimeBtn setImage:[UIImage imageNamed:@"check_box_checked"] forState:UIControlStateNormal];
    [self.fourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.eightTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.twentyFourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
}

- (IBAction)fourTimr:(UIButton *)sender {
    self.banTime = 1;
    
    [self.oneTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.fourTimeBtn setImage:[UIImage imageNamed:@"check_box_checked"] forState:UIControlStateNormal];
    [self.eightTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.twentyFourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
}

- (IBAction)eightTime:(UIButton *)sender {
    self.banTime = 2;
    
    [self.oneTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.fourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.eightTimeBtn setImage:[UIImage imageNamed:@"check_box_checked"] forState:UIControlStateNormal];
    [self.twentyFourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
}

- (IBAction)twentyFourTime:(UIButton *)sender {
    self.banTime = 3;
    
    [self.oneTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.fourTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.eightTimeBtn setImage:[UIImage imageNamed:@"check_box"] forState:UIControlStateNormal];
    [self.twentyFourTimeBtn setImage:[UIImage imageNamed:@"check_box_checked"] forState:UIControlStateNormal];
}

-(void)settingInitDataWithMessageModel:(CSMessageModel *)vMessageModel{
    self.messageModel = vMessageModel;
}

-(void) setInitData:(NSMsgItem *)chatMessage{

    self.chatMessage = chatMessage;
 

}
- (IBAction)yesBtn:(UIButton *)sender {
    
    //禁言
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        if  (self.messageModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
            //喇叭接口
            [[ChatServiceController chatServiceControllerSharedManager].gameHost banPlayerNoticeWithUserUid:self.messageModel.message.uid andWithBanTimeLever:self.banTime];
        }else{
             [[ChatServiceController chatServiceControllerSharedManager].gameHost ban:self.messageModel.message.uid :self.chatMessage.name :self.banTime];
        }
       
    }else{
        [[ChatServiceController chatServiceControllerSharedManager].gameHost ban:self.chatMessage.uid :self.chatMessage.name :self.banTime];
    }
    
    [self removeFromParentViewController];
    [self.view removeFromSuperview];
}


- (IBAction)noBtn:(UIButton *)sender {
    [self removeFromParentViewController];
    [self.view removeFromSuperview];
}

@end
