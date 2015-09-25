//
//  PushSettingCommand.cpp
//  IF
//
//  Created by zhaohf on 14-4-11.
//
//

#include "PushSettingCommand.h"

bool PushSettingCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PUSH_SETTING_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }

    if (params->objectForKey("errorCode")) {
        // todo: parse errorCode
        callFail(NetResult::create(Error_Network_Lost));
    } else {
        callSuccess(NetResult::create());
    }
    
    return true;
}