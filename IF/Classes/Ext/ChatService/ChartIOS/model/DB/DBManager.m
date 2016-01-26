//
//  DBManager.m
//  IF
//
//  Created by 张彦涛 on 15/7/27.
//
//

#import "DBManager.h"
#import "NSUserInfo.h"
#import "ChatChannel.h"
#import "ChannelManager.h"
#import "NSString+Extension.h"
#import "MailInfoDataModel.h"
#import "ServiceInterface.h"
/**
 * UserID和Type的取值枚举
 * 								(后缀不同是为了防止id冲突)
 * 1.玩家（会与消息关联）			UserID:玩家id				Type:2
 * 2.国家频道						UserID:国家号@country		Type:0
 * 3.联盟频道						UserID:联盟id@alliance		Type:1
 * 4.群							UserID:群id@chatroom			Type:3
 * 5.官方信息（类似公众号）			UserID:频道id@official		Type:4
 * 					更新公告		UserID:announce@official        Type:4
 * 					系统邮件		UserID:update@official		Type:4
 * 					战斗报告		UserID:report@official		Type:4
 */

const NSString * DBManager_ID_POSTFIX_COUNTRY   = @"@country";
const NSString * DBManager_ID_POSTFIX_ALLIANCE  = @"alliance";
const NSString * DBManager_ID_POSTFIX_USER      = @"";
const NSString * DBManager_ID_POSTFIX_CHATROOM  = @"@chatroom";
const NSString * DBManager_ID_POSTFIX_OFFICIAL  = @"@official";
//const NSString * DBManager_ID_POSTFIX_MOD       = @"@mod";

const int DBManager_TYPE_POSTFIX_COUNTRY    = 0;
const int DBManager_TYPE_POSTFIX_ALLIANCE   = 1;
const int DBManager_TYPE_POSTFIX_USER       = 2;
const int DBManager_TYPE_POSTFIX_CHATROOM   = 3;
const int DBManager_TYPE_POSTFIX_OFFICIAL   = 4;

@implementation DBManager


+ (DBManager *)sharedDBManager
{
    static DBManager *sharedDBManagerInstance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedDBManagerInstance = [[self alloc] init ];
    });
    return sharedDBManagerInstance;
}

-(instancetype)init{
    self = [super init];
    if (self ) {
//        LKDBHelper *helper = [[LKDBHelper alloc]initWithCustomNameDB];
//        _dbHelper = helper;
        [self settingTable];
    }
    
    return self;
}
-(LKDBHelper *)dbHelper {
    if (_dbHelper  == nil) {
        _dbHelper = [[LKDBHelper alloc]initWithCustomNameDB];
    }
    return _dbHelper;
}
-(void)removeDBManager{
    _dbHelper = nil;
 
}
-(void)settingTable{
 
    int channelCount = [self.dbHelper rowCountWithModelClassName:[ChatChannel getTableName] withModelClass:[ChatChannel class] where:nil];
    if (channelCount > 0) {
        ChatChannel *tempChannel = [_dbHelper searchSingleWithModelTableName:[ChatChannel getTableName] andWithModelClass:[ChatChannel class] where:nil orderBy:@"_id desc"];
        if (tempChannel.tableVer <2) {
            [_dbHelper dropAllTable];
        }
    }
 
    
}

#pragma mark -
#pragma mark 消息表名生成

-(NSString *)gettingChatTableNameWithChatChannel:(ChatChannel *)vChatChannel{
    if (vChatChannel && vChatChannel.channelID.length> 0 ) {
        NSString *nameString =@"";
        nameString = [nameString stringByAppendingString:vChatChannel.channelID];
        nameString =[nameString stringByAppendingString:[DBManager gettingTypeDefineWithChannelType:vChatChannel.channelType]];
        
        nameString = [@"Chat_" stringByAppendingString:[nameString md5]];
        
        
        return nameString;

    }else{
 
        
        return @"";

    }
}
+(NSString *)gettingTypeDefineWithChannelType:(int )vTpye{
    switch (vTpye) {
        case DBManager_TYPE_POSTFIX_COUNTRY:{
            return DBManager_ID_POSTFIX_COUNTRY;
        }break;
        case DBManager_TYPE_POSTFIX_ALLIANCE:{
            return DBManager_ID_POSTFIX_ALLIANCE;
        }break;
        case DBManager_TYPE_POSTFIX_USER:{
            return DBManager_ID_POSTFIX_USER;
        }break;
        case DBManager_TYPE_POSTFIX_CHATROOM:{
            return DBManager_ID_POSTFIX_CHATROOM;
        }break;
        case DBManager_TYPE_POSTFIX_OFFICIAL:{
            return DBManager_ID_POSTFIX_OFFICIAL;
        }break;
        default:{
            return @"";
        }
            break;
    }
}

