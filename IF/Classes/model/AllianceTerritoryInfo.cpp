//
//  AllianceTerritoryInfo.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/9.
//
//

#include "AllianceTerritoryInfo.h"
void AllianceTerritoryInfo::parseInfo(CCDictionary* dict){
    if(dict->objectForKey("uid")){
        m_territoryId = dict->valueForKey("uid")->getCString();
    }
    if(dict->objectForKey("allianceAbbr")){
        m_allianceAbbr = dict->valueForKey("allianceAbbr")->getCString();
    }
    if(dict->objectForKey("allianceName")){
        m_allianceName = dict->valueForKey("allianceName")->getCString();
    }
    if(dict->objectForKey("pic")){
        m_territoryPic = dict->valueForKey("pic")->getCString();
    }
    if(dict->objectForKey("stat")){
        m_territoryState = dict->valueForKey("stat")->intValue();
    }
    if(dict->objectForKey("type")){
        m_territoryType = (WorldCityType)dict->valueForKey("type")->intValue();
    }
    if(dict->objectForKey("resType")){
        m_territoryResourceType = (WorldResourceType)dict->valueForKey("resType")->intValue();
    }
    if(dict->objectForKey("name")){
        m_territoryName = dict->valueForKey("name")->getCString();
        if (dict->objectForKey("count")) {
            m_territoryCount = dict->valueForKey("count")->intValue();
        }
        if (m_territoryType == Tile_allianceArea) {
            GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[m_territoryCount] = make_pair(m_territoryId, m_territoryName);
        }
    }
    if(!dict->objectForKey("name")){
        if (m_territoryType == Tile_allianceArea) {
            if (dict->objectForKey("count")) {
                m_territoryCount = dict->valueForKey("count")->intValue();
                m_territoryName = _lang_1("115312", CC_ITOA(m_territoryCount));
            }
            else
                m_territoryName = _lang_1("115312", "1");
            GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[m_territoryCount] = make_pair(m_territoryId, m_territoryName);
        }
        else if (m_territoryType == tile_superMine) {
            switch (m_territoryResourceType) {
                case Food:
                    m_territoryName = _lang("115376");
                    break;
                case Wood:
                    m_territoryName = _lang("115377");
                    break;
                case Iron:
                    m_territoryName = _lang("115378");
                    break;
                case Stone:
                    m_territoryName = _lang("115379");
                    break;
                default:
                    break;
            }
        }
        else if (m_territoryType == tile_tower) {
            if (dict->objectForKey("count")) {
                m_territoryCount = dict->valueForKey("count")->intValue();
                m_territoryName = _lang_1("115406", CC_ITOA(m_territoryCount));
            }
            else
                m_territoryName = _lang_1("115406", "1");
        }
        else if (m_territoryType == tile_wareHouse) {
            m_territoryName = _lang("115364");
        }
    }
    if(dict->objectForKey("level")){
        m_territoryLv = dict->valueForKey("level")->intValue();
    }
    if(dict->objectForKey("curDef")){
        m_territoryDef = dict->valueForKey("curDef")->intValue();
    }
    if(dict->objectForKey("tq")){
        m_territoryRes = dict->valueForKey("tq")->intValue();
    }
    if(dict->objectForKey("pointId")){
        m_territoryPtIndex = dict->valueForKey("pointId")->intValue();
    }
    if (!dict->objectForKey("pointId")) {
        m_territoryPtIndex = 0;
    }
    if(dict->objectForKey("owner")){
        m_ownerFlag = true;
    }
    if(!dict->objectForKey("owner")){
        m_ownerFlag = false;
    }
    
    m_territoryActive = 1;
    
    m_totalResStorage = 0;
    if (dict->objectForKey("resourceList")) {
        CCArray* array = (CCArray*)dict->objectForKey("resourceList");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                if (item->objectForKey("uid")) {
                    if (item->objectForKey("wood")) {
                        m_totalResStorage += item->valueForKey("wood")->longValue() * GlobalData::shared()->resources_weight_1;
                    }
                    if (item->objectForKey("food")) {
                        m_totalResStorage += item->valueForKey("food")->longValue() * GlobalData::shared()->resources_weight_4;
                    }
                    if (item->objectForKey("stone")) {
                        m_totalResStorage += item->valueForKey("stone")->longValue() * GlobalData::shared()->resources_weight_2;
                    }
                    if (item->objectForKey("iron")) {
                        m_totalResStorage += item->valueForKey("iron")->longValue() * GlobalData::shared()->resources_weight_3;
                    }
                }
            }
        }
    }
    
    m_banner = "";
    if (dict->objectForKey("banner")) {
        m_banner = dict->valueForKey("banner")->getCString();
    }
}