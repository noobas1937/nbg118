//
//  PortActCommand.cpp
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#include "PortActCommand.h"
#include "ActivityController.h"

bool TimeRwdCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TIME_RWD_COMMAND) != 0) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    PortActController::getInstance()->endTimeRwd(params);
    return true;
}

bool TimeExchangeInitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TIME_EX_INIT_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        PortActController::getInstance()->endGetExchangeData(params, m_type);
    }
    return true;
}

bool TimeExchangeCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TIME_EX_COMMAND) != 0) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString* pStr = params->valueForKey("errorCode");
    if (pStr->compare("") != 0) {
//        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
        PortActController::getInstance()->endExchange(params);
    } else {
        PortActController::getInstance()->endExchange(params);
    }
    return true;
}

bool DailyRewardInitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_REWARD_INIT_COMMAND) != 0)
        return false;    
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        PortActController::getInstance()->endGetDailyRwdData(params);
    }
    return true;
}

bool DailyRewardCheckInCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_REWARD_CHECKIN_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
//        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }
    PortActController::getInstance()->endGetCheckInRwd(params, m_isVip);
    return true;
}

bool NewPlayerRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(NEW_PLAYER_RWD_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    PortActController::getInstance()->endGetNewPlayerRD(params);
    return true;
}