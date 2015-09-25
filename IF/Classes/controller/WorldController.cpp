//
//  WorldController.cpp
//  IF
//
//  Created by 邹 程 on 14-1-21.
//
//

#include "WorldController.h"
#include "WorldMapView.h"
#include "BattleView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "QueueController.h"
#include "GeneralManager.h"
#include "ArmyController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "TroopsController.h"
#include "MonsterDetailView.h"
#include "ScienceController.h"
#include "ScienceView.h"
#include "SceneController.h"
#include "GetAllServerListCommand.h"
#include "ServerListPopUpView.h"
#include "TimerController.h"
#include "SoundController.h"
#include "IFSkeletonDataManager.h"
#include "CCMathUtils.h"
#include "CCSafeNotificationCenter.h"
#include "TitanInfoCommand.h"
bool MarchInfo::isDelay() {
    string delayId = "501010";
    return isStatus(delayId);
}

bool MarchInfo::isLightning() {
    string lightningId = "501000";
    return isStatus(lightningId);
}

bool MarchInfo::isStatus(const string &esId) {
    auto es = effect.find(esId);
    if (es != effect.end()) {
        auto now = WorldController::getInstance()->getTime();
        return (now > es->second.first && now < es->second.second);
    }
    return false;
}

ccColor3B MarchInfo::getMarchColor() {
    unsigned int b = marchColor % 1000;
    unsigned int r = marchColor / 1000000;
    unsigned int g = (marchColor - r * 1000000) / 1000;
    return ccColor3B{(GLubyte)r,(GLubyte)g,(GLubyte)b};
}

double MarchInfo::getDigEndTime(){
    double time = digStartTime + troopMax * 1.0 / digSpeed * 1000;
    if(changeCollectSpdTime != 0){
        time = changeCollectSpdTime + (troopMax - digStartNum) * 1.0 / digSpeed * 1000;
    }
    return time;
}

void MarchInfo::calculateCurrentLine(CCPoint &wholeLineStart, CCPoint &wholeLineEnd, CCPoint &currentLineStart, CCPoint &currentLineEnd){
    if(m_timeInfo.size() == 1 || m_timeInfo.size() == 0){
        currentLineStart.x = wholeLineStart.x;
        currentLineStart.y = wholeLineStart.y;
        currentLineEnd.x = wholeLineEnd.x;
        currentLineEnd.y = wholeLineEnd.y;
    }else{
        //
        int currentIndex = getCurrentLineIndex();
        auto vector = WorldController::getInstance()->getCrossPointAtRelicGround(wholeLineStart, wholeLineEnd);
        if(vector.size() > currentIndex + 1){
            currentLineStart.x = vector[currentIndex].x;
            currentLineStart.y = vector[currentIndex].y;
            currentLineEnd.x = vector[currentIndex + 1].x;
            currentLineEnd.y = vector[currentIndex + 1].y;
        }
    }
}

void MarchInfo::parseMarchTime(CCArray *arr){
    m_timeInfo.clear();
    if(arr == NULL || arr->count() == 0){
        return;
    }
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj){
        auto dict = _dict(obj);
        MarchTimeInfo info = MarchTimeInfo();
//        info.startPoint = dict->valueForKey("sp")->intValue();
//        info.endPoint = dict->valueForKey("dp")->intValue();
        info.costTime = dict->valueForKey("mt")->intValue();
        m_timeInfo.push_back(info);
    }
}

double MarchInfo::getCurrentLineTimeLeft(){
    double now = WorldController::getInstance()->getTime();
    if(m_timeInfo.size() == 1 || m_timeInfo.size() == 0){
        return endStamp - now;
    }
    if (now >= endStamp) {
        return 1;
    }else{
        int index = 0;
        double time = startStamp;
        while (index < m_timeInfo.size()) {
            auto &timeInfo = m_timeInfo[index];
            time += timeInfo.costTime;
            if(time > now){
                return time - now;
            }
            index++;
        }
    }
    return 1;
}

int MarchInfo::getCurrentLineIndex(){
    double now = WorldController::getInstance()->getTime();
    if (now >= endStamp) {
        return 0;
    }else{
        int index = 0;
        double time = startStamp;
        while (index < m_timeInfo.size()) {
            auto &timeInfo = m_timeInfo[index];
            time += timeInfo.costTime;
            if(time > now){
                return index;
            }
            index++;
        }
    }
    return 0;
}

double MarchInfo::getCurrentLineTotalTime(){
    double totalTime = endStamp - startStamp;
    if(m_timeInfo.size() > 1){
        int index = getCurrentLineIndex();
        totalTime = m_timeInfo[index].costTime;
    }
    return totalTime;
}

CCPoint MarchInfo::calculateCurrentLineLeft(CCPoint &wholeLineStart, CCPoint &wholeLineEnd, CCPoint &currentPt){
    CCPoint lineStart;
    CCPoint lineEnd;
    calculateCurrentLine(wholeLineStart, wholeLineEnd, lineStart, lineEnd);
    return ccpSub(lineEnd, currentPt);
}

CCPoint MarchInfo::getCurrentPos(CCPoint &wholeLineStart, CCPoint &wholeLineEnd){
    CCPoint lineStart;
    CCPoint lineEnd;
    calculateCurrentLine(wholeLineStart, wholeLineEnd, lineStart, lineEnd);
    double timeLeft = getCurrentLineTimeLeft();
    double totalTime = getCurrentLineTotalTime();
    float s = 1 - timeLeft / totalTime;
    CCPoint pt = lineStart + (lineEnd - lineStart) * s;
    return pt;
}

void MarchInfo::updateSoldiers(CCDictionary *dict){
    if(dict == NULL){
        return;
    }
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("army"));
    if(!arr || arr->count() == 0){
        return;
    }
    marchSoldier.clear();
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj){
        auto soldier = _dict(obj);
        int type = soldier->valueForKey("armId")->intValue();
        if(type == 4)//fusheng 种类是泰坦
        {
            TitanInfoCommand *ticmd = new TitanInfoCommand();
            ticmd->sendAndRelease();
        }
        int total = soldier->valueForKey("total")->intValue();
        int dead = soldier->valueForKey("dead")->intValue();
        marchSoldier[type + 1].type = SOILDERTYPE(type + 1);
        marchSoldier[type + 1].total = total;
        marchSoldier[type + 1].dead = dead;
    }
    
    CCArray *walkArr = NULL;
    if(dict->objectForKey("leaderArmy")){
        walkArr = dynamic_cast<CCArray*>(dict->objectForKey("leaderArmy"));
    }
    if(!walkArr || walkArr->count() == 0){
        walkArr = dynamic_cast<CCArray*>(dict->objectForKey("army"));
    }

    walkSoldier.clear();
    CCObject *walkObj;
    CCARRAY_FOREACH(walkArr, walkObj){
        auto soldier = _dict(walkObj);
        int type = soldier->valueForKey("armId")->intValue();
        int total = soldier->valueForKey("total")->intValue();
        int dead = soldier->valueForKey("dead")->intValue();
        walkSoldier[type + 1].type = SOILDERTYPE(type + 1);
        walkSoldier[type + 1].total = total;
        walkSoldier[type + 1].dead = dead;
    }
}

void WorldCityInfo::setOriginInfo() {
    if(WorldMapView::instance() == NULL){
        return;
    }
    auto map = WorldMapView::instance()->m_map;
    if (map) {
        auto layer1 = map->layerNamed("ground");
        if (layer1) {
            int gid = layer1->tileGIDAt(cityTilePoint);
            auto info = map->propertiesForGID(gid);
            tileGid = gid;
            if (info.getType() == Value::Type::MAP) {
                auto terrainArr = ((info.asValueMap())["terrain"].asValueVector());
                auto terrain =  (terrainArr.back().asString());
                auto terrainInfo = dynamic_cast<CCDictionary*>(layer1->getTileSet()->m_terrainInfoDict->objectForKey(terrain.c_str()));
                if (terrainInfo) {
                    string name = terrainInfo->valueForKey("name")->getCString();
                    string type = terrainInfo->valueForKey("type")->getCString();
                    cityName = name;
                    cityOriginType = type;
                }
            }
        }
        if(cityOriginType != "space" && cityOriginType != ""){
            return;
        }
        auto layer = map->layerNamed("tile");
        if (layer) {
            int gid = layer->tileGIDAt(cityTilePoint);
            tileGid = gid;
//            todo tile
            auto info = map->propertiesForGID(gid);
            if (info.getType() == Value::Type::MAP) {
                auto terrainArr = ((info.asValueMap())["terrain"].asValueVector());
                auto terrain =  (terrainArr.back().asString());
                auto terrainInfo = dynamic_cast<CCDictionary*>(layer->getTileSet()->m_terrainInfoDict->objectForKey(terrain.c_str()));
                if (terrainInfo) {
                    string name = terrainInfo->valueForKey("name")->getCString();
                    string type = terrainInfo->valueForKey("type")->getCString();
                    cityName = name;
                    cityOriginType = type;
                }
            }
        }

    }
}

void WorldCityInfo::setCityIndex(unsigned int index){
    cityIndex = index;
    cityTilePoint = WorldController::getPointByIndex(index);
}

void WorldCityInfo::setInfo(unsigned int index,int ServerId){
    if (index < 1 || index > WorldController::getInstance()->_current_index_limit) {
        return;
    }
    cityIndex = index;
    tileServerId = ServerId;
    CCPoint tempPoint = WorldController::getPointByIndex(cityIndex);
    
//    CCLOG("log server:%d,cityindex:%d,pointx%f,pointy%f",tileServerId,cityIndex,tempPoint.x,tempPoint.y);
    
    cityTilePoint = WorldController::getPointByIndex(index);
    setOriginInfo();
    cityType = OriginTile;
}

