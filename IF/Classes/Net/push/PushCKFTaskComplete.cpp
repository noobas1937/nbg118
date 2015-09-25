//
//  PushCKFTaskComplete.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/29.
//
//

#include "PushCKFTaskComplete.h"
#include "QuestController.h"

void PushCKFTaskComplete::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCArray* ckfTask = dynamic_cast<CCArray*>(params->objectForKey("ckfTask"));
    QuestController::getInstance()->updateCKFTask(ckfTask);
}