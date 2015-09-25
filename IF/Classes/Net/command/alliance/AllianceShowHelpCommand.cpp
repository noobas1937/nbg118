//
//  AllianceShowHelpCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-16.
//
//

#include "AllianceShowHelpCommand.h"

bool AllianceShowHelpCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_SHOWHELP) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params->objectForKey("helps"))));
    }
    return true;
}