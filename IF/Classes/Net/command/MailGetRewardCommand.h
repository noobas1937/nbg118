//
//  MailGetRewardCommand.h
//  IF
//
//  Created by 李锐奇 on 14-1-8.
//
//

#ifndef __IF__MailGetRewardCommand__
#define __IF__MailGetRewardCommand__

#define MAIL_GET_REWARD "mail.reward"
#include "CommandBase.h"
#define MIAL_GET_REWARD_REFRESH "mailGetRewardRefresh"

class MailGetRewardCommand : public CommandBase{
public:
    MailGetRewardCommand(std::string uid, int type) : m_uid(uid),m_type(type), CommandBase(MAIL_GET_REWARD){
        putParam("uid", CCString::create(uid));
        putParam("type", CCInteger::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
    int m_type;
};

#endif /* defined(__IF__MailGetRewardCommand__) */
