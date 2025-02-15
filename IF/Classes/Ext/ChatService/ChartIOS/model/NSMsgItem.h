//
//  NSMsgItem.h
//  IF
//
//  Created by mzc on 15/6/25.
//
//

#import <Foundation/Foundation.h>
#import "NSUserInfo.h"
#define SENDING 0
#define SEND_FAILED 1
#define SEND_SUCCESS 2

typedef void(^MsgUserBlock)(NSUserInfo *userInfo) ;
@interface NSMsgItem : NSObject

/**数据库对应*/
@property (nonatomic,assign) int        dbId;
@property (nonatomic,assign) int        tableVer;
/** 消息服务器序号  */
@property (nonatomic,assign) int        sequenceId;
/** 用来标识邮件的id */
@property (nonatomic,strong) NSString * mailId;
/** 消息发送者uid  */
@property (nonatomic,strong) NSString   *uid;                               // uid，群聊时才会存数据库
/**
 *  0 国家 1 联盟 2 邮件 3 聊天室  4备用
 */
@property (nonatomic,assign) int        channelType;                        // 频道类型
@property (nonatomic,assign) int        createTime;                         // 收到的消息会在C++中初始化此字段，对应后台传回来的createTime 作用于更新信息创建时间。针对别的玩家
@property (nonatomic,assign) int        post;                               // 数据库中名为type：是否为系统信息，“0”表示不是，非“0”表示是 100 聊天室系统消息 6喇叭消息
@property (nonatomic,strong) NSString   *msg;                               // 消息体
@property (nonatomic,strong) NSString   *translateMsg;                      // 翻译信息
@property (nonatomic,strong) NSString   *originalLang;                      // 源语言国别类型
@property (nonatomic,strong) NSString   *translatedLang;                    // 翻译的语言国别类型
@property (nonatomic,assign) int        sendState;                          // 发送状态，0正在发送，1发送失败，2发送成功
@property (nonatomic,strong) NSString   *attachmentId;                      // 战报UID，侦察战报UID
@property (nonatomic,strong) NSString   *media;                             //// 媒体资源
@property (nonatomic,strong) NSString   *localLanString;                    //被翻译的语言类型

/**运行时属性*/
@property (nonatomic,assign) BOOL       isSelfMsg;                          // 是否是自己的信息
@property (nonatomic,assign) BOOL       isNewMsg;                           // 是否是新消息

@property (nonatomic,strong) NSString   *currentText;
@property (nonatomic,assign) BOOL       hasTranslated;                      // 是否被翻译过
@property (nonatomic,assign) BOOL       isSendDataShowed;
@property (nonatomic,assign) int        lastUpdateTime;
@property (nonatomic,assign) int       sendLocalTime;                      // 本地发送时间戳 作用于自己发送的信息状态对比。
@property (nonatomic,assign) BOOL       isSelfisTranslateByGoogleMsg;
@property (nonatomic,assign) BOOL       isFirstNewMsg;
@property (nonatomic,assign) int        sendfirstNewMsgStateState;          // 0:不是第一条 1:第一条且新消息数小于等于200条 2:第一条且新消息数超过200条

@property (assign,getter= isShowTransLate,nonatomic) BOOL  showTransLate; //当前是否显示翻译



@property (nonatomic,strong) NSString   *name;                              // 发送者名称
@property (nonatomic,strong) NSString   *asn;                               // 联盟简称
@property (nonatomic,strong) NSString   *vip;                               // vip信息
@property (nonatomic,strong) NSString   *headPic;                           // 系统头像
@property (nonatomic,assign) int        gmod;
@property (nonatomic,assign) int        headPicVer;                         // 自定义头像
@property (nonatomic,strong) NSString   *showMsg;                           // 显示到屏幕上的信息 根据函数显示原文或者译文
@property (nonatomic,strong) NSString   *showUserInfoLabel;                 // 显示到屏幕上的用户信息
@property (nonatomic,strong) NSString   *endMsg;                            // 显示到屏幕上的结尾信息 如:00:00 + 翻译信息

@property (nonatomic, strong)NSString *customHeadPicPath;       //自定义头像路径
@property (nonatomic, strong)NSString *customHeadPicUrl;        //自定义头像URL
//@property (nonatomic, assign)UIImage  *gettingCustomHeadPicImg; //获取自定义头像img
@property (nonatomic, strong)NSString *gmodStr;                 //自定义头像URL

-(NSUserInfo*)  gettingUserInfo;                                //获取用户信息
-(NSString*)    gettingServerId;
-(NSString*)    gettingAsn;
-(NSString*)    gettingHeadPic;
-(int)          gettingGmod;
-(int)          HeadPicVer;
-(BOOL)         isInAlliance;                                               //是否拥有联盟
-(NSString*)    gettingNSUserInfoLabel;                                     //获取用于显示用户信息的label
-(BOOL)         isSystemMessage;                                            //判断是否是系统系统消息
-(NSString*)    gettingSendTimeFormat;                                            //获取发送信息时间
-(void)         packedMsg;                                                  //信息组装成IOS显示需要的信息
-(NSString*)    gettingGmodStr;                                             //根据gmod值设置gmod图标名称
-(NSString*)    gettingEndMsg;                                              //根据时间 ＋ 翻译信息组装成需要显示的结尾信息

-(NSString*) nsDataSwitchNSString:(NSUInteger) time;                        //时间戳转字符串

-(BOOL) isBattleReport;
-(BOOL) isDetectReport;
-(BOOL) isAnnounceInvite;
-(void) updateMsg;
-(BOOL) playerIsVip;

-(void) initUserForReceivedMsg;
-(void)initMsgUserWithBlock:(MsgUserBlock)vBlock;
//作用于发送信息
-(instancetype) initSend:(NSString*) uidStr isNewMsg:(BOOL) isNewMsg isSelf:(BOOL) isSelf channelType:(int) channelType post:(int) post msgStr:(NSString*) msgStr sendLocalTime:(NSString*) sendLocalTime;

-(NSString*) gettingShowMsg;

 
-(void)settingShowMsg:(NSString *)vlocalLanString;
-(void)settingMsgUserNickString;

@end
