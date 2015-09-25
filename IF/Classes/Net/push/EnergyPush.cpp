//
//  EnergyPush.cpp
//  IF
//
//  Created by fubin on 13-12-5.
//
//

#include "EnergyPush.h"

void EnergyPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = dynamic_cast<cocos2d::CCDictionary*>(dict->objectForKey("params"));
    GlobalData::shared()->lordInfo.energy = result->valueForKey("energy")->intValue();
    GlobalData::shared()->lordInfo.nextEnergyRecoverTime = result->valueForKey("nextEnergyRecoverTime")->doubleValue();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_ENERGY_UPDATE);
    return;
}