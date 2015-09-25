//
//  AllianceCommentShieldList.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#include "AllianceCommentShieldList.h"

bool AllianceCommentShieldList::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_COMMENT_SHIELD_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        callSuccess(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}