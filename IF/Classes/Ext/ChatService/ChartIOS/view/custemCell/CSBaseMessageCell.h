//
//  CSBaseMessageCell.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CSMessageModel.h"
#import "CellView.h"
#import "CSBubbleView.h"

@class CSBaseMessageCell;
@protocol CSMsgTableCellDelegate <NSObject>
@optional
-(void)msgCellAvatarViewSelected:(CSBaseMessageCell *)vCell;
-(void)msgcellResendButtonSelected:(CSBaseMessageCell *)vCell;
-(void)msgcellBubbleViewSelected:(CSBaseMessageCell *)vCell;
-(void)msgcellTextBubbleViewTouchedWithCell:(CSBaseMessageCell *)vCell andWithTouchString:(NSString *)vTouchString;
-(void)msgcellTextBubbleViewTouchedWithCell:(CSBaseMessageCell *)vCell andWithTouchInfo:(NSDictionary *)vInfoDic;
@end


@interface CSBaseMessageCell : UITableViewCell
@property (nonatomic,assign)id<CSMsgTableCellDelegate>msgTableCellDelegate;
@property (nonatomic,strong) CSMessageModel *messageModel;
@property (nonatomic,strong) CellView *cellView;
@property (nonatomic,strong) CSBubbleView *bubbleView;
/** resendButton size */
@property (nonatomic) CGFloat resendButtonSize UI_APPEARANCE_SELECTOR; //default 20;
/** 菊花 size */
@property (nonatomic) CGFloat activitySize UI_APPEARANCE_SELECTOR; //default 20;
/** 气泡最大宽度 */
@property (nonatomic) CGFloat bubbleMaxWidth UI_APPEARANCE_SELECTOR; //default 200;
/** 名字默认大小 */
@property  (nonatomic)CGFloat nicklabelFontSize UI_APPEARANCE_SELECTOR;
/** 左边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets leftBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 15, 8, 10);
/** 右边bubble在气泡backImage的内边距 */
@property (nonatomic) UIEdgeInsets rightBubbleMargin UI_APPEARANCE_SELECTOR; //default UIEdgeInsetsMake(8, 10, 8, 15);
/** 头像边距 */
@property (nonatomic)CGFloat avatarMargin;

- (instancetype)initWithStyle:(UITableViewCellStyle)style
              reuseIdentifier:(NSString *)reuseIdentifier
                        model:(CSMessageModel *)vModel;
-(void)initialize;
@end
