//
//  ActivityPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#include "ActivityPush.h"
#include "ActivityController.h"

void ActivityPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    ActivityController::getInstance()->addActivity(params);
}
