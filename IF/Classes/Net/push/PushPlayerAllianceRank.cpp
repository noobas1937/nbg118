//
//  PushPlayerAllianceRank.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-13.
//
//

#include "PushPlayerAllianceRank.h"

void PushPlayerAllianceRank::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params->objectForKey("rank")){
        int rank = params->valueForKey("rank")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.rank = rank;
    }
}