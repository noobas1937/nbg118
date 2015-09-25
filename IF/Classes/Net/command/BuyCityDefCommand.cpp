//
//  BuyCityDefCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-8.
//
//

#include "BuyCityDefCommand.h"
#include "GameController.h"

bool BuyCityDefCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(BUY_CITY_DEF) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        if(params->objectForKey("remainGold")){
            int gold = params->valueForKey("remainGold")->intValue();
            GlobalData::shared()->playerInfo.gold = gold;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}