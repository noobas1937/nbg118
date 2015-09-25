//
//  ChannelManager.m
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import "ChannelManager.h"
#import "ChannelView.h"
#import "UserManager.h"
#import "ServiceInterface.h"
#import "ChatServiceCocos2dx.h"

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
-(void)deletingChatChannel:(ChatChannel *)vChatChannel{
      NSArray *keyArray = [self.channel_map allKeys];
    if ([keyArray containsObject:vChatChannel.channelID]) {
        [self.channel_map removeObjectForKey:vChatChannel.channelID];
        
    }
    
    /**
     *  如果频道是联盟 删除联盟对应的数据源数据
     */
    if (vChatChannel.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.dataSourceArray removeAllObjects];
    }
    
    if (ChatServiceCocos2dx::DB_ChatChannel_switch){
        ChatChannel *tempChannel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelID = %@",vChatChannel.channelID] orderBy:@"rowid asc"];
        if (tempChannel != nil) {
            [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[ChatChannel getTableName] andWithModel:vChatChannel];
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
 
    [self.channel_map  setObject:vChannel forKey:vChannelId];
    [vChannel setChannelID:vChannelId];
    if (ChatServiceCocos2dx::DB_ChatChannel_switch) {
        ChatChannel *tempChannel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:[NSString stringWithFormat:@"ChannelID = %@",vChannelId] orderBy:@"rowid asc"];
        if (tempChannel == nil) {
            [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[ChatChannel getTableName] andWithModel:vChannel];
        }else{
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[ChatChannel getTableName] andWithModel:vChannel where:[NSString stringWithFormat:@"ChannelID = %@",vChannelId]];
        }
    }
}

-(void) updateChatChannelInfo:(NSString *)newMsgInfo
{
    NSLog(@"newMsgInfo === %@",newMsgInfo);
    NSArray *chatChannelInfos = [newMsgInfo componentsSeparatedByString:@","];
    [self settingChatChannelAttribute:chatChannelInfos];
    
}

//获取频道属性 频道类型|频道ID|serverMinSeqId|serverMaxSeqId,道类型|频道ID|serverMinSeqId|serverMaxSeqId
-(void) settingChatChannelAttribute:(NSArray*)chatChannelInfos
{
    for(NSString *chatChannelInfo in chatChannelInfos)
    {
        NSArray *chatChannelAttributes = [chatChannelInfo componentsSeparatedByString:@"|"];
        NSLog(@"chatChannelAttributes === %@",chatChannelAttributes);
        
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
     ChatChannel *cc = [self.channel_map objectForKey:allianceId];
    if(cc == nil){
        [self createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:[UserManager sharedUserManager].currentUser.allianceId];
        cc = [self.channel_map objectForKey:allianceId];
    }
   
    return cc;
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
    ChatChannel *cc = [self.channel_map objectForKey:chatChannelId];
    return cc;
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

-(ChannelManager*)init {
    self = [super init];
    if (self) {
       
        self.userInfos = [[NSMutableDictionary alloc]init];
        
        NSLog(@"ChannelManager.init");
        self.channel_map = [[NSMutableDictionary alloc] init];
        
        self.channelViews = [[NSMutableArray alloc] init];
        for (int i = 0; i < CHANNEL_COUNT; i++)
        {
            ChannelView *channelView = [[ChannelView alloc]init];
            channelView.tab = i;
            [self.channelViews addObject:channelView];
        }
        
        self.channel_map = [[NSMutableDictionary alloc]init];
        
        if  (ChatServiceCocos2dx::DB_ChatChannel_switch){
            int  count  =  [[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[ChatChannel getTableName] where:nil];
            if(count > 0){
                NSArray  * channelArray = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:nil orderBy:@"rowid asc" offset:0 count:count];
                for (ChatChannel *channel  in channelArray) {
                    DVLog(@"%@",[channel printAllPropertys]);
                    if (channel.channelID.length > 0    ){
                        [self.channel_map setValue:channel forKey:channel.channelID];
                    }
                    
 
                }
            }
        }
        

    }
    
    return self;
}

-(void) removeChannel:(NSString*) keyStr
{
    if (keyStr.length > 0) {
        [self.channel_map removeObjectForKey:keyStr];
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
        shareChannelManager = [[ChannelManager alloc] init];
    });
    return shareChannelManager;
}

@end
