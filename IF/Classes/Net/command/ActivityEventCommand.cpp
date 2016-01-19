//
//  ActivityEventCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-24.
//
//

#include "ActivityEventCommand.h"
#include "ActivityController.h"
bool ActivityEventCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_EVENT_COMMAND) != 0)
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
        auto scoreAct = _dict(params->objectForKey("scoreAct"));
        if(scoreAct){
            ActivityController::getInstance()->updateCurrentEvent(scoreAct);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INIT_ACTIVITY_EVENT);
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}


bool ActivitySingleScoreGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_SINGLESCORE_COMMAND) != 0)
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
        ActivityController::getInstance()->retSingleScoreData(params);
    }
    return true;
}

bool GetSingleScoreRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_SINGLESCORE_RANK_COMMAND) != 0)
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
        ActivityController::getInstance()->retCurRankRwd(params, m_type);
    }
    return true;
}

bool GetSingleHistoryRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_SINGLES_RANK_HISTORY_COMMAND) != 0)
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
        ActivityController::getInstance()->retHistroyRank(params, m_type);
    }
    return true;
}

bool GetAllianceConRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_AL_RANK_CON_COMMAND) != 0)
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
        ActivityController::getInstance()->retAllianceConRank(params);
    }
    return true;
}

bool GetScoreRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACTIVITY_RANK_SCORE_COMMAND) != 0)
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
        ActivityController::getInstance()->retRank(params);
    }
    return true;
}