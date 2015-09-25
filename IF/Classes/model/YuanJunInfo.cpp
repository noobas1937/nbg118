//
//  YuanJunInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-14.
//
//

#include "YuanJunInfo.h"

void YuanJunInfo::parseInfo(CCDictionary* dict){
    m_isLeader = false;
    m_canUse = true;
    if(dict->objectForKey("uid")){
        m_uid = dict->valueForKey("uid")->getCString();
    }
    if(dict->objectForKey("uuid")){
        m_uuid = dict->valueForKey("uuid")->getCString();
    }
    m_allianceId = "";
    if(dict->objectForKey("allianceId")){
        m_allianceId = dict->valueForKey("allianceId")->getCString();
    }
    if(dict->objectForKey("name")){
        m_name = dict->valueForKey("name")->getCString();
    }
    if(dict->objectForKey("pic")){
        m_pic = dict->valueForKey("pic")->getCString();
    }
    m_picVer = 0;
    if (dict->objectForKey("picVer")) {
        m_picVer = dict->valueForKey("picVer")->intValue();
    }
    if(dict->objectForKey("power")){
        m_power = dict->valueForKey("power")->intValue();
    }
    if(dict->objectForKey("totalTroops")){
        m_num = dict->valueForKey("totalTroops")->intValue();
    }
    if(dict->objectForKey("arrivalTime")){
        auto temp = dict->valueForKey("arrivalTime")->doubleValue()/1000;
        if(temp!=0){
            m_arrivalTime = GlobalData::shared()->changeTime(temp);
        }else{
            m_arrivalTime = 0;
        }
    }else{
        m_arrivalTime = 0;
    }
    if(dict->objectForKey("startTime")){
        auto temp = dict->valueForKey("startTime")->doubleValue()/1000;
        if(temp!=0){
            m_startTime = GlobalData::shared()->changeTime(temp);
        }else{
            m_startTime = 0;
        }
    }else{
        m_startTime = 0;
    }
    
    if(dict->objectForKey("state")){
        m_state = dict->valueForKey("state")->intValue();
    }
    if(dict->objectForKey("soldiers")){
        m_soldiers = dynamic_cast<CCArray*>(dict->objectForKey("soldiers"));
    }
    if(m_soldiers==NULL){
       m_soldiers = CCArray::create();
    }
    if(dict->objectForKey("generals")){
        m_generals = dynamic_cast<CCArray*>(dict->objectForKey("generals"));
    }
    if(m_generals==NULL){
        m_generals = CCArray::create();
    }
    if(m_pic=="" || m_pic=="0"){
        m_pic = "g044";
    }
    
    m_open = false;
    m_index = 0;
}

string YuanJunInfo::getTeamPic(){
    if(m_pic=="" || m_pic=="0"){
        return "g044_team.png";
    }
    std::string head = m_pic;
    head.append("_team.png");
    return head;
}