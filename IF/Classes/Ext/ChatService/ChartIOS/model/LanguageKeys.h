//
//  LanguageKeys.h
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import <Foundation/Foundation.h>

@interface LanguageKeys : NSObject
+(instancetype)lkShared;

@property (nonatomic, strong) NSString *TIP_JOIN_ALLIANCE;							//您未加入联盟，暂时无法使用联盟聊天频道
@property (nonatomic, strong) NSString *MENU_JOIN;									//加入
@property (nonatomic, strong) NSString *MENU_BAN;										//禁言
@property (nonatomic, strong) NSString *MENU_UNBAN;									//已禁言
@property (nonatomic, strong) NSString *TIP_BAN;										//是否禁言：{0}？
@property (nonatomic, strong) NSString *BTN_COUNTRY;									//国家
@property (nonatomic, strong) NSString *BTN_SEND;										//发送
@property (nonatomic, strong) NSString *MENU_COPY;									//复制
@property (nonatomic, strong) NSString *TIP_SENDMSG_WARN;								//您发布的聊天消息过于频繁，请稍等！
@property (nonatomic, strong) NSString *MENU_SENDMSG;									//发送邮件
@property (nonatomic, strong) NSString *MENU_VIEWPLAYER;								//查看玩家
@property (nonatomic, strong) NSString *MENU_SHIELD;									//屏蔽
@property (nonatomic, strong) NSString *TIP_SHIELD_PLAYER;							//是否要屏蔽{0}，屏蔽后你将不会收到该玩家的任何聊天信息和邮件，但是你可以通过设置来解除对该玩家的屏蔽。
@property (nonatomic, strong) NSString *MENU_UNSHIELD;									//解除屏蔽

@property (nonatomic, strong) NSString *TITLE_CHAT;									//聊天
@property (nonatomic, strong) NSString *TIP_TRANSLATED_BY;							//由{0}翻译
@property (nonatomic, strong) NSString *MENU_ORIGINALLAN;							//原文
@property (nonatomic, strong) NSString *TIP_DOWNLOADMORE;							//下滑加载更多
@property (nonatomic, strong) NSString *BTN_ALLIANCE;								//联盟
@property (nonatomic, strong) NSString *MENU_INVITE;									//邀请加入联盟
@property (nonatomic, strong) NSString *MENU_SHIELD_PLAYERCHAT;						//屏蔽该玩家留言

@property (nonatomic, strong) NSString *MENU_SHIELD_ALLIANCECHAT;					//屏蔽该联盟留言
@property (nonatomic, strong) NSString *TIP_SHIELD_PLAYER_CHATTO_ALLIANCE;			//是否要屏蔽{0}，屏蔽后该玩家将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该玩家的屏蔽。
@property (nonatomic, strong) NSString *TIP_SHIELD_ALLIANCE_CHATTO_ALLIANCE;			//是否要屏蔽{0}，屏蔽后该联盟将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该联盟的屏蔽。
@property (nonatomic, strong) NSString *TITLE_ALLIANCEMSG;							//联盟留言
@property (nonatomic, strong) NSString *TIP_SYSTEM;									//系统
@property (nonatomic, strong) NSString *TIP_INVITE;									//我邀请了{0}加入我们的联盟，希望他能和我们一起并肩作战。
@property (nonatomic, strong) NSString *MENU_BATTLEREPORT;							//查看战报
@property (nonatomic, strong) NSString *TIP_BATTLEREPORT;							//抱歉，该战报已无法查看

@property (nonatomic, strong) NSString *TIP_ADDALLIANCE_REWARD;						//立即加入联盟获得金币
@property (nonatomic, strong) NSString *TIP_ADDALLIANCE_COIN;						//{0}金币
@property (nonatomic, strong) NSString *TIP_ADDALLIANCE_REWARD_SENDBYMAIL;			//通过邮件发送

@property (nonatomic, strong) NSString *MENU_TRANSLATE;								//翻译
@property (nonatomic, strong) NSString *TIP_PULLDOWN;								//下拉可加载更多
@property (nonatomic, strong) NSString *TIP_NOPULLDOWN;

