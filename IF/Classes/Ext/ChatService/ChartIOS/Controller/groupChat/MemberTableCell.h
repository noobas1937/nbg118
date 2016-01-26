//
//  MemberTableCell.h
//  CreatMoreChat
//
//  Created by 张彦涛 on 15/6/10.
//  Copyright (c) 2015年 elex-tech. All rights reserved.
//

#import <UIKit/UIKit.h>
@class NSUserInfo;
@class MemberTableCell;

@protocol MemberTableCellDelegate <NSObject>
@optional
- (void)clickButtonActionWithCell:(MemberTableCell *)vCell
                andSelectedButton:(UIButton *)vSender;

@end
@interface MemberTableCell : UITableViewCell

@property (nonatomic , strong) NSUserInfo * userInfo;
@property (nonatomic,assign)id<MemberTableCellDelegate>memberCellDelegate;
+ (instancetype)cellWithTableView:(UITableView *)tableView;
@end
