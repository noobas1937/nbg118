//
//  UserManager.m
//  IF
//
//  Created by mzc on 15/4/10.
//
//

#import "UserManager.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatChannel.h"
#import "ChatServiceController.h"
#import "ServiceInterface.h"
#import "UserGroup.h"
#import "ChatServiceCocos2dx.h"


@interface UserManager()

-(BOOL)isInRestrictList:(NSString *)name :(int)type;
@property (nonatomic,strong) NSMutableDictionary *rankMap;      //用于联盟成员等级排序
@property (nonatomic,strong) NSMutableDictionary *allianceMemberMap;
@property (nonatomic,strong) NSMutableArray *fechingUids;
@property (nonatomic,strong) NSMutableArray *queueUids;
@property (nonatomic,strong) NSMutableArray *userList;
@end

@implementation UserManager

-(void) addRestrictUser:(NSString *)name :(int)type
{
    if (![self isInRestrictList:name :type] && type == 1){
        [self.blockNameList addObject:name];
    }else if(![self isInRestrictList:name :type] && type == 2){
        [self.banNameList addObject:name];
    }
    
}

-(void) removeRestrictUser:(NSString *)name :(int)type
{
    if (type == 1)
    {
        for(NSString *n in self.blockNameList){
            if([n isEqualToString:name]){
                [self.blockNameList removeObject:n];
            }
        }
    }
    else if (type == 2)
    {
        for(NSString *n in self.banNameList){
            if([n isEqualToString:name]){
                [self.banNameList removeObject:n];
            }
        }
    }
}

-(BOOL) isInRestrictList:(NSString *)name :(int)type
{
    if(type == 1){              //屏蔽
        for(NSString *n in self.blockNameList){
            if([n isEqualToString:name]){
                return true;
            }
        }
    }else if(type == 2){        //禁言
        for(NSString *n in self.banNameList){
            if([n isEqualToString:name]){
                return true;
            }
        }
    }
    return false;
}


/**
 *  获取到userinfor的数据array
 *
 *  @param userInfos 模型数组
 */
-(void) onReceiveUserInfo:(NSMutableArray *)userInfos
{
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        //加入耗时操作
        [self saveToDB:userInfos];
        
    });
    self.lastCallSuccessTime = [self gettingCurrentTimeMillis];
    
    for(NSUserInfo *user in userInfos)
    {
        
        [self putChatRoomMemberInMap:user];
        
        [user initNullField];
        
        user.isDummy = FALSE;
        
        NSUserInfo *oldUser = [self gettingUser:user.uid];
        //        if([self.fechingUids containsObject:user.uid]){
        //            [self.fechingUids removeObject:user.uid];
        //        }
        
        if (oldUser == nil)
        {
            //				System.out.println("impossible >>>>> onReceiveUserInfo(): oldUser is null");
            //				System.out.println(LogUtil.typeToString(user));
            [self addUser:user];
        }
        else if (oldUser.isDummy || user.lastUpdateTime > oldUser.lastUpdateTime)
        {
            //				System.out.println("onReceiveUserInfo(): update user");
            //				System.out.println(LogUtil.compareObjects(new Object[] { user, oldUser }));
            [self updateUser:user];
            //            ChatServiceController.getInstance().notifyCurrentDataSetChanged();
        }
        else
        {
            //				System.out.println("impossible >>>>> onReceiveUserInfo(): user is not newer");
            //				System.out.println(LogUtil.compareObjects(new Object[] { user, oldUser }));
        }
        
        //ChatServiceCocos2dx::updateCellFrame([ns_uid UTF8String]);
//        [[ServiceInterface serviceInterfaceSharedManager] updateChatCellFrame:user.uid];
    }
}

-(void) putChatRoomMemberInMap:(NSUserInfo*)user
{
    int rank=user.allianceRank;
    NSString *uid=user.uid;
    NSString *ns_allianceId = [UserManager sharedUserManager].currentUser.allianceId;
    
    if(ns_allianceId!=nil && [ns_allianceId isEqualToString:user.allianceId])
    {
        [self.allianceMemberInfoMap setObject:user forKey:uid];
        if(rank>0)
        {
            NSString *rankKey = [self getRankLang:rank];
            [self.rankMap setObject:[NSNumber numberWithInteger:rank] forKey:rankKey];
            [self resetAllianceRank:rankKey];
            NSMutableArray *userArr = [self.allianceMemberMap objectForKey:rankKey];
            BOOL isInRank=false;
            for(NSUserInfo *info in userArr)
            {
                if([info.uid isEqualToString:user.uid])
                {
                    [[self.allianceMemberMap objectForKey:rankKey] removeObject:info];
                    [[self.allianceMemberMap objectForKey:rankKey] addObject:user];
                    isInRank=true;
                    return;
                }
            }
            if(!isInRank)
            [[self.allianceMemberMap objectForKey:rankKey] addObject:user];
        }
    }else{
        //添加非联盟成员
        [self putNoAllianceInMap:user];
    }
    
}

