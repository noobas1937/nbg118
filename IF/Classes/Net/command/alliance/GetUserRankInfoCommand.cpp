//
//  GetUserRankInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/4/10.
//
//

#include "GetUserRankInfoCommand.h"

bool GetUserRankInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MS_GET_USER_TOP_INFO) != 0)
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