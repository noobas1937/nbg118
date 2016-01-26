//
//  SysMailListCell.m
//  IF
//
//  Created by 张彦涛 on 15/9/9.
//
//

#import "SysMailListCell.h"
#import "NSString+Cocos2dHelper.h"
#import "MailData.h"
#import "UIColor+MakeColor.h"
#import "ServiceInterface.h"

@interface SysMailListCell ()

{
    YYLabel * label;
}

@property (weak, nonatomic) IBOutlet UIView *rootMainView;
@property (nonatomic,weak)IBOutlet UIButton * selectedButton;

@property (weak, nonatomic) IBOutlet UIButton *zhegaiButton;


@property (weak, nonatomic) IBOutlet UIView *showMainView;

@property (nonatomic,weak)IBOutlet UIImageView *headBackImageView;
@property (nonatomic,weak)IBOutlet UIImageView * headImageView;
@property (nonatomic,weak)IBOutlet UILabel * titleLabel;
//@property (nonatomic,weak)IBOutlet YYLabel * contentsLabel;
@property (nonatomic,strong)YYLabel * contentsLabel;
@property (nonatomic,weak)IBOutlet UILabel  *timeLabel;
@property (nonatomic,weak)IBOutlet UIImageView * giftImageView;
@property (nonatomic,weak)IBOutlet UIImageView  * unReadStatusImageView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *showMianViewLeadingConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *titleLabelLeadIngConstraint;

@property (weak, nonatomic) IBOutlet NSLayoutConstraint *titleLabelWidth_lessThan_Constraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *contentsLabelWidth_lessThan_Constraint;

@end

@implementation SysMailListCell

+ (instancetype)sysMailListCellWithTableView:(UITableView *)tableView
{
    static NSString *identify = @"SysMailListCell";
    SysMailListCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
        
        cell = [[[NSBundle mainBundle ]loadNibNamed:@"SysMailListCell" owner:nil options:nil]lastObject];
        cell.backgroundColor =[UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleDefault;
        UIImageView *backImageView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"mail_list_selected_bg"]];
        backImageView.frame = CGRectMake(0, 0, cell.width, cell.height);
        cell.selectedBackgroundView = backImageView;
        [cell initView];
    }
 
    return cell;
}

-(void)awakeFromNib{
 
    _headBackImageView.backgroundColor =[UIColor clearColor];
    _headImageView.layer.cornerRadius = _headImageView.height /2.0;
    _headImageView.backgroundColor = [UIColor clearColor];
    _headImageView.clipsToBounds = YES;
    _titleLabel.textColor =[UIColor colorWith256Red:0 green:0 blue:0 alpha:1];
    _contentsLabel.textColor =[UIColor colorWith256Red:87 green:79 blue:51 alpha:1];
    
   _titleLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_nameLabelSize];
    _contentsLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    self.timeLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_timeLabelSize];
    [_timeLabel sizeToFit];
}

+(CGFloat)rowHeightWithSysMailListCell{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return 90;
    }else{
        return 60;
    }
}

-(void)setSelectedButtonShowing:(BOOL )selectedButtonShowing{
    _selectedButtonShowing = selectedButtonShowing;
    if (YES == _selectedButtonShowing) {
         _selectedButton.hidden = NO;
        self.showMianViewLeadingConstraint.constant = [SysMailListCell rowHeightWithSysMailListCell]* 0.6;
       
    }else{
        self.showMianViewLeadingConstraint.constant = 0;
        _selectedButton.hidden = YES;
    }
     _zhegaiButton.hidden = _selectedButton.hidden;
    [self updateConstraintsIfNeeded];
    _titleLabelWidth_lessThan_Constraint.constant = 0- self.height-10-_timeLabel.width-20-20;
    _contentsLabelWidth_lessThan_Constraint.constant = 0- self.height-10 -_giftImageView.width-10-10 ;
    [self updateConstraintsIfNeeded];
}

