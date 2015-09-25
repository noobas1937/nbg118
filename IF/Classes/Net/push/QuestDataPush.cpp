//
//  QuestDataPush.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#include "QuestDataPush.h"
#include "QuestController.h"


void QuestDataPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
    QuestController::getInstance()->updateTask(tasks);
    CCArray* ckfTask = dynamic_cast<CCArray*>(params->objectForKey("ckfTask"));
    QuestController::getInstance()->updateCKFTask(ckfTask);
}