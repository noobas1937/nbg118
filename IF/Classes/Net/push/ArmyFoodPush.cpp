//
//  ArmyFoodPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-4.
//
//

#include "ArmyFoodPush.h"

void ArmyFoodPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    GlobalData::shared()->resourceInfo.lFood = params->valueForKey("food")->intValue();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
}