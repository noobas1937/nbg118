//
//  CKFNextRoundCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/7/1.
//
//

#include "CKFNextRoundCommand.h"

bool CKFNextRoundCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CKF_NEXT_ROUND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    return true;
}