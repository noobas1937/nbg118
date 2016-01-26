//
//  MailWriteEditView.m
//  IF
//
//  Created by 张彦涛 on 15/11/16.
//
//

#import "MailWriteEditView.h"
#import "UIImage+resized.h"
@interface MailWriteEditView ()<UITextViewDelegate>


@property (weak, nonatomic) IBOutlet UIImageView *textViewBackImageView;
@property (weak, nonatomic) IBOutlet UIButton *nameButton;

@property (weak, nonatomic) IBOutlet UIButton *writeContentsButton;

@property (weak, nonatomic) IBOutlet UIButton *addMemberButton;

@end
@implementation MailWriteEditView

+(instancetype)mailWriteEditViewWithFrame:(CGRect )vRect{
   UIView *mailWriteEditView = [[[NSBundle mainBundle ]loadNibNamed:@"MailWriteEditView" owner:nil options:nil] lastObject];
    mailWriteEditView.frame = vRect;
    return mailWriteEditView;
}
-(void)awakeFromNib{
    _nameTextField.background =[UIImage resizedImage:@"cs_mail_write_textView_back"];
    _textViewBackImageView.image =[UIImage resizedImage:@"cs_mail_write_textView_back"];
    _contentsTextView.delegate = self;
}

- (IBAction)touchDown:(id)sender {
    if ([self.mailWriteViewDelegate respondsToSelector:@selector(backViewTouchDownWithMailWriteView:)]) {
        [self.mailWriteViewDelegate backViewTouchDownWithMailWriteView:self];
    }
}

- (IBAction)addButtonAction:(id)sender {
    
    if ([self.mailWriteViewDelegate respondsToSelector:@selector(mailWriteViewAddButtonActionWith:andWithAddButton:)]) {
        [self.mailWriteViewDelegate mailWriteViewAddButtonActionWith:self andWithAddButton:sender];
    }
}

- (void)textViewDidChange:(UITextView *)textView{
    if ([self.mailWriteViewDelegate respondsToSelector:@selector(mailWriteViewTextViewChangedWithMailWriteView:)]) {
        [self.mailWriteViewDelegate mailWriteViewTextViewChangedWithMailWriteView:self];
    }
    
}
@end
