//
//  BanPopUpController.h
//  IF
//
//  Created by mzc on 15/4/17.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"
@class CSMessageModel;
@interface BanPopUpController : UIViewController
-(void) setInitData:(NSMsgItem*) chatMessage;
-(void)settingInitDataWithMessageModel:(CSMessageModel *)vMessageModel;
@end
