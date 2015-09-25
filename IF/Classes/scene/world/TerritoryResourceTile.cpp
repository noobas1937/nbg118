//
//  TerritoryResourceTile.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/15.
//
//

#include "TerritoryResourceTile.h"
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
#include "TerritoryResourceDetailView.h"
#include "GetAllianceListCommand.h"
#include "TerritoryResourceInfoView.h"
#include "FunBuildController.h"
#include "TerritoryTowerTipsView.h"
bool TerritoryResourceTile::init() {
    NewBaseTileInfo::init();
    
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

void TerritoryResourceTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TerritoryResourceTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void TerritoryResourceTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.parentCityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryResourceTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void TerritoryResourceTile::onDetailCallback(cocos2d::CCObject *obj) {
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
                cityIt->second.m_superMineInfo.trstat = (AAreaState)territory->valueForKey("stat")->intValue();
                if (cityIt->second.m_superMineInfo.trstat == 3 || cityIt->second.m_superMineInfo.trstat == 4 || cityIt->second.m_superMineInfo.trstat == 6) {
                    this->m_iconNode->setPositionX(m_iconNode->getPositionX() - 20);
                    this->m_title->setPositionX(m_title->getPositionX() - 20);
                }
            }
            if (territory->objectForKey("uid")) {
                cityIt->second.m_superMineInfo.truid = territory->valueForKey("uid")->getCString();
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
                if (cityIt->second.m_superMineInfo.trstat == 0) {
                    cityIt->second.m_superMineInfo.buildSpeed = territory->valueForKey("buildingSpd")->doubleValue() * 1000;
                }
                else if (cityIt->second.m_superMineInfo.trstat == 6 || cityIt->second.m_superMineInfo.trstat == 4 || cityIt->second.m_superMineInfo.trstat == 3) {
                    cityIt->second.m_superMineInfo.digSpeed = territory->valueForKey("buildingSpd")->doubleValue() * 1000;
                }
            }
            if (territory->objectForKey("updateTime")) {
                cityIt->second.m_superMineInfo.trutime = territory->valueForKey("updateTime")->doubleValue() / 1000;
            }
            if (territory->objectForKey("startTime")) {
                if (cityIt->second.m_superMineInfo.trstat == 0) {
                    cityIt->second.m_superMineInfo.trstime = territory->valueForKey("startTime")->doubleValue() / 1000;
                }
                else if (cityIt->second.m_superMineInfo.trstat == 6 || cityIt->second.m_superMineInfo.trstat == 4 || cityIt->second.m_superMineInfo.trstat == 3) {
                    cityIt->second.m_superMineInfo.digStartTime = territory->valueForKey("startTime")->doubleValue() / 1000;
                }
            }
            if (territory->objectForKey("curDef")) {
                cityIt->second.m_superMineInfo.trdef = territory->valueForKey("curDef")->intValue();
            }
            if (territory->objectForKey("maxDef")) {
                cityIt->second.m_superMineInfo.trdefmax = territory->valueForKey("maxDef")->intValue();
            }
            if (territory->objectForKey("resType")) {
                cityIt->second.m_superMineInfo.type= (WorldResourceType)territory->valueForKey("resType")->intValue();
            }
        }
        if(info->objectForKey("tq")){
            cityIt->second.m_superMineInfo.sum = info->valueForKey("tq")->intValue();
        }
        
        if (info->objectForKey("territory_positive_effect")) {
            GlobalData::shared()->alliance_territory_positive_effect_gather = 1;
        }
        else
            GlobalData::shared()->alliance_territory_positive_effect_gather = 0;

    }
    
    updateView();
    if (dataBack) {
       CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}


