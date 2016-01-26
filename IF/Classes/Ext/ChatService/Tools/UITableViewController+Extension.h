//
//  UITableViewController+Extension.h
//  IF
//
//  Created by 马纵驰 on 15/8/3.
//
//

#import <UIKit/UIKit.h>

@interface UITableViewController (Extension)
/**滚动到最新消息*/
-(void) tableViewScrollCurrentLine;
/**下拉刷新跳转下拉起始位置*/
- (void) adjustLocation:(CGFloat) offsetY;
/**跳转C++ 返回 原生IOS时tabelview滚动的Y点*/
- (void) adjustLocation2:(CGPoint) offsetPoint;
/**开始刷新*/
-(void) beginRefreshing;
@end
