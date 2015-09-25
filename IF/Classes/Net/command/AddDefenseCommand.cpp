//
//  AddDefenseCommand.cpp
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#include "AddDefenseCommand.h"

bool AddDefenseCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ADD_DEFENSE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyHint("", "", _lang(params->valueForKey("errorCode")->getCString()));
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}