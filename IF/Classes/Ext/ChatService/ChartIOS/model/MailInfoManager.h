//
//  MailInfoManager.h
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import <Foundation/Foundation.h>
#import "MailInfo.h"
#import "MailInfoDataModel.h"



@interface MailInfoManager : NSObject

+(MailInfoManager *)sharedMailInfoManager;
+(MailInfoDataModel *)maildataForOnlinePushWithCCDictionary:(CCDictionary *)vDic;
+(MailInfoDataModel *)maildataWithMailInfo:(MailInfo *)vMailInfo;

+(MailInfoDataModel *)maildataWithCCDictionary:(CCDictionary *)vDic;

-(NSString*) gettingMailIconByName:(NSString*) name;


#pragma mark -
#pragma mark  服务器接口返回状态回调
//奖励领取
+(void)settingRewardStatusToReadWithMailIDArray:(NSArray *)vMailIdArray;
//邮件已读
+(void)settingUnReadStatusToReadWithMailIDArray:(NSArray *)vMailIdArray;
+(void)settingUnReadStatusToReadWith_MailType:(int )vMailType;
+(void)mailInfoDataFor_deleteWithMailID:(NSString *)vMailIdString;
+(void)mailInfoDataFor_deleteWithmailArray:(NSArray *)vMailArray;
//系统邮件数据库更新
+(void)mailInfoDataUpdateToDBWithMailDate:(MailInfoDataModel *)vMailInfoData;
//根据mailID 返回一个mailmodel
+(MailInfoDataModel *)mailInfoModelFromDBWithMailID:(NSString *)vMailIdString;
//更具邮件类型批量删除邮件
+(void)banchDeleteMailInfoWithMailType:(int )vType;
//更具邮件channelID批量删除邮件
+(void)banchDeleteMailInfoWIthMailChannelID:(NSString *)vChannelID;
//获取所有的主城战报战报
+(NSArray *)mainCityInvestigativeNewsAfterCreatTime:(int)vCreatTime;
+(NSArray *)gettingUnReadSystemMailTypeWithChannelID:(NSString *)vChannelID;
@end
