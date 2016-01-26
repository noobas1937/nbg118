//
//  MailListCell.h
//  IF
//
//  Created by 张彦涛 on 15/8/19.
//
//

#import <UIKit/UIKit.h>
#import "SlideCellProtocol.h"
@class MailListCell;
@class ChatChannel;
@protocol MailListCellDelegate <NSObject>
@optional
-(void)mailListCellSelectedButtonActionWithCell:(MailListCell *)vCell andWithButton:(UIButton *)sender;

@end
@interface MailListCell : UITableViewCell

-(void)initView;
-(void)closeUpCell:(MailListCell*)cell;
@property(nonatomic,strong)UIButton * readButton;
@property(nonatomic,strong)UIButton * deleteButton;
@property(nonatomic,weak)id<SlideCellProtocol>delegate;


@property (nonatomic,strong)ChatChannel *chatChannel;
@property (nonatomic,assign,getter=isShowSelectedButton)BOOL showSelectedButton;
@property (nonatomic,strong)id<MailListCellDelegate>mailListCellDelegate;
+ (instancetype)mailListCellWithTableView:(UITableView *)tableView;
+(CGFloat )mailListCellRowHeight;

-(void)settingSelectedButtonSelected:(BOOL)isSelected;
- (IBAction)selectButtonAction:(UIButton *)sender;

-(void)initView;
-(void)closeUpCell:(MailListCell*)cell;

@end
