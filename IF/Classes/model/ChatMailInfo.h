//
//  ChatMailInfo.h
//  IF
//
//  Created by xupengzhan on 15-4-2.
//
//用于通过JNI批量传递聊天或者邮件消息的类

#ifndef __IF__ChatMailInfo__
#define __IF__ChatMailInfo__

#include "cocos2d.h"
#include "ChatInfo.h"
#include "MailInfo.h"
#include "MailDialogInfo.h"
#include "CCCommonUtils.h"
#include "MailController.h"

class ChatMailInfo:public cocos2d::CCNode{
    
public:
    static ChatMailInfo* create(ChatInfo chatInfo,bool isNewMsg);
    static ChatMailInfo* create(MailInfo* mailInfo,MailDialogInfo* mailDialogInfo,bool isNewMsg,int msgType=CHANNEL_TYPE_USER);
    ChatMailInfo();
    ChatMailInfo(ChatInfo chatInfo,bool isNewMsg);
    ChatMailInfo(MailInfo* mailInfo,MailDialogInfo* mailDialogInfo,bool isNewMsg,int msgType=CHANNEL_TYPE_USER);
protected:
    ~ChatMailInfo();
    
public:
    std::string id;             //邮件使用的用来标识每一封邮件的id
    bool isNewMsg;      //是否是新消息
    int channelMsgType;		//频道消息类型，“0”表示国家，“1”表示联盟,"2"表示邮件,"3"表示聊天室,"4"表示系统邮件
    std::string msg;		//消息体
    std::string translateMsg;	//翻译信息
    std::string time;		//时间
    time_t createTime;		//时间
    std::string fromName;   //邮件对方姓名
    std::string channelId;     //聊天室ID
    std::string name;		//发送者名称
    std::string vip;		//vip信息
    std::string headPic;	//头像链接
    int headPicVer;
    std::string customHeadPicUrl;      //自定义头像http地址
    std::string customHeadPic;      //自定义头像
    bool isCustomHeadPicExist;      //自定义头像是否存在本地
    bool isSelfMsg;	//是否是自己的信息
    std::string uid;		//uid发送者uid
    std::string asn;		//联盟简称
    int post;		//是否为系统信息，“0”表示不是，非“0”表示是
    int gmod;		//gm和mod信息，如果为"2"、"4"、"5"表示为mod，如果为“3”表示为gm，
    //当为mod时名字颜色为(65,119,41)，如果为gm时名字颜色为(33,111,169),否则名字颜色为(111,26,0)
    //mod为"2"、"4"、"5"时对应的图片分别为mod.png、tmod.png、smod.png，gm为3时对应的图片为gm.png，其他的不显示mod和gm
    std::string allianceId;	//联盟ID
    std::string reportUid;		//战报UID
    std::string detectReportUid;	//侦察战报UID
    int equipId;        //装备ID
    std::string originalLang;	//源语言
    int hornItemId;         //喇叭itemId
    std::string sendLocalTime; //本地发送时间戳
    int sequenceId;
    int lastUpdateTime;
    string teamUid;         //集结队伍ID
    string lotteryInfo; //抓盘分享ID
    string redPackets;      //红包ID
    int server;     //红包服务器ID
};

#endif /* defined(__IF__ChatMailInfo__) */
