//
//  ChatChannel.m
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import "ChatChannel.h"
#import "ServiceInterface.h"
#import "UserManager.h"
#import "MsgMessage.h"
#import "LKDBHelper.h"
#import "ChatServiceController.h"
#import "ChatServiceCocos2dx.h"
#import "MailInfoDataModel.h"
#import "ResourceHelpMailContents.h"
#import "ResourceHelpMailData.h"
#import "ResourceMailContents.h"
#import "MonsterMailContents.h"
#import "MonsterMailData.h"
#import "CSShowTimeTool.h"
#import "ChannelManager.h"
#import "NSDateFormatter+Category.h"
#import "NSArray+SortHelper.h"
#import "BattleMailData.h"


#import "ChatChannel+ChatMessage.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
#import "CSMessageModel+ModelManager.h"

#import "ChatChannel+Command.h"
@interface ChatChannel ()

@property (nonatomic,strong) dispatch_queue_t chatMessageQueue;
@property (nonatomic,strong) dispatch_queue_t chatCommandRequestQueue;
@end

@implementation ChatChannel
-(void)setUnreadCount:(int)unreadCount{
    if (_unreadCount > 0 && unreadCount == 0){
        
    }
    _unreadCount = unreadCount;
}

-(void)setNameString:(NSString *)nameString{
    _nameString = nameString;
}
+(NSString*) getMembersString:(NSMutableArray*)members
{
    NSString *uidsStr = @"";
    if (members == nil) return uidsStr;
    
    for (int i = 0; i < [members count]; i++)
    {
        
        uidsStr = [uidsStr stringByAppendingString:(i > 0 ? @"|" : @"")];
        
        uidsStr = [uidsStr stringByAppendingString:[members objectAtIndex:i]];
    
    }
    return uidsStr;
}

-(BOOL) containCurrentUser
{
    // 已经退出的国家
    if (self.channelType == IOS_CHANNEL_TYPE_COUNTRY
        && ![self.channelID isEqualToString:[NSString stringWithFormat:@"%ld",[UserManager sharedUserManager].currentUser.serverId]]);
    {
        return false;
    }
    // 已经退出的联盟
    if (self.channelType == IOS_CHANNEL_TYPE_ALLIANCE
        && ![self.channelID isEqualToString:[UserManager sharedUserManager].currentUser.allianceId]);
    {
        return false;
    }
    // 已经退出的聊天室
    if (self.channelType == IOS_CHANNEL_TYPE_CHATROOM && self.isMember == 0)
    {
        return false;
    }
    return true;
}

-(instancetype)init  {
    self = [super init];
    if (self) {
        self.memberUidArray = [[NSMutableArray alloc]init];
        self.msgList = [[NSMutableArray alloc]init];
        self.sendingMsgList = [[NSMutableArray alloc]init];
        self.dbMinSeqId = 0;
        self.dbMaxSeqId = 0;
        self.serverMinSeqId = 0;
        self.serverMaxSeqId = 0;
    }
    return self;
}

 
-(dispatch_queue_t)chatMessageQueue{
    return [ChannelManager sharedChannelManager].messageQueue;
}
-(dispatch_queue_t)chatCommandRequestQueue{
    return [ChannelManager sharedChannelManager].commandRequestQueue;
    
}
#pragma mark -
#pragma mark setter
-(void)setChannelID:(NSString *)channelID{
    _channelID = channelID;
    
}
#pragma mark -
#pragma mark getter
-(NSMutableArray *)memberUidArray{
    if  (_memberUidArray == nil){
        _memberUidArray = [NSMutableArray array ];
    }
    return _memberUidArray;
}
-(void)gettingMsgWithLatestCount:(int )vCount{
     if (ChatServiceCocos2dx::DB_MsgItem_switch) {
         NSArray *msgArray = [self gettingMsgItemWithDBWithCount:vCount];
         NSMutableArray *msgFrameArr = [NSMutableArray array];
         if (msgArray.count >0){
          
          
             for (NSMsgItem *msg in msgArray) {
                 [msg msgFormatFromDBwithAdd:self.channelID];
                 ChatCellFrame *cellFrame = [self createNewCellFrame:msg];
                 [msgFrameArr addObject:cellFrame];
             }
             self.msgList = msgFrameArr;
         }
     }
}
#pragma mark -
#pragma mark  action

-(NSMutableArray *)messageFormatToMessageModel:(NSArray *)vTempArray{
    NSMutableArray *msgFrameArr = [NSMutableArray array];
    
    for (CSMessage *msg in vTempArray) {
        //                        DVLog(@"%@",[msg printAllPropertys]);
        
        //                    [msg msgFormatFromDBwithAdd:self.channelID];
        
        CSMessageModel *msgModel =[[CSMessageModel alloc]initWithMessage:msg];
        msgModel.currentChannelID = self.channelID;
        if  (msgModel.message.createTime > self.latestTime){
            if  (msgModel.message.createTime> self.latestModifyTime){
                if  (![msgModel.message.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
                        if (msgModel.message.readState == 0) {
                            self.unreadCount = self.unreadCount + 1;
                        }
                    }else{
                        self.unreadCount = self.unreadCount + 1;
                    }

                }
                
            }
        }
        [msgModel messageModelShowingFormat];
    
  
        [msgFrameArr addObject:msgModel];
    }
    return msgFrameArr;
}

