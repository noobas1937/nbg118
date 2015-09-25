//
//  HeadEditView.m
//  IF
//
//  Created by 张彦涛 on 15/6/17.
//
//

#import "HeadEditView.h"
#import "UIImage+resized.h"
#import "ServiceInterface.h"
#import "LanguageKeys.h"
#import "LanguageManager.h"
@interface HeadEditView () 

@property (weak, nonatomic) IBOutlet UIImageView *groupBackImageView;
@property (weak, nonatomic) IBOutlet UILabel *instructionLabel;


@end

@implementation HeadEditView

+(instancetype)headEditView{
    return [[[NSBundle mainBundle]loadNibNamed:@"HeadEditView" owner:nil options:nil] lastObject];
}

-(void)awakeFromNib{
    [_groupBackImageView setImage:[UIImage  resizableImage:@"groupBack"]];
    _groupBackImageView.backgroundColor =[UIColor clearColor];
    _editView.background = [UIImage resizableImage:@"chat_room_input_back"];
    UIView *leftView =[[UIView alloc]initWithFrame:CGRectMake(0, 0, 12, _editView.frame.size.height)];
   
    _editView.leftView = leftView;
    _editView.leftViewMode = UITextFieldViewModeAlways;
    _editView.placeholder = @"";
//    DVLog(@"headerEditView Frame :%@",NSStringFromCGRect(self.frame));
    
   NSString *titleString = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_MODIFY_CHATROOM_NAME];
    
    _instructionLabel.text = titleString;
   _instructionLabel.textColor = [UIColor colorWithRed:212/255.0 green:187/255.0 blue:163/255.0 alpha:0.9];
    _instructionLabel.font =[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].sectionFontSize];
    [_instructionLabel sizeToFit];
    _instructionLabel.numberOfLines = 1;
  
//    CGSize textSize =[_instructionLabel.text sizeWithAttributes:@{NSFontAttributeName:_instructionLabel.font}];
//}

}


-(void)setTitleString:(NSString *)titleString{
    _titleString =titleString;
    _instructionLabel.text = titleString;
}
@end
