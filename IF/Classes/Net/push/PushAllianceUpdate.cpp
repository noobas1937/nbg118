//
//  PushAllianceUpdate.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-14.
//
//

#include "PushAllianceUpdate.h"
#include "ScienceController.h"
void PushAllianceUpdate::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    auto dic = _dict(params->objectForKey("alliance"));
    GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(dic);
}

void PushAllianceScience::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params) {
        ScienceController::getInstance()->pushUpdateAllInfo(params);
    }
}