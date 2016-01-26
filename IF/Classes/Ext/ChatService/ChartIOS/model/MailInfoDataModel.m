//
//  MailInfoDataModel.m
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import "MailInfoDataModel.h"
#import "UserManager.h"

#import "LKDBHelper.h"
@implementation MailInfoDataModel
-(NSString *)mailInfoDataForChannelWithSelf{
    NSString *channelId= @"";
    if (self.type == Mail_System) {
        if ([self.title isEqualToString:@"114111"] ||//114111=加入联盟奖励
            [self.title isEqualToString:@"105726"] ||//105726=青铜联盟礼物
            [self.title isEqualToString:@"105727"] ||//105727=黑铁联盟礼物
            [self.title isEqualToString:@"105728"] ||//105728=白银联盟礼物
            [self.title isEqualToString:@"105729"] ||//105729=黄金联盟礼物
            [self.title isEqualToString:@"105730"] ||//105730=传奇联盟礼物
            [self.title isEqualToString:@"115429"])//115429=联盟指令
        {
            channelId = Mail_ChannelID_Alliance;
        }else if ([self isWorldBossKillRewardMail] ||
                  [self isKnightActivityStartMail] ||
                  [self.title isEqualToString:@"150335"]){//150335=恭喜您！
            channelId = Mail_ChannelID_Event;
        }else{
            channelId = Mail_ChannelID_System;
        }
    }else if(self.type == Mail_Battle_Report){
        if ([self isKNightFightReport]) {
            //黑骑士战报
            channelId = Mail_ChannelID_Knight;
        }else if([self isKNightActivityEndMail]){
            channelId = Mail_ChannelID_Event;
        }
        else{
            //普通战报
            channelId = Mail_ChannelID_Fight;
        }
    
    }
    else{
        switch (self.type) {
 
            case Mail_Detect:
            case Mail_Detect_Report:
            case Mail_Encamp:
 
            case World_New_Explore:
            case World_Monster_Special:
                channelId = Mail_ChannelID_Fight;
                break;
            case Mail_Allianceinvaite:
            case Mail_Alliance_All:
            case Mail_Allianceapply:
            case Mail_Alliance_Kickout:
            case Mail_Invite_teleport:
            case Mail_Refuse_All_Apply:
            case Mail_Resource_Help:
            case Mail_Donate:
            case Mail_Allinace_Package:
            case Mail_Allinace_RankChange:
                channelId = Mail_ChannelID_Alliance;
                break;
            case All_Service:
            case Mail_Sysupdate:
                channelId = Mail_ChannelID_Studio;
                break;
            case Mail_Gift:
            case Mail_Sysnotice:
            case Mail_Service:
            case Mail_Fresher:
            case Mail_Wounded:
                channelId = Mail_ChannelID_System;
                break;
            case Mail_Attackmonster:
                channelId = Mail_ChannelID_Monster;
                break;
            case Mail_Resource:
                channelId = Mail_ChannelID_Resource;
                break;
            case Mail_World_boss:
                channelId = Mail_ChannelID_Event;
                break;
        }
    }
    return channelId;
}

//不全，只用于部分channelID获取
+(NSString *)channelStringWithMailType:(int)vMailType{
     NSString *channelId= @"";
    switch (vMailType) {
        case Mail_Battle_Report:
        case Mail_Detect:
        case Mail_Detect_Report:
        case Mail_Encamp:
        case Mail_World_boss:
        case World_New_Explore:
        case World_Monster_Special:
            channelId = Mail_ChannelID_Fight;
            break;
        case Mail_Allianceinvaite:
        case Mail_Alliance_All:
        case Mail_Allianceapply:
        case Mail_Alliance_Kickout:
        case Mail_Invite_teleport:
        case Mail_Refuse_All_Apply:
        case Mail_Resource_Help:
        case Mail_Donate:
        case Mail_Allinace_Package:
            channelId = Mail_ChannelID_Alliance;
            break;
        case All_Service:
        case Mail_Sysupdate:
            channelId = Mail_ChannelID_Studio;
            break;
        case Mail_Gift:
        case Mail_Sysnotice:
        case Mail_Service:
        case Mail_Fresher:
        case Mail_Wounded:
            channelId = Mail_ChannelID_System;
            break;
        case Mail_Attackmonster:
            channelId = Mail_ChannelID_Monster;
            break;
        case Mail_Resource:
            channelId = Mail_ChannelID_Resource;
            break;
    }
    return channelId;

}

