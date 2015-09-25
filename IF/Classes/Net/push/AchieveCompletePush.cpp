//
//  AchieveCompletePush.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/19.
//
//

#include "AchieveCompletePush.h"
#include "AchievementController.h"

void AchieveCompletePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("achieve"));
    AchievementController::getInstance()->updateAchievement(arr);
}