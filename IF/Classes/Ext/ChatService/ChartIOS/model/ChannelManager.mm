//
//  ChannelManager.m
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import "ChannelManager.h"
#import "UserManager.h"
#import "ServiceInterface.h"
#import "ChatServiceCocos2dx.h"
#import "NSArray+SortHelper.h"
#import "NSString+Cocos2dHelper.h"
#import "MailDataManager.h"
#import "MonsterMailData.h"
#import "BattleMailData.h"
#import "MonsterMailContents.h"
#import "MailHeiqishiCellInfo.hpp"
#import "ResourceMailContents.h"
#import "CSCommandManager.h"
#import "CCJSONConverter.h"
#import "MailController.h"
#import "MailInfoManager.h"
#import "CSConvertViewManager.h"

#import "ChatChannel+ChatMessage.h"
#import "CSMessage.h"
@interface ChannelManager ()
@property (nonatomic,assign) int currentChannelViewIndex;
typedef enum{
    CHATCHANNELTYPE,
    CHATCHANNELID,
    CHATCHANNELSERVERMINSEQID,
    CHATCHANNELSERVEREMAXSEQID
}ChatChannelAttribute;
@end

@implementation ChannelManager


-(void)createChatChannel:(int)channelType withAddChannelID:(NSString *)channelID
{
    ChatChannel *channel = [self.channel_map objectForKey:channelID];
    if (channel == nil) {
        ChatChannel *cc = [[ChatChannel alloc]init];
        cc.channelID =channelID;
        cc.channelType = channelType;
        [self addChannelWithChannel:cc andWithChannelID:channelID];
    }
}

-(void)settingChatChannel:(ChatChannel *)vChatChannel{
    NSArray *keyArray = [self.channel_map allKeys];
    //检查channelMap中有没有当前channel
    if ([keyArray containsObject:vChatChannel.channelID]) {
        ChatChannel *tempChanel =[self.channel_map objectForKey:vChatChannel.channelID];
        tempChanel = [vChatChannel copy];
        [self addChannelWithChannel:tempChanel andWithChannelID:tempChanel.channelID];
    }else{
        [self addChannelWithChannel:vChatChannel andWithChannelID:vChatChannel.channelID];
    }
   
    
}
-(void)deleteChannelWithChannelArray:(NSArray *)vArray{
    for (int x = 0 ; x<vArray.count ;x++    ) {
        [self deletingChatChannel:[vArray objectAtIndex:x]];
    }
}
-(void)deletingChatChannel:(ChatChannel *)vChatChannel{
    if  (vChatChannel){
        NSArray *keyArray = [self.channel_map allKeys];
        if ([keyArray containsObject:vChatChannel.channelID]) {
            [self.channel_map removeObjectForKey:vChatChannel.channelID];
            /**
             *  如果频道是联盟 删除联盟对应的数据源数据
             */
            if (vChatChannel.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
                [vChatChannel.msgList removeAllObjects];
                [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.dataSourceArray removeAllObjects];
            }
            
            if (ChatServiceCocos2dx::DB_ChatChannel_switch){
                ChatChannel *tempChannel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"(ChannelID = '%@')",vChatChannel.channelID] orderBy:@"_id asc"];
                if (tempChannel  ) {
                    //channel删除
                   
                    [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[ChatChannel getTableName] where:[NSString stringWithFormat:@"_id = %ld",tempChannel._id]];
                }
            }

        }
        
        

    }
}

-(void) createChatChannelRoom:(NSString *)channelId withChannelMemberUids:(NSString *)memberUids
{
    if (![[self.channel_map allKeys] containsObject:channelId]) {
        ChatChannel *cc = [[ChatChannel alloc] init];
        cc.channelType = IOS_CHANNEL_TYPE_CHATROOM;
        cc.channelID = channelId;
        
        if (memberUids.length > 0) {
            NSArray *uidArr =[memberUids componentsSeparatedByString:NSLocalizedString(@"_", nil)];//拆分成数组
            for(NSString* uid in uidArr)
            {
                [cc.memberUidArray addObject:uid];
            }
        }
        [self addChannelWithChannel:cc andWithChannelID:channelId];
    }
}

