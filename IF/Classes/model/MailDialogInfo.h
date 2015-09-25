//
//  MailDalogInfo.h
//  IF
//
//  Created by lifangkai on 14-9-19.
//
//

#ifndef __IF__MailDialogInfo__
#define __IF__MailDialogInfo__

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;

class MailDialogInfo : public CCNode{
public:
    static MailDialogInfo* create();
    void parse(CCDictionary* dic);
    void addOneDialog(CCDictionary* dic);
    MailDialogInfo():uid(""),fromUid(""),toUid(""),contents(""),pic(""),isShowTime(true),translationMsg(""),originalLang(""),isShowOrginal(true),title(""),sequenceId(0),lastUpdateTime(0),tipFlag(-1),picVer(0),senderName(""),vip(0),vipEndTime(0),asn(""),allianceId(""),status(0){};
    ~MailDialogInfo();
    std::string uid;        //dialog person uid
    time_t createTime;
    int type;
    std::string fromUid;              //两人之间总的通话数
    std::string toUid;
    std::string contents;
    std::string pic;
    int picVer;
    bool isShowTime;
    std::string translationMsg;
    std::string originalLang;
    std::string sendLocalTime;
    std::string title;
    int sequenceId;
    int lastUpdateTime;
    int tipFlag;    //是否是tip消息
    bool isShowOrginal;
    
    //聊天室使用，暂时的
    std::string senderName;     //发送者名称
    int vip;                                    //vip等级
    long vipEndTime;                     //vip结束时间
    std::string asn;                            //联盟简称
    std::string allianceId;                            //联盟ID
    int status;
    
};

#endif /* defined(__IF__MailDalogInfo__) */
