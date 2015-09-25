//
//  StaminaRollbackPush.cpp
//  IF
//
//  Created by wangchaoyi on 15/3/31.
//
//

#include "StaminaRollbackPush.h"
#include "WorldController.h"

void StaminaRollbackPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        WorldController::getInstance()->currentStamine = params->valueForKey("stamina")->intValue();
        WorldController::getInstance()->lastStamineTime = params->valueForKey("lastStaminaTime")->doubleValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
    }
}