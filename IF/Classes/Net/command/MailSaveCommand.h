//
//  MailSaveCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-27.
//
//

#ifndef __IF__MailSaveCommand__
#define __IF__MailSaveCommand__

#include "CommandBase.h"
#include "CommonInclude.h"

#define MAIL_SAVE_COMMAND "mail.save"
#define MAIL_CANCEL_SAVE_COMMAND "mail.cancel.save"

class MailSaveCommand: public CommandBase {
public:
    MailSaveCommand(std::string uid, int type):
    m_uid(uid),
    m_type(type),
    CommandBase(MAIL_SAVE_COMMAND)
    {
        putParam("uid", CCString::create(uid));
        putParam("type", CCInteger::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uid;
    int m_type;
};



class MailCancelSaveCommand: public CommandBase {
public:
    MailCancelSaveCommand(std::string uid, int type):
    m_uid(uid),
    m_type(type),
    CommandBase(MAIL_CANCEL_SAVE_COMMAND)
    {
        putParam("uid", CCString::create(uid));
        putParam("type", CCInteger::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uid;
    int m_type;
};

#endif /* defined(__IF__MailSaveCommand__) */
