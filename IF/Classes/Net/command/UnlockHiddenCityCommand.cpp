//
//  UnlockHiddenCityCommand.cpp
//  IF
//
//  Created by 邹 程 on 13-10-14.
//
//

#include "UnlockHiddenCityCommand.h"

bool UnlockHiddenCityCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(UNLOCK_HIDDEN_CITY_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    if (params->valueForKey("cityId")->intValue() != m_cityId) {
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