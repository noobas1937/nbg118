//
//  FortCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-4.
//
//

#include "FortCommand.h"
#include "fireandcomman.h"
#include "QueueController.h"

bool FortStartCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FORT_START_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        params->setObject(CCString::create(CC_ITOA(m_num)), "train");
        ResultParser::parseResult(FORT_START_COMMAND, m_id, params);
        auto queue = _dict(params->objectForKey("queue"));
        QueueController::getInstance()->addQueueInfo(queue);
        callSuccess(NetResult::create());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
    }
    return true;
}

bool FortEndCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FORT_END_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
        GlobalData::shared()->playerInfo.fortPower = GlobalData::shared()->playerInfo.fortPower - GlobalData::shared()->playerInfo.addPower;
    }else{
        GlobalData::shared()->fortList[m_id].finishTime = 0;
        GlobalData::shared()->fortList[m_id].free = params->valueForKey("free")->intValue();
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(GlobalData::shared()->fortList[m_id].armyId);
        std::string icon = it->second.getBodyIcon();
        std::string name = it->second.getName();
        CCCommonUtils::flyHint(icon, _lang("103675"), _lang_1("103676", name.c_str()));
        ResultParser::parseResult(FORT_END_COMMAND, m_id, params);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ARMY_NUM_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS_HARVEST);
    }
    GlobalData::shared()->playerInfo.addPower = 0;
    return true;
}


bool FortStartUseGoldCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FORT_START_USE_GOLD_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        ResultParser::parseResult(FORT_START_USE_GOLD_COMMAND, m_id, params);
        auto queue = _dict(params->objectForKey("queue"));
        QueueController::getInstance()->addQueueInfo(queue);
        callSuccess(NetResult::create());
    }
    return true;
}

bool FortDismissCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FORT_DISMISS_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        ResultParser::parseResult(FORT_DISMISS_COMMAND, m_id, params);
    }
    return true;
}

bool FortCancelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(FORT_CANCEL_COMMAND) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        ResultParser::parseResult(FORT_CANCEL_COMMAND, m_id, params);
    }
    return true;
}

void ResultParser::parseResult(std::string cmd, std::string armyId, cocos2d::CCDictionary *dict){
    const CCString *pStr = dict->valueForKey("gold");
    if(pStr->compare("")!=0){
        int gold = pStr->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
    }
    GlobalData::shared()->resourceInfo.setResourceData(dict);
}