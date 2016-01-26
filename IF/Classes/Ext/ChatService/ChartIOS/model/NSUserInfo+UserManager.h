//
//  NSUserInfo+UserManager.h
//  IF
//
//  Created by 张彦涛 on 15/12/28.
//
//

#import "NSUserInfo.h"
#include "ChatMailInfo.h"
@interface NSUserInfo (UserManager)
+(void)userCheckWithChatMailInfo:(ChatMailInfo *)vChatMailInfo;
-(NSString *)vipStringWithSelf;
-(BOOL)isVipShow;
@end
