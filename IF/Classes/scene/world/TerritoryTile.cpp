//
//  TerritoryTile.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/10.
//
//

#include "TerritoryTile.h"
#include "ResourceTile.h"
#include "ScoutInfo.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "RoleInfoView.h"
#include "TipsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "TerritoryInformationView.h"
#include "TerritoryFunctionView.h"
#include "TerritoryInfoDetailView.h"
#include "TroopInformationView.h"
bool TerritoryTile::init() {
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

void TerritoryTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TerritoryTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void TerritoryTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.parentCityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void TerritoryTile::onDetailCallback(cocos2d::CCObject *obj) {
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
            if (territory) {
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
                    cityIt->second.m_allianceAreaOwnerInfo.ownerMarchUuid = "";
                }
                if (territory->objectForKey("stat")) {
                    cityIt->second.m_allianceAreaInfo.state = (AAreaState)territory->valueForKey("stat")->intValue();
                }
                if (territory->objectForKey("speed")){
                    cityIt->second.m_allianceAreaInfo.buildSpeed = territory->valueForKey("speed")->doubleValue() * 1000;
                }
                if (territory->objectForKey("curDef")) {
                    cityIt->second.m_allianceAreaInfo.defence = territory->valueForKey("curDef")->intValue();
                }
                if (territory->objectForKey("maxDef")) {
                    cityIt->second.m_allianceAreaInfo.defenceMax = territory->valueForKey("maxDef")->intValue();
                }
                if (territory->objectForKey("updateTime")) {
                    cityIt->second.m_allianceAreaInfo.updateTime = territory->valueForKey("updateTime")->doubleValue() / 1000;
                }
                if (territory->objectForKey("startTime")) {
                    cityIt->second.m_allianceAreaInfo.startTime = territory->valueForKey("startTime")->doubleValue() / 1000;
                }
                if (territory->objectForKey("allianceAbbr")) {
                    cityIt->second.m_aArea_simpleName = territory->valueForKey("allianceAbbr")->getCString();
                }
                if (territory->objectForKey("name")) {
                    cityIt->second.m_aArea_nickname = territory->valueForKey("name")->getCString();
                }
                if (territory->objectForKey("allianceId")) {
                    cityIt->second.m_aArea_id = territory->valueForKey("allianceId")->getCString();
                }
                if (!territory->objectForKey("name")) {
                    if (territory->valueForKey("count")) {
                        int num = territory->valueForKey("count")->intValue();
                        cityIt->second.m_aArea_nickname = _lang_1("115312", CC_ITOA(num));
                    }
                }
            }
        }
    }
    
    updateView();
    if (dataBack) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}


