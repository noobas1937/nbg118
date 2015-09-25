//
//  TerritoryTowerTile.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/6/8.
//
//

#include "TerritoryTowerTile.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "PopupViewController.h"
#include "TerritoryInformationView.h"
#include "TerritoryTowerTipsView.h"
#include "TerritoryInfoDetailView.h"
bool TerritoryTowerTile::init() {
    NewBaseTileInfo::init();
    
    soldierAlready = 0;
    soldierMax = 0;
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

void TerritoryTowerTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TerritoryTowerTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void TerritoryTowerTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.cityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryTowerTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void TerritoryTowerTile::onDetailCallback(cocos2d::CCObject *obj) {
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
            m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
            m_cityInfo.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
            m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
            m_cityInfo.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
            m_cityInfo.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
        }
        if (!owner) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = "";
            m_cityInfo.m_allianceAreaOwnerInfo.ownerName = "";
            m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = "";
            m_cityInfo.m_allianceAreaOwnerInfo.allianceId = "";
            m_cityInfo.m_allianceAreaOwnerInfo.ownerId = "";
        }
        if (territory->objectForKey("stat")) {
            m_cityInfo.m_towerInfo.state = (AAreaState)territory->valueForKey("stat")->intValue();
        }
        if (territory->objectForKey("uid")) {
            m_cityInfo.m_towerInfo.uid = territory->valueForKey("uid")->getCString();
        }
        if (territory->objectForKey("allianceId")) {
            m_cityInfo.m_aArea_id = territory->valueForKey("allianceId")->getCString();
        }
        if (territory->objectForKey("allianceName")) {
            m_cityInfo.m_aArea_name = territory->valueForKey("allianceName")->getCString();
        }
        if (territory->objectForKey("allianceAbbr")) {
            m_cityInfo.m_aArea_simpleName = territory->valueForKey("allianceAbbr")->getCString();
        }
        if (territory->objectForKey("buildingSpd")) {
            m_cityInfo.m_towerInfo.buildSpeed = territory->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
        if (territory->objectForKey("updateTime")) {
            m_cityInfo.m_towerInfo.updateTime = territory->valueForKey("updateTime")->doubleValue() / 1000;
        }
        if (territory->objectForKey("startTime")) {
            m_cityInfo.m_towerInfo.startTime = territory->valueForKey("startTime")->doubleValue() / 1000;
        }
        if (territory->objectForKey("curDef")) {
            m_cityInfo.m_towerInfo.defence = territory->valueForKey("curDef")->intValue();
        }
        if (territory->objectForKey("maxDef")) {
            m_cityInfo.m_towerInfo.defenceMax = territory->valueForKey("maxDef")->intValue();
        }
        if (territory->objectForKey("name")) {
            m_cityInfo.m_aArea_nickname = territory->valueForKey("name")->getCString();
        }
        if (!territory->objectForKey("name")) {
            if (territory->valueForKey("count")) {
                int num = territory->valueForKey("count")->intValue();
                m_cityInfo.m_aArea_nickname = _lang_1("115406", CC_ITOA(num));
            }
        }
    }
    
    updateView();
    if (dataBack) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}


void TerritoryTowerTile::updateView() {
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
            case TerritoryTowerUnfinished: {//未完成
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115332"));
                setButtonState(3, ButtonBuilding);
            }
                break;
            case TerritoryTowerBuilding: {//建造中
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.cityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(1, _lang("115332"));
                    setButtonState(1, ButtonBuilding);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(1);
                    }
                }
                else{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115332"));
                    setButtonState(3, ButtonBuilding);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryTowerFinished: {
                buttonCount = 1;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
            }
                break;
            case TerritoryTowerOther:{
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
        setButtonCallback(i, cccontrol_selector(TerritoryTowerTile::onClickButton));
    }
}

void TerritoryTowerTile::update(float sec) {
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
    if (WorldController::getInstance()->forceUpdateTerritoryTowerTile == true) {
        flag = true;
        WorldController::getInstance()->forceUpdateTerritoryTowerTile = false;
    }
    
    double now = GlobalData::shared()->getTimeStamp();
    //设置超级矿当前量
    int stat = m_cityInfo.m_towerInfo.state;
    switch (stat) {
        case 0:{
            double timePass = now - m_cityInfo.m_towerInfo.startTime;
            timePass = MAX(timePass, 0.0f);
            double defTemp = m_cityInfo.m_towerInfo.buildSpeed * timePass;
            auto currentDef = m_cityInfo.m_towerInfo.defence + defTemp;
            if (currentDef>=m_cityInfo.m_towerInfo.defenceMax) {
                currentDef = m_cityInfo.m_towerInfo.defenceMax;
            }
            m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
            m_title->setColor({0, 190, 13});
        }
            break;
        case 1:{
            auto currentDef = m_cityInfo.m_towerInfo.defence;
            m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
            m_title->setColor({220, 176, 0});
        }
            break;
        case 3:{
            auto currentDef = m_cityInfo.m_towerInfo.defenceMax;
            m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
        }
            break;
        default:
            break;
    }
    if(flag){
        updateView();
    }
}

bool TerritoryTowerTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

TerritoryTowerTileState TerritoryTowerTile::getState() {
    int type = -1;
    TerritoryTowerTileState m_tempState;
    
    type = m_cityInfo.m_towerInfo.state;
    if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
        if (type == 0) {
            m_tempState = TerritoryTowerBuilding;
        }
        else if (type == 1) {
            m_tempState = TerritoryTowerUnfinished;
        }
        else if (type == 3) {
            m_tempState = TerritoryTowerFinished;
        }
        else
            m_tempState = TerritoryTowerNone;
    }
    else
        m_tempState = TerritoryTowerOther;
    
    return m_tempState;
}

SEL_CCControlHandler TerritoryTowerTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* TerritoryTowerTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,m_title->getString());
}

void TerritoryTowerTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonBuilding:{
            if (soldierAlready >= soldierMax && soldierMax != 0) {
                CCCommonUtils::flyHint("", "", _lang("115403"), 3, 0, true);
            }
            else {
                if (dataBack) {
                    WorldController::getInstance()->openMarchDeploy(m_cityInfo.cityIndex,0);
                }
            }
        }
            break;
        case ButtonInformation: {
            if (m_cityInfo.m_towerInfo.state == 0 || m_cityInfo.m_towerInfo.state == 1) {
                SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
                PopupViewController::getInstance()->addPopupInView(TerritoryInfoDetailView::create(m_cityInfo));
            }
            else if (m_cityInfo.m_towerInfo.state == 3) {
                PopupViewController::getInstance()->addPopupInView(TerritoryInformationView::create(m_cityInfo));
            }
        }
            break;
        case ButtonFunction: {
            if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
                PopupViewController::getInstance()->addPopupView(TerritoryTowerTipsView::create(m_cityInfo, true));
            }
            else
                PopupViewController::getInstance()->addPopupView(TerritoryTowerTipsView::create(m_cityInfo, false));
        }
            break;
        case ButtonGoHome: {
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            auto it = selfMarch.find(m_cityInfo.cityIndex);
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
                        if(marchInfo.second.endPointIndex == m_cityInfo.cityIndex)
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