void WorldCityInfo::setInfo(Json *json,bool isRange  ,int ServerId) {
    unsigned int index = Json_getInt(json, "i", 0);
    tileServerId = ServerId;
    CCPoint tempPoint = WorldController::getPointByIndex(cityIndex);
//    CCLOG("server:%d,cityindex:%d,pointx%f,pointy%f",tileServerId,index,tempPoint.x,tempPoint.y);
    if (index < 1 || index > WorldController::getInstance()->_current_index_limit) {
        return;
    }
    
    if (WorldMapView::instance() && !cityIndex) {
        cityIndex = index;
        cityTilePoint = WorldController::getPointByIndex(index);
        setOriginInfo();
    }
    unsigned int resType = Json_getInt(json, "t", 0);
    cityType = (WorldCityType)resType;
    
    string asn = Json_getString(json, "asn", "");
    leagueAsn = asn;
    
    string aName = Json_getString(json, "afn", "");
    allianceName = aName;

    mc_state = Json_getInt(json, "mc_state", 0);
    string ownerName = Json_getString(json, "o","");
    pic = Json_getString(json, "pic","");
    cross = Json_getInt(json, "cross", -1);
    srcServer = Json_getInt(json, "srcServer", -1);
    winNum = Json_getInt(json, "winNum", -1);
    
    if (!ownerName.empty()) {
        auto &selfInfo = GlobalData::shared()->playerInfo;
        auto &player = WorldController::getInstance()->m_playerInfo[ownerName];
        playerName = ownerName;
        if(cityType == CityTile || cityType == ResourceTile || cityType == CampTile){
            const char* allianceId = Json_getString(json, "ai", 0);
            if (ownerName == selfInfo.name) {
                player.type = PlayerSelf;
            } else if (allianceId && selfInfo.isInAlliance() && allianceId == selfInfo.getAllianceId()) {
                player.type = PlayerAlliance;
            } else {
                player.type = PlayerOther;
            }
            if(allianceId){
                player.allianceLong = allianceId;
            }else{
                player.allianceLong = "";
            }
        }
    }
     m_aArea_isActive = Json_getInt(json, "tactive", 0);//领地范围是否激活
     m_aArea_name = Json_getString(json, "tname","");//领地联盟名称
     m_aArea_simpleName = Json_getString(json, "tabbr","");//领地联盟简称
     m_aArea_flag = Json_getString(json, "ticon","");//领地联盟旗帜
     m_aArea_lv = Json_getInt(json, "tlv", -1);//等级
     m_aArea_nickname = Json_getString(json, "tnickname","");//名称
    if(!isRange){
        m_aArea_id = Json_getString(json, "taid", "");//领地联盟id
        m_aArea_direction = Json_getString(json, "tdirection","");
        m_aArea_pointId = Json_getInt(json, "tid", -1);//领地中心点所在坐标
    }
//    CCLOG("logInfo=====cityIndex---%d",index);
    auto _point = WorldController::getPointByIndex(index);
    int pointx = _point.x;
    int pointy = _point.y;
//    CCLOG("logInfo=====point:%d%d",pointx,pointy);
//    CCLOG("logInfo=====dirInfo---%s",m_aArea_direction.c_str());
//    CCLOG("logInfo=====type %d",resType);
//    CCLOG("logInfo=====taid %s",m_aArea_id.c_str());
    
    
    switch (cityType) {
        case Throne:{
            auto &player = WorldController::getInstance()->m_playerInfo[ownerName];
            std::string pic = Json_getString(json, "throne_pic", "301");
            throne_pic = pic;
            std::string oid = Json_getString(json, "oid", "");
            kingBuildInfo.uid = oid;
            std::string aid = Json_getString(json, "ai", "");
            kingBuildInfo.allianceUid = aid;
            double openTime = Json_getInt(json, "opt", 0);
            double startTime = Json_getInt(json, "st", 0);
            double endTime = Json_getInt(json, "et", 0);
            kingBuildInfo.openTime = openTime * 1000;
            kingBuildInfo.startTime = startTime * 1000;
            kingBuildInfo.endTime = endTime * 1000;
            string allianceShort = Json_getString(json, "asn", "");
            player.allianceShort = allianceShort;
            std::string allianceFlag = Json_getString(json, "aic", "");
            player.allianceFlag = allianceFlag;
            if(GlobalData::shared()->playerInfo.isInSelfServer()){
                WorldController::getInstance()->updateWorldActivityTime(json, FIGHT_OF_KING);
            }
        }
            break;
        case Trebuchet:{
            auto &player = WorldController::getInstance()->m_playerInfo[ownerName];
            std::string oid = Json_getString(json, "oid", "");
            trebuchetInfo.uid = oid;
            std::string aid = Json_getString(json, "ai", "");
            trebuchetInfo.allianceUid = aid;
            double openTime = Json_getInt(json, "opt", 0);
            double startTime = Json_getInt(json, "st", 0);
            double endTime = Json_getInt(json, "et", 0);
            trebuchetInfo.openTime = openTime * 1000;
            trebuchetInfo.startTime = startTime * 1000;
            trebuchetInfo.endTime = endTime * 1000;
            string allianceShort = Json_getString(json, "asn", "");
            player.allianceShort = allianceShort;
            std::string allianceFlag = Json_getString(json, "aic", "");
            player.allianceFlag = allianceFlag;
            if(GlobalData::shared()->playerInfo.isInSelfServer()){
                WorldController::getInstance()->updateWorldActivityTime(json, FIGHT_OF_KING);
            }

            auto centerPt = ccp(int(WorldController::getInstance()->_current_tile_count_x / 2), int(WorldController::getInstance()->_current_tile_count_y / 2));//map center
            if(centerPt.x > cityTilePoint.x){
                if(centerPt.y > cityTilePoint.y){
                    trebuchetInfo.direction = 3;
                }else{
                    trebuchetInfo.direction = 2;
                }
            }else{
                if(centerPt.y > cityTilePoint.y){
                    trebuchetInfo.direction = 4;
                }else{
                    trebuchetInfo.direction = 1;
                }
            }
            trebuchetInfo.state = Wait;
        }
            break;
        case OriginTile:{
            // if there is a big king texture cover this tile , it will has a cover index
            string cover = Json_getString(json, "t1", "");
            if (!cover.empty()) {
                unsigned int coverIndex = atoi(cover.c_str());
                cityIcon = coverIndex*10;
            }
        }
            break;
        case CityTile:{
            cityIcon = 6;
            auto &player = WorldController::getInstance()->m_playerInfo[ownerName];
            player.cityIndex = index;
            int protectStampSec = Json_getInt(json, "pt", 0);
            player.protectStamp = (double)protectStampSec*1000.0;
            int fireStampSec = Json_getInt(json, "ft", 0);
            player.fireStamp = (double)fireStampSec*1000.0;
            double resourceProtectedTime = Json_getInt(json, "pt_resource", 0);
            player.resourceProtectStamp = resourceProtectedTime * 1000.0;
            unsigned int lv = Json_getInt(json, "l", 0);
            player.cityLv = lv;
            string allianceShort = Json_getString(json, "asn", "");
            player.allianceShort = allianceShort;
            std::string allianceFlag = Json_getString(json, "aic", "");
            player.allianceFlag = allianceFlag;
            std::string officer = Json_getString(json, "officer", "");
            player.officer = officer;
            unsigned int userType = Json_getInt(json, "userType", 0);
            player.userType = userType;
        }
            break;
        case CampTile:{
            cityIcon = 7;
        }
            break;
        case ResourceTile:{
            unsigned int resValue = Json_getInt(json, "v", 0);
            unsigned int refresh = Json_getInt(json, "rt", 0);
            double rStamp = (double)refresh*1000.0;
            auto now = WorldController::getInstance()->getTime();
            citySubType = Json_getString(json, "t1", "");
            
            if (!resValue || citySubType.empty() || (now > rStamp && ownerName.empty())) {
                // empty resource to blank land
                cityIcon = -1;
                cityType = OriginTile;
            } else {
                cityIcon = atoi(citySubType.c_str());
                resource.refreshStamp = rStamp;
                unsigned int resLv = Json_getInt(json, "l", 0);
                
                resource.lv = resLv;
                resource.sum = resValue;
                resource.type = (WorldResourceType)cityIcon;
            }
        }
            break;
        case MonsterTile:{
            cityIcon = 8;
            unsigned int refresh = Json_getInt(json, "rt", 0);
            unsigned int start = Json_getInt(json, "st", 0);
            double rStamp = (double)refresh * 1000.0;
            double sStamp = (double)start * 1000.0;
            auto now = WorldController::getInstance()->getTime();
            if (now > rStamp || now < sStamp) {
                // monster need to refresh
                cityIcon = -1;
                cityType = OriginTile;
            } else {
                refreshStamp = rStamp;
                startStamp = sStamp;
            }
        }
            break;
        case FieldMonster:{
            unsigned int refresh = Json_getInt(json, "rt", 0);
            unsigned int start = Json_getInt(json, "st", 0);
            double rStamp = (double)refresh * 1000.0;
            double sStamp = (double)start * 1000.0;
            int currentHp = Json_getInt(json, "mchp", 0);

            auto now = WorldController::getInstance()->getTime();
            if (now > rStamp || now < sStamp || currentHp == 0) {
                // monster need to refresh
                cityIcon = -1;
                cityType = OriginTile;
            } else {
                refreshStamp = rStamp;
                startStamp = sStamp;
            }
            std::string monsterId = Json_getString(json, "t1", "");
            int totalHp = Json_getInt(json, "mmhp", 1);
            fieldMonsterInfo.monsterId = monsterId;
            fieldMonsterInfo.totalHp = totalHp;
            fieldMonsterInfo.currentHp = currentHp;
            fieldMonsterInfo.isHpChange = false;
        }
            break;
        case ActBossTile:{
            unsigned int refresh = Json_getInt(json, "rt", 0);
            unsigned int start = Json_getInt(json, "st", 0);
            double rStamp = (double)refresh * 1000.0;
            double sStamp = (double)start * 1000.0;
            int currentHp = Json_getInt(json, "mchp", 0);
            
            auto now = WorldController::getInstance()->getTime();
            if (now > rStamp || now < sStamp || currentHp == 0) {
                // monster need to refresh
                cityIcon = -1;
                cityType = OriginTile;
            } else {
                refreshStamp = rStamp;
                startStamp = sStamp;
            }
            std::string monsterId = Json_getString(json, "t1", "");
            int totalHp = Json_getInt(json, "mmhp", 1);
            fieldMonsterInfo.monsterId = monsterId;
            fieldMonsterInfo.totalHp = totalHp;
            fieldMonsterInfo.currentHp = currentHp;
            fieldMonsterInfo.isHpChange = false;
        }
            break;
        case Tile_allianceArea:{
            m_allianceAreaInfo.tuid =  Json_getString(json, "tuid","");
            m_allianceAreaInfo.state = (AAreaState)Json_getInt(json,"tstat" , -1);
            m_allianceAreaInfo.updateTime = Json_getInt(json,"tutime" , -1) * 1000;
            m_allianceAreaInfo.startTime = Json_getInt(json,"tstime" , -1) * 1000;
            m_allianceAreaInfo.defence = Json_getInt(json,"tdef" , -1);
            m_allianceAreaInfo.defenceMax = Json_getInt(json,"tdefmax" , -1);
            m_allianceAreaOwnerInfo.allianceId = Json_getString(json, "ai", "");
            m_allianceAreaOwnerInfo.ownerId = Json_getString(json, "oid", "");
            m_allianceAreaInfo.range = Json_getInt(json,"trange" , 0);
            m_allianceAreaInfo.dialogId = Json_getInt(json,"tcount" , 0);
            m_aArea_id = Json_getString(json, "taid", "");
//            CCLOG("range-----%d",m_allianceAreaInfo.range);
        }
            break;
        case tile_superMine:{
            m_superMineInfo.truid = Json_getString(json, "truid","");
            m_superMineInfo.trutime = Json_getInt(json,"trutime" , -1) * 1000;
            m_superMineInfo.trstime = Json_getInt(json,"trstime" , -1) * 1000;
            m_superMineInfo.trdef = Json_getInt(json,"trdef" , -1);
            m_superMineInfo.trdefmax = Json_getInt(json,"trdefmax" , -1);
            m_superMineInfo.trstat = (AAreaState)Json_getInt(json,"trstat" , -1);
            m_superMineInfo.type = (WorldResourceType)Json_getInt(json,"tres" , -1);
            m_allianceAreaOwnerInfo.allianceId = Json_getString(json, "ai", "");
            m_allianceAreaOwnerInfo.ownerId = Json_getString(json, "oid", "");
            m_aArea_id = Json_getString(json, "taid", "");
        }
            break;
        case tile_tower:{
            m_towerInfo.uid = Json_getString(json, "ttuid","");
            m_towerInfo.updateTime = Json_getInt(json,"ttutime" , -1) * 1000;
            m_towerInfo.startTime = Json_getInt(json,"ttstime" , -1) * 1000;
            m_towerInfo.defence = Json_getInt(json,"ttdef" , -1);
            m_towerInfo.defenceMax = Json_getInt(json,"ttdefmax" , -1);
            m_towerInfo.state =  (AAreaState)Json_getInt(json,"ttstat" , -1);
            m_towerInfo.dialog = Json_getInt(json,"ttcount", 0);//TODO:WDZ
            m_allianceAreaOwnerInfo.allianceId = Json_getString(json, "ai", "");
            m_allianceAreaOwnerInfo.ownerId = Json_getString(json, "oid", "");
            m_aArea_id = Json_getString(json, "taid", "");
        }
            break;
        case tile_banner:{
            m_bannerInfo.uid = Json_getString(json, "tfuid","");
            m_bannerInfo.updateTime = Json_getInt(json,"tfutime" , -1) * 1000;
            m_bannerInfo.startTime = Json_getInt(json,"tfstime" , -1) * 1000;
            m_bannerInfo.defence = Json_getInt(json,"tfdef" , -1);
            m_bannerInfo.defenceMax = Json_getInt(json,"tfdefmax" , -1);
            m_bannerInfo.state =  (AAreaState)Json_getInt(json,"tfstat" , -1);
            m_bannerInfo.banner = Json_getString(json, "tflag", "");
            m_allianceAreaOwnerInfo.allianceId = Json_getString(json, "ai", "");
            m_allianceAreaOwnerInfo.ownerId = Json_getString(json, "oid", "");
            m_aArea_id = Json_getString(json, "taid", "");
        }
            break;
        case tile_wareHouse:{//////////////
            m_warehouseInfo.uid = Json_getString(json, "twuid", "");
            m_warehouseInfo.updateTime = Json_getInt(json, "twutime", -1) * 1000;
            m_warehouseInfo.startTime = Json_getInt(json, "twstime", -1) * 1000;
            m_warehouseInfo.defence = Json_getInt(json, "twdef", -1);
            m_warehouseInfo.defenceMax = Json_getInt(json, "twdefmax", -1);
            m_warehouseInfo.state = (AAreaState)Json_getInt(json, "twstat", -1);
            m_allianceAreaOwnerInfo.allianceId = Json_getString(json, "ai", "");
            m_allianceAreaOwnerInfo.ownerId = Json_getString(json, "oid", "");
            m_aArea_id = Json_getString(json, "taid", "");
        }
            break;
        default:
            break;
    }
}

static WorldController* _instance = NULL;
WorldController* WorldController::getInstance() {
    if (!_instance) {
        _instance = new WorldController();
        _instance->initCityInfo();
    }
    return _instance;
}

void WorldController::purge() {
    isInWorld = false;
    isRefreshQueue = false;
    m_cityInfo.clear();
    isInfoTake = false;
    m_monsterAttackInfo->removeAllObjects();
    initWorld = false;
    initKingFlag = true;
    IFSkeletonDataManager::shared()->purge();
    
    std::map<std::string, int> doNotDelete;
    
    //删除不是自己类型的行军信息，如果有组队，组队的信息也要保留
    for (auto it = m_marchInfo.begin(); it != m_marchInfo.end(); it++) {
        if(it->second.uuid != "" && it->second.ownerType == PlayerSelf){
            doNotDelete[it->first] = 1;
            std::string teamUid = it->second.teamUid;
            if(teamUid != "" && m_marchInfo.find(teamUid) != m_marchInfo.end()){
                doNotDelete[teamUid] = 1;
            }
        }
    }
    
    for (auto it = m_marchInfo.begin(); it != m_marchInfo.end();) {
        if(doNotDelete.find(it->first) != doNotDelete.end()){
            ++it;
        }else{
            m_marchInfo.erase(it++);
        }
    }
}

unsigned int WorldController::getMarchTag() {
    return marchTagCounter++;
}

void WorldController::updateWorldActivityTime(CCDictionary *dict){
    WorldActivityType type = WorldActivityType(dict->valueForKey("t")->intValue());
    double openTime = dict->valueForKey("opt")->doubleValue();
    double startTime = dict->valueForKey("st")->doubleValue();
    double endTime = dict->valueForKey("et")->doubleValue();
    updateWorldActivityTime(type, openTime, startTime, endTime);
}

