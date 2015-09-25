//
//  ItemPush.cpp
//  IF
//
//  Created by fubin on 14-4-24.
//
//

#include "ItemPush.h"
#include "CommonInclude.h"
#include "ToolController.h"

void ItemAddPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    ToolController::getInstance()->pushAddTool(result);
    return;
}

void ItemDelPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    ToolController::getInstance()->pushDelTool(result);
    return;
}

void ItemUsePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showFirework", result);
    return;
}