/*
 旧逻辑废弃
-(void)mailInfoDataSettingTabType{
    if (self.type == Mail_Battle_Report || self.type == Mail_Detect || self.type == Mail_Detect_Report || self.type == Mail_Encamp || self.type == Mail_World_boss
        ) {
        self.tabType = Mail_BigType_Fight;
    }else if (self.type == All_Service ){
        self.tabType = Mail_BigType_Studio;
    }else if (self.type == Mail_Fresher || self.type == Mail_Sysnotice || self.type == Mail_Sysupdate ){
        self.tabType = Mail_BigType_Notice;
    }else if (self.type <= Mail_User || self.type == Mail_Alliance_All || self.type == Chat_Room){
        self.tabType = Mail_BigType_User;
    }else if (self.type == Mail_Mod_Send || self.type == Mail_Mod_Personal){
        if ([UserManager sharedUserManager].currentUser.mGmod == 2 || [UserManager sharedUserManager].currentUser.mGmod == 5) {
            self.tabType = Mail_BigType_Mod;
        }else{
            self.tabType = Mail_BigType_User;
        }
    }else if (self.type == Mail_Resource){
        self.tabType = Mail_ChannelID_Resource;
    }
    else if (self.type == Mail_Attackmonster){
        self.channelID = Mail_ChannelID_Monster;
    }
    else{
        self.tabType = Mail_BigType_System;
    }
    
}


-(void)mailInfoDataSettingChannelID{
    int switchKey = self.tabType;
    switch (switchKey) {
        case Mail_BigType_User:{
            
            if (self.type == Mail_Mod_Personal) {
                self.channelID = [NSString stringWithFormat:@"%@@mod",self.fromUserUid];
                
            }else{
                self.channelID = self.fromUserUid;
            }
        }
            break;
        case Mail_BigType_System:{
            self.channelID = Mail_ChannelID_System;
        }
            break;
        case Mail_BigType_Notice:{
            self.channelID = Mail_ChannelID_Notice;
        }
            break;
        case Mail_BigType_Studio:{
            self.channelID = Mail_ChannelID_Studio;
        }
            break;
        case Mail_BigType_Fight:{
            self.channelID = Mail_ChannelID_Fight;
        }
            break;
        case Mail_BigType_Mod:{
            self.channelID = Mail_ChannelID_Mod;
        }
            break;
        case Mail_BigType_Resource:{
            self.channelID = Mail_ChannelID_Resource;
        }
            break;
        default:
            break;
    }
    
    if (self.type == Mail_Resource) {
        self.channelID = Mail_ChannelID_Resource;
    }else if (self.type == Mail_Attackmonster){
        self.channelID = Mail_ChannelID_Monster;
    }else if (self.type == Mail_Resource_Help){
        self.channelID = Mail_ChannelID_Resourcehelp;
    }
}
*/

-(BOOL)isWorldBossKillRewardMail
{
    if(self.itemIdFlag == 1 && self.title.length > 0 && [self.title isEqualToString:@"137460"])//137460=世界BOSS消灭奖励
        return true;
    return false;
}
-(BOOL)isKnightActivityStartMail
{
    if (self.title.length> 0 && [self.title isEqualToString:@"133270"]){//133270=黑骑士来袭活动开始
        return true;
    }else{
          return false;
    }

}
-(BOOL)isKNightActivityEndMail{
    if (self.type == Mail_Battle_Report) {
        if ([self.contents rangeOfString:@"\"msReport\":1"].location != NSNotFound) {
            return YES;
        }
    }
    return NO;
}
-(BOOL)isKNightFightReport{
    if (self.type == Mail_Battle_Report) {
      
        if([self.contents rangeOfString:@"\"battleType\":6"].location !=NSNotFound)
        {
           return YES;
        }
        else
        {
             return NO;
        }
    }else{
        return NO;
    }
}

