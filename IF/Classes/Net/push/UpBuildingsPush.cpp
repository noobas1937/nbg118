//
//  UpBuildingsPush.cpp
//  IF
//
//  Created by fubin on 13-9-16.
//
//

#include "CommonInclude.h"
#include "UpBuildingsPush.h"
#include "FunBuildController.h"

void UpBuildingsPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return;
    
    if (params) {
        CCString* str = CCString::create(params->valueForKey("counter")->getCString());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE, str);
    }
}

void ResBuildingsPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params) {
        FunBuildController::getInstance()->refreshResTime(params);
    }
}