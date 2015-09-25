//
//  AllianceFightInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#include "AllianceFightInfo.h"
#include "RewardController.h"
#include <spine/Json.h>

void AllianceFightInfo::parseInfo(string info){
    m_role = 0;
    m_selfIsAtt = false;
    Json* jReport = Json_create(info.c_str());
    m_time = Json_getFloat(jReport,"time",0);
    m_win = Json_getInt(jReport,"win",0);
    m_type = Json_getInt(jReport,"type",0);
    Json* att = Json_getItem(jReport,"att");
    m_reward = "";
    if (att!=NULL) {
        m_attackName = Json_getString(att,"name","");
        m_attackAlliance = Json_getString(att,"asn","");
        m_attackAID = Json_getString(att,"aid","");
        m_attAsn = Json_getString(att,"asn","");
        m_role = Json_getInt(att,"role",0);
        m_monsterId = Json_getString(att,"monsterId","");
        if(m_attackAID==GlobalData::shared()->playerInfo.allianceInfo.uid){
            m_selfIsAtt = true;
        }
        Json *res=Json_getItem(att,"res");
        if(res){
            int size = Json_getSize(res);
            for (int i=0; i<size; i++) {
                Json *item = Json_getItemAt(res, i);
                int itemType = Json_getInt(item,"t",-1);
                int itemV = Json_getInt(item,"v",-1);
                if(itemType!=-1 && itemV>0){
                    m_reward += RewardController::getInstance()->getNameByType(itemType,itemV);
                    m_reward += ":";
                    m_reward += CC_CMDITOA(itemV);
                    if(i!=(size-1)){
                        m_reward += ",";
                    }
                }
            }
        }
    }
    if(m_reward==""){
        m_reward = "0";
    }
    Json* def = Json_getItem(jReport,"def");
    if (def!=NULL) {
        m_defName = Json_getString(def,"name","");
        m_defAlliance = Json_getString(def,"asn","");
        m_defkill = Json_getInt(def,"kill",0);
        m_defAID = Json_getString(def,"defAID","");
        m_defAsn = Json_getString(def,"asn","");
    }
    Json_dispose(jReport);
}
