//
//  WorldCommand.cpp
//  IF
//
//  Created by 邹 程 on 14-2-18.
//
//

#include "WorldCommand.h"
#include "WorldController.h"
#include "ToolController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "LogoutCommand.h"
#include "NetController.h"
#include "FlyHint.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "AllianceManager.h"
#include "FBUtilies.h"
#include "EnemyInfoController.h"
#include "MailController.h"
#include "YesNoDialog.h"
#include "TitanController.h"
#include "PopupViewController.h"
#include "DynamicTiledMap.h"

bool WorldResourceUpdateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_RESOURCE_UPDATE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params || !params->objectForKey("uuid")) {
        return false;
    }
    string uuid = params->valueForKey("uuid")->getCString();
    if (uuid != m_uuid) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        TimeUpMarchProcesser::getInstance()->removeTimeUpMarch(m_uuid);
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}


bool WorldMarchDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MARCH_DETAIL_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params || !params->objectForKey("point") || !params->objectForKey("army")) {
        return false;
    }
    
    unsigned int point = params->valueForKey("point")->intValue();
    
    if (point != m_index) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params->objectForKey("army")));
    }
    
    return true;
}

bool WorldMarchDetailUpdateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MARCH_DETAIL_UPDATE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params || !params->objectForKey("uuid")) {
        return false;
    }
    
    string uuid = params->valueForKey("uuid")->getCString();
    
    if (uuid != m_uuid) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}


bool WorldCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params || !params->objectForKey("timeStamp") || !params->objectForKey("points")) {
        WorldController::getInstance()->setisAsyEnd(true);
        WorldMapView::instance()->onSendHelpCmd();
        return false;
    }
    CCString* tempStr = dynamic_cast<CCString*>(params->objectForKey("points"));
    CCArray* tempArr = dynamic_cast<CCArray*>(params->objectForKey("points"));
    
    if (!tempStr && (!tempArr/* || (tempArr && tempArr->count() == 0)*/)) {
        return false;
    }
    
    auto timeStamp = params->valueForKey("timeStamp")->intValue();
    auto x = params->valueForKey("x")->intValue();
    auto y = params->valueForKey("y")->intValue();
    if (timeStamp != m_timeStamp || x != (int)m_point.x || y != (int)m_point.y) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        WorldController::getInstance()->setisAsyEnd(true);
        WorldMapView::instance()->onSendHelpCmd();
        callFail(NetResult::createWithFail(params));
    } else {
        params->setObject(CCString::create(CC_ITOA(m_type)), "t");
        if(WorldMapView::instance()){
            WorldMapView::instance()->callBackMapInfo(params);
        }
    }
        
    return true;
}


bool WorldLeaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_LEAVE_COMMAND) != 0)
        return false;
    
    return true;
}

bool WorldDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_DETAIL_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params->objectForKey("point")) {
        return false;
    }
    
    int point = params->valueForKey("point")->intValue();
    
    if (point != m_point) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    if(params->objectForKey("erp")){
        WorldController::getInstance()->m_monsterAttackInfo->removeAllObjects();
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("erp"));
        if(arr){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj){
                WorldController::getInstance()->addMonsetResearchInfo(dynamic_cast<CCDictionary*>(obj), false);
            }
        }
        WorldController::getInstance()->monsterAttEndTime = params->valueForKey("et")->doubleValue() / 1000;
        WorldController::getInstance()->monsterAttStartTime = params->valueForKey("st")->doubleValue() / 1000;
        WorldController::getInstance()->isInfoTake = true;
    }
    
    if (params->objectForKey("mmhp") && params->objectForKey("mchp")) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_DEFAIL_VIEW, params);
    }
    
    callSuccess(NetResult::createWithSuccess(params));
    
    return true;
}

bool WorldBuyPVECommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_BUY_PVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params->objectForKey("point")) {
        return false;
    }
    
    int point = params->valueForKey("point")->intValue();
    
    if (point != m_point) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}

