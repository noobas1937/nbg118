//
//  EnemyReportPush.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "EnemyReportPush.h"
#include "EnemyInfoController.h"

void EnemyReportPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    EnemyInfoController::getInstance()->pushEnemyData(result);
    return;
}