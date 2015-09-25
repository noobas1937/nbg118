//
//  WorldActivityPush.cpp
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#include "WorldActivityPush.h"
#include "WorldController.h"
#include "WorldMapView.h"

void WorldActivityPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    WorldController::getInstance()->updateWorldActivityTime(params);

}

void WorldMarchDeletePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    std::string marchUid = params->valueForKey("uuid")->getCString();
    auto it = WorldController::getInstance()->m_marchInfo.find(marchUid);
    if(it != WorldController::getInstance()->m_marchInfo.end()){
        it->second.stateType = StateReturn;
        it->second.endStamp = 0;
    }
}

void WorldTrebuchetAttPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(WorldMapView::instance() && WorldController::getInstance()->isInWorld){
        WorldMapView::instance()->trebuchetAttack(params);
    }
}

void WorldMarchAddPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    auto arr = CCArray::create();
    arr->addObject(params);
    WorldController::getInstance()->initMarchInfo(arr);
}

void OfficerClearPush::handleResponse(cocos2d::CCDictionary *dict){
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    std::string name = params->valueForKey("o")->getCString();
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_map->updateDynamicMap();
    }
    for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
        if(it->second.playerName == name){
            it->second.officer = "";
            it->second.isChangingSpeed = true;
        }
    }
    string uid = params->valueForKey("uid")->getCString();
    if (GlobalData::shared()->playerInfo.uid == uid) {
        GlobalData::shared()->playerInfo.officer = "";
    }
}

void OfficerAddPush::handleResponse(cocos2d::CCDictionary *dict){
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    int officer = params->valueForKey("officer")->intValue();
    string officerId = params->valueForKey("officer")->getCString();
    double time = WorldController::getInstance()->getTime() + 1000*59;
    string tip = "";
    string ownerName = params->valueForKey("o")->getCString();
    string officerName = CCCommonUtils::getNameById(officerId);

    if(WorldMapView::instance()){
        WorldMapView::instance()->m_map->updateDynamicMap();
    }
    for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
        if(it->second.playerName == ownerName){
            it->second.officer = officerId;
            it->second.isChangingSpeed = true;
        }
    }
    if(ownerName == GlobalData::shared()->playerInfo.name){
        GlobalData::shared()->playerInfo.officer = officerId;
        if(params->objectForKey("relinquishEndTime")){
            double relinquishEndTime = params->valueForKey("relinquishEndTime")->doubleValue();
            int relinquishFlag = params->valueForKey("relinquishFlag")->intValue();
            GlobalData::shared()->playerInfo.relinquishFlag = relinquishFlag;
            GlobalData::shared()->playerInfo.relinquishEndTime = relinquishEndTime;
        }
    }
    
    if(officer>216008){
        tip = _lang_2("110080", ownerName.c_str(),officerName.c_str());
    }else if(officer>216000){
        tip = _lang_2("110079", ownerName.c_str(), officerName.c_str());
    }
    if (officerId!="" && officerId != KINGDOM_KING_ID) {
        CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_KING);
    }
}