-(void) putNoAllianceInMap:(NSUserInfo *) user
{
    [self.allianceMemberInfoMap setObject:user forKey:user.uid];
}

-(NSString*) getRankLang:(int)rank
{
    NSString *rankStr=@"";
    switch(rank)
    {
        case 1:
            rankStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_RANK1];
            break;
        case 2:
            rankStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_RANK2];
            break;
        case 3:
            rankStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_RANK3];
            break;
        case 4:
            rankStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_RANK4];
            break;
        case 5:
            rankStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_RANK5];
            break;
    }
    return rankStr;
}

-(void) resetAllianceRank:(NSString*) key
{
    if([[self.allianceMemberMap allKeys] containsObject:key])
        return;
    NSMutableArray *userInfoArray = [[NSMutableArray alloc]init];
    [self.allianceMemberMap setObject:userInfoArray forKey:key];
}

-(NSUserInfo*) gettingUser:(NSString*)userID
{
    for(int i = 0 ; i < self.userList.count ; i ++)
    {
        NSUserInfo *userInfo = self.userList[i];
        if ([userID isEqualToString:userInfo.uid])
            return userInfo;
    }
    
    return nil;
}

-(void) gettingServicerUser:(NSString*)userID
{
    NSMutableArray *userUidArray = [[NSMutableArray alloc]init];
    [userUidArray addObject:userID];
    [[UserManager sharedUserManager] getMultiUserInfo:userUidArray];
}

-(void) clearAllianceMember
{
    if(self.allianceMemberMap!= nil)
        [self.allianceMemberMap removeAllObjects];
    if(self.allianceMemberInfoMap!=nil)
        [self.allianceMemberInfoMap removeAllObjects];
    if(self.rankMap!=nil)
        [self.rankMap removeAllObjects];
}

-(void) addUser:(NSUserInfo *)userInfo
{
    [self.userList addObject:userInfo];
}

-(void) getMultiUserInfo:(NSMutableArray *)userUidArray
{
    for (NSString *uid in userUidArray)
    {
        if(![self.fechingUids containsObject:uid] && ![self.queueUids containsObject:uid]){
            [self.queueUids addObject:uid];
            self.lastAddUidTime = [self gettingCurrentTimeMillis];
            if(self.timer == nil){
                [self startTimer];
            }
            
        }
    }
}

-(void) startTimer
{
    // stopTimer()之后，如果被getMultiUserInfo线程抢占，可能会调两次startTimer()，导致开启多个TimerTask，且不被中止
    // 可能会导致OutOfMemoryError
    if (_timer != nil){
        return;
    }
    
    _timer = [NSTimer timerWithTimeInterval:0.1 target:self selector:@selector(startTimeCallFun) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop]addTimer:_timer forMode:NSDefaultRunLoopMode];
}

-(void) stopTimer
{
    if (_timer){
        [_timer invalidate];
        _timer = nil;
    }
}

-(void) startTimeCallFun
{
    long long now = [self gettingCurrentTimeMillis];
//    DVLog(@"now:  %lld",now);
    if ([self.queueUids count] > 0 && (now - self.lastAddUidTime) > 500 && ([self isLastCallSuccess] || [self isLastCallTimeOut])) {
        [self actualCall];
        [self stopTimer];
        if ([self.queueUids count] > 0) {
            [self startTimer];
        }
    }
    
}

-(void) actualCall
{
    
    if([self.fechingUids count] > 0)
    {
        if ([self isLastCallSuccess]) {
            [self.fechingUids removeAllObjects];
        }else{
            NSLog(@"超时：fechingUids is not empty");
        }
    }
    
    int count = [self.queueUids count] > (20 - [self.fechingUids count]) ? (20 - [self.fechingUids count]) : [self.queueUids count];
    
    for(int i = 0 ; i < count ; i++){
        [self.fechingUids addObject:[self.queueUids objectAtIndex:0]];
        [self.queueUids removeObjectAtIndex:0];
    }
    
    NSString *uidsStr = [ChatChannel getMembersString:self.fechingUids];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost getMultiUserInfo:uidsStr];
    self.lastCallTime = [self gettingCurrentTimeMillis];
    self.lastCallSuccessTime = [self gettingCurrentTimeMillis] * 2;
    
}

-(long long ) gettingCurrentTimeMillis
{
    return (long long )[[NSDate date] timeIntervalSince1970] * 1000;
}
/** 当前uid更新成功 */
-(BOOL) isLastCallSuccess
{
    return ([self gettingCurrentTimeMillis] - self.lastCallSuccessTime) > 0;
}
/** 当前uid更新超时 */
-(BOOL) isLastCallTimeOut
{
    return self.lastCallTime > 0 && ([self gettingCurrentTimeMillis] - self.lastCallTime) > 8000;
}