void WorldController::updateWorldActivityTime(Json *json, WorldActivityType type){
    double openTime = Json_getInt(json, "opt", 0);
    double startTime = Json_getInt(json, "st", 0);
    double endTime = Json_getInt(json, "et", 0);
    updateWorldActivityTime(type, openTime, startTime, endTime);
}

void WorldController::updateWorldActivityTime(WorldActivityType type, double openTime, double startTime, double endTime){
    auto it = m_worldActivity.find(type);
    if(it != m_worldActivity.end()){
        auto &info = it->second;
        info.timeInfo.startTime = startTime * 1000.0;
        info.timeInfo.endTime = endTime * 1000.0;
        info.timeInfo.openTime = openTime * 1000.0;
        info.currentState = getKingActivityStateByType(type);
    }else{
        auto info = WorldActivityInfo();
        info.timeInfo.startTime = startTime * 1000.0;
        info.timeInfo.endTime = endTime * 1000.0;
        info.timeInfo.openTime = openTime * 1000.0;
        info.currentState = getKingActivityStateByType(type);
        WorldController::getInstance()->m_worldActivity[type] = info;
    }
}

WorldActivityState WorldController::getKingActivityStateByType(WorldActivityType type){
    auto it = m_worldActivity.find(type);
    if(it == m_worldActivity.end()){
        return NotOpen;
    }
    auto &info = it->second;
    WorldActivityState state = getKingActivityState(info.timeInfo.openTime, info.timeInfo.startTime, info.timeInfo.endTime);
    return state;
}

WorldActivityState WorldController::getKingActivityState(double openTime, double startTime, double endTime){
    double time = WorldController::getInstance()->getTime();
    if(openTime == 0){
        return NotOpen;
    }
    if(time < openTime){
            return OpenNoKing;
    }else{
        if(time >= startTime && time <= endTime){
                return WarTime;
        }
        return PeaceTime;
    }
    return NotOpen;
}

void WorldController::initCityInfo() {
    this->currentStamine = -1;
    lastStamineTime = -1;
    lastInputX = -1;
    lastInputY = -1;
    lastInputZ = -1;
    openTargetIndex = -1;
    openMarchId = "";
    playAniTargetIndex = -1;
    buildAllianceTerritory = 0;
    allianceTerritoryId = "";
    allianceTerritoryIndex = -1;
    allianceTerritoryBanner = "";
    m_bAAreaCost = 0;
    m_monsterAttackInfo = CCArray::create();
    isInfoTake = false;
    if (m_cityInfo.size() > 0) {
        m_cityInfo.clear();
    }
    startTimeStampOffset = 0.0;
    marchTagCounter = 1;
    currentMapZoom = 1.0;
    if (CCCommonUtils::isIosAndroidPad()) {

        currentMapZoom = WORLD_DEFAULT_HD_SCALE;
    }
    initTime = false;
    initWorld = false;
    initKingFlag = true;
    forceUPdate = false;
    firstInWorld = false;
    forceUpdateTerritoryTile = false;
    forceUpdateTerritoryInfo = false;
    forceUpdateTerritoryResourceTile = false;
    forceUpdateTerritoryTowerTile = false;
    forceUpdateTerritoryWarehouseTile = false;
    forceUpdateTerritoryBannerTile = false;
    forceUpdateTerritoryResourceDetail = false;
    marchingAlertFlag = false;
    // todo : init city info
    serverMap_count_x = 0;
    serverMap_count_y = 0;
    serverMap_centerPoint = CCPointZero;
    serverMap_centerViewPoint = CCPointZero;
    serverMap_width = 0;
    serverMap_height = 0;
    m_isAsyEnd = true;
    m_helpCmdVec.clear();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void WorldController::update(float dt) {
    if (!initTime) {
        return;
    }

    initWorld = (initWorld && WorldMapView::instance());
    auto now = getTime();
    int addNum = 0;
    for (auto it = m_marchInfo.begin(); it != m_marchInfo.end();) {
        auto &marchInfo = it->second;
        if(marchInfo.uuid == ""){
            ++it;
            continue;
        }
        if(marchInfo.marchType == MethodRally && marchInfo.uuid != marchInfo.teamUid && marchInfo.stateType == StateMarch){
            ++it;
            continue;
        }
        if(marchInfo.stateType == StateReturn && marchInfo.uuid != marchInfo.teamUid && marchInfo.marchType == MethodRally){
            auto leaderIt = m_marchInfo.find(marchInfo.teamUid);
            if(leaderIt != m_marchInfo.end()){
                if(leaderIt->second.stateType == StateMarch
                   || ((leaderIt->second.stateType == StateReturn || leaderIt->second.stateType == StateOccupy)
                   && (leaderIt->second.action == MarchActionPrepareAttack || leaderIt->second.action == MarchActionAttack))){
                    ++it;
                    continue;
                }
            }
        }
        if (initWorld && addNum < MarchAddPerFrame) {
            if(WorldMapView::instance()->updateMarchTarget(marchInfo, now, dt)){
                addNum++;
            }
        }
        if(checkOverTimeMarch(marchInfo, dt)){
            bool isSelf = (it->second.ownerType == PlayerSelf);
            std::string uid = it->second.uuid;
            CCLOG("logInfo===========================deleteMarchUid    %s",uid.c_str());
            CCLOG("logInfo===========================deleteMarchTag    %d",it->second.marchTag);
            if (initWorld){
                it->second.action = MarchActionNone;
                WorldMapView::instance()->updateMarchTarget(marchInfo, now, dt);
            }
            m_marchInfo.erase(it++);
            if(isSelf){
                TimeUpMarchProcesser::getInstance()->addTimeUpMarch(uid);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
            }
        }else{
            TimeUpMarchProcesser::getInstance()->check(marchInfo);
            ++it;
        }
    }
    autoAddStamine();
    TimeUpMarchProcesser::getInstance()->refresh();
    
    auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
    if(activityIt != WorldController::getInstance()->m_worldActivity.end()){
        auto &activityInfo = activityIt->second;
        bool stateChange = false;
        WorldActivityState state = WorldController::getInstance()->getKingActivityStateByType(FIGHT_OF_KING);
        if(activityInfo.currentState != state){
            stateChange = true;
        }
        activityInfo.currentState = state;
        if(stateChange && !initKingFlag){
            TimerController::getInstance()->showSystemNotice(activityInfo.currentState);
        }
        initKingFlag = false;
    }
}

//用于出征信息的数据纠正
bool WorldController::checkOverTimeMarch(MarchInfo &info, float dt){
    auto now = getTime();
    bool delFlag = false;
    bool updateInfoFlag = false;
    bool updateQueueFlag = false;
    int delaySec = 10;
    if(info.ownerType == PlayerSelf){
        //self march
        if(info.stateType == StateReturn){//目标回家
            if(now + dt - 100 > info.endStamp){
                delFlag = true;
                updateInfoFlag = true;
            }
        }else if(info.stateType == StateOccupy){
            if(info.targetType == ResourceTile){//目标采矿到时间没有返回
                auto digEndTime = info.getDigEndTime();
                if(digEndTime < now - delaySec * 1000){
                    unsigned int tmp = info.startPointIndex;
                    info.startPointIndex = info.endPointIndex;
                    info.endPointIndex = tmp;
                    info.stateType = StateReturn;
                    info.startStamp = digEndTime;
                    info.endStamp = digEndTime + getMarchTime(info);
                    info.mt = info.endStamp - info.startStamp;
                    updateQueueFlag = true;
                }
            }else if(info.targetType == MonsterTile){//目标迷宫到时间没有返回
                if(info.mazeEndTime < now - delaySec * 1000){
                    unsigned int tmp = info.startPointIndex;
                    info.startPointIndex = info.endPointIndex;
                    info.endPointIndex = tmp;
                    info.stateType = StateReturn;
                    info.startStamp = info.mazeEndTime;
                    info.endStamp = info.mazeEndTime + getMarchTime(info);
                    info.mt = info.endStamp - info.startStamp;
                    updateQueueFlag = true;
                }
            }
        }else if(info.stateType == StateMarch){
            bool returnFlag = false;
            if((info.marchType == MethodBattle && info.targetType == CityTile)//出征城市
               || info.marchType == MethodScout//侦查
               || info.marchType == MethodDeal//交易
               || info.marchType == MethodFieldMonster//打野怪
               || info.marchType == MethodWarehouse//联盟仓库
            ){
                if(info.endStamp < now - delaySec * 1000){
                    returnFlag = true;
                }
            }
            if(returnFlag){
                unsigned int tmp = info.startPointIndex;
                info.startPointIndex = info.endPointIndex;
                info.endPointIndex = tmp;
                info.stateType = StateReturn;
                info.startStamp = info.endStamp;
                info.endStamp = info.endStamp + getMarchTime(info);
                info.mt = info.endStamp - info.startStamp;
                updateQueueFlag = true;
            }
        }
    }else if(info.ownerType == PlayerOther && info.marchType == MethodHeiqishi){
        if(info.endStamp < now - 6 * 1000){
            return true;
        }
    }
    else{
        //其他人的数据
        if(info.stateType == StateReturn && now + dt - 100 > info.endStamp){//目标回家
            delFlag = true;
        }
    }
    
    if(updateInfoFlag && info.uuid != "1"  && info.uuid != "2" && info.uuid != "3" ){
        updateTroop(info.uuid);
        switch (info.targetType)
        {
            case CampTile:
                CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                break;
            case ResourceTile:
                CCCommonUtils::flyHint("march_icon.png", _lang("136004"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                break;
            case MonsterTile:
                CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                break;
            case FieldMonster:
                CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                break;
            case ActBossTile:
                CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                break;
            case Tile_allianceArea:
                if (info.endPointIndex == WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint)) {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                }
                else
                    CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                break;
            case tile_superMine:
                if (info.endPointIndex == WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint)) {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                }
                else
                    CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                break;
            case tile_tower:
                if (info.endPointIndex == WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint)) {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                }
                else
                    CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                break;
            case tile_banner:
                if (info.endPointIndex == WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint)) {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                }
                else
                    CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                break;
            case tile_wareHouse:
                if (info.endPointIndex == WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint)) {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                }
                else
                    CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                break;
            case CityTile:
            {
                switch (info.marchType) {
                    case MethodScout:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136007"), _lang_1("136008", info.playerName.c_str()), 3, 0, true);
                        break;
                    case MethodBattle:
                    case MethodUnion:
                    case MethodRally:
                        if(!info.isKilled)
                        {
                            CCCommonUtils::flyHint("march_icon.png", _lang("136002"), _lang_1("136003", info.playerName.c_str()), 3, 0, true);
                        }
                        break;
                    case MethodDeal:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136016"), _lang("136012"), 3, 0, true);
                        break;
                    case MethodWarehouse:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136016"), _lang("136012"), 3, 0, true);
                        break;
                    case MethodYuanSolider:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136011"), _lang("136012"), 3, 0, true);
                        break;
                    default:
                        break;
                }
                
            }
                break;
            default:
                break;
        }
        SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
    }
    if(info.ownerType == PlayerSelf){
        if(updateQueueFlag){
            QueueController::getInstance()->addMarchQueueInfo(info.uuid);
        }
    }
    return delFlag;
}

