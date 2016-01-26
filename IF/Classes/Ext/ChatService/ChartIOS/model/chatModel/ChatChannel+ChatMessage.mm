//
//  ChatChannel+ChatMessage.m
//  IF
//
//  Created by 张彦涛 on 15/12/15.
//
//

#import "ChatChannel+ChatMessage.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
#import "DBManager.h"
#import "ServiceInterface.h"
#import "NSArray+SortHelper.h"
#import "UserManager.h"
@implementation ChatChannel (ChatMessage)

-(BOOL)chackSequenceIDContinuouslyWithSequenceID:(long)vMinSeq{
    if (vMinSeq > 0) {
        
        NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
        LKLog(@"聊天表名:%@",nameString);
        long firstSeq = vMinSeq-20;
        if (firstSeq > 0) {
            int count =[[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:nameString withModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(SequenceID >= %ld and SequenceID < %ld)",firstSeq,vMinSeq]];
            if (count == 20) {
                return YES;
            }else{
                return NO;
            }
        }else{
            return YES;
        }
        
    }else{
        return YES;
    }
}

/** 根据当前channel获取channel对应聊天表最后的count条数据 */
-(NSArray *)gettingMsgItemFromDBWithMsgCount:(int )vCount{
    if (vCount <= 0) {
        vCount = 20;
    }
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    NSArray *array;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:nil orderBy:@"CreateTime desc" offset:0 count:vCount];
    }else{
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:nil orderBy:@"SequenceID desc" offset:0 count:vCount];
    }
    
    NSArray *msgArray = nil;
    
    /** 保证sequenceId 一小到大的顺序 */
    if (array.count>= 1) {
        msgArray = [NSArray sortingMessageWithMsgArray:(NSMutableArray *)array andWithChannelType:self.channelType];
         DVLog(@"%@",msgArray);
    }
    return msgArray;
}
/** 获取DB历史数据 */
-(NSArray *)gettingOldMessageFromDBWithMsgCount:(int )vCount{
    /** 是以msgList中的消息为计算基准，聊天消息，时间正序，所以取msgList的第一条数据计算 */
    if (vCount<= 0) {
        vCount = 20;
    }
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    NSInteger  offsetCount = self.msgList.count ;
    
    NSArray *array;
    /** 保证sequenceId 一小到大的顺序 */
    //    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
    //        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:nil orderBy:@"CreateTime desc " offset:offsetCount count:10];
    //    }else{
    if (offsetCount >0){
        CSMessageModel *firstModel =[self.msgList objectAtIndex:0];
        
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"CreateTime < %ld",firstModel.message.createTime] orderBy:@"CreateTime desc " offset:0 count:vCount];
    }
    
    //    }
    
    NSArray *msgArray = [NSArray sortingMessageWithMsgArray:(NSMutableArray *)array andWithChannelType:self.channelType];
        DVLog(@"%@",msgArray);
    return msgArray;
}

