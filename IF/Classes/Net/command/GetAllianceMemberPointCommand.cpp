//
//  GetAllianceMemberPointCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-10-14.
//
//

#include "GetAllianceMemberPointCommand.h"

bool GetAllianceMemberPointCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MEMBER_POINT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GET_MEMBER_POINT_BACK, params);
    }
    
    return true;
}