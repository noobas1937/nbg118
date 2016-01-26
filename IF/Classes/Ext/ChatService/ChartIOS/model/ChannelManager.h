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
 
@property (nonatomic,strong) ChatChannel            *countryChannel;
@property (nonatomic,strong) ChatChannel            *allianceChannel;
@property (nonatomic,strong) NSMutableDictionary    *redPackageUnHandleMap;
@property (nonatomic,assign) BOOL                   isFetching;

/**联盟所有成员信息*/
@property (nonatomic,strong) NSMutableDictionary         *userInfos;

@property (nonatomic,strong)   dispatch_queue_t messageQueue;
@property (nonatomic,strong)  dispatch_queue_t commandRequestQueue;

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
-(void)deleteChannelWithChannelArray:(NSArray *)vArray;
-(void)updatingChannelForDBWithChannel:(ChatChannel *)vChannel;
-(void)loadingAllChannelWithDB;
/** 填入从数据库获取的channel的基本信息 */
-(void)getMailChatChannelDetail;
-(ChatChannel *)gettingMailChannelWithLastChanged;
-(ChatChannel *)gettingRoomChatMailChannelWithLastChanged;
-(void)updatingChannelForMail;
-(void)updatingChannelForSysMail;
/** 聊天室新数据请求command */
-(NSString *)stringForChatRoomWithAllRoomChannel;
-(NSArray *)gettingSystemMailArrayWithDB ;
-(NSArray *)gettingSystemMailArrayWithChannelIDFromDB:(NSString *)vChannelID andGetCount:(int)vCount;
-(NSArray *)gettingOldSystemMailArrayFromDBWithChannelID:(NSString *)vChannelID
                                       andWithBeforeMail:(MailInfoDataModel *)vMailModel;
-(NSArray *)gettingOneChatAndRoomChatChannelWithDB;

-(int )systemMailChannelUnreadManager;
-(void)systemMailChannelUnreadUpdateWithAppOpened;
-(int )chatMailCHannelUnreadManager;
-(int )chechAllUnreadCount;

-(NSArray *)channelWithNormal;
-(NSArray *)channelWithMod;

/** 所有消息标记已读 */
-(void)settingChatMailMsgReaded;
-(void)settingModChatMailMsgReaded;
/**
 *  更新DB 作用于更新红包领取状态
 */
-(void) saveToDBByMsgItem:(NSMsgItem*) msgItem;
/** 根据channelID返回对应的types类型 */
+(NSArray *)channelTypesWithChannelId:(NSString *)vChannelIDString;


-(void) loadMoreSysMailFromDBByChannel:(ChatChannel*) channel withAddId:(MailInfoDataModel*)mailId;

-(void) dealMailFrom2dxByMailId:(NSString*) mailUid;
+(NSDictionary *)lastMessageForUserUidArray:(NSArray *)vUidArray;
@end
