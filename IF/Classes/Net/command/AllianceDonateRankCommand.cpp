//
//  AllianceDonateRankView.cpp
//  IF
//
//  Created by lifangkai on 14-9-3.
//
//

#include "AllianceDonateRankCommand.h"
#include "AllianceDonateInfo.h"
bool AllianceDonateAllCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_DONATE_RANK_ALL) != 0)
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
        // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("donateRank"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        
        vector<AllianceDonateInfo*>::iterator it;
        for (it = GlobalData::shared()->allianceDonateAllList.begin(); it != GlobalData::shared()->allianceDonateAllList.end(); it++) {
            AllianceDonateInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }
        GlobalData::shared()->allianceDonateAllList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            AllianceDonateInfo* allianceDonateInfo = AllianceDonateInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->allianceDonateAllList.push_back(allianceDonateInfo);
        }

        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_DONATE_RANK_ALL);
    }
    return true;
}
bool AllianceDonateTodayCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_DONATE_RANK_TODAY) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("donateRank"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        
        vector<AllianceDonateInfo*>::iterator it;
        for (it = GlobalData::shared()->allianceDonateTodayList.begin(); it != GlobalData::shared()->allianceDonateTodayList.end(); it++) {
            AllianceDonateInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateTodayList.erase(it);
            
        }
        GlobalData::shared()->allianceDonateTodayList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            AllianceDonateInfo* allianceDonateInfo = AllianceDonateInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->allianceDonateTodayList.push_back(allianceDonateInfo);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_DONATE_RANK_TODAY);
    }
    return true;
}

bool AllianceDonateWeekCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_DONATE_RANK_WEEK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("donateRank"));
        double time = 0;
        if (params->objectForKey("refreshTime")) {
            time = params->valueForKey("refreshTime")->doubleValue()/1000;
        }
        
        vector<AllianceDonateInfo*>::iterator it;
        for (it = GlobalData::shared()->allianceDonateWeekList.begin(); it != GlobalData::shared()->allianceDonateWeekList.end(); it++) {
            AllianceDonateInfo* tmp = *it;
            tmp->release();
        }
        GlobalData::shared()->allianceDonateWeekList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            AllianceDonateInfo* allianceDonateInfo = AllianceDonateInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->allianceDonateWeekList.push_back(allianceDonateInfo);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_DONATE_RANK_WEEK);
    }
    return true;
}