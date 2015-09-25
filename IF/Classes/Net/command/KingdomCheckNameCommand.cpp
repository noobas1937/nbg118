//
//  KingdomCheckNameCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#include "KingdomCheckNameCommand.h"

bool KingdomCheckNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(KINGDOM_CHECK_NAME) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}