void TerritoryTile::updateView() {
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
        setButtonName(1, _lang("115331"));
        setButtonState(1, ButtonInformation);
    }else{
        switch (m_currentState) {
            case TerritoryUnfinished: {
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
                setButtonName(3, _lang("115332"));
                setButtonState(3, ButtonBuilding);
                setButtonName(2, _lang("115370"));
                setButtonState(2, ButtonFunction);
            }
                break;
            case TerritoryBuilding: {//建造中
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 4;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(4, _lang("115332"));
                    setButtonState(4, ButtonBuilding);
                    setButtonName(5, _lang("115370"));
                    setButtonState(5, ButtonFunction);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(4);
                    }
                }
                else{
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(1, _lang("115331"));
                    setButtonState(1, ButtonInformation);
                    setButtonName(3, _lang("115332"));
                    setButtonState(3, ButtonBuilding);
                    setButtonName(2, _lang("115370"));
                    setButtonState(2, ButtonFunction);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryStation: {//驻扎
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 4;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(4, _lang("115333"));
                    setButtonState(4, ButtonStation);
                    setButtonName(5, _lang("115370"));
                    setButtonState(5, ButtonFunction);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(4);
                    }
                }
                else{
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115333"));
                    setButtonState(3, ButtonStation);
                    setButtonName(1, _lang("115370"));
                    setButtonState(1, ButtonFunction);

                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryUnstation: {
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("115331"));
                setButtonState(2, ButtonInformation);
                setButtonName(3, _lang("115333"));
                setButtonState(3, ButtonStation);
                setButtonName(1, _lang("115370"));
                setButtonState(1, ButtonFunction);
            }
                break;
            case TerritoryRepair: {//修复
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 4;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                    setButtonName(4, _lang("115334"));
                    setButtonState(4, ButtonRepair);
                    setButtonName(5, _lang("115370"));
                    setButtonState(5, ButtonFunction);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(4);
                    }
                }
                else{
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("115334"));
                    setButtonState(3, ButtonRepair);
                    setButtonName(1, _lang("115370"));
                    setButtonState(1, ButtonFunction);
                    
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        setButtonEnable(3);
                    }
                }
            }
                break;
            case TerritoryBroken: {
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
                setButtonName(3, _lang("115334"));
                setButtonState(3, ButtonRepair);
                setButtonName(2, _lang("115370"));
                setButtonState(2, ButtonFunction);
            }
                break;
            case TerritoryMineOccupy: {//本联盟建筑被占领
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(4, _lang("108726"));
                setButtonName(5, _lang("115370"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(4, ButtonRally);
                setButtonState(5, ButtonFunction);
            }
                break;
            case TerritoryOtherUnOccupy: {//非本联盟建筑未被占领
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(4, _lang("108726"));
                setButtonName(5, _lang("108722"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(4, ButtonRally);
                setButtonState(5, ButtonScout);
            }
                break;
            case TerritoryOtherOccupy: {//非本联盟建筑被非本联盟占领
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));
                setButtonName(2, _lang("115331"));
                setButtonName(4, _lang("108726"));
                setButtonName(5, _lang("108722"));
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonInformation);
                setButtonState(4, ButtonRally);
                setButtonState(5, ButtonScout);
            }
                break;
            case TerritoryOtherOccupyByMe: {//非本联盟建筑被本联盟占领
                auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
                auto it = selfMarch.find(m_cityInfo.parentCityIndex);
                if (it != selfMarch.end()) {//判断是否已经有部队正在建造中，如有则变成返回
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("115331"));
                    setButtonState(2, ButtonInformation);
                    setButtonName(3, _lang("108725"));
                    setButtonState(3, ButtonGoHome);
                }
                else {
                    buttonCount = 1;
                    setButtonCount(buttonCount);
                    setButtonName(1, _lang("115331"));
                    setButtonState(1, ButtonInformation);
                }
            }
                break;
            default: {
                buttonCount = 1;
                setButtonCount(buttonCount);
                setButtonName(1, _lang("115331"));
                setButtonState(1, ButtonInformation);
            }
                break;
        }
    }
    
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(TerritoryTile::onClickButton));
    }
}

