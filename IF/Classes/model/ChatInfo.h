//
//  ChatInfo.h
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#ifndef IF_ChatInfo_h
#define IF_ChatInfo_h

#include "CommonInclude.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
#define CHAT_NOTICE_SYS_DIALOG "3000002"
class ChatInfo{
    
public:
    ChatInfo(CCDictionary* dict);
    ChatInfo():
    uid(""),
    type(0),
    name(string("")),
    msg(string("")),
    time(0),
    senderPic(string("")),
    isNew(true),
    asn(""),
    vip(0),
    translationMsg(string("")),
    originalLang(""),
    post(0),
    gmFlag(0),
    banTime(0),
    banGmName(""),
    isTimeShowed(false),
    showedTime(""),
    allianceId(string("")),
    senderAllianceId(string("")),
    reportAtt(string("")),
    reportDef(string("")),
    reportLang(string("")),
    reportUid(string("")),
    reportAttAbbr(string("")),
    reportDefAbbr(string("")),
    itemId(0),
    senderPicVer(0),
    sequenceId(0),
    lastUpdateTime(0),
    equipId(0)
    {}
    string uid;
    int type;
    string name;
    string msg;
    time_t time;
    std::string senderPic;
    bool isNew;
    string asn;
    int vip;
    string translationMsg;
    string originalLang;
    int post;//是否为系统消息
    int gmFlag;//是否是mod
    time_t banTime;  //禁言时间
    string banGmName;//执行禁言的名称
    bool isTimeShowed;//是否在聊天已经显示过时间
    string showedTime;//已显示的时间，防止加载more时，把时间合并，引起显示变化。
    bool showOriginal;
    string officer;//官职
    string allianceId;//联盟ID
    string senderAllianceId;//写评论者的联盟id
    int stype;//写评论者为个人还是联盟
    int itemId; // 大喇叭使用的道具id
    int senderPicVer;//是否使用自定义头像
    string sendLocalTime; //本地发送时间戳
    int sequenceId;
    int lastUpdateTime;
    
    //战报分享
    string reportAtt;
    string reportDef;
    string reportLang;
    string reportUid;
    string reportAttAbbr;
    string reportDefAbbr;
    
    //侦查分享
    string detectRepUid;
    //装备分享
    int equipId;
};
#endif /* defined(__IF__ChatInfo__) */
