//
//  TipPopUpController.m
//  IF
//
//  Created by mzc on 15/4/17.
//
//

#import "TipPopUpController.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatCellIOS.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "UserManager.h"
#import "MsgMessage.h"


@interface TipPopUpController ()
@property (nonatomic, strong) IBOutlet UIButton *yesBtn;
@property (nonatomic, strong) IBOutlet UIButton *noBtn;
@property (nonatomic, strong) IBOutlet UIImageView *backgroundImage;
@property (nonatomic, strong) IBOutlet UILabel *yesStr;
@property (nonatomic, strong) IBOutlet UIImageView *goldImage;
@property (nonatomic, strong) IBOutlet UILabel *goldCount;
@property (retain, nonatomic) IBOutlet UIButton *yesBtn2;



@property (nonatomic, strong) NSString *yes;
@property (nonatomic, strong) NSString *no;
@end

@implementation TipPopUpController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor =[[UIColor blackColor]colorWithAlphaComponent:0.3];
    // Do any additional setup after loading the view from its nib.
    
    self.view.frame = [ UIScreen mainScreen ].bounds;
    
    CGFloat chatFontSize = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    UIFont *uf = [UIFont systemFontOfSize:chatFontSize];
    
    //本地化
    self.yes = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM];
    self.no = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CANCEL];
    
    //9宫格背景图
    UIImage *image = self.backgroundImage.image;
    
    self.backgroundImage.image = [image resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(image.size.height/2), floorf(image.size.width/2), floorf(image.size.height/2), floorf(image.size.width/2))];
    
    self.yesBtn.titleLabel.font = uf;
    self.noBtn.titleLabel.font = uf;
    self.yesBtn2.titleLabel.font = uf;
    
    self.yesStr.font = uf;
    self.goldCount.font = uf;
    
    self.tipText.font = uf;
    
    
    /**设置tip 说明*/
    NSString *tipStr = nil;
    switch(self.tipType)
    {
        case BLOCKTYPE:
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_SHIELD_PLAYER :self.cell.cellFrame.chatMessage.name];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        case RESENDTYPE:
            tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_RESEND];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        case NOHORN:
            {
                //号角    TIP_HORN
                NSString *horn = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_HORN];
                tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_ITEM_NOT_ENOUGH :horn];
                self.tipText.text = tipStr;
                //隐藏确定按钮文字
                [self.yesBtn setTitle:@"" forState:UIControlStateNormal];
                [self.noBtn setTitle:self.no forState:UIControlStateNormal];
                //显示label确定字样
                self.yesStr.text = self.yes;
                self.yesStr.textAlignment = NSTextAlignmentCenter;
                //显示金币图片
                self.goldImage.image = [UIImage imageNamed:@"ui_gold_coin"];
                //显示发送一个广播的金币数额
                int gc = [[ChatServiceController chatServiceControllerSharedManager] radioCount];
                self.goldCount.text = [NSString stringWithFormat:@"%d",gc];
                self.yesBtn2.hidden = YES;
            }
            break;
        case NOTICEPRICE:
            {
                //金币不足 暂时没走这里。如果需要二级弹窗。走这里
                tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_CORN_NOT_ENOUGH];
                self.tipText.text = tipStr;
                [self.yesBtn2 setTitle:self.yes forState:UIControlStateNormal];
                self.yesBtn.hidden = YES;
                self.yesStr.hidden = YES;
                self.goldImage.hidden = YES;
                self.goldCount.hidden = YES;
                self.noBtn.hidden = YES;
            }
            break;
        case RADIOTYPE:
        {
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            NSString *horn = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_HORN];
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_USEITEM :horn];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
        }
            break;
        case INVITATION:
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_INVITE :@"拉萨凉凉的"];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        default:
            ;
    }
    
    
}

-(void) removeSelf
{
    [self removeFromParentViewController];
    [self.view removeFromSuperview];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark
 - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
- (IBAction)onClickYesBtn:(UIButton *)sender {
    switch(self.tipType)
    {
        case BLOCKTYPE:
            {
                [[ChatServiceController chatServiceControllerSharedManager].gameHost block:self.cell.cellFrame.chatMessage.uid :self.cell.cellFrame.chatMessage.name];
                //[[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :@"屏蔽成功" :0.0 :0.0 :true];
                [self removeSelf];
            }
            break;
        case RESENDTYPE:
            {
                
                if(self.cell.cellFrame.chatMessage.post == 6 && self.cell.cellFrame.chatMessage.channelType == 0){
                    
                    [[ChatServiceController chatServiceControllerSharedManager].gameHost sendRadio:self.cell.cellFrame.chatMessage];
                    [self removeSelf];
                }else{
                    [self.cell exitResetSend];
                }
                
                [self removeSelf];
            }
            break;
        case NOHORN:
            {
                //判断金币是否足够
                if([[ChatServiceController chatServiceControllerSharedManager] isSatisfySendRadio]){
                    NSLog(@"金币足够");
                    //NSDate *datenow = [NSDate date];
                    //NSString *timeSp = [NSString stringWithFormat:@"%d", (long)[datenow timeIntervalSince1970]];
                    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:self.chatMessage];
                    [[MsgMessage msgMessageShared] addChatMsgList:self.chatMessage];
                    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
                    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:self.chatMessage.msg :200011 :true :[NSString stringWithFormat:@"%d",self.chatMessage.sendLocalTime]];
                    [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney = FALSE;
                    [self removeSelf];
                }else{
                    NSLog(@"金币不足");
                    NSString *tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_CORN_NOT_ENOUGH];
                    self.tipText.text = tipStr;
                    [self.yesBtn2 setTitle:self.yes forState:UIControlStateNormal];
                    self.yesBtn.hidden = YES;
                    self.yesStr.hidden = YES;
                    self.goldImage.hidden = YES;
                    self.goldCount.hidden = YES;
                    self.noBtn.hidden = YES;
                    self.yesBtn2.hidden = NO;
                    self.tipType = NOTICEPRICE;
                }
            }
            break;
        case NOTICEPRICE:
            {
                [self removeSelf];
            }
            break;
        case RADIOTYPE:
        {
//            NSDate *datenow = [NSDate date];
//            NSString *timeSp = [NSString stringWithFormat:@"%d", (long)[datenow timeIntervalSince1970]];
            ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:self.chatMessage];
            [[MsgMessage msgMessageShared] addChatMsgList:self.chatMessage];
            [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
            [[ChatServiceController chatServiceControllerSharedManager] sendNotice:self.chatMessage.msg :200011 :false :[NSString stringWithFormat:@"%d",self.chatMessage.sendLocalTime]];
            [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount = FALSE;
            [self removeSelf];
        }
            break;

        case INVITATION:
        {
//            [self.personSelectVC commitCreateRoom];
        }
        break;
        default:
        ;
    }
    
    
}
- (IBAction)onClickNoBtn:(UIButton *)sender {
    [self removeSelf];
}

@end
