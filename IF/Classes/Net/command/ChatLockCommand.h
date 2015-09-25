//
//  ChatLockCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#ifndef __IF__ChatLockCommand__
#define __IF__ChatLockCommand__

#include "CommandBase.h"
#define CHAT_LOCK_COMMAND "chat.lock"
#define CHAT_BAN_COMMAND "chat.ban"
#define CHAT_UNBAN_COMMAND "chat.unban"

class ChatLockCommand : public CommandBase{
public:
    ChatLockCommand(std::string uid): CommandBase(CHAT_LOCK_COMMAND){
        putParam("uid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class ChatBanCommand : public CommandBase{
public:
    ChatBanCommand(std::string uid,int time): CommandBase(CHAT_BAN_COMMAND){
        m_uid = uid;
        m_time = time;
        putParam("uid", CCString::create(uid));
        putParam("time", CCInteger::create(time));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string  m_uid;
    int     m_time;
};
class ChatUnBanCommand : public CommandBase{
public:
    ChatUnBanCommand(std::string uid): CommandBase(CHAT_UNBAN_COMMAND){
        m_uid = uid;
        putParam("uid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_uid;
};

#endif /* defined(__IF__ChatLockCommand__) */
