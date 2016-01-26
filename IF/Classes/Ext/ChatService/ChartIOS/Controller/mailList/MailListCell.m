//
//  MailListCell.m
//  IF
//
//  Created by 张彦涛 on 15/8/19.
//
//

#import "MailListCell.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
#import "UIImageView+EMWebCache.h"
#import "UserManager.h"
#import "UIColor+MakeColor.h"
@interface MailListCell ()
@property (weak, nonatomic) IBOutlet UIButton *selectedButton ;

@property (weak, nonatomic) IBOutlet UIButton *zhegaiButton;

@property (weak,nonatomic)  IBOutlet UIImageView *headBackImageView;
@property (weak, nonatomic) IBOutlet UIImageView *headImageView;
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;
@property (weak, nonatomic) IBOutlet UILabel *contentsLabel;
@property (weak,nonatomic)IBOutlet UIImageView *unReadStatusImageView;
@property (weak, nonatomic)IBOutlet   UILabel *timeLabel;
@property (weak, nonatomic) IBOutlet UIView *showMainView;

@property (retain, nonatomic) IBOutlet NSLayoutConstraint *showMainViewLefi_Constraint;
@property (retain, nonatomic) IBOutlet NSLayoutConstraint *titleLabelLeft_ConStraint;

@end

@implementation MailListCell


+ (instancetype)mailListCellWithTableView:(UITableView *)tableView
{
    static NSString *identify = @"MailListCell";
    MailListCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
        // 从xib中加载cell
        cell = [[[NSBundle mainBundle] loadNibNamed:@"MailListCell" owner:nil options:nil] lastObject];
        cell.backgroundColor =[UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleDefault;
        UIImageView *backImageView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"mail_list_selected_bg"]];
        backImageView.frame = CGRectMake(0, 0, cell.width, cell.height);
        cell.selectedBackgroundView = backImageView;
        [cell initView];
    }
    
    return cell;
}

- (void)awakeFromNib {
   
    self.timeLabel.textAlignment = NSTextAlignmentRight;
    
    _nameLabel.textColor =[UIColor colorWith256Red:0 green:0 blue:0 alpha:1];
    _contentsLabel.textColor =[UIColor colorWith256Red:87 green:79 blue:51 alpha:1];
    
    
//    self.headImageView.tintColor =[UIColor colorWith256Red:39 green:53 blue:81 alpha:1] ;
    ;
    //头像视图切圆
    self.headBackImageView.image =[UIImage imageNamed:@"mail_list_head_bg_chat"];
//    self.headBackImageView.backgroundColor =[UIColor orangeColor];
//    self.headBackImageView.layer.cornerRadius = self.headBackImageView.width/2;
//    self.headBackImageView.clipsToBounds = YES;
    self.headImageView.layer.cornerRadius = ([MailListCell mailListCellRowHeight]-10)/2;
    self.headImageView.clipsToBounds = YES;
    self.headImageView.backgroundColor =[UIColor clearColor];
    
 
    
    self.nameLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_nameLabelSize];
    self.contentsLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    self.timeLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_timeLabelSize];
    [_timeLabel sizeToFit];
 
    self.showSelectedButton = NO;
}


 
-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    if ( _chatChannel.unreadCount> 0) {
        [self settingUnReadStatusImageViewShow:YES];
    }else{
        [self settingUnReadStatusImageViewShow:NO];
    }
    self.selectedButton.selected = _chatChannel.selected;
//    DVLog(@"%@",[chatChannel printAllPropertys])
    if(_chatChannel.channelType == IOS_CHANNEL_TYPE_USER  ){
        
        if (chatChannel.faceCustemString.length > 0) {
            [self.headImageView sd_setImageWithURL:[NSURL URLWithString:chatChannel.faceCustemString] placeholderImage:[UIImage imageNamed:chatChannel.faceImageString]];
        }else{
            self.headImageView.image = [UIImage imageNamed:chatChannel.faceImageString];
        }
        if (_chatChannel.nameString.length > 0) {
            self.nameLabel.text = _chatChannel.nameString;
        }
        
        self.contentsLabel.text =_chatChannel.contentsString;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];
    }
    else if(_chatChannel.channelType == IOS_CHANNEL_TYPE_CHATROOM  ){
        self.headImageView.image = [UIImage imageNamed:@"mail_pic_flag_31"];
        if (_chatChannel.customName.length > 0) {
            self.nameLabel.text = _chatChannel.customName;
        }else{
            self.nameLabel.text =@"多人聊天";
        }
        
        self.contentsLabel.text =_chatChannel.contentsString;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];
    }
   
 
      [self updateConstraintsIfNeeded];
}


