//
//  MODCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14/11/20.
//
//

#include "MODCommand.h"
#include "ModController.h"
#include "RewardController.h"
#include "UIComponent.h"

bool MODCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MOD_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
//        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}
bool MODLanGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MOD_LAN) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
        //        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool MODRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MOD_REWARD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
//        callFail(NetResult::create());
    }else{
        CCCommonUtils::flyHint("", "", _lang_1("132007", CC_ITOA(ModController::getInstance()->rewardGold)));
        int gold = params->valueForKey("gold")->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool MODGetUserInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_USER_INFO) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        CCCommonUtils::flyHint("", "", _lang("132016"));
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool MODGetServerModInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ALL_SERVER_USER_INFO) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
//        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool MODSetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SET_MOD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
//        callFail(NetResult::create());
    }else{
        int remain = params->valueForKey("remain")->intValue();
        CCCommonUtils::flyHint("", "", _lang_1("132018", CC_ITOA(remain)));
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool MODRewardInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MODE_REWARD_STATUS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
//        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}