void WorldController::initMarchInfo(cocos2d::CCArray *myMarchArr,int serverId) {
    if (!myMarchArr) {
        return;
    }
    if (serverId == -1) {
        serverId = GlobalData::shared()->playerInfo.selfServerId;
    }
    CCObject* obj;
    CCARRAY_FOREACH(myMarchArr, obj) {
        auto info = dynamic_cast<CCDictionary*>(obj);
        string uuid = info->valueForKey("uuid")->getCString();
        unsigned int staPoint = info->valueForKey("sp")->intValue();
        unsigned int desPoint = info->valueForKey("dp")->intValue();
        double startTime = info->valueForKey("st")->doubleValue();
        double endTime = info->valueForKey("et")->doubleValue();
        double mt = endTime - startTime;
        if(info->objectForKey("mt")){
            mt = info->valueForKey("mt")->doubleValue();
        }
        double treasureMapTime = 0;
        if(info->objectForKey("treasureMapTime")){
            treasureMapTime  = info->valueForKey("treasureMapTime")->intValue();
        }
        double treasureMapFTime = 0;
        if(info->objectForKey("treasureMapFinishTime")){
            treasureMapFTime =info->valueForKey("treasureMapFinishTime")->doubleValue();
        }
            
        CCArray *arr = dynamic_cast<CCArray*>(info->objectForKey("mt2"));

        double allianceTeamCreateTime = -1;
        
        std::string asn = "";
        if(info->objectForKey("asn")){
            asn = info->valueForKey("asn")->getCString();
        }
        if(info->objectForKey("tct")){
            allianceTeamCreateTime = info->valueForKey("tct")->doubleValue();
        }
        MarchStateType stateType = (MarchStateType)info->valueForKey("sType")->intValue();

        std::string team = "";
        if(info->objectForKey("team")){
            team = info->valueForKey("team")->getCString();
        }

        MarchMethodType marchType = (MarchMethodType)info->valueForKey("mType")->intValue();
        WorldCityType targetType = (WorldCityType)info->valueForKey("tType")->intValue();
        PlayerType ownerType = (PlayerType)info->valueForKey("oType")->intValue();
        string playerName = info->valueForKey("o")->getCString();
        std::string desLeague = "";
        if(info->objectForKey("tali")){
            desLeague = info->valueForKey("tali")->getCString();
        }
        
        unsigned int marchColor = info->valueForKey("color")->intValue();
        map<string,pair<double, double> > tmpEffect;
        if (info->objectForKey("es")) {
            auto esInfo = _dict(info->objectForKey("es"));
            CCDictElement* dictObj;
            CCDICT_FOREACH(esInfo, dictObj) {
                string id = dictObj->getStrKey();
                auto esTime = _dict(dictObj->getObject());
                double st = esTime->valueForKey("st")->intValue();
                double et = esTime->valueForKey("et")->intValue();
                tmpEffect.insert(pair<string, pair<double, double> > (id,pair<double, double>(st,et)));
            }
        }
        string ouid("");
        if (info->objectForKey("ouid")) {
            ouid = info->valueForKey("ouid")->getCString();
        }
        int  pMonsterLevel = 0;
        if (info->objectForKey("monsterLevel")) {
            pMonsterLevel = info->valueForKey("monsterLevel")->intValue();
        }
        int  pMonsterCount = 0;
        if (info->objectForKey("monsterCount")) {
            pMonsterCount = info->valueForKey("monsterCount")->intValue();
        }
        auto& marchInfo = WorldController::getInstance()->m_marchInfo[uuid];
        marchInfo.uuid = uuid;
        marchInfo.marchToIndexLeague = desLeague;
        marchInfo.startPointIndex = staPoint;
        marchInfo.endPointIndex = desPoint;
        marchInfo.startStamp = startTime;
        marchInfo.asn = asn;
        marchInfo.endStamp = endTime;
        marchInfo.mt = mt;
        marchInfo.allianceTeamCreateTime = allianceTeamCreateTime;
        marchInfo.teamUid = team;
        marchInfo.marchType = marchType;
        marchInfo.stateType = stateType;
        marchInfo.targetType = targetType;
        marchInfo.ownerType = ownerType;
        marchInfo.effect = tmpEffect;
//        marchInfo.isChangingSpeed = true;
        marchInfo.playerName = playerName;
        marchInfo.updateSoldiers(info);
        marchInfo.marchColor = marchColor;
        marchInfo.parseMarchTime(arr);
        marchInfo.ouid = ouid;
        marchInfo.monsterLevel = pMonsterLevel;
        marchInfo.monsterCount = pMonsterCount;
        marchInfo.serverId = serverId;
        marchInfo.treasureMapFinishTime = treasureMapFTime;
        marchInfo.treasureMapTime = treasureMapTime;
        if (marchInfo.marchTag == 0) {
            marchInfo.marchTag = getMarchTag();
        }
        //领主技能
        bool slowDown=false;
        if (info->objectForKey("slowDown")) {
            slowDown = info->valueForKey("slowDown")->boolValue();
        }
        marchInfo.slowDown = slowDown;
        // init self march state
        if (ownerType == PlayerSelf) {
            switch (stateType) {
                case StateMarch: {
                    QueueController::getInstance()->addMarchQueueInfo(marchInfo.uuid);
                }
                    break;
                case StateOccupy: {
                    m_selfMarchUuid[desPoint] = marchInfo.uuid;
                    if(info->objectForKey("q")){
                        marchInfo.troopMax = info->valueForKey("q")->intValue();
                        marchInfo.digSpeed = info->valueForKey("spd")->floatValue();
                        marchInfo.digStartTime = info->valueForKey("sdt")->doubleValue();
                        marchInfo.digStartNum = info->valueForKey("collected")->intValue();
                        marchInfo.changeCollectSpdTime = info->valueForKey("changeCollectSpdTime")->doubleValue();
                    }
                    if(info->objectForKey("exploreSt")){
                        marchInfo.mazeStartTime = info->valueForKey("exploreSt")->doubleValue();
                        marchInfo.mazeEndTime = info->valueForKey("exploreTime")->doubleValue() + marchInfo.mazeStartTime;

                    }
                    
                    if (info->objectForKey("territoryStartTime")) {
                        marchInfo.territoryStartTime = info->valueForKey("territoryStartTime")->doubleValue();
                    }
                    if (info->objectForKey("territoryUpdateTime")) {
                        marchInfo.territoryFinishTime = info->valueForKey("territoryUpdateTime")->doubleValue();
                    }
                    if (info->objectForKey("territoryStat")) {
                        marchInfo.territoryStat = info->valueForKey("territoryStat")->intValue();
                    }
                    
                    QueueController::getInstance()->addMarchQueueInfo(marchInfo.uuid);
                }
                    break;
                case StateReturn: {
                    QueueController::getInstance()->addMarchQueueInfo(marchInfo.uuid);
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void WorldController::resetTime(double timeStamp) {
//    cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
    
    struct timeval now;
    gettimeofday(&now, nullptr);
    initTime = true;
    
    startTimeStampOffset = timeStamp - ((double)now.tv_sec * 1000.0f + (double)now.tv_usec / 1000.0f);
}

double WorldController::getTime() {
//    cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
    struct timeval now;
    gettimeofday(&now, nullptr);
    return startTimeStampOffset + ((double)now.tv_sec * 1000.0f + (double)now.tv_usec / 1000.0f);
}

void WorldController::initFavoriteInfo() {
    if (m_favoriteTiles.empty()) {
        auto cmd = new WorldGetFavoriteCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WorldController::onFavoriteInfoCallback), NULL));
        cmd->sendAndRelease();
    }
}

void WorldController::onFavoriteInfoCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : error parse
        return;
    }
    auto arr = dynamic_cast<CCArray*>(ret->getData());
    if (arr) {
        CCObject* p = NULL;
        CCARRAY_FOREACH(arr, p) {
            auto info = dynamic_cast<CCDictionary*>(p);
            if (info) {
                unsigned int index = info->valueForKey("point")->intValue();
                std::string name = info->valueForKey("name")->getCString();
                auto type = (FavoriteType)info->valueForKey("type")->intValue();
                auto point = getPointByIndex(index);
                int serverId = info->valueForKey("server")->intValue();
                long createTime = info->valueForKey("createTime")->longValue();
                double key = WorldController::getInstance()->getFavoriteKey(index, serverId);
                m_favoriteTiles[key] = FavoriteTile{point,name,type, false, serverId, createTime};
            }
        }
    }
}

double WorldController::getFavoriteKey(int index, int serverId){
    double key = serverId * 10000000 + index;
    return key;
}

bool WorldController::isInFavorite(WorldCityInfo &info, int serverId){
    bool flag = false;
    double index = getFavoriteKey(info.cityIndex, serverId);
    
    if(info.parentCityIndex != -1){
        auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            flag = true;
        }
        int index_1 = index - 1;
        it = WorldController::getInstance()->m_favoriteTiles.find(index_1);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            flag = true;
        }
        int index_2 = index - 1 - _current_tile_count_x;
        it = WorldController::getInstance()->m_favoriteTiles.find(index_2);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            flag = true;
        }
        int index_3 = index - _current_tile_count_x;
        it = WorldController::getInstance()->m_favoriteTiles.find(index_3);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            flag = true;
        }
        
    }else{
        auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            flag = true;
        }
    }
    return flag;
}

void WorldController::updateTroop(const string &uuid) {
    // TODO: LH 只要回到家，就发送这个命令更新一下资源和兵力
    auto cmd = new WorldResourceUpdateCommand(uuid);
    cmd->setCallback(CCCallFuncO::create(WorldController::getInstance(), callfuncO_selector(WorldController::onTroopBack), NULL));
    cmd->sendAndRelease();
}

void WorldController::onTroopBack(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : error parse
        return;
    }
    CCArray *arr = CCArray::create();
    auto dict = dynamic_cast<CCDictionary*>(ret->getData());
    auto soilders = dynamic_cast<CCArray*>(dict->objectForKey("soldiers"));
    if(soilders){
        CCObject* p = NULL;
        CCARRAY_FOREACH(soilders, p) {
            auto info = dynamic_cast<CCDictionary*>(p);
            auto id = info->objectForKey("id");
            if (id) {
                auto tempID = info->valueForKey("id")->intValue(); //fusheng 士兵id均为数字
                if (tempID<=107430&&tempID>=107401) {
                    TitanInfoCommand *ticmd = new TitanInfoCommand();
                    ticmd->sendAndRelease();
                }
            }
            ArmyController::getInstance()->refreshArmyFreeNum(info);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_BACK);
    }
    auto generals = dynamic_cast<CCArray*>(dict->objectForKey("generals"));
    if (generals) {
        CCObject* p = NULL;
        CCARRAY_FOREACH(generals, p) {
            std::string generalId = dynamic_cast<CCString*>(p)->getCString();
            if (generalId != "") {
                GlobalData::shared()->generals[generalId].state = FREE;
                if(arr->count() == 0){
                    arr->addObject(CCString::create(generalId));
                }else{
                    int i = 0;
                    bool addFlag = false;
                    while(i < arr->count()){
                        std::string uid = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
                        
                        if(GlobalData::shared()->generals[generalId].getPower() < GlobalData::shared()->generals[uid].getPower()){
                            arr->insertObject(CCString::create(generalId), i);
                            addFlag = true;
                            break;
                        }
                        i++;
                    }
                    if(!addFlag){
                        arr->addObject(CCString::create(generalId));
                    }
                }
            }
        }
    }
    int index = 0;
    map<int, std::string>::iterator it;
    for(it = GeneralManager::getInstance()->defenceGeneral.begin(); it != GeneralManager::getInstance()->defenceGeneral.end(); it++){
        if(it->second == "" && index < arr->count()){
            it->second = dynamic_cast<CCString*>(arr->objectAtIndex(index))->getCString();
            index++;
        }
    }
    if(dict->objectForKey("gold")){
        int gold = dict->valueForKey("gold")->intValue();
        GlobalData::shared()->playerInfo.gold = gold;
        UIComponent::getInstance()->updateGold(gold);
    }
}

CCPoint WorldController::getPointByIndex(unsigned int index, int tileX/* = _current_tile_count_x*/, int tileY /*= _current_tile_count_y*/) {
    if (index == 0) {
        return ccp(1, 1);
    }
    if(tileX == -1){
        tileX = WorldController::getInstance()->_current_tile_count_x;
        tileY = WorldController::getInstance()->_current_tile_count_y;
    }

    if(index < 1 || index > tileX * tileY){
        return ccp(1, 1);
    }
    unsigned int z = index - 1;
    return ccp((int)(z%tileX), (int)(z/tileX));
}

CCPoint WorldController::getPointByMapTypeAndIndex(unsigned int index,MapType mType){
    CCPoint pos = ccp(1, 1);
    switch (mType) {
        case NORMAL_MAP:
            pos = getPointByIndex(index);
            break;
        case SERVERFIGHT_MAP:
            pos = getPointByIndex(index,GlobalData::shared()->worldConfig.ckf_world_row,GlobalData::shared()->worldConfig.ckf_world_row);
            break;
        default:
            pos = getPointByIndex(index);
            break;
    }
    return pos;
}

static std::map<int, CCPoint> serverPointCache;


CCPoint WorldController::getServerPosById(int serverId){
    if( serverPointCache.find(serverId) != serverPointCache.end() ) {
        return serverPointCache[serverId];
    }
    CCPoint tempPoint = CCPointZero;
    auto allDic =  LocalController::shared()->DBXMLManager()->getGroupByKey("serverpos");
    if (!allDic) {
        CCLOGFUNC("error: no groupId  named serverpos");
    }
    if (allDic) {
        auto modelDic = _dict(allDic->objectForKey(CC_ITOA(serverId)));
        if (!modelDic) {
            CCLOG("error: no serverId: %d",serverId);
        }
        else{
            int x = modelDic->valueForKey("X")->intValue();
            int y = modelDic->valueForKey("Y")->intValue();
            tempPoint.x = x;
            tempPoint.y = y;
        }
    }
    if (tempPoint.x == 0 && 0 == tempPoint.y && serverId != 0) {
//        CCAssert(false, "error pos");
    }
    serverPointCache[serverId] = tempPoint;
    return tempPoint;
}

static std::map<CCPoint, int> pointServerCache;

