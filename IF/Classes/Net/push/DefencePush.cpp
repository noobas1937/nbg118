//
//  DefencePush.cpp
//  IF
//
//  Created by fubin on 14-4-15.
//
//

#include "DefencePush.h"
#include "TroopsController.h"

void DefencePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    TroopsController::getInstance()->pushDefenceData(result);
    return;
}