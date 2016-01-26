//
//  CSMessageTableCell.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSMessageTableCell.h"
#import "CSMessageModel.h"
#import "CSMessage.h"
#import "UIViewExt.h"
#import "UIImage+resized.h"
#import "NSString+Extension.h"
#import "Cellconfig.h"
#import "UIImageView+YYWebImage.h"
#import "ChatServiceController.h"

@implementation CSMessageTableCell

-(void)initialize
{
    // UIAppearance Proxy Defaults
//    CSMessageTableCell *cell = [self appearance];
    [super initialize];
    self.timeLabelFont = cellConfig.timeLabelFont;
    self.timeLabelHeight = cellConfig.timeLabelHeight;
    self.timeLabelTextColor = cellConfig.timeLabelTextColor;
    self.timeLabelbackColor =cellConfig.timeLabelbackColor;
    self.avatarSize = cellConfig.avatarSize;
    self.avatarCornerRadius = cellConfig.avatarCornerRadius;
    
    self.messageNameColor = cellConfig.messageNameColor;
    self.messageNameFont = cellConfig.messageNameFont;
    self.messageNameHeight = cellConfig.messageNameHeight;

    self.translatExplainFont =cellConfig.translatExplainFont;
 
}

- (instancetype)initWithStyle:(UITableViewCellStyle)style
              reuseIdentifier:(NSString *)reuseIdentifier
                        model:(CSMessageModel *)model
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier model:model];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        [self initialize];
        [self _setupSubView];
    }
    
    return self;
}

-(void)_setupSubView{
    self.cellView.timeLabel.font = _timeLabelFont;
    self.cellView.timeLabel.textColor = _timeLabelTextColor;
    self.cellView.timeLabel.backgroundColor = _timeLabelbackColor;
    self.cellView.timeLabel.height = _timeLabelHeight;
    self.cellView.nameLabel.font =_messageNameFont;
    self.cellView.nameLabel.textColor =_messageNameColor;
    self.cellView.nameLabel.height = _messageNameHeight;
    
    self.cellView.avatarbackView.layer.cornerRadius = _avatarCornerRadius;
    self.cellView.avatarbackView.layer.masksToBounds = YES;
   
//    self.cellView.avatarView.layer.cornerRadius= _avatarCornerRadius;
//    self.cellView.avatarView.layer.masksToBounds = YES;
    self.cellView.avatarView.backgroundColor =[UIColor clearColor];
    
    self.cellView.reSendButton.frame = CGRectMake(0, 0, 25, 25);
//    self.cellView.reSendButton.contentEdgeInsets =UIEdgeInsetsMake(5,5,5,5);
    self.cellView.reSendButton.hidden = YES;
    self.cellView.sendStatusActivityView.hidden = YES;
}

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(void)longRecognizerPresed{

    //长按图片
    [self setterPressedBubbleBackImage];
}
-(void)longRecognizerResumeNormal{

    //恢复默认图片
    [self setterNormalBubbleBackImage];
   
}


-(void)setterNormalBubbleBackImage{
 
        if (self.messageModel.isSelfSender) {
            if (self.messageModel.messageBodyType == CSMessageBodyType_Text_Normal ) {
                
                if ([self.messageModel.message.uid isEqualToString:[ChatServiceController chatServiceControllerSharedManager].kingUidString ]) {
                    [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_right_normal"]];
                }else{
                    [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_NorMsg_normal"]];
                }
//                 [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_NorMsg_normal"]];
            }else if( self.messageModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
                 [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_Vip_normal"]];
            }
            else if( self.messageModel.messageBodyType == CSMessageBodyType_RedPackge){
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"chat_redenvelope_right_bg_normal"]];
            }else{
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_System_normal"]];
//                 [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_right_normal"]];
            }
            
           
        }else{
            if (self.messageModel.messageBodyType == CSMessageBodyType_Text_Normal ) {
                if ([self.messageModel.message.uid isEqualToString:[ChatServiceController chatServiceControllerSharedManager].kingUidString ]) {
                    [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_left_normal"]];
                }else{
                    [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_NorMsg_normal"]];
                }
                
//              [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_NorMsg_normal"]];
            
            }else if( self.messageModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_Vip_normal"]];
            }else if( self.messageModel.messageBodyType == CSMessageBodyType_RedPackge){
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"chat_redenvelope_left_bg_normal"]];
            }else{
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_System_normal"]];
//                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_left_normal"]];
            }
           
        }
}
-(void)setterPressedBubbleBackImage{
 
    if (self.messageModel.isSelfSender) {
        if (self.messageModel.messageBodyType == CSMessageBodyType_Text_Normal ) {
            if ([self.messageModel.message.uid isEqualToString:[ChatServiceController chatServiceControllerSharedManager].kingUidString ]) {
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_right_normal"]];
            }else{
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_NorMsg_pressed"]];
            }

     
        }else if( self.messageModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_right_Vip_pressed"]];
        }else if( self.messageModel.messageBodyType == CSMessageBodyType_RedPackge){
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"chat_redenvelope_right_bg_pressed"]];
        }else{
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableImage:@"cs_chat_right_System_pressed"]];
        }
       
    }else{
        if (self.messageModel.messageBodyType == CSMessageBodyType_Text_Normal ) {
            if ([self.messageModel.message.uid isEqualToString:[ChatServiceController chatServiceControllerSharedManager].kingUidString ]) {
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_king_bubble_left_normal"]];
            }else{
                [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_NorMsg_pressed"]];
            }
          
        }else if( self.messageModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker){
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_Vip_pressed"]];
        }else if( self.messageModel.messageBodyType == CSMessageBodyType_RedPackge){
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"chat_redenvelope_left_bg_pressed"]];
        }else{
            [self.bubbleView.bubbleBackImageView setImage:[UIImage resizableBubbleImage:@"cs_chat_left_System_pressed"]];
        }
    }
}