int WorldController::getServerIdByServerPoint(const CCPoint & serverPoint){
    
    if( pointServerCache.find(serverPoint) != pointServerCache.end() ) {
        return pointServerCache[serverPoint];
    }
    
    int tempServerId = 0;
    auto allDic =  LocalController::shared()->DBXMLManager()->getGroupByKey("serverpos");
    if (allDic) {
        CCDictElement* pElem = NULL;
        CCDICT_FOREACH(allDic, pElem){
            auto modelDic = _dict(pElem->getObject());
            if (modelDic) {
                int x = modelDic->valueForKey("X")->intValue();
                int y = modelDic->valueForKey("Y")->intValue();
                if (serverPoint.x == x && serverPoint.y == y) {
                    tempServerId = modelDic->valueForKey("id")->intValue();
                }
            }
        }
    }
    
    pointServerCache[serverPoint] = tempServerId;
    return tempServerId;
}
CCPoint WorldController::getChangePointByTypeNum(int typeNum){
    switch (typeNum) {
        case 0:
            return ccp(0, 0);
            break;
        case 1:
            return ccp(0,1);
            break;
        case 2:
            return ccp(1, 1);
            break;
        case 3:
            return ccp(1, 0);
            break;
        case 4:
            return ccp(1, -1);
            break;
        case 5:
            return ccp(0, -1);
            break;
        case 6:
            return ccp(-1, -1);
            break;
        case 7:
            return ccp(-1, 0);
            break;
        case 8:
            return ccp(-1, 1);
            break;
        default:
            return ccp(0, 0);
            break;
    }
}
CCPoint WorldController::getChangeViewPosByServerPos(const CCPoint &serverTilePoint,bool shouldAddEdge/* = false*/){
    if (WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP) {
        return  CCPointZero;
    }
    auto disPoint = ccpSub(serverTilePoint, CCPointZero);
    if(!shouldAddEdge){
        return ccp((disPoint.x-disPoint.y)*_map_width/2,(disPoint.x+disPoint.y)*_map_height/2);
    }else{
        return ccp((disPoint.x-disPoint.y)*_big_mapWidth/2,(disPoint.x+disPoint.y)*_big_mapHeight/2);
    }
    
}
int WorldController::getRoundServer(int centerServerId, int roundTypeNum){
    CCPoint centerServerPoint = getServerPosById(centerServerId);
    vector<CCPoint> pointVec;
    
    CCPoint point_left = centerServerPoint + ccp(-1, 0);
    
    CCPoint point_top_left = centerServerPoint + ccp(-1, 1);
    CCPoint point_top = centerServerPoint + ccp(0, 1);
    CCPoint point_top_right = centerServerPoint + ccp(1, 1);
    
    CCPoint point_right = centerServerPoint + ccp(1, 0);
    
    CCPoint point_bot_right = centerServerPoint + ccp(1, -1);
    CCPoint point_bot = centerServerPoint + ccp(0, -1);
    CCPoint point_bot_left = centerServerPoint + ccp(-1, -1);
    
    pointVec.push_back(point_left);
    pointVec.push_back(point_top_left);
    pointVec.push_back(point_top);
    pointVec.push_back(point_top_right);
    pointVec.push_back(point_right);
    pointVec.push_back(point_bot_right);
    pointVec.push_back(point_bot);
    pointVec.push_back(point_bot_left);
    int serverId = getServerIdByServerPoint(pointVec.at(roundTypeNum));
    return serverId;
}
CCPoint WorldController::getServerViewPosByPos(const CCPoint &serverTilePoint){
    if (WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP) {
        return  CCPointZero;
    }
    auto disPoint = ccpSub(serverTilePoint, WorldController::getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
     return ccp(serverMap_centerViewPoint.x+(disPoint.x-disPoint.y)*_big_mapWidth/2,serverMap_centerViewPoint.y+(disPoint.x+disPoint.y)*_big_mapHeight/2) - ccp(_big_mapWidth/ 2 ,_big_mapHeight / 2) + ccp(_halfTileSize.width *3, _tile_height *3);
}
unsigned int WorldController::getIndexByPoint(const cocos2d::CCPoint &point, int tileX, int tileY) {
    if(tileX == -1){
        tileX = WorldController::getInstance()->_current_tile_count_x;
        tileY = WorldController::getInstance()->_current_tile_count_y;
    }
    if (point.x < 0 || point.y < 0 || point.x > tileX - 1 || point.y > tileY - 1) {
        return 0;
    }

    return (unsigned int)(point.x) + (unsigned int)(point.y*tileX) + 1;
}

void WorldController::openMarchDeploy(unsigned int targetIndex,unsigned int haveOwner,int rally,int bType,int wtIndex,std::string other, int targetType,float slow) {
    // judge march max limit
    unsigned int current = getCurrentMarchCount();
    unsigned int max = getMaxMarchCount();
    if (current >= max) {
        showMarchAlert(max);
        return;
    }
    auto endPos = getPointByIndex(targetIndex);
//    float distance = ccpSub(selfPoint, endPos).getLength();
    bool isFourTile = false;
    if(m_cityInfo.find(targetIndex) != m_cityInfo.end()){
        isFourTile = (m_cityInfo[targetIndex].cityType == MonsterTile || m_cityInfo[targetIndex].cityType == CityTile);
    }

    int index = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
    PopupViewController::getInstance()->addPopupInView(BattleView::create(index, targetIndex, haveOwner,slow,rally,bType,wtIndex,other,targetType));
}

void WorldController::showMarchAlert(int max){
    int index = 0;
    while(index < _capTech.size()){
        std::string sid = _capTech[index];
        if(ScienceController::getInstance()->getScienceLevel(atoi(sid.c_str())) == 0){
            string showTip = _lang_2("108863"
                                     , CC_ITOA(int(max))
                                     , CCCommonUtils::getNameById(sid).c_str()
                                     );
            showTip.append("\n\n");
            showTip.append(_lang("103048"));
            showTip.append("\n");
            auto dialog = YesNoDialog::show(showTip.c_str(),
                                            CCCallFunc::create(this, callfunc_selector(WorldController::startVIPGuide)),
                                            0,
                                            false,
                                            CCCallFunc::create(this, callfunc_selector(GuideController::startScienceGuide)));
            dialog->showCancelButton();
            CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("108865").c_str());
            dialog->setYesButtonTitle(_lang("114020").c_str());
            return;
        }
        index++;
    }
    
    YesNoDialog::showYesDialog(_lang_1("108867", CC_ITOA((int)max)));
}
void WorldController::startVIPGuide(){
    UIComponent::getInstance()->onVipBtnClick(NULL,Control::EventType::TOUCH_DOWN);
}
unsigned int WorldController::getMaxMarchCount() {
    return 1 + CCCommonUtils::getEffectValueByNum(55);
}

unsigned int WorldController::getCurrentMarchCount() {
    unsigned int ret = 0;
    for (auto &march:WorldController::getInstance()->m_marchInfo) {
        if (march.second.ownerType == PlayerSelf && march.second.uuid != "") {
            ret++;
        }
    }
    return ret;
}


int WorldController::getWalkTime(float distance){
    int time = 0;
    //    (MAX(k1*SIN(k2*两点间的距离+k3)+k4,k5)*k6/最慢速度
    //  原行军时间*max（（100+行军速度【60】）/100，0）
    int spd = TroopsController::getInstance()->m_minSpeed;
    if (!spd) {
        return time;
    }
    double k1 = GlobalData::shared()->worldConfig.distance[0];
    
    float effect = MAX((100.0+CCCommonUtils::getEffectValueByNum(60))/100.0,0.001);
    time = (int)(k1*distance/spd/effect);
    return time;
}


void WorldController::addMonsetResearchInfo(CCDictionary *dict, bool postNotification){
    MonsterSearchResultInfo *info = MonsterSearchResultInfo::create();
    info->type = MonsterSearchResultType(dict->valueForKey("t")->intValue());
    info->time = dict->valueForKey("time")->doubleValue();
    if(dict->objectForKey("fr")){
        info->fType = MonsterFightResultType(dict->valueForKey("fr")->intValue());
    }
    if(dict->objectForKey("exp")){
        info->exp = dict->valueForKey("exp")->intValue();
    }
    if(dict->objectForKey("itemId")){
        info->itemId = dict->valueForKey("itemId")->getCString();
    }
    if(dict->objectForKey("dead")){
        info->dead = dynamic_cast<CCDictionary*>(dict->objectForKey("dead"));
    }
    if(dict->objectForKey("npcId")){
        info->npcId = dict->valueForKey("npcId")->getCString();
    }
    if(dict->objectForKey("fid")){
        info->fid = dict->valueForKey("fid")->getCString();
    }
    if(dict->objectForKey("resType")){
        info->resType = dict->valueForKey("resType")->getCString();
    }
    if(dict->objectForKey("resValue")){
        info->resValue = dict->valueForKey("resValue")->getCString();
    }
    int index = m_monsterAttackInfo->count();
    info->infoStr += _lang_1("108674", CC_ITOA(index));
    if(info->type == FIGHT){
        if(info->fType == BATTLE_WIN){
            info->infoStr += _lang("108661") + _lang_2("108663", CCCommonUtils::getNameById(info->npcId).c_str(), CC_ITOA(info->exp));
        }else{
            info->infoStr += _lang("108662") + _lang_2("108663", CCCommonUtils::getNameById(info->npcId).c_str(), CC_ITOA(info->exp));
        }
    }else if(info->type == CHEST){
        info->infoStr += _lang_1("108664", CCCommonUtils::getNameById(info->itemId).c_str());
    }else if(info->type == LOST_SOLDIER){
        CCDictElement *element;
        std::string addStr = "";
        auto dic = info->dead;
        CCDICT_FOREACH(dic, element){
            std::string armyId = element->getStrKey();
            int value = info->dead->valueForKey(armyId)->intValue();
            addStr = addStr + CCCommonUtils::getNameById(armyId) + " * " + CC_ITOA(value) + " ";
        }
        
        info->infoStr += _lang_1("108665", addStr.c_str());
    }else if(info->type == RESOURCE){
        vector<std::string> vector1;
        vector<std::string> vector;
        CCCommonUtils::splitString(info->resType, "|", vector);
        CCCommonUtils::splitString(info->resValue, "|", vector1);
        int i = 0;
        std::string addStr = "";
        while(i < vector.size()){
            int type = atoi(vector[i].c_str());
            int value = atoi(vector1[i].c_str());
            std::string typeStr = CCCommonUtils::getResourceNameByType(type);
            addStr = addStr + typeStr + " * " + CC_ITOA(value) + " ";
            i++;
        }
        info->infoStr += _lang_1("108664", addStr.c_str());
    }
    
    m_monsterAttackInfo->addObject(info);
    
    if(postNotification){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_MONSTER_RESEARCH_CHANGE, info);
    }
}

bool WorldController::isCityCanSettle(unsigned int index,int tileCount){
    if(tileCount==2){
    int lp = index - 1;
    int up = index - _current_tile_count_x;
    int lup = up - 1;
    if(lp <= 0 || lup <= 0){
        return false;
    }
    
    if(m_cityInfo[lp].cityType != OriginTile
       || m_cityInfo[index].cityType != OriginTile
       || m_cityInfo[up].cityType != OriginTile
       || m_cityInfo[lup].cityType != OriginTile
       ){
        return false;
    }
    if(m_cityInfo[lp].cityOriginType == "stop"
       || m_cityInfo[index].cityOriginType == "stop"
       || m_cityInfo[up].cityOriginType == "stop"
       || m_cityInfo[lup].cityOriginType == "stop"
       ){
        return false;
    }
    }else if (tileCount==1){
        if(m_cityInfo[index].cityType != OriginTile){
            return false;
        }
        if(m_cityInfo[index].cityOriginType == "stop"){
            return false;
        }
    }
    return true;
}

bool WorldController::isHasMonsterAttackMarch(){
    map<unsigned int , string>::iterator it;
    for(it = m_selfMarchUuid.begin(); it != m_selfMarchUuid.end(); it++){
        map<string, MarchInfo>::iterator selfMarchIt = m_marchInfo.find(it->second);
        if(selfMarchIt != m_marchInfo.end()){
            if(selfMarchIt->second.targetType == MonsterTile){
                return true;
            }
        }
    }
    return false;
}

void WorldController::doWhenBackToGame(CCArray *arr, bool isLogOut){
    WorldController::getInstance()->m_marchInfo.clear();
    WorldController::getInstance()->m_selfMarchUuid.clear();
    WorldController::getInstance()->removeAllQueue();
    TimeUpMarchProcesser::getInstance()->clear();
    
    if(arr){
        WorldController::getInstance()->initMarchInfo(arr);
    }
    WorldController::getInstance()->m_monsterAttackInfo->removeAllObjects();
    WorldController::getInstance()->isInfoTake = false;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance() && !isLogOut){
        WorldMapView::instance()->needClearAll = true;
        WorldMapView::instance()->clearAllMarch();
        WorldMapView::instance()->m_map->updateDynamicMap();
        MonsterAttackDetailView *view = dynamic_cast<MonsterAttackDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
        if(view){
            int idx = view->getIndex();
            view->closeSelf();
            if(WorldController::getInstance()->m_selfMarchUuid.find(idx) != WorldController::getInstance()->m_selfMarchUuid.end()){
                PopupViewController::getInstance()->addPopupInView(MonsterAttackDetailView::create(idx), false);
            }
        }
    }
}

CCPoint WorldController::getCrossPointAtCity(MarchInfo &info, float s){
    unsigned int targetIndex = info.stateType == StateReturn ? info.startPointIndex : info.endPointIndex;
    unsigned int index = info.stateType == StateReturn ? info.endPointIndex : info.startPointIndex;

    auto startPos = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getInstance()->getPointByIndex(index));
    auto endPos = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getInstance()->getPointByIndex(targetIndex));

    startPos = startPos + ccp(0, _tile_height / 2);
    
    vector<CCPoint> vector;
    float areaScale = 1.0;
    if(info.targetType == CityTile || info.targetType == Trebuchet  || info.targetType == Tile_allianceArea  || info.targetType == tile_superMine || info.targetType == tile_wareHouse){
        endPos = endPos + ccp(0, _tile_height / 2);
    }else if(info.targetType == Throne){
        areaScale = 2;
    }
    
    if(s > 0){
        areaScale = s;
    }
    
    vector.push_back(endPos + ccp(-1, _tile_height * areaScale + 1));
    vector.push_back(endPos + ccp(_tile_width * areaScale + 1, -1));
    vector.push_back(endPos + ccp(_tile_width * areaScale + 1, 1));
    vector.push_back(endPos + ccp(-1, -_tile_height * areaScale - 1));
    vector.push_back(endPos + ccp(1, -_tile_height * areaScale - 1));
    vector.push_back(endPos + ccp(-_tile_width * areaScale - 1, 1));
    vector.push_back(endPos + ccp(-_tile_width * areaScale - 1, -1));
    vector.push_back(endPos + ccp(1, _tile_height * areaScale + 1));
        
    auto pt = CCCommonUtils::getCrossPoint(vector, startPos, endPos);
    vector.clear();
    return pt;
}