-(void)addChannelWithChannel:(ChatChannel *)vChannel andWithChannelID:(NSString *)vChannelId{
    
    if (!vChannel) {
        return ;
    }
    if (vChannelId.length <= 0) {
        return ;
    }
    
    [self.channel_map  setObject:vChannel forKey:vChannelId];
    [vChannel setChannelID:vChannelId];
    if (ChatServiceCocos2dx::DB_ChatChannel_switch) {
        [self saveChannelToDB:vChannel];
    }
}

-(void)updatingChannelForDBWithChannel:(ChatChannel *)vChannel{
    [self saveChannelToDB:vChannel];
}

-(void)updatingChannelForMail{
    NSArray *channelArray =[self.channel_map allValues];
    for (ChatChannel *channel in channelArray) {
        if (channel.channelType == IOS_CHANNEL_TYPE_CHATROOM || channel.channelType == IOS_CHANNEL_TYPE_USER) {
            [self saveChannelToDB:channel];
        }
    }
}
-(void)updatingChannelForSysMail{
    NSArray *channelArray =[self.channel_map allValues];
    for (ChatChannel *channel in channelArray) {
        if (channel.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL  ) {
            [self saveChannelToDB:channel];
        }
    }
}
-(void) updateChatChannelInfo:(NSString *)newMsgInfo
{
    DVLog(@"newMsgInfo === %@",newMsgInfo);
    NSArray *chatChannelInfos = [newMsgInfo componentsSeparatedByString:@","];
    [self settingChatChannelAttribute:chatChannelInfos];
    
}

//获取频道属性 频道类型|频道ID|serverMinSeqId|serverMaxSeqId,道类型|频道ID|serverMinSeqId|serverMaxSeqId
-(void) settingChatChannelAttribute:(NSArray*)chatChannelInfos
{
    for(NSString *chatChannelInfo in chatChannelInfos)
    {
        NSArray *chatChannelAttributes = [chatChannelInfo componentsSeparatedByString:@"|"];
        DVLog(@"chatChannelAttributes === %@",chatChannelAttributes);
        
        NSString *chatChannelID = chatChannelAttributes[CHATCHANNELID];
        if ([chatChannelID isEqualToString:@"0"]) {
            chatChannelID = [NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId];
        }
        int chatChannelType = [chatChannelAttributes[CHATCHANNELTYPE] intValue];
        int serverMinSeqId = [chatChannelAttributes[CHATCHANNELSERVERMINSEQID] intValue];
        int serverMaxSeqId = [chatChannelAttributes[CHATCHANNELSERVEREMAXSEQID] intValue];
        
        ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:chatChannelID];
        cc.serverMinSeqId = serverMinSeqId;
        cc.serverMaxSeqId = serverMaxSeqId;
    }
}


-(BOOL)isContainFromUid:(NSString *)fromUid
{
    return [[self.channel_map allKeys] containsObject:fromUid];
}

-(ChatChannel *)gettingAllianceChannel
{
    NSString *allianceId = [UserManager sharedUserManager].currentUser.allianceId;
    if (allianceId.length> 0){
        ChatChannel *cc = [self.channel_map objectForKey:allianceId];
        if(cc == nil){
            [self createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:[UserManager sharedUserManager].currentUser.allianceId];
            cc = [self.channel_map objectForKey:allianceId];
        }
        
        return cc;
    }else{
        return nil;
    }

}

-(ChatChannel *)gettingCountryChannel
{
    ChatChannel *cc = [self.channel_map objectForKey:[NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId]];
    if(cc == nil){
         [self createChatChannel:IOS_CHANNEL_TYPE_COUNTRY withAddChannelID:[NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId]];
        cc = [self.channel_map objectForKey:[NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId]];
    }
    return cc;
}

-(ChatChannel *)gettingChannelInfo:(NSString *)chatChannelId
{
    if  (chatChannelId.length> 0){
        ChatChannel *cc = [self.channel_map objectForKey:chatChannelId];
        return cc;
    }else{
        return nil;
    }

}

-(void) updateChatChannelRoom:(int) channelType withAddChannelID:(NSString *) channelID withAddUidArr:(NSString *) memberUids withAddRoomOwner:(NSString *) roomOwner withAddCustomName:(NSString *) customName
{
    
    ChatChannel *cc = [self gettingChannelInfo:channelID];
    
    if (!cc) {
        return ;
    }
    if (roomOwner.length > 0) {
        cc.roomOwner = roomOwner;
    }
    if (customName.length > 0) {
        cc.customName = customName;
    }
    if (cc.memberUidArray.count > 0) {
        [cc.memberUidArray removeAllObjects];
    }
    
    NSArray *uidArr =[memberUids componentsSeparatedByString:NSLocalizedString(@"_", nil)];//拆分成数组
    for(NSString* uid in uidArr)
    {
        [cc.memberUidArray addObject:uid];
    }
    
}
/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

