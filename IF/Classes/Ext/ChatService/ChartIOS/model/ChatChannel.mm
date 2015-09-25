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
@interface ChatChannel ()

@end

@implementation ChatChannel

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
        && ![self.channelID isEqualToString:[NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId]]);
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



-(void)setChannelID:(NSString *)channelID{
    _channelID = channelID;
    if  (ChatServiceCocos2dx::DB_MsgItem_switch){
        NSString *nameString =[[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
        
        [[DBManager sharedDBManager].dbHelper createTableWithModelClassTableName:nameString andModelClass:[NSMsgItem class]];
    }
    
 

}

-(void)gettingFirstMsg{
    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
        if(self.msgList.count == 0){
            NSArray *msgArray = [self gettingMsgItemWithDB];
            NSMutableArray *msgFrameArr = [NSMutableArray array];
            if (msgArray.count == 0 || msgArray == nil){
                //DB中没有数据
                [self gettingServiceChannelNewMsg];
            }else{
                for (NSMsgItem *msg in msgArray) {
                    ChatCellFrame *cellFrame = [self createNewCellFrame:msg];
                    [msgFrameArr addObject:cellFrame];
                }
                self.msgList = msgFrameArr;
                ResponseMsgType msgTpye =ResponseMsgTypeInitHistoryMSG;
                
                [self notificationChatOrMail:msgTpye withAddChatCount:msgFrameArr.count withAddMailCount:msgFrameArr.count];
            }

        }else{
            
            ResponseMsgType msgTpye =ResponseMsgTypeInitHistoryMSG;
            
            [self notificationChatOrMail:msgTpye withAddChatCount:self.msgList.count withAddMailCount:self.msgList.count];
        }
        
    }else{
        if (self.msgList.count== 0) {
            [self gettingServiceChannelNewMsg];
        }else{
            ResponseMsgType msgTpye =ResponseMsgTypeInitHistoryMSG;
            
            [self notificationChatOrMail:msgTpye withAddChatCount:self.msgList.count withAddMailCount:self.msgList.count];
        }
    }
}

-(BOOL)gettingOldMsg{
    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
        NSArray *msgArray =[self gettingOldMsgItemWithDB];
        if (msgArray.count == 0 || msgArray == nil) {
            return [self gettingServiceChannelOldMsg];
            
        }else{
            NSMutableArray *msgFrameArr = [NSMutableArray array];
            for (NSMsgItem *msg  in msgArray) {
                [msg packedMsg];
                [msg initUserForReceivedMsg];
                ChatCellFrame *cellFrame = [self createNewCellFrame:msg];
                [msgFrameArr addObject:cellFrame];
                [cellFrame isShowUiTopView:msgFrameArr];
            }
            [self.msgList insertObjects:msgFrameArr atIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, [msgFrameArr count])]];
            if (msgArray.count < 20) {
                return [self gettingServiceChannelOldMsg];
                
            }else{
                ResponseMsgType msgTpye =ResponseMsgTypeActionMSG;
                
                [self notificationChatOrMail:msgTpye withAddChatCount:msgArray.count withAddMailCount:msgFrameArr.count];
               
                 return NO;
            }
        }

    }else{
        return [self gettingServiceChannelOldMsg];
    }
}

-(NSMsgItem *)gettingLastMsg{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSArray *array ;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
         array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"createTime desc " offset:0 count:1];
    }else
    {
       array= [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"sequenceId desc " offset:0 count:1];
    }
   
    if (array.count> 0) {
        return (NSMsgItem *)[array lastObject];
    }else{
        return nil;
    }
}

-(void)chatMsgPushArray:(NSArray *)vArray andMsgType:(ResponseMsgType)msgTpye{
    /** 消息id从小到大排列 */
    NSArray *msgArray =[self sortingDataSource:vArray];
    NSMutableArray *tempMsgArray =[NSMutableArray array ];
    for (NSMsgItem *msgItem in msgArray) {
        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
             [self savingMsgItemToDBWithMsgItem:msgItem];
        }
        ChatCellFrame *cellFrame = [self createNewCellFrame:msgItem];
        [tempMsgArray addObject:cellFrame];
        [cellFrame isShowUiTopView:tempMsgArray];
    }
    
    if (msgTpye == ResponseMsgTypeInitHistoryMSG) {
        self.msgList = tempMsgArray;
    }else if(msgTpye == ResponseMsgTypeActionMSG){
        [self.msgList insertObjects:tempMsgArray atIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, [tempMsgArray count])]];
    }else{
        for (ChatCellFrame *cellFrame in tempMsgArray) {
            [self oneMsgFun:cellFrame];
        }
    }
    
    [self notificationChatOrMail:msgTpye withAddChatCount:tempMsgArray.count withAddMailCount:tempMsgArray.count];
}