#pragma mark -
#pragma mark setter




-(void)setMessageModel:(CSMessageModel *)messageModel{
    //父视图属性,传给父视图去负值
    [super setMessageModel:messageModel];
    //设置bubble背景
    [self setterNormalBubbleBackImage];
    
    if (messageModel.avatarCustomURLPath) {
        [self.cellView.avatarView setImageWithURL:[NSURL URLWithString:messageModel.avatarCustomURLPath] placeholder:[UIImage imageNamed:messageModel.avatarDefaultImageName]];
    }else{
        self.cellView.avatarView.image = [UIImage imageNamed:messageModel.avatarDefaultImageName];
    }
    //消息时间
    if (messageModel.isTimeLabelShow) {
        self.cellView.timeLabel.text = messageModel.timeString;
        [self.cellView.timeLabel sizeToFit];
 
        self.cellView.timeLabel.hidden = NO;
        self.cellView.timeLabel.top =10;
        CGSize timeSize =[messageModel.timeString sizeWithFont:_timeLabelFont maxSize:CGSizeMake(MAXFLOAT, MAXFLOAT)];
        self.cellView.timeLabel.width = timeSize.width + 10;
        self.cellView.timeLabel.height = timeSize.height + 6;
        self.cellView.timeLabel.left = (self.cellView.width - self.cellView.timeLabel.width)/2;
  
        self.cellView.bodyView.frame =CGRectMake(0, _timeLabelHeight, self.cellView.width, self.cellView.height-_timeLabelHeight  );
    }else{
        self.cellView.timeLabel.hidden = YES;
         self.cellView.bodyView.frame =CGRectMake(0, 0, self.cellView.width, self.cellView.height );
    }
    
   

    if (messageModel.isNickNameShow ) {
        self.cellView.nameLabel.hidden = NO;
        if (messageModel.nickNameAttribut) {
            self.cellView.nameLabel.attributedText = messageModel.nickNameAttribut;
             [self.cellView.nameLabel sizeToFit];
        }else{
            self.cellView.nameLabel.text = messageModel.nickname;
            [self.cellView.nameLabel sizeToFit];

        }
  
    }else{
         self.cellView.nameLabel.text = messageModel.nickname;
            [self.cellView.nameLabel sizeToFit];
        self.cellView.nameLabel.hidden = YES;
    }
    if (!messageModel.isSelfSender) {
        //左侧的消息
        self.cellView.reSendButton.hidden = YES;
        self.cellView.sendStatusActivityView.hidden = YES;
        
//        [self.cellView.avatarView setFrame:CGRectMake(self.avatarMargin, self.avatarMargin, self.avatarSize, self.avatarSize)];
//        self.cellView.avatarbackView.frame = self.cellView.avatarView.frame;
        [self.cellView.avatarbackView setFrame:CGRectMake(self.avatarMargin/2, self.avatarMargin, self.avatarSize, self.avatarSize)];
        CGRect headRect = self.cellView.avatarbackView.frame ;
        headRect.origin.x = headRect.origin.x+ 5;
        headRect.origin.y = headRect.origin.y + 5;
        headRect.size.width = headRect.size.width - 10;
        headRect.size.height = headRect.size.height -10;
        self.cellView.avatarView.frame = headRect;
        
        [self.cellView.nameLabel setFrame:CGRectMake(self.cellView.avatarbackView.right +self.avatarMargin, self.avatarMargin, self.cellView.nameLabel.width, self.messageNameHeight)];
        self.cellView.nameLabel.textAlignment = NSTextAlignmentLeft;
        CGFloat bubbleBackImageViewY = 0;
        if (messageModel.isNickNameShow == YES) {
            bubbleBackImageViewY = bubbleBackImageViewY + self.avatarMargin + self.messageNameHeight;
        }else{
            bubbleBackImageViewY = bubbleBackImageViewY + self.avatarMargin;
        }
        self.bubbleView.frame = CGRectMake(self.avatarMargin +self.avatarSize , bubbleBackImageViewY, messageModel.textBubbleBackImageSize.width, messageModel.textBubbleBackImageSize.height);
        [self.bubbleView.bubbleBackImageView setFrame:CGRectMake(0, 0, self.bubbleView.width, self.bubbleView.height)];
        
        
    }else{
        [self.cellView.avatarbackView setFrame:CGRectMake( kMainScreenWidth - self.avatarMargin/2-self.avatarSize, self.avatarMargin, self.avatarSize, self.avatarSize)];
        CGRect headRect = self.cellView.avatarbackView.frame ;
        headRect.origin.x = headRect.origin.x+ 5;
        headRect.origin.y = headRect.origin.y + 5;
        headRect.size.width = headRect.size.width - 10;
        headRect.size.height = headRect.size.height -10;
        self.cellView.avatarView.frame = headRect;
        [self.cellView.nameLabel setFrame:CGRectMake( kMainScreenWidth - self.avatarSize -self.avatarMargin -self.cellView.nameLabel.width-self.avatarMargin, self.avatarMargin, self.cellView.nameLabel.width, self.messageNameHeight)];
         self.cellView.nameLabel.textAlignment = NSTextAlignmentRight;
        
        CGFloat bubbleBackImageViewY = 0;
        if (messageModel.isNickNameShow == YES) {
            bubbleBackImageViewY = bubbleBackImageViewY + self.avatarMargin + self.messageNameHeight;
        }else{
            bubbleBackImageViewY = bubbleBackImageViewY + self.avatarMargin;
        }
        
        self.bubbleView.frame = CGRectMake(kMainScreenWidth - self.avatarMargin -self.avatarSize- messageModel.textBubbleBackImageSize.width, bubbleBackImageViewY, messageModel.textBubbleBackImageSize.width, messageModel.textBubbleBackImageSize.height);
        [self.bubbleView.bubbleBackImageView setFrame:CGRectMake(0, 0, self.bubbleView.width, self.bubbleView.height)];
        self.cellView.reSendButton.left = self.bubbleView.left-self.cellView.reSendButton.width;
        self.cellView.reSendButton.top = self.bubbleView.top+self.bubbleView.height/2-self.cellView.reSendButton.height/2;
        self.cellView.sendStatusActivityView.center = self.cellView.reSendButton.center;
        
        if (messageModel.messageSendStatus == CSMessageSendState_Delivering) {
            self.cellView.reSendButton.hidden = YES;
            [self.cellView.sendStatusActivityView startAnimating];
            self.cellView.sendStatusActivityView.hidden = NO;
        }else if (messageModel.messageSendStatus == CSMessageSendState_Failure) {
             self.cellView.sendStatusActivityView.hidden = YES;
            self.cellView.reSendButton.hidden = NO;
            [self.cellView.sendStatusActivityView stopAnimating];
           
        }else{
            self.cellView.sendStatusActivityView.hidden = YES;
            self.cellView.reSendButton.hidden = YES;
        }
        
    }
    
    
    
}


- (void)setMessageNameFont:(UIFont *)messageNameFont
{
    _messageNameFont = messageNameFont;
    if (self.cellView.nameLabel) {
        self.cellView.nameLabel.font = _messageNameFont;
    }
}

- (void)setMessageNameColor:(UIColor *)messageNameColor
{
    _messageNameColor = messageNameColor;
    if (self.cellView.nameLabel) {
        self.cellView.nameLabel.textColor = _messageNameColor;
    }
}

- (void)setMessageNameHeight:(CGFloat)messageNameHeight
{
    _messageNameHeight = messageNameHeight;
    if (self.cellView.nameLabel) {
        self.cellView.nameLabel.height = _messageNameHeight;
    }
}

- (void)setAvatarSize:(CGFloat)avatarSize
{
    _avatarSize = avatarSize;
    
}

- (void)setAvatarCornerRadius:(CGFloat)avatarCornerRadius
{
    _avatarCornerRadius = avatarCornerRadius;
    if (self.cellView.avatarView){
        self.cellView.avatarView.layer.cornerRadius = avatarCornerRadius;
    }
}

@end
