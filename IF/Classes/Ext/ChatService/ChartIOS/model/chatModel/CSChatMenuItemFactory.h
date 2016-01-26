//
//  CSChatMenuItemFactory.h
//  IF
//
//  Created by 张彦涛 on 15/12/23.
//
//

#import <Foundation/Foundation.h>
#import "ChatServiceManagerDefs.h"
#import "QBPopupMenu.h"
@class CSMessageModel;
@interface CustemMenuItem :UIMenuItem
@property (nonatomic,assign)CSMenuItemType menuItemType;
@end


@interface CSChatMenuItemFactory : NSObject
+(NSMutableArray *)addMenuItemWithChatMsgModel:(CSMessageModel *)vChatMsgModel andWithTarget:(UIViewController * )vc andWithSelect:(SEL)vSel;
@end
