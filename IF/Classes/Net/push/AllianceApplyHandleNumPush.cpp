//
//  AllianceApplyHandleNumPush.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/5.
//
//

#include "AllianceApplyHandleNumPush.h"
void AllianceApplyHandleNumPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        if (params->objectForKey("num")) {
            GlobalData::shared()->playerInfo.allianceInfo.applyNum = params->valueForKey("num")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_APPLY_NUM);
        }
    }
}