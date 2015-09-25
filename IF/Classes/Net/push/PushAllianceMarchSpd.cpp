//
//  PushAllianceMarchSpd.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-28.
//
//

#include "PushAllianceMarchSpd.h"

void PushAllianceMarchSpd::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_TEAM_ARRIVE_TIME,params);
}