-(void) notificationChatOrMail:(ResponseMsgType) msgTpye withAddChatCount:(NSInteger) chatCount withAddMailCount:(NSInteger) mailCount
{
    if (self.channelType == IOS_CHANNEL_TYPE_COUNTRY || self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        [[NSNotificationCenter defaultCenter]postNotificationName:kChatMsgPushFromServer object:nil userInfo:@{@"channelID":self.channelID,@"msgType":[NSNumber numberWithInt:msgTpye],@"msgCount":[NSNumber numberWithInt:chatCount] }];
    }else{
        
        [[NSNotificationCenter defaultCenter]postNotificationName:kChatMailMsgPushFromServer object:nil userInfo:@{@"channelID":self.channelID,@"msgType":[NSNumber numberWithInt:msgTpye],@"msgCount":[NSNumber numberWithInt:mailCount] }];
    }
}


///**最新消息处理*/
-(void) oneMsgFun:(ChatCellFrame *)vCellFrame
{
    
    if (vCellFrame.chatMessage.isSelfMsg) {
        if (vCellFrame.chatMessage.post ==0 || vCellFrame.chatMessage.post == 6) {
            NSMsgItem * cm_remove = nil;
            NSMutableArray *tempChatList = [MsgMessage msgMessageShared].chatMsgList;
            /**flg 用于判断是否是普通消息*/
            BOOL flg = TRUE;
            
            for(NSMsgItem * mi in tempChatList){
                NSLog(@"%@",mi.sendLocalTime);
                NSLog(@"%@",vCellFrame.chatMessage.sendLocalTime);
                /**
                 *  变动 原 邮件的发送没有发送时间 由于先上屏的缘故采用字符串对比。现 发送mail增加了发送时间 现在采用时间戳对比
                 */
                
                if (mi.sendLocalTime  == vCellFrame.chatMessage.sendLocalTime) {
                    [self updateNSMsgItem:mi tempMi:vCellFrame.chatMessage];
                    cm_remove = vCellFrame.chatMessage;
                    flg = FALSE;
                }
            }
            
            if (flg) {
                [self.msgList addObject:[self createNewCellFrame:vCellFrame.chatMessage]];
            }else{
                [tempChatList removeObject:cm_remove];
            }
            //如果是自己的信息并且是系统消息
        }else{
            [self.msgList addObject:[self createNewCellFrame:vCellFrame.chatMessage]];
        }
    }else{
        ChatCellFrame* ccf = [self createNewCellFrame:vCellFrame.chatMessage];
        [self.msgList addObject:ccf];
        [ccf isShowUiTopView:self.msgList];
    }
    
    self.serverMaxSeqId = vCellFrame.chatMessage.sequenceId;
}


-(ChatCellFrame*) createNewCellFrame:(NSMsgItem*) mi
{
    [self updateSendStateToSuccess:mi];
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
    
    [self updateSendStateToSuccess:mi];
}

-(void) updateSendStateToSuccess:(NSMsgItem*) mi
{
    mi.sendState = 2;
}

-(int) gettingChatChannelMaxSeqId
{
    ChatCellFrame * ccf = [self.msgList firstObject];
    NSMsgItem *mi = ccf.chatMessage;
    return mi.sequenceId - 1;
}

-(void) gettingServiceChannelNewMsg
{
    if (self.channelType == IOS_CHANNEL_TYPE_USER) {
    //单人邮件
    }else {
        [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingServiceChannelNewMsg:self];

    }
}

-(BOOL) gettingServiceChannelOldMsg
{
    if (self.channelType == IOS_CHANNEL_TYPE_USER) {
        //单人邮件
        return NO;
    }else {
       return [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingServiceChannelOldMsg:self];
        
    }
    
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

#pragma mark -
#pragma mark Msg DB管理
-(void)savingMsgItemToDBWithMsgItem:(NSMsgItem *)vMsgItem{
    
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSMsgItem *tempMsg;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        tempMsg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"mailId = '%@'",vMsgItem.mailId] orderBy:@"rowid asc"];
    }else{
       tempMsg = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:[NSString stringWithFormat:@"sequenceId = %d",vMsgItem.sequenceId] orderBy:@"rowid asc"];
    }
   
    if (tempMsg == nil) {
       
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:nameString andWithModel:vMsgItem];
    }else{
        if  (self.channelType == IOS_CHANNEL_TYPE_USER){
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:nameString andWithModel:vMsgItem where:[NSString stringWithFormat:@"mailId = '%@'",vMsgItem.mailId]];
        }else{
             [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:nameString andWithModel:vMsgItem where:[NSString stringWithFormat:@"sequenceId = %d",vMsgItem.sequenceId]];
        }
        
   
    }
}

