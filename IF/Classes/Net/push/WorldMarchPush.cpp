//
//  WorldMarchPush.cpp
//  IF
//
//  Created by 邹 程 on 14-2-28.
//
//

#include "WorldMarchPush.h"
#include "CCCommonUtils.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "CityCrashView.h"
#include "QueueController.h"
#include "FunBuildController.h"
#include "TroopsController.h"
#include "RewardController.h"
#include "SceneController.h"
#include "SoundController.h"
#include "WorldMapView.h"
#include "ChatServiceCocos2dx.h"
#include "DynamicTiledMap.h"

void WorldMarchPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    

    string uuid = params->valueForKey("uuid")->getCString();
    MarchStateType stateType = (MarchStateType)params->valueForKey("sType")->intValue();

    std::string team = "";
    if(params->objectForKey("team")){
        team = params->valueForKey("team")->getCString();
    }

    unsigned int staPoint = params->valueForKey("sp")->intValue();
    unsigned int desPoint = params->valueForKey("dp")->intValue();
    double startTime = params->valueForKey("st")->doubleValue();
    double endTime = params->valueForKey("et")->doubleValue();
    double mt = endTime - startTime;
    if(params->objectForKey("mt")){
        mt = params->valueForKey("mt")->doubleValue();
    }
    CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("mt2"));

    double allianceTeamCreateTime = -1;
    if(params->objectForKey("tct")){
        allianceTeamCreateTime = params->valueForKey("tct")->doubleValue();
    }
    std::string asn = "";
    if(params->objectForKey("asn")){
        asn = params->valueForKey("asn")->getCString();
    }

    MarchMethodType marchType = (MarchMethodType)params->valueForKey("mType")->intValue();
    WorldCityType targetType = (WorldCityType)params->valueForKey("tType")->intValue();
    PlayerType ownerType = (PlayerType)params->valueForKey("oType")->intValue();
    string playerName = params->valueForKey("o")->getCString();
    std::string desLeague = "";
    if(params->objectForKey("tali")){
        desLeague = params->valueForKey("tali")->getCString();
    }
    std::string officer = params->valueForKey("officer")->getCString();
    unsigned int marchColor = params->valueForKey("color")->intValue();
    
    string ouid = "";
    if (params->objectForKey("ouid")) {
        ouid = params->valueForKey("ouid")->getCString();
    }
    int  pMonsterLevel = 0;
    if (params->objectForKey("monsterLevel")) {
        pMonsterLevel = params->valueForKey("monsterLevel")->intValue();
    }
    int  pMonsterCount = 0;
    if (params->objectForKey("monsterCount")) {
        pMonsterCount = params->valueForKey("monsterCount")->intValue();
    }
    double tfinishTime = 0;
    if( params->objectForKey("treasureFinishTime")){
        tfinishTime = params->valueForKey("treasureFinishTime")->doubleValue();
    }
    auto& marchInfo = WorldController::getInstance()->m_marchInfo[uuid];
    marchInfo.uuid = uuid;
    marchInfo.officer = officer;
    marchInfo.marchToIndexLeague = desLeague;
    marchInfo.startPointIndex = staPoint;
    marchInfo.endPointIndex = desPoint;
    marchInfo.startStamp = startTime;
    marchInfo.mt = mt;
    marchInfo.endStamp = endTime;
    marchInfo.allianceTeamCreateTime = allianceTeamCreateTime;
    marchInfo.teamUid = team;
    marchInfo.asn = asn;
    marchInfo.marchType = marchType;
    marchInfo.stateType = stateType;
    marchInfo.targetType = targetType;
    marchInfo.ownerType = ownerType;
    marchInfo.isChangingSpeed = true;
    marchInfo.playerName = playerName;
    marchInfo.updateSoldiers(params);
    marchInfo.marchColor = marchColor;
    marchInfo.parseMarchTime(arr);
    marchInfo.ouid = ouid;
    marchInfo.monsterLevel = pMonsterLevel;
    marchInfo.monsterCount = pMonsterCount;
    marchInfo.serverId = GlobalData::shared()->playerInfo.selfServerId;
    if (marchInfo.marchTag == 0) {
        marchInfo.marchTag = WorldController::getInstance()->getMarchTag();
    }
    
    //领主技能
    bool slowDown=false;
    if (params->objectForKey("slowDown")) {
        slowDown = params->valueForKey("slowDown")->boolValue();
    }
    marchInfo.slowDown = slowDown;
    
    marchInfo.treasureMapFinishTime = tfinishTime;
    CCLOG("logInfo:--------------worldmarchPush------------uuid----------------------%s",marchInfo.uuid.c_str());
    CCLOG("logInfo:--------------worldmarchPush------------marchTag----------------------%d",marchInfo.marchTag);
    if (stateType == StateReturn && targetType == ResourceTile && marchType == MethodBattle) {
        if (params->objectForKey("empty")) {
            marchInfo.action = MarchActionUnsetResource;
        } else {
            marchInfo.action = MarchActionUnsetOccupy;
        }
    }
    
    if(stateType == StateReturn && targetType == MonsterTile){
        WorldController::getInstance()->isInfoTake = false;
        WorldController::getInstance()->m_monsterAttackInfo->removeAllObjects();
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if(stateType == StateReturn && targetType == Tile_allianceArea){
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if(stateType == StateReturn && targetType == tile_superMine){
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if(stateType == StateReturn && targetType == tile_tower){
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if (stateType == StateReturn && targetType == tile_wareHouse) {
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if (stateType == StateReturn && targetType == tile_banner) {
        marchInfo.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    if(stateType == StateMarch && targetType == MonsterTile){
        if (marchInfo.ownerType == PlayerSelf) {
            WorldController::getInstance()->m_selfMarchUuid[marchInfo.endPointIndex] = marchInfo.uuid;
        }
    }
    
    if (params->objectForKey("prt")) {
        // cancel protect state
        if(ownerType == PlayerSelf){
            GlobalData::shared()->playerInfo.protectTimeStamp = 0;
            GlobalData::shared()->statusMap[PLAYER_PROTECTED_TIME1] = 0;
            GlobalData::shared()->statusMap[PLAYER_PROTECTED_TIME2] = 0;
            GlobalData::shared()->statusMap[PLAYER_PROTECTED_TIME3] = 0;
            GlobalData::shared()->statusMap[PLAYER_PROTECTED_TIME4] = 0;
            GlobalData::shared()->statusMap[PLAYER_PROTECTED_TIME5] = 0;
        }
        WorldController::getInstance()->m_playerInfo[playerName].protectStamp = 2;
    }
    
    if(marchInfo.stateType == StateReturn && marchInfo.targetType == CityTile && marchInfo.ownerType == PlayerSelf)
    {
        switch (marchInfo.marchType) {
            case MethodScout:
                CCCommonUtils::flyHint("march_icon.png", _lang("136005"), _lang_1("136006", marchInfo.playerName.c_str()), 3, 0, true);
                break;
            case MethodDeal:
                CCCommonUtils::flyHint("march_icon.png", _lang("136014"), _lang_1("136015", marchInfo.playerName.c_str()), 3, 0, true);
            case MethodWarehouse:
                CCCommonUtils::flyHint("march_icon.png", _lang("136014"), _lang_1("136015", marchInfo.playerName.c_str()), 3, 0, true);/////
            case MethodUnion:
                CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", marchInfo.playerName.c_str()), 3, 0, true);
                break;
            default:
                break;
        }
        if(marchInfo.marchType == MethodScout || marchInfo.marchType == MethodDeal || marchInfo.marchType == MethodWarehouse)
        {
            SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
        }
    }
    
    if (ownerType == PlayerSelf) {
        TroopsController::getInstance()->retSaveBattle();
        QueueController::getInstance()->addMarchQueueInfo(uuid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARCH_UPDATE,CCString::create(uuid));
        if(marchInfo.stateType == StateMarch && marchInfo.marchType == MethodBattle){
            auto player = WorldController::getInstance()->m_playerInfo.find(GlobalData::shared()->playerInfo.name);
            if (player != WorldController::getInstance()->m_playerInfo.end() && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) {
                if(player->second.protectStamp > 1) {
                    WorldMapView::instance()->m_map->updateDynamicMap();//just monitor this code TODO:wdz
                }
            }
        }
    }
    double currentTime = WorldController::getInstance()->getTime();
    int dTime = currentTime - startTime;
    dTime = abs(dTime);
    if(dTime > 1000 && dTime < 10000){
        WorldController::getInstance()->resetTime(startTime);
    }
}

void WorldMarchWinPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    
    string uuid = params->valueForKey("uuid")->getCString();
    for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
        if(it->second.marchType == MethodUnion || it->second.marchType == MethodRally){
            if(it->second.teamUid == uuid && it->second.teamUid != it->second.uuid && it->second.ownerType == PlayerSelf){
                uuid = it->second.uuid;
                break;
            }
        }
    }
    string ownerName = params->objectForKey("o") ? params->valueForKey("o")->getCString() : "";
    bool hasBattle = params->objectForKey("fight") ? true : false;
//    hasBattle = true;
    
    auto &info = WorldController::getInstance()->m_marchInfo[uuid];
    auto &resInfo = WorldController::getInstance()->getCityInfos()[info.stateType == StateReturn ? info.startPointIndex : info.endPointIndex];
    
    switch (info.targetType) {
        case Throne:{
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }

            info.stateType = StateOccupy;
            if(WorldMapView::instance() && WorldController::getInstance()->initWorld){
                if(WorldMapView::instance()->m_map){
                    WorldMapView::instance()->m_map->updateDynamicMap();
                }
            }
        }
            break;
        case Trebuchet:{
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
            if(WorldMapView::instance() && WorldController::getInstance()->initWorld){
                if(WorldMapView::instance()->m_map){
                    WorldMapView::instance()->m_map->updateDynamicMap();
                }
            }
        }
            break;
        case CampTile:{
            resInfo.playerName = ownerName;
            if (info.ownerType == PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
                
            }

            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
            
            if(WorldMapView::instance() && WorldController::getInstance()->initWorld){
                if(WorldMapView::instance()->m_map){
                    WorldMapView::instance()->m_map->updateDynamicMap();
                }
            }
            
            if(params->objectForKey("treasureMap")){
                CCDictionary *dictTrea = _dict(params->objectForKey("treasureMap"));
                if(dictTrea->objectForKey("finishtime")){
                    info.treasureMapFinishTime = dictTrea->valueForKey("finishtime")->doubleValue();
                }
                if(dictTrea->objectForKey("time")){
                    info.treasureMapTime = dictTrea->valueForKey("time")->intValue();
                }
                if(info.ownerType == PlayerSelf){
                    if(dictTrea->objectForKey("useitem")){
                        string itemid = dictTrea->valueForKey("useitem")->getCString();
                        ToolController::getInstance()->useTool(atoi(itemid.c_str()),1,false);
                    }
                }
                if(dictTrea->objectForKey("treasureuuid")){
                    WorldController::getInstance()->removeTreasureInfo(dictTrea->valueForKey("treasureuuid")->getCString());
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.treasure.map.info.change");
                }
            }
        }
            break;
        case CityTile:{
            if(info.marchType == MethodYuanSolider){
                info.action = MarchActionSetOccupy;
                info.stateType = StateOccupy;
                if (info.ownerType == PlayerSelf) {
                    WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                    CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", ownerName.c_str()), 3, 0, true);
                }
            }else if (info.marchType == MethodUnion) {
                // union for rally attack , troop should stay there
                double allianceTeamCreateTime = -1;
                if(params->objectForKey("tct")){
                    allianceTeamCreateTime = params->valueForKey("tct")->doubleValue();
                }
                std::string team = "";
                if(params->objectForKey("team")){
                    team = params->valueForKey("team")->getCString();
                }
                double startTime = params->valueForKey("st")->doubleValue();
                double endTime = params->valueForKey("et")->doubleValue();
                double mt = endTime - startTime;
                if(params->objectForKey("mt")){
                    mt = params->valueForKey("mt")->doubleValue();
                }

                info.startStamp = startTime;
                info.endStamp = endTime;
                info.mt = mt;
                info.updateSoldiers(params);
                info.allianceTeamCreateTime = allianceTeamCreateTime;
                info.teamUid = team;
                info.action = MarchActionSetOccupy;
                info.stateType = StateOccupy;
                if (info.ownerType == PlayerSelf)
                {
                    CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", info.playerName.c_str()), 3, 0, true);
                }
                
            } else {
                // attack others city
                double fireStamp = params->valueForKey("ft")->doubleValue();
                WorldController::getInstance()->m_playerInfo[resInfo.playerName].fireStamp = fireStamp;
                if (WorldMapView::instance() && info.marchType != MethodHeiqishi) {
                    WorldMapView::instance()->delBatchItem(FireTag, resInfo.cityIndex);
                    WorldMapView::instance()->addBatchItem(FireTag, resInfo.cityIndex);
                }
                unsigned int staPoint = params->valueForKey("sp")->intValue();
                unsigned int desPoint = params->valueForKey("dp")->intValue();
                double startTime = params->valueForKey("st")->doubleValue();
                double endTime = params->valueForKey("et")->doubleValue();
                double mt = endTime - startTime;
                if(params->objectForKey("mt")){
                    mt = params->valueForKey("mt")->doubleValue();
                }

                info.updateSoldiers(params);
                info.startPointIndex = staPoint;
                info.endPointIndex = desPoint;
                info.startStamp = startTime + AttackAnimationTime;
                info.endStamp = endTime + AttackAnimationTime;
                info.mt = mt;
                info.action = MarchActionPrepareAttack;
                info.stateType = StateReturn;
                if (info.marchType == MethodHeiqishi) {
                    info.endStamp = WorldController::getInstance()->getTime() + 8 * 1000;
                }
                if (info.ownerType == PlayerSelf)
                {
                    if(info.marchType == MethodRally)
                    {
                        CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", ownerName.c_str()), 3, 0, true);
                    }
                    else
                    {
                        CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
                    }
                }
            }
        }
            break;
        case ResourceTile:{
            if (info.ownerType == PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            resInfo.playerName = ownerName;
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case Tile_allianceArea:{
            if (info.ownerType==PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case tile_superMine:{
            if (info.ownerType==PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case tile_tower:{
            if (info.ownerType==PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case tile_wareHouse:{
            if (info.ownerType==PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case tile_banner:{
            if (info.ownerType==PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            } else {
                info.action = MarchActionSetOccupy;
            }
            info.stateType = StateOccupy;
        }
            break;
        case KingTile:{
            resInfo.playerName = ownerName;
            if (hasBattle) {
                info.action = MarchActionPrepareAttack;
            }
            info.stateType = StateOccupy;
        }
            break;
        case BattleTile:{
            unsigned int subType = params->valueForKey("t1")->intValue();
            if (subType == 3) {
                info.action = MarchActionCollectChest;
                
                unsigned int staPoint = params->valueForKey("sp")->intValue();
                unsigned int desPoint = params->valueForKey("dp")->intValue();
                double startTime = params->valueForKey("st")->doubleValue();
                double endTime = params->valueForKey("et")->doubleValue();
                double mt = endTime - startTime;
                if(params->objectForKey("mt")){
                    mt = params->valueForKey("mt")->doubleValue();
                }

                info.updateSoldiers(params);
                info.startPointIndex = staPoint;
                info.endPointIndex = desPoint;
                info.startStamp = startTime;
                info.endStamp = endTime;
                info.mt = mt;
                info.stateType = StateReturn;

            } else {
                // set tower
                resInfo.playerName = ownerName;
                if (hasBattle) {
                    info.action = MarchActionPrepareAttack;
                }
                info.stateType = StateOccupy;
            }
        }
            break;
        case MonsterTile:{
            if (info.ownerType == PlayerSelf) {
                WorldController::getInstance()->m_selfMarchUuid[info.endPointIndex] = info.uuid;
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
            info.action = MarchActionSetOccupy;
            info.stateType = StateOccupy;
        }
            break;
        case FieldMonster:{
            unsigned int staPoint = params->valueForKey("sp")->intValue();
            unsigned int desPoint = params->valueForKey("dp")->intValue();
            double startTime = params->valueForKey("st")->doubleValue();
            double endTime = params->valueForKey("et")->doubleValue();
            double mt = endTime - startTime;
            if(params->objectForKey("mt")){
                mt = params->valueForKey("mt")->doubleValue();
            }
            if(params->objectForKey("mId")){
                std::string mId = params->valueForKey("mId")->getCString();
                int level = atoi(CCCommonUtils::getPropById(mId, "level").c_str());
                if(WorldController::getInstance()->currentMonsterLevel < level && info.ownerType == PlayerSelf){
                    WorldController::getInstance()->currentMonsterLevel = level;
                }
            }
            info.updateSoldiers(params);
            info.startPointIndex = staPoint;
            info.endPointIndex = desPoint;
            info.startStamp = startTime + AttackAnimationTime;
            info.endStamp = endTime + AttackAnimationTime;
            info.mt = mt;
            info.action = MarchActionPrepareAttack;
            info.stateType = StateReturn;
            
            if(params->objectForKey("mchp")){
                unsigned int mchp = params->valueForKey("mchp")->intValue();
                resInfo.fieldMonsterInfo.currentHp = mchp;
                resInfo.fieldMonsterInfo.isHpChange = true;
            }
            if (info.ownerType == PlayerSelf)
            {
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
        }
            break;
        case ActBossTile:{
            unsigned int staPoint = params->valueForKey("sp")->intValue();
            unsigned int desPoint = params->valueForKey("dp")->intValue();
            double startTime = params->valueForKey("st")->doubleValue();
            double endTime = params->valueForKey("et")->doubleValue();
            double mt = endTime - startTime;
            if(params->objectForKey("mt")){
                mt = params->valueForKey("mt")->doubleValue();
            }
            if(params->objectForKey("mId")){
                std::string mId = params->valueForKey("mId")->getCString();
                int level = atoi(CCCommonUtils::getPropById(mId, "level").c_str());
                if(WorldController::getInstance()->currentMonsterLevel < level && info.ownerType == PlayerSelf){
                    WorldController::getInstance()->currentMonsterLevel = level;
                }
            }
            info.updateSoldiers(params);
            info.startPointIndex = staPoint;
            info.endPointIndex = desPoint;
            info.startStamp = startTime + AttackAnimationTime;
            info.endStamp = endTime + AttackAnimationTime;
            info.mt = mt;
            info.action = MarchActionPrepareAttack;
            info.stateType = StateReturn;
            
            if(params->objectForKey("mchp")){
                unsigned int mchp = params->valueForKey("mchp")->intValue();
                resInfo.fieldMonsterInfo.currentHp = mchp;
                resInfo.fieldMonsterInfo.isHpChange = true;
            }
            if (info.ownerType == PlayerSelf)
            {
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", ownerName.c_str()), 3, 0, true);
            }
        }
            break;
        default:
            break;
    }
    if (info.ownerType == PlayerSelf) {
        // add reward
        if (params->objectForKey("reward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        
        if (params->objectForKey("rateReward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("rateReward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        
        if (params->objectForKey("killReward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("killReward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        
        if(params->objectForKey("stamina")){
            WorldController::getInstance()->currentStamine = params->valueForKey("stamina")->intValue();
        }
        if(params->objectForKey("lastStaminaTime")){
            WorldController::getInstance()->lastStamineTime = params->valueForKey("lastStaminaTime")->doubleValue();
        }

        if (info.targetType == MonsterTile) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MONSTER_UPDATE,CCInteger::create(info.startPointIndex));
        }
        unsigned int qid = QueueController::getInstance()->getMarchQueueQid(info.uuid);
        GlobalData::shared()->allQueuesInfo.erase(qid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
        if(info.stateType == StateOccupy){
            if(params->objectForKey("q")){
                info.troopMax = params->valueForKey("q")->intValue();
                info.digSpeed = params->valueForKey("spd")->floatValue();
                info.digStartTime = params->valueForKey("sdt")->doubleValue();
                info.digStartNum = params->valueForKey("collected")->intValue();
                info.changeCollectSpdTime = params->valueForKey("changeCollectSpdTime")->doubleValue();
            }
            if(params->objectForKey("exploreSt")){
                info.mazeStartTime = params->valueForKey("exploreSt")->doubleValue();
                info.mazeEndTime = params->valueForKey("exploreTime")->doubleValue() + info.mazeStartTime;
            }
        }
        
        SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
        
        QueueController::getInstance()->addMarchQueueInfo(info.uuid);
    }
}

void WorldMarchLosePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    
    string uuid = params->valueForKey("uuid")->getCString();
    unsigned int staPoint = params->valueForKey("sp")->intValue();
    unsigned int desPoint = params->valueForKey("dp")->intValue();
    double startTime = params->valueForKey("st")->doubleValue();
    double endTime = params->valueForKey("et")->doubleValue();
    bool hasBattle = params->objectForKey("fight") ? true : false;
    double mt = endTime - startTime;
    if(params->objectForKey("mt")){
        mt = params->valueForKey("mt")->doubleValue();
    }
    
    auto &info = WorldController::getInstance()->m_marchInfo[uuid];
    info.updateSoldiers(params);
    info.startPointIndex = staPoint;
    info.endPointIndex = desPoint;
    info.startStamp = startTime;
    info.endStamp = endTime;
    info.treasureMapFinishTime=0;
    info.treasureMapTime=0;
//    info.isChangingSpeed = true;
    info.mt = mt;
    if (hasBattle) {
        info.action = MarchActionPrepareAttack;
        info.startStamp += AttackAnimationTime;
        if(endTime == 0){
            info.endStamp = WorldController::getInstance()->getTime() + AttackTime + 2000;
            info.isKilled = true;
        }else{
            info.endStamp += AttackAnimationTime;
        }
    }else{
        info.isChangingSpeed = true;
    }
    if (info.marchType == MethodHeiqishi) {
        info.endStamp = WorldController::getInstance()->getTime() + 8 * 1000;
    }
    info.stateType = StateReturn;

    if (info.targetType == FieldMonster || info.targetType == ActBossTile) {
        if(params->objectForKey("mchp")){
            unsigned int mchp = params->valueForKey("mchp")->intValue();
            auto it = WorldController::getInstance()->getCityInfos().find(info.startPointIndex);
            if(it != WorldController::getInstance()->getCityInfos().end()){
                it->second.fieldMonsterInfo.currentHp = mchp;
                it->second.fieldMonsterInfo.isHpChange = true;
            }
        }
    }
    
    if(info.ownerType == PlayerSelf)
    {
        switch (info.targetType)
        {
            case CampTile:
            case ResourceTile:
            case FieldMonster:
            case ActBossTile:
            case MonsterTile:
                CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                break;

            case CityTile:
                switch (info.marchType)
                {
                    case MethodRally:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", info.playerName.c_str()), 3, 0, true);
                        break;
                    default:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                        break;
                }
                SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                break;
            case Tile_allianceArea:
                switch (info.marchType) {
                    case MethodRally:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", info.playerName.c_str()), 3, 0, true);
                        break;
                    default:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                        break;
                }
                SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                break;
            case tile_banner:
                switch (info.marchType) {
                    case MethodRally:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136009"), _lang_1("136010", info.playerName.c_str()), 3, 0, true);
                        break;
                    default:
                        CCCommonUtils::flyHint("march_icon.png", _lang("136000"), _lang_1("136001", info.playerName.c_str()), 3, 0, true);
                        break;
                }
                SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                break;
            default:
                break;
        }
    }

    if (info.ownerType == PlayerSelf) {
        // add reward
        if (params->objectForKey("reward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        
        if (params->objectForKey("rateReward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("rateReward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        
        if (params->objectForKey("killReward")) {
            auto rewardArr = dynamic_cast<CCArray*>(params->objectForKey("killReward"));
            RewardController::getInstance()->retReward(rewardArr);
        }
        if(params->objectForKey("stamina")){
            WorldController::getInstance()->currentStamine = params->valueForKey("stamina")->intValue();
        }
        if(params->objectForKey("lastStaminaTime")){
            WorldController::getInstance()->lastStamineTime = params->valueForKey("lastStaminaTime")->doubleValue();
        }

        if (info.targetType == MonsterTile) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MONSTER_UPDATE,CCInteger::create(info.startPointIndex));
        }
        QueueController::getInstance()->updateMarchQueueInfo(uuid);
    }
}

void WorldMarchSpeedUpPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    
    string uuid = params->valueForKey("uuid")->getCString();
    
    std::string team = "";
    if(params->objectForKey("team")){
        team = params->valueForKey("team")->getCString();
        for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
            if(it->second.ownerType == PlayerSelf && it->second.teamUid == uuid && it->second.uuid != uuid){
                return;
            }
        }
    }

    double newTime = params->valueForKey("et")->doubleValue();
    auto &info = WorldController::getInstance()->m_marchInfo[uuid];
    info.endStamp = newTime;
    info.isChangingSpeed = true;
    CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("mt2"));
    info.parseMarchTime(arr);
    if (info.ownerType == PlayerSelf) {
        QueueController::getInstance()->updateMarchQueueInfo(uuid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARCH_UPDATE,CCString::create(uuid));
    }
}

void WorldMarchCancelPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    
    string uuid = params->valueForKey("uuid")->getCString();
    
    unsigned int staPoint = params->valueForKey("sp")->intValue();
    unsigned int desPoint = params->valueForKey("dp")->intValue();
    double startTime = params->valueForKey("st")->doubleValue();
    double endTime = params->valueForKey("et")->doubleValue();
    double mt = endTime - startTime;
//    if(params->objectForKey("mt")){
//        mt = params->valueForKey("mt")->doubleValue();
//    }
    CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("mt2"));

    auto &info = WorldController::getInstance()->m_marchInfo[uuid];
    info.updateSoldiers(params);
    info.startPointIndex = staPoint;
    info.endPointIndex = desPoint;
    info.startStamp = startTime;
    info.endStamp = endTime;
    info.mt = mt;
    info.parseMarchTime(arr);
    info.treasureMapFinishTime = 0;
    info.treasureMapTime = 0;
    info.isChangingSpeed = true;
    
    if (info.stateType == StateOccupy && (info.targetType == CampTile || info.targetType == ResourceTile || info.targetType == Tile_allianceArea || info.targetType == tile_superMine || info.targetType == tile_tower || info.targetType == tile_wareHouse || info.targetType == tile_banner || info.targetType == MonsterTile)) {
        info.action = MarchActionUnsetOccupy;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        map<unsigned int , string>::iterator it;
        for(it = selfMarch.begin(); it != selfMarch.end(); it++){
            if(it->second == uuid){
                selfMarch.erase(it);
                break;
            }
        }
    }
    
    //////////联盟领地撤军刷新状态
    if (info.stateType == StateOccupy && info.targetType == Tile_allianceArea) {
        auto &m_info = WorldController::getInstance()->getCityInfos()[staPoint];
        if (m_info.cityType == Tile_allianceArea) {
            AAreaState stat = Astate_None;
            if (m_info.m_allianceAreaInfo.state == Astate_Building) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Building;
                }
                else
                    stat = Astate_BeforeBuild;
            }
            else if (m_info.m_allianceAreaInfo.state == Astate_Garrison) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Garrison;
                }
                else
                    stat = Astate_UnGarrison;
            }
            else if (m_info.m_allianceAreaInfo.state == Astate_FixIng) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_FixIng;
                }
                else
                    stat = Astate_Damaged;
            }
            else if (m_info.m_allianceAreaInfo.state == Astate_Destroy) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Destroy;
                }
                else
                    stat = Astate_Damaged;
            }
            else
                stat = m_info.m_allianceAreaInfo.state;
            if (WorldMapView::instance()) {
                WorldMapView::instance()->onRefreshAAreaBuilding(staPoint, stat);
            }
        }
    }
    
    //////////联盟领地超级矿撤军刷新状态
    if (info.stateType == StateOccupy && info.targetType == tile_superMine) {
        auto &m_info = WorldController::getInstance()->getCityInfos()[staPoint];
        if (m_info.cityType == tile_superMine) {
            AAreaState stat = Astate_None;
            if (m_info.m_superMineInfo.trstat == Astate_Building) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Building;
                }
                else
                    stat = Astate_BeforeBuild;
            }
            else if (m_info.m_superMineInfo.trstat == Astate_Destroy) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Destroy;
                }
                else
                    stat = Astate_UnGarrison;
            }
            else
                stat = m_info.m_superMineInfo.trstat;
            if (WorldMapView::instance()) {
                WorldMapView::instance()->onRefreshAAreaBuilding(staPoint, stat, true);
            }
        }
    }

    //////////联盟领地箭塔撤军刷新状态
    if (info.stateType == StateOccupy && info.targetType == tile_tower) {
        auto &m_info = WorldController::getInstance()->getCityInfos()[staPoint];
        if (m_info.cityType == tile_tower) {
            AAreaState stat = Astate_None;
            if (m_info.m_towerInfo.state == Astate_Building) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Building;
                }
                else
                    stat = Astate_BeforeBuild;
            }
            else
                stat = m_info.m_towerInfo.state;
            if (WorldMapView::instance()) {
                WorldMapView::instance()->onRefreshAAreaBuilding(staPoint, stat, false, true);
            }
        }
    }
    
    //////////联盟领地仓库撤军刷新状态
    if (info.stateType == StateOccupy && info.targetType == tile_wareHouse) {
        auto &m_info = WorldController::getInstance()->getCityInfos()[staPoint];
        if (m_info.cityType == tile_wareHouse) {
            AAreaState stat = Astate_None;
            if (m_info.m_towerInfo.state == Astate_Building) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Building;
                }
                else
                    stat = Astate_BeforeBuild;
            }
            else
                stat = m_info.m_towerInfo.state;
            if (WorldMapView::instance()) {
                WorldMapView::instance()->onRefreshAAreaBuilding(staPoint, stat, false, false, true);
            }
        }
    }
    
    //////////联盟领地国旗撤军刷新状态
    if (info.stateType == StateOccupy && info.targetType == tile_banner) {
        auto &m_info = WorldController::getInstance()->getCityInfos()[staPoint];
        if (m_info.cityType == tile_banner) {
            AAreaState stat = Astate_None;
            if (m_info.m_bannerInfo.state == Astate_Building) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Building;
                }
                else
                    stat = Astate_BeforeBuild;
            }
            else if (m_info.m_bannerInfo.state == Astate_Garrison) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Garrison;
                }
                else
                    stat = Astate_UnGarrison;
            }
            else if (m_info.m_bannerInfo.state == Astate_FixIng) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_FixIng;
                }
                else
                    stat = Astate_Damaged;
            }
            else if (m_info.m_bannerInfo.state == Astate_Destroy) {
                if (m_info.m_allianceAreaOwnerInfo.ownerId != "") {
                    stat = Astate_Destroy;
                }
                else
                    stat = Astate_Damaged;
            }
            else
                stat = m_info.m_bannerInfo.state;
            if (WorldMapView::instance()) {
                WorldMapView::instance()->onRefreshAAreaBuilding(staPoint, stat, false, false, false, true);
            }
        }
    }
    
    if(info.targetType == MonsterTile){
        WorldController::getInstance()->isInfoTake = false;
        WorldController::getInstance()->m_monsterAttackInfo->removeAllObjects();
    }
    
    info.stateType = StateReturn;

    
    if (info.ownerType == PlayerSelf) {
        QueueController::getInstance()->updateMarchQueueInfo(uuid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARCH_UPDATE,CCString::create(uuid));
    }
}

void WorldAllianceAreaPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("tid")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doAllianceArea(1, pointIndex);
    }
}
void WolrdSuperMinePush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("tid")->intValue();
    int type = param->valueForKey("type")->intValue();
    string aaid = param->valueForKey("aaid")->getCString();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doAllianceArea(1, pointIndex,true,false,(WorldResourceType)type,aaid);
    }
}
void WolrdSuperMineCancelPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doAllianceArea(0, pointIndex,true);
    }
}
void WolrdWareHousePush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("tid")->intValue();
    string aaid = param->valueForKey("aaid")->getCString();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doAllianceArea(1, pointIndex,false,true,Wood,aaid);
    }
}
void WolrdWareHouseCancelPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doAllianceArea(0, pointIndex,false,true);
    }
}
void WorldWareHouseTakeBackPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
}
void WorldWareHouseStorePush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    if (param->objectForKey("storage")) {
        GlobalData::shared()->territory_warehouse_dayStorage = param->valueForKey("storage")->longValue();
    }
    if (param->objectForKey("storageTotal")) {
        GlobalData::shared()->territory_warehouse_totalStorage = param->valueForKey("storageTotal")->longValue();
    }
}
void WorldWareHouseRetrievePush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    if (param->objectForKey("storageTotal")) {
        GlobalData::shared()->territory_warehouse_totalStorage = param->valueForKey("storageTotal")->longValue();
    }
}
void WolrdTowerBuildPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("tid")->intValue();
    int dialog = param->valueForKey("ttcount")->intValue();
    string aaid = param->valueForKey("aaid")->getCString();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->pushAddTower(1, pointIndex,dialog,aaid);
    }
}
void WolrdTowerBuildCancelPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->pushAddTower(0, pointIndex,0,"");
        WorldMapView::instance()->initTowerRange(pointIndex, false);
    }
}
void WolrdBannerBuildPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("tid")->intValue();
    string banner = param->valueForKey("tflag")->getCString();
    string aaid = param->valueForKey("aaid")->getCString();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->pushAddTower(1, pointIndex,0,aaid,true,banner);
    }
}
void WolrdBannerBuildCancelPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->pushAddTower(0, pointIndex,0,"",true);
    }
}
void WolrdBannerBuildCrachPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    string defAl = "";
    string atkAl = "";
    if (param->objectForKey("defAlliance")) {
        defAl = param->valueForKey("defAlliance")->getCString();
    }
    if (param->objectForKey("atkAlliance")) {
        atkAl = param->valueForKey("atkAlliance")->getCString();
    }
    string msg = _lang_2("115546", defAl.c_str(), atkAl.c_str());
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*59;
    CCCommonUtils::flySystemUpdateHint(time, true,msg,FLY_HINT_SYSTEM);
}
void WorldAllianceAreaCancelPush::handleResponse(cocos2d::CCDictionary* pDict){
    if (!pDict) {
        return;
    }
    auto param = _dict(pDict->objectForKey("params"));
    if (!param) {
        return;
    }
    int pointIndex = param->valueForKey("pointId")->intValue();
    int range = param->valueForKey("range")->intValue();
    if (WorldMapView::instance()) {
        WorldMapView::instance()->resetAAreaInfo(pointIndex, range);
        WorldMapView::instance()->doAllianceArea(0, pointIndex);
    }
}


void WorldTeleportPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("i") || !params->objectForKey("t") || !params->objectForKey("o"))
        return;
    
    // type :
    // 0 : out
    // 1 : in
    unsigned int type = params->valueForKey("t")->intValue();
    unsigned int index = params->valueForKey("i")->intValue();
    string name = params->valueForKey("o")->getCString();
    if (type == 1) {
        double protectStamp = params->valueForKey("time")->doubleValue();
        unsigned int level = params->valueForKey("l")->intValue();
        if(params->objectForKey("srcServer")){
            unsigned int srcServer = params->valueForKey("srcServer")->intValue();
            auto it = WorldController::getInstance()->getCityInfos().find(index);
            if (it != WorldController::getInstance()->getCityInfos().end()) {
                WorldController::getInstance()->getCityInfos()[index].srcServer = srcServer;
            }
        }
        auto &self = GlobalData::shared()->playerInfo;
//        WorldCityCountry playerCoutry = (WorldCityCountry)params->valueForKey("oc")->intValue();
        auto &player = WorldController::getInstance()->m_playerInfo[name];
//        player.playerCountry = playerCoutry;
        player.protectStamp = protectStamp;
        player.fireStamp = 0;
        player.cityIndex = index;
        player.cityLv = level;
        
        if (self.name == name) {
            player.type = PlayerSelf;
            // self fire cancel , and recover city defense
            GlobalData::shared()->cityDefenseVal = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + CCCommonUtils::getEffectValueByNum(125);
            GlobalData::shared()->cityFireStamp = 0;
            GlobalData::shared()->cityLastUpdateDefenseStamp = 0;
            GlobalData::shared()->cityStartFireStamp = 0;
            CCPoint pos = WorldController::getInstance()->selfPoint;
            int tempIndex = WorldController::getInstance()->getIndexByPoint(pos);
            if (tempIndex!=index) {//被打飞了
                WorldController::getInstance()->selfPoint = WorldController::getInstance()->getPointByIndex(index);
            }
            if (WorldMapView::instance()) {
                WorldMapView::instance()->selfViewPoint = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getInstance()->selfPoint);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_CITY_FIRE);
//            GlobalData::shared()->cityTileCountry = WorldController::getInstance()->getCityInfos()[index].cityCountry;
            auto pt = WorldController::getPointByIndex(index);
            int dx = abs(pt.x - _relic_center_x);
            int radiusX = _relic_tile_width / 2;
            int dy = abs(pt.y - _relic_center_y);
            int radiusY = _relic_tile_height / 2;
            
            if(dx <= radiusX && dy <= radiusY){
                GlobalData::shared()->cityTileCountry = NEUTRALLY;
            }else{
                GlobalData::shared()->cityTileCountry = SPACE;
            }
        } else {
            player.type = PlayerOther;
        }
