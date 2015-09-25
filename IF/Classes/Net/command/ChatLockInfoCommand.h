//
//  ChatLockInfoCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#ifndef __IF__ChatLockInfoCommand__
#define __IF__ChatLockInfoCommand__

#include "CommandBase.h"
#define CHAT_LOCK_INFO_COMMAND "chat.shield.list"

class ChatLockInfoCommand : public CommandBase{
public:
    ChatLockInfoCommand(): CommandBase(CHAT_LOCK_INFO_COMMAND){

    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ChatLockInfoCommand__) */
