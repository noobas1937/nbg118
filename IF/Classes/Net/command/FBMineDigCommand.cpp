//
//  FBMineDigCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-22.
//
//

#include "FBMineDigCommand.h"
#include "SocialController.h"

bool FBMineDigCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(FB_MINE_DIG_COMMAND) != 0)
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
        int gold = params->valueForKey("gold")->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
        SocialController::getInstance()->parseCollect(params);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}