std::vector<CCPoint> WorldController::getCrossPointAtRelicGround(CCPoint &startPt, CCPoint &endPt){
    vector<CCPoint> vector;
    float areaScale = _relic_tile_width * 0.5;
    auto centerPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(ccp(_relic_center_x, _relic_center_y));
    vector.push_back(centerPt + ccp(-1, _tile_height * areaScale + 1));
    vector.push_back(centerPt + ccp(_tile_width * areaScale + 1, -1));
    vector.push_back(centerPt + ccp(_tile_width * areaScale + 1, 1));
    vector.push_back(centerPt + ccp(-1, -_tile_height * areaScale - 1));
    vector.push_back(centerPt + ccp(1, -_tile_height * areaScale - 1));
    vector.push_back(centerPt + ccp(-_tile_width * areaScale - 1, 1));
    vector.push_back(centerPt + ccp(-_tile_width * areaScale - 1, -1));
    vector.push_back(centerPt + ccp(1, _tile_height * areaScale + 1));
    
    auto tmp = CCCommonUtils::getCrossPoints(vector, startPt, endPt);
    vector.clear();
    //排序
    std::vector<CCPoint> result;
    result.push_back(ccp(startPt.x, startPt.y));
    result.push_back(ccp(endPt.x, endPt.y));
    while (tmp.size() > 0) {
        auto pt = tmp.back();
        tmp.pop_back();
        float dis = ccpDistance(startPt, pt);
        bool addFlag = false;
        for (auto it = result.begin(); it != result.end(); it++) {
            float dis1 = ccpDistance(startPt, *it);
            if(dis1 > dis){
                addFlag = true;
                result.insert(it, pt);
                break;
            }
        }
        if(!addFlag){
            result.push_back(pt);
        }
    }
    return result;
}

void WorldController::resetStaime(int num, double time){
    currentStamine = num;
    lastStamineTime = time;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
}

void WorldController::autoAddStamine(){
    if(this->currentStamine == -1 || lastStamineTime == -1){
        return;
    }
    int stamineIntervalTime = getStatmineIntervalTime();
    double dTime = (lastStamineTime + stamineIntervalTime * 1000) - WorldController::getInstance()->getTime();
    if(dTime <= 0 && currentStamine < GlobalData::shared()->worldConfig.stamineMax){
        int num = currentStamine + GlobalData::shared()->worldConfig.stamineIntervalNum;
        if(num > GlobalData::shared()->worldConfig.stamineMax){
            num = GlobalData::shared()->worldConfig.stamineMax;
        }
        resetStaime(num, WorldController::getInstance()->getTime());
    }
    if(currentStamine >= GlobalData::shared()->worldConfig.stamineMax){
        resetStaime(GlobalData::shared()->worldConfig.stamineMax, WorldController::getInstance()->getTime());
    }
}

int WorldController::getStatmineIntervalTime(){
    int value = GlobalData::shared()->worldConfig.stamineIntervalTime;
    int stamine  = GlobalData::shared()->worldConfig.stamineIntervalNum;
    value = stamine * 1.0 / (stamine * 1.0 / value * MAX(1.0 + CCCommonUtils::getEffectValueByNum(WORLD_STAMINE_INTERVAL) / 100, 0));
    return value;
}

bool WorldController::canAttackMonster(string monsterId){
    int usePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
    if (monsterId != "") {
        int power = atoi( CCCommonUtils::getPropById(monsterId, "usePower").c_str() );
        if (power > 0 ) {
            usePower = power;
        }
    }
    return currentStamine >= usePower;
}

int WorldController::getMonsterUsePower(int index)
{
    int ret = 0;
    auto info = WorldController::getInstance()->getCityInfos().find(index);
    if(info != WorldController::getInstance()->getCityInfos().end() && info->second.cityType == FieldMonster)
    {
        string monsterId = info->second.fieldMonsterInfo.monsterId;
        ret = atoi( CCCommonUtils::getPropById(monsterId, "usePower").c_str() );
    }
    return ret;
}

double WorldController::getMarchTime(double dis, WorldCityType type, MarchMethodType marchType, float slowDown){
    int spd = TroopsController::getInstance()->m_minSpeed;
    if(spd == 0){
        spd = getMinSpeed();
    }
    
    double k1 = GlobalData::shared()->worldConfig.distance[0];
    double k2 = GlobalData::shared()->worldConfig.distance[1];
    double k3 = GlobalData::shared()->worldConfig.distance[2];
    double k4 = GlobalData::shared()->worldConfig.distance[3];

    float effect = MAX(1.0 + CCCommonUtils::getEffectValueByNum(60)/100.0, 0);
    if(type == FieldMonster){
        effect = MAX(1.0 + (CCCommonUtils::getEffectValueByNum(60)+CCCommonUtils::getEffectValueByNum(WORLD_FIELD_MONSTER_TIME))/100.0, 0);
    }
    double time = (pow(dis, k1) * k2 / (spd * effect));

    if(marchType == MethodUnion){
        time = time / (1.0 + CCCommonUtils::getEffectValueByNum(WORLD_UNIN_EFFECT) / 100.0);
    }
    if(marchType == MethodYuanSolider){
        time = time / (1.0 + CCCommonUtils::getEffectValueByNum(WORLD_YUAN_EFFECT) / 100.0);
    }
    if(marchType == MethodDeal){
        time = pow(dis, k1) * k4;
        time = time * 1.0 /(1.0 + CCCommonUtils::getEffectValueByNum(ALLIANCE_TRADE_SPEED_TIME_EFFECT) /100.0);
    }
    if (marchType == MethodWarehouse) {
        time = pow(dis, k1) * k4;
        time = time * 1.0 /(1.0 + CCCommonUtils::getEffectValueByNum(ALLIANCE_TRADE_SPEED_TIME_EFFECT) /100.0);
    }
    return int(time * 1000 * slowDown);
}

int WorldController::getMinSpeed(){
    map<std::string, ArmyInfo>::iterator it;
    int result = 0;
    for (it = GlobalData::shared()->armyList.begin(); it != GlobalData::shared()->armyList.end();it++) {
        if (result == 0 || result > it->second.speed) {
            result = it->second.speed;
        }
    }
    return result;
}

double WorldController::getMarchTime(int startIndex, int endIndex, WorldCityType type, MarchMethodType marchType, bool toTileFour, bool isThrone){
    auto it = WorldController::getInstance()->getCityInfos().find(endIndex);
    if(it != WorldController::getInstance()->getCityInfos().end()){
        if(it->second.parentCityIndex > 0){
            endIndex = it->second.parentCityIndex;
            toTileFour = true;
        }
        if(it->second.cityType == Throne){
            isThrone = true;
        }
    }
    
    double totalTime = 0;
    auto arr = CCArray::create();
    
    auto srcXY = getPointByIndex(startIndex);
    auto dstXY = getPointByIndex(endIndex);
    int a1 = srcXY.x, b1 = srcXY.y, a2 = dstXY.x, b2 = dstXY.y;
    int a3 = 0, a4 = 0, b3 = 0, b4 = 0;
    int radius = _relic_tile_width / 2;

    if (abs(a2 - a1) >= abs(b2 - b1)) {
        int i = 0, y, j = 0;
        while (i != a2 - a1) {
            y = (int) ((b1 - b2) * 1.0f / (a1 - a2) * i + b1);
            if (max(abs(a1 + i - _relic_center_x), abs(y - _relic_center_y)) == radius) {
                a3 = a1 + i;
                b3 = y;
                j = i + (a2 - a1) / abs(a2 - a1);

                break;
            }
            i += (a2 - a1) / abs(a2 - a1);
        }
        if (a3 != 0) {
            while (j != a2 - a1) {
                y = (int) ((b1 - b2) * 1.0f / (a1 - a2) * j + b1);
                if (max(abs(a1 + j - _relic_center_x), abs(y - _relic_center_y)) == radius) {
                    a4 = a1 + j;
                    b4 = y;

                    double dis = sqrt(pow(abs(a4 - a3), 2.0) + pow(abs(b4 - b3), 2.0));
                    if(dis > 1) {
                        break;
                    }
                }
                j += (a2 - a1) / abs(a2 - a1);
            }
        }
    } else {
        int m=0, x, n = 0;
        while (m != b2 - b1) {
            x = (int) ((a1 - a2) * 1.0f / (b1 - b2) * m + a1);
            if (max(abs(b1 + m - _relic_center_x), abs(x - _relic_center_y)) == radius) {
                b3 = b1 + m;
                a3 = x;
                n = m + (b2 - b1) / abs(b2 - b1);

                break;
            }
            m += (b2 - b1) / abs(b2 - b1);
        }
        
        if (b3 != 0) {
            while (n != b2 - b1) {
                x = (int) ((a1 - a2) * 1.0f / (b1 - b2) * n + a1);
                
                if (max(abs(b1 + n - _relic_center_x), abs(x - _relic_center_y)) == radius) {
                    b4 = b1 + n;
                    a4 = x;

                    double dis = sqrt(pow(abs(a4 - a3), 2.0) + pow(abs(b4 - b3), 2.0));
                    if(dis > 1) {
                        break;
                    }
                }
                n += (b2 - b1) / abs(b2 - b1);
            }
        }
    }

    if(a4 > 0) {
        double dis = sqrt(pow(abs(a4 - a3), 2.0) + pow(abs(b4 - b3), 2.0));
        if(dis <= 1) {
            a4 = 0;
            b4 = 0;
        }
    }
    
    auto addPt = [](int x, int y, CCArray *arr){
        if(x == 0 || y == 0){
            
        }else{
            int cityIndex = WorldController::getIndexByPoint(ccp(x, y));
            arr->addObject(CCInteger::create(cityIndex));
        }
    };
    
    addPt(a3, b3, arr);
    addPt(a4, b4, arr);

    double k = GlobalData::shared()->worldConfig.distance[4];
    arr->addObject(CCInteger::create(endIndex));
    arr->insertObject(CCInteger::create(startIndex), 0);
    int lineStart = dynamic_cast<CCInteger*>(arr->objectAtIndex(0))->getValue();
    int index = 1;
    while (index < arr->count()) {
        int lineEnd = dynamic_cast<CCInteger*>(arr->objectAtIndex(index))->getValue();
        auto lineEndPt = getPointByIndex(lineEnd);
        auto startPt = getPointByIndex(lineStart);
        bool isToTileFour = false;
        bool isFromeTileFour = false;
        bool isThronePt = false;
        if(index == 1){
            isFromeTileFour = true;
        }
        if(index == arr->count() - 1){
            isToTileFour = toTileFour;
            isThronePt = isThrone;
        }

        double dis = getDistance(startPt, lineEndPt, isFromeTileFour, isToTileFour, isThronePt);

        auto isInRelic = [](int start, int end){
            auto startPt = getPointByIndex(start);
            auto endPt = getPointByIndex(end);
            auto centerPt = (startPt + endPt) / 2;
            if(centerPt.x <= int(_relic_center_x + _relic_tile_width / 2)
               && centerPt.x >= int(_relic_center_x - _relic_tile_width / 2)
               && centerPt.y <= int(_relic_center_y + _relic_tile_height / 2)
               && centerPt.y >= int(_relic_center_y - _relic_tile_height / 2)
               ){
                return true;
            }
            return false;
        };
        int time = getMarchTime(dis, type, marchType, (isInRelic(lineStart, lineEnd) ? k : 1.0));
        totalTime += time;
        index++;
        lineStart = lineEnd;
    }
    return totalTime;
}

double WorldController::getMarchTime(MarchInfo &info){
    return getMarchTime(info.startPointIndex, info.endPointIndex, info.targetType);
}

void WorldController::addTestData(){
    auto getIndex = [](CCPoint &pt, int index){
        CCPoint result = ccp(-1, -1);
        result.x = pt.x + 40 - rand() % 80;
        result.y = pt.y + 40 - rand() % 80;
        
        int value = WorldController::getInstance()->getIndexByPoint(result);
        return value;
    };
    CCArray *arr = CCArray::create();
    static int index = 1;
    int startIndex = 0;
    static int total = 1000;
    int selfPt = getIndexByPoint(selfPoint);
    while (index <= total && startIndex < 10) {
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(index)), "uuid");
        int startPt = getIndex(WorldController::getInstance()->selfPoint, index);
        dict->setObject(CCString::create(CC_ITOA(startPt)), "sp");
        dict->setObject(CCString::create(CC_ITOA(selfPt)), "dp");
        dict->setObject(CCString::create(CC_ITOA(getTime())), "st");
        dict->setObject(CCString::create(CC_ITOA(getTime() + 300 * 1000)), "et");
        dict->setObject(CCString::create(CC_ITOA(int(MethodBattle))), "mType");
        dict->setObject(CCString::create(CC_ITOA(int(CityTile))), "tType");
        dict->setObject(CCString::create(CC_ITOA(int(PlayerSelf))), "oType");
        dict->setObject(CCString::create(CC_ITOA(int(StateReturn))), "sType");
        dict->setObject(CCString::create(""), "o");
        dict->setObject(CCString::create(CC_ITOA(1)), "color");
        CCArray *soldierArr = CCArray::create();
        CCDictionary *soldier = CCDictionary::create();
        soldier->setObject(CCString::create(CC_ITOA(index - 1)), "armId");
        soldier->setObject(CCString::create("100000"), "total");
        soldier->setObject(CCString::create("0"), "dead");
        soldierArr->addObject(soldier);
        dict->setObject(soldierArr, "army");
        arr->addObject(dict);
        index++;
        startIndex++;
    }
    
    initMarchInfo(arr);
}