-(void)setMailInforDataModel:(MailInfoDataModel *)mailInforDataModel{
    _mailInforDataModel = mailInforDataModel;
    DVLog(@"%@",[mailInforDataModel printAllPropertys]);
  
    //status = 0 是未读
    if(_mailInforDataModel.status > 0){
        //1是已读
        [self settingUnReadStatusImageViewShow:NO];
    }else{
        [self settingUnReadStatusImageViewShow:YES];

    }
    if (mailInforDataModel.type == Mail_System) {
        _headImageView.image = [UIImage imageNamed:@"mail_list_icon_system_other"];

    }else{
        _headImageView.image = [UIImage imageNamed:_mailInforDataModel.maildata.mailIcon];
        
    }
    _selectedButton.selected = mailInforDataModel.isSelected;
    _timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:_mailInforDataModel.creatTime];
    //[_timeLabel sizeToFit];
    _titleLabel.text =  _mailInforDataModel.maildata.nameText;
    //    _contentsLabel.text = _mailInforDataModel.maildata.contentText;
   // NSString *string =@"[ui_food]+100 [ui_wood]+200 [ui_steel]+300 [ui_silver]+400jasdlasdjlasjdlajsdljasdasasdasDasdasDasdasDasdaSDasdaSDasdasDasdaSDasdaSDsad";
        NSString * string = _mailInforDataModel.maildata.contentText ;
    self.titleLabel.backgroundColor=[UIColor clearColor];
    NSMutableAttributedString *text = [NSMutableAttributedString new];
    [text appendAttributedString:[[NSMutableAttributedString alloc] initWithString:string attributes:nil]];
    self.contentsLabel.attributedText = [self returnText:text];
    CGSize size=[self needH:self.contentsLabel.attributedText];
    

   
    self.contentsLabel.size = CGSizeMake(kMainScreenWidth-[SysMailListCell rowHeightWithSysMailListCell]-10-self.timeLabel.origin.x, MAXFLOAT);
     if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
         self.contentsLabel.frame=CGRectMake([SysMailListCell rowHeightWithSysMailListCell]+10, [SysMailListCell rowHeightWithSysMailListCell]/2.6, size.width, size.height);
     }else{
         self.contentsLabel.frame=CGRectMake([SysMailListCell rowHeightWithSysMailListCell]+5, self.titleLabel.bottom+5, size.width, size.height);
     }
    self.contentsLabel.textColor=[UIColor colorWith256Red:87 green:79 blue:51 alpha:1];
 
        if (_mailInforDataModel.rewardStatus == 0 ) {
        _giftImageView.hidden = NO;
    }else {
        //已领取
        _giftImageView.hidden = YES;
        if (_mailInforDataModel.saveFlag == 1) {
            _giftImageView.image = [UIImage imageNamed:@"mail_list_edit_favorite"];
            _giftImageView.hidden = NO;
        }else{
             _giftImageView.image = [UIImage imageNamed:@"mail_list_edit_gift"];
            _giftImageView.hidden = YES;
        }
       
    }
     [self settingHeadImageViewBackGrounColourWithChannelID:_mailInforDataModel.channelID];
      [self updateConstraintsIfNeeded];
}

