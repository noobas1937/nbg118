//
//  DailyActivePush.cpp
//  IF
//
//  Created by chenfubi on 15/4/27.
//
//

#include "DailyActivePush.h"
#include "DailyActiveController.h"

void DailyActivePush::handleResponse(CCDictionary *dict)
{
    if (!dict || DailyActiveController::shared()->getDailyActiveOpenFlag()!=1)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("quest"));
    if (arr)
    {
        CCObject* obj = NULL;
        string key = "";
        CCDictionary* d = NULL;
        CCARRAY_FOREACH(arr, obj)
        {
            d = dynamic_cast<CCDictionary*>(obj);
            key = d->valueForKey("id")->getCString();
            DailyActiveController::shared()->updateItemInfo(key, d, true);
        }
        DailyActiveController::shared()->updateActivePoint();
        DailyActiveController::shared()->updateBoxInfo(NULL);
    }
}