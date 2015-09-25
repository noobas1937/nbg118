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
#import "NSString+Extension.h"

#import "ServiceInterface.h"
/**
 * UserID和Type的取值枚举
 * 								(后缀不同是为了防止id冲突)
 * 1.玩家（会与消息关联）			UserID:玩家id				Type:2
 * 2.国家频道						UserID:国家号@country		Type:0
 * 3.联盟频道						UserID:联盟id@alliance		Type:1
 * 4.群							UserID:群id@chatroom			Type:3
 * 5.官方信息（类似公众号）			UserID:频道id@official		Type:4
 * 					更新公告		UserID:announce@official	Type:4
 * 					系统邮件		UserID:update@official		Type:4
 * 					战斗报告		UserID:report@official		Type:4
 */

const NSString * DBManager_ID_POSTFIX_COUNTRY   = @"@country";
const NSString * DBManager_ID_POSTFIX_ALLIANCE  = @"alliance";
const NSString * DBManager_ID_POSTFIX_USER      = @"";
const NSString * DBManager_ID_POSTFIX_CHATROOM  = @"@chatroom";
const NSString * DBManager_ID_POSTFIX_OFFICIAL  = @"@official";
const NSString * DBManager_ID_POSTFIX_MOD       = @"@mod";

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
        sharedDBManagerInstance = [[self alloc] init];
    });
    return sharedDBManagerInstance;
}

-(instancetype)init{
    self = [super init];
    if (self ) {
        LKDBHelper *helper = [[LKDBHelper alloc]initWithCustomNameDB];
        _dbHelper = helper;
        [self settingTable];
    }
    
    return self;
}

-(void)settingTable{
    [_dbHelper createTableWithModelClassTableName:[NSUserInfo getTableName] andModelClass:[NSUserInfo class]];
    [_dbHelper createTableWithModelClassTableName:[ChatChannel getTableName] andModelClass:[ChatChannel class]];
}

#pragma mark -
#pragma mark 消息表名生成

-(NSString *)gettingChatTableNameWithChatChannel:(ChatChannel *)vChatChannel{
    NSString *nameString =@"";
    nameString = [nameString stringByAppendingString:vChatChannel.channelID];
    nameString =[nameString stringByAppendingString:[DBManager gettingTypeDefineWithChannelType:vChatChannel.channelType]];
  
        nameString = [@"Chat_" stringByAppendingString:[nameString md5]];
   
    
    return nameString;
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

@end
