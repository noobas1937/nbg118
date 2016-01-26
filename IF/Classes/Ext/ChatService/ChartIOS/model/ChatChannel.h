//
//  ChatChannel.h
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
#import "MailInfoDataModel.h"
#import "ResourceMailData.h"

typedef enum : NSUInteger {
    ResponseMsgTypeInitHistoryMSG = 0,
    ResponseMsgTypeActionMSG = 1,//下拉刷新数据
    ResponseMsgTypeOneMSG = 2 ,//新消息
    ResponseMsgTypeLockScreenCache = 3,
}ResponseMsgType;

@class ChatChannel;
@protocol ChatChannelDelegate <NSObject>
-(void)showNewMsg:(ChatChannel *)vChannle;
-(void)showOldMsg:(ChatChannel *)vChannel;
-(void)showscreenLockedMsg:(ChatChannel *)vChannel;
-(void)showOldmsg_NoOldMsg:(ChatChannel *)vChannel;
-(void)systemMail_pushNewMsgWith:(ChatChannel *)vChannel andMsg:(MailInfoDataModel *)vMailInfoModel;
@end


@interface ChatChannel : NSObject

//DB 存储的属性



/** 
 COUNTRY = 0   ALLIANCE =1 USER = 2 CHATROOM = 3  OFFICIAL = 4  BBS =5
 */
@property (nonatomic,assign)    int                       tableVer;
@property (nonatomic,assign)    long                      _id;
@property (nonatomic,assign)    int                    channelType;                 //-1
@property (nonatomic, strong)   NSString              * channelID;
@property (nonatomic,assign)    long                    dbMinSeqId;                  //-1
@property (nonatomic,assign)    long                    dbMaxSeqId;                  //-1
@property (nonatomic,strong)    NSString                *memberUidString;           // 竖线分割的uid字符串
@property (nonatomic, strong)   NSMutableArray        *memberUidArray;              // 聊天室成员uid列表
@property (nonatomic, strong)   NSString              *roomOwner;                   // 聊天室房主
@property (nonatomic,assign)    int                   isMember;                    // 是否是聊天室成员 FALSE
@property (nonatomic, strong)   NSString              *customName;                  // 聊天室自定义名称
@property (nonatomic,assign)    int                     unreadCount ;                //未读数
@property (nonatomic,assign)    long                         latestTime;                  //最近消息时间
@property (nonatomic,assign)    long                      latestModifyTime ;            // 最近修改时间，仅针对系统邮件
@property (nonatomic,strong) NSString               * latestId;                     //最近消息的id（邮件专用）
@property (nonatomic,strong) NSString               * settings;                     // 置顶、免打扰、保存到通讯录、聊天背景、是否显示昵称等开关型属性，key1:0|key2:1形式（无顺序，没有就是默认值），或者用0|1|0形式





@property (nonatomic,strong)    NSMutableArray         *msgList;                    // 消息对象List，保存所有消息
@property (nonatomic,strong)    NSMutableArray         *sendingMsgList;             // 正在发送的消息
@property (nonatomic,assign)    BOOL                   hasRequestDataBefore;        // 是否获取到消息过
@property (nonatomic,assign)    BOOL                   noMoreDataFlag;              // 是否没有更多消息了
@property (nonatomic,assign)    CGPoint                lastPosition;            //lastPosition 记录退出视图时保存的位置 x为cell的索引 y为cell最上面的值

@property (nonatomic,assign)    long                     serverMinSeqId;
@property (nonatomic,assign)    long                    serverMaxSeqId;
@property (nonatomic,assign)    long                     prevDBMaxSeqId;// 收取前db的最大id

@property (nonatomic,assign)    BOOL                   isLoadingAllNew; // 是否正在批量加载新消息
@property (nonatomic,assign)    BOOL                    hasLoadingAllNew;// 是否已经批量加载过新消息
@property (nonatomic,assign)    int                    firstNewMsgSeqId;
/** 新版邮件批量操作，已选择标记 */
@property (nonatomic,assign,getter=isSelected)BOOL selected;