void WorldController::addGuideData(){
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
    auto getIndex = [](CCPoint &pt, int index){
        CCPoint result = ccp(-1, -1);
        if (index == 1) {
            result.x = pt.x - 4 + rand() % 3;
            result.y = pt.y - 4 + rand() % 3;
        }else if(index == 2){
            result.x = pt.x - 1 + rand() % 3;
            result.y = pt.y + 4 + rand() % 3;
        }else if(index == 3){
            result.x = pt.x + 4 + rand() % 3;
            result.y = pt.y - 1 + rand() % 3;
        }
        int value = WorldController::getInstance()->getIndexByPoint(result);
        return value;
    };
    CCArray *arr = CCArray::create();
    int index = 1;
    int selfPt = getIndexByPoint(selfPoint);
    while (index <= 3) {
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(index)), "uuid");
        int startPt = getIndex(WorldController::getInstance()->selfPoint, index);
        dict->setObject(CCString::create(CC_ITOA(startPt)), "sp");
        dict->setObject(CCString::create(CC_ITOA(selfPt)), "dp");
        dict->setObject(CCString::create(CC_ITOA(getTime() - 10 * 1000)), "st");
        dict->setObject(CCString::create(CC_ITOA(getTime() + 5 * 1000)), "et");
        dict->setObject(CCString::create(CC_ITOA(int(MethodBattle))), "mType");
        dict->setObject(CCString::create(CC_ITOA(int(CityTile))), "tType");
        dict->setObject(CCString::create(CC_ITOA(int(PlayerSelf))), "oType");
        dict->setObject(CCString::create(CC_ITOA(int(StateReturn))), "sType");
        dict->setObject(CCString::create(""), "o");
        dict->setObject(CCString::create(CC_ITOA(1)), "color");
        CCArray *soldierArr = CCArray::create();
        CCDictionary *soldier = CCDictionary::create();
        soldier->setObject(CCString::create(CC_ITOA(index - 1)), "armId");
        soldier->setObject(CCString::create("100000"), "total");
        soldier->setObject(CCString::create("0"), "dead");
        soldierArr->addObject(soldier);
        dict->setObject(soldierArr, "army");
        arr->addObject(dict);
        index++;
    }
    
    initMarchInfo(arr);
}

double WorldController::getDistance(WorldCityInfo &from, WorldCityInfo &to){
    bool fromFourTile = (from.parentCityIndex != -1);
    bool toFourTile = (to.parentCityIndex != -1);
    int fromIndex = (fromFourTile ? from.parentCityIndex : from.cityIndex);
    int toIndex = (toFourTile ? to.parentCityIndex : to.cityIndex);
    auto fromPt = getPointByIndex(fromIndex);
    auto toPt = getPointByIndex(toIndex);
    return getDistance(fromPt, toPt, fromFourTile, toFourTile, to.cityType == Throne);
}

double WorldController::getDistance(WorldCityInfo &to){
    bool toFourTile = (to.cityIndex != -1);
    int toIndex = (toFourTile ? to.parentCityIndex : to.cityIndex);
    auto toPt = getPointByIndex(toIndex);
    return getDistance(toPt, toFourTile, to.cityType == Throne);
}

double WorldController::getDistance(CCPoint &from, CCPoint &to, bool isFromFourTile, bool isToFourTile, bool isThrone){
    auto addPtToVector = [](std::vector<CCPoint> &vector, bool isFourTile, CCPoint &pt, bool isThrone){
        vector.push_back(ccp(pt.x, pt.y));
        if(isThrone){
            int startX = -_throne_w / 2;
            int endX = _throne_w / 2;
            int startY = -_throne_h / 2;
            int endY = _throne_h / 2;
            int x = startX;
            int y = startY;
            while (x <= endX) {
                y = startY;
                while (y <= endY) {
                    vector.push_back(ccp(pt.x + x, pt.y + y));
                    y++;
                }
                x++;
            }

        }else if(isFourTile){
            vector.push_back(ccp(pt.x - 1, pt.y));
            vector.push_back(ccp(pt.x - 1, pt.y - 1));
            vector.push_back(ccp(pt.x, pt.y - 1));
        }
    };
    std::vector<CCPoint> fromVector;
    addPtToVector(fromVector, isFromFourTile, from, false);
    std::vector<CCPoint> toVector;
    addPtToVector(toVector, isToFourTile, to, isThrone);
    int i = 0;
    double result = -1.0f;
    while (i < fromVector.size()) {
        auto &fromPt = fromVector[i];
        int j = 0;
        while (j < toVector.size()) {
            auto &toPt = toVector[j];
            double dis = ccpSub(fromPt, toPt).getLength();
            if(dis < result || result < 0){
                result = dis;
            }
            j++;
        }
        i++;
    }
    return result;
}

double WorldController::getDistance(CCPoint &to, bool isToFourTile, bool isThrone){
    return getDistance(WorldController::getInstance()->selfPoint, to, true, isToFourTile, isThrone);
}

void WorldController::getServerList(){
    GetAllServerListCommand *cmd = new GetAllServerListCommand();
    cmd->sendAndRelease();
}

void WorldController::parseServerList(CCObject *obj){
    auto list = _dict(obj);
    if(!list->objectForKey("list")){
        return;
    }
    m_serverList.clear();
    CCArray *arr = dynamic_cast<CCArray*>(list->objectForKey("list"));
    if(arr){
        int num = arr->count();
        int unitNum = ceil(num * 1.0 / CITY_NUM_PER_UNIT);
        int totalNum = unitNum * NUM_PER_UNIT;
        int xNum = ceil(sqrt(totalNum)) + 2 * ADD_NUM;
        xNum = (xNum + 1) / 2;
        auto currentPt = ccp(int(xNum * 3 / 2) - 1, int(xNum * 3 / 2));
        auto centerPt = ccp(int(xNum * 3 / 2), int(xNum * 3 / 2));;
        int direction = 0;//0 down 1 left 2 up 3 right
        int totalDirection = 4;
        int totalPerSide = 0;
        int count = 1;
        
        auto getCityIconIndex = [](CCPoint &centerPt, CCPoint &currentPt){
            int dy = centerPt.y - currentPt.y;
            int dx = centerPt.x - currentPt.x;
            int cityIconIndex = ((3 + dy * 2 + dx) % 6 + 6) % 6 + 1;
            return cityIconIndex = cityIconIndex;
        };
        
        int index = 0;
        while(index < arr->count()){
            auto dict = _dict(arr->objectAtIndex(index));
            ServerListInfo info = ServerListInfo();
            info.serverId = dict->valueForKey("id")->intValue();
            
            if(dict->objectForKey("name")){
                info.serverName = dict->valueForKey("name")->getCString();
            }else{
                info.serverName = CCString::createWithFormat("Kingdom#%d", info.serverId)->getCString();
            }

            if(dict->objectForKey("hot")){
                info.isHot = true;
            }else{
                info.isHot = false;
            }
            
            if(dict->objectForKey("banner")){
                info.banner = dict->valueForKey("banner")->getCString();
                if(info.banner == ""){
                    info.banner = "UN";
                }
            }else{
                info.banner = "UN";
            }
//            info.allianceName = dict->valueForKey("kingAllianceName")->getCString();
            if(dict->objectForKey("abbr")){
                info.abbr = dict->valueForKey("abbr")->getCString();
            }else{
                info.abbr = "";
            }
//            info.allianceFlag = dict->valueForKey("allianceIcon")->getCString();
            if(dict->objectForKey("kingName")){
                info.kingName = dict->valueForKey("kingName")->getCString();
            }else{
                info.kingName = "";
            }

            if(info.serverId == GlobalData::shared()->playerInfo.selfServerId){
                GlobalData::shared()->playerInfo.serverName = info.serverName;
            }
            int iconIndex = 1;
            if(index == 0){
                iconIndex = getCityIconIndex(centerPt, currentPt);
                info.cityIconIndex = iconIndex;
                info.x = currentPt.x;
                info.y = currentPt.y;
                m_serverList.push_back(info);
                index++;
                continue;
            }
            int addX = 0;
            int addY = 0;
            int sideLen = totalPerSide;
            if(direction == 0){
                addY = -1;
                sideLen = totalPerSide + 1;
            }else if(direction == 1){
                addX = -1;
                sideLen = totalPerSide - 1;
            }else if(direction == 2){
                addY = 1;
            }else if(direction == 3){
                addX = 1;
            }
            
            currentPt = currentPt + ccp(addX, addY);
            iconIndex = getCityIconIndex(centerPt, currentPt);
            info.cityIconIndex = iconIndex;
            info.x = currentPt.x;
            info.y = currentPt.y;
            m_serverList.push_back(info);
            
            ++count;
            if(count >= sideLen){
                count = 0;
                direction = ++direction % totalDirection;
                if(direction == 1){
                    totalPerSide = totalPerSide / 2 * 2 + 2;
                }
            }
            
            ++index;
            CCLOG("index:%d, %d", info.x, info.y);
        }
    }
}

bool WorldController::isSelfThrone(WorldCityInfo &info){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        if(GlobalData::shared()->playerInfo.allianceInfo.uid == info.kingBuildInfo.allianceUid){
            return true;
        }
    }else{
        if(GlobalData::shared()->playerInfo.uid == info.kingBuildInfo.uid){
            return true;
        }
    }
    return false;
}

bool WorldController::isSelfTrebuchet(WorldCityInfo &info){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        if(GlobalData::shared()->playerInfo.allianceInfo.uid == info.trebuchetInfo.allianceUid){
            return true;
        }
    }else{
        if(GlobalData::shared()->playerInfo.uid == info.trebuchetInfo.uid){
            return true;
        }
    }
    return false;
}

bool WorldController::isInFresherForbidenArea(int index, bool isInServerFight){
    if(isInServerFight){
        return false;
    }
    auto checkPt = [](const CCPoint &pt){
        bool isIn = false;
        if (pt.x >= _relic_center_x - _relic_tile_width / 2
            && pt.x <= _relic_center_x + _relic_tile_width / 2
            && pt.y >= _relic_center_y - _relic_tile_height / 2
            && pt.y <= _relic_center_x + _relic_tile_width / 2
            ) {
            isIn = true;
        }
        int config = GlobalData::shared()->worldConfig.reskey;
        
        int x = (int) ceil(abs(pt.x - ((_tile_count_x - 1) / 2)) / (config * 1.0));
        int y = (int) ceil(abs(pt.y - ((_tile_count_y - 1) / 2)) / (config * 1.0));
        int resLv = max(x, y);
        
        if(resLv < 3){
            isIn = true;
        }
        return isIn;
    };
    auto currentPt = WorldController::getPointByIndex(index);
    bool flag1 = checkPt(currentPt);
    bool flag2 = checkPt(currentPt - ccp(1, 0));
    bool flag3 = checkPt(currentPt - ccp(0, 1));
    bool flag4 = checkPt(currentPt - ccp(1, 1));
    return flag1 || flag2 || flag3 || flag4;
}
bool WorldController::isFourGrid(WorldCityType type){
    if (type == CityTile || type == MonsterTile || type == Trebuchet || type == Tile_allianceArea || type == tile_superMine || type == tile_wareHouse) {
        return true;
    }
    return false;
}
void WorldController::removeAllQueue(){
    QueueController::getInstance()->removeQueueByType(TYPE_MARCH);
    QueueController::getInstance()->removeQueueByType(TYPE_OCCUPY_RESOURCE);
    QueueController::getInstance()->removeQueueByType(TYPE_OCCUPY_ALLIANCE);
    QueueController::getInstance()->removeQueueByType(TYPE_OCCUPY_MAZE);
    QueueController::getInstance()->removeQueueByType(TYPE_OCCUPY_CAMP);
    QueueController::getInstance()->removeQueueByType(TYPE_ARMY_MASS);
    QueueController::getInstance()->removeQueueByType(TYPE_BUILDING_TERRITORY);
    QueueController::getInstance()->removeQueueByType(TYPE_DESTROY_TERRITORY);
    QueueController::getInstance()->removeQueueByType(TYPE_REPAIR_TERRITORY);
    QueueController::getInstance()->removeQueueByType(TYPE_STATION_TERRITORY);
    QueueController::getInstance()->removeQueueByType(TYPE_OCCUPY_TERRITORY_RESOURCE);
}
int WorldController::getArmyDirectionByInfo(MarchInfo &marchInfo){
    auto addStartPt = ccp(0, 0);
    auto addEndPt = ccp(0, 0);
    if(marchInfo.stateType == StateReturn){
        addEndPt = ccp(0, _tile_height / 2);
    }else{
        addStartPt = ccp(0, _tile_height / 2);
    }
    
    if(isFourGrid(marchInfo.targetType)){
        if(marchInfo.stateType == StateMarch){
            addEndPt = ccp(0, _tile_height / 2);
        }else{
            addStartPt = ccp(0, _tile_height / 2);
        }
    }
    
    CCPoint startPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(marchInfo))) + addStartPt;
    CCPoint endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getEndIndex(marchInfo))) + addEndPt;
    
    int direction = CCMathUtils::getAngle(endPt, startPt);
    int directionStr = 0;
    
    direction += 360;
    direction = fmodf(direction, 360);
    
    float angle = 360 / 16;
    
    if((direction >= 360 - angle && direction <= 360) || (direction >= 0 && direction <= angle)){
        directionStr = 0;//向西行军
    }else if(direction > angle && direction <= 90 - angle){
        directionStr = 1;
    }else if(direction > 90 - angle && direction <= 90 + angle){
        directionStr = 2;
    }else if(direction > 90 + angle && direction <= 180 - angle){
        directionStr = 3;
    }else if(direction > 180 - angle && direction <= 180 + angle){
        directionStr = 4;
    }else if(direction > 180 + angle && direction <= 270 - angle){
        directionStr = 5;
    }else if(direction > 270 - angle && direction <= 270 + angle){
        directionStr = 6;
    }else if(direction > 270 + angle && direction <= 360 - angle){
        directionStr = 7;
    }
    return directionStr;//8个象限，0，45，90……
}