-(ChannelManager*)_init {
    self = [super init];
    if (self) {
       
        self.userInfos = [[NSMutableDictionary alloc]init];
        
        DVLog(@"ChannelManager.init");
        self.channel_map = [[NSMutableDictionary alloc] init];
        
 
        
        self.channel_map = [[NSMutableDictionary alloc]init];
        self.redPackageUnHandleMap = [[NSMutableDictionary alloc] init];
        
        self.messageQueue = dispatch_queue_create("com.chatServer.ChannelManager", NULL);
        self.commandRequestQueue =dispatch_queue_create("com.chatServer.CommandRequestQunen", NULL);
    }
    
    return self;
}

-(void) removeChannel:(NSString*) keyStr
{
    if (keyStr.length > 0) {
        [self.channel_map removeObjectForKey:keyStr];
    }
}

-(void)loadingAllChannelWithDB{
    if  (ChatServiceCocos2dx::DB_ChatChannel_switch){
//        int count = [[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
//        int  count  =  [[DBManager sharedDBManager].dbHelper row:[ChatChannel getTableName] where:nil];
        
             NSArray  * channelArray = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:nil orderBy:@"_id asc" offset:0 count:0];
            for (ChatChannel *channel  in channelArray) {
              
                if (channel.channelID.length > 0    ){
                    [self.channel_map setValue:channel forKey:channel.channelID];

                }
            }
      
    }
}

-(void)getMailChatChannelDetail{
    NSArray *channelArray =[self.channel_map allValues];
    
    for (ChatChannel *tempChannel in channelArray) {
        if  (tempChannel.channelType == IOS_CHANNEL_TYPE_USER){
            NSString *uid =  tempChannel.channelID;
            NSUserInfo *tempUser = [[UserManager sharedUserManager]gettingUserInforWithUid:uid];
            if (tempUser != nil){
                tempChannel.nameString = tempUser.userName;
                tempChannel.faceImageString = tempUser.headPic;
                tempChannel.faceCustemString = [NSString gettingCustomHeadPic:uid intValue:tempUser.headPicVer];
            }

//            [[ChannelManager sharedChannelManager].channel_map setObject:tempChannel forKey:tempChannel.channelID];
        }else if (tempChannel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
            tempChannel.nameString = tempChannel.customName;
        }
         [tempChannel settingMsgListWithLastMsgFromDB];
    }
}
 

-(NSString*) gettingChannelInfo
{
    NSString *result = @"";
    
    for(NSString *channelkey in [self.channel_map allKeys])
    {
        ChatChannel *chatChannel = [self.channel_map objectForKey:channelkey];
        if(![chatChannel containCurrentUser]){
            continue;
        }
        
        if (result.length >0) {
            result = [result stringByAppendingString:@","];
        }
        
        
        result =  [result stringByAppendingFormat:@"%@,%@,%@,%@,%@",chatChannel.channelID,@"|",chatChannel.dbMaxSeqId,@"|",chatChannel.channelType];
    }

    return result;
}

+(instancetype)sharedChannelManager
{
    static ChannelManager *shareChannelManager=nil;
    static dispatch_once_t onceToken1;
    dispatch_once(&onceToken1, ^{
        shareChannelManager = [[ChannelManager alloc] _init];
    });
    return shareChannelManager;
}

-(NSArray *)channelWithMod{
    NSArray *channelArray = [self.channel_map allValues];
    NSMutableArray *temparray = [NSMutableArray array ];
    for (ChatChannel *tempChannel in channelArray) {
        if (tempChannel.channelType == IOS_CHANNEL_TYPE_USER || tempChannel.channelType ==IOS_CHANNEL_TYPE_CHATROOM ){
            if ( [tempChannel.channelID hasSuffix:@"@mod"]) {
                [temparray addObject:tempChannel];
            }
        }
    }
    return (NSArray *)temparray;
}
-(NSArray *)channelWithNormal{
    NSArray *channelArray = [self.channel_map allValues];
    NSMutableArray *temparray = [NSMutableArray array ];
    for (ChatChannel *tempChannel in channelArray) {
        if (tempChannel.channelType == IOS_CHANNEL_TYPE_USER || tempChannel.channelType ==IOS_CHANNEL_TYPE_CHATROOM){
            if ( ![tempChannel.channelID hasSuffix:@"@mod"]) {
                [temparray addObject:tempChannel];
            }
        }
    }
    return (NSArray *)temparray;
}

