//
//  TerritoryWarehouseTile.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/7/7.
//
//

#include "TerritoryWarehouseTile.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "AllianceInfoView.h"
#include "PopupViewController.h"
#include "CheckAllianceInfoView.h"
#include "TipsWithPicView.h"
#include "GetAllianceListCommand.h"
#include "TerritoryInformationView.h"
#include "TerritoryTowerTipsView.h"
#include "TerritoryWarehouseView.h"
#include "TerritoryWarehouseDetailView.h"
#include "TerritoryInfoDetailView.h"
bool TerritoryWarehouseTile::init() {
    NewBaseTileInfo::init();
    
    m_resOnTheWay = false;
    dataBack = false;
    requestDetail();
    onDetailCallback(NULL);
    m_nameNode->removeAllChildren();
    auto bg = CCLoadSprite::createSprite("name_bg.png");
    m_nameNode->addChild(bg);
    
    updateView();
    m_isInUpdate = false;
    addToParent();
    
    return true;
}

void TerritoryWarehouseTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TerritoryWarehouseTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void TerritoryWarehouseTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.parentCityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryWarehouseTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void TerritoryWarehouseTile::onDetailCallback(cocos2d::CCObject *obj) {
    m_isInUpdate = false;
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    if(!this->getParent()){
        return;
    }
    
    if (info) {
        dataBack = true;
    }
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        if (info->objectForKey("maxSoldier")) {
            soldierMax = info->valueForKey("maxSoldier")->intValue();
        }
        else
            soldierMax = 0;
        
        if (info->objectForKey("alreadySoldier")) {
            soldierAlready = info->valueForKey("alreadySoldier")->intValue();
        }
        else
            soldierAlready = 0;
        
        if (info->objectForKey("territory")) {
            CCDictionary* territory = _dict(info->objectForKey("territory"));
            CCDictionary* owner = _dict(territory->objectForKey("owner"));
            if (owner) {
                cityIt->second.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
                cityIt->second.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
                cityIt->second.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
                cityIt->second.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
                cityIt->second.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
            }
            if (!owner) {
                cityIt->second.m_allianceAreaOwnerInfo.ownerPic = "";
                cityIt->second.m_allianceAreaOwnerInfo.ownerName = "";
                cityIt->second.m_allianceAreaOwnerInfo.allianceAbbr = "";
                cityIt->second.m_allianceAreaOwnerInfo.allianceId = "";
                cityIt->second.m_allianceAreaOwnerInfo.ownerId = "";
                
            }
            if (territory->objectForKey("stat")) {
                cityIt->second.m_warehouseInfo.state = (AAreaState)territory->valueForKey("stat")->intValue();
            }
            if (territory->objectForKey("uid")) {
                cityIt->second.m_warehouseInfo.uid = territory->valueForKey("uid")->getCString();
            }
            if (territory->objectForKey("allianceId")) {
                cityIt->second.m_aArea_id = territory->valueForKey("allianceId")->getCString();
            }
            if (territory->objectForKey("allianceName")) {
                cityIt->second.m_aArea_name = territory->valueForKey("allianceName")->getCString();
            }
            if (territory->objectForKey("allianceAbbr")) {
                cityIt->second.m_aArea_simpleName = territory->valueForKey("allianceAbbr")->getCString();
            }
            if (territory->objectForKey("buildingSpd")) {
                cityIt->second.m_warehouseInfo.buildSpeed = territory->valueForKey("buildingSpd")->doubleValue() * 1000;
            }
            if (territory->objectForKey("updateTime")) {
                cityIt->second.m_warehouseInfo.updateTime = territory->valueForKey("updateTime")->doubleValue() / 1000;
            }
            if (territory->objectForKey("startTime")) {
                cityIt->second.m_warehouseInfo.startTime = territory->valueForKey("startTime")->doubleValue() / 1000;
            }
            if (territory->objectForKey("curDef")) {
                cityIt->second.m_warehouseInfo.defence = territory->valueForKey("curDef")->intValue();
            }
            if (territory->objectForKey("maxDef")) {
                cityIt->second.m_warehouseInfo.defenceMax = territory->valueForKey("maxDef")->intValue();
            }
        }
    }
    
    updateView();
    if (dataBack) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}


