//
//  CSBaseMessageCell.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSBaseMessageCell.h"
#import "CSMessageModel.h"
#import "UIViewExt.h"
#import "Cellconfig.h"
#import "NSObject+HelpModel.h"
#import "CSMessageTableCell.h"
@interface CSBaseMessageCell()<CSBubbleViewDelegate>

@end
@implementation CSBaseMessageCell

-(void)initialize{
//    CSBaseMessageCell *cell = [self appearance];
    self.resendButtonSize = cellConfig.resendButtonSize;
    self.activitySize = cellConfig.activitySize;
    self.bubbleMaxWidth = cellConfig.bubbleMaxWidth;
    self.leftBubbleMargin = cellConfig.leftBubbleMargin;
    self.rightBubbleMargin = cellConfig.rightBubbleMargin;
    self.avatarMargin = cellConfig.avatarMargin;
}

- (instancetype)initWithStyle:(UITableViewCellStyle)style
              reuseIdentifier:(NSString *)reuseIdentifier
                        model:(CSMessageModel *)vModel
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        
        [self initialize];
        [self _setupSubViews];
    }else{
//        [self.bubbleView clearContentSubViews];
    }
    
    return self;
}
-(void)_setupSubViews{
   
    _cellView = [[CellView alloc]initWithFrame:CGRectZero];
    [self.contentView addSubview:_cellView];
 
    _bubbleView =[[CSBubbleView alloc]initWithFrame:CGRectZero];
    _bubbleView.bubbleViewDelegate = self;
    _bubbleView.backgroundColor =[UIColor clearColor];
    [self.cellView.bodyView addSubview:_bubbleView];
    //触摸事件
    [_cellView.reSendButton addTarget:self action:@selector(resendButtonAction) forControlEvents:UIControlEventTouchUpInside];
    UITapGestureRecognizer *tapRecognizer2 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(avatarViewTapAction:)];
    [_cellView.avatarView addGestureRecognizer:tapRecognizer2];
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(bubbleViewTapAction:)];
    [_bubbleView addGestureRecognizer:tapRecognizer];

}


- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(void)setMessageModel:(CSMessageModel *)messageModel{
    _messageModel = messageModel;
    [self.cellView setFrame:CGRectMake(0, 0, kMainScreenWidth, messageModel.cellHeight)];
    if (messageModel.isSelfSender) {
        if (messageModel.messageBodyType == CSMessageBodyType_RedPackge) {
            _bubbleView.margin = cellConfig.redPacketRightBubbleMargin;
        }else{
            _bubbleView.margin = _rightBubbleMargin;
        }
        
    }else{
        if (messageModel.messageBodyType == CSMessageBodyType_RedPackge) {
            _bubbleView.margin = cellConfig.redPacketLeftBubbleMargin;
        }else{
            _bubbleView.margin = _leftBubbleMargin;
        }
       
    }
    _bubbleView.msgModel = messageModel;
  
}
#pragma mark - action
- (void)avatarViewTapAction:(UITapGestureRecognizer *)tapRecognizer
{
    DVLog(@"点击头像 message:%@",[self.messageModel printAllPropertys])
    if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgCellAvatarViewSelected:)]) {
        [_msgTableCellDelegate msgCellAvatarViewSelected:self];
    }
    
}

- (void)bubbleViewTapAction:(UITapGestureRecognizer *)tapRecognizer
{
    if (self.messageModel.messageBodyType ==CSMessageBodyType_RedPackge ||
       self.messageModel.messageBodyType == CSMessageBodyType_Text_EquipShare ||
         self.messageModel.messageBodyType == CSMessageBodyType_Text_FightReport ||
         self.messageModel.messageBodyType == CSMessageBodyType_Text_InvestigateReport ||
         self.messageModel.messageBodyType == CSMessageBodyType_Text_AllianceConciles ||
         self.messageModel.messageBodyType == CSMessageBodyType_Text_TurntableShare ||
         self.messageModel.messageBodyType == CSMessageBodyType_Text_AllianceTask    ) {
        CSMessageTableCell *subSelf = (CSMessageTableCell *)self;
        if (tapRecognizer.state == UIGestureRecognizerStateEnded) {
            [subSelf longRecognizerPresed];
            [self performSelector:@selector(bubbleBackResumeNormal) withObject:nil afterDelay:0.3];
            
            if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgcellBubbleViewSelected:)]) {
                [_msgTableCellDelegate msgcellBubbleViewSelected:self];
            }
        }

    }else {
        return;
    }
}

-(void)bubbleBackResumeNormal{
    CSMessageTableCell *subSelf = (CSMessageTableCell *)self;
     [subSelf longRecognizerResumeNormal];
}
-(void)bubbleViewTextTouchDownWithTouchTypeString:(NSString *)vTypeString andTouchString:(NSString *)vTouchString{
    if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgcellTextBubbleViewTouchedWithCell:andWithTouchInfo:)]) {
        [_msgTableCellDelegate msgcellTextBubbleViewTouchedWithCell:self andWithTouchInfo:@{vTypeString:vTouchString}];
    }
}
- (void)resendButtonAction
{
    if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgcellResendButtonSelected:)]) {
        [_msgTableCellDelegate msgcellResendButtonSelected:self];
    }
}

#pragma mark -
#pragma mark  CSBubbleViewDelegate
-(void)bubbleViewTouchDownStringWithBubbleView:(CSBubbleView *)vBubbleView andWithTouchType:(NSString *)vTypeString  andWithTouchString:(NSString *)vTouchString{
    [self bubbleViewTextTouchDownWithTouchTypeString:vTypeString andTouchString:vTouchString];
}
@end
