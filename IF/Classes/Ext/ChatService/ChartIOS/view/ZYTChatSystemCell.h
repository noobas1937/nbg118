//
//  ZYTChatSystemCell.h
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import <UIKit/UIKit.h>
@class ChatCellFrame;
@interface ZYTChatSystemCell : UITableViewCell
+ (instancetype)chatSystemCellWithTableView:(UITableView *)tableView;
@property (nonatomic,strong)ChatCellFrame *chatCellFrame;

@end