#pragma mark -
#pragma mark 消息数据库获取接口
-(void)gettingFirstMsg{
    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
       
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New
//            && (self.channelType== IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE)
            ){
            NSArray *msgArray = [self gettingMsgItemFromDBWithMsgCount:20];
 
            self.msgList = [self messageFormatToMessageModel:msgArray];
                [self timeLabelShowStatusFormat];
                if (self.channelDelegate != nil){
                    if  ([self.channelDelegate respondsToSelector:@selector(showNewMsg:)]){
                        [self.channelDelegate showNewMsg:self];
                    }
                }
                
                if (msgArray.count <20){
                    dispatch_async([self chatCommandRequestQueue], ^{
                        //加入耗时操
//                        [self gettingServiceChannelOldMsg];
                    });
                    
                }
//
       
        }else{
            NSArray *msgArray = [self gettingMsgItemWithDB];
            NSMutableArray *msgFrameArr = [NSMutableArray array];
            if (msgArray.count == 0 || msgArray == nil){
                //DB中没有数据
                [self gettingServiceChannelNewMsg];
                if (self.channelDelegate != nil){
                    if  ([self.channelDelegate respondsToSelector:@selector(showNewMsg:)]){
                        [self.channelDelegate showNewMsg:self];
                    }
                }
            }else{
                for (NSMsgItem *msg in msgArray) {
                    //                        DVLog(@"%@",[msg printAllPropertys]);
                    
                    [msg msgFormatFromDBwithAdd:self.channelID];
                    
                    
                    ChatCellFrame *cellFrame = [self createNewCellFrame:msg];
                    [msgFrameArr addObject:cellFrame];
                }
                self.msgList = msgFrameArr;
                [self timeLabelShowStatusFormat];
                if (self.channelDelegate != nil){
                    if  ([self.channelDelegate respondsToSelector:@selector(showNewMsg:)]){
                        [self.channelDelegate showNewMsg:self];
                    }
                }
                
                if (msgArray.count <20){
                    dispatch_async([self chatCommandRequestQueue], ^{
                        //加入耗时操
                        [self gettingServiceChannelOldMsg];
                    });
                    
                }
            }

        }
        
        
    }else{
        if (self.msgList.count== 0) {
            [self gettingServiceChannelNewMsg];
        }else{
//            ResponseMsgType msgTpye =ResponseMsgTypeInitHistoryMSG;
            
//            [self notificationChatOrMail:msgTpye withAddChatCount:self.msgList.count withAddMailCount:self.msgList.count];
        }
    }
}
-(BOOL)settingMsgListWithLastMsgFromDB{
    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            CSMessage *msg = nil;
            msg = [self gettingLastMessageFromDB];
            if  (msg != nil){
                 CSMessageModel *msgModel =[[CSMessageModel alloc]initWithMessage:msg];
                [msgModel messageModelShowingFormat];
                [self.msgList removeAllObjects];
                [self.msgList addObject:msgModel];
                return YES;
            }else{
                return NO;
            }
        }else{
            NSMsgItem *lastMsg =  nil;
            lastMsg =[self gettingLastMsg];
            if (lastMsg != nil) {
                [lastMsg msgFormatFromDBwithAdd:_channelID];
                ChatCellFrame *cellFrame = [self createNewCellFrame:lastMsg];
                [self.msgList removeAllObjects];
                [self.msgList addObject:cellFrame];
                return YES;
                
            }else{
                return NO;
            }
        }

        
    }else{
        return NO;
    }
}
-(BOOL)gettingOldMsg{
    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New
//            && (self.channelType== IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE)
            ){
            NSArray *msgArray =[self gettingOldMessageFromDBWithMsgCount:kMsgPulledCount];
            if (msgArray.count > 0){
                NSMutableArray *tempArray =[self messageFormatToMessageModel:msgArray];
                [tempArray addObjectsFromArray:self.msgList];
                self.msgList = tempArray;
                self.msgList = [NSArray sortingDataForMessageWithMsgModelArray:self.msgList andWithChannelType:self.channelType];
                [self timeLabelShowStatusFormat];
                
                if  (self.channelDelegate != nil){
                    if ([self.channelDelegate respondsToSelector:@selector(showOldMsg:)]){
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [self.channelDelegate showOldMsg: self];
                        });
                    }
                }
                if(msgArray.count<kMsgPulledCount){
//                    [self gettingServiceChannelOldMsg];
                }

            }
            return NO;
        }
        else{
            long   checkSequnceID =[[self.msgList objectAtIndex:0] chatMessage].sequenceId;
            BOOL isContinuous =  [self  chackSequenceIDContinuouslyWithSequenceID:checkSequnceID];
            NSArray *msgArray =nil;
            if (isContinuous ){
                 msgArray =[self gettingOldMsgItemWithDB];
            }else{
                if ((checkSequnceID-kMsgPulledCount) < self.serverMinSeqId){
                    msgArray =[self gettingOldMsgItemWithDB];
                }else{
                    long  maxSeq = [[self.msgList objectAtIndex:0] chatMessage].sequenceId-1;
                    long   minSeq= maxSeq - kMsgPulledCount-1;
                    BOOL commandStart = [self gettingServerOldMsgForCountryWithMinSeq:minSeq andWithMaxSeq:maxSeq];
                    
                    if (commandStart){
                        return NO;
                    }else{
                        return YES;
                    }

                }
               
            }
           
            
            if(msgArray.count > 0){
            
                dispatch_async([self chatMessageQueue], ^{
                    NSMutableArray *msgFrameArr = [NSMutableArray array];
                    for (NSMsgItem *msg  in msgArray) {
                        //                    [msg initAttribute];
                        [msg msgFormatFromDBwithAdd:self.channelID];
                        ChatCellFrame *cellFrame = [self createNewCellFrame:msg];
                        [msgFrameArr addObject:cellFrame];
                        
                    }
                    [msgFrameArr addObjectsFromArray:self.msgList];
                    self.msgList = msgFrameArr;
                    self.msgList = [self sortingDataForMsgItemWithCreatTimeSource:self.msgList];
                    [self timeLabelShowStatusFormat];
                    
                    if  (self.channelDelegate != nil){
                        if ([self.channelDelegate respondsToSelector:@selector(showOldMsg:)]){
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [self.channelDelegate showOldMsg: self];
                            });
                        }
                    }
                    if(msgArray.count<kMsgPulledCount){
                        //                    [self gettingServiceChannelOldMsg];
                    }
                    
                });
                return NO;
                
            }else{
                
                long   maxSeq = [[self.msgList objectAtIndex:0] chatMessage].sequenceId-1;
                long   minSeq= maxSeq - kMsgPulledCount-1;
                if (minSeq<self.serverMinSeqId){
                    //服务器没有更早的数据了，结束刷新
                    return YES;
                }else{
                    BOOL commandStart = [self gettingServerOldMsgForCountryWithMinSeq:minSeq andWithMaxSeq:maxSeq];
                    
                    if (commandStart){
                        return NO;
                    }else{
                        return YES;
                    }

                }
                
            }
        }
        
       
    }else{
//        dispatch_async([self chatCommandRequestQueue], ^{
//            [self gettingServiceChannelOldMsg];
//        });
        long   maxSeq = [[self.msgList objectAtIndex:0] chatMessage].sequenceId-1;
        long   minSeq= maxSeq - kMsgPulledCount-1;
        BOOL commandStart = [self gettingServerOldMsgForCountryWithMinSeq:minSeq andWithMaxSeq:maxSeq];
        
        if (commandStart){
            return NO;
        }else{
            return YES;
        }

        return NO;
    }
}


#pragma mark -
#pragma mark tool function



-(void)timeLabelShowStatusFormat{
    [self whetherShowTime:self.msgList];
}
-(void)msgArrayFormatToStartState{
    NSMutableArray *tempArr  = [NSMutableArray arrayWithArray:self.msgList];
    long  msgCount = tempArr.count;
    if (msgCount-kMsgPulledCount > 0) {
        NSRange range  = NSMakeRange(0, msgCount -kMsgPulledCount);
        [tempArr removeObjectsInRange:range];
        self.msgList = tempArr;
    }
}

-(BOOL)refreshingMsgListAtLastStatus{
    NSArray *tempArray=[self gettingNewMsgAfterMsg:[[self.msgList lastObject] chatMessage]];
    if (tempArray.count > 0){
        NSMutableArray *tempMsgArray =[NSMutableArray array ];
        // something
        for (NSMsgItem *msgItem in tempArray) {
            
            [msgItem msgFormatFromDBwithAdd:self.channelID];
            ChatCellFrame *cellFrame = [self oneMsgFun:msgItem];;
            [tempMsgArray addObject:cellFrame];
        }
        [self.msgList addObjectsFromArray:tempMsgArray];
        [self updatingLatestTimeWithMsg];
        return YES;
    }else{
        return NO;
    }
}

