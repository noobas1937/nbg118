//
//  GameHost.h
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import <Foundation/Foundation.h>
#import "ChatMessage.h"
#import "ChatCellIOS.h"
@interface GameHostDemo : NSObject

-(void) setActionAfterResume:(NSString*) action :(NSString*) uid :(NSString*) name :(NSString*) reportUid :(NSString*) detectReportUid :(BOOL) returnToChatAfterPopup;
-(void) onResume:(int) chatType;



/**解除屏蔽玩家*/
-(void) unBlock:(NSString*) uid :(NSString*) name;

/**屏蔽玩家*/
-(void) block:(NSString*) uid :(ChatCellIOS*)cell;

/**解除禁言玩家*/
-(void) unBan:(NSString*) uid;

/**禁言玩家*/
-(void) ban:(ChatMessage*) chatMessage;

/**根据联盟id加入联盟*/
-(void) joinAlliance:(NSString*) allianceId :(NSString*)name;

/**根据x , y 跳转坐标位置*/
-(void) skipPos:(NSString*) posX :(NSString*) posY;

/**发送信息*/
-(void) sendText:(NSString*) msg :(int) m_curChatType;

/**发送广播*/
-(void) sendRadio:(ChatMessage *)chatMessage;

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
-(void) directlySendMsg:(ChatMessage *)chatMessage;

//初始化聊天20条历史数据
-(void) initChatToHistory;

//获取喇叭消费金币数额
-(int) radioCount;

//屏蔽玩家
-(void) shieldPlayer:(NSString*) uid;

//是否足够金币发送喇叭
-(BOOL) isSatisfySendRadio;

//发送喇叭向后台请求
-(void) sendNotice:(NSString *) msg :(int)itemid :(BOOL)usePoint :(NSString*) sendLocalTime ;

//禁言玩家
-(void) banPlayer:(NSString*)uid :(int)banTime;

-(void) sendRequestChatFromAndroid:(int)type;


//向c＋＋获取头像文件在本地的路径
-(NSString*) headPath:(NSString*) url;

//从网络下载头像
-(void) downHeadImage:(NSString *)url;

-(void) initChatToHistory_allance;

-(void) initChatToHistory_countries;
@end