+(NSString *)dbSqlWithChannelID:(NSString *)vChannelIdStr{
    NSArray *typesArr =[ChannelManager channelTypesWithChannelId:vChannelIdStr];
    if (typesArr.count > 0) {
        NSString *sql = @"";
        NSString *temp = @"";
        NSArray *specailTitle =@[@"114111",@"105726",@"105727",@"105728",@"105729",@"105730",@"115429"];
        NSString  * allianceSpecialSql = @"";
         NSString  * systemSpecialSql = @"";
         NSString  * knightSpecialSql =  @"Contents LIKE '%\"battleType\":6%'";
         NSString  * fightSpecialSql =   @"Contents NOT LIKE '%\"battleType\":6%' AND Contents NOT LIKE '%\"msReport\":1%'";
        NSString * kNightActivitySpecialSql = @"Contents LIKE '%\"msReport\":1%'";
        for (int x  = 0; x<specailTitle.count; x++) {
            if (x > 0) {
                allianceSpecialSql = [allianceSpecialSql stringByAppendingString:@" OR "];
                systemSpecialSql = [systemSpecialSql stringByAppendingString:@" AND "];
            }
            allianceSpecialSql = [allianceSpecialSql stringByAppendingFormat:@"Title = '%@'",specailTitle[x]];
            systemSpecialSql = [systemSpecialSql stringByAppendingFormat:@"Title <> '%@'",specailTitle[x]];
        }
        if (systemSpecialSql.length > 0) {
            systemSpecialSql = [systemSpecialSql stringByAppendingFormat:@" AND Title <> '137460' AND Title <> '133270' AND Title <> '150335'" ];
        }else{
            systemSpecialSql = [systemSpecialSql stringByAppendingFormat:@" Title <> '137460' AND Title <> '133270' AND Title <> '150335'" ];
        }
        
        for (int i= 0; i < typesArr.count; i++) {
            int type = [typesArr[i] intValue];
            if (type > 0) {
                if (i > 0) {
                    temp = [temp stringByAppendingString:@" OR "];
                }
                if ([vChannelIdStr isEqualToString:Mail_ChannelID_Alliance] &&
                     type == Mail_System &&  allianceSpecialSql.length > 0)
                {
                    temp = [temp stringByAppendingFormat:@" ( Type = '%d' AND (%@))",type ,allianceSpecialSql];
                }else if ([vChannelIdStr isEqualToString:Mail_ChannelID_System] &&
                          type == Mail_System &&  systemSpecialSql.length> 0)
                {
                    temp = [temp stringByAppendingFormat:@" ( Type = '%d'  AND (%@))",type ,systemSpecialSql];
                }else if ([vChannelIdStr isEqualToString:Mail_ChannelID_Fight] &&
                          type == Mail_Battle_Report && fightSpecialSql.length > 0)
                {
                    temp = [temp stringByAppendingFormat:@"( Type = '%d' AND (%@))",type,fightSpecialSql];
                }else if ([vChannelIdStr isEqualToString:Mail_ChannelID_Knight ] &&
                          type == Mail_Battle_Report && knightSpecialSql.length > 0)
                {
                    temp = [temp stringByAppendingFormat:@"( Type = '%d' AND (%@))",type ,knightSpecialSql];
                }
                else if ([vChannelIdStr isEqualToString:Mail_ChannelID_Event ] &&
                         type == Mail_Battle_Report && kNightActivitySpecialSql.length > 0)
                {
                    temp = [temp stringByAppendingFormat:@"( Type = '%d' AND (%@))",type ,kNightActivitySpecialSql];
                }else{
                    temp =[temp stringByAppendingFormat:@" Type = '%d' ",type];
                }
                
            }
        }
        if ([vChannelIdStr isEqualToString:Mail_ChannelID_Event]) {
            if (temp.length > 0) {
                temp = [temp stringByAppendingFormat:@" OR ( Type = '%d' AND (Title = '137460' OR Title = '133270' OR Title = '150335'))",Mail_System];
            }else{
                temp = [temp stringByAppendingFormat:@"(Type = '%d' AND (Title = '137460' OR Title = '133270' OR Title = '150335'))",Mail_System];
            }
        }
        sql = [NSString stringWithFormat:@"(%@)",temp];
        return sql;
        
    }else{
        return @"";
    }
}

@end