@property (nonatomic,strong) NSString * nameString;  //显示的名字
@property (nonatomic,strong) NSString * contentsString;//显示的详情内容
@property (nonatomic,strong) NSString * faceImageString;//头像字符串
@property (nonatomic,strong) NSString * timeString;//显示时间
@property (nonatomic,strong) NSString * faceCustemString;//自定义头像url;
@property (nonatomic,assign) BOOL isHasGift;//是否有礼品



@property(nonatomic,assign)id<ChatChannelDelegate>channelDelegate;

/**处理数据*/
//-(void)addCellFrame:(NSMsgItem *)mi :(ResponseMsgType)msgType;

+(NSString*) getMembersString:(NSMutableArray*)members;
-(BOOL) containCurrentUser;

-(int) gettingChatChannelMaxSeqId;

-(int) gettingChatChannelMinSeqId;

-(void) gettingServiceChannelNewMsg;

-(BOOL) gettingServiceChannelOldMsg;
/** 获取数据库中最后一条消息 */
-(BOOL)settingMsgListWithLastMsgFromDB;
/** 保存当前cc的一条消息到对应聊天表 */
-(void)savingMsgItemToDBWithMsgItem:(NSMsgItem *)vMsgItem;
/** 刚刚发送的还未传到服务器的消息插库 */
-(void)savingMsgForSelfSendToServerWithMsgItem:(NSMsgItem *)vMsg;
/**
 *  红包更新DB状态
 */
-(void)redPacketMsgSaveToDBWithMsgItem:(NSMsgItem *)vMsgItem;
-(void)msgUpdateWithMsgItem:(NSMsgItem *)vMsgItem andWithPropertyName:(NSArray *)vPropertyList ;
/** 根据attachmentID 查找红包消息*/
-(NSMsgItem *)redPackageSearchWithAttachmentId:(NSString *)vAttachMentID;
-(void)gettingFirstMsg;
-(BOOL)gettingOldMsg;
// 有新消息推送过来
-(void)chatNewMsgPushed:(NSArray *)vArray;
-(void)chatMsgOpenAppPushArray:(NSArray *)vArray;//第一次启动应用调用
//下拉刷新
-(void)chatMsgPushWithGetOldMsgArray:(NSArray *)vArray;

-(void)chatmsgpushWithPullDownAfterScreenlockedMsgArray:(NSArray *)vArray;
-(void)chatMailMsgPushArray:(NSArray *)vArray;
//
//-(void)oldChatMsgPushArray:(NSArray *)vArray ;
-(NSMsgItem *)gettingLastMsg;
- (NSMutableArray *)sortingDataForMsgItemWithCreatTimeSource:(NSMutableArray *)vArray;
- (NSMutableArray *)sortingMailWithCreateTimeDataSource:(NSMutableArray *)vArray;
-(void)initMsgUserWithUserUid:(NSString *)vUid andBlock:(MsgUserBlock)vBlock;

-(void)msgArrayFormatToStartState;//恢复消息到20条
-(BOOL)refreshingMsgListAtLastStatus;//刷新消息体状态
-(void)channelUpDateWithSelfChanged;//更新当前channel数据库存储信息
/** 聊天消息未读数 */
//-(void)channelUnreadCountWithChatMsg;
/** 系统消息未读数计算 */
-(void)channelUnreadCountcalculatedWithSystemMail;
//-(void)chatMsgMarkToReadStatsu;
-(void)channelMarkToReadStatus;
/**
 *  合并怪物邮件数据
 *
 *  @return 合并后的数据
 */
-(MailData*) mergeMonsterMailData;

/**
 *  合并资源邮件数据
 *
 *  @return 合并后的数据
 */
-(MailData*) mergeResourceMailData;
/**
 *  合并黑骑士邮件
 *
 *  @return 合并后的数据
 */
-(MailData*) mergeKnightMailData;

@end
 
