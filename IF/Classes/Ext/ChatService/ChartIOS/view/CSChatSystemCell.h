//
//  CSChatSystemCell.h
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import <UIKit/UIKit.h>
@class ChatCellFrame;
@class CSMessageModel;

@interface InsetsSubLabel : UILabel

@property(nonatomic) UIEdgeInsets insets;

- (id)initWithFrame:(CGRect)frame andInsets:(UIEdgeInsets) insets;
- (id)initWithInsets:(UIEdgeInsets) insets;

@end



@interface CSChatSystemCell : UITableViewCell
+ (instancetype)chatSystemCellWithTableView:(UITableView *)tableView;
@property (nonatomic,strong)ChatCellFrame *chatCellFrame;
@property (nonatomic,strong)CSMessageModel *msgModel;
@end
