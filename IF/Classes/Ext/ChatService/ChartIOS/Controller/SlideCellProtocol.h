//
//  SlideCellProtocol.h
//  IF
//
//  Created by Zsl on 15/11/20.
//
//

#import <Foundation/Foundation.h>


@class CSMailCategroyListTableCell,SysMailListCell,MailListCell;
@protocol SlideCellProtocol <NSObject>
@optional

-(void)cellbuttonClick:(UIButton*)button;
-(void)cellbuttonClick:(UIButton*)button andWithMailCategroyListCell:(UITableViewCell *)vCell;


-(BOOL)RootCellopen:(CSMailCategroyListTableCell*)cell;
-(BOOL)RootCellclose:(CSMailCategroyListTableCell *)cell;


-(BOOL)SysMailopen:(SysMailListCell*)cell;
-(BOOL)SysMailclose:(SysMailListCell *)cell;

-(BOOL)Mailopen:(MailListCell*)cell;
-(BOOL)Mailclose:(MailListCell *)cell;

-(BOOL)isSelectedButtonShow;
@end
