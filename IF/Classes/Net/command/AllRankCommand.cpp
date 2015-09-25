//
//  AllRankCommand.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#include "AllRankCommand.h"
#include "AllianceRankInfo.h"
//alliance power rank
bool AllianceRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("allianceRanking"));

        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.allianceRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<AllianceRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.allianceRankList.begin(); it != GlobalData::shared()->rankListInfo.allianceRankList.end(); it++) {
            AllianceRankInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }

        GlobalData::shared()->rankListInfo.allianceRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            AllianceRankInfo* allianceDonateInfo = AllianceRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic);
            GlobalData::shared()->rankListInfo.allianceRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_RANK);
    }
    return true;
}
//alliance kill rank
bool AllianceKillRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_KILL_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("allianceRanking"));
        
        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.allianceRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<AllianceRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.allianceRankList.begin(); it != GlobalData::shared()->rankListInfo.allianceRankList.end(); it++) {
            AllianceRankInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }
        
        GlobalData::shared()->rankListInfo.allianceRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            AllianceRankInfo* allianceDonateInfo = AllianceRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic);
            GlobalData::shared()->rankListInfo.allianceRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ALLIANCE_RANK);
    }
    return true;
}

bool PlayerRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PLAYER_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("serverRanking"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.playerRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<PlayerRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.playerRankList.begin(); it != GlobalData::shared()->rankListInfo.playerRankList.end(); it++) {
            PlayerRankInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }
        GlobalData::shared()->rankListInfo.playerRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            PlayerRankInfo* allianceDonateInfo = PlayerRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->rankListInfo.playerRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLAYER_RANK);
    }
    return true;
}

bool PlayerKillRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PLAYER_KILL_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("serverRanking"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.playerRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<PlayerRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.playerRankList.begin(); it != GlobalData::shared()->rankListInfo.playerRankList.end(); it++) {
            PlayerRankInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }
        GlobalData::shared()->rankListInfo.playerRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            PlayerRankInfo* allianceDonateInfo = PlayerRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->rankListInfo.playerRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLAYER_RANK);
    }
    return true;
}
bool PlayerBuildingRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PLAYER_BUILDING_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("serverRanking"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.playerRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<PlayerRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.playerRankList.begin(); it != GlobalData::shared()->rankListInfo.playerRankList.end(); it++) {
            PlayerRankInfo* tmp = *it;
            tmp->release();
            //GlobalData::shared()->allianceDonateAllList.erase(it);
            
        }
        GlobalData::shared()->rankListInfo.playerRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            PlayerRankInfo* allianceDonateInfo = PlayerRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->rankListInfo.playerRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLAYER_RANK);
    }
    return true;
}
bool PlayerLevelRankListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PLAYER_LEVEL_RANK) != 0)
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
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("serverRanking"));
        double time = params->valueForKey("refreshTime")->doubleValue()/1000;
        if(params->objectForKey("selfRanking")){
            GlobalData::shared()->rankListInfo.playerRank =params->valueForKey("selfRanking")->intValue();
        }
        vector<PlayerRankInfo*>::iterator it;
        for (it = GlobalData::shared()->rankListInfo.playerRankList.begin(); it != GlobalData::shared()->rankListInfo.playerRankList.end(); it++) {
            PlayerRankInfo* tmp = *it;
            tmp->release();
        }
        GlobalData::shared()->rankListInfo.playerRankList.clear();
        CCObject *itemObject;
        CCARRAY_FOREACH(arr, itemObject){
            CCDictionary *goodsDic = _dict(itemObject);
            std::string uid = goodsDic->valueForKey("uid")->getCString();
            PlayerRankInfo* allianceDonateInfo = PlayerRankInfo::create();
            allianceDonateInfo->retain();
            allianceDonateInfo->update(goodsDic, time);
            GlobalData::shared()->rankListInfo.playerRankList.push_back(allianceDonateInfo);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLAYER_RANK);
    }
    return true;
}