//
//  ChatServiceBridge.h
//  IF
//
//  Created by mzc on 15/4/3.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ChatViewController.h"
#import "MailViewController.h"
#import "BBSIOSViewController.h"

#import "CSMailChattingVC.h"

#define IOS_CHANNEL_TYPE_COUNTRY    0
#define IOS_CHANNEL_TYPE_ALLIANCE   1
#define IOS_CHANNEL_TYPE_USER       2
#define IOS_CHANNEL_TYPE_CHATROOM   3
#define IOS_CHANNEL_TYPE_SYSTEMMAIL 4
#define IOS_CHANNEL_TYPE_BBS        5
#define IOS_CHANNEL_TYPE_MAILLIST   6

#define CSCONTAINS(str1, str2) ([str1 rangeOfString: str2 ].location != NSNotFound)

typedef enum{
    OpenChatIOS_Normal,
    OpenChatIOS_Jump
}OpenChatState;

@interface ServiceInterface : NSObject
@property (nonatomic,assign) int mGmod;
@property (nonatomic,strong) NSString *userName;
@property (nonatomic,strong) NSString * uid;
@property (nonatomic,strong) NSString * headPic;
@property (nonatomic,strong) NSString * vipInfo;
@property (nonatomic,assign) int m_curChatType;
@property (nonatomic,assign) BOOL isInMailDialog;
@property (nonatomic,assign) BOOL rememberPosition;

///**是否第一次进入ios聊天。有一些数据需要重新初始化*/
//@property (nonatomic,assign) BOOL isFirstOpenIOS;

@property (nonatomic,strong) UIWindow *vc_win;
/**自定义根window*/
@property (nonatomic,strong)UIWindow *chatRootWindow;
@property (nonatomic,strong) ChatViewController *chatViewController;
@property (nonatomic,strong) MailViewController *mailViewController;

@property (nonatomic,assign)CSMailChattingVC *mailChatVC ;
@property (nonatomic,strong) BBSIOSViewController *bbsIOSViewController;
///**是否加载国家数据*/
//@property (nonatomic,assign) BOOL isLoadCountriesData;
///**是否加载联盟数据*/
//@property (nonatomic,assign) BOOL isLoadAllanceData;
/**是否是第一次进入聊天频道*/
@property (nonatomic,assign) BOOL isToOpenZheChat;
/**是否锁屏,用于做平后不在显示数据*/
@property (nonatomic,assign) BOOL isLockScreen;
//记录锁屏后的时间点
@property (nonatomic,assign) long screenLockTime;

/**是否返回时打开ios聊天视图*/
@property (nonatomic,assign) BOOL isReturnOpengIOS;

@property float autoSizeScaleX;

@property float autoSizeScaleY;

@property float autoSizeScaleWidth;

@property float autoSizeScaleHeight;
@property (assign,nonatomic) CGFloat  chatSystemSize;
//聊天字体
@property float chatFontSize;
//时间字体
@property float timefontSize;
//红包时间字体
@property float redPackageTimeSize;

//用户信息字体
@property float userInfofontSize;
//用户信息字体适配调整
@property float userInfoX;
//聊天结束时间字体
@property float endtimeFontSize;
//通用字体
@property float fontSize;
//2dx弹出提示框字体
@property float font2dxSize;
/**
 *  红包适配配置
 */
@property float redPackageTimePosY;
@property float redEnvelopeFont;
@property float redEnvelopeFont2; //点击领取 点击查看 红包使用改属性

//不同尺寸屏幕x值
@property float x;

//文本信息x的值
@property float contentX;

//列表section字体大小
@property (nonatomic,assign) CGFloat sectionFontSize;



/**发送喇叭是否是第一次扣除道具*/
@property (nonatomic,assign) BOOL isFirstDeductRadioCount;

/**发送喇叭是否是第一次扣除金币*/
@property (nonatomic,assign) BOOL isFirstDeductRadioMoney;

//zyt
@property (nonatomic,assign)CGFloat chatLabel_icon_size_width ;
@property (nonatomic,assign)CGFloat chatLabel_icon_size_height;

@property (nonatomic,assign)CGFloat chatLabel_icon_size_width_1 ;
@property (nonatomic,assign)CGFloat chatLabel_icon_size_height_1;
@property (nonatomic,assign)CGFloat chatLabel_LINE_HEIGHT;
@property (nonatomic,assign)CGFloat mail_native_nameLabelSize;
@property (nonatomic,assign)CGFloat mail_native_contentsLabelSize;
@property (nonatomic,assign)CGFloat mail_native_timeLabelSize;

