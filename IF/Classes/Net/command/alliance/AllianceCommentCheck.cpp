//
//  AllianceCommentCheck.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-16.
//
//

#include "AllianceCommentCheck.h"

bool AllianceCommentCheck::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_COMMENT_CHECK) != 0)
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