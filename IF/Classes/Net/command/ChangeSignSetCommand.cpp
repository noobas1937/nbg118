//
//  ChangeSignSetCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#include "ChangeSignSetCommand.h"

bool ChangeSignSetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SIGN_SET) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        
        callSuccess(NetResult::create());
    }
    return true;
}