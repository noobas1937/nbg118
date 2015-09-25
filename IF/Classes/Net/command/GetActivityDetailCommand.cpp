//
//  GetActivityDetailCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "GetActivityDetailCommand.h"

bool GetActivityDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ACTIVITY_DETAIL) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}