void TerritoryTile::update(float sec) {
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
    if (WorldController::getInstance()->forceUpdateTerritoryTile == true) {
        flag = true;
        WorldController::getInstance()->forceUpdateTerritoryTile = false;
    }
    
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        double now = GlobalData::shared()->getTimeStamp();
        //设置联盟建筑当前城防值
        switch (cityIt->second.m_allianceAreaInfo.state) {
            case 1: {//未建造
                auto currentDef = cityIt->second.m_allianceAreaInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
            }
                break;
            case 0: {//建造中
                double timePass = now - cityIt->second.m_allianceAreaInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_allianceAreaInfo.defence + defTemp;
                if (currentDef>=cityIt->second.m_allianceAreaInfo.defenceMax) {
                    currentDef = cityIt->second.m_allianceAreaInfo.defenceMax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 4: {
                auto currentDef = cityIt->second.m_allianceAreaInfo.defence;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 5: {//修理中
                double timePass = now - cityIt->second.m_allianceAreaInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_allianceAreaInfo.defence + defTemp;
                if (currentDef>=cityIt->second.m_allianceAreaInfo.defenceMax) {
                    currentDef = cityIt->second.m_allianceAreaInfo.defenceMax;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({0, 190, 13});
            }
                break;
            case 2:
            case 3: {//满值
                auto currentDef = cityIt->second.m_allianceAreaInfo.defenceMax;
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({190, 0, 0});
            }
                break;
            case 6: {//摧毁中
                double timePass = now - cityIt->second.m_allianceAreaInfo.startTime;
                timePass = MAX(timePass, 0.0f);
                double defTemp = cityIt->second.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = cityIt->second.m_allianceAreaInfo.defence - defTemp;
                if (currentDef<=0) {
                    currentDef = 0;
                }
                m_title->setString(CCCommonUtils::getResourceStr(currentDef).c_str());
                m_title->setColor({220, 176, 0});
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

bool TerritoryTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

TerritoryTileState TerritoryTile::getState() {
    int type = -1;
    TerritoryTileState m_tempState;
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        type = cityIt->second.m_allianceAreaInfo.state;
        if (cityIt->second.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (type == 0) {
                m_tempState = TerritoryBuilding;
            }
            else if (type == 1) {
                m_tempState = TerritoryUnfinished;
            }
            else if (type == 2) {
                m_tempState = TerritoryStation;
            }
            else if (type == 3) {
                m_tempState = TerritoryUnstation;
            }
            else if (type == 4) {
                m_tempState = TerritoryBroken;
            }
            else if (type == 5) {
                m_tempState = TerritoryRepair;
            }
            else if (type == 6) {
                m_tempState = TerritoryMineOccupy;
            }
        }
        else {
            if (type != 6) {
                m_tempState = TerritoryOtherUnOccupy;
            }
            else {
                std::string m_allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
                std::string m_uId = GlobalData::shared()->playerInfo.uid;
                if ((m_allianceId != "" && cityIt->second.m_allianceAreaOwnerInfo.allianceId == m_allianceId) || (m_allianceId == "" && cityIt->second.m_allianceAreaOwnerInfo.ownerId == m_uId)) {//非本联盟建筑被本联盟占领判断条件，待加
                    m_tempState = TerritoryOtherOccupyByMe;
                }
                else
                    m_tempState = TerritoryOtherOccupy;
            }
        }
    }
    return m_tempState;
}

SEL_CCControlHandler TerritoryTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* TerritoryTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.parentCityIndex,m_title->getString());
}

void TerritoryTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonStation:
        case ButtonRepair:
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
        case ButtonMarch: {
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
                
            unsigned int index = m_cityInfo.parentCityIndex;
            auto func = [&,index](){
                WorldController::getInstance()->openMarchDeploy(index,1);
            };
                
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonRally: {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            if (now < selfProtect) {
                CCCommonUtils::flyHint("", "", _lang("108623"));
                closeThis();
                return;
            }
            if(!GlobalData::shared()->playerInfo.isInAlliance()){
                CCCommonUtils::flyHint("", "", _lang("115218"));
                closeThis();
                return;
            }
            unsigned int index = m_cityInfo.parentCityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto func = [&,index](){
                AllianceManager::getInstance()->openAllianceMassView(index);
                closeThis();
            };
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);
            } else {
                func();
                closeThis();
            }
            return;
        }
            break;
        case ButtonScout: {
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.parentCityIndex;
            auto func = [&,index](){
                auto world = WorldMapView::instance();
                if (world) {
                    world->addPopupView(ScoutInfo::create(WorldController::getInstance()->getCityInfos()[index]));
                }
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonInformation: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(TerritoryInfoDetailView::create(WorldController::getInstance()->getCityInfos()[index]));
        }
            break;
        case ButtonFunction: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(TerritoryFunctionView::create(WorldController::getInstance()->getCityInfos()[index].m_allianceAreaInfo.state));
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