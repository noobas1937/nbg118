//
//  SacrificeCommand.cpp
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#include "SacrificeCommand.h"
#include "SacrificeInfo.h"
#include "GlobalData.h"

bool SacrificeGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SACRIFICE_GET) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        return true;
    }
    SacrificeInfo &sacrificeInfo = GlobalData::shared()->sacrificeInfo;
    sacrificeInfo.parse(params);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SACRIFICE_GET);
    return true;
}
bool SacrificeBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SACRIFICE_BUY) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        return true;
    }
    SacrificeInfo &sacrificeInfo = GlobalData::shared()->sacrificeInfo;
    sacrificeInfo.parse(params);
    if(params->objectForKey("goldRemain")){
        GlobalData::shared()->playerInfo.gold = params->valueForKey("goldRemain")->intValue();
    }
    if(params->objectForKey("WOOD")){
        GlobalData::shared()->resourceInfo.lWood = params->valueForKey("WOOD")->intValue();
    }
    if(params->objectForKey("FOOD")){
        GlobalData::shared()->resourceInfo.lFood = params->valueForKey("FOOD")->intValue();
    }
    if(params->objectForKey("IRON")){
        GlobalData::shared()->resourceInfo.lIron = params->valueForKey("IRON")->intValue();
    }
    if(params->objectForKey("STONE")){
        GlobalData::shared()->resourceInfo.lStone = params->valueForKey("STONE")->intValue();
    }
    if(params->objectForKey("SILVER")){
        GlobalData::shared()->resourceInfo.lMoney = params->valueForKey("SILVER")->intValue();
    }
    CCDictionary* objDict = CCDictionary::create();
    if(params->objectForKey("hit")){
        int hint = params->valueForKey("hit")->intValue();
        objDict->setObject(CCString::create(CC_ITOA(hint)), "hint");
        int resource = params->valueForKey("resource")->intValue();
        objDict->setObject(CCString::create(CC_ITOA(resource)), "resource");
        int num = params->valueForKey("num")->intValue();
        objDict->setObject(CCString::create(CC_ITOA(num)), "num");
    }

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SACRIFICE_BUY,objDict);
    return true;
}