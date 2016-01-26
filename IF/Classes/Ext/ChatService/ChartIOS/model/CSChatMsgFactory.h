//
//  CSChatMsgFactory.h
//  IF
//
//  Created by 张彦涛 on 15/10/13.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
#import "CSMessage.h"
#import "ChatMailInfo.h"
@interface CSChatMsgFactory : NSObject

+(NSMsgItem *)chatMsgWithChatMailInfo:(ChatMailInfo *)vChatMailInfo;
+(CSMessage *)chatMessageWithChatMailInfo:(ChatMailInfo *)vChatMailInfo;
@end
