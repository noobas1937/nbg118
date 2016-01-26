//
//  TopButtonView.m
//  IF
//
//  Created by 张彦涛 on 15/6/26.
//
//

#import "TopButtonView.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
@interface TopButtonView  (){
    NSString *leftButtonName;
    NSString *rightButtonName;
}


@property (weak, nonatomic) IBOutlet UIButton *countryButton;

@property (weak, nonatomic) IBOutlet UIButton *allianceButton;


@end

@implementation TopButtonView
+(instancetype)topButtonViewWithLeftButtonName:(NSString *)vLeftTitleStr andWithRightButtonName:(NSString *)vRightTitleStr{
    return  [[TopButtonView alloc]initWithLeftButtonName:vLeftTitleStr andWithRightButtonName:vRightTitleStr];
}

-(instancetype)initWithLeftButtonName:(NSString *)vLeftTitleStr andWithRightButtonName:(NSString *)vRightTitleStr {

    self = [[[NSBundle mainBundle] loadNibNamed:@"TopButtonView" owner:nil options:nil] lastObject];
    if (self) {
        if (vLeftTitleStr.length>0) {
             [self setLeftButtonName:vLeftTitleStr];
        }
        if (vRightTitleStr.length >0) {
             [self setRightButtonName:vRightTitleStr];
        }

    }
    return self;
}

-(void)setLeftButtonName:(NSString *)vLeftButtonName{
//    NSString *countriesStr = [NSString stringWithMultilingualWithKey:vLeftButtonName];
    [_countryButton setTitle:vLeftButtonName forState:UIControlStateNormal];
    [_countryButton setTitle:vLeftButtonName forState:UIControlStateSelected];
}
-(void)setRightButtonName:(NSString *)vRightButtonName{
//    NSString *allianceStr = [NSString stringWithMultilingualWithKey:vRightButtonName];
    [_allianceButton setTitle:vRightButtonName forState:UIControlStateNormal];
    [_allianceButton setTitle:vRightButtonName forState:UIControlStateSelected];
}
-(void)awakeFromNib{
    

    //字体
    UIFont  *buttonFont  = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].fontSize];

    _countryButton.titleLabel.font = buttonFont;
    _allianceButton.titleLabel.font = buttonFont;

    
}

-(void)settingLeftButtonSelected:(BOOL)vBool{
    _countryButton.selected = vBool;
    _allianceButton.selected = !vBool;
}
-(void)settingRightButtonSelected:(BOOL)vBool{
    _allianceButton.selected = vBool;
     _countryButton.selected = !vBool;
}

- (IBAction)countryButtonAction:(UIButton *)sender {
    if (sender.selected == YES) {
        return;
    }
    [self settingLeftButtonSelected:YES];
    if ([self.topButtonViewDelegate respondsToSelector:@selector(topButtonViewBtnPressedAction:)]) {
        [self.topButtonViewDelegate topButtonViewBtnPressedAction:sender ];
    }
}

- (IBAction)allianceButtonAction:(UIButton *)sender {
    if (sender.selected == YES) {
        return;
    }
    [self settingRightButtonSelected:YES];
    if ([self.topButtonViewDelegate respondsToSelector:@selector(topButtonViewBtnPressedAction:)]) {
        [self.topButtonViewDelegate topButtonViewBtnPressedAction:sender ];
    }

}



@end
