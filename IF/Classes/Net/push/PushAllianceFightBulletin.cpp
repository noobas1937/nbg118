//
//  PushAllianceFightBulletin.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#include "PushAllianceFightBulletin.h"

void PushAllianceFightBulletin::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params->objectForKey("rank")){

    }
}