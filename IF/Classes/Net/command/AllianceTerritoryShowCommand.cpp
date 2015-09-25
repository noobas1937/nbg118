//
//  AllianceTerritoryShowCommand.cpp
//  IF
//
//  Created by wangchaoyi on 15/4/9.
//
//

#include "AllianceTerritoryShowCommand.h"

bool AllianceTerritoryShowCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_TERRITORY_SHOW) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_TERRITORY_DATA_BACKFAIL);
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
//        callFail(NetResult::create(Error_OK, params));
    }else{
//        callSuccess(NetResult::create(Error_OK, params));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_TERRITORY_DATA_BACK, params);
    }
    
    return true;
}

bool AllianceTerritoryDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_TERRITORY_DETAIL) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool AllianceTerritoryCheckIfStorageCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_TERRITORY_CHECKIFSTORAGE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHECKIFSTORAGE_DATA_BACKFAIL);
        CCCommonUtils::flyText(pStr->getCString());
        //        callFail(NetResult::create(Error_OK, params));
    }else{
        //        callSuccess(NetResult::create(Error_OK, params));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHECKIFSTORAGE_DATA_BACK, params);
    }
    return true;
}

bool AllianceTerritoryQianfanCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_TERRITORY_CHECKIFSTORAGE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}