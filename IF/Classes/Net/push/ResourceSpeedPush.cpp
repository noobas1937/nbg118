//
//  ResourceSpeedPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-9-22.
//
//

#include "ResourceSpeedPush.h"
#include "CommonInclude.h"
#include "WorldController.h"
#include "QueueController.h"

void ResourceSpeedPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* params = _dict(dict->objectForKey("params"));
    
    if (!params)
        return;
    
    std::string uuid = params->valueForKey("uuid")->getCString();
    auto it = WorldController::getInstance()->m_marchInfo.find(uuid);
    if(it != WorldController::getInstance()->m_marchInfo.end()){
        auto &marchInfo = it->second;
        marchInfo.troopMax = params->valueForKey("q")->intValue();
        marchInfo.digSpeed = params->valueForKey("spd")->floatValue();
        marchInfo.digStartTime = params->valueForKey("sdt")->doubleValue();
        marchInfo.digStartNum = params->valueForKey("collected")->intValue();
        marchInfo.changeCollectSpdTime = params->valueForKey("changeCollectSpdTime")->doubleValue();

        QueueController::getInstance()->addMarchQueueInfo(uuid);
    }
    return;
}