-(void)msgRepeatCheckWithMsgItem:(NSMsgItem *)vMsgItem{
    ChatCellFrame *tempCellFrame =nil;
    for (ChatCellFrame *frame  in self.msgList) {
        if (self.channelType == IOS_CHANNEL_TYPE_USER) {
            if (frame.chatMessage.isSelfMsg){
                if (frame.chatMessage.sendLocalTime == vMsgItem.sendLocalTime){
                    tempCellFrame = frame;
                }
            }else{
                if (frame.chatMessage.mailId == vMsgItem.mailId) {
                    //插入的新消息，在msgList中已经存在
                    tempCellFrame = frame;
                    break;
                }
            }
           
        }else if (self.channelType == IOS_CHANNEL_TYPE_CHATROOM ||
                  self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
                  self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
           
                
                if (frame.chatMessage.isSelfMsg){
                    if (frame.chatMessage.sendLocalTime == vMsgItem.sendLocalTime){
                        tempCellFrame = frame;
                    }
                }else{
                    if (frame.chatMessage.sequenceId == vMsgItem.sequenceId) {
                        //插入的新消息，在msgList中已经存在
                        tempCellFrame = frame;
                        break;
                    }
                }
           }
    }
    
    if  (tempCellFrame ){
        [self.msgList removeObject:tempCellFrame];
    }
 
}
-(NSMutableArray *)msgRepeatCheckWithMessageModelArray:(NSMutableArray *)vModelArr{
    NSMutableArray *mutArray = [NSMutableArray array ];
    for (CSMessageModel *currentModel in vModelArr) {
        CSMessageModel *repeatModel = nil;
        for (CSMessageModel *tempModel  in self.msgList) {
            if  (currentModel.isSelfSender ){
                if (currentModel.messageBodyType == CSMessageBodyType_Text_Normal ||
                    currentModel.messageBodyType == CSMessageBodyType_Text_Loudspeaker ||
                    currentModel.messageBodyType == CSMessageBodyType_Text_ModMsg  ){
                    if (currentModel.message.sendLocalTime == 0){
                        if  ([tempModel.message.msg isEqualToString:currentModel.message.msg]){
                            repeatModel = tempModel;
                            break;
                        }
                    }else{
                        if (currentModel.message.sendLocalTime == tempModel.message.sendLocalTime){
                            repeatModel = tempModel;
                            break;
                        }
                    }
                }else{
//                    if ([tempModel.message.msg isEqualToString:currentModel.message.msg] && tempModel.messageBodyType == currentModel.messageBodyType) {
//                        repeatModel = tempModel;
//                        break;
//                    }
                    if (self.channelType == IOS_CHANNEL_TYPE_CHATROOM ||
                        self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
                        self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                        if (tempModel.sequenceId == currentModel.sequenceId) {
                            repeatModel = tempModel;
                            break;
                        }
                        
                    }else{
                        if ([tempModel.messageId isEqualToString:currentModel.messageId]){
                            repeatModel = tempModel;
                            break;
                        }
                    }
                }
            }else{
                if (self.channelType == IOS_CHANNEL_TYPE_CHATROOM ||
                    self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
                    self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                    if (tempModel.sequenceId == currentModel.sequenceId) {
                        repeatModel = tempModel;
                        break;
                    }

                }else{
                    if ([tempModel.messageId isEqualToString:currentModel.messageId]){
                        repeatModel = tempModel;
                        break;
                    }
                }
           }
        }
        if (repeatModel ){
            repeatModel.message = currentModel.message;
            [mutArray addObject:currentModel];
        }
        
    }
    
    [vModelArr removeObjectsInArray:mutArray];
    return vModelArr;
 
}
#pragma mark -
#pragma mark  消息推送接口
-(void)chatNewMsgPushed:(NSArray *)vArray{
     dispatch_async(self.chatMessageQueue , ^{
        if  (vArray.count> 0){
            NSMutableArray *tempMsgArray =[NSMutableArray array ];
            
            if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                tempMsgArray = [self messageFormatToMessageModel:vArray];
              tempMsgArray =   [self msgRepeatCheckWithMessageModelArray:tempMsgArray];
            }else{
                // something
                for (NSMsgItem *msgItem in vArray) {
                    
                    [msgItem msgFormatFromDBwithAdd:self.channelID];
                    
                    if  (!msgItem.isSelfMsg && (self.channelType == IOS_CHANNEL_TYPE_USER ||self.channelType == IOS_CHANNEL_TYPE_CHATROOM)){
                        //不是自己的消息，并且是聊天室或者单人邮件消息
                        if  (msgItem.createTime > self.latestTime){
                            if  (msgItem.createTime> self.latestModifyTime){
                                
                                if  (self.channelType == IOS_CHANNEL_TYPE_USER){
                                    if (msgItem.readState == 0) {
                                        self.unreadCount = self.unreadCount + 1;
                                    }
                                }else{
                                    self.unreadCount = self.unreadCount + 1;
                                }
                            }
                        }
                        
                        
                    }
                    [self msgRepeatCheckWithMsgItem:msgItem];
                    ChatCellFrame *cellFrame = [self oneMsgFun:msgItem];;
                    [tempMsgArray addObject:cellFrame];
                }

            }
            
            [self.msgList addObjectsFromArray:tempMsgArray];
            
            
             [self updatingLatestTimeWithMsg];
            dispatch_async(dispatch_get_main_queue(), ^{
               
                if (self.channelDelegate  && [self.channelDelegate isKindOfClass:[UIViewController class]]) {
                    if  ([self.channelDelegate respondsToSelector:@selector(showNewMsg:)]){
                        [self.channelDelegate showNewMsg:self];
                    }
                }
                
            });

        }
    
     });
}
///** 邮件启动消息 */
//-(void)chatMsgOpenAppPushArray:(NSArray *)vArray{
//    dispatch_async(self.chatMessageQueue , ^{
//        if  (vArray.count> 0){
//            NSMutableArray *tempMsgArray =[NSMutableArray array ];
//            
//            
//            tempMsgArray =  [self sortingDataSource:vArray];
//            for (NSMsgItem *tempMsg in tempMsgArray) {
//                [tempMsg msgFormatFromDBwithAdd:self.channelID];
//                if  (!tempMsg.isSelfMsg ){
//                    if(tempMsg.createTime > self.latestTime){
//                        if  (tempMsg.createTime > self.latestModifyTime){
//                            if  (tempMsg.channelType == IOS_CHANNEL_TYPE_USER){
//                                if (tempMsg.readState == 0) {
//                                    self.unreadCount = self.unreadCount + 1;
//                                }
//                            }else{
//                                self.unreadCount = self.unreadCount + 1;
//                            }
//                        }
//
//                    }
//                }
//                
// 
//            }
////            [self channelUnreadCountWithChatMsg];
//            self.latestTime  =[self gettingLastMsg].createTime;
//            [self channelUpDateWithSelfChanged];
//        }
//        
//    });
//
//}