MapType WorldController::getMapTypeByDefault(){
    MapType mapType = NORMAL_MAP;
    if(GlobalData::shared()->isCrossService){
        mapType = SERVERFIGHT_MAP;
    }
    return mapType;
}

void WorldController::enterMap(MapType mapType){
    if(mapType == DEFAULT_MAP){
        mapType = getMapTypeByDefault();
    }
    int mapX = _tile_count_x;
    int mapY = _tile_count_y;
    if(mapType == MapType::SERVERFIGHT_MAP){
        mapX = GlobalData::shared()->worldConfig.ckf_world_row;
        mapY = GlobalData::shared()->worldConfig.ckf_world_row;
    }
    enterMap(mapType, mapX, mapY);
}

int WorldController::getMapXByType(MapType type){
    if(type == DEFAULT_MAP){
        type = getMapTypeByDefault();
    }
    int mapX = _tile_count_x;
    if(type == MapType::SERVERFIGHT_MAP){
        mapX = GlobalData::shared()->worldConfig.ckf_world_row;
    }
    return mapX;
}

int WorldController::getMapYByType(MapType type){
    if(type == DEFAULT_MAP){
        type = getMapTypeByDefault();
    }
    int mapY = _tile_count_y;
    if(type == MapType::SERVERFIGHT_MAP){
        mapY = GlobalData::shared()->worldConfig.ckf_world_row;
    }
    return mapY;
}

void WorldController::enterMap(MapType mapType, int mapx, int mapy){
    if(mapType == DEFAULT_MAP){
        mapType = getMapTypeByDefault();
    }

    currentMapType = mapType;
    _current_tile_count_x = mapx;
    _current_tile_count_y = mapy;
    _current_map_width = mapx * _tile_width;
    _current_map_height = mapy * _tile_height;
    _current_index_limit = mapx * mapy;
    _current_map_has_relic_area = (mapType == NORMAL_MAP);
}

bool WorldController::isInMap(int x, int y, int tileX/* = _current_tile_count_x*/, int tileY/* = _current_tile_count_y*/){
    if(tileX == -1){
        tileX = WorldController::getInstance()->_current_tile_count_x;
        tileY = WorldController::getInstance()->_current_tile_count_y;
    }
    if(x < 0 || x >= tileX || y < 0 || y >= tileY){
        return false;
    }
    return true;
}

bool WorldController::isInMap(CCPoint &pt, int tileX, int tileY){
    if(tileX == -1){
        tileX = WorldController::getInstance()->_current_tile_count_x;
        tileY = WorldController::getInstance()->_current_tile_count_y;
    }

    return isInMap(pt.x, pt.y, tileX, tileY);
}
void WorldController::initServerMapPos(){
        serverMap_count_x = 3;
        serverMap_count_y = 3;
        serverMap_centerPoint = CCPointMake(0, 0);
        serverMap_width = serverMap_count_x * _big_mapWidth;
        serverMap_height = serverMap_count_y * _big_mapHeight;
        serverMap_centerViewPoint = CCPointMake(serverMap_width / 2, serverMap_height / 2);
        CCLog("serverMapWidth:%d serverMapHeight:%d",serverMap_width,serverMap_height);
        CCLog("centerPoint x:%f y:%f",serverMap_centerViewPoint.x,serverMap_centerViewPoint.y);
}

bool WorldController::isInSelfServer(int tileServerId){
     return tileServerId == GlobalData::shared()->playerInfo.selfServerId;
}
void WorldController::initTreasureInfo(CCArray* array){
    clearTreasureInfo();
    if(!array){
        if(false){ //fake data, test code
//            int index = 0;
//            while(index<10){
//                int targetid = CCMathUtils::getRandomInt(100000, 300000);
//                int color = CCMathUtils::getRandomInt(1, 5);
//                TreasureInfo trInfo = {"","","","",targetid,600,color,0};
////                CCPoint ccccp = getPointByIndex(targetid);
////                CCLog("treasure_point:[%d, %d]",(int)ccccp.x,(int)ccccp.y);
//                m_treasureInfo.push_back(trInfo);
//                ++index;
//            }
        }
    }else{
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array, obj){
            CCDictionary* ccDict = _dict(obj);
            addTreasureInfo(ccDict);
        }
    }
}
void WorldController::addTreasureInfo(cocos2d::CCDictionary *ccDict){
    if(!ccDict)
        return;
    TreasureInfo trInfo = {"","","","",0,0,0,0};
    if(ccDict->objectForKey("uuid")){
        trInfo.uuid = ccDict->valueForKey("uuid")->getCString();
    }
    if(ccDict->objectForKey("treasureId")){
        trInfo.treasuerId = ccDict->valueForKey("treasureId")->getCString();
    }
    if(ccDict->objectForKey("currentOwner")){
        trInfo.currentOwner = ccDict->valueForKey("currentOwner")->getCString();
    }
    if(ccDict->objectForKey("originalOwner")){
        trInfo.originalOwner = ccDict->valueForKey("originalOwner")->getCString();
    }
    if(ccDict->objectForKey("targetId")){
        trInfo.targetId = ccDict->valueForKey("targetId")->intValue();
//        CCPoint ccccp = getPointByIndex(trInfo.targetId);
//        CCLog("treasure_point:[%d, %d]",(int)ccccp.x,(int)ccccp.y);
    }
    if(ccDict->objectForKey("time")){
        trInfo.time = ccDict->valueForKey("time")->intValue();
    }
    if(ccDict->objectForKey("colour")){
        trInfo.colour = ccDict->valueForKey("colour")->intValue();
    }
    if(ccDict->objectForKey("state")){
        trInfo.state = ccDict->valueForKey("state")->intValue();
    }
    if(!trInfo.treasuerId.empty()){
        m_treasureInfo.push_back(trInfo);
    }
}
void WorldController::removeTreasureInfo(string treasureuuid){
    auto iter = m_treasureInfo.begin();
    while (iter!=m_treasureInfo.end()) {
        if((*iter).uuid ==  treasureuuid){
            m_treasureInfo.erase(iter);
            break;
        }
        ++iter;
    }
}
bool WorldController::inTreasureArea(const cocos2d::CCPoint &pos){
    auto iter = m_treasureInfo.begin();
    while (iter!=m_treasureInfo.end()) {
        CCPoint ccp = getPointByIndex((*iter).targetId);
        CCRect rect = CCRect(ccp.x-5, ccp.y-5, 10, 10);
        if(rect.containsPoint(pos)){
            return true;
        }
        ++iter;
    }
    return false;
}
void WorldController::clearTreasureInfo(){
    m_treasureInfo.clear();
}
void WorldController::sendGetTreasureCMD(int itemid){
    if(ToolController::getInstance()->m_toolInfos.find(itemid)!=ToolController::getInstance()->m_toolInfos.end()){
        WorldTreasureMapGetCommand *cmd = new WorldTreasureMapGetCommand(CC_ITOA(itemid));
        cmd->sendAndRelease();
    }
}
void WorldController::speedupTreasureMap(string marchid){
    WorldTreasureMapSpeedupCommand *cmd = new WorldTreasureMapSpeedupCommand(marchid);
    cmd->sendAndRelease();
}
int WorldController::getTreasureInfoCount(){
    return m_treasureInfo.size();
}
void WorldController::getTreasureList(int color , vector<TreasureInfo> &list){
    auto iter = m_treasureInfo.begin();
    while (iter!=m_treasureInfo.end()) {
        if((*iter).colour == color){
            list.push_back((*iter));
        }
        ++iter;
    }
}
//-----------
static TimeUpMarchProcesser* _timeUpInstance = NULL;

TimeUpMarchProcesser *TimeUpMarchProcesser::getInstance(){
    if(!_timeUpInstance){
        _timeUpInstance = new TimeUpMarchProcesser();
    }
    return _timeUpInstance;
}

void TimeUpMarchProcesser::addTimeUpMarch(std::string marchUid){
    if(m_requestMap.find(marchUid) == m_requestMap.end()){
        m_requestMap[marchUid] = WorldController::getInstance()->getTime();
    }
}

void TimeUpMarchProcesser::removeTimeUpMarch(std::string marchUid){
    auto it = m_requestMap.find(marchUid);
    if(it != m_requestMap.end()){
        CCLOG("remove march:%s", marchUid.c_str());
        m_requestMap.erase(it);
    }
}

void TimeUpMarchProcesser::check(MarchInfo &info){
    if(info.ownerType == PlayerSelf && info.playerName == GlobalData::shared()->playerInfo.name){
        if(info.stateType == StateMarch){
            if(info.endStamp < WorldController::getInstance()->getTime()){
                CCLOG("check march:%s%s", info.uuid.c_str(), CC_ITOA(int(info.marchTag)));
                addTimeUpMarch(info.uuid);
            }
        }else if(info.stateType == StateReturn){
            removeTimeUpMarch(info.uuid);
        }else{
            removeTimeUpMarch(info.uuid);
        }
    }
//    else{
//        removeTimeUpMarch(info.uuid);
//    }
}

void TimeUpMarchProcesser::refresh(){
    if(m_retryTime < MAX_RETRY_TIME && isNeedSendRequest()){
        sendRequest();
    }
}

void TimeUpMarchProcesser::requestDataBack(CCDictionary *dict){
    clear();
    if(dict){
        CCArray *soldierArr = NULL;
        if(dict->objectForKey("army")){
            soldierArr = dynamic_cast<CCArray*>(dict->objectForKey("army"));
        }
        if(soldierArr){
            CCObject* p = NULL;
            CCARRAY_FOREACH(soldierArr, p) {
                auto info = dynamic_cast<CCDictionary*>(p);
                ArmyController::getInstance()->refreshArmyFreeNum(info);
            }
        }
        
        CCArray *marchArr = NULL;
        if(dict->objectForKey("m")){
            marchArr = dynamic_cast<CCArray*>(dict->objectForKey("m"));
        }
        auto now = WorldController::getInstance()->getTime();
        //删除自己所有现有行军队列
        for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end();) {
            if(it->second.ownerType == PlayerSelf){
                if(WorldMapView::instance()){
                    if(WorldMapView::instance()->m_occupyPointerNode){
                        WorldMapView::instance()->m_occupyPointerNode->removeChildByTag(it->second.marchTag);
                    }
                }
                it->second.endStamp = now;
                it->second.action = MarchActionNone;
                it->second.stateType = StateReturn;
                if (WorldController::getInstance()->initWorld && WorldMapView::instance()){
                    WorldMapView::instance()->updateMarchTarget(it->second, now, 1000);
                }

                WorldController::getInstance()->m_marchInfo.erase(it++);
            }else{
                ++it;
            }
        }
        WorldController::getInstance()->m_selfMarchUuid.clear();
        WorldController::getInstance()->removeAllQueue();
        
        if(marchArr){
            WorldController::getInstance()->initMarchInfo(marchArr);
        }
    }
}

bool TimeUpMarchProcesser::isNeedSendRequest(){
    bool result = true;
    if(m_requestMap.size() == 0){
        result = false;
    }else{
        double currentTime = WorldController::getInstance()->getTime();
        for (auto it = m_requestMap.begin(); it != m_requestMap.end(); it++) {
            if(currentTime - MARCH_TIMEUP_DELAY_REQUEST_TIME * 1000 <= it->second){
                result = false;
            }
        }
    }
    return result;
}

void TimeUpMarchProcesser::sendRequest(){
    for (auto it = m_requestMap.begin(); it != m_requestMap.end(); it++) {
        it->second = WorldController::getInstance()->getTime();
    }
    WorldMarchTimeUpCheckCommand *cmd = new WorldMarchTimeUpCheckCommand();
    cmd->sendAndRelease();
    m_retryTime++;
    CCLOG("call time_up command.");
}

void TimeUpMarchProcesser::clear(){
    m_requestMap.clear();
    m_retryTime = 0;
}

void TimeUpMarchProcesser::resetRetryTime(){
    m_retryTime = 0;
}
