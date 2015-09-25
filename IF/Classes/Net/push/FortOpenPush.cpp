//
//  FortOpenPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-28.
//
//

#include "FortOpenPush.h"
#include "ArmyController.h"

void FortOpenPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    std::string itemId = result->valueForKey("id")->getCString();
    GlobalData::shared()->fortList[itemId] = ArmyInfo(result);
    return;
}