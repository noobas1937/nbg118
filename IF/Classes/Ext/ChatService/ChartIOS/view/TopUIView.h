//
//  TopUIView.h
//  气泡
//
//  Created by mzc on 15/3/31.
//  Copyright (c) 2015年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol TopUIViewDelegate <NSObject>
@optional
- (void)TopUIViewRightButtonAction ;
-(void)topUIViewCancalButtonAction;
- (void)clickButtonWithTag:(NSInteger)vTag ;
-(void) openFriendsView;
@end

@interface TopUIView : UIView

@property (nonatomic,strong) UIButton *allianceButton;
@property (nonatomic,strong) UIButton *countriesButton;
@property (nonatomic,strong) UIView *buttonView;
@property (nonatomic,strong) UIView *topView;
@property (nonatomic,strong) UIImageView *huitiaoImage;
@property (nonatomic,strong) UIImageView *topBackGround;
@property (nonatomic,strong) UILabel *titlePlayerName;

@property (nonatomic,assign)id <TopUIViewDelegate>topUIViewDelegate;

@property (nonatomic,assign) int chatType;

-(void) addUINavigationBar;
-(void) addUISegmentedControl;

-(void)setUIButton:(UIButton*) button view:(UIView*)buttonView type:(int) buttonType;
-(void)allianceButtonPressed:(id)sender;
-(void)countriesButtonPressed:(id)sender;
-(void)backButtonPressed:(id)sender;
-(id) init:(int)chatType;
/**选择联盟*/
-(void) selectASN;
/**选择国家*/
-(void) selectCoun;

-(void)settingAddMemberButtonShow:(BOOL)vShowBool;

@end