-(void)settingUnReadStatusImageViewShow:(BOOL)vShowing{
    if (vShowing) {
        _unReadStatusImageView.hidden = NO;
        _titleLabelLeadIngConstraint.constant = 10+_unReadStatusImageView.width+4;
    }else{
        _unReadStatusImageView.hidden = YES;
        _titleLabelLeadIngConstraint.constant = 10;
    }
   
}
/** 系统邮件头像背景色设置 */
-(void)settingHeadImageViewBackGrounColourWithChannelID:(NSString *)vChannelID{
    if ([vChannelID isEqualToString:Mail_ChannelID_Alliance]) {
        _headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_Invite"];
//           _headImageView.backgroundColor = [UIColor colorWith256Red:51 green:99 blue:56 alpha:1];
    }else if([vChannelID isEqualToString:Mail_ChannelID_Fight]){
            _headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_battle"];
//        _headImageView.backgroundColor = [UIColor colorWith256Red:124 green:30 blue:34 alpha:1];
    }else if([vChannelID isEqualToString:Mail_ChannelID_Studio]){
            _headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_Studio"];
//        _headImageView.backgroundColor = [UIColor colorWith256Red:56 green:57 blue:61 alpha:1];
    }else if([vChannelID isEqualToString:Mail_ChannelID_System]){
            _headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_Studio"];
//        _headImageView.backgroundColor = [UIColor colorWith256Red:118 green:85 blue:16 alpha:1];
    }
    else if([vChannelID isEqualToString:Mail_ChannelID_Event]){
        _headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_Event"];
        //        _headImageView.backgroundColor = [UIColor colorWith256Red:118 green:85 blue:16 alpha:1];
    }else{
            _headBackImageView.image =[UIImage imageNamed:@""];
//        _headImageView.backgroundColor =[UIColor clearColor];
    }
}
- (IBAction)selectButtonAction:(UIButton *)sender {
    sender.selected = !sender.selected ;
    self.mailInforDataModel.isSelected = sender.selected;
    if ([self.sysMailListCellDelegate respondsToSelector:@selector(sysMailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.sysMailListCellDelegate sysMailListCellSelectedButtonActionWithCell:self andWithButton:sender];
    }
}
- (IBAction)zhegaiButtonAction:(id)sender {
    _selectedButton.selected = !_selectedButton.selected;
    self.mailInforDataModel.isSelected = _selectedButton.selected;
    if ([self.sysMailListCellDelegate respondsToSelector:@selector(sysMailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.sysMailListCellDelegate sysMailListCellSelectedButtonActionWithCell:self andWithButton:_selectedButton];
    }

}

-(void)initView
{
//    self.rootMainView.backgroundColor=[UIColor colorWithRed:(float)222/255.f green:(float)150/255.f blue:(float)62/255.f  alpha:1];
   UIWindow *window=[[[UIApplication sharedApplication] delegate] window];
    self.readButton=[UIButton buttonWithType:UIButtonTypeCustom];
    self.readButton.frame=CGRectMake(window.frame.size.width, 0, 70, self.showMainView.frame.size.height);
    self.readButton.backgroundColor=[UIColor grayColor];
    [self.readButton setTintColor:[UIColor blackColor]];
    self.readButton.titleLabel.font =[UIFont systemFontOfSize:12.0];
    self.readButton.titleLabel.lineBreakMode= NSLineBreakByWordWrapping;
    NSString *readString =[NSString stringWithMultilingualWithKey:@"132120"];//132120=标记已读
    [self.readButton setTitle:readString forState:UIControlStateNormal];
    [self.readButton addTarget:self action:@selector(click:) forControlEvents:UIControlEventTouchUpInside];
    self.readButton.tag=1;
    [self.contentView insertSubview:self.readButton atIndex:0];
    
    self.deleteButton=[UIButton buttonWithType:UIButtonTypeCustom];
    self.deleteButton.frame=CGRectMake(window.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
    self.deleteButton.backgroundColor=[UIColor redColor];
    [self.deleteButton setTintColor:[UIColor blackColor]];
    self.deleteButton.titleLabel.font =[UIFont systemFontOfSize:12.0];
    self.deleteButton.titleLabel.lineBreakMode= NSLineBreakByWordWrapping;
      NSString *deleteString =[NSString stringWithMultilingualWithKey:@"108523"];//108523=删除
    [self.deleteButton setTitle:deleteString forState:UIControlStateNormal];
    [self.deleteButton addTarget:self action:@selector(click:) forControlEvents:UIControlEventTouchUpInside];
    self.deleteButton.tag=2;
    [self.contentView insertSubview:self.deleteButton atIndex:0];
    
    
    UISwipeGestureRecognizer *swipeLeft=[[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeCell:)];
    swipeLeft.direction=UISwipeGestureRecognizerDirectionLeft;
    [self addGestureRecognizer:swipeLeft];
    
    UISwipeGestureRecognizer *swipeRight=[[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeCell:)];
    swipeRight.direction=UISwipeGestureRecognizerDirectionRight;
    [self addGestureRecognizer:swipeRight];
    
    
    self.contentsLabel=[YYLabel new];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            self.contentsLabel.font=[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize-4];
    }else{
         self.contentsLabel.font=[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    }
   
    self.contentsLabel.backgroundColor =[UIColor clearColor];
    self.contentsLabel.numberOfLines = 2;
    self.contentsLabel.textVerticalAlignment = YYTextVerticalAlignmentTop;
    [self.showMainView insertSubview:self.contentsLabel atIndex:1];
//    [self.showMainView addSubview:self.contentsLabel];
}
-(void)openCell
{
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(isSelectedButtonShow)])
    {
        if ([self.delegate isSelectedButtonShow]) {
            return;
        };
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(SysMailopen:)]) {
        
        if ([self.delegate SysMailopen:self]) {
            [UIView animateWithDuration:0.15 animations:^{
                self.rootMainView.frame=CGRectMake(-145, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width-145, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width-75, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.15 animations:^{
                    self.rootMainView.frame=CGRectMake(-135, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                    self.readButton.frame=CGRectMake(self.frame.size.width-135, 0, 70, self.showMainView.frame.size.height);
                    self.deleteButton.frame=CGRectMake(self.frame.size.width-65, 0, 70, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.15 animations:^{
                        self.rootMainView.frame=CGRectMake(-140, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                        self.readButton.frame=CGRectMake(self.frame.size.width-140, 0, 70, self.showMainView.frame.size.height);
                        self.deleteButton.frame=CGRectMake(self.frame.size.width-70, 0, 70, self.showMainView.frame.size.height);
                    } completion:^(BOOL finished) {
                        
                    }];
                }];
            }];
        };
    }
    
}
-(void)closeCell
{
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(isSelectedButtonShow)])
    {
        if ([self.delegate isSelectedButtonShow]) {
            return;
        };
    }
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(SysMailclose:)]) {
        
        if ([self.delegate SysMailclose:self]) {
            [UIView animateWithDuration:0.2 animations:^{
                self.rootMainView.frame=CGRectMake(0, 0, self.rootMainView.frame.size.width,  self.rootMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                
            }];
        }else{
            [UIView animateWithDuration:0.1 animations:^{
                self.rootMainView.frame=CGRectMake(10, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width+10, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width+80, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.2 animations:^{
                    self.rootMainView.frame=CGRectMake(-10, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                    self.readButton.frame=CGRectMake(self.frame.size.width-10, 0, 70, self.showMainView.frame.size.height);
                    self.deleteButton.frame=CGRectMake(self.frame.size.width+60, 0, 70, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.1 animations:^{
                        self.rootMainView.frame=CGRectMake(0, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
                        self.readButton.frame=CGRectMake(self.frame.size.width, 0, 70, self.showMainView.frame.size.height);
                        self.deleteButton.frame=CGRectMake(self.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
                    } completion:^(BOOL finished) {
                        
                    }];
                }];
            }];
        }
    }
    
}
-(void)closeUpCell:(SysMailListCell*)cell
{
    [UIView animateWithDuration:0.2 animations:^{
        cell.rootMainView.frame=CGRectMake(0, 0, self.rootMainView.frame.size.width, self.rootMainView.frame.size.height);
        self.readButton.frame=CGRectMake(self.frame.size.width, 0, 70, self.showMainView.frame.size.height);
        self.deleteButton.frame=CGRectMake(self.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
    } completion:^(BOOL finished) {
        
    }];
}
-(void)swipeCell:(UISwipeGestureRecognizer *)sender
{
    if (sender.direction==UISwipeGestureRecognizerDirectionLeft) {
        
        [self openCell];
        
    }else if (sender.direction==UISwipeGestureRecognizerDirectionRight){
        
        [self closeCell];
    }
}
-(void)click:(UIButton*)button
{
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(cellbuttonClick:andWithMailCategroyListCell:)]) {
        
        [self.delegate cellbuttonClick:button andWithMailCategroyListCell:self]
        ;
    }
}
-(CGSize)needH:(NSMutableAttributedString*)text
{
    
    
    if (label==nil) {
        label=[YYLabel new];
    }
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        text.font=[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize-4];
    }else{
        text.font=[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    }
    
    label.userInteractionEnabled = YES;
    label.numberOfLines = 2;
    
    label.size = CGSizeMake(kMainScreenWidth -[SysMailListCell rowHeightWithSysMailListCell]-10-self.timeLabel.origin.x, MAXFLOAT);
      
    
    label.frame=CGRectMake(0, 0, kMainScreenWidth *0.6,222);
    label.textVerticalAlignment = YYTextVerticalAlignmentTop;
    label.attributedText = text;
    [label sizeToFit];
    return label.size;
    
}


-(NSMutableAttributedString*)returnText:(NSMutableAttributedString*)text
{
    
    NSRegularExpression *regex=
    [NSRegularExpression regularExpressionWithPattern:@"\\[[^ \\[\\]]+?\\]" options:kNilOptions error:NULL];
    NSArray *emoticonResults = [regex matchesInString:text.string options:0 range:text.rangeOfAll];
    NSUInteger emoClipLength = 0;
    for (NSTextCheckingResult *emo in emoticonResults) {
        if (emo.range.location == NSNotFound && emo.range.length <= 1) continue;
        NSRange range = emo.range;
        range.location -= emoClipLength;
        if ([text attribute:YYTextHighlightAttributeName atIndex:range.location]) continue;
        if ([text attribute:YYTextAttachmentAttributeName atIndex:range.location]) continue;
        NSString *emoString = [text.string substringWithRange:range];
        NSAttributedString *emoText;
        UIImage *image;
        CGRect  rect;
          if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
               rect=CGRectMake(-2,-3, 30, 25);
          }else{
              rect=CGRectMake(-2,-3, 20, 15);
          }
       // ui_wood.png ui_stone.png ui_iron.png
        if ([emoString isEqualToString:@"[ui_stone.png]"]) {
            image = [UIImage imageNamed:@"ui_stone"];
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:13 needOffset:0 andImageFrame:rect];
            
        }else if([emoString isEqualToString:@"[ui_wood.png]"]){
            image = [UIImage imageNamed:@"ui_wood"];
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:13 needOffset:0 andImageFrame:rect];
            
        }else if([emoString isEqualToString:@"[ui_iron.png]"]){
            image = [UIImage imageNamed:@"ui_iron"];
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:13 needOffset:0 andImageFrame:rect];
            
        }else if([emoString isEqualToString:@"[ui_food.png]"]){
            image = [UIImage imageNamed:@"ui_food"];
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:13 needOffset:0 andImageFrame:rect];
            
        }
        
        if (image == nil)continue;
        
        [text replaceCharactersInRange:range withAttributedString:emoText];
        emoClipLength += range.length - 1;
    }
    NSMutableParagraphStyle * paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setLineSpacing:3];
    [text addAttribute:NSParagraphStyleAttributeName value:paragraphStyle range:NSMakeRange(0, [text length])];
    
    
    return text;
    
}

 

@end
