//
//  RebuildCommand.cpp
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#include "RebuildCommand.h"

bool RebuildCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(REBUILD_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));

    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}