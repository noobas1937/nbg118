//
//  ActivityScoreHistoryCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-3.
//
//

#include "ActivityScoreHistoryCommand.h"

bool ActivityScoreHistoryCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ACTIVITY_SCORE_HISTORY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool ActivityLastHistoryCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ACTIVITY_LAST_HISTORY) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}