//
//  ChatTestCommand.h
//  IF
//
//  Created by xupengzhan on 15-7-23.
//
//

#ifndef __IF__ChatTestCommand__
#define __IF__ChatTestCommand__

#include "CommandBase.h"

#define CHAT_ROOM_CREATE "chat.test"

class ChatTestCommand : public CommandBase{
public:
    ChatTestCommand() :
    CommandBase(CHAT_ROOM_CREATE){
        putParam("testsysmail", CCInteger::create(1));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ChatTestCommand__) */
