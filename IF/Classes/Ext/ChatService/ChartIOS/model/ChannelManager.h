//
//  ChannelManager.h
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import <Foundation/Foundation.h>
#import "ChatChannel.h"
#define  CHANNEL_COUNT                      3
#define  LOAD_MORE_COUNT                   20
#define  LOAD_ALL_MORE_MIN_COUNT           20

@interface ChannelManager : NSObject
+(instancetype)sharedChannelManager;
@property (nonatomic,assign) BOOL                   isGetingNewMsg;
//保存频道数据 对应安卓 Map<String, ChatChannel>
@property (nonatomic,strong) NSMutableDictionary    *channel_map;
@property (nonatomic,strong) NSMutableArray         *channelViews;
@property (nonatomic,strong) ChatChannel            *countryChannel;
@property (nonatomic,strong) ChatChannel            *allianceChannel;
@property (nonatomic,assign) BOOL                   isFetching;

/**联盟所有成员信息*/
@property (nonatomic,strong) NSMutableDictionary         *userInfos;

-(BOOL) isContainFromUid:(NSString*)fromUid;

-(ChatChannel*) gettingChannelInfo:(NSString*) chatChannelId;
-(ChatChannel*) gettingCountryChannel;
-(ChatChannel*) gettingAllianceChannel;

-(void)createChatChannel:(int)channelType withAddChannelID:(NSString *)channelID;
/**创建聊天室*/
-(void) createChatChannelRoom:(NSString *)channelId withChannelMemberUids:(NSString *)memberUids;
/**更新聊天室*/
-(void) updateChatChannelRoom:(int) channelType withAddChannelID:(NSString *) channelID withAddUidArr:(NSString *) memberUids withAddRoomOwner:(NSString *) roomOwner withAddCustomName:(NSString *) customName;

-(void) updateChatChannelInfo:(NSString*) newMsgInfo;

/** 外部调用，会操作数据库 */
-(void)settingChatChannel:(ChatChannel *)vChatChannel;
-(void)deletingChatChannel:(ChatChannel *)vChatChannel;
@end