/** channelID 转换通过系统邮件的Type */
+(NSArray *)channelTypesWithChannelId:(NSString *)vChannelIDString{
    NSMutableArray *typesArray = [NSMutableArray array];
    if ([vChannelIDString isEqualToString:Mail_ChannelID_Fight] ) {
    
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Battle_Report ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Detect ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Detect_Report ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Encamp ]];
//        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_World_boss ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",World_New_Explore ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",World_Monster_Special ]];
        
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_Alliance]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_System ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Donate ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Allianceinvaite]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Alliance_All ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Allianceapply ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Alliance_Kickout ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Invite_teleport ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Refuse_All_Apply ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Resource_Help ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Allinace_Package ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Allinace_RankChange ]];
    
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_Studio]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",All_Service ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Sysupdate ]];
        
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_System]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Sysnotice ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_System ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Service ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Fresher ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Wounded ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Gift ]];
        
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_Resource]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Resource ]];
        
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_Knight]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Battle_Report]];
        
    }
    else if ([vChannelIDString isEqualToString:Mail_ChannelID_Monster]){
         [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Attackmonster ]];
        
    }else if ([vChannelIDString isEqualToString:Mail_ChannelID_Event]){
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_World_boss ]];
        [typesArray addObject:[NSString stringWithFormat:@"%d",Mail_Battle_Report ]];
    }
    return (NSArray *)typesArray;
    
}

#pragma mark -
#pragma mark message 相关
+(NSDictionary *)lastMessageForUserUidArray:(NSArray *)vUidArray{
    NSMutableDictionary *dic =[NSMutableDictionary dictionary ];
    for (NSString *uidString in vUidArray) {
        ChatChannel *channel= [[ChannelManager sharedChannelManager]gettingChannelInfo:uidString];
        CSMessage * tempmsg = [channel gettingLastMessageForNotSelfSendWithDB];
        if  (tempmsg){
            [dic setObject:tempmsg.msg forKey:uidString];
        }else{
             [dic setObject:@"" forKey:uidString];
        }
    }
    return dic;
}

#pragma mark -
#pragma mark 聊天室command字符串

