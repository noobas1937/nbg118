//
//  ChatChannel+ChatMessage.h
//  IF
//
//  Created by 张彦涛 on 15/12/15.
//
//

#import "ChatChannel.h"
@class CSMessage;
@interface ChatChannel (ChatMessage)
/** 判断数据库seq之前的20条数据是不是全的 */
-(BOOL)chackSequenceIDContinuouslyWithSequenceID:(long)vMinSeq;
/** 获取最后 count条数据 */
-(NSArray *)gettingMsgItemFromDBWithMsgCount:(int )vCount;
/** 获取内存第一条之前的数据库count条数据 */
-(NSArray *)gettingOldMessageFromDBWithMsgCount:(int )vCount;



/** 存储自己真在发送的消息 */
-(void)savingMessageForSelfSendToServerWithMessage:(CSMessage *)vMsg;
/** 服务器消息存储 */
-(void)savingMessageToDBWithMsg:(CSMessage *)vMsgItem;

/** 当前channel的最后一条消息 */
-(CSMessage *)gettingLastMessageFromDB;

-(CSMessage *)gettingLastMessageForNotSelfSendWithDB;
/** 更新数据库红包消息状态 */
-(void)redPacketMsgUpdateStautsWithMessage:(CSMessage *)vMsgItem;


@end

