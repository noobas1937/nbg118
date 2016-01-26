//
//  CSAlertView.m
//  IF
//
//  Created by 张彦涛 on 15/6/18.
//
//

#import "CSAlertView.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
@interface CSAlertView ()
@property (nonatomic, weak) IBOutlet UIButton *yesBtn;
@property (nonatomic, weak) IBOutlet UIButton *noBtn;
@property (nonatomic, weak) IBOutlet UIImageView *backgroundImage;
@property (nonatomic, weak) IBOutlet UILabel *yesStr;
@property (nonatomic, weak) IBOutlet UIImageView *goldImage;
@property (nonatomic, weak) IBOutlet UILabel *goldCount;
@property (weak, nonatomic) IBOutlet UIButton *yesBtn2;

@property (nonatomic, strong) NSString *yes;
@property (nonatomic, strong) NSString *no;

@end

@implementation CSAlertView
+(instancetype)alertViewWithTitleString:(NSString *)vTitleStr{
    CSAlertView *tipPop=[[[NSBundle mainBundle]loadNibNamed:@"CSAlertView" owner:nil options:nil]lastObject];
    
//    tipPop.nameString = vTitleStr;
    return tipPop;
}
-(void)awakeFromNib{
     self.frame = [ UIScreen mainScreen ].bounds;
    self.backgroundColor =[[UIColor blackColor]colorWithAlphaComponent:0.3];
    // Do any additional setup after loading the view from its nib.
    //本地化
    self.yes = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM];
    self.no = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CANCEL];
   
    //9宫格背景图
    UIImage *image = self.backgroundImage.image;
    
    self.backgroundImage.image = [image resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(image.size.height/2), floorf(image.size.width/2), floorf(image.size.height/2), floorf(image.size.width/2))];
    CGFloat chatFontSize = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    UIFont *uf = [UIFont systemFontOfSize:chatFontSize];
    self.yesBtn.titleLabel.font = uf;
    self.noBtn.titleLabel.font = uf;
    self.yesBtn2.titleLabel.font = uf;
    
    self.yesStr.font = uf;
    self.goldCount.font = uf;
    
    self.tipText.font = uf;
    
    
    [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
    [self.noBtn setTitle:self.no forState:UIControlStateNormal];
    self.yesStr.hidden = YES;
    self.goldImage.hidden = YES;
    self.goldCount.hidden = YES;
    self.yesBtn2.hidden = YES;
    
}

-(void) showViewByType
{
    if (self.viewType == BANSENDMSG || self.viewType == NOGETREWARD || self.viewType == CHANGENAME) {
        [self.yesBtn2 setTitle:self.yes forState:UIControlStateNormal];
        self.yesBtn2.hidden = NO;
        
        [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
        [self.noBtn setTitle:self.no forState:UIControlStateNormal];
        self.yesStr.hidden = YES;
        self.goldImage.hidden = YES;
        self.goldCount.hidden = YES;
        self.yesBtn.hidden = YES;
        self.noBtn.hidden = YES;
    }else{
        [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
        [self.noBtn setTitle:self.no forState:UIControlStateNormal];
        self.yesStr.hidden = YES;
        self.goldImage.hidden = YES;
        self.goldCount.hidden = YES;
        self.yesBtn2.hidden = YES;
    }
}

-(void)setNameString:(NSString *)nameString{
    _nameString = nameString;
    NSString *tipStr = nil;
    if (self.titleType == ZYTAlertViewTitleType_add) {
        tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_INVITE :self.nameString];
        self.tipText.text = tipStr;

    }else if(self.titleType == ZYTAlertViewTitleType_sub){
        tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_KICK :self.nameString];
        self.tipText.text = tipStr;

    }else if(self.titleType == ZYTAlertViewTitleType_reName){
        tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_MODIFYNAME :self.nameString];
        self.tipText.text = tipStr;
    }else if(self.titleType == ZYTAlertViewTitleType_creat){
        tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_INVITE :self.nameString];
        self.tipText.text = tipStr;
    }else if (self.titleType == ZYTAlertViewTitleType_quit){
        tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_QUIT :self.nameString];
        self.tipText.text = tipStr;
    }else if (self.titleType == ZYTAlertViewTitleType_shield){
        self.tipText.text = nameString;
    }
    
}
- (IBAction)cancalButtonAction:(id)sender {
    if ( [self.alertViewDelegate respondsToSelector:@selector(cancalBtnAction:)]) {
        [self.alertViewDelegate cancalBtnAction:self];
    }
    [self removeFromSuperview];
}
- (IBAction)enterButtonAction:(id)sender {
    if ( [self.alertViewDelegate respondsToSelector:@selector(clickEnterButtonWithZYTAlertView:)]) {
        [self.alertViewDelegate clickEnterButtonWithZYTAlertView:self];
    }
    
    [self removeFromSuperview];
}

- (IBAction)enterButtonAction2:(id)sender {
    if (self.viewType == CHANGENAME) {
        if ([ServiceInterface serviceInterfaceSharedManager].cs_CountryChat_OC_Native_New) {
            if ([self.alertViewDelegate respondsToSelector:@selector(popUpChangeName)]) {
                [self.alertViewDelegate popUpChangeName];
            }
        }else{
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost changeNickName];
        }
        
    }else{
        if ( [self.alertViewDelegate respondsToSelector:@selector(clickEnterButtonWithZYTAlertView:)]) {
            [self.alertViewDelegate clickEnterButtonWithZYTAlertView:self];
        }
    }
    [self removeFromSuperview];
}


@end