/** 自己发送的消息先插库 */
-(void)savingMessageForSelfSendToServerWithMessage:(CSMessage *)vMsg{
    if (vMsg.createTime != vMsg.sendLocalTime){
        vMsg.createTime = vMsg.sendLocalTime;
    }
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    CSMessage *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(LocalSendTime = %ld )",vMsg.sendLocalTime  ] orderBy:@"_id desc"];
    if (tempMsg == nil) {
        
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsg];
    }else{
        vMsg._id = tempMsg._id;
        [[DBManager sharedDBManager].dbHelper  updateToDBWithModelTableName:nameString
                                                               andWithModel:vMsg
                                                                      where:[NSString stringWithFormat:@"(_id = %ld)",vMsg._id ]  ];
    }
}
/** 消息数据插库，服务器消息 */
-(void)savingMessageToDBWithMsg:(CSMessage *)vMsgItem{
    
    
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    
    
    if ([[UserManager sharedUserManager].currentUser.uid isEqualToString:vMsgItem.uid]){
        
        
        //自己的消息
        if (vMsgItem.sendLocalTime == 0) {
            CSMessage *tempMsg = nil;
            if (self.channelType != IOS_CHANNEL_TYPE_USER){
                
                tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"SequenceID = %ld",vMsgItem.sequenceId] orderBy:@"CreateTime desc"];
            }else{
                tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(MailID = '%@')", vMsgItem.mailId] orderBy:@"CreateTime desc"];
            }
            //            CSMessage *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(MailID = '%@')", vMsgItem.mailId] orderBy:@"CreateTime desc"];
            if (tempMsg == nil) {
                
                [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsgItem];
            }else{
                vMsgItem._id = tempMsg._id;
                [[DBManager sharedDBManager].dbHelper    updateToDBWithModelTableName:nameString
                                                                         andWithModel:vMsgItem
                                                                                where:[NSString stringWithFormat:@"(_id = %ld)",vMsgItem._id ]  ];
            }
        }else{
            CSMessage *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(LocalSendTime = %ld )",vMsgItem.sendLocalTime  ] orderBy:@"_id desc"];
            if (tempMsg == nil) {
                
                [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsgItem];
            }else{
                vMsgItem._id = tempMsg._id;
                //                [[DBManager sharedDBManager].dbHelper updateToDBBaseWithModelTableName:nameString andWithModel:vMsgItem where:[NSString stringWithFormat:@"(LocalSendTime = %ld and Msg = '%@' and LocalSendTime >0)",vMsgItem.sendLocalTime,vMsgItem.msg]];
                [[DBManager sharedDBManager].dbHelper    updateToDBWithModelTableName:nameString
                                                                         andWithModel:vMsgItem
                                                                                where:[NSString stringWithFormat:@"(_id = %ld)",vMsgItem._id ]  ];
            }
        }
        
    }else{
        
        CSMessage *tempMsg = nil;
        if (self.channelType != IOS_CHANNEL_TYPE_USER){
            
            tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"SequenceID = %ld",vMsgItem.sequenceId] orderBy:@"CreateTime desc"];
        }else{
            tempMsg =[[DBManager sharedDBManager].dbHelper
                      searchSingleWithModelTableName:nameString
                      andWithModelClass:[CSMessage class]
                      where:[NSString stringWithFormat:@"(MailID = '%@')",vMsgItem.mailId]
                      orderBy:@"CreateTime desc"];
        }
        
        if (tempMsg ){
            vMsgItem._id = tempMsg._id;
            
            //            [[DBManager sharedDBManager].dbHelper updateToDBBaseWithModelTableName:nameString andWithModel:vMsgItem where:[NSString stringWithFormat:@"CreateTime = %ld",vMsgItem.createTime]];
            if (vMsgItem.post == 12) {
                return;
            }
            [[DBManager sharedDBManager].dbHelper    updateToDBWithModelTableName:nameString
                                                                     andWithModel:vMsgItem
                                                                            where:[NSString stringWithFormat:@"(_id = %ld)",vMsgItem._id ]  ];
            
        }else{
            [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsgItem];
        }
        
    }
}


-(CSMessage *)gettingLastMessageFromDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSArray *array ;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:nil orderBy:@"CreateTime desc " offset:0 count:1];
    }else if(self.channelType == IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.channelType == IOS_CHANNEL_TYPE_CHATROOM )
    {
        array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:nil orderBy:@"SequenceID desc " offset:0 count:1];
    }
    
    if (array.count> 0) {
        return (CSMessage *)[array lastObject];
    }else{
        return nil;
    }
}

-(CSMessage *)gettingLastMessageForNotSelfSendWithDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSArray *array ;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"UserID <> '%@'",[UserManager sharedUserManager].currentUser.uid] orderBy:@"CreateTime desc " offset:0 count:1];
    }
    if (array.count> 0) {
        return (CSMessage *)[array lastObject];
    }else{
        return nil;
    }
}
-(void)redPacketMsgUpdateStautsWithMessage:(CSMessage *)vMsgItem{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    CSMessage *tempMsg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[CSMessage class] where:[NSString stringWithFormat:@"(SequenceID = %ld)",vMsgItem.sequenceId] orderBy:@"_id desc"];
    if (tempMsg){
        tempMsg.attachmentId = vMsgItem.attachmentId;
        [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:nameString andWithModel:tempMsg where:[NSString stringWithFormat:@"(_id =  %ld)",tempMsg._id]];
    }
}

 
@end