-(NSString *)stringForChatRoomWithAllRoomChannel{
       int count = [[DBManager sharedDBManager ].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
    NSArray *roomChannelArray =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelType = 3" ] orderBy:@"_id asc" offset:0 count:count];
    NSString *roomCommandString = @"";
    if(roomChannelArray.count > 0){
        for (ChatChannel *roomChannel in roomChannelArray) {
           NSMsgItem *msgItem = [roomChannel gettingLastMsg];
            if(roomCommandString.length > 0){
                roomCommandString = [roomCommandString stringByAppendingString:@","];
            }
            if (msgItem ){
                roomCommandString  = [roomCommandString stringByAppendingFormat:@"%@|%ld|3",roomChannel.channelID,msgItem.sequenceId];
            }else{
                roomCommandString  = [roomCommandString stringByAppendingFormat:@"%@|0|3",roomChannel.channelID ];
            }
        }
    }else{
        roomCommandString = @"0|0|3";
    }
    DVLog(@"聊天室的请求command ：%@",roomCommandString);
    return roomCommandString;
}

-(int )chechAllUnreadCount{
    int allUnreadCount =0;
   allUnreadCount =  [self systemMailChannelUnreadManager];
    
    int chatUnreadMember =[self chatMailCHannelUnreadManager];
    DVLog(@"系统消息：%d \n有未读的消息联系人数：%d",allUnreadCount,chatUnreadMember);
  allUnreadCount = allUnreadCount +  chatUnreadMember;
     
    return allUnreadCount;
}
#pragma mark -
#pragma mark 未读数管理

//便利刷新所有的系统邮件未读
-(void)systemMailChannelUnreadUpdateWithAppOpened{
    NSArray *channelIdArray =@[
                               //第一组
                               Mail_ChannelID_Alliance,
                               Mail_ChannelID_Fight,
                               Mail_ChannelID_Event,
                               Mail_ChannelID_Studio,
                               Mail_ChannelID_System,
                               //第二组
                               Mail_ChannelID_Resource,
                               Mail_ChannelID_Monster,
                               Mail_ChannelID_Mod];
    for (NSString *channelIDString  in channelIdArray) {
        //mod邮件和聊天邮件一样，不能用系统邮件的方式计算，放在聊天邮件中计算
        if  (![channelIDString isEqualToString:Mail_ChannelID_Mod]){
            ChatChannel *channel = [self gettingChannelInfo:channelIDString];
            
            [channel channelUnreadCountcalculatedWithSystemMail];
            [channel channelUpDateWithSelfChanged];
        }
        
    }
    
}


-(int )systemMailChannelUnreadManager{
    int count = 0;
    NSArray *channelIdArray =@[
                               //第一组
                               Mail_ChannelID_Alliance,
                                Mail_ChannelID_Fight,
                                 Mail_ChannelID_Event,
                                Mail_ChannelID_Studio,
                                Mail_ChannelID_System,
                                //第二组
                               Mail_ChannelID_Knight,
                                Mail_ChannelID_Resource,
                                Mail_ChannelID_Monster,
                                Mail_ChannelID_Mod];
    for (NSString *channelIDString  in channelIdArray) {
        ChatChannel *channel = [self gettingChannelInfo:channelIDString];
 
        count = count + channel.unreadCount;
    }
    return count ;
}

-(int )chatMailCHannelUnreadManager{
    NSArray *channleArray =[self.channel_map allValues];
    //消息
    ChatChannel *messageChannel = [self gettingChannelInfo:Mail_ChannelID_Message];
    if  (messageChannel == nil){
         [self createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:Mail_ChannelID_Message];
        messageChannel = [self gettingChannelInfo:Mail_ChannelID_Message];
        
    }
    //mod
    ChatChannel *modChannel =[self gettingChannelInfo:Mail_ChannelID_Mod];
    if  (modChannel == nil){
        [self createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:Mail_ChannelID_Mod];
        modChannel = [self gettingChannelInfo:Mail_ChannelID_Mod];
        
    }
    messageChannel.unreadCount = 0;
    modChannel.unreadCount = 0;
    for (ChatChannel *channel in channleArray) {

        if  (channel.channelType == IOS_CHANNEL_TYPE_USER || channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
                   DLog(@"%@   %d",channel.channelID,channel.channelType);
//            [channel channelUnreadCountWithChatMsg];
 
            if (channel.unreadCount > 0){
               
                if ([channel.channelID hasSuffix:@"@mod"]){
                    modChannel.unreadCount = modChannel.unreadCount + 1;
                }else{
                    messageChannel.unreadCount = messageChannel.unreadCount +1;
                }
            }
            
        }
    }
    [messageChannel channelUpDateWithSelfChanged];
    [modChannel channelUpDateWithSelfChanged];
    return messageChannel.unreadCount ;
}
/** 所有普通消息标记已读 */
-(void)settingChatMailMsgReaded{
    NSArray * channelArr =[self.channel_map allValues];
    for (ChatChannel *channel  in channelArr) {
        if (channel.channelType == IOS_CHANNEL_TYPE_USER || channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
            channel.unreadCount = 0;
            if (![channel.channelID hasSuffix:@"@mod"]){
               [channel channelUpDateWithSelfChanged];
            }
            
        }
    }
}
/** 所有mod邮件标记已读 */
-(void)settingModChatMailMsgReaded{
    NSArray * channelArr =[self.channel_map allValues];
    for (ChatChannel *channel  in channelArr) {
        if (channel.channelType == IOS_CHANNEL_TYPE_USER || channel.channelType == IOS_CHANNEL_TYPE_CHATROOM){
            channel.unreadCount = 0;
            if ([channel.channelID hasSuffix:@"@mod"]){
                [channel channelUpDateWithSelfChanged];
            }
            
        }
    }
}
#pragma mark -
#pragma mark channel DB 
-(void)saveChannelToDB:(ChatChannel *)vChannel{
    ChatChannel *tempChannel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelID = '%@'",vChannel.channelID] orderBy:@"_id asc"];
    if (tempChannel == nil) {
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[ChatChannel getTableName] andWithModel:vChannel];
    }else{
        vChannel._id = tempChannel._id;
        [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[ChatChannel getTableName]  andWithModel:vChannel where:[NSString stringWithFormat:@"(_id = %ld)",vChannel._id]];
        
        
        
    }
}


