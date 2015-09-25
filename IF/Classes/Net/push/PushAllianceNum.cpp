//
//  PushAllianceNum.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-1.
//
//

#include "PushAllianceNum.h"

void PushAllianceNum::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params->objectForKey("t")){
        int type = params->valueForKey("t")->intValue();
        if(type==0){
            int num = params->valueForKey("n")->intValue();
            GlobalData::shared()->playerInfo.allianceInfo.militaryNum = num;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_MILITARY_NUM);
        }
    }
}