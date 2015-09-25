//
//  MailSendCommand.h
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#ifndef IF_MailSendCommand_h
#define IF_MailSendCommand_h

#define MAIL_SEND "mail.send"
#include "CommandBase.h"
#include "MailController.h"

#define MAIL_GIFT_SEND "mail.gift.send"

class MailSendCommand : public CommandBase{
public:
    MailSendCommand(std::string toName, std::string title, std::string contents, std::string allianceId="", std::string mailUid="",bool isflag = false, int type = MAIL_SELF_SEND,std::string thxMail="",bool showTip = true,std::string sendLocalTime = "",std::string targetUid="") :
    m_name(toName)
    , m_title(title)
    , m_contents(contents)
    , m_allianceId(allianceId)
    ,m_mailUid(mailUid)
    ,m_type(type)
    ,m_thanksMail(thxMail)
    ,m_showTip(showTip)
    ,m_sendLocalTime(sendLocalTime)
    , CommandBase(MAIL_SEND){
        putParam("name", CCString::create(toName));
        putParam("title", CCString::create(title));
        putParam("contents", CCString::create(contents));
        putParam("allianceId", CCString::create(allianceId));
        if(targetUid!="")
            putParam("targetUid", CCString::create(targetUid));
        if(isflag){
            putParam("getUnread", CCInteger::create(1));
        }
        if(sendLocalTime != ""){
            putParam("sendLocalTime", CCString::create(sendLocalTime));
        }
        if(type == MAIL_MOD_PERSONAL || type == MAIL_MOD_SEND){
            putParam("type", CCInteger::create(MAIL_MOD_SEND));
        }
        if(!thxMail.empty()){
            putParam("thanksUid", CCString::create(thxMail));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_name;
    std::string m_title;
    std::string m_contents;
    std::string m_allianceId;
    std::string m_mailUid;
    std::string m_thanksMail;
    std::string m_sendLocalTime;
    bool m_showTip;
    int m_type;
};


class MailGiftSendCommand : public CommandBase{
public:
    MailGiftSendCommand(std::string toName, int consum_item, int gift_item, std::string contents,std::string uids = "", int gift_count=0) :
    m_name(toName)
    , m_contents(contents)
    , m_consum_id(consum_item)
    ,m_giftid(gift_item)
    ,m_uids(uids)
    ,m_giftcount(gift_count)
    , CommandBase(MAIL_GIFT_SEND){
        putParam("name", CCString::create(toName));
        putParam("contents", CCString::create(contents));
        putParam("consum_item",CCString::create(CC_ITOA(consum_item)));
        putParam("gift_item", CCString::create(CC_ITOA(gift_item)));
        if(!uids.empty()){
            putParam("uids", CCString::create(uids));
        }
        if(gift_count > 0){
            putParam("gift_count", CCInteger::create(gift_count));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_name;
    std::string m_contents;
    int m_consum_id;
    int m_giftid;
    std::string m_uids;
    int m_giftcount;
};
#endif
