//
//  InputView.m
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import "InputView.h"

#import "UIImage+resized.h"

#import "ServiceInterface.h"



@interface InputView ()<UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UIButton *senderButton;



@property (weak, nonatomic) IBOutlet UIImageView *backImageView;

@end

@implementation InputView

+(instancetype)inputViewWithFrame:(CGRect)vFrame{
 
    InputView *inputView = [[[NSBundle mainBundle]loadNibNamed:@"InputView" owner:nil options:nil] lastObject];
    inputView.frame = vFrame;
    return inputView;
    
}

-(void)settingButtonTitle:(NSString *)vButtonTitleStr{
    [_senderButton setTitle:vButtonTitleStr forState:UIControlStateNormal];
    
}

-(void)awakeFromNib{
    
    _backImageView.image = [UIImage resizableImage:@"chuzheng_frame02"];
    
    _inputTextField.font = [UIFont fontWithName:@"HelveticaNeue" size:[ServiceInterface serviceInterfaceSharedManager].fontSize ];
    
    _inputTextField.returnKeyType = UIReturnKeyDefault;
    
    _inputTextField.background = [UIImage resizableImage:@"text_field_bg2"];
    
    
    
    //输入框左侧加入内边距
    
    CGRect frame =_inputTextField.frame;
    
    frame.size.width = 10.0f;
    
    UIView *leftview = [[UIView alloc] initWithFrame:frame];
    
    _inputTextField.leftViewMode = UITextFieldViewModeAlways;
    
    _inputTextField.leftView = leftview;
    

}
- (IBAction)senderButtonAction:(id)sender {
    if ([self.inputViewDelegate respondsToSelector:@selector(inputViewBtnPressedAction:)]) {
        [self.inputViewDelegate inputViewBtnPressedAction:sender];
    }
}



@end