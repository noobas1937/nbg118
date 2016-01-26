//
//  UserManager.h
//  IF
//
//  Created by mzc on 15/4/10.
//
//

#import <Foundation/Foundation.h>
#import "NSUserInfo.h"
#import "MailInfoIOS.h"

@interface UserManager : NSObject

+(instancetype)sharedUserManager;

@property (nonatomic,strong) NSUserInfo *currentUser;
@property (nonatomic,strong) MailInfoIOS *currentMail;
@property (nonatomic,strong) NSMutableDictionary *allianceMemberInfoMap;        //联盟成员信息
@property (nonatomic,strong) NSMutableDictionary *nonAllianceMemberInfoMap;     //非联盟成员信息map
/**
 *  普通好友 缓存
 */
@property (nonatomic,strong) NSMutableArray *friends_ordinary;
/**
 *  标星好友 缓存
 */
@property (nonatomic,strong) NSMutableArray *friends_FAVO;
@property (nonatomic,strong) NSMutableArray *groupArray;
@property (nonatomic,assign) long long lastAddUidTime;
@property (nonatomic,assign) long long lastCallSuccessTime;
@property (nonatomic,assign) long long lastCallTime;
@property (nonatomic,strong)NSTimer  *timer;

-(void)addRestrictUser:(NSString*)uid :(int)type;
-(void)removeRestrictUser:(NSString*)uid :(int)type;
-(BOOL)isInRestrictList:(NSString*)uid :(int)type;

//禁言黑名单
@property (nonatomic,strong) NSMutableArray *banNameList;
//屏蔽黑名单
@property (nonatomic,strong) NSMutableArray *blockNameList;
/**
 *  举报头像名单
 */
@property (nonatomic,strong) NSMutableArray *reportList;
/**
 *  添加举报名单
 *
 *  @param uid 用户ID
 */
-(void) addReportList:(NSString*)uid;
/**
 *  移除举报名单
 *
 *  @param uid 用户ID
 */
-(void) removeReportList:(NSString*)uid;
/**
 *  查询该用户是否存在于举报名单中
 *
 *  @param uid 用户ID
 *
 *  @return true or false
 */
-(BOOL) isUid2ReportList:(NSString*)uid;

//添加黑名单
-(void) addRestrictUser:(NSString *)name :(int)type;

//移除黑名单
-(void) removeRestrictUser:(NSString *)name :(int)type;

-(void) onReceiveUserInfo:(NSMutableArray*) userInfos;

-(NSArray*) gettingMemberInfo;

-(NSString*) getRankLang:(int)rank;

-(void) clearAllianceMember;

-(NSUserInfo*) gettingUser:(NSString*)userID;

-(void) addUser:(NSUserInfo*)userInfo;

-(void) putChatRoomMemberInMap:(NSUserInfo*)userInfo;

-(void) getMultiUserInfo:(NSMutableArray*)userUidArray;

-(void) gettingServicerUser:(NSString*)userID;

/**userInfoArray 转换 需要的模型 groupArray*/
-(void) userInfoArraySwitchGroupArray;


-(NSUserInfo *)gettingUserInforWithUid:(NSString *)vUid;
-(NSUserInfo *)gettingUserInfoForMemoryAndDBWithUidString:(NSString *)vUidString;

@end
static UserManager * userManager_U = nil;
const static int um_BLOCK_LIST = 1;
const static int um_BAN_LIST = 2;