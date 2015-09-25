//
//  MsgMessage.m
//  IF
//
//  Created by mzc on 15/4/22.
//
//

#import "MsgMessage.h"
#import "NSMsgItem.h"
#import "ChannelManager.h"
#import "ChatCellFrame.h"
@implementation MsgMessage

-(void) addChatMsgList:(NSMsgItem *)cm
{
    [self.chatMsgList addObject:cm];
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

-(MsgMessage*)init
{
    self = [super init];
    if (self) {
        self.mailInitDataList = [[NSMutableArray alloc]init];
        self.chatMsgList = [[NSMutableArray alloc]init];
    }
    return self;
}

+ (instancetype)msgMessageShared
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        msgMessage = [[MsgMessage alloc] init];
    });
    return msgMessage;
}

+(id)allocWithZone:(struct _NSZone *)zone{
    //调用dispatch_once保证在多线程中也只被实例化一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        msgMessage = [super allocWithZone:zone];
    });
    return msgMessage;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

-(NSMsgItem*)gettingFirstMsg:(NSString *)channelID
{
    ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:channelID];
    ChatCellFrame *ccf = [cc.msgList firstObject];
    return ccf.chatMessage;
}
@end
