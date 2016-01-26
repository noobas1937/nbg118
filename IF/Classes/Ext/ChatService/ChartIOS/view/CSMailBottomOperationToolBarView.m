//
//  CSMailBottomOperationToolBarView.m
//  IF
//
//  Created by 张彦涛 on 15/11/5.
//
//

#import "CSMailBottomOperationToolBarView.h"
#import "NSString+Cocos2dHelper.h"
#import "UIImage+resized.h"
@interface CSMailBottomOperationToolBarView()
@property (weak,nonatomic)IBOutlet UIImageView *backImageView;

@property (weak, nonatomic) IBOutlet UIButton *checkButton;

@property (weak, nonatomic) IBOutlet UIButton *giftButton;
@property (weak, nonatomic) IBOutlet UIButton *delectButton;
@property (weak, nonatomic) IBOutlet UIButton *starButton;


@property (retain, nonatomic) IBOutlet UILabel *checkAllLabel;

@end

@implementation CSMailBottomOperationToolBarView

+(instancetype)mailBottomOperationToolBarViewWithFrame:(CGRect)vFrame{
    CSMailBottomOperationToolBarView *view = [[[NSBundle mainBundle]loadNibNamed:@"CSMailBottomOperationToolBarView" owner:nil options:nil]lastObject];
    view.frame = vFrame;
    return view;
}

//101298=取消全选
//102145=全选
-(void)awakeFromNib{
    _backImageView.image =[UIImage resizableImage:@"mail_list_edit_buttonbar_bg"];
}
-(void)settingDeleteButtonShow:(BOOL)vdelButtonShow
              andgitButtonShow:(BOOL)vGiftButtonShow
             andStarButtonShow:(BOOL)vStarButtonShow{
    _delectButton.hidden = !vdelButtonShow;
    _giftButton.hidden = !vGiftButtonShow;
    _starButton.hidden = !vStarButtonShow;
}
- (IBAction)selectAllButtonAction:(UIButton *)sender {
    sender.selected = !sender.selected;
    if ([_mailBottomToolBarDelegate respondsToSelector:@selector(mailBottomToolBarView:selectAllButtonActionWithButton:)]) {
        [_mailBottomToolBarDelegate mailBottomToolBarView:self selectAllButtonActionWithButton:sender];
    }
}
- (IBAction)giftButtonAction:(id)sender {
    if ([_mailBottomToolBarDelegate respondsToSelector:@selector(mailBottomToolBarView:giftButtonActionWithButton:)]) {
        [_mailBottomToolBarDelegate mailBottomToolBarView:self giftButtonActionWithButton:sender];
    }
}
- (IBAction)starButtonAction:(id)sender {
    if ([_mailBottomToolBarDelegate respondsToSelector:@selector(mailBottomToolBarView:starButtonActionWithButton:)]) {
        [_mailBottomToolBarDelegate mailBottomToolBarView:self starButtonActionWithButton:sender];
    }
}

- (IBAction)delectButtonAction:(id)sender {
    if ([_mailBottomToolBarDelegate respondsToSelector:@selector(mailBottomToolBarView:delectButtonActionWithButton:)]) {
        [_mailBottomToolBarDelegate mailBottomToolBarView:self delectButtonActionWithButton:sender];
    }
    
}

-(void)settingStartStatus{
    _checkButton.selected = NO;
}

@end
