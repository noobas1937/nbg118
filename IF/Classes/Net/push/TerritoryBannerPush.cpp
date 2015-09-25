//
//  TerritoryBannerPush.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/19.
//
//

#include "TerritoryBannerPush.h"
#include "WorldController.h"
#include "QueueController.h"
#include "SoundController.h"
#include "WorldMapView.h"
void TerritoryBannerPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    WorldController::getInstance()->forceUpdateTerritoryBannerTile = true;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        if (params->objectForKey("uuid")) {
            string uuid = params->valueForKey("uuid")->getCString();
            auto &info = WorldController::getInstance()->m_marchInfo[uuid];
            if (info.ownerType == PlayerSelf) {
                if(info.stateType == StateOccupy){
                    info.territoryFinishTime = params->valueForKey("updateTime")->doubleValue();
                    info.territoryStartTime = params->valueForKey("startTime")->doubleValue();
                    info.territoryStat = params->valueForKey("stat")->intValue();
                }
                //info.territoryFinishTime = info.territoryStartTime + 1800000;//////////////
                SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                QueueController::getInstance()->addMarchQueueInfo(info.uuid);
            }
        }
        
        if (params->objectForKey("pointId")) {
            int pointId = params->valueForKey("pointId")->intValue();
            auto it = WorldController::getInstance()->getCityInfos().find(pointId);
            if (it != WorldController::getInstance()->getCityInfos().end()) {
                if (params->objectForKey("buildingSpd")) {
                    it->second.m_bannerInfo.buildSpeed = params->valueForKey("buildingSpd")->doubleValue() * 1000;
                }
                if (params->objectForKey("curDef")) {
                    it->second.m_bannerInfo.defence = params->valueForKey("curDef")->intValue();
                }
                if (params->objectForKey("maxDef")) {
                    it->second.m_bannerInfo.defenceMax = params->valueForKey("maxDef")->intValue();
                }
                if (params->objectForKey("stat")) {
                    it->second.m_bannerInfo.state = (AAreaState)params->valueForKey("stat")->intValue();
                }
                if (params->objectForKey("updateTime")) {
                    it->second.m_bannerInfo.updateTime = params->valueForKey("updateTime")->doubleValue() / 1000;
                }
                if (params->objectForKey("startTime")) {
                    it->second.m_bannerInfo.startTime = params->valueForKey("startTime")->doubleValue() / 1000;
                }
                if (params->objectForKey("flag")) {
                    it->second.m_bannerInfo.banner = params->valueForKey("flag")->getCString();
                }
                if (!params->objectForKey("owner")) {
                    it->second.m_allianceAreaOwnerInfo.ownerId = "";
                    it->second.m_allianceAreaOwnerInfo.ownerName = "";
                    it->second.m_allianceAreaOwnerInfo.allianceId = "";
                    it->second.m_allianceAreaOwnerInfo.allianceAbbr = "";
                    it->second.m_allianceAreaOwnerInfo.ownerPic = "";
                    it->second.m_allianceAreaOwnerInfo.team = false;
                }
                if (params->objectForKey("owner")) {
                    CCDictionary* owner = _dict(params->objectForKey("owner"));
                    if (owner->objectForKey("uid")) {
                        it->second.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
                    }
                    if (owner->objectForKey("name")) {
                        it->second.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
                    }
                    if (owner->objectForKey("allianceId")) {
                        it->second.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
                    }
                    if (owner->objectForKey("abbr")) {
                        it->second.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
                    }
                    if (owner->objectForKey("team")) {
                        it->second.m_allianceAreaOwnerInfo.team = owner->valueForKey("team")->boolValue();
                    }
                    if (owner->objectForKey("pic")) {
                        it->second.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
                    }
                }
                if (WorldMapView::instance()) {
                    WorldMapView::instance()->onRefreshAAreaBuilding(it->second.cityIndex, it->second.m_bannerInfo.state, false,false,false,true);
                }
            }
        }
    }
}