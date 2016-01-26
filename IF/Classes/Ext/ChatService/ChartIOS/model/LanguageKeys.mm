//
//  self.m
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import "LanguageKeys.h"

@implementation LanguageKeys

-(LanguageKeys*)init
{
    self = [super init];
    if (self) {
        self.TIP_JOIN_ALLIANCE = @"E100068";							//您未加入联盟，暂时无法使用联盟聊天频道
        self.MENU_JOIN = @"115020";									//加入
        self.MENU_BAN = @"105207";										//禁言
        self.MENU_UNBAN = @"105209";									//已禁言
        self.TIP_BAN = @"105210";										//是否禁言：{0}？
        self.BTN_COUNTRY = @"105300";									//国家
        self.BTN_SEND = @"105302";										//发送
        self.MENU_COPY = @"105304";									//复制
        self.TIP_SENDMSG_WARN = @"105307";								//您发布的聊天消息过于频繁，请稍等！
        self.MENU_SENDMSG = @"105308";									//发送邮件
        self.MENU_VIEWPLAYER = @"105309";								//查看玩家
        self.MENU_SHIELD = @"105312";									//屏蔽
        self.TIP_SHIELD_PLAYER = @"105313";							//是否要屏蔽{0}，屏蔽后你将不会收到该玩家的任何聊天信息和邮件，但是你可以通过设置来解除对该玩家的屏蔽。
        self.MENU_UNSHIELD = @"105315";									//解除屏蔽
        
        self.TITLE_CHAT = @"105316";									//聊天
        self.TIP_TRANSLATED_BY = @"105321";							//由{0}翻译
        self.MENU_ORIGINALLAN = @"105322";							//原文
        self.TIP_DOWNLOADMORE = @"105502";							//下滑加载更多
        self.BTN_ALLIANCE = @"105602";								//联盟
        self.MENU_INVITE = @"108584";									//邀请加入联盟
        self.MENU_SHIELD_PLAYERCHAT = @"115922";						//屏蔽该玩家留言
        
        self.MENU_SHIELD_ALLIANCECHAT = @"115923";					//屏蔽该联盟留言
        self.TIP_SHIELD_PLAYER_CHATTO_ALLIANCE = @"115925";			//是否要屏蔽{0}，屏蔽后该玩家将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该玩家的屏蔽。
        self.TIP_SHIELD_ALLIANCE_CHATTO_ALLIANCE = @"115926";			//是否要屏蔽{0}，屏蔽后该联盟将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该联盟的屏蔽。
        self.TITLE_ALLIANCEMSG = @"115929";							//联盟留言
        self.TIP_SYSTEM = @"115181";									//系统
        self.TIP_INVITE = @"115182";									//我邀请了{0}加入我们的联盟，希望他能和我们一起并肩作战。
        self.MENU_BATTLEREPORT = @"115281";							//查看战报
        self.TIP_BATTLEREPORT = @"115282";							//抱歉，该战报已无法查看
        
        self.TIP_ADDALLIANCE_REWARD = @"115168";						//立即加入联盟获得金币
        self.TIP_ADDALLIANCE_COIN = @"115169";						//{0}金币
        self.TIP_ADDALLIANCE_REWARD_SENDBYMAIL = @"115170";			//通过邮件发送
        
        self.MENU_TRANSLATE = @"105326";								//翻译
        self.TIP_PULLDOWN = @"105327";								//下拉可加载更多
        self.TIP_NOPULLDOWN = @"105328";								//松开载入更多
        
        self.FLYHINT_DOWN_MIN = @"105324";								//服务器即将在{0}分后停机更新
        self.FLYHINT_DOWN_SECOND = @"105325";								//服务器即将在{0}秒后停机更新
        
        self.BTN_JOIN_NOW = @"115068";								//立即加入
        
        self.BTN_CONFIRM = @"confirm";								//确定
        self.BTN_CANCEL = @"cancel_btn_label";								//取消
        self.TIP_LOADING = @"114110";								//加载中
        self.TIP_REFRESH=@"104932";								//刷新
        self.TIP_ALLIANCE=@"105564";								//全体联盟成员
        self.TITLE_MAIL=@"101205";								//邮件
        self.TITLE_FORUM=@"105329";								//论坛
        self.MENU_DETECTREPORT=@"105522";								//侦察战报
        self.TIP_TIME=@"105591";									//小时
        self.TIP_RESEND=@"105330";									//是否重发此消息
        
        self.TIP_HORN_TEXT = @"105331";                         //公告
        self.TIP_USEITEM = @"105332";                           //发送王国公告将消耗1个 {0}！
        self.TIP_HORN = @"104371";                              //号角
        self.TIP_ITEM_NOT_ENOUGH = @"105333";                   //领主大人，您的 {0} 不足，花费一些金币即可发送王国公告！
        self.TIP_CORN_NOT_ENOUGH = @"104912";                   //领主大人，您的金币不足，赶快去购买一些吧！
        
        self.TITLE_RANK1 = @"115100";
        self.TITLE_RANK2 = @"115101";
        self.TITLE_RANK3 = @"115102";
        self.TITLE_RANK4 = @"115103";
        self.TITLE_RANK5 = @"115104";
        
        self.VIP_INFO = @"103000";			// VIP
        self.TIP_CHATROOM_KICK = @"105348";			//是否将{0}从群组中移除?
        self.TIP_CHATROOM_INVITE = @"105349";			//是否将{0}加入群组?
        self.TIP_CHATROOM_QUIT = @"105350";			//是否确认退出群组?
        self.TIP_CHATROOM_MODIFYNAME = @"105351";			//是否将群组改名为{0}？
        self.NEW_MESSAGE_ALERT = @"105352";			//{0}条新消息
        self.TIP_NEW_MESSAGE_BELOW = @"105353";			//以下为新消息
        self.TITLE_CHATROOM = @"105354";			//多人聊天
        self.TIP_CHATROOM_CREATER = @"105355";			//创建者
        self.BTN_QUIT_CHATROOM = @"105344";			//退出群组
        self.TIP_MODIFY_CHATROOM_NAME = @"119004";			//变更名称
        self.BTN_SEARCH = @"113907";			//搜索
        self.TIP_SELECTED_MEMBER = @"105356";			//已加入
        self.TIP_SEARCH_RESULT = @"105357";			//搜索结果
        self.TIP_SEARCH_ThisCheck = @"4100011";     //本次勾选
        self.TIP_EQUIP_SHARE = @"111660";           //我刚刚在铁匠铺在成功的锻造出了{0}
        self.MENU_VIEW_EQUIPMENT = @"111665";       //查看装备
        self.MENU_REPORT_HEADIMG = @"105777";       //举报头像
    }
    return self;
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

+ (instancetype)lkShared
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        languageKeys = [[LanguageKeys alloc] init];
    });
    return languageKeys;
}

+(id)allocWithZone:(struct _NSZone *)zone{
    //调用dispatch_once保证在多线程中也只被实例化一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        languageKeys = [super allocWithZone:zone];
    });
    return languageKeys;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}


@end