//
//  PayPush.cpp
//  COK
//
//  Created by zhaohf on 14-4-15.
//
//

#include "PayPush.h"
#include "CommonInclude.h"
#include "PayController.h"

void PayPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = _dict(dict->objectForKey("params"));
    
    if (!result)
        return;
    
    if(result->objectForKey("buy_gold_pack")){
        CCCommonUtils::payParseData(result);
//        GlobalData::shared()->isBind = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        PayController::getInstance()->removeWebView();
#endif
    }else if(result->objectForKey("gold")){
        GlobalData::shared()->playerInfo.gold = result->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    return;
}