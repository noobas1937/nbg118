//
//  DBManager.h
//  IF
//
//  Created by 张彦涛 on 15/7/27.
//
//

#import <Foundation/Foundation.h>
#import "LKDBHelper.h"
@class ChatChannel;
@interface DBManager : NSObject


+ (DBManager *)sharedDBManager;

@property (nonatomic,strong)  LKDBHelper * dbHelper;
/**
 *  拼接表名
 *
 *  @param vChatChannel chatChannel
 *
 *  @return 返回表名字符串
 */
-(NSString *)gettingChatTableNameWithChatChannel:(ChatChannel *)vChatChannel;
-(void)removeDBManager;

+(NSString *)dbSqlWithChannelID:(NSString *)vChannelIdStr;
@end