//        // todo : add alliance
//        else if (self.isInAlliance() && self.getAllianceId() == ) {
//            
//        }
    }
    if (WorldMapView::instance()) {
        WorldMapView::instance()->doTeleport(type, index, name);
    }
}

void WorldCrashPush::handleResponse(cocos2d::CCDictionary *dict)
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(CityCrashView::create());
}

void WorldRefreshPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (WorldMapView::instance()) {
        WorldMapView::instance()->m_map->updateDynamicMap();
    }
}

void WorldRelicUpdatePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("id"))
        return;
    unsigned int point = params->valueForKey("id")->intValue();
    double refresh = params->valueForKey("rt")->doubleValue();
    auto &info = WorldController::getInstance()->getCityInfos();
    auto it = info.find(point);
    if (it != info.end()) {
        info[point].battle.refreshStamp = refresh;
    }
}

void WorldRelicFinishPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (WorldMapView::instance()) {
        WorldMapView::instance()->m_map->updateDynamicMap();
    }
}

void WorldTowerPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("uuid"))
        return;
    string marchId = params->valueForKey("uuid")->getCString();
    auto &marchInfo = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfo.find(marchId);
    if (it != marchInfo.end()) {
        
        map<string,pair<double, double> > tmpEffect;
        if (params->objectForKey("es")) {
            auto esInfo = _dict(params->objectForKey("es"));
            CCDictElement* dictObj;
            CCDICT_FOREACH(esInfo, dictObj) {
                string id = dictObj->getStrKey();
                auto esTime = _dict(dictObj->getObject());
                double st = esTime->valueForKey("st")->intValue();
                double et = esTime->valueForKey("et")->intValue();
                it->second.effect[id] = pair<double, double>(st,et);
            }
        }
    }
}

void WorldProtectUpdatePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("i"))
        return;
    auto cityIndex = params->valueForKey("i")->intValue();
    auto &citys = WorldController::getInstance()->getCityInfos();
    auto cityIt = citys.find(cityIndex);
    if (cityIt == citys.end()) {
        return;
    }
    auto &players = WorldController::getInstance()->m_playerInfo;
    auto playerIt = players.find(cityIt->second.playerName);
    if (playerIt == players.end()) {
        return;
    }
    
    double protectTime = params->valueForKey("pt")->intValue() * 1000.0;
    playerIt->second.protectStamp = protectTime;
    auto world = WorldMapView::instance();
    if (world) {
        world->createCity(cityIt->second);
    }
}

void WorldResourceProtectUpdatePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    auto params= _dict(dict->objectForKey("params"));
    if (!params->objectForKey("i"))
        return;
    auto cityIndex = params->valueForKey("i")->intValue();
    auto &citys = WorldController::getInstance()->getCityInfos();
    auto cityIt = citys.find(cityIndex);
    if (cityIt == citys.end()) {
        return;
    }
    auto &players = WorldController::getInstance()->m_playerInfo;
    auto playerIt = players.find(cityIt->second.playerName);
    if (playerIt == players.end()) {
        return;
    }
    
    double protectTime = params->valueForKey("pt_resource")->intValue() * 1000.0;
    playerIt->second.resourceProtectStamp = protectTime;
    auto world = WorldMapView::instance();
    if (world) {
        world->createCity(cityIt->second);
    }
}

void WorldAddItemPush::handleResponse(cocos2d::CCDictionary *dict){
    if(!dict)
        return;
    WorldMapView::instance()->m_map->updateDynamicMap();
}