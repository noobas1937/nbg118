//
//  BeenAttackedPush.cpp
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#include "BeenAttackedPush.h"
#include "CommonInclude.h"
#include "WorldController.h"
#include "UIComponent.h"

void BeenAttackedPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("cityDefValue") || !params->objectForKey("ft"))
        return;
    GlobalData::shared()->cityDefenseVal = params->valueForKey("cityDefValue")->intValue();
    GlobalData::shared()->cityFireStamp = params->valueForKey("ft")->doubleValue();
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
    UIComponent::getInstance()->showCityFireBtn(NULL);
}