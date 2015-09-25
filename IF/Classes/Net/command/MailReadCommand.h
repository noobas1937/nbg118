//
//  MailReadCommand.h
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#ifndef IF_MailReadCommand_h
#define IF_MailReadCommand_h

#define MAIL_READ "mail.read"
#include "CommandBase.h"
#include "MailController.h"

class MailReadCommand : public CommandBase{
public:
    MailReadCommand(std::string uid, std::string type) : m_uid(uid), CommandBase(MAIL_READ), m_isFromChat(false){
        m_type = type;
        putParam("uid", CCString::create(uid));
        putParam("type", CCString::create(type));
    }
    
    void setFromChat();
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);

private:
    std::string m_uid;
    std::string m_type;
    bool m_isFromChat;
};

#define MAIL_READ_STATUS "mail.read.status"
class MailReadStatusCommand : public CommandBase{
public:
    MailReadStatusCommand(std::string uid, int type) : m_uid(uid), CommandBase(MAIL_READ_STATUS){
        m_type = type;
        putParam("uid", CCString::create(uid));
        putParam("type", CCInteger::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
    int m_type;
};

#define MAIL_READ_BATCH "mread.batch"
class MailReadBatchCommand : public CommandBase{
public:
    MailReadBatchCommand(int type ,int num, int offset): CommandBase(MAIL_READ_BATCH){
        m_num = num;
        m_offset = offset;
        m_type = type;
        putParam("num", CCInteger::create(num));
        putParam("offset", CCInteger::create(offset));
        putParam("type", CCInteger::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_type;
    int m_num;
    int m_offset;
};

#define MAIL_DIALOG_READ_BATCH "mread.chat"
class MailDialogReadBatchCommand : public CommandBase{
public:
    MailDialogReadBatchCommand(string fromUser,string mailUid,int num, int offset): CommandBase(MAIL_DIALOG_READ_BATCH){
        m_num = num;
        m_offset = offset;
        m_fromUser = fromUser;
        m_mailUid = mailUid;
        int type = 0;
        auto mail = GlobalData::shared()->mailList.find(mailUid);
        if(mail != GlobalData::shared()->mailList.end()){
            if(mail->second->type == MAIL_MOD_SEND || mail->second->type == MAIL_MOD_PERSONAL){
                type = 9;
            }
        }
        if(type != 0){
            putParam("type", CCInteger::create(type));
        }
        putParam("num", CCInteger::create(num));
        putParam("offset", CCInteger::create(offset));
        putParam("fromUser", CCString::create(fromUser));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    string m_fromUser;
    string m_mailUid;
    int m_num;
    int m_offset;
};

#define MAIL_DIALOG_TRANSLATE  "mail.translate"

class MailDialogTranslateCommand : public CommandBase{
public:
    MailDialogTranslateCommand(std::string uids,std::string refUids): CommandBase(MAIL_DIALOG_TRANSLATE),m_refUIDs(refUids){
        putParam("uids", CCString::create(uids));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_refUIDs;
};
#endif
