//
//  AllianceTeamInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-18.
//
//

#include "AllianceTeamInfo.h"

void AllianceTeamInfo::parseInfo(CCDictionary* dict){
    m_targetCityLevel = 1;
    m_monsterCount = 0;
    if(dict->objectForKey("attackUid")){
        m_attackUid = dict->valueForKey("attackUid")->getCString();
    }
    if(dict->objectForKey("attackName")){
        m_attackName = dict->valueForKey("attackName")->getCString();
    }
    if(dict->objectForKey("targetPic")){
        m_targetPic = dict->valueForKey("targetPic")->getCString();
    }
    m_targetPicVer = dict->valueForKey("targetPicVer")->intValue();
    if(dict->objectForKey("targetAIcon")){
        m_targetAIcon = dict->valueForKey("targetAIcon")->getCString();
    }
    if(dict->objectForKey("waitTime")){
        auto temp = dict->valueForKey("waitTime")->doubleValue()/1000;
        if(temp!=0){
            m_waitTime = GlobalData::shared()->changeTime(temp);
        }else{
            m_waitTime = 0;
        }
    }
    if(dict->objectForKey("currSoldiers")){
        m_currSoldiers = dict->valueForKey("currSoldiers")->intValue();
    }
    if(dict->objectForKey("maxSoldiers")){
        m_maxSoldiers = dict->valueForKey("maxSoldiers")->intValue();
    }
    if(dict->objectForKey("targetUid")){
        m_targetUid = dict->valueForKey("targetUid")->getCString();
    }
    if(dict->objectForKey("targetId")){
        m_targetId = dict->valueForKey("targetId")->intValue();
    }
    if(dict->objectForKey("targetType")){
        m_targetType = dict->valueForKey("targetType")->intValue();
    }
    if(dict->objectForKey("attackId")){
        m_attackId = dict->valueForKey("attackId")->intValue();
    }
    if(dict->objectForKey("type")){
        m_type = dict->valueForKey("type")->intValue();
    }
    if(dict->objectForKey("targetName")){
        m_targetName = dict->valueForKey("targetName")->getCString();
    }
    if(dict->objectForKey("targetAName")){
        m_targetAName = dict->valueForKey("targetAName")->getCString();
    }
    if(dict->objectForKey("attackAName")){
        m_attackAName = dict->valueForKey("attackAName")->getCString();
    }
    if(dict->objectForKey("teamId")){
        m_teamId = dict->valueForKey("teamId")->getCString();
    }else{
        m_teamId = "";
    }
    
    if(dict->objectForKey("attackAIcon")){
        m_attackAIcon = dict->valueForKey("attackAIcon")->getCString();
    }
    if(dict->objectForKey("mySide")){
        m_mySide = dict->valueForKey("mySide")->intValue();
    }
    if(dict->objectForKey("attackAAbbr")){
        m_attackAAbb = dict->valueForKey("attackAAbbr")->getCString();
    }
    if(dict->objectForKey("attackPic")){
        m_attackPic = dict->valueForKey("attackPic")->getCString();
    }
    m_attackPicVer = dict->valueForKey("attackPicVer")->intValue();
    if(dict->objectForKey("targetAAbbr")){
        m_targetAAbbr = dict->valueForKey("targetAAbbr")->getCString();
    }
    if(dict->objectForKey("targetCityLevel")){
        m_targetCityLevel = dict->valueForKey("targetCityLevel")->intValue();
    }
    if (dict->objectForKey("territoryFight")) {
        m_territoryFlag = dict->valueForKey("territoryFight")->getCString();
        if (dict->objectForKey("territory")) {
            CCDictionary* territory = _dict(dict->objectForKey("territory"));
            if (dict->objectForKey("building")) {
                m_territoryType = dict->valueForKey("building")->getCString();
            }
            if (territory->objectForKey("name")) {
                m_territoryName = territory->valueForKey("name")->getCString();
            }
            if (!territory->objectForKey("name")) {
                if (territory->objectForKey("count")) {
                    int num = territory->valueForKey("count")->intValue();
                    m_territoryName = _lang_1("115312", CC_ITOA(num));
                }
                else {
                    if (m_territoryType == "tower") {
                        m_territoryName = _lang_1("115312", CC_ITOA(1));
                    }
                    else if (m_territoryType == "flag") {
                        m_territoryName = _lang("115534");
                    }
                }
            }
            if (territory->objectForKey("pic")) {
                m_territoryPic = territory->valueForKey("pic")->getCString();
            }
            if (!territory->objectForKey("pic")) {
                m_territoryPic = "";
            }
            if (territory->objectForKey("stat")) {
                m_territoryStat = territory->valueForKey("stat")->intValue();
            }
            if (!territory->objectForKey("stat")) {
                m_territoryStat = -1;
            }
            if (territory->objectForKey("allianceAbbr")) {
                m_territoryAlAbbr = territory->valueForKey("allianceAbbr")->getCString();
            }
            if (!territory->objectForKey("allianceAbbr")) {
                m_territoryAlAbbr = "";
            }
            if (territory->objectForKey("updateTime")) {
                m_territoryCrashTime = territory->valueForKey("updateTime")->doubleValue() / 1000;
            }
            if (!territory->objectForKey("updateTime")) {
                m_territoryCrashTime = 0;
            }
            if (territory->objectForKey("level")) {
                m_territoryLevel = territory->valueForKey("level")->intValue();
            }
            if (!territory->objectForKey("level")) {
                m_territoryLevel = 1;
            }
        }
    }
    if (!dict->objectForKey("territoryFight")) {
        m_territoryFlag = "";
        m_territoryName = "";
        m_territoryPic = "";
        m_territoryStat = -1;
        m_territoryAlAbbr = "";
        m_territoryCrashTime = 0;
        m_territoryLevel = 1;
        m_territoryType = "";
    }
    if(dict->objectForKey("uuid")){
        m_marchId = dict->valueForKey("uuid")->getCString();
    }
    if(dict->objectForKey("mType")){//todowdz
        m_marchType = dict->valueForKey("mType")->intValue();
    }
    m_reinfoceCount = dict->valueForKey("reinfoceCount")->intValue();
    m_reinforceMax = dict->valueForKey("reinforceMax")->intValue();
    if(dict->objectForKey("marchTime")){
        auto temp = dict->valueForKey("marchTime")->doubleValue()/1000;
        if(temp!=0){
            m_marchTime = GlobalData::shared()->changeTime(temp);
        }else{
            m_marchTime = 0;
        }
    }
    if(dict->objectForKey("createTime")){
        auto temp = dict->valueForKey("createTime")->doubleValue()/1000;
        if(temp!=0){
            m_createTime = GlobalData::shared()->changeTime(temp);
        }else{
            m_createTime = 0;
        }
    }
    
    if(m_teamId==""){
        if(m_mySide==1){
            m_battleType = 0;
        }else{
            m_battleType = 1;
        }
    }else{
        if(m_mySide==1){
            m_battleType = 2;
        }else{
            m_battleType = 3;
        }
    }
    if(dict->objectForKey("monsterCount")){
        m_monsterCount = dict->valueForKey("monsterCount")->intValue();
        if (m_monsterCount<=1) {
            m_battleType = 1;
        }else{
            m_battleType = 3;
        }
    }
    m_member = CCArray::create();
    if(dict->objectForKey("member")){
        CCArray* array = dynamic_cast<CCArray*>(dict->objectForKey("member"));
        if(array){
            int count = array->count();
            for (int i=0; i<count; i++) {
                auto dic = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(dic){
                    YuanJunInfo* info = new YuanJunInfo();
                    info->parseInfo(dic);
                    info->setIsLeader(m_attackUid==info->getUid());
                    info->setTeamSoldier(m_maxSoldiers - m_currSoldiers);
                    m_member->addObject(info);
                    CC_SAFE_RELEASE(info);
                }
            }
        }
    }
    m_reinforce = CCArray::create();
    if(dict->objectForKey("reinforce")){
        CCArray* reins = dynamic_cast<CCArray*>(dict->objectForKey("reinforce"));
        if(reins){
            int count = reins->count();
            for (int i=0; i<count; i++) {
                auto dic = dynamic_cast<CCDictionary*>(reins->objectAtIndex(i));
                if(dic){
                    YuanJunInfo* info = new YuanJunInfo();
                    info->parseInfo(dic);
                    m_reinforce->addObject(info);
                    CC_SAFE_RELEASE(info);
                }
            }
        }
    }
}


void AllianceTeamInfo::parseOnePersonInfo(CCDictionary* dict){

}

string AllianceTeamInfo::getAttAlliancePic(){
    std::string allianceIcon = "";
    if(m_attackAIcon==""){
        allianceIcon = "Allance_flay.png";
    }else{
        allianceIcon = m_attackAIcon;
        allianceIcon.append(".png");
    }
    return allianceIcon;
}

string AllianceTeamInfo::getTargetAlliancePic(){
    std::string allianceIcon = "";
    if(m_targetAIcon==""){
        allianceIcon = "Allance_flay.png";
    }else{
        allianceIcon = m_targetAIcon;
        allianceIcon.append(".png");
    }
    return allianceIcon;
}
