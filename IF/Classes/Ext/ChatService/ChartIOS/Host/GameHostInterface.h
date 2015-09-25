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
@interface GameHostInterface : NSObject

-(void) setActionAfterResume:(NSString*) action :(NSString*) uid :(NSString*) name :(NSString*) reportUid :(NSString*) detectReportUid :(BOOL) returnToChatAfterPopup;
-(void) onResume:(int) chatType;



/**解除屏蔽玩家*/
-(void) unBlock:(NSString*) uid :(NSString*) name;

/**屏蔽玩家*/
-(void) block:(NSString*) uid :(NSString*)name;

/**解除禁言玩家*/
-(void) unBan:(NSString*) uid;

/**禁言玩家*/
-(void) ban:(NSString*) uid :(NSString*)name :(int) banTime;

/**根据联盟id加入联盟*/
-(void) joinAlliance:(NSString*) allianceId :(NSString*)name;

/**根据x , y 跳转坐标位置*/
-(void) skipPos:(NSString*) posX :(NSString*) posY;

/**发送信息*/
-(void) sendText:(NSMsgItem *) msg :(int) m_curChatType;

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

//初始化聊天20条历史数据
-(void) initChatToHistory;

//获取喇叭消费金币数额
-(int) radioCount;

//屏蔽玩具
-(void) shieldPlayer:(NSString*) uid;

//是否足够金币发送喇叭
-(BOOL) isSatisfySendRadio;

//发送喇叭向后台请求
-(void) sendNotice:(NSString *) msg :(int)itemid :(BOOL)usePoint :(NSString*) sendLocalTime ;

//禁言玩家
-(void) banPlayer:(NSString*)uid :(int)banTime;

//向c＋＋获取头像文件在本地的路径
-(NSString*) headPath:(NSString*) url;

//从网络下载头像
-(void) downHeadImage:(NSString *)url;

//未读邮件变成已读邮件
-(void) updateMailList;

-(void) initChatToHistory_allance;

-(void) initChatToHistory_countries;

//发送邮件
-(void) sendMail:(NSMsgItem *)mi;

/**下拉请求历史数据seqId 方式*/
-(void) requestActionHistoryMsg:(NSString*) channelId;

//第一次打开邮件
-(void) openMailDialogViewFirst:(NSString*)fromUid :(NSString*) fromName;
//更新c++ 界面联盟或国家频道的显示
-(void) updateChatInfo:(int)type;
///**加载国家数据*/
//-(void) requestCountriesData;
///**加载联盟数据*/
//-(void) requestAllanceData;
///**判断从本地还是服务器加载联盟数据*/
//-(BOOL) isfromServiceOrLocal_allance;
///**判断从本地还是服务器加载国家数据*/
//-(BOOL) isfromServiceOrLocal_courtries;

///**从新请求数据*/
//-(void) getGameDataAfterInit;

/**点击按钮加入联盟*/
-(void) joinAllianceBtnClick;

/**C++ 频道切换为国家*/
-(void) changeCountry;

-(NSMutableArray*) getUserInfoArray:(int) index;

-(void) getMultiUserInfo:(NSString*)ns_uidsStr;
//选择群聊成员
-(void) selectChatRoomMember:(NSString*) roomName :(NSString*)memberName :(NSString*) memberUid;
/**获取c++聊天数据*/
-(NSArray*) getChatInfoArray:(int)chatInfoNo :(NSString*)msgTypeStr;

/**存储初始化信息*/
-(void) saveInitChatMsg:(NSArray*) chatMessages :(NSString*)chatMessageId;


-(void)addGroupChatMemberWithGroupID:(NSString *)vGroupID
               andWithMemberUidArray:(NSArray  *)vMemberUidArray
                   andWithMemberName:(NSString *)vMemberName;
-(void)subGroupChatMemberWithGroupID:(NSString *)vGroupID
               andWithMemberUidArray:(NSArray  *)vMemberUidArray
                   andWithMemberName:(NSString *)vMemberName;

-(void)reNameGroupChatTitleWithGroupID:(NSString *)vGroupID andWithGroupName:(NSString *)vNameString;
-(void)quitGroupChatWithGroupID:(NSString *)vGroupId;

/**自定义头像网络URL*/
-(NSString*) gettingCustomHeadPic:(NSString*) uid int:(int) picVer;

/*根据输入的字符串查询用户*/
-(void)gettingUsersWithSearchString:(NSString *)vSearchString ;

/**获取被翻译语言的类型*/
-(NSString*) gettingLocalLanString;

-(void) close2MailList;

-(void) refreshChatInfo:(NSMsgItem*)mi;

-(void) gettingChatRoomInfo:(NSString*) chatRoomUid;

-(void) gettingServiceChannelNewMsg:(ChatChannel*)cc;

-(BOOL) gettingServiceChannelOldMsg:(ChatChannel*)cc;

@end
