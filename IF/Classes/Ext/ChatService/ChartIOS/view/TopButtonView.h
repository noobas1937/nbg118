//
//  TopButtonView.h
//  IF
//
//  Created by 张彦涛 on 15/6/26.
//
//

#import <UIKit/UIKit.h>


@protocol TopButtonViewDelegate <NSObject>
@optional
-(void)topButtonViewBtnPressedAction:(UIButton *)sender;

@end


@interface TopButtonView : UIView
@property (nonatomic,assign)id<TopButtonViewDelegate>topButtonViewDelegate;

+(instancetype)topButtonViewWithLeftButtonName:(NSString *)vLeftTitleStr
                        andWithRightButtonName:(NSString *)andRightTitleStr;
-(void)settingLeftButtonSelected:(BOOL)vBool;
-(void)settingRightButtonSelected:(BOOL)vBool;
@end
