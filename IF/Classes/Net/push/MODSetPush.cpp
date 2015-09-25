//
//  MODSetPush.cpp
//  IF
//
//  Created by 李锐奇 on 14/11/21.
//
//

#include "MODSetPush.h"

void MODSetPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
}