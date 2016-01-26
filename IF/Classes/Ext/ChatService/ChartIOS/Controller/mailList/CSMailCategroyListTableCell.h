//
//  CSMailCategroyListTableCell.h
//  IF
//
//  Created by 张彦涛 on 15/10/9.
//
//

#import <UIKit/UIKit.h>
#import "CSMailCategoryListModel.h"
#import "SlideCellProtocol.h"
@interface CSMailCategroyListTableCell : UITableViewCell<UIGestureRecognizerDelegate>



@property (nonatomic,strong) CSMailCategoryListModel * mailCategoryListModel;

//
@property(nonatomic,strong)UIView * myContentView;
@property(nonatomic,strong)UIButton * readButton;
@property(nonatomic,weak)id<SlideCellProtocol>delegate;
@property(nonatomic,assign)BOOL isOpenCell;

@property (weak, nonatomic) IBOutlet UIView *showMainView;

-(void)initView;
-(void)closeUpCell:(CSMailCategroyListTableCell*)cell;
//

+ (instancetype)mailCategroyListTableCellWithTableView:(UITableView *)tableVie;


@end
