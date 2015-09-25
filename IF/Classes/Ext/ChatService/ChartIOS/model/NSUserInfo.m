//
//  NSUserInfo.m
//  IF
//
//  Created by mzc on 15/6/25.
//
//

#import "NSUserInfo.h"
#import "ChannelManager.h"
#import "ServiceInterface.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatServiceController.h"
#import "GameHostInterface.h"
#import "GameHostInterface.h"


#import "LKDBHelper.h"
@implementation NSUserInfo

-(instancetype) init:(NSString *)uid
{
    self = [super init];
    if (self) {
        
        self.uid = uid;
        self.headPic = @"g026";
        self.userName = uid;
        self.isDummy = TRUE;
   
    }
    return self;
}

-(instancetype) init
{
    self = [super init];
    if (self) {
        self.allianceId = @"";
    }
    return self;
}

-(void) initNullField
{
    if (self.allianceId == nil)
    {
        self.allianceId = @"";
    }
    if (self.asn == nil)
    {
        self.asn = @"";
    }
}

-(void)settingAllianceId:(NSString *)allianceId
{
    if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:allianceId])
    {
        [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:allianceId];
    }
    
    self.allianceId = allianceId;
}

-(NSString *)gettingVipInfo
{
    NSInteger dtime = self.vipEndTime - (NSUInteger)[[NSDate date] timeIntervalSince1970];
    NSString *vipInfo = (self.vipLevel > 0 && dtime > 0) ? [self gettingVipStr]:@"";
    return vipInfo;
}
-(NSString *)gettingVipString
{
    NSInteger dtime = self.vipEndTime - (NSUInteger)[[NSDate date] timeIntervalSince1970];
    NSString *vipInfo = (self.vipLevel > 0 && dtime > 0) ? [self gettingVipStr]:@"";
    return vipInfo;
}


/**@brief 获取组装后的vip+num字符串*/
-(NSString*) gettingVipStr
{
    NSString *vipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].VIP_INFO];
    NSString *vipNum = [NSString stringWithFormat:@"%d",self.vipLevel];
    NSString *vip = [vipStr stringByAppendingString:vipNum];
    return vip;
}

//// 自定义头像本地路径
//-(NSString*) gettingCustomHeadPic
//{
//    NSString *result = [ChatServiceController chatServiceControllerSharedManager].gameHost getCustomHeadPic(getCustomHeadPicUrl());
//    // System.out.println("getCustomHeadPic(): result = " + result);
//    return result;
//}

-(int) gettingHeadPicVer
{
    return self.headPicVer;
}

-(BOOL) isCustomHeadImage
{
    if(self.isDummy)
    return false;
    return self.headPicVer > 0 && self.headPicVer < 1000000;
}

-(NSString*) gettingCustomHeadPicUrl
{
    NSString *result = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingCustomHeadPic:self.uid int:self.headPicVer];
    return result;
}

-(NSString*) gettingCustomHeadPicPath
{
    NSString *customHeadPicPath = [[ChatServiceController chatServiceControllerSharedManager] headPath:[self gettingCustomHeadPicUrl]] ;
    return customHeadPicPath;
}


#pragma mark -
#pragma mark DB 相关



#ifdef DEBUG
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

#endif

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"            :@"tableVer",
             @"UserID"              :@"uid",
             @"NickName"            :@"userName",
             @"AllianceId"          :@"allianceId",
             @"AllianceName"        :@"asn",
             @"AllianceRank"        :@"allianceRank",
             @"ServerId"            :@"serverId",
             @"CrossFightSrcServerId":@"crossFightSrcServerId",
             @"Type"                :@"type",
             @"HeadPic"             :@"headPic",
             @"CustomHeadPic"       :@"headPicVer",
             @"Privilege"           :@"mGmod",
             @"VipLevel"            :@"vipLevel",
             @"VipEndTime"          :@"vipEndTime",
             @"LastUpdateTime"      :@"lastUpdateTime",
             @"LastChatTime"        :@"lastChatTime"
             };
}
 
//+(NSString *)getPrimaryKey
//{
//    return @"UserID";
//}
+(int)getTableVersion
{
    return 1;
}
//表名
+(NSString *)getTableName
{
    return @"User";
}
//更新数据模型后，需要调用

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

/** 给某个字段赋特定的值 */
//-(id)modelGetValue:(LKDBProperty *)property{
//    if ([property.propertyName isEqualToString:@"tempArray"]) {
//        return  nil;
//    }else{
//        return  [super modelGetValue:property];
//    }
//}

@end
