//
//  InputView.h
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import <UIKit/UIKit.h>
@class InputView;
@protocol InputViewDelegate <NSObject>
@optional
-(void)inputViewBtnPressedAction:(UIButton *)sender;

@end

@interface InputView : UIView

@property (nonatomic,assign) id<InputViewDelegate>inputViewDelegate;
@property (weak, nonatomic) IBOutlet UITextField *inputTextField;
+(instancetype)inputViewWithFrame:(CGRect)vFrame;

-(void)settingButtonTitle:(NSString *)vButtonTitleStr;
@end
