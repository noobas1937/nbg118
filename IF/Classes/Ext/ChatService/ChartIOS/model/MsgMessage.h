//
//  MsgMessage.h
//  IF
//
//  Created by mzc on 15/4/22.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
@interface MsgMessage : NSObject
+(instancetype)msgMessageShared;


//自己的信息本地存储
@property (nonatomic,strong) NSMutableArray *chatMsgList;
//邮件初始化信息
@property (nonatomic,strong) NSMutableArray *mailInitDataList;
/**根据id获取频道最后一条消息*/
-(NSMsgItem*) gettingFirstMsg:(NSString*) channelID;

@property (nonatomic,assign) int redPackageTime;

-(void) addChatMsgList:(NSMsgItem *)cm;
@end
static MsgMessage * msgMessage = nil;