-(NSArray *)gettingMsgItemWithDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    
    NSArray *array;
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
         array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"createTime desc" offset:0 count:20];
    }else{
         array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"sequenceId desc" offset:0 count:20];
    }
   
    /** 保证sequenceId 一小到大的顺序 */
//    NSArray *msgArray = [[array reverseObjectEnumerator]allObjects];
    NSArray *msgArray =[self sortingDataSource:array];
//    DVLog(@"%@",msgArray);
    return msgArray;
}

-(NSArray *)gettingOldMsgItemWithDB{
    NSString *nameString = [[DBManager sharedDBManager]gettingChatTableNameWithChatChannel:self];
    DVLog(@"聊天表名:%@",nameString);
    NSInteger  offsetCount = self.msgList.count ;
    NSArray *array;
    /** 保证sequenceId 一小到大的顺序 */
    if  (self.channelType == IOS_CHANNEL_TYPE_USER){
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"createTime desc " offset:offsetCount count:20];
    }else{
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:nameString andWithModelClass:[NSMsgItem class] where:nil orderBy:@"sequenceId desc " offset:offsetCount count:20];
    }
    
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
    NSArray *conversations = vArray;
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
/** 给某个字段赋特定的值 */
//-(id)modelGetValue:(LKDBProperty *)property{
// 
//    if ([property.propertyName isEqualToString:@"msgList"]) {
//        return  nil;
//    }else if ([property.propertyName isEqualToString:@"sendingMsgList"]) {
//        return  nil;
//    }else if ([property.propertyName isEqualToString:@"memberUidArray"]) {
//        return  nil;
//    }else{
//        return  [super modelGetValue:property];
//    }
//}
//-(void)modelSetValue:(LKDBProperty*)property value:(id)value{
//    if ([property.propertyName isEqualToString:@"msgList"]) {
//        
//        self.msgList = [NSMutableArray array ];
//    }else if ([property.propertyName isEqualToString:@"sendingMsgList"]) {
//       self.sendingMsgList = [NSMutableArray array ];
//    }else if ([property.propertyName isEqualToString:@"memberUidArray"]) {
//        self.sendingMsgList = [NSMutableArray array ];
//    }else{
//        [super modelSetValue:property value:value];
//    }
//    
//}

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"             :@"tableVer",
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
             @"Settings"            :@"settings"
             };
}

+(int)getTableVersion
{
    return 1;
}
//表名
+(NSString *)getTableName
{
    return @"Channel";
}


+(LKTableUpdateType)tableUpdateWithModelTableName:(NSString *)vTableName
                                 andForOldVersion:(int)oldVersion
                                       newVersion:(int)newVersion{
    if (newVersion != oldVersion) {
        
        //        [self tableUpdateAddColumeWithModelTableName:vTableName andWithPN:@"tempArray"];
    }
    
    return LKTableUpdateTypeCustom;
    
}
+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithPN:(NSString*)propertyName{
    
    [super tableUpdateAddColumeWithModelTableName:vTableName andWithPN:propertyName];
}




// 将要插入数据库
+(void)dbWillInsert:(NSObject *)entity
{
    LKLog(@"will insert : %@",NSStringFromClass(self));
    
}
//已经插入数据库
+(void)dbDidInserted:(NSObject *)entity result:(BOOL)result
{
    LKLog(@"did insert Flag: %d\n完成插入的数据:%@",result,[entity printAllPropertys] );
}

+(void)dbWillDelete:(NSObject *)entity{
    LKLog(@"will Delete : %@",NSStringFromClass(self));
    
}
+(void)dbDidDeleted:(NSObject *)entity result:(BOOL)result{
    
    LKLog(@"did Deleted Flag: %d\n完成删除的数据:%@",result,[entity printAllPropertys] );
}

+(void)dbWillUpdate:(NSObject *)entity{
    LKLog(@"will Update : %@",NSStringFromClass(self));
    
}
+(void)dbDidUpdated:(NSObject *)entity result:(BOOL)result{
    
    LKLog(@"did Updated Flag: %d\n完成更新的数据:%@",result,[entity printAllPropertys] );
}
+(void)dbDidSeleted:(NSObject *)entity{
    LKLog(@"did seleted %@",[entity printAllPropertys]);
}

@end