void TerritoryWarehouseTile::updateView() {
    this->m_iconNode->removeAllChildren();
    this->m_nameNode->removeAllChildren();
    
    auto sprite = CCLoadSprite::createSprite("territory_def.png");
    CCCommonUtils::setSpriteMaxSize(sprite, 30);
    this->m_iconNode->addChild(sprite);
    
    m_currentState = getState();
    //设置按钮
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1, _lang("115370"));
        setButtonState(1, ButtonFunction);
    }else{
        switch (m_currentState) {
            case TerritoryWarehouseUnfinished: {//未完成
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115332"));
                setButtonState(3, ButtonBuilding);
            }
                break;
            case TerritoryWarehouseBuilding: {//建造中
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(1, _lang("115332"));
                    setButtonState(1, ButtonBuilding);
                }
                else{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115332"));
                    setButtonState(3, ButtonBuilding);
                }
            }
                break;
            case TerritoryWarehouseStoring: {//存储中
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
                setButtonName(3, _lang("115452"));
                setButtonState(3, ButtonStorage);
                setButtonName(2, _lang("115456"));
                setButtonState(2, ButtonRetreat);
                if (GlobalData::shared()->territory_warehouse_totalStorage == 0) {
                    setButtonEnable(2);
                }
            }
                break;
            case TerritoryWarehouseOther:{
                buttonCount = 1;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115370"));
                setButtonState(1, ButtonFunction);
            }
                break;
            default: {
                buttonCount = 1;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115370"));
                setButtonState(1, ButtonFunction);
            }
                break;
        }
    }
    
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(TerritoryWarehouseTile::onClickButton));
    }
}

void TerritoryWarehouseTile::update(float sec) {
    if (m_isInUpdate) {
        return;
    }
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    if(!this->getParent()){
        return;
    }
    
    bool flag = false;
    if (WorldController::getInstance()->forceUpdateTerritoryWarehouseTile == true) {
        flag = true;
        WorldController::getInstance()->forceUpdateTerritoryWarehouseTile = false;
    }
    
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        double now = GlobalData::shared()->getTimeStamp();
        //设置超级矿当前量
        int stat = cityIt->second.m_warehouseInfo.state;
        switch (stat) {
            case 0:{
                double timePass = now - cityIt->second.m_warehouseInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_warehouseInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_warehouseInfo.defence + defTemp;
                if (currentDef>=cityIt->second.m_warehouseInfo.defenceMax) {
                    currentDef = cityIt->second.m_warehouseInfo.defenceMax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 1:{
                auto currentDef = cityIt->second.m_warehouseInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
            }
                break;
            case 3:{
                auto currentDef = cityIt->second.m_warehouseInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
            }
                break;
            default:
                break;
        }
    }
    if(flag){
        updateView();
    }
}

bool TerritoryWarehouseTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

TerritoryWarehouseTileState TerritoryWarehouseTile::getState() {
    int type = -1;
    TerritoryWarehouseTileState m_tempState;
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        type = cityIt->second.m_warehouseInfo.state;
        if (cityIt->second.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (type == 0) {
                m_tempState = TerritoryWarehouseBuilding;
            }
            else if (type == 1) {
                m_tempState = TerritoryWarehouseUnfinished;
            }
            else if (type == 3) {
                m_tempState = TerritoryWarehouseStoring;
            }
            else
                m_tempState = TerritoryWarehouseNone;
        }
        else
            m_tempState = TerritoryWarehouseOther;
    }
    return m_tempState;
}

SEL_CCControlHandler TerritoryWarehouseTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* TerritoryWarehouseTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.parentCityIndex,m_title->getString());
}

