//
//  SysMailListCell.h
//  IF
//
//  Created by 张彦涛 on 15/9/9.
//
//
#import "YYKit.h"
#import "YYLabel.h"
#import <UIKit/UIKit.h>
#import "MailInfoDataModel.h"
#import "SlideCellProtocol.h"
@class SysMailListCell;
@protocol SysMailListCellDelegate <NSObject>
@optional
-(void)sysMailListCellSelectedButtonActionWithCell:(SysMailListCell *)vCell andWithButton:(UIButton *)sender;

@end
@interface SysMailListCell : UITableViewCell


-(void)initView;
-(void)closeUpCell:(SysMailListCell*)cell;

@property(nonatomic,strong)UIButton * readButton;
@property(nonatomic,strong)UIButton * deleteButton;

@property (nonatomic,assign,getter=isSelectedButtonShowing) BOOL  selectedButtonShowing;
@property (nonatomic,strong)MailInfoDataModel *mailInforDataModel;
@property (nonatomic,assign) id<SysMailListCellDelegate> sysMailListCellDelegate;
+ (instancetype)sysMailListCellWithTableView:(UITableView *)tableView;
+(CGFloat)rowHeightWithSysMailListCell;
- (IBAction)selectButtonAction:(UIButton *)sender;
@property(nonatomic,weak)id<SlideCellProtocol>delegate;
 
@end
