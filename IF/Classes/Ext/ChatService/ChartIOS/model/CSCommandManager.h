//
//  CSCommandManager.h
//  IF
//
//  Created by 张彦涛 on 15/10/25.
//
//

#import <Foundation/Foundation.h>

@interface CSCommandManager : NSObject

+ (CSCommandManager *)sharedCommandManager;
-(void)commandStart;
/**
 *  国家联盟消息拉取
 *
 *  @param vCommandType   是应用启动请求，还是锁屏拉取
 *  @param vCommandString 请求的字符串
 */
-(void)getChatMailMsgWithCommandType:(int )vCommandType
                andWithCommandString:(NSString *)vCommandString;

-(void)settingreadSatusWithMailID:(NSString *)vMailIDString andWithMailTpye:(int)vMailTpye;

-(void)settingReadStatus_Chat_WithChatFromUid:(NSString *)vFromUid
                            andWithModFlag:(BOOL)vModFlag;

-(void)settingReadStatus_Banched_WithMailType:(int)vMailType
                               andWithModFlag:(BOOL)vModFlag
                             andWithMailTypes:(NSString *)vMailTypesString;
/**系统邮件删除*/
-(void)deletingMailDataWithMailID:(NSString *)vMailIDString;
/**通过传入邮件数组删除*/
-(void)deletingMailDataWithALlMailIDArray:(NSArray *)vMailIdArray;
/**批量领取奖励*/
-(void)settingRewardStatusWithMailDataArray:(NSArray *)vMailArray;
/** 单个聊天channel删除  */
-(void)deletingChatChannelWithChannelID:(NSString *)vChatChannelID;
/**
 *  多个聊天channl一块儿删除
 */
-(void)deletingChannelsWithChannelIDArray:(NSArray *)vMailArray;

-(BOOL)isConnected;
//系统邮件更具UID批量标记已读
-(void)settingReadStatus_mail_withMailArray:(NSArray *)vMailArray;
@end