-(void) updateUser:(NSUserInfo*)user
{
    for (int i = 0; i < [self.userList count]; i++)
    {
        NSUserInfo *userListInUser = [self.userList objectAtIndex:i];
        if ([userListInUser.uid isEqualToString:user.uid])
        {
            [self.userList setObject:user atIndexedSubscript:i];
        }
    }
}

-(void) userInfoArraySwitchGroupArray
{
    
    NSMutableArray *rank1=[[NSMutableArray alloc]init];
    NSMutableArray *rank2=[[NSMutableArray alloc]init];
    NSMutableArray *rank3=[[NSMutableArray alloc]init];
    NSMutableArray *rank4=[[NSMutableArray alloc]init];
    NSMutableArray *rank5 =[[NSMutableArray alloc]init];
    NSArray *arr=@[rank1,rank2,rank3,rank4,rank5];
    
    NSMutableArray *newGroupArray = [[NSMutableArray alloc]init];
    
    NSArray *userUidKeys = [[UserManager sharedUserManager].allianceMemberInfoMap allKeys];
    for(NSString *keyUid in userUidKeys)
    {
        NSUserInfo *userInfo =  [[UserManager sharedUserManager].allianceMemberInfoMap objectForKey:keyUid];
        if (userInfo.allianceRank == 1) {
            [rank1 addObject:userInfo];
        }else if (userInfo.allianceRank == 2){
            [rank2 addObject:userInfo];
        }else if (userInfo.allianceRank == 3){
            [rank3 addObject:userInfo];
        }else if ( userInfo.allianceRank == 4){
            [rank4 addObject:userInfo];
        }else{
            [rank5 addObject:userInfo];
        }
    }
    
    for (NSArray *tempArr in arr) {
        if (tempArr.count>0) {
            if ([self.groupArray count] > 0) {
                [self.groupArray removeAllObjects];
            }
            UserGroup *tempGroup =[UserGroup userGroup];
            tempGroup.grade = [(NSUserInfo *)[tempArr objectAtIndex:0]allianceRank];
            tempGroup.open = YES;
            tempGroup.groupNameString= [[UserManager sharedUserManager]getRankLang:tempGroup.grade];
            tempGroup.memberArray = tempArr;
            [newGroupArray addObject:tempGroup];
        }
    }
    
    self.groupArray = newGroupArray;
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

-(UserManager*)init {
    self = [super init];
    if (self) {
        self.banNameList = [[NSMutableArray alloc]init];
        self.blockNameList = [[NSMutableArray alloc]init];
        self.currentUser = [[NSUserInfo alloc]init];
        self.currentMail = [[MailInfoIOS alloc]init];
        self.allianceMemberInfoMap = [[NSMutableDictionary alloc]init];
        self.rankMap = [[NSMutableDictionary alloc]init];
        self.allianceMemberMap = [[NSMutableDictionary alloc]init];
        self.fechingUids = [[NSMutableArray alloc]init];
        self.queueUids = [[NSMutableArray alloc]init];
        self.userList = [[NSMutableArray alloc]init];
        self.lastAddUidTime = 0;
        self.lastCallSuccessTime = 0;
        self.lastCallTime = 0;
    }
    return self;
}

+(instancetype)sharedUserManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        userManager_U = [[UserManager alloc] init];
    });
    return userManager_U;
}

+ (id)allocWithZone:(NSZone *)zone
{
    //调用dispatch_once保证在多线程中也只被实例化一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        userManager_U = [super allocWithZone:zone];
    });
    return userManager_U;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}


#pragma mark -
#pragma mark DB 
-(void)saveToDB:(NSArray *)vArray {
    
    if  (ChatServiceCocos2dx::DB_UserInfo_switch){
        for (NSUserInfo *tempUserInfor in vArray) {
            NSUserInfo *dbTempUser =  [self gettingUserInforWithUid:tempUserInfor.uid];
            if (dbTempUser) {
                [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[NSUserInfo getTableName] andWithModel:tempUserInfor where:[NSString stringWithFormat:@"UserID=%@",tempUserInfor.uid]];
            }else{
                [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[NSUserInfo getTableName] andWithModel:tempUserInfor];
            }
        }
    }
    
}

-(NSUserInfo *)gettingUserInforWithUid:(NSString *)vUid{
    if  (ChatServiceCocos2dx::DB_UserInfo_switch){
       NSUserInfo *tempUser =  [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[NSUserInfo getTableName] andWithModelClass:[NSUserInfo class] where:[NSString stringWithFormat:@"UserID=%@",vUid] orderBy:@"rowid asc"];
        if (tempUser != nil) {
            tempUser.isDummy = NO;
            return  tempUser;
        }else{
            return nil;
        }
    }else{
        return nil;
    }
}

@end