bool WorldMarchCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_MARCH_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        std::string monsterFailure = "103730";
        std::string yuanjunFailure = "115166";
        std::string bossFailure = "137469";
        std::string yuanjunBossFailure = "137470";
        std::string ziyuanFailure = "108793";
        if(pStr->getCString() == monsterFailure){
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                CCCommonUtils::flyHint("", "", _lang("103730"));
                WorldMapView::instance()->m_map->updateDynamicMap(ccp(-1, -1));
                if (usePower > 0) {
                    WorldController::getInstance()->currentStamine += usePower;
                }else {
                    WorldController::getInstance()->currentStamine += GlobalData::shared()->worldConfig.stamineCostPerTime;
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            }
        }else if(pStr->getCString() == yuanjunFailure){
            int max = params->valueForKey("max")->intValue();
            CCCommonUtils::flyText(_lang_1(pStr->getCString(),CC_ITOA(max)));
        }
        else if(pStr->getCString() == bossFailure || pStr->getCString() == yuanjunBossFailure)
        {
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                WorldMapView::instance()->m_map->updateDynamicMap(ccp(-1, -1));
                WorldController::getInstance()->currentStamine += GlobalData::shared()->worldConfig.boss_decr;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            }
            CCCommonUtils::flyHint("", "", _lang( pStr->getCString() ));
        }
        else if (pStr->getCString() == ziyuanFailure){
//            CCCommonUtils::flyText(_lang(pStr->getCString()));
            YesNoDialog::showVariableTitle(_lang("108793").c_str(),CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)),_lang("confirm").c_str());
        }
        else{
            if(Error::TITAN_NO_ENOUGH_TILI ==(Error) pStr->intValue())
            {
                TitanController::getInstance()->handleErrorCode(pStr->intValue());
            }
            else
            {
                PopupViewController::getInstance()->removeAllPopupView();//fusheng 出错了但不是泰坦体力不足 removeAllPopupView
                CCCommonUtils::flyText(_lang(pStr->getCString()));
                
            }
            
        }
    }else{
        if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD && m_type!=MethodRally){
           
            PopupViewController::getInstance()->removeAllPopupView();  //fusheng 满足条件 关闭弹窗
        }
        
        
        if(m_type == MethodDeal){
            if(params->objectForKey("gold")){
                GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            }
            GlobalData::shared()->resourceInfo.setResourceData(params);
        }
        if (m_type == MethodWarehouse) {
            if(params->objectForKey("gold")){
                GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
            }
            GlobalData::shared()->resourceInfo.setResourceData(params);
        }
        if(m_type == MethodRally){
            callSuccess(NetResult::create(Error_OK, params));
        }else{
            callSuccess(NetResult::create());
        }
    }
    return true;
}

bool WorldMarchSpeedUpCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_MARCH_SPEED_UP_COMMAND) != 0)
        return false;
    
    return true;
}

bool WorldMarchCancelCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_MARCH_CANCEL_COMMAND) != 0)
        return false;
    CCDictionary *params = _dict(dict->objectForKey("params"));
    callSuccess(NetResult::createWithSuccess(params));
    return true;
}

bool WorldAddFavoriteCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_ADD_FAVORITE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params->objectForKey("point") || params->valueForKey("point")->intValue() != m_point) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        auto dict = CCDictionary::create();
        dict->setObject(CCInteger::create(m_point), "point");
        dict->setObject(CCString::create(m_name), "name");
        dict->setObject(CCInteger::create(m_type), "type");
        dict->setObject(CCString::create(CC_ITOA(m_serverId)), "server");
        long createTime = params->valueForKey("createTime")->longValue();
        dict->setObject(CCString::create(CC_ITOA(createTime)), "createTime");
//        callSuccess(NetResult::createWithSuccess(dict));
        if(WorldMapView::instance()){
            WorldMapView::instance()->onAddFavoriteCallback(dict);
        }
        CCCommonUtils::flyHint("", "", _lang("108871"));
        UIComponent::getInstance()->setFavoriteIconVisible(true);
    }
    
    return true;
}

