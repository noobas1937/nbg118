//
//  AllianceRallyListCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-13.
//
//

#include "AllianceRallyListCommand.h"

bool AllianceRallyListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_REF_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}