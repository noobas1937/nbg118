//
//  SearchEditView.m
//  IF
//
//  Created by 张彦涛 on 15/7/6.
//
//

#import "SearchEditView.h"
#import "UIImage+resized.h"
#import "ServiceInterface.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
@interface SearchEditView ()
@property (weak, nonatomic) IBOutlet UIImageView *groupBackImageView;
@property (weak, nonatomic) IBOutlet UILabel *instructionLabel;
@property (weak, nonatomic) IBOutlet UIButton *enterButton;

@end

@implementation SearchEditView

+(instancetype)searchEditView{
    return [[[NSBundle mainBundle]loadNibNamed:@"SearchEditView" owner:nil options:nil] lastObject];
}

-(void)awakeFromNib{
    [_groupBackImageView setImage:[UIImage  resizableImage:@"groupBack"]];
    _groupBackImageView.backgroundColor =[UIColor clearColor];
    [_enterButton setTitle:[LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM] forState:UIControlStateNormal];
    
    _searchEditTextField.background = [UIImage resizableImage:@"chat_room_input_back"];
    UIView *leftView =[[UIView alloc]initWithFrame:CGRectMake(0, 0, 12, _searchEditTextField.frame.size.height)];
    
    _searchEditTextField.leftView = leftView;
    _searchEditTextField.leftViewMode = UITextFieldViewModeAlways;
    _searchEditTextField.placeholder = @"";
 
    
    NSString *titleString = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_SEARCH];
    
    _instructionLabel.text = titleString;
    _instructionLabel.textColor = [UIColor colorWithRed:212/255.0 green:187/255.0 blue:163/255.0 alpha:0.9];
    _instructionLabel.font =[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].sectionFontSize];
    [_instructionLabel sizeToFit];
    _instructionLabel.numberOfLines = 1;
        
}
- (IBAction)enterButtonAction:(UIButton *)sender {
    if ([self.seachEditViewDelegate respondsToSelector:@selector(searchEditViewBtnPressedAction:)]) {
        [self.seachEditViewDelegate searchEditViewBtnPressedAction:sender];
    }
}


@end