bool WorldDelFavoriteCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_DEL_FAVORITE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params->objectForKey("point") || params->valueForKey("point")->intValue() != m_point) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        double key = WorldController::getInstance()->getFavoriteKey(m_point, m_server);
        WorldController::getInstance()->m_favoriteTiles.erase(key);
        callSuccess(NetResult::createWithSuccess(dict));
    }
    
    return true;
}

bool WorldGetFavoriteCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_GET_FAVORITE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params || !params->objectForKey("favo")) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params->objectForKey("favo")));
    }
    
    return true;
}



bool WorldScoutDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_SCOUT_DETAIL_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params->objectForKey("point") || params->valueForKey("point")->intValue() != m_point) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}

void WorldCityMoveCommand::addMailUid(string mailUid) {
    m_mailUid = mailUid;
    if (m_mailUid != "") {
        putParam("mailId", CCString::create(mailUid));
    }
}

void WorldCityMoveCommand::setAlMovType(int type)
{
    m_alMoveType = type;
    putParam("alMove", CCInteger::create(1));
}

bool WorldCityMoveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_CITY_MOVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));

        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        if (!params->objectForKey("point") || (params->valueForKey("point")->intValue() != m_point && m_point != 0)) {
            return false;
        }

        if (m_alMoveType > 0) {
            auto& info = ToolController::getInstance()->getToolInfoById(ITEM_ALLIANCE_CITY_MOVE);
            int toolCnt = info.getCNT() - 1;
            info.setCNT(toolCnt);
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(ITEM_ALLIANCE_CITY_MOVE)), _lang("104908"), _lang_1("104909", info.getName().c_str()));
        }
        else if(m_point == 0){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_RANDOM);
            int toolCount = info.getCNT() - 1;
            info.setCNT(toolCount);
        } else {
            auto &rewardTool = ToolController::getInstance()->getToolInfoById(ITEM_FREE_MOVE_CITY);//获取新手迁城道具
            if (rewardTool.getCNT()>0) {
                ToolController::getInstance()->useTool(ITEM_FREE_MOVE_CITY);
            }else{
                auto &tool = ToolController::getInstance()->getToolInfoById(ITEM_MOVE_CITY);//获取迁城道具
                if (tool.getCNT()>0) {
                    ToolController::getInstance()->useTool(ITEM_MOVE_CITY);
                }
            }

        }
        if (m_mailUid != "") {
            AllianceManager::getInstance()->endTeleport(m_mailUid);
        }
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_secret);
        EnemyInfoController::getInstance()->m_enemyInfos.clear();
        
        unsigned int index = params->valueForKey("point")->intValue();
        auto selfPoint = WorldController::getPointByIndex(index);
        WorldController::getInstance()->selfPoint = selfPoint;

        if(WorldMapView::instance()){
            WorldMapView::instance()->onCityMoveCallback(params->objectForKey("point"));
        }
//        callSuccess(NetResult::createWithSuccess(params->objectForKey("point")));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
    }
    
    return true;
}

bool WorldAddItemCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(WORLD_ADD_ITEM_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("pointId")));
    } else {
        if (!params->objectForKey("pointId") || (params->valueForKey("pointId")->intValue() != m_point && m_point != 0)) {
            return false;
        }
//        bool refreshAll = false;
        if(params->objectForKey("itemid")){
            int toolid = params->valueForKey("itemid")->intValue();
            auto &tool = ToolController::getInstance()->getToolInfoById(toolid);//获取迁城道具
            if (tool.getCNT()>0) {
                ToolController::getInstance()->useTool(toolid);
            }
//            if(tool.type == 12){
//                refreshAll=true;
//            }
        }
        
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_secret);
        EnemyInfoController::getInstance()->m_enemyInfos.clear();
        
//        unsigned int index = params->valueForKey("pointId")->intValue();
//        auto selfPoint = WorldController::getPointByIndex(index);
//        WorldController::getInstance()->selfPoint = selfPoint;
        
