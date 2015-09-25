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
@interface MailListCell ()

@property (weak, nonatomic) IBOutlet UIImageView *headImageView;
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;

@property (weak, nonatomic) IBOutlet UILabel *contentsLabel;
@property (strong, nonatomic)   UILabel *timeLabel;
@property (weak, nonatomic) IBOutlet UIButton *selectedButton ;

@property (strong, nonatomic)   UIImageView *giftImageView;

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
  
    }else{
        for ( UIView *subView in cell.contentView.subviews) {
            [subView removeFromSuperview];
        }
    }
    return cell;
}
-(void)initCustomView{
    self.timeLabel= [[UILabel alloc]initWithFrame:CGRectZero];
    self.giftImageView = [[UIImageView alloc]initWithFrame:CGRectZero];
    [self.contentView addSubview:self.timeLabel];
    [self.contentView addSubview:self.giftImageView];
}
- (void)awakeFromNib {
    [self initCustomView];
    self.timeLabel.textAlignment = NSTextAlignmentRight;
    self.timeLabel.text = @"6:00";
    self.giftImageView.image = [UIImage imageNamed:@"mail_reward"];
    
    self.nameLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_nameLabelSize];
    self.contentsLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    self.timeLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_timeLabelSize];
    [_timeLabel sizeToFit];
    [self settingSelectedButtonHidden:[self isShowSelectedButton]];
}


-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    self.giftImageView.hidden = YES;
//    DVLog(@"%@",[chatChannel printAllPropertys])
    if (_chatChannel.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL ) {
        if (_chatChannel.faceImageString.length>0) {
            self.headImageView.image = [UIImage imageNamed:_chatChannel.faceImageString];
        }
        self.nameLabel.text = _chatChannel.nameString;
        self.contentsLabel.text = _chatChannel.contentsString;
        self.timeLabel.text = _chatChannel.timeString;
        self.giftImageView.hidden = !_chatChannel.isHasGift ;

    }else if(_chatChannel.channelType == IOS_CHANNEL_TYPE_USER  ){
        self.headImageView.image = [UIImage imageNamed:chatChannel.faceImageString];
        if (_chatChannel.nameString.length > 0) {
            self.nameLabel.text = _chatChannel.nameString;
        }
        
        self.contentsLabel.text =_chatChannel.contentsString;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];
    }
    else if(_chatChannel.channelType == IOS_CHANNEL_TYPE_CHATROOM  ){
        self.headImageView.image = [UIImage imageNamed:@"mail_pic_flag_31"];
        if (_chatChannel.nameString.length > 0) {
            self.nameLabel.text = _chatChannel.nameString;
        }else{
            self.nameLabel.text =@"多人聊天";
        }
        
        self.contentsLabel.text =_chatChannel.contentsString;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];
    }
 
    else if (_chatChannel.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        self.headImageView.image = [UIImage imageNamed:@"mail_btn_write"];
        
            self.nameLabel.text =@"国家";
         
        
        self.contentsLabel.text = [(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].showMsg;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];

    }else if (_chatChannel.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        self.headImageView.image = [UIImage imageNamed:@"mail_btn_write"];
        
        self.nameLabel.text =@"联盟";
        
        
        self.contentsLabel.text = [(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].showMsg;
        long creatTime =[(ChatCellFrame *)[_chatChannel.msgList lastObject] chatMessage].createTime;
        self.timeLabel.text = [NSString stringWithTimeFormatWithCreateTime:creatTime];
        
    }
}


-(void)setShowSelectedButton:(BOOL)showSelectedButton{
    _showSelectedButton = showSelectedButton;
    [self settingSelectedButtonHidden:![self isShowSelectedButton]];
}

- (IBAction)selectButtonAction:(UIButton *)sender {
    sender.selected = !sender.selected ;
    if ([self.mailListCellDelegate respondsToSelector:@selector(mailListCellSelectedButtonActionWithCell:andWithButton:)]) {
        [self.mailListCellDelegate mailListCellSelectedButtonActionWithCell:self andWithButton:sender];
    }
}




-(void)settingSelectedButtonHidden:(BOOL)vBool{
    self.selectedButton.hidden = vBool;
    CGFloat rowHeight = [MailListCell mailListCellRowHeight];
    CGFloat timelabelH =20;
    CGFloat timeLabelW=100;
    CGFloat timeLabelX ;
    CGFloat timeLabelY  = ((rowHeight /2)-timelabelH)/2;

    CGFloat giftImageViewH = rowHeight * 0.3;
    CGFloat giftImageViewW = rowHeight * 0.3;
    CGFloat giftImageViewX;
    CGFloat giftImageViewY = rowHeight /2 + (rowHeight /2-giftImageViewH)/2;

   
    if (vBool) {
        //button隐藏
        timeLabelX = kMainScreenWidth - timeLabelW -20 ;
        giftImageViewX = kMainScreenWidth -giftImageViewW -20;
    }else{
        timeLabelX = kMainScreenWidth - timeLabelW - (rowHeight - 20);
        giftImageViewX = kMainScreenWidth - giftImageViewW - (rowHeight-20);
    }
    
    _timeLabel.frame = CGRectMake(timeLabelX, timeLabelY, timeLabelW, timelabelH);
    _giftImageView.frame = CGRectMake(giftImageViewX, giftImageViewY, giftImageViewW, giftImageViewH);
}


+(CGFloat )mailListCellRowHeight{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        return 90;
    }else{
        return 60;
    }
}
@end