-(BOOL )isGrandCastleInvestigation{
    if (self.type == Mail_Detect_Report ) {
        if ([self.contents rangeOfString:@"\"pointType\":1"].location !=NSNotFound) {
            return YES;
        }else{
            return NO;
        }
    }else{
        return NO;
    }
}

#pragma mark -
#pragma mark DB

+(MailInfoDataModel *)gettingLastSystemMailFromDB{
    MailInfoDataModel *tempmodel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:nil orderBy:@"CreateTime desc"];
    tempmodel =  [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"CreateTime = %ld",tempmodel.creatTime] orderBy:@"_id desc"];
    if (tempmodel) {
        return tempmodel;
    }else{
        return nil;
    }
    
}

-(void)upDatingForDB{
    MailInfoDataModel *tempmodel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')",self.mail_ID] orderBy:@"_id asc"];
    if (tempmodel == nil) {
        [[DBManager sharedDBManager].dbHelper insertToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:self];
    }else{
        self._id = tempmodel._id;
        [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:self where:[NSString stringWithFormat:@"(ID = '%@')",self.mail_ID]];
    }
}
-(void)settingToReadForDB{
    MailInfoDataModel *tempModel = [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')",self.mail_ID] orderBy:@"_id asc"];
    
    if (tempModel ) {
        self.status = 1;
        [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:self where:[NSString stringWithFormat:@"(ID = '%@')",self.mail_ID]];

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
+(void)dbDidDeleted:(NSObject *)entity result:(BOOL)result{
    if (result) {
        LKLog(@"did Deleted Flag: 【%d】\n删除成功的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did Deleted Flag: 【%d】\n 删除失败的数据:%@",result,[entity printAllPropertys] );
    }
}
#endif//在类 初始化的时候
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

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"            :@"tableVer",
             @"_id"                  :@"_id"     ,
             @"ID"                  :@"mail_ID",
             @"ChannelID"           :@"channelID",
             @"FromUser"            :@"fromUserUid",
             @"FromName"            :@"fromName",
             @"Title"               :@"title",
             @"Contents"            :@"contents",
             @"RewardId"            :@"rewardId",//奖励
             @"ItemIdFlag"          :@"itemIdFlag",//title 或者 contents为多语言
             @"Status"              :@"status",//是否已读
             @"Type"                :@"type",
             @"MailRewardLevel"     :@"mailRewardLevel",//邮件奖励领取条件，比较主城等级
             @"RewardStatus"        :@"rewardStatus",//奖励领取状态
             @"SaveFlag"            :@"saveFlag", //锁定邮件
             @"CreateTime"          :@"creatTime",
             @"Reply"               :@"reply",
             @"Translation"         :@"translation",
             @"TitleText"           :@"nameText",
             @"Summary"             :@"contentText",
             @"Language"            :@"language",
             @"ParseVersion"        :@"parseVersion",//
             };
}

+(int)getTableVersion
{
    return kDBVersion;
}

+(NSString *)getPrimaryKey
{
    return @"_id";
}
+(NSString *)getTableName
{
    return @"Mail";
}

-(id)modelGetValue:(LKDBProperty *)property{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        return [NSString stringWithFormat:@"%d",[MailInfoDataModel getTableVersion]];
    }else{
        return  [super modelGetValue:property];
    }
    
}
+(void)columnAttributeWithProperty:(LKDBProperty *)property
{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        property.defaultValue = [NSString stringWithFormat:@"%d",[MailInfoDataModel getTableVersion]];
    }
    //    if([property.propertyName isEqualToString:@"date"])
    //    {
    //        property.isUnique = YES;
    //        property.checkValue = @"MyDate > '2000-01-01 00:00:00'";
    //        property.length = 30;
    //    }
}


@end