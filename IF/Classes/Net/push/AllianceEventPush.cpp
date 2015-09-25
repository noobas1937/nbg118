//
//  AllianceEventPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-8.
//
//

#include "AllianceEventPush.h"
#include "AllianceManager.h"
#include "AllianceEventInfo.h"

void AllianceEventPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    AllianceEventInfo* info = AllianceEventInfo::create();
    info->parse(params);
    map<string, AllianceEventInfo* >::iterator it = AllianceManager::getInstance()->eventsMap.find(info->srcTime);
    if(it!=AllianceManager::getInstance()->eventsMap.end()){
        it->second->parse(params);
    }else{
        info->retain();
        AllianceManager::getInstance()->eventsMap[info->srcTime] = info;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ADD_ALLIANCE_EVENT);
}