//        if(WorldMapView::instance()){
//            WorldMapView::instance()->onCityMoveCallback(params->objectForKey("pointId"));
//        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
        
//        if(refreshAll==false){
            WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getInstance()->getPointByIndex(m_point));
//        }else{
//             WorldMapView::instance()->m_map->updateDynamicMap();
//        }
        
    }
    
    return true;
}

bool WorldAllianceAreaCreateCommand::handleRecieve(cocos2d::CCDictionary *dict){

    if (dict->valueForKey("cmd")->compare(WORLD_AAREA_CREATE) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        if(params->objectForKey("remainGold")) {
            int tmpInt = params->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
    }
    return true;
}

bool WorldAllianceAreaCancelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    if (dict->valueForKey("cmd")->compare(WORLD_AAREA_CANCEL) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("pointId")));
    } else {
        if (!params->objectForKey("pointId") || (params->valueForKey("pointId")->intValue() != m_point && m_point != 0)) {
            return false;
        }
    }
    return true;
}

bool WorldTowerCreateCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TOWER_CREATE)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldTowerCancelCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TOWER_CANCEL)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};

bool WorldTerritoryResCreateCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_RES_CREATE)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldTerritoryResCancelCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_RES_CANCEL)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldTerritoryWarehouseCreateCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_WAREHOUSE_CREATE)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldTerritoryWarehouseCancelCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_WAREHOUSE_CANCEL)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldTerritoryBannerCreateCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_BANNER_CREATE)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        if(params->objectForKey("remainGold")) {
            int tmpInt = params->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
    }
    return true;
};
bool WorldTerritoryBannerCancelCommand::handleRecieve(cocos2d::CCDictionary* pDic){
    if (pDic->valueForKey("cmd")->compare(WORLD_TERRITORY_BANNER_CANCEL)  != 0) {
        return false;
    }
    CCDictionary *params = _dict(pDic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        
    }
    return true;
};
bool WorldCrossCityMoveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_CROSS_MOVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));

        if(WorldMapView::instance()){
            WorldMapView::instance()->m_touchDelegateView->notMove = false;
            WorldMapView::instance()->isCrossServer = false;
        }
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        MailController::getInstance()->quitAllChatRoom();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_ALL_HINT);
        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->release();
            SceneController::getInstance()->showBG = NULL;
        }
        WorldMapView::instance()->removeCover();
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        auto texture = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
        texture->beginWithClear(0, 0, 0, 1.0);
        scene->visit();
        texture->end();
        SceneController::getInstance()->showBG = texture;
        SceneController::getInstance()->showBG->retain();
        
        auto serverInfo = _dict(params->objectForKey("serverInfo"));
        std::string ip = serverInfo->valueForKey("ip")->getCString();
        std::string zone = serverInfo->valueForKey("zone")->getCString();
        int port = serverInfo->valueForKey("port")->intValue();
        std::string gameUid = params->valueForKey("uid")->getCString();
        
        if(FBUtilies::fbIsLogin()){//如果fb是登录状态的token 要把它清掉
            FBUtilies::fbLogout();
        }
        
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, port);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
        CCUserDefault::sharedUserDefault()->flush();
        
        NetController::shared()->setIp(ip);
        NetController::shared()->setZone(zone);
        NetController::shared()->setPort(port);
        NetController::shared()->setIsLogout(true);
        NetController::shared()->disconnect();
        
        LogoutCommand* cmd = new LogoutCommand();
        CCDictionary* _params = CCDictionary::create();
        _params->setObject(CCString::create(zone), "zoneName");
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create("logout"), "cmd");
        dict->setObject(_params, "params");
        cmd->handleRecieve(dict);
        cmd->release();
//        this->retain();
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WorldCrossCityMoveCommand::onNext), this, 0.0f,0, 0.02f, false);
    }
    return true;
}

bool WorldMarchTimeUpCheckCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MARCH_CHECK_COMMAND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        TimeUpMarchProcesser::getInstance()->requestDataBack(NULL);
    }else{
        TimeUpMarchProcesser::getInstance()->requestDataBack(params);
    }
    return true;
}

void WorldCityMoveGoldCommand::addMailUid(string mailUid) {
    m_mailUid = mailUid;
    if (m_mailUid != "") {
        putParam("mailId", CCString::create(mailUid));
    }
}


bool WorldCityMoveGoldCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_CITY_MOVE_GOLD_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        if(params->objectForKey("gold")) {
            int tmpInt = params->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        callFail(NetResult::create(Error_OP_FAILURE,params->objectForKey("point")));
    } else {
        if (!params->objectForKey("point") || (params->valueForKey("point")->intValue() != m_point && m_point != 0)) {
            return false;
        }
        if (m_mailUid != "") {
            AllianceManager::getInstance()->endTeleport(m_mailUid);
        }
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_secret);
        EnemyInfoController::getInstance()->m_enemyInfos.clear();
        
        unsigned int index = params->valueForKey("point")->intValue();
        auto selfPoint = WorldController::getPointByIndex(index);
        WorldController::getInstance()->selfPoint = selfPoint;

        if(WorldMapView::instance()){
            WorldMapView::instance()->onCityMoveCallback(params->objectForKey("point"));
        }
        if(params->objectForKey("remainGold")) {
            int tmpInt = params->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
        PopupViewController::getInstance()->removeAllPopupView();
    }
    
    return true;
}


bool WorldTreasureMapGetCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(WORLD_TREASUREMAP_GET_COMMAND) != 0)
        return false;
    CCDictionary *params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
    } else {
        if(params->objectForKey("treasureMap")){
            CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("treasureMap"));
            if(arr && arr->count()>0){
                CCObject *ccObj = NULL;
                CCARRAY_FOREACH(arr, ccObj){
                    WorldController::getInstance()->addTreasureInfo(_dict(ccObj));
                }
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.treasure.map.info.change");
        }
    }
    return true;
}
bool WorldTreasureMapDestroyCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(WORLD_TREASUREMAP_DESTROY_COMMAND) != 0)
        return false;
    CCDictionary *params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
    } else {
        if(params->objectForKey("uuid") && params->objectForKey("itemId") && params->objectForKey("count")){
            string itemid = params->valueForKey("itemId")->getCString();
            int count = params->valueForKey("count")->intValue();
            auto iter = ToolController::getInstance()->m_toolInfos.find(atoi(itemid.c_str()));
            if(iter!=ToolController::getInstance()->m_toolInfos.end()){
                (*iter).second.setCNT(count);
            }
        }
        if(params->objectForKey("treasureuuid")){
            WorldController::getInstance()->removeTreasureInfo(params->valueForKey("treasureuuid")->getCString());
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.treasure.map.info.change");
        }
    }
    return true;
}
bool WorldTreasureMapSpeedupCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(WORLD_TREASUREMAP_SPD_COMMAND) != 0)
        return false;
    CCDictionary *params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        const CCString *pStr = params->valueForKey("errorCode");
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
    } else {
//        if(params->objectForKey("marchid")){
//            string marchID = params->valueForKey("marchid")->getCString();
            auto iter = WorldController::getInstance()->m_marchInfo.find(mmarchid);
            if(iter!=WorldController::getInstance()->m_marchInfo.end()){
                MarchInfo& marchInfo = (*iter).second;
                marchInfo.treasureMapTime = 0;
                marchInfo.treasureMapFinishTime = 0;
//                //TODO 去重新create city
//                auto iter1 = WorldController::getInstance()->getCityInfos().find((*iter).second.endPointIndex);
//                if(iter1!=WorldController::getInstance()->getCityInfos().end()){
//                    (*iter1).second.cityType = OriginTile;
//                    WorldMapView::instance()->createCity((*iter1).second);
//                }
            }
//        }
        if(params->objectForKey("gold")) {
            int tmpInt = params->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
    }
    return true;
}

