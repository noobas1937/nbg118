//
//  AchievementPush.cpp
//  IF
//
//  Created by ganxiaohua on 13-12-23.
//
//

#include "AchievementPush.h"

void AchievementPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
//    std::string id = params->valueForKey("id")->getCString();
//    map<string,AchievementInfo* >::iterator iter = GlobalData::shared()->achievementList.find(id);
//    if(iter!=GlobalData::shared()->achievementList.end()){
//        iter->second->setProgress(params->valueForKey("progress")->intValue());
//        iter->second->setTimes(params->valueForKey("times")->intValue());
//    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ACHIEVEMENT_DATA_CHANGE);
}