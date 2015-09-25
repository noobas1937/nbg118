//
//  AllianceLeaderReplaceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-4.
//
//

#include "AllianceLeaderReplaceCommand.h"

bool AllianceLeaderReplaceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_REPLACE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}