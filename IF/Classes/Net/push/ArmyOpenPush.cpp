//
//  ArmyOpenPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-25.
//
//

#include "ArmyOpenPush.h"
#include "ArmyController.h"

void ArmyOpenPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    std::string itemId = result->valueForKey("id")->getCString();
    GlobalData::shared()->armyList[itemId] = ArmyInfo(result);
    return;
}