//获取屏幕适配
-(void) screenAdaptation;

-(void)showChatIOSFrom2dx:(int)chatType withAddState:(OpenChatState) ocs;

-(void) openBBS;
/**接收信息*/
-(void)refreshData:(ResponseMsgType) rmt withAdd:(ChatChannel *) cc;

//单例
+(instancetype)serviceInterfaceSharedManager;

//设置用户信息
-(void) setPlayerInfo:(int)gmod headPicVer:(int)headPicVer customHeadImageUrl:(NSString*)customHeadImageUrl name:(NSString *)name uidStr:(NSString *)uidStr picStr:(NSString *)picStr vipStr:(NSString *)vipStr;

//设置邮件信息
-(void) setMailInfo:(NSString*) mailFromUid :(NSString*) mailUid :(NSString*) mailName :(int) mailType;

//设置联盟信息
-(void) setPlayerAllianceInfo:(NSString*) asnStr :(NSString*) allianceIdStr :(int) alliancerank :(BOOL) isFirstJoinAlliance;

-(void) selectOpenView:(int)chatType;

-(void) setM_curChatType:(int)m_curChatType;

//设置先上屏用户数据
-(NSMsgItem *) createChatMessage:(NSString*)msg;

//控制显示聊天界面是联盟还是国家
-(void) selectAsnORCountries;

//弹出提示窗
-(void) flyHint:(NSString*)icon :(NSString*)titleText :(NSString*)contentText :(CGFloat)time :(CGFloat)dy :(BOOL)useDefaultIcon;

////初始化国家，联盟 历史数据
//-(void) firstOpenChat;

/**初始化是否加载国家或联盟数据的条件变量*/
-(void) initIsLoadVariable;

/**判断 下拉刷新 打开ios聊天view 自动跳转 是否刷新到最新消息*/
@property (nonatomic,assign) BOOL isSrollNewMsg;

//持有原根控制器
@property (nonatomic,assign )NSObject *rootVC;

/**是否返回时打开ios聊天视图*/
@property (nonatomic,assign) BOOL isExecuteAllanceMemberInfo;

@property (nonatomic,assign) ResponseMsgType rmt;;

//应用进入后台瞬间记录数据库消息体顺序
@property (nonatomic,strong) NSString * countrySeqIDWithPause;
@property (nonatomic,strong) NSString * allianceSeqIDWithPause;
@property (nonatomic,strong) NSString * mailIDWithPause;
@property (nonatomic,strong) NSString * chatRoomCommandString;
@property (nonatomic,strong) NSString * mail_system_LastMailID;
@property (nonatomic,assign)  long  mail_system_lastCreateTime;

@property (nonatomic,strong)   dispatch_queue_t csChatMsgQueue;
@property (nonatomic,strong)   dispatch_queue_t csGetUserInfoQueue;

@property (nonatomic,strong) NSString * msgCommandString;

/**隐藏ios聊天视图*/
-(void) hideChatViewIOS;

/**显示ios聊天视图*/
-(void) showChatViewIOS;
/**
 *  新MailList视图隐藏
 */
-(void) hideMailListIOS;
/**
 *  新MailList视图显示
 */
-(void) showMailListIOS;

-(void)loadingChatVC;

-(void) updateChatCellFrame:(NSString*)uid;

/**刷新所有页面数据*/
-(void) reloadDataAll;

/**更改聊天室房间名*/
-(void) settingChatRoomName:(NSString*) roomName;

/**tabelview加载数据*/
-(void) tabelViewReloadData:(UITableViewController*) tvc withAdd:(ResponseMsgType) rmt withAdd:(ChatChannel*) cc;

/**初始化聊天时图和本地化类*/
-(void) initView;
/**
 *  判断设备版本是否是7.1
 *
 *  @return 判断设备版本是否是7.1
 */
-(BOOL) isSystemVersion7;

/** 关闭原生邮件列表，对象彻底释放 */
-(void)closed_The_OC_Native_mailVC;
 
-(void)openMail_OC_Native_Categary_ListWith3DTouch;

-(void)packageMsgCommand;
/**
 *  新版红包开关
 */
@property (nonatomic,assign) BOOL cs_CountryChat_OC_Native_New;

@property (nonatomic,assign) BOOL isChangeLanguage;
@end
static ServiceInterface * serviceInterface = nil;