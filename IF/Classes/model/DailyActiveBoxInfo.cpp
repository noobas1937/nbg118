//
//  DailyActiveBoxInfo.cpp
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#include "DailyActiveBoxInfo.h"

void DailyActiveBoxInfo::initData(string itemId)
{
    CCDictionary* dict = LocalController::shared()->DBXMLManager()->getObjectByKey(itemId);
    initData(dict);
}

void DailyActiveBoxInfo::initData(cocos2d::CCDictionary *dict)
{
    updateData(dict);
}

void DailyActiveBoxInfo::updateData(cocos2d::CCDictionary *dict)
{
    if (dict == NULL)
    {
        return;
    }
    if (dict->objectForKey("point"))
    {
        point = dict->valueForKey("point")->intValue();
    }
    if (dict->objectForKey("info"))
    {
        reward = (CCArray*)dict->objectForKey("info");
    }
}