void TerritoryResourceTile::updateView() {
    this->m_iconNode->removeAllChildren();
    this->m_nameNode->removeAllChildren();
    
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        if (cityIt->second.m_superMineInfo.trstat == 3 || cityIt->second.m_superMineInfo.trstat == 4 || cityIt->second.m_superMineInfo.trstat == 6) {
            auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(cityIt->second.m_superMineInfo.type).c_str());
            CCCommonUtils::setSpriteMaxSize(sprite, 30);
            this->m_iconNode->addChild(sprite);
        }
        else {
            auto sprite = CCLoadSprite::createSprite("territory_def.png");
            CCCommonUtils::setSpriteMaxSize(sprite, 30);
            this->m_iconNode->addChild(sprite);
        }
    }
    
    m_currentState = getState();
    //设置按钮
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1, _lang("115370"));
        setButtonState(1, ButtonFunction);
    }else{
        switch (m_currentState) {
            case TerritoryResourceUnfinished: {//未完成
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115332"));
                setButtonState(3, ButtonBuilding);
            }
                break;
            case TerritoryResourceBuilding: {//建造中
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
            case TerritoryResourceGathering: {//有人采集
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在采集中，如有则变成返回
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                }
                else {
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115386"));
                    setButtonState(3, ButtonGather);
                }
            }
                break;
            case TerritoryResourceUngathering:{
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115386"));
                setButtonState(3, ButtonGather);
            }
                break;
            case TerritoryResourceOther:{
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
        setButtonCallback(i, cccontrol_selector(TerritoryResourceTile::onClickButton));
    }
}

void TerritoryResourceTile::update(float sec) {
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
    if (WorldController::getInstance()->forceUpdateTerritoryResourceTile == true) {
        flag = true;
        WorldController::getInstance()->forceUpdateTerritoryResourceTile = false;
    }
    
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        double now = GlobalData::shared()->getTimeStamp();
        //设置超级矿当前量
        int stat = cityIt->second.m_superMineInfo.trstat;
        switch (stat) {
            case 0:{
                double timePass = now - cityIt->second.m_superMineInfo.trstime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_superMineInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_superMineInfo.trdef + defTemp;
                if (currentDef>=cityIt->second.m_superMineInfo.trdefmax) {
                    currentDef = cityIt->second.m_superMineInfo.trdefmax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 1:{
                auto currentDef = cityIt->second.m_superMineInfo.trdef;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
            }
                break;
            case 3:
            case 4:{
                auto currentRes = cityIt->second.m_superMineInfo.sum;
                m_title->setString(CCCommonUtils::getResourceStr(currentRes).c_str());
            }
                break;
            case 6:{
                double duration = now - cityIt->second.m_superMineInfo.digStartTime;
                duration = MAX(duration, 0.0f);
                double currentLoad = cityIt->second.m_superMineInfo.digSpeed * duration;
                auto resouceSum = cityIt->second.m_superMineInfo.sum - currentLoad;
                
//                CCLog("territoryresource...time:%s",CC_ITOA(duration));
//                CCLog("territoryresource...load:%d",currentLoad);
                if (cityIt->second.m_superMineInfo.sum - currentLoad <= 0) {
                    currentLoad = cityIt->second.m_superMineInfo.sum;
                }
                m_title->setString(CCString::createWithFormat("%s", CCCommonUtils::getResourceStr(resouceSum).c_str())->getCString());
                m_title->setColor(ccWHITE);
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

bool TerritoryResourceTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

TerritoryResourceTileState TerritoryResourceTile::getState() {
    int type = -1;
    TerritoryResourceTileState m_tempState;
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        type = cityIt->second.m_superMineInfo.trstat;
        if (cityIt->second.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (type == 0) {
                m_tempState = TerritoryResourceBuilding;
            }
            else if (type == 1) {
                m_tempState = TerritoryResourceUnfinished;
            }
            else if (type == 3 || type == 4) {
                m_tempState = TerritoryResourceUngathering;
            }
            else if (type == 6) {
                m_tempState = TerritoryResourceGathering;
            }
            else
                m_tempState = TerritoryResourceNone;
        }
        else
            m_tempState = TerritoryResourceOther;
    }
    return m_tempState;
}

SEL_CCControlHandler TerritoryResourceTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* TerritoryResourceTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.parentCityIndex,m_title->getString());
}

void TerritoryResourceTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonGather:{
            if (dataBack) {
                if(FunBuildController::getInstance()->getMainCityLv() < GlobalData::shared()->alliance_territory_supermine_limit){
                    YesNoDialog::showVariableTitle(_lang_1("115548", CC_ITOA(GlobalData::shared()->alliance_territory_supermine_limit)), CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)), _lang("confirm").c_str());
                }
                else{
                    if(m_cityInfo.m_superMineInfo.type == Stone){
                        if (FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k4) {
                            YesNoDialog::showVariableTitle(_lang_2("108666", CC_ITOA(FunBuildController::getInstance()->building_base_k4), _lang("115379").c_str()), CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)), _lang("confirm").c_str());
                        }
                        else
                            WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
                    }
                    else if(m_cityInfo.m_superMineInfo.type == Iron){
                        if (FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k3) {
                            YesNoDialog::showVariableTitle(_lang_2("108666", CC_ITOA(FunBuildController::getInstance()->building_base_k3), _lang("115378").c_str()), CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)), _lang("confirm").c_str());
                        }
                        else
                            WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
                    }
                    else {
                        WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
                    }
                }
            }
        }
            break;
        case ButtonBuilding:{
            if (dataBack) {
                WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
            }
        }
            break;
        case ButtonInformation: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            PopupViewController::getInstance()->addPopupInView(TerritoryResourceDetailView::create(WorldController::getInstance()->getCityInfos()[index]));
        }
            break;
        case ButtonFunction: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
            if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
                if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {//是本服的
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