//
//  TavernPush.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-21.
//
//

#include "TavernPush.h"

void TavernPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
//    GeneralGambleController::getInstance()->addResetCard(params);
//    GeneralGambleController::getInstance()->isFirstTime = true;
}