-(ChatChannel *)gettingMailChannelWithLastChanged{
    ChatChannel *channel = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelType = 2"]  orderBy:@"LatestTime desc"];
    
   
    
//    int count = [[DBManager sharedDBManager ].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
//    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelType = 2"] orderBy:@"_id asc" offset:0 count:count];
    
    
    if (channel){
       
        return  channel;
        
    }else{
        return nil;
    }
}
-(ChatChannel *)gettingRoomChatMailChannelWithLastChanged{
    int count = [[DBManager sharedDBManager ].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelType = 3"] orderBy:@"_id asc" offset:0 count:count];
    
    
    if (channelArr.count > 0){
        channelArr = [NSArray chatChannelSortingDataSource:channelArr];
        return  (ChatChannel *)channelArr[0];
        
    }else{
        return nil;
    }
}

-(NSArray *)gettingOneChatAndRoomChatChannelWithDB{
    int count = [[DBManager sharedDBManager ].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"(ChannelType = 2 or ChannelType = 3)"] orderBy:@"LatestTime desc" offset:0 count:count];
    
    
    if (channelArr.count > 0){
        channelArr = [NSArray chatChannelSortingDataSource:channelArr];
        return   channelArr;
        
    }else{
        return nil;
    }
}
-(NSArray *)gettingSystemMailArrayWithDB {
    int count =[[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:@"ChannelType = 4" orderBy:@"_id asc" offset:0 count:count ];
    if  (channelArr.count > 0){
        return channelArr;
    }else{
        return nil;
    }
}

/**  根据channelID从邮件表获取系统邮件 */
-(NSArray *)gettingSystemMailArrayWithChannelIDFromDB:(NSString *)vChannelID andGetCount:(int)vCount{
    
//        NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ChannelID = '%@')",vChannelID] orderBy:@"_id desc" offset:0 count:vCount ];
    /** 新方法根据邮件type获取对应的邮件，channelID类型不再作为唯一的 */
    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[DBManager dbSqlWithChannelID:vChannelID] orderBy:@"_id desc" offset:0 count:vCount ];
    
    if  (channelArr.count > 0){
        return channelArr;
    }else{
        return nil;
    }
}

-(NSArray *)gettingOldSystemMailArrayFromDBWithChannelID:(NSString *)vChannelID
                                       andWithBeforeMail:(MailInfoDataModel *)vMailModel{
//    NSArray *oldMailArr = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ChannelID = '%@' and CreateTime < %ld)",vChannelID,vMailModel.creatTime] orderBy:@"_id desc" offset:0 count:20];
    
    
    NSString *seqliteString =[DBManager dbSqlWithChannelID:vChannelID];
    seqliteString = [NSString stringWithFormat:@"(%@ and CreateTime < %ld)",seqliteString,vMailModel.creatTime];
     NSArray *oldMailArr = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:seqliteString orderBy:@"_id desc" offset:0 count:10];
    
    if  (oldMailArr.count > 0){
        return oldMailArr;
    }else{
        return nil;
    }
}

-(void) saveToDBByMsgItem:(NSMsgItem*) msgItem
{
    if (msgItem && ![msgItem.msg isEqualToString:@"1"]){
        msgItem.msg = @"1";
        if (msgItem.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
            ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingAllianceChannel];
            [channel savingMsgItemToDBWithMsgItem:msgItem];
        }else if(msgItem.channelType == IOS_CHANNEL_TYPE_COUNTRY){
            ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingAllianceChannel];
            [channel savingMsgItemToDBWithMsgItem:msgItem];
        }
    }
}

