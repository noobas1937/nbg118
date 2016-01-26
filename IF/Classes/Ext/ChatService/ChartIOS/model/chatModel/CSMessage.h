//
//  CSMessage.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <Foundation/Foundation.h>
 

@interface CSMessage : NSObject


@property (nonatomic,assign) int        tableVer;
@property (nonatomic,assign)    long         _id;
/** 消息服务器序号  */
@property (nonatomic,assign) long          sequenceId;
/** 用来标识邮件的id */
@property (nonatomic,strong) NSString   * mailId;
/** 消息发送者uid  */
@property (nonatomic,strong) NSString   *uid;    // uid，群聊时才会存数据库

/** 聊天频道类型  0 国家 1 联盟 2 邮件 3 聊天室  4备用*/
@property (nonatomic,assign) int        channelType;   
@property (nonatomic,assign) long        createTime;    // 收到的消息会在C++中初始化此字段，对应后台传回来的createTime 作用于更新信息创建时间。针对别的玩家
/** 本地发送时间戳 作用于自己发送的信息状态对比。 */
@property (nonatomic,assign) long        sendLocalTime;
/** 数据库中名为type： “0”普通聊天消息，  2、联盟加入消息 3、联盟邀请消息 4战报 5侦查战报 6喇叭消息 7装备分享 8打招呼 9联盟集结 10轮盘分享 11联盟任务 12红包 200mod邮件 100 聊天室系统消息 
 */
@property (nonatomic,assign) int        post;
@property (nonatomic,strong) NSString   *msg;       // 消息体
@property (nonatomic,strong) NSString   *translateMsg;      // 翻译信息
@property (nonatomic,strong) NSString   *originalLang;        // 源语言国别类型
@property (nonatomic,strong) NSString   *translatedLang;        // 翻译的语言国别类型
@property (nonatomic,assign) int        sendState;    // 发送状态，0正在发送，1发送失败，2发送成功
@property (nonatomic,strong) NSString   *attachmentId;     // 战报UID，侦察战报UID post == 12 attachmentId格式为 红包ID_服务器号_红包状态
@property (nonatomic,strong) NSString   *media;  // 媒体资源

//@property (nonatomic,strong) NSString   *localLanString;   //被翻译的语言类型

@property (nonatomic,assign) int        readState;
@end
