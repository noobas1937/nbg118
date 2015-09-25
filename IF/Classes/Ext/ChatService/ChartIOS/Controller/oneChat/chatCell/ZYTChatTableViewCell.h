//
//  ZYTChatTableViewCell.h
//  IF
//
//  Created by 张彦涛 on 15/7/9.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"

@interface ZYTChatTableViewCell : UITableViewCell


@property (nonatomic,strong)NSMsgItem *msgItem;

+(instancetype)chatTableVIewCellWithTableView:(UITableView *)vTableView;
@end
