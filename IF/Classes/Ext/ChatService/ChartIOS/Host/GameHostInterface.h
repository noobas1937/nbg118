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
#import "MailData.h"
@class CSMessage;
@class  CSMessageModel;
@interface GameHostInterface : NSObject

-(void) setActionAfterResume:(NSString*) action :(NSString*) uid :(NSString*) name :(NSString*) reportUid :(NSString*) detectReportUid :(int)equipId :(BOOL) returnToChatAfterPopup;
-(void) onResume:(int) chatType;



/**解除屏蔽玩家*/
-(void) unBlock:(NSString*) uid :(NSString*) name;

/**屏蔽玩家*/
-(void) block:(NSString*) uid :(NSString*)name;

/**解除禁言玩家*/
-(void) unBan:(NSString*) uid :(NSString*)name;

/**禁言玩家*/
-(void) ban:(NSString*) uid :(NSString*)name :(int) banTime;

/**根据联盟id加入联盟*/
-(void) joinAlliance:(NSString*) allianceId :(NSString*)name;

/**根据x , y 跳转坐标位置*/
-(void) skipPos:(NSString*) posX :(NSString*) posY;

/**发送信息*/
-(void) sendText:(NSMsgItem *) msg :(int) m_curChatType;
-(void)sendTextWithChatMessage:(CSMessage *)vMsg andWithSendMsgType:(int )vSendType;
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
-(void) sendMailWithMessage:(CSMessage *)mi;
/**
 *  发送聊天室数据
 *
 *  @param msg     信息
 *  @param groupId 聊天室ID
 */
-(void) sendChatRoomMsg:(NSString*) msg :(NSString*) groupId sendLocalTime:(NSString*)sendLocalTime;

/**下拉请求历史数据seqId 方式*/
-(void) requestActionHistoryMsg:(NSString*) channelId;

//第一次打开邮件
-(void) openMailDialogViewFirst:(NSString*)fromUid :(NSString*) fromName;
//更新c++ 界面联盟或国家频道的显示
-(void) updateChatInfo:(int)type;
/** 新的数据模型方法 */
-(void)refreshChatInfoWithCSMessage:(CSMessageModel*)mi;

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
/**
 *  查看装备
 *
 *  @param equipId 装备ID
 */
-(void)viewEquipment:(NSString *)equipId;
/**
 *  举报头像
 *
 *  @param uid 举报对象的uid
 */
-(void)reportCustomHeadPic:(NSString*) uid;

/**
 *  弹出cocosUI
 *
 *  @param mailData cocosUI所需的数据
 */
-(void) popUpCocosMailfByData:(MailData *)mailData;

/**
 *  返回解析JSON后的结果
 *
 *  @param uid 邮件uid
 *
 *  @return 字典
 */
-(NSDictionary*) result2JsonWithAddUid:(NSString*)uid;

/**
 *  获取采集资源icon
 *
 *  @param type 类型
 *  @param val  值
 *
 *  @return 图片名称
 */
-(NSString*) gettingPicByType:(int) type withVal:(int) val;

/**
 *  举报内容
 *
 *  @param uid      内容所属用户的uid
 *  @param content  内容
 */
-(void) contentofReportWithUid:(NSString*)uid withContent:(NSString*)content;
/**
 *  是否开启自动翻译
 
 */
-(BOOL) isAutoTranslate;

-(NSString*) gettingNameById:(NSString*) defId;

-(NSString*) gettingPropById:(NSString*) defId ByLeve:(NSString*) defName;

/**
 *  获取邮件头像
 *
 *  @param name name
 *
 *  @return 
 */
-(NSString *)gettingMailIconByName:(NSString *)name;

-(NSString*) gettingPointByOccupyIdx:(int) occupyId;

-(NSString*) gettingPointByMapIndex:(int) occupyPointId ByType:(int) serverType;

/**
 *  集结信息接口
 */
-(void) viewRallyInfoByStr:(NSString*) teamUid;

/**
 *  轮盘分享
 
 */
-(void) viewLotteryInfoById:(NSString*) lotteryInfo;
/**
 *  获取本地化语言
 
 */
-(NSArray*) gettingChatLangArray;

/**
 *  打开领主改名界面
 */
-(void) changeNickName;

-(void) gettingFriendsList;
/**
 *  查看红包
 */
-(void) viewRedPackage;

/**
 *  参数版本号和本地客户端版本比较
 *
 *  @param version 版本号
 
 */
-(BOOL) checkVersionByVersion:(NSString*) version;
/**
 *  打开好友视图
 */
-(void) openFriendsView;
/**
 *  获取红包状态
 *
 *  @param redPackageUid 红包id
 *  @param serverId      服务器号
 */
-(void) gettingRedPackageStatusById:(NSString*) redPackageUid withServerId:(NSString*)serverId;
/**
 *  查询喇叭数量是否大于0
 
 */
-(BOOL) isPopUpRadioConsumptionTip;
/**
 *  查看发送喇叭金币是否足够
 
 */
-(BOOL) isPopUpMoneyConsumptionTip;

-(int) gettingPlayerLevel;

-(void) showYesDialogByStr:(NSString*) dialog;


/** 喇叭禁言 */
-(void)banPlayerNoticeWithUserUid:(NSString *)vUidString andWithBanTimeLever:(int)vTimeLever;
/** 取消喇叭禁言 */
-(void)closeBanPlayNoticeWithUserUid:(NSString *)vUidString;
@end
