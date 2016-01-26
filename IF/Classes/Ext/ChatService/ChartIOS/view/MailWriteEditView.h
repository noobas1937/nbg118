//
//  MailWriteEditView.h
//  IF
//
//  Created by 张彦涛 on 15/11/16.
//
//

#import <UIKit/UIKit.h>
@class MailWriteEditView;
@protocol MailWriteEditViewDelegate <NSObject>
@optional
-(void)backViewTouchDownWithMailWriteView:(MailWriteEditView *)vMailWriteView;
-(void)mailWriteViewAddButtonActionWith:(MailWriteEditView *)vMailWriteView andWithAddButton:(UIButton *)sender;
-(void)mailWriteViewTextViewChangedWithMailWriteView:(MailWriteEditView *)vmailWriteView;
@end

@interface MailWriteEditView : UIView
+(instancetype)mailWriteEditViewWithFrame:(CGRect )vRect;

@property (weak, nonatomic) IBOutlet UITextView *contentsTextView;
@property (weak, nonatomic) IBOutlet UITextField *nameTextField;

@property(nonatomic,assign)id<MailWriteEditViewDelegate>mailWriteViewDelegate;
@end
