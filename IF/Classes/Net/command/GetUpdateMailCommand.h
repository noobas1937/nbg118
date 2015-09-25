//
//  GetUpdateMailCommand.h
//  IF
//
//  Created by xupengzhan on 15-7-23.
//
//

#ifndef __IF__GetUpdateMailCommand__
#define __IF__GetUpdateMailCommand__

#include "CommandBase.h"

#define CHAT_GET_UPDATE_MAILS "chat.get.update.mails"

class GetUpdateMailCommand : public CommandBase{
public:
    GetUpdateMailCommand(std::string time) :
    CommandBase(CHAT_GET_UPDATE_MAILS){
        putParam("time", CCString::create(time));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__GetUpdateMailCommand__) */
