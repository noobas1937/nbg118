//
//  CSMailBottomOperationToolBarView.h
//  IF
//
//  Created by 张彦涛 on 15/11/5.
//
//

#import <UIKit/UIKit.h>
@class CSMailBottomOperationToolBarView;
@protocol MailBottomToolBarDelegate <NSObject>
@optional

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView selectAllButtonActionWithButton:(UIButton *)sender;

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView giftButtonActionWithButton:(UIButton *)sender;

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView starButtonActionWithButton:(UIButton *)sender;

-(void)mailBottomToolBarView:(CSMailBottomOperationToolBarView *)vToolBarView delectButtonActionWithButton:(UIButton *)sender;
@end


@interface CSMailBottomOperationToolBarView : UIView

@property (nonatomic,strong) id<MailBottomToolBarDelegate> mailBottomToolBarDelegate;
+(instancetype)mailBottomOperationToolBarViewWithFrame:(CGRect)vFrame;
-(void)settingStartStatus;
-(void)settingDeleteButtonShow:(BOOL)vdelButtonShow
              andgitButtonShow:(BOOL)vGiftButtonShow
             andStarButtonShow:(BOOL)vStarButtonShow;
@end