void TerritoryWarehouseTile::onClickButton(CCObject * pSender, CCControlEvent pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonStorage:{
            map<int, QueueInfo>::iterator it;
            for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
                if((it->second).type == TYPE_MARCH) {
                    std::string uuid = it->second.uuid;
                    auto marchIt = WorldController::getInstance()->m_marchInfo.find(uuid);
                    if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
                        if (marchIt->second.ownerType == PlayerSelf && marchIt->second.marchType == MethodWarehouse) {
                            m_resOnTheWay = true;
                            break;
                        }
                    }
                }
            }
            if (m_resOnTheWay) {
                CCCommonUtils::flyHint("", "", _lang("115522"), 3, 0, true);
            }
            else
                PopupViewController::getInstance()->addPopupInView(TerritoryWarehouseView::create(m_cityInfo.parentCityIndex, true));
        }
            break;
        case ButtonRetreat:{
            if (GlobalData::shared()->territory_warehouse_totalStorage > 0) {
                PopupViewController::getInstance()->addPopupInView(TerritoryWarehouseView::create(m_cityInfo.parentCityIndex, false));
            }
            else
                CCCommonUtils::flyHint("", "", _lang("115478"), 3, 0, true);
        }
            break;
        case ButtonBuilding:{
            if (soldierAlready >= soldierMax && soldierMax != 0) {
                CCCommonUtils::flyHint("", "", _lang("115403"), 3, 0, true);
            }
            else {
                if (dataBack) {
                    WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
                }
            }
        }
            break;
        case ButtonInformation: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
            if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
                if (cityIt->second.m_warehouseInfo.state == 0 || cityIt->second.m_warehouseInfo.state == 1) {
                    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
                    PopupViewController::getInstance()->addPopupInView(TerritoryInfoDetailView::create(WorldController::getInstance()->getCityInfos()[index]));
                }
                else if (cityIt->second.m_warehouseInfo.state == 3) {
                    PopupViewController::getInstance()->addPopupInView(TerritoryWarehouseDetailView::create(WorldController::getInstance()->getCityInfos()[index]));
                }
            }
        }
            break;
        case ButtonFunction: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
            if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
                if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
                    PopupViewController::getInstance()->addPopupView(TerritoryTowerTipsView::create(cityIt->second, true));
                }
                else {
                    PopupViewController::getInstance()->addPopupView(TerritoryTowerTipsView::create(cityIt->second, false));
                }
            }
        }
            break;
        case ButtonGoHome: {
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            auto it = selfMarch.find(m_cityInfo.parentCityIndex);
            if (it == selfMarch.end()) {
                // todo : show error
                return;
            }
            string uuid = it->second;
            
            if (uuid.empty()) {
                // todo : show error
                return;
            }
            
            auto dict = CCDictionary::create();
            dict->setObject(CCString::create(uuid), "marchId");
            WorldMapView::instance()->afterMarchCancel(dict);
            
            vector<EnemyInfo>::iterator enemyIt = EnemyInfoController::getInstance()->m_enemyInfos.begin();
            for(auto &enemyInfo : EnemyInfoController::getInstance()->m_enemyInfos)
            {
                for(auto &marchInfo : WorldController::getInstance()->m_marchInfo)
                {
                    if((enemyInfo.type == ENEMY_TYPE_BATTLE || enemyInfo.type == ENEMY_TYPE_TEAM) && enemyInfo.uuid == marchInfo.second.uuid)
                    {
                        if(marchInfo.second.endPointIndex == m_cityInfo.parentCityIndex)
                        {
                            // 停止闪红
                            //                                enemyInfo.type = ENEMY_TYPE_TARGET_RETREAT;
                            EnemyInfoController::getInstance()->m_enemyInfos.erase(enemyIt);
                            break;
                        }
                    }
                }
                enemyIt++;
            }
        }
            break;
            
        default:
            break;
    }
    
    closeThis();
}