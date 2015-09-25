//
//  CheckNickNameCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#include "CheckNickNameCommand.h"

bool CheckNickNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USER_CHECK_NICK_NAME) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));//->valueForKey("exist")
    }
    return true;
}