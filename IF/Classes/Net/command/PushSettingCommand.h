//
//  PushSettingCommand.h
//  IF
//
//  Created by zhaohf on 14-4-11.
//
//

#ifndef __IF__PushSettingCommand__
#define __IF__PushSettingCommand__

#include "CommandBase.h"
#define PUSH_SETTING_COMMAND "parse.push.set"

class PushSettingCommand : public CommandBase {
public:
    PushSettingCommand(int type,int status, int sound):
    CommandBase(PUSH_SETTING_COMMAND)
    ,m_type(type),m_status(status)
    {
        putParam("type", CCInteger::create(type));
        putParam("status", CCInteger::create(status));
        putParam("audio", CCInteger::create(sound));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_type;
    int m_status;
};

#endif /* defined(__IF__PushSettingCommand__) */
