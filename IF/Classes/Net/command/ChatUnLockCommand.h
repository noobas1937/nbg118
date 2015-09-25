//
//  ChatUnLockCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#ifndef __IF__ChatUnLockCommand__
#define __IF__ChatUnLockCommand__

#include "CommandBase.h"
#define CHAT_UNLOCK_COMMAND "chat.unlock"

class ChatUnLockCommand : public CommandBase{
public:
    ChatUnLockCommand(std::string uuid): CommandBase(CHAT_UNLOCK_COMMAND){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ChatUnLockCommand__) */
