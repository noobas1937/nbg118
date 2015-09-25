//
//  ChristmasPush.cpp
//  IF
//
//  Created by fubin on 14-12-11.
//
//

#include "ChristmasPush.h"
#include "ActivityController.h"

void ChristmasPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    ChristmasActController::getInstance()->pushSocksMsg(params);
}