-(void) loadMoreSysMailFromDBByChannel:(ChatChannel*) channel withAddId:(MailInfoDataModel*)mailId
{
    NSArray *tempArray  = [[ChannelManager sharedChannelManager]gettingOldSystemMailArrayFromDBWithChannelID:channel.channelID andWithBeforeMail:[channel.msgList lastObject]];
    
    if (!tempArray)
        return ;
    int count = [[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[MailInfoDataModel getTableName] withModelClass:[MailInfoDataModel class] where:[DBManager dbSqlWithChannelID:channel.channelID]];
    
    if (tempArray && [tempArray count] > 0){
        for (MailInfoDataModel *mailModel in tempArray) {
            if (!mailModel.maildata) {
                if ([channel.channelID isEqualToString:Mail_ChannelID_Resource]) {
                    
                    mailModel.maildata = [[ResourceMailData alloc] initWithAddMailData:mailModel withAddCount:count];
                    ResourceMailData *resourceMailData =(ResourceMailData*)mailModel.maildata;
                    ResourceMailContents *resourceMailContents = (ResourceMailContents*)[resourceMailData.collect firstObject];
                    NSDictionary *tempDic = resourceMailContents.keyValues;
                    NSString *jsonStr = [tempDic dataTOjsonString];
                    
                    CCDictionary *dic = CCJSONConverter::sharedConverter()->dictionaryFrom([jsonStr UTF8String]);
                    
                    MailResourceCellInfo* mailResourceCellInfo = NULL;
                    map<string, MailInfo*>::iterator it;
                    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                        if(it->second->type==MAIL_RESOURCE){
                            mailResourceCellInfo = dynamic_cast<MailResourceCellInfo*>(it->second);
                            break;
                        }
                    }
                    MailController::getInstance()->addOneResourceToMail(dic, mailResourceCellInfo);
                }else if ([channel.channelID isEqualToString:Mail_ChannelID_Monster]){
                    MonsterMailData *monsterMaiData= [[MonsterMailData alloc] initWithAddMailData:mailModel withAddCount:count];
                    mailModel.maildata = monsterMaiData;
                    
                    MonsterMailContents *monsterMailData = (MonsterMailContents*)monsterMaiData.monster[0];
                    
                    NSDictionary *tempDic = monsterMailData.keyValues;
                    NSString *jsonStr = [tempDic dataTOjsonString];
                    CCDictionary *dic = CCJSONConverter::sharedConverter()->dictionaryFrom([jsonStr UTF8String]);
                    
                    MailMonsterCellInfo* mailMonsterCellInfo = NULL;
                    map<string, MailInfo*>::iterator it;
                    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                        if(it->second->type==MAIL_ATTACKMONSTER){
                            mailMonsterCellInfo = dynamic_cast<MailMonsterCellInfo*>(it->second);
                            break;
                        }
                    }
                    MailController::getInstance()->addOneMonsterToMail(dic, mailMonsterCellInfo);
                }else if ([channel.channelID isEqualToString:Mail_ChannelID_Knight]){
                    BattleMailData *battleMailData = [[BattleMailData alloc] initWithAddMailData:mailModel withAddCount:count];
                    mailModel.maildata = battleMailData;
                    BattleMailContents *battleMailContents = (BattleMailContents*)battleMailData.knight [0];
                    
                    NSDictionary *tempDic = battleMailContents.keyValues;
                    NSString *jsonStr = [tempDic dataTOjsonString];
                    CCDictionary *dic = CCJSONConverter::sharedConverter()->dictionaryFrom([jsonStr UTF8String]);
                    
                    MailHeiqishiCellInfo* mailHeiqishiCellInfo = NULL;
                    map<string, MailInfo*>::iterator it;
                    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
                        if(it->second->battleType == 6){
                            mailHeiqishiCellInfo = dynamic_cast<MailHeiqishiCellInfo*>(it->second);
                            break;
                        }
                    }
//                    MailController::getInstance()->addOneHeiqishiToMail(dic, mailHeiqishiCellInfo); simon
                }
            }
        }
        [channel.msgList addObjectsFromArray:tempArray];
        channel.msgList  = [NSArray mailInfoDataSortingDataSource:channel.msgList];
    }
    int length = [tempArray count];
    if(length>0){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_ADD, CCInteger::create(length));
    }
    
}

-(void)dealMailFrom2dxByMailId:(NSString *)mailUid
{
    MailInfoDataModel * tempMode = [CSConvertViewManager sharedMailInfoManager].currentmailInfoDataModel;
//    ChatChannel *cc = [self gettingChannelInfo:tempMode.channelID];
    [tempMode.maildata settingMailDealStatus];
    tempMode.contents = tempMode.maildata.contents;
    [MailInfoManager mailInfoDataUpdateToDBWithMailDate:tempMode];

}
@end
