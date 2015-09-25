//
//  ExplorePush.cpp
//  IF
//
//  Created by 李锐奇 on 14-6-5.
//
//

#include "ExplorePush.h"
#include "WorldController.h"

void ExplorePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    WorldController::getInstance()->addMonsetResearchInfo(result);
//    std::string itemId = result->valueForKey("id")->getCString();
//    GlobalData::shared()->fortList[itemId] = ArmyInfo(result);
    return;
}