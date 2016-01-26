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
#import "NSString+Cocos2dHelper.h"

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
        self.relation = -1;
    }
    return self;
}

-(instancetype)initFriendByPic:(NSString *)pic withPicVer:(int)picVer withServerId:(int)serverId withVipEndTime:(double)vipEndTime withAllianceId:(NSString *)allianceId withVipLevel:(int)vipLevel withUid:(NSString *)uid withCrossFightSrcServerId:(int)crossFightSrcServerId withGmFlag:(int)gmFlag withName:(NSString *)name withRank:(int)rank withfriendsType:(int) type
{
    self = [super init];
    if (self) {
        
        if (pic.length > 0) {
            self.headPic = pic;
        }else{
            self.headPic = @"";
        }
        
        if (allianceId.length > 0) {
            self.allianceId = allianceId;
        }else{
            self.allianceId = @"";
        }
        
        if (uid.length > 0) {
            self.uid = uid;
        }else{
            self.uid = @"";
        }
        
        if (name.length > 0) {
            self.userName = name;
        }else{
            self.userName = @"";
        }
        
        if (picVer >= 0) {
            self.headPicVer = picVer;
        }else{
            self.headPicVer = 0;
        }
        
        if (serverId >= 0) {
            self.serverId = serverId;
        }else{
            self.serverId = 0;
        }
        
        if (vipEndTime >= 0.0) {
            self.vipEndTime = vipEndTime;
        }else{
            self.vipEndTime = 0.0;
        }
        
        if (vipLevel >= 0) {
            self.vipLevel = vipLevel;
        }else{
            self.vipLevel = 0;
        }
        
        if (crossFightSrcServerId >= 0) {
            self.crossFightSrcServerId = crossFightSrcServerId;
        }else{
            self.crossFightSrcServerId = 0;
        }
        
        if (gmFlag >= 0) {
            self.mGmod = gmFlag;
        }else{
            self.mGmod = 0;
        }
        
        if (rank >= 0) {
            self.allianceRank = rank;
        }else{
            self.allianceRank = 0;
        }
        
        if (type >= 0) {
            self.relation = type;
        }else{
            self.relation = -1;
        }
        
    }
    return self;
}

-(instancetype)initWithMsgSenderWithMsg:(NSMsgItem *)vMsg{
    self = [super init];
    if (self) {
        if (vMsg.name ) {
            self.allianceId = @"";
            self.uid = vMsg.uid;
            self.userName = vMsg.name;
            self.asn = vMsg.asn;
            self.vip = vMsg.vip;
            self.headPic = vMsg.headPic;
            self.headPicVer = vMsg.headPicVer;
            self.lastUpdateTime = 0;
            self.isDummy = false;
        }else{
            self.allianceId = @"";
            self.uid = vMsg.uid;
            self.userName = vMsg.uid;
            self.lastUpdateTime = 0;
            self.isDummy = false;
        }

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
    
//    NSString *result = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingCustomHeadPic:self.uid int:self.headPicVer];
//    return result;
    
    return  [NSString gettingCustomHeadPic:self.uid intValue:self.headPicVer];
}

-(NSString*) gettingCustomHeadPicPath
{
    NSString *customHeadPicPath = [[ChatServiceController chatServiceControllerSharedManager] headPath:[self gettingCustomHeadPicUrl]] ;
    return customHeadPicPath;
}


#pragma mark -
#pragma mark DB 相关

-(void)updateToDBWithSelf{
    NSUserInfo *tempUser=[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[NSUserInfo getTableName] andWithModelClass:[NSUserInfo class] where:[NSString stringWithFormat:@"UserID = '%@'",self.uid] orderBy:@"rowid asc"];
    if (tempUser == nil) {
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[NSUserInfo getTableName] andWithModel:self];
    }else{
        self._id = tempUser._id;
        if (tempUser.lastUpdateTime<self.lastUpdateTime) {
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[NSUserInfo getTableName] andWithModel:self where:[NSString stringWithFormat:@"(UserID='%@')",self.uid] ];
            
        }
    }
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

//在类 初始化的时候
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

-(id)modelGetValue:(LKDBProperty *)property{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        return [NSString stringWithFormat:@"%d",[NSUserInfo getTableVersion]];
    }else{
        return  [super modelGetValue:property];
    }
    
}
+(void)columnAttributeWithProperty:(LKDBProperty *)property
{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        property.defaultValue = [NSString stringWithFormat:@"%d",[NSUserInfo getTableVersion]];
    }
    //    if([property.propertyName isEqualToString:@"date"])
    //    {
    //        property.isUnique = YES;
    //        property.checkValue = @"MyDate > '2000-01-01 00:00:00'";
    //        property.length = 30;
    //    }
}

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"            :@"tableVer",
             @"_id"                  :@"_id",
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
 
+(NSString *)getPrimaryKey
{
    return @"_id";
}
+(int)getTableVersion
{
    return kDBVersion;
}
//表名
+(NSString *)getTableName
{
    return @"User";
}
////更新数据模型后，需要调用
//
//+(LKTableUpdateType)tableUpdateWithModelTableName:(NSString *)vTableName
//                                 andForOldVersion:(int)oldVersion
//                                       newVersion:(int)newVersion{
//    if (newVersion != oldVersion) {
//       
////        [self tableUpdateAddColumeWithModelTableName:vTableName andWithPN:@"tempArray"];
//    }
//    
//    return LKTableUpdateTypeCustom;
//    
//}
//+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithPN:(NSString*)propertyName{
//    
//    [super tableUpdateAddColumeWithModelTableName:vTableName andWithPN:propertyName];
//}

/** 给某个字段赋特定的值 */
//-(id)modelGetValue:(LKDBProperty *)property{
//    if ([property.propertyName isEqualToString:@"tempArray"]) {
//        return  nil;
//    }else{
//        return  [super modelGetValue:property];
//    }
//}


-(BOOL) isHaveNameRestrict
{
    BOOL ret = false;
    if(self.uid.length >= 3)
    {
        NSString *uidPostfix = [self.uid substringFromIndex:self.uid.length - 3];
        if([uidPostfix isPureInt])
        {
            int serverId = [uidPostfix intValue];
            uidPostfix = [NSString stringWithFormat:@"%d",serverId];
            if(self && self.userName.length > 0)
            {
                if ([self.userName hasPrefix:@"Empire"] && [self.userName hasSuffix:uidPostfix])
                    return true;
                else
                    return false;
            }
        }
        
    }
    
    return ret;
}
@end