@property (nonatomic, strong) NSString *FLYHINT_DOWN_MIN;                           //服务器即将在{0}分后停机更新
@property (nonatomic, strong) NSString *FLYHINT_DOWN_SECOND;                        //服务器即将在{0}秒后停机更新

@property (nonatomic, strong) NSString *BTN_JOIN_NOW;

@property (nonatomic, strong) NSString *BTN_CONFIRM;                               //确定
@property (nonatomic, strong) NSString *BTN_CANCEL;
@property (nonatomic, strong) NSString *TIP_LOADING;
@property (nonatomic, strong) NSString *TIP_REFRESH;
@property (nonatomic, strong) NSString *TIP_ALLIANCE;
@property (nonatomic, strong) NSString *TITLE_MAIL;
@property (nonatomic, strong) NSString *TITLE_FORUM;
@property (nonatomic, strong) NSString *MENU_DETECTREPORT;
@property (nonatomic, strong) NSString *TIP_TIME;
@property (nonatomic, strong) NSString *TIP_RESEND;

@property (nonatomic, strong) NSString *TIP_USEITEM;                                    //发送王国公告将消耗1个 {0}！
@property (nonatomic, strong) NSString *TIP_HORN;                                       //号角
@property (nonatomic, strong) NSString *TIP_HORN_TEXT;                                  //公告
@property (nonatomic, strong) NSString *TIP_ITEM_NOT_ENOUGH;                            //领主大人，您的 {0} 不足，花费一些金币即可发送王国公告
@property (nonatomic, strong) NSString *TIP_CORN_NOT_ENOUGH;                            //领主大人，您的金币不足，赶快去购买一些吧！
@property (nonatomic, strong) NSString *TITLE_RANK1;                                    // 联盟阶级1
@property (nonatomic, strong) NSString *TITLE_RANK2;                                    // 联盟阶级2
@property (nonatomic, strong) NSString *TITLE_RANK3;                                    // 联盟阶级3
@property (nonatomic, strong) NSString *TITLE_RANK4;                                    // 联盟阶级4
@property (nonatomic, strong) NSString *TITLE_RANK5;                                    // 联盟阶级5

@property (nonatomic, strong) NSString *VIP_INFO;										// VIP{0}
@property (nonatomic, strong) NSString *TIP_CHATROOM_KICK;								//是否将{0}从群组中移除?
@property (nonatomic, strong) NSString *TIP_CHATROOM_INVITE;							//是否将{0}加入群组?
@property (nonatomic, strong) NSString *TIP_CHATROOM_QUIT;								//是否确认退出群组?
@property (nonatomic, strong) NSString *TIP_CHATROOM_MODIFYNAME;							//是否将群组改名为{0}？
@property (nonatomic, strong) NSString *NEW_MESSAGE_ALERT;								//{0}条新消息
@property (nonatomic, strong) NSString *TIP_NEW_MESSAGE_BELOW;							//以下为新消息
@property (nonatomic, strong) NSString *TITLE_CHATROOM;									//多人聊天
@property (nonatomic, strong) NSString *TIP_CHATROOM_CREATER;							//创建者
@property (nonatomic, strong) NSString *BTN_QUIT_CHATROOM;								//退出群组
@property (nonatomic, strong) NSString *TIP_MODIFY_CHATROOM_NAME;						//变更名称
@property (nonatomic, strong) NSString *BTN_SEARCH;										//搜索
@property (nonatomic, strong) NSString *TIP_SELECTED_MEMBER;							//已加入
@property (nonatomic, strong) NSString *TIP_SEARCH_RESULT;								//搜索结果
@property (nonatomic, strong) NSString *TIP_SEARCH_ThisCheck;                           //本次勾选
@property (nonatomic, strong) NSString *TIP_EQUIP_SHARE;                                //我刚刚在铁匠铺在成功的锻造出了{0}
@property (nonatomic, strong) NSString *MENU_VIEW_EQUIPMENT;                            //查看装备
@property (nonatomic, strong) NSString *MENU_REPORT_HEADIMG;                            //举报头像
@end
static LanguageKeys * languageKeys = nil;