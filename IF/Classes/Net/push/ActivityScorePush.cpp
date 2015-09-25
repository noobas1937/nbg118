//
//  ActivityScorePush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-8.
//
//

#include "ActivityScorePush.h"
#include "ActivityController.h"

void ActivityScorePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    auto scoreAct = _dict(params->objectForKey("scoreAct"));
    if(scoreAct){
        ActivityController::getInstance()->updateCurrentEvent(scoreAct);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ACTIVITY_EVENT);
    }
}