/** 缓存消息补取  */
-(void)chatmsgpushWithPullDownAfterScreenlockedMsgArray:(NSArray *)vArray{
    if (vArray.count > 0){
        dispatch_async([self chatMessageQueue], ^{
             NSMutableArray *msgArray = nil;
            
            if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                msgArray = [self messageFormatToMessageModel:vArray];
                msgArray =   [self msgRepeatCheckWithMessageModelArray:msgArray];
                [self.msgList addObjectsFromArray:msgArray];
            }else{
                msgArray = [NSMutableArray arrayWithArray:[self sortingDataSource:(NSMutableArray *)vArray]];
                NSMutableArray *repeatingArray = [NSMutableArray array ];
                for (int xx = 0; xx<msgArray.count; xx++) {
                    NSMsgItem *tempMsgItem = [msgArray objectAtIndex:xx];
                    for (ChatCellFrame *tempFrame in self.msgList) {
                        if  (tempMsgItem.createTime == tempFrame.chatMessage.createTime){
                            [repeatingArray addObject:tempMsgItem];
                        }
                    }
                }
                for (NSMsgItem *msg in repeatingArray) {
                    [msgArray removeObject:msg ];
                }
                //数组复用，移除原来的数据
                [repeatingArray removeAllObjects];;
                if (msgArray.count> 0) {
                    for (NSMsgItem *msgItem in msgArray) {
                        [msgItem msgFormatFromDBwithAdd:self.channelID];
                        //计算未读数
                        if  (!msgItem.isSelfMsg && (self.channelType == IOS_CHANNEL_TYPE_USER ||self.channelType == IOS_CHANNEL_TYPE_CHATROOM)){
                            //不是自己的消息，并且是聊天室或者单人邮件消息
                            if  (msgItem.createTime> self.latestModifyTime){
                                if  (msgItem.channelType == IOS_CHANNEL_TYPE_USER){
                                    if (msgItem.readState == 0) {
                                        self.unreadCount = self.unreadCount + 1;
                                    }
                                }else{
                                    self.unreadCount = self.unreadCount + 1;
                                }
                            }
                            
                        }
                        
                        ChatCellFrame *cellFrame = [self oneMsgFun:msgItem];;
                        [repeatingArray addObject:cellFrame];
                    }
                    [self.msgList addObjectsFromArray:repeatingArray];
            }
           
                [self updatingLatestTimeWithMsg];
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (self.channelDelegate && [self.channelDelegate isKindOfClass:[UIViewController class]]){
                        if  ([self.channelDelegate respondsToSelector:@selector(showscreenLockedMsg:)]){
                            [self.channelDelegate showscreenLockedMsg:self];
                        }
                    }

                    
                });
            }
        });
 
    }
}
/** 下拉刷新 */
-(void)chatMsgPushWithGetOldMsgArray:(NSArray *)vArray{
    if (vArray.count > 0){
 
        dispatch_async([self chatMessageQueue], ^{
            NSMutableArray *msgArray =[NSMutableArray arrayWithArray:[self sortingDataSource:(NSMutableArray *)vArray]];
            
            NSMutableArray *repeatingArray = [NSMutableArray array ];
            for (int xx = 0; xx<msgArray.count; xx++) {
                NSMsgItem *tempMsgItem = [msgArray objectAtIndex:xx];
                for (ChatCellFrame *tempFrame in self.msgList) {
                    if  (tempMsgItem.createTime == tempFrame.chatMessage.createTime){
                        [repeatingArray addObject:tempMsgItem];
                    }
                }
            }
            for (NSMsgItem *msg in repeatingArray) {
                [msgArray removeObject:msg ];
            }
            //数组复用，移除原来的数据
            [repeatingArray removeAllObjects];;
            if (msgArray.count> 0) {
                for (NSMsgItem *msgItem in msgArray) {
                    [msgItem msgFormatFromDBwithAdd:self.channelID];
                    ChatCellFrame *cellFrame = [self oneMsgFun:msgItem];;
                    [repeatingArray addObject:cellFrame];
                }
                [repeatingArray addObjectsFromArray:self.msgList];
                self.msgList = repeatingArray;
                [self updatingLatestTimeWithMsg];
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (self.channelDelegate && [self.channelDelegate isKindOfClass:[UIViewController class]]){
                        if  ([self.channelDelegate respondsToSelector:@selector(showOldMsg:)]){
                            [self.channelDelegate showOldMsg:self];
                        }
                    }
                });
            }
        });
        
    }
}
-(BOOL)checkMsgIsEqualedMsgWithFirstMsg:(NSMsgItem *)vFirstMsg andWithSecondMsg:(NSMsgItem *)vSecondMsg{
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        if  ([vFirstMsg.mailId isEqualToString:vSecondMsg.mailId]){
            return YES;
        }else{
            return NO;
        }
    }else if(self.channelType == IOS_CHANNEL_TYPE_ALLIANCE ||
             self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
             self.channelType == IOS_CHANNEL_TYPE_CHATROOM){
        if  (vFirstMsg.sequenceId == vSecondMsg.sequenceId){
            return YES;
        }else{
            return NO;
        }
    }else{
        return NO;
    }

}

-(void)updatingLatestTimeWithMsg{
    if(ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        //对msgList排序
 
        self.msgList =   [NSArray sortingDataForMessageWithMsgModelArray:self.msgList andWithChannelType:self.channelType];
        if  (self.msgList.count > 0){
            CSMessageModel *lastMsgModel =  [self.msgList lastObject] ;
 
            
            if (self.latestModifyTime < lastMsgModel.message.createTime  ) {
                if (self.channelType == IOS_CHANNEL_TYPE_USER || self.channelType == IOS_CHANNEL_TYPE_CHATROOM) {
                    if  (self.unreadCount> 0){
                        NSString *channelIdString = self.channelID;
                        if(ChatServiceCocos2dx::Mail_OC_Native_Enable){
                            //新版邮件发送通知让channel列表刷新
                            dispatch_async(dispatch_get_main_queue(), ^{
                                ChatServiceCocos2dx::updateUnreadCountOnMastermind();
                                [[NSNotificationCenter defaultCenter]postNotificationName:kChatChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channelIdString}];
                            });
                        }
                        
                    }
                }
            }
            self.contentsString = lastMsgModel.showContentsString;
            self.latestTime = lastMsgModel.message.createTime ;
      
            [self channelUpDateWithSelfChanged];
            
            [self whetherShowTime:self.msgList];
            //如果是国家或者联盟消息刷新外部消息条
            ChatServiceCocos2dx::updateUnreadCountOnMastermind();
            if (self.channelType ==IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                dispatch_async(dispatch_get_main_queue(), ^{
                    [[ChatServiceController chatServiceControllerSharedManager].gameHost refreshChatInfoWithCSMessage:lastMsgModel];
                });
            }
            
        }

    }else{
        //对msgList排序
        self.msgList =   [NSArray chatMsgSortingDataSource:self.msgList andWithChannelType:self.channelType];
        if  (self.msgList.count > 0){
            NSMsgItem *lastMsg = [(ChatCellFrame *)[self.msgList lastObject] chatMessage] ;
            //        NSMsgItem *dbLastMsg  = [self gettingLastMsg];
            //        if (dbLastMsg){
            //            if  (![self checkMsgIsEqualedMsgWithFirstMsg:lastMsg andWithSecondMsg:dbLastMsg]){
            //                //消息不同时
            //                NSArray *lastMsgArray =[self gettingNewMsgAfterMsg:lastMsg];
            //                NSMutableArray *tempArray =[NSMutableArray array];
            //            }
            //        }
            
            if (self.latestModifyTime < lastMsg.createTime  ) {
                if (self.channelType == IOS_CHANNEL_TYPE_USER || self.channelType == IOS_CHANNEL_TYPE_CHATROOM) {
                    if  (self.unreadCount> 0){
                        NSString *channelIdString = self.channelID;
                        if(ChatServiceCocos2dx::Mail_OC_Native_Enable){
                            //新版邮件发送通知让channel列表刷新
                            dispatch_async(dispatch_get_main_queue(), ^{
                                ChatServiceCocos2dx::updateUnreadCountOnMastermind();
                                [[NSNotificationCenter defaultCenter]postNotificationName:kChatChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channelIdString}];
                            });
                        }
                        
                    }
                }
            }
            self.contentsString = lastMsg.showMsg;
            self.latestTime = lastMsg.createTime ;
 
            [self channelUpDateWithSelfChanged];
            
            [self whetherShowTime:self.msgList];
            //如果是国家或者联盟消息刷新外部消息条
            ChatServiceCocos2dx::updateUnreadCountOnMastermind();
            if (self.channelType ==IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
                dispatch_async(dispatch_get_main_queue(), ^{
                    [[ChatServiceController chatServiceControllerSharedManager].gameHost refreshChatInfo:lastMsg];
                });
            }
            
        }

    }
    
    
 
}



///**最新消息处理*/
-(ChatCellFrame *) oneMsgFun:(NSMsgItem  *)vMsgItem
{
    
    if (vMsgItem.isSelfMsg) {
        if (vMsgItem.post ==0 || vMsgItem.post == 6) {
            NSMsgItem * cm_remove = nil;
            NSMutableArray *tempChatList = [MsgMessage msgMessageShared].chatMsgList;
            /**flg 用于判断是否是普通消息*/
            BOOL flg = TRUE;
//            if (tempChatList.count > 0){
//                for(NSMsgItem * mi in tempChatList){
//                    DLog(@"%ld",mi.sendLocalTime);
//                    DLog(@"%ld",vMsgItem.sendLocalTime);
//                    /**
//                     *  变动 原 邮件的发送没有发送时间 由于先上屏的缘故采用字符串对比。现 发送mail增加了发送时间 现在采用时间戳对比
//                     */
//                    
//                    if (mi.sendLocalTime  == vMsgItem.sendLocalTime) {
//                        [self updateNSMsgItem:mi tempMi:vMsgItem];
//                        cm_remove = vMsgItem;
//                        flg = FALSE;
//                    }
//                }
//            }

            
            if (flg == false) {
//                 [tempChatList removeObject:cm_remove];
                
            }
//            self.serverMaxSeqId = vMsgItem.sequenceId;

             return [self createNewCellFrame:vMsgItem];
            //如果是自己的信息并且是系统消息
        }else{
//             self.serverMaxSeqId = vMsgItem.sequenceId;
             return [self createNewCellFrame:vMsgItem];
        }
    }else{
//         self.serverMaxSeqId = vMsgItem.sequenceId;
        return [self createNewCellFrame:vMsgItem];
    }
    
   
}


-(ChatCellFrame*) createNewCellFrame:(NSMsgItem*) mi
{
    
//    [self updateSendStateToSuccess:mi];
//    mi.sendState = 2;
    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:mi];
    
    return cellFrame;
}

-(void) updateNSMsgItem:(NSMsgItem*) mi tempMi:(NSMsgItem*) tempMi
{
    mi.vip = tempMi.vip;
    mi.createTime = tempMi.createTime;
    mi.uid =  tempMi.uid;
    mi.name = tempMi.name;
    mi.asn = tempMi.asn;
    mi.msg = tempMi.msg;
    mi.translateMsg = tempMi.translateMsg;
    mi.headPic = tempMi.headPic;
    
    mi.originalLang = tempMi.originalLang;
    
    mi.isNewMsg = tempMi.isNewMsg;
    mi.isSelfMsg = tempMi.isSelfMsg;
    mi.channelType = tempMi.channelType;
    mi.gmod = tempMi.gmod;
    mi.post = tempMi.post;
    mi.headPicVer = tempMi.headPicVer;
    mi.sequenceId = tempMi.sequenceId;
    mi.lastUpdateTime = tempMi.lastUpdateTime;
    mi.createTime = tempMi.createTime;
    mi.sendLocalTime = tempMi.sendLocalTime;
    
    [mi packedMsg];
    
//    [self updateSendStateToSuccess:mi];
}

-(void) updateSendStateToSuccess:(NSMsgItem*) mi
{
    mi.sendState = 2;
}


-(int) gettingChatChannelMaxSeqId
{
    //    ChatCellFrame * ccf = [self.msgList firstObject];
    //    NSMsgItem *mi = ccf.chatMessage;
    
    if (self.channelType != IOS_CHANNEL_TYPE_USER) {
        NSMsgItem *msg = [self gettingSmallerCreatTimeForMsgWithDB];
        return msg.sequenceId - 1;
    }else{
        return  0;
    }
    
}

-(void) gettingServiceChannelNewMsg
{
 
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingServiceChannelNewMsg:self];
 
}

-(BOOL) gettingServiceChannelOldMsg
{
 
       return [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingServiceChannelOldMsg:self];
        
 
}

-(int)gettingChatChannelMinSeqId
{
    int maxSeq = [self gettingChatChannelMaxSeqId];
    int minSeq;
    if (maxSeq > 19) {
        minSeq = maxSeq - 19;
    }else if(maxSeq <= 19 && maxSeq >0){
        minSeq = 1;
    }else{
        return 0;
    }
    
    return minSeq;
}

-(void)initMsgUserWithUserUid:(NSString *)vUid andBlock:(MsgUserBlock)vBlock{
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:vUid];
    if(user == nil){
        user =[[ UserManager sharedUserManager] gettingUserInforWithUid:vUid];
        if (user == nil) {
             [[UserManager sharedUserManager] gettingServicerUser:vUid];
            if (vBlock != nil) {
                vBlock (nil);
            }
        }else{
            if (vBlock != nil) {
                vBlock(user);
            }
            
        }
    }else{
        if (vBlock != nil) {
            vBlock(user);
        }
    }
    
 
}

-(void)whetherShowTime:(NSArray *)vArray {
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        if  (vArray.count> 0){
            long oldCreatTime = 0;
            long interval = 5*60;//五分钟
            for (CSMessageModel *tempModel in vArray) {
                if (oldCreatTime == 0) {
                    tempModel.isTimeLabelShow = YES;
                    oldCreatTime = tempModel.message.createTime;
                }else{
//                    DVLog(@"上一个时间戳  %ld  ",oldCreatTime );
//                   
//                    DVLog(@"当前时间戳 %ld  \n 时间串%@",tempModel.message.createTime,tempModel.timeString);
          
                    long tempTimeInterVal = tempModel.message.createTime - oldCreatTime;
//                    DVLog(@"两者差值  %ld",tempTimeInterVal);
                    if  (tempTimeInterVal > interval){
                        tempModel.isTimeLabelShow = YES;
                    }else{
                        tempModel.isTimeLabelShow = NO;
                    }
                    oldCreatTime = tempModel.message.createTime;
                }
                
            }
        }
    }else{
        if (vArray.count > 0) {
            long  oldCreateTime = 0;
            //        NSDate *fistMsgDate = [[NSDate alloc]initWithTimeIntervalSince1970:createTime];
            NSDateFormatter *dateFarmatter = [NSDateFormatter custemDateFarmetterWithYearMonthDay];
            for (ChatCellFrame *cellFrame  in vArray) {
                if (oldCreateTime == 0) {
                    cellFrame.isShowUiTopView_BOOL = YES;
                    cellFrame.uiDownViewRect=CGRectMake(0,cellFrame.uiTopViewRect.size.height,kMainScreenWidth,cellFrame.cellHeight);
                    oldCreateTime = [[(ChatCellFrame *)[vArray objectAtIndex:0] chatMessage] createTime];
                    
                }else{
                    NSDate *previousMsgDate = [[NSDate alloc]initWithTimeIntervalSince1970:oldCreateTime];
                    NSString *oldTimeString = [dateFarmatter stringFromDate:previousMsgDate];
                    long currentMsgCreatTime  =cellFrame.chatMessage.createTime;
                    NSDate *currentMsgDate =[[NSDate alloc]initWithTimeIntervalSince1970:currentMsgCreatTime];
                    NSString *currentTimeString = [dateFarmatter stringFromDate:currentMsgDate];
                    oldCreateTime =currentMsgCreatTime;
                    //                 DLog(@"oldTimeSting %@",oldTimeString);
                    //                 DLog(@"currentTimeSting %@",currentTimeString);
                    if (![oldTimeString isEqualToString:currentTimeString]) {
                        
                        //时间戳字符串不相等，说明不是同一天的消息
                        cellFrame.isShowUiTopView_BOOL = YES;
                        cellFrame.uiDownViewRect=CGRectMake(0,cellFrame.uiTopViewRect.size.height,kMainScreenWidth,cellFrame.cellHeight);
                    }else{
                        cellFrame.isShowUiTopView_BOOL = NO;
                        cellFrame.uiDownViewRect=CGRectMake(0,0,kMainScreenWidth,cellFrame.cellHeight);
                    }
                    
                }
            }
        }

    }
}
#pragma mark -
#pragma mark 消息未读数计算
//-(void)channelUnreadCountWithChatMsg{
//    if  (self.channelType == IOS_CHANNEL_TYPE_USER || self.channelType == IOS_CHANNEL_TYPE_CHATROOM){
//        
//        //系统邮件
//        int count =0;
//        NSString *tableNameString =[[DBManager sharedDBManager] gettingChatTableNameWithChatChannel:self];
//        count = [[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:tableNameString withModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"ReadStatus = 0 and UserID != '%@'",[UserManager sharedUserManager].currentUser.uid]];
//        
//        
//        if( count> 0){
//            //     self.unreadCount = mailArray.count;
//            self.unreadCount =  count;
//        }else{
//            self.unreadCount = 0;
//        }
//        
//    }
//
//}
-(void)channelUnreadCountcalculatedWithSystemMail{
    if  (self.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL){
      
        NSString *systemMailString =[DBManager dbSqlWithChannelID:self.channelID];
        NSString *whereString =[NSString stringWithFormat:@"Status = 0 and (%@)",systemMailString];
        
        //系统邮件
        int count =[[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[MailInfoDataModel getTableName] withModelClass:[MailInfoDataModel class] where:whereString];
        
 
//               NSArray *mailArray  =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(Status = 0 and ChannelID = '%@')",self.channelID] orderBy:@"_id desc" offset:0 count:count];
                if( count> 0){
 
                    self.unreadCount =  count;
                }else{
                    self.unreadCount = 0;
                }
 
    }
}

-(void)channelMarkToReadStatus{
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        CSMessage  *lastMsg =nil;
        if(self.msgList> 0){
            lastMsg = [(CSMessageModel *)[self.msgList lastObject] message];
        }else{
            lastMsg = [self gettingLastMessageFromDB];
        }
        if (lastMsg){
            self.latestTime = lastMsg.createTime;
            self.latestModifyTime = lastMsg.createTime;
            self.unreadCount = 0;
        }
        [self channelUpDateWithSelfChanged];

    }else{
        NSMsgItem *lastMsg =nil;
        if(self.msgList> 0){
            lastMsg = [[self.msgList lastObject] chatMessage];
        }else{
            lastMsg = [self gettingLastMsg];
        }
        if (lastMsg){
            self.latestTime = lastMsg.createTime;
            self.latestModifyTime = lastMsg.createTime;
            self.unreadCount = 0;
        }
        [self channelUpDateWithSelfChanged];

    }
}

#pragma mark -
#pragma mark Msg DB管理
-(NSMsgItem *)gettingLastMsg{
//    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
//    LKLog(@"聊天表名:%@",nameString);
// 
//    NSMsgItem *msg = nil;
//    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
//        NSMsgItem *msg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class]   where:@"(_id > 0)" orderBy:@"CreateTime desc"];
//
//    }else if(self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
//             self.channelType == IOS_CHANNEL_TYPE_ALLIANCE ||
//             self.channelType == IOS_CHANNEL_TYPE_CHATROOM )
//    {
//        NSMsgItem *msg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class]   where:@"_id > 0" orderBy:@"SequenceID desc"];
// 
//    }
//    return msg;
//
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSArray *array ;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime desc " offset:0 count:1];
    }else if(self.channelType == IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE || self.channelType == IOS_CHANNEL_TYPE_CHATROOM )
    {
        array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"SequenceID desc " offset:0 count:1];
    }
    
    if (array.count> 0) {
        return (NSMsgItem *)[array lastObject];
    }else{
        return nil;
    }
}

-(NSMsgItem *)gettingSmallerCreatTimeForMsgWithDB {
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    
    NSMsgItem *msg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime asc "];
    return msg;
}

-(void)savingMsgForSelfSendToServerWithMsgItem:(NSMsgItem *)vMsg{
    if (vMsg.createTime != vMsg.sendLocalTime){
        vMsg.createTime = vMsg.sendLocalTime;
    }
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    NSMsgItem *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(LocalSendTime = %ld )",vMsg.sendLocalTime  ] orderBy:@"_id desc"];
    if (tempMsg == nil) {
        
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsg];
    }else{
        vMsg._id = tempMsg._id;
        [[DBManager sharedDBManager].dbHelper  updateToDBWithModelTableName:nameString
                                                                 andWithModel:vMsg
                                                                        where:[NSString stringWithFormat:@"(_id = %ld)",vMsg._id ]  ];
    }
}
-(void)savingMsgItemToDBWithMsgItem:(NSMsgItem *)vMsgItem{
    
    
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    
    
    if ([[UserManager sharedUserManager].currentUser.uid isEqualToString:vMsgItem.uid]){
 
        
        //自己的消息
        if (vMsgItem.sendLocalTime == 0) {
            NSMsgItem *tempMsg = nil;
            if (self.channelType != IOS_CHANNEL_TYPE_USER){
                
                tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"SequenceID = %ld",vMsgItem.sequenceId] orderBy:@"CreateTime desc"];
            }else{
                tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(MailID = '%@')", vMsgItem.mailId] orderBy:@"CreateTime desc"];
            }
//            NSMsgItem *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(MailID = '%@')", vMsgItem.mailId] orderBy:@"CreateTime desc"];
            if (tempMsg == nil) {
                
                [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsgItem];
            }else{
                vMsgItem._id = tempMsg._id;
                [[DBManager sharedDBManager].dbHelper    updateToDBWithModelTableName:nameString
                                                                              andWithModel:vMsgItem
                                                                                     where:[NSString stringWithFormat:@"(_id = %ld)",vMsgItem._id ]  ];
            }
        }else{
            NSMsgItem *tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(LocalSendTime = %ld )",vMsgItem.sendLocalTime  ] orderBy:@"_id desc"];
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
        
        NSMsgItem *tempMsg = nil;
        if (self.channelType != IOS_CHANNEL_TYPE_USER){
            
            tempMsg =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"SequenceID = %ld",vMsgItem.sequenceId] orderBy:@"CreateTime desc"];
        }else{
             tempMsg =[[DBManager sharedDBManager].dbHelper
                       searchSingleWithModelTableName:nameString
                                    andWithModelClass:[NSMsgItem class]
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

/** 根据attachmentID 查找红包消息*/
-(NSMsgItem *)redPackageSearchWithAttachmentId:(NSString *)vAttachMentID{
    if (vAttachMentID.length== 0){
        return nil;
    }
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    // ((Type ='12' AND ( AttachmentId LIKE '%d2a39ef9d7fe46bc9979e0616a94264e_1%')))
    NSString *attachMentIDString = @"%";
    attachMentIDString = [attachMentIDString stringByAppendingFormat:@"%@",vAttachMentID ];
    attachMentIDString = [attachMentIDString stringByAppendingString:@"%"];
    NSString *where = [NSString stringWithFormat:@"((Type ='12' AND ( AttachmentId LIKE '%@')))",attachMentIDString];
   
   NSMsgItem *tempMsg =  [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:where orderBy:@"_id desc"];
    if (tempMsg){
        return tempMsg;
    }else{
        return nil;
    }
}

-(void)redPacketMsgSaveToDBWithMsgItem:(NSMsgItem *)vMsgItem{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
   NSMsgItem *tempMsg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(SequenceID = %ld)",vMsgItem.sequenceId] orderBy:@"_id desc"];
    if (tempMsg){
        tempMsg.attachmentId = vMsgItem.attachmentId;
        [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:nameString andWithModel:tempMsg where:[NSString stringWithFormat:@"(_id =  %ld)",tempMsg._id]];
    }
}


-(void)msgUpdateWithMsgItem:(NSMsgItem *)vMsgItem andWithPropertyName:(NSArray *)vPropertyList {
    if (vPropertyList.count> 0){
       
        NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
        LKLog(@"聊天表名:%@",nameString);
        NSMsgItem *tempMsg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"(SequenceID = %ld)",vMsgItem.sequenceId] orderBy:@"_id desc"];
        if (tempMsg){
            for (NSString *vPropertyName in vPropertyList) {
                DVLog(@"要修改的属性名 ：%@",vPropertyName);
                [tempMsg setValue:[vMsgItem valueForKey: vPropertyName] forKey:vPropertyName];
            }
            DVLog(@"修改后的属性:%@",[tempMsg printAllPropertys]);
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:nameString andWithModel:tempMsg where:[NSString stringWithFormat:@"(_id =  %ld)",tempMsg._id]];
        }
    }
    
}
-(NSArray *)gettingMsgItemWithDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    
    NSArray *array;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
         array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime desc" offset:0 count:20];
    }else{
         array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime desc" offset:0 count:20];
    }
//   
//    for (NSMsgItem *tempItem in array) {
//        DLog(@"%d    seq: %d  msg:%@",tempItem.createTime,tempItem.sequenceId,tempItem.msg);
//        
//    }
    
    /** 保证sequenceId 一小到大的顺序 */
//    NSArray *msgArray = [[array reverseObjectEnumerator]allObjects];
    NSArray *msgArray =[self sortingDataSource:array];
//    DVLog(@"%@",msgArray);
    return msgArray;
}
-(NSArray *)gettingMsgItemWithDBWithCount:(int )vCount{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    
    NSArray *array;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime desc" offset:0 count:vCount];
    }else{
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"SequenceID desc" offset:0 count:vCount];
    }
    
    /** 保证sequenceId 一小到大的顺序 */
    //    NSArray *msgArray = [[array reverseObjectEnumerator]allObjects];
    NSArray *msgArray =[self sortingDataSource:array];
    //    DVLog(@"%@",msgArray);
    return msgArray;
}

-(NSArray *)gettingNewMsgAfterMsg:(NSMsgItem *)vMsgItem{
        NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
       LKLog(@"聊天表名:%@",nameString);
    NSArray *array = nil;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"CreateTime > %ld",vMsgItem.createTime] orderBy:@"CreateTime desc" offset:0 count:0];

        
    }else if(self.channelType == IOS_CHANNEL_TYPE_COUNTRY ||
             self.channelType == IOS_CHANNEL_TYPE_ALLIANCE ||
             self.channelType == IOS_CHANNEL_TYPE_CHATROOM )
    {
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"SequenceID > %ld",vMsgItem.sequenceId] orderBy:@"CreateTime desc" offset:0 count:0];

        
    }
         NSArray *msgArray =[self sortingDataSource:array];
    //    DVLog(@"%@",msgArray);
    return msgArray;
}
-(NSArray *)gettingOldMsgItemWithDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    LKLog(@"聊天表名:%@",nameString);
    NSInteger  offsetCount = self.msgList.count ;
    
    NSArray *array;
    /** 保证sequenceId 一小到大的顺序 */
//    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
//        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"CreateTime desc " offset:offsetCount count:10];
//    }else{
    if (offsetCount >0){
   
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"CreateTime < %ld",[(ChatCellFrame *)[self.msgList objectAtIndex:0] chatMessage].createTime] orderBy:@"CreateTime desc " offset:0 count:20];
    }
    
//    }
    
    NSArray *msgArray =[self sortingDataSource:array];
    //    DVLog(@"%@",msgArray);
    return msgArray;
}

/**
 *  从小到大排序
 *
 *  @param vArray 传入的array
 *
 *  @return 返回的array
 */
- (NSMutableArray *)sortingDataSource:(NSMutableArray *)vArray
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(NSMsgItem *obj1, NSMsgItem* obj2){
 
                          if(self.channelType == IOS_CHANNEL_TYPE_USER){
                              if(obj1.createTime < obj2.createTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                          }else{
                              if(obj1.sequenceId < obj2.sequenceId) {
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                          }             
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}
- (NSMutableArray *)sortingDataForMsgItemWithCreatTimeSource:(NSMutableArray *)vArray{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
   NSArray *conversations = vArray ;
 
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(ChatCellFrame *obj1, ChatCellFrame* obj2){
                       
 
                              if(obj1.chatMessage.createTime < obj2.chatMessage.createTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
 
                          
                     }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;

}
- (NSMutableArray *)sortingMailWithCreateTimeDataSource:(NSMutableArray *)vArray
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(MailInfoDataModel *obj1, MailInfoDataModel* obj2){
                          
                          
                              if(obj1.creatTime < obj2.creatTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                         
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}

#pragma mark -
#pragma mark DB 

-(void)channelUpDateWithSelfChanged{
    [[ChannelManager sharedChannelManager].channel_map setValue:self forKey:self.channelID];
    ChatChannel *tempChannel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"(ChannelID = '%@' and ChannelType = %d)",self.channelID,self.channelType] orderBy:@"_id asc"];
    if (tempChannel == nil) {
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[ChatChannel getTableName] andWithModel:self];
    }else{
        self._id = tempChannel._id;
 
        [[DBManager sharedDBManager].dbHelper    updateToDBWithModelTableName:[ChatChannel getTableName]
                                                                      andWithModel:self
                                                                             where:[NSString stringWithFormat:@"(_id = '%ld')",self._id]  ];
 
     
    }

} //在类 初始化的时候
+(void)initialize
{
    //remove unwant property
    //比如 getTableMapping 返回nil 的时候   会取全部属性  这时候 就可以 用这个方法  移除掉 不要的属性
//    [self removePropertyWithColumnName:@"error"];
    
    
    //simple set a column as "LKSQL_Mapping_UserCalculate"
    //根据 属性名  来启用自己计算
    //[self setUserCalculateForCN:@"error"];
    
    
    //根据 属性类型  来启用自己计算
    //[self setUserCalculateForPTN:@"NSDictionary"];
    
    //enable own calculations
    //[self setUserCalculateForCN:@"address"];
    
    //enable the column binding property name
//    [self setTableColumnName:@"MyAge" bindingPropertyName:@"age"];
//    [self setTableColumnName:@"MyDate" bindingPropertyName:@"date"];
}

+(void)dbDidAlterTable:(LKDBHelper *)helper tableName:(NSString *)tableName addColumns:(NSArray *)columns
{
//    for (int i=0; i<columns.count; i++)
//    {
//        LKDBProperty* p = [columns objectAtIndex:i];
//        if([p.propertyName isEqualToString:@"error"])
//        {
//            [helper executeDB:^(FMDatabase *db) {
//                NSString* sql = [NSString stringWithFormat:@"update %@ set error = name",tableName];
//                [db executeUpdate:sql];
//            }];
//        }
//    }
//    LKErrorLog(@"your know %@",columns);
}

+(void)columnAttributeWithProperty:(LKDBProperty *)property
{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        property.defaultValue = [NSString stringWithFormat:@"%d",[ChatChannel getTableVersion]];
    }
//    if([property.propertyName isEqualToString:@"date"])
//    {
//        property.isUnique = YES;
//        property.checkValue = @"MyDate > '2000-01-01 00:00:00'";
//        property.length = 30;
//    }
}
+(NSString *)getPrimaryKey
{
    return @"_id";
}

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"             :@"tableVer",
             @"_id"                  :@"_id"     ,
             @"ChannelType"          :@"channelType",
             @"ChannelID"            :@"channelID",
             @"MinSequenceID"        :@"dbMinSeqId",
             @"MaxSequenceID"        :@"dbMaxSeqId",
             @"ChatRoomMembers"      :@"memberUidString",
             @"ChatRoomOwner"        :@"roomOwner",
             @"IsMember"             :@"isMember",
             @"CustomName"           :@"customName",
             @"UnreadCount"          :@"unreadCount",
             @"LatestId"             :@"latestId",
             @"LatestTime"           :@"latestTime",
             @"LatestModifyTime"     :@"latestModifyTime",
             @"Settings"             :@"settings"
             };
}

+(int)getTableVersion
{
    return kDBVersion;
}
//表名
+(NSString *)getTableName
{
    return @"Channel";
}
//-(void)userSetValueForModel:(LKDBProperty *)property value:(id)value{
////    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
////    if([sqlColumnname isEqualToString:@"TableVer"])
////    {
////        
////    }
//    [super userSetValueForModel:property value:value ];
//}
 -(id)modelGetValue:(LKDBProperty *)property{
         NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
         if([sqlColumnname isEqualToString:@"TableVer"])
         {
             return [NSString stringWithFormat:@"%d",[ChatChannel getTableVersion]];
         }else{
              return  [super modelGetValue:property];
         }
    
 }
//-(id)userGetValueForModel:(LKDBProperty *)property
//{
//    return nil;
//}
#pragma mark -
#pragma mark 合并资源采集报告
-(MailData*) mergeResourceMailData
{
    ResourceMailData *data = (ResourceMailData*)[self gettingLastData];
    if (!data)
        return nil;
    if ([self.msgList count] > 1) {
        for(int i = 0 ; i < [self.msgList count] - 1 ; i++)
        {
            MailInfoDataModel *tempMode = (MailInfoDataModel*)[self.msgList objectAtIndex:i];

            ResourceMailData *resourceMailData = (ResourceMailData*)tempMode.maildata;
            ResourceMailContents *detail = (ResourceMailContents*)[resourceMailData.collect firstObject];

            [data.collect insertObject:detail atIndex:i];

        }
    }
    [data parseMode2Json];
    return data;
}

#pragma mark -
#pragma mark 合并攻打怪物报告
-(MailData*) mergeMonsterMailData
{
    MonsterMailData *data = (MonsterMailData*)[self gettingLastData];
    if (!data)
        return nil;
    if ([self.msgList count] > 1) {
        for(int i = 0 ; i < [self.msgList count] - 1 ; i++)
        {
            MailInfoDataModel *tempMode = (MailInfoDataModel*)[self.msgList objectAtIndex:i];
            
            MonsterMailData *monsterMailData = (MonsterMailData*)tempMode.maildata;
            MonsterMailContents *detail = (MonsterMailContents*)[monsterMailData.monster firstObject];
            
            [data.monster insertObject:detail atIndex:i];
        }
    }
    
    [data parseMode2Json];
    
    return data;
}

#pragma mark 合并黑骑士
-(MailData*) mergeKnightMailData
{
    BattleMailData *data = (BattleMailData*)[self gettingLastData];
    if (!data)
        return nil;
    if (data.knight){
        if ([self.msgList count] > 1) {
            for(int i = 0 ; i < [self.msgList count] - 1 ; i++)
            {
                MailInfoDataModel *tempMode = (MailInfoDataModel*)[self.msgList objectAtIndex:i];
                if (!tempMode) {
                    return nil;
                }
                BattleMailData *battleMailData = (BattleMailData*)tempMode.maildata;
                if (!battleMailData) {
                    return nil;
                }
                BattleMailContents *detail = (BattleMailContents*)[battleMailData.knight firstObject];
                if (!detail) {
                    return nil;
                }
                if (detail)
                [data.knight insertObject:detail atIndex:i];
                
            }
        }
        [data parseMode2Json];
        return data;
    }else{
        return nil;
    }
}

-(MailData*) gettingLastData
{
    MailInfoDataModel *mode = (MailInfoDataModel*)[self.msgList lastObject];
    if (mode)
        return mode.maildata;
    else
        return nil;
}

#ifdef DEBUG
+(void)dbDidInserted:(NSObject *)entity result:(BOOL)result
{
    if (result) {
        LKLog(@"did insert Flag: 【%d】\n完成插入的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did insert Flag: 【%d】\n插入失败的数据:%@",result,[entity printAllPropertys] );
    }
    
}


+(void)dbDidUpdated:(NSObject *)entity result:(BOOL)result{
    if (result) {
        LKLog(@"did Update Flag: 【%d】\n更新成功的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did Update Flag: 【%d】\n 更新失败的数据:%@",result,[entity printAllPropertys] );
    }
}

#endif


@end
 
