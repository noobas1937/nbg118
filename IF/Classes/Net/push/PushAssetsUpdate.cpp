//
//  PushAssetsUpdate.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/17.
//
//

#include "PushAssetsUpdate.h"

void PushAssetsUpdate::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if (params->objectForKey("winPoint")) {
        long winPoint = params->valueForKey("winPoint")->doubleValue();
        GlobalData::shared()->playerInfo.winPoint = winPoint;
    }
}