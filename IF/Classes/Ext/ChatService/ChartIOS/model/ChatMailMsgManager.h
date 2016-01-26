//
//  ChatMailMsgManager.h
//  IF
//
//  Created by 张彦涛 on 15/8/28.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
#import "ChatMailInfo.h"
@interface ChatMailMsgManager : NSObject

+(NSMsgItem *)chatMsgWithChatMailInfo:(ChatMailInfo *)vMailInfo;
@end
