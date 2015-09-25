//
//  TerritoryResourcePush.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/19.
//
//

#include "TerritoryResourcePush.h"
#include "WorldController.h"
#include "QueueController.h"
#include "SoundController.h"
#include "WorldMapView.h"
void TerritoryResourcePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    WorldController::getInstance()->forceUpdateTerritoryResourceTile = true;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        if (params->objectForKey("uuid")) {
            string uuid = params->valueForKey("uuid")->getCString();
            auto &info = WorldController::getInstance()->m_marchInfo[uuid];
            if (info.ownerType == PlayerSelf) {
                //unsigned int qid = QueueController::getInstance()->getMarchQueueQid(info.uuid);
                //GlobalData::shared()->allQueuesInfo.erase(qid);
                //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
                if(info.stateType == StateOccupy){
                    info.territoryStat = params->valueForKey("stat")->intValue();
                    info.territoryFinishTime = params->valueForKey("updateTime")->doubleValue();
                    info.territoryStartTime = params->valueForKey("startTime")->doubleValue();
                    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                    QueueController::getInstance()->addMarchQueueInfo(info.uuid);
                }
            }
        }
        
        if (params->objectForKey("pointId")) {
            int pointId = params->valueForKey("pointId")->intValue();
            auto it = WorldController::getInstance()->getCityInfos().find(pointId);
            if (it != WorldController::getInstance()->getCityInfos().end()) {
                if (params->objectForKey("buildingSpd")) {
                    it->second.m_superMineInfo.buildSpeed = params->valueForKey("buildingSpd")->doubleValue() * 1000;
                    it->second.m_superMineInfo.digSpeed = params->valueForKey("buildingSpd")->doubleValue() * 1000;
                }
                if (params->objectForKey("curDef")) {
                    it->second.m_superMineInfo.trdef = params->valueForKey("curDef")->intValue();
                }
                if (params->objectForKey("maxDef")) {
                    it->second.m_superMineInfo.trdefmax = params->valueForKey("maxDef")->intValue();
                }
                if (params->objectForKey("stat")) {
                    it->second.m_superMineInfo.trstat = (AAreaState)params->valueForKey("stat")->intValue();
                }
                if (params->objectForKey("updateTime")) {
                    it->second.m_superMineInfo.trutime = params->valueForKey("updateTime")->doubleValue() / 1000;
                }
                if (params->objectForKey("startTime")) {
                    it->second.m_superMineInfo.trstime = params->valueForKey("startTime")->doubleValue() / 1000;
                    it->second.m_superMineInfo.digStartTime = params->valueForKey("startTime")->doubleValue() /1000;
                }
                if(params->objectForKey("tq")){
                    it->second.m_superMineInfo.sum = params->valueForKey("tq")->intValue();
                }
                if (WorldMapView::instance()) {
                    WorldMapView::instance()->onRefreshAAreaBuilding(it->second.parentCityIndex, it->second.m_superMineInfo.trstat, true);
                }
                
            }
        }
    }
}