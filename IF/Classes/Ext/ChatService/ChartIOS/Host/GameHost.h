//
//  GameHost.h
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import <Foundation/Foundation.h>
#import "NSMsgItem.h"
#import "ChatCellIOS.h"
#import "GameHostInterface.h"
@interface GameHost : GameHostInterface

-(void) setActionAfterResume:(NSString*) action :(NSString*) uid :(NSString*) name :(NSString*) reportUid :(NSString*) detectReportUid :(int)equipId :(BOOL) returnToChatAfterPopup;
-(void) onResume:(int) chatType;



/**解除屏蔽玩家*/
-(void) unBlock:(NSString*) uid :(NSString*) name;

/**屏蔽玩家*/
-(void) block:(NSString*) uid :(NSString*)name;

/**禁言玩家*/
-(void) ban:(NSString*) uid :(NSString*)name :(int) banTime;

/**解除禁言玩家*/
-(void) unBan:(NSString*) uid :(NSString*)name;

/**根据联盟id加入联盟*/
-(void) joinAlliance:(NSString*) allianceId :(NSString*)name;

/**根据x , y 跳转坐标位置*/
-(void) skipPos:(NSString*) posX :(NSString*) posY;

/**发送信息*/
-(void) sendText:(NSMsgItem *) cm :(int) m_curChatType;

/**发送广播*/
-(void) sendRadio:(NSMsgItem *)chatMessage;

/**邀请加入联盟*/
-(void) invitejoin:(NSString *)uid :(NSString *)name;

/**查看战报*/
-(void) battleMsg:(NSString*)uid :(NSString*)reportUid;

/**查看侦查战报*/
-(void) detectMsg:(NSString*)uid :(NSString*)detectReportUid;

/**查看用户信息*/
-(void) showPlayerInfo:(NSString*)uid :(NSString*)name;

/**重新发送*/
-(void) resetSend:(ChatCellIOS*)cell;

/**先上屏*/
-(void) directlySendMsg:(NSMsgItem *)chatMessage;

//获取喇叭消费金币数额
-(int) radioCount;

//是否足够金币发送喇叭
-(BOOL) isSatisfySendRadio;

//发送喇叭向后台请求
-(void) sendNotice:(NSString *) msg :(int)itemid :(BOOL)usePoint :(NSString*) sendLocalTime ;

//向c＋＋获取头像文件在本地的路径
-(NSString*) headPath:(NSString*) url;

//从网络下载头像
-(void) downHeadImage:(NSString *)url;
//未读邮件变成已读邮件
-(void) updateMailList;
//发送邮件
-(void) sendMail:(NSMsgItem *)mi;

@end