-(void)setShowSelectedButton:(BOOL)showSelectedButton{
    _showSelectedButton = showSelectedButton;
    if (YES ==[self isShowSelectedButton]) {
        //显示选择按钮
        _selectedButton.hidden = NO;
        self.showMainViewLefi_Constraint.constant = [MailListCell mailListCellRowHeight] *0.6;
    }else{
        self.showMainViewLefi_Constraint.constant = 0;
        _selectedButton.hidden = YES;
    }
    _zhegaiButton.hidden = _selectedButton.hidden;
      [self updateConstraintsIfNeeded];
 
}

- (IBAction)selectButtonAction:(UIButton *)sender {
    sender.selected = !sender.selected ;
    self.chatChannel.selected = sender.selected;
    if ([self.mailListCellDelegate respondsToSelector:@selector(mailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.mailListCellDelegate mailListCellSelectedButtonActionWithCell:self andWithButton:sender];
    }
}

- (IBAction)zhegaiButtonAction:(id)sender {
    _selectedButton.selected = !self.selectedButton.selected;
    self.chatChannel.selected = _selectedButton.selected;
    if ([self.mailListCellDelegate respondsToSelector:@selector(mailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.mailListCellDelegate mailListCellSelectedButtonActionWithCell:self andWithButton:_selectedButton];
    }
}

-(void)settingUnReadStatusImageViewShow:(BOOL)vShowing{
    if (vShowing) {
        _unReadStatusImageView.hidden = NO;
        _titleLabelLeft_ConStraint.constant = 10+_unReadStatusImageView.width+4;
    }else{
        _unReadStatusImageView.hidden = YES;
        _titleLabelLeft_ConStraint.constant = 10;
    }
    
}
-(void)settingSelectedButtonSelected:(BOOL)isSelected{
    
    _selectedButton.selected = isSelected;
    self.chatChannel.selected = _selectedButton.selected;
    if ([self.mailListCellDelegate respondsToSelector:@selector(mailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.mailListCellDelegate mailListCellSelectedButtonActionWithCell:self andWithButton:_selectedButton];
    }

}

+(CGFloat )mailListCellRowHeight{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return 90;
    }else{
        return 60;
    }
}

-(void)initView
{
 
    UIWindow *window=[[[UIApplication sharedApplication] delegate] window];
    self.readButton=[UIButton buttonWithType:UIButtonTypeCustom];
    self.readButton.frame=CGRectMake(window.frame.size.width , 0, 70, self.showMainView.frame.size.height);
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
    self.deleteButton.frame=CGRectMake(window.frame.size.width+140, 0, 70, self.showMainView.frame.size.height);
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
}
-(void)openCell
{
    if (self.delegate && [self.delegate respondsToSelector:@selector(isSelectedButtonShow)])
    {
        if ([self.delegate isSelectedButtonShow]) {
            return;
        };
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(Mailopen:)]) {
        
        if ([self.delegate Mailopen:self]) {
            [UIView animateWithDuration:0.15 animations:^{
                self.showMainView.frame=CGRectMake(-145, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width-145, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width-75, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.15 animations:^{
                    self.showMainView.frame=CGRectMake(-135, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                    self.readButton.frame=CGRectMake(self.frame.size.width-135, 0, 70, self.showMainView.frame.size.height);
                    self.deleteButton.frame=CGRectMake(self.frame.size.width-65, 0, 70, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.15 animations:^{
                        self.showMainView.frame=CGRectMake(-140, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
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
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(Mailclose:)]) {
        
        if ([self.delegate Mailclose:self]) {
            [UIView animateWithDuration:0.2 animations:^{
                self.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width,  self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                
            }];
        }else{
            [UIView animateWithDuration:0.1 animations:^{
                self.showMainView.frame=CGRectMake(10, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.frame.size.width+10, 0, 70, self.showMainView.frame.size.height);
                self.deleteButton.frame=CGRectMake(self.frame.size.width+80, 0, 70, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.2 animations:^{
                    self.showMainView.frame=CGRectMake(-10, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                    self.readButton.frame=CGRectMake(self.frame.size.width-10, 0, 70, self.showMainView.frame.size.height);
                    self.deleteButton.frame=CGRectMake(self.frame.size.width+60, 0, 70, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.1 animations:^{
                        self.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                        self.readButton.frame=CGRectMake(self.frame.size.width, 0, 70, self.showMainView.frame.size.height);
                        self.deleteButton.frame=CGRectMake(self.frame.size.width+70, 0, 70, self.showMainView.frame.size.height);
                    } completion:^(BOOL finished) {
                        
                    }];
                }];
            }];
        }
    }
}
-(void)closeUpCell:(MailListCell*)cell
{
    [UIView animateWithDuration:0.2 animations:^{
        cell.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
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


@end
