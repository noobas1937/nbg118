//
//  TerritoryInfoDetailView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/7/17.
//
//

#include "TerritoryInfoDetailView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AllianceTerritoryShowCommand.h"
#include "YuanJunInfo.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "AllianceScienceView.h"
#include "TerritoryFunctionView.h"
#include "TerritoryTowerTipsView.h"
#include "YuanYunReturnCommand.h"
#define  MSG_TERRITORY_INFO_DETAIL_CELL_CLICK       "msg_territory_info_detail_cell_click"
#define  MSG_TERRITORY_INFO_QIANFAN             "msg_territory_info_qianfan"
TerritoryInfoDetailView::TerritoryInfoDetailView(WorldCityInfo& info):m_cityInfo(info){
};

TerritoryInfoDetailView::~TerritoryInfoDetailView(){
};

TerritoryInfoDetailView *TerritoryInfoDetailView::create(WorldCityInfo& info){
    TerritoryInfoDetailView *ret = new TerritoryInfoDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryInfoDetailView::onEnter(){
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(208, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(208, false);
    });
    m_tabView->reloadData();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryInfoDetailView::onCellClick), MSG_TERRITORY_INFO_DETAIL_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryInfoDetailView::qianFanCallBack), MSG_TERRITORY_INFO_QIANFAN, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void TerritoryInfoDetailView::onExit(){
    setTouchEnabled(false);
   // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_INFO_DETAIL_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_INFO_QIANFAN);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool TerritoryInfoDetailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryInfoDetailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (m_cityInfo.cityType == Tile_allianceArea) {
        if (WorldController::isInSelfServer(m_cityInfo.tileServerId) && isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=4) {
            CCCommonUtils::flyHint("", "", _lang("115492"), 3, 0, true);
        }
    }else if (m_cityInfo.cityType == tile_banner){
        if (WorldController::isInSelfServer(m_cityInfo.tileServerId) && isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=4) {
            CCCommonUtils::flyHint("", "", _lang("115529"), 3, 0, true);
        }
    }else {
        if (WorldController::isInSelfServer(m_cityInfo.tileServerId) && isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=3) {
            CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
        }
    }
}

bool TerritoryInfoDetailView::init(){
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(208, false);
        });
        setIsHDPanel(true);
        m_data = CCArray::create();
        
        auto node = CCBLoadFile("TerritoryInfoDetailView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(node->getContentSize());
            int extH = getExtendHeight();
            m_infoList->setPositionY(m_infoList->getPositionY() - extH);
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
            m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        }
        
        CCCommonUtils::setButtonTitle(m_withdrawBtn, _lang("115343").c_str());
        m_withdrawBtn->setVisible(true);
        m_withdrawBtn->setEnabled(false);
        
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        auto picBg1 = CCLoadSprite::createSprite("technology_09.png");
//        picBg1->setAnchorPoint(ccp(0, 0));
//        picBg1->setPosition(ccp(0, 0));
//        if (CCCommonUtils::isIosAndroidPad()) {
//            picBg1->setScaleX(2.4);
//        }
//        tBatchNode->addChild(picBg1);
//        int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 79;
//        int curHeight = picBg1->getContentSize().height;
//        while(curHeight < maxHeight)
//        {
//            auto picBg2 = CCLoadSprite::createSprite("technology_09.png");
//            picBg2->setAnchorPoint(ccp(0, 0));
//            picBg2->setPosition(ccp(0, curHeight));
//            if (CCCommonUtils::isIosAndroidPad()) {
//                picBg2->setScaleX(2.4);
//            }
//            tBatchNode->addChild(picBg2);
//            curHeight += picBg2->getContentSize().height;
//        }
//        m_bgNode->addChild(tBatchNode);
        
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        //        m_titleTxt->setString(_lang("115331"));
        std::string picStr = "";
        if (m_cityInfo.cityType == Tile_allianceArea) {
            picStr = "territory_building_lv1.png";
            if (CCCommonUtils::isIosAndroidPad()) {
                m_picNode->setScale(1.8);
            }
            else
                m_picNode->setScale(1.2);
        }
        else if (m_cityInfo.cityType == tile_tower) {
            picStr = "territory_tower0.png";
            if (CCCommonUtils::isIosAndroidPad()) {
                m_picNode->setScale(1.8);
            }
            else
                m_picNode->setScale(1.2);
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            picStr = "territory_warehouse0.png";
            if (CCCommonUtils::isIosAndroidPad()) {
                m_picNode->setScale(2);
            }
        }
        else if (m_cityInfo.cityType == tile_banner) {
            picStr = "territory_tower0.png";//////////////////
            if (CCCommonUtils::isIosAndroidPad()) {
                m_picNode->setScale(1.8);
            }
            else
                m_picNode->setScale(1.2);
        }
        auto picSprite = CCLoadSprite::createSprite(picStr.c_str());
        if (picSprite) {
            m_picNode->addChild(picSprite);
        }
        destroyingArmy = 0;
        m_bannerNode->setVisible(false);
        if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
            m_territoryFlag = false;
            refreshView();
            m_isInUpdate = false;
            return true;
        }
        requestDetail();
        addLoadingAni();
        m_isInUpdate = false;
        return true;
    }
    
    return false;
}

void TerritoryInfoDetailView::requestDetail() {
    m_isInUpdate = true;
    unsigned index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        auto cmd = new AllianceTerritoryDetailCommand(index);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryInfoDetailView::onDetailCallback), nullptr));
        cmd->sendAndRelease();
    }
}

void TerritoryInfoDetailView::onDetailCallback(cocos2d::CCObject *obj) {
    removeLoadingAni();
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
    
    if (info->objectForKey("stat")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
        }
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
    
    if (!info->objectForKey("owner")) {
        m_cityInfo.m_allianceAreaOwnerInfo.ownerId = "";
        m_cityInfo.m_allianceAreaOwnerInfo.ownerName = "";
        m_cityInfo.m_allianceAreaOwnerInfo.allianceId = "";
        m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = "";
        m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = "";
        m_cityInfo.m_allianceAreaOwnerInfo.team = false;
    }
    if (info->objectForKey("owner")) {
        CCDictionary* owner = _dict(info->objectForKey("owner"));
        if (owner->objectForKey("uid")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
        }
        if (owner->objectForKey("name")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
        }
        if (owner->objectForKey("allianceId")) {
            m_cityInfo.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
        }
        if (owner->objectForKey("abbr")) {
            m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
        }
        if (owner->objectForKey("team")) {
            m_cityInfo.m_allianceAreaOwnerInfo.team = owner->valueForKey("team")->boolValue();
        }
        if (owner->objectForKey("pic")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
        }
    }
    
    if (info->objectForKey("curDef")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.defence = info->valueForKey("curDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.defence = info->valueForKey("curDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.defence = info->valueForKey("curDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.defence = info->valueForKey("curDef")->intValue();
        }
    }
    if (info->objectForKey("maxDef")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.defenceMax = info->valueForKey("maxDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.defenceMax = info->valueForKey("maxDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.defenceMax = info->valueForKey("maxDef")->intValue();
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.defenceMax = info->valueForKey("maxDef")->intValue();
        }
    }
    if (info->objectForKey("updateTime")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
        }
    }
    if (info->objectForKey("startTime")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
    }
    if (info->objectForKey("buildingSpd")) {
        if (m_cityInfo.cityType == Tile_allianceArea) {
            m_cityInfo.m_allianceAreaInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
        else if (m_cityInfo.cityType == tile_tower) {
            m_cityInfo.m_towerInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
        else if (m_cityInfo.cityType == tile_wareHouse) {
            m_cityInfo.m_warehouseInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
        else if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_bannerInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
    }
    if (info->objectForKey("flag")) {
        m_cityInfo.m_bannerInfo.banner = info->valueForKey("flag")->getCString();
    }
    if (info->objectForKey("name")) {
        m_cityInfo.m_aArea_nickname = info->valueForKey("name")->getCString();
    }
    if (!info->objectForKey("name")) {
        if (info->valueForKey("count")) {
            int num = info->valueForKey("count")->intValue();
            if (m_cityInfo.cityType == Tile_allianceArea) {
                m_cityInfo.m_aArea_nickname = _lang_1("115312", CC_ITOA(num));
            }
            else if (m_cityInfo.cityType == tile_tower) {
                m_cityInfo.m_aArea_nickname = _lang_1("115406", CC_ITOA(num));
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                m_cityInfo.m_aArea_nickname = _lang("115364");
            }
        }
        if (m_cityInfo.cityType == tile_banner) {
            m_cityInfo.m_aArea_nickname = _lang("115534");
        }
    }
    if (info->objectForKey("allianceAbbr")) {
        m_cityInfo.m_aArea_simpleName = info->valueForKey("allianceAbbr")->getCString();
    }
    
    m_data->removeAllObjects();
    if (info->objectForKey("allianceId")) {
        std::string allianceId = info->valueForKey("allianceId")->getCString();
        if (allianceId.compare(GlobalData::shared()->playerInfo.allianceInfo.uid)==0) {
            m_ownerFlag = true;
        }
        else
            m_ownerFlag = false;
        
        if (allianceId.compare(GlobalData::shared()->playerInfo.allianceInfo.uid)==0) {
            m_territoryFlag = true;
            std::string ownerAllianceId;
            if (info->objectForKey("owner")) {
                CCDictionary* owner = _dict(info->objectForKey("owner"));
                ownerAllianceId = owner->valueForKey("allianceId")->getCString();
                if (ownerAllianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    m_owner = true;
                }
                if (ownerAllianceId != GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    m_owner = false;//enermy
                }
            }
            else {
                m_owner = true;
            }
            bool tempFlag = true;
            if (info->objectForKey("army")) {
                CCArray* array = (CCArray*)info->objectForKey("army");
                if(array){
                    int count = array->count();
                    for(int i=0;i<count;i++){
                        auto item = _dict(array->objectAtIndex(i));
                        YuanJunInfo* info = new YuanJunInfo();
                        info->parseInfo(item);
                        m_data->addObject(info);
                        if (info->getUid()==GlobalData::shared()->playerInfo.uid) {
                            tempFlag = false;
                        }
                        info->release();
                        if (m_cityInfo.cityType == Tile_allianceArea && m_cityInfo.m_allianceAreaInfo.state == 6) {
                            if (item != NULL && item->objectForKey("totalTroops")) {
                                destroyingArmy += item->valueForKey("totalTroops")->intValue();
                            }
                        }
                        else if (m_cityInfo.cityType == tile_banner && m_cityInfo.m_bannerInfo.state == 6) {
                            if (item != NULL && item->objectForKey("totalTroops")) {
                                destroyingArmy += item->valueForKey("totalTroops")->intValue();
                            }
                        }
                    }
                }
            }
            
            if (m_owner) {
                int num = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX));
                int gapNum = num - m_data->count();
                gapNum = MAX(gapNum,0);
                YuanJunInfo* info = NULL;
                for (int i=0; i<gapNum; i++) {
                    YuanJunInfo* info = new YuanJunInfo();
                    CCArray* array = CCArray::create();
                    info->setSoldiers(array);
                    info->setCanUse(tempFlag);
                    if (soldierAlready >= soldierMax && soldierMax != 0) {
                        info->setIndex(4);
                    }
                    else
                        info->setIndex(2);
                    m_data->addObject(info);
                    info->release();
                }
                info = new YuanJunInfo();
                CCArray* array = CCArray::create();
                info->setSoldiers(array);
                info->setIndex(3);
                m_data->addObject(info);
                info->release();
            }
        }
        else {
            std::string ownerAllianceId;
            std::string ownerId;
            if (info->objectForKey("owner")) {
                CCDictionary* owner = _dict(info->objectForKey("owner"));
                ownerAllianceId = owner->valueForKey("allianceId")->getCString();
                ownerId = owner->valueForKey("uid")->getCString();
                if ((ownerAllianceId != "" && ownerAllianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) || (ownerAllianceId == "" && ownerId == GlobalData::shared()->playerInfo.uid)) {
                    m_territoryFlag = true;
                    if (info->objectForKey("army")) {
                        CCArray* array = (CCArray*)info->objectForKey("army");
                        if(array){
                            int count = array->count();
                            for(int i=0;i<count;i++){
                                auto item = _dict(array->objectAtIndex(i));
                                YuanJunInfo* info = new YuanJunInfo();
                                info->parseInfo(item);
                                m_data->addObject(info);
                                info->release();
                                if (m_cityInfo.cityType == Tile_allianceArea && m_cityInfo.m_allianceAreaInfo.state == 6) {
                                    if (item != NULL && item->objectForKey("totalTroops")) {
                                        destroyingArmy += item->valueForKey("totalTroops")->intValue();
                                    }
                                }
                                else if (m_cityInfo.cityType == tile_banner && m_cityInfo.m_bannerInfo.state == 6) {
                                    if (item != NULL && item->objectForKey("totalTroops")) {
                                        destroyingArmy += item->valueForKey("totalTroops")->intValue();
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    m_territoryFlag = false;//enermy
                    if (info->objectForKey("army")) {
                        CCArray* array = (CCArray*)info->objectForKey("army");
                        if(array){
                            int count = array->count();
                            for(int i=0;i<count;i++){
                                auto item = _dict(array->objectAtIndex(i));
                                if (m_cityInfo.cityType == Tile_allianceArea && m_cityInfo.m_allianceAreaInfo.state == 6) {
                                    if (item != NULL && item->objectForKey("totalTroops")) {
                                        destroyingArmy += item->valueForKey("totalTroops")->intValue();
                                    }
                                }
                                else if (m_cityInfo.cityType == tile_banner && m_cityInfo.m_bannerInfo.state == 6) {
                                    if (item != NULL && item->objectForKey("totalTroops")) {
                                        destroyingArmy += item->valueForKey("totalTroops")->intValue();
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {
                m_territoryFlag = false;
            }
        }
    }
    
    refreshView();
}

void TerritoryInfoDetailView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void TerritoryInfoDetailView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void TerritoryInfoDetailView::refreshView() {
    if (m_cityInfo.cityType == Tile_allianceArea) {
        m_territoryStat = m_cityInfo.m_allianceAreaInfo.state;
    }
    else if (m_cityInfo.cityType == tile_tower) {
        m_territoryStat = m_cityInfo.m_towerInfo.state;
    }
    else if (m_cityInfo.cityType == tile_wareHouse) {
        m_territoryStat = m_cityInfo.m_warehouseInfo.state;
    }
    else if (m_cityInfo.cityType == tile_banner) {
        m_territoryStat = m_cityInfo.m_bannerInfo.state;
    }
    
    if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {
        m_withdrawBtn->setEnabled(true);
        if (m_cityInfo.cityType == Tile_allianceArea || m_cityInfo.cityType == tile_banner) {
            if (GlobalData::shared()->playerInfo.allianceInfo.rank == 4) {
                m_withdrawBtn->setEnabled(false);
            }
        }
    }
    
    if (m_cityInfo.m_aArea_id != GlobalData::shared()->playerInfo.allianceInfo.uid || GlobalData::shared()->playerInfo.allianceInfo.rank < 4) {
        m_withdrawBtn->setEnabled(false);
    }
    
    std::string strTitle = "(";
    strTitle.append(m_cityInfo.m_aArea_simpleName);
    strTitle.append(")");
    strTitle.append(m_cityInfo.m_aArea_nickname);
    m_titleTxt->setString(strTitle);
    m_statLabel->setColor({243,69,0});
    std::string strStat = "";//状态
    switch (m_territoryStat) {
        case 0: {
            strStat =  _lang("115305");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_tower) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_towerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_warehouseInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_troopLabel1->setString(_lang_1("115317",""));
            m_troopLabel2->setString(troopStr);
            m_stat = 0;
        }
            break;
        case 1: {
            strStat = _lang("115306");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                m_tipsLabel->setString(_lang("115322"));
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_tower) {
                m_tipsLabel->setString(_lang("115407"));
                std::string strDef = CC_ITOA(m_cityInfo.m_towerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                m_tipsLabel->setString(_lang("115468"));
                std::string strDef = CC_ITOA(m_cityInfo.m_warehouseInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                m_tipsLabel->setString(_lang("115526"));
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115317",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 1;
        }
            break;
        case 2:{
            strStat = _lang("115346");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115326"));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115326"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115320",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 2;
        }
            break;
        case 3: {
            strStat = _lang("115308");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115327"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115530"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115320",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 3;
        }
            break;
        case 4: {
            strStat = _lang("115309");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115324"));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115531"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115318",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 4;
        }
            break;
        case 5: {
            strStat = _lang("115310");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115323"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115323"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_troopLabel1->setString(_lang_1("115318",""));
            m_troopLabel2->setString(troopStr);
            m_stat = 5;
        }
            break;
        case 6: {
            if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                strStat = _lang("115311");
            }
            else {
                strStat = _lang("115347");
            }
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115325"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115532"));
            }
            m_troopLabel1->setString(_lang_1("115319",""));
            std::string troopStr = CC_ITOA(destroyingArmy);
            m_troopLabel2->setString(troopStr);
            m_stat = 6;
        }
            break;
        default:
            break;
    }
    
    if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (m_cityInfo.m_bannerInfo.defence == m_cityInfo.m_bannerInfo.defenceMax) {
                string bannerStr = m_cityInfo.m_bannerInfo.banner;
                bannerStr.append(".png");
                auto bannerFlag = CCLoadSprite::createSprite(bannerStr.c_str());
                if (bannerFlag) {
                    m_bannerNode->addChild(bannerFlag);
                    m_bannerNode->setVisible(true);
                    m_tipsLabel->setString(_lang_1("115527", _lang(m_cityInfo.m_bannerInfo.banner).c_str()));
                    if (CCCommonUtils::isIosAndroidPad()) {
                        m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(72, 0));
                    }
                    else {
                        m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(30, 0));
                    }
                }
            }
        }
        else {
            string bannerStr = m_cityInfo.m_bannerInfo.banner;
            bannerStr.append(".png");
            auto bannerFlag = CCLoadSprite::createSprite(bannerStr.c_str());
            if (bannerFlag) {
                m_bannerNode->addChild(bannerFlag);
                m_bannerNode->setVisible(true);
                m_tipsLabel->setString(_lang_1("115527", _lang(m_cityInfo.m_bannerInfo.banner).c_str()));
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(72, 0));
                }
                else {
                    m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(30, 0));
                }
            }
        }
    }
    if (!m_territoryFlag) {
        //        m_timeLabel1->setVisible(false);
        //        m_troopLabel1->setVisible(false);
        //        m_timeLabel2->setVisible(false);
        //        m_troopLabel2->setVisible(false);
    }
    m_tabView->reloadData();
}

void TerritoryInfoDetailView::refreshOnce() {
    if (m_cityInfo.cityType == Tile_allianceArea) {
        m_territoryStat = m_cityInfo.m_allianceAreaInfo.state;
    }
    else if (m_cityInfo.cityType == tile_tower) {
        m_territoryStat = m_cityInfo.m_towerInfo.state;
    }
    else if (m_cityInfo.cityType == tile_wareHouse) {
        m_territoryStat = m_cityInfo.m_warehouseInfo.state;
    }
    m_statLabel->setColor({0xff, 0x54, 0x54});
    std::string strStat = "";//状态
    switch (m_territoryStat) {
        case 0: {
            strStat =  _lang("115305");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_tower) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_towerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_warehouseInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                m_tipsLabel->setString(_lang("115321"));
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_troopLabel1->setString(_lang_1("115317",""));
            m_troopLabel2->setString(troopStr);
            m_stat = 0;
        }
            break;
        case 1: {
            strStat = _lang("115306");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                m_tipsLabel->setString(_lang("115322"));
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_tower) {
                m_tipsLabel->setString(_lang("115407"));
                std::string strDef = CC_ITOA(m_cityInfo.m_towerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                m_tipsLabel->setString(_lang("115468"));
                std::string strDef = CC_ITOA(m_cityInfo.m_warehouseInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                m_tipsLabel->setString(_lang("115526"));
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115317",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 1;
        }
            break;
        case 2:{
            strStat = _lang("115346");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115326"));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115326"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115320",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 2;
        }
            break;
        case 3: {
            strStat = _lang("115308");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115327"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115530"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115320",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 3;
        }
            break;
        case 4: {
            strStat = _lang("115309");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115324"));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115531"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_timeLabel1->setString(_lang_1("115318",""));
            m_timeLabel2->setString(troopStr);
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 4;
        }
            break;
        case 5: {
            strStat = _lang("115310");
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115323"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115323"));
            }
            std::string troopStr = CC_ITOA(soldierAlready);
            troopStr.append("/");
            troopStr.append(CC_ITOA(soldierMax));
            m_troopLabel1->setString(_lang_1("115318",""));
            m_troopLabel2->setString(troopStr);
            m_stat = 5;
        }
            break;
        case 6: {
            if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                strStat = _lang("115311");
            }
            else {
                strStat = _lang("115347");
            }
            m_statLabel->setString(strStat);
            if (m_cityInfo.cityType == Tile_allianceArea) {
                std::string strDef = CC_ITOA(m_cityInfo.m_allianceAreaInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115325"));
                
            }
            else if (m_cityInfo.cityType == tile_banner) {
                std::string strDef = CC_ITOA(m_cityInfo.m_bannerInfo.defence);
                strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax);
                m_defLabel1->setString(_lang_1("115345",""));
                m_defLabel2->setString(strDef);
                m_tipsLabel->setString(_lang("115532"));
            }
            m_troopLabel1->setString(_lang_1("115319",""));
            std::string troopStr = CC_ITOA(destroyingArmy);
            m_troopLabel2->setString(troopStr);
            m_stat = 6;
        }
            break;
        default:
            break;
    }
    
    if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            if (m_cityInfo.m_bannerInfo.defence == m_cityInfo.m_bannerInfo.defenceMax) {
                auto bannerFlag = CCLoadSprite::createSprite(m_cityInfo.m_bannerInfo.banner.c_str());
                if (bannerFlag) {
                    m_bannerNode->addChild(bannerFlag);
                    m_bannerNode->setVisible(true);
                    m_tipsLabel->setString(_lang_1("115527", _lang(m_cityInfo.m_bannerInfo.banner).c_str()));
                    m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(0, bannerFlag->getContentSize().width * bannerFlag->getScaleX() / 2));
                }
            }
        }
        else {
            auto bannerFlag = CCLoadSprite::createSprite(m_cityInfo.m_bannerInfo.banner.c_str());
            if (bannerFlag) {
                m_bannerNode->addChild(bannerFlag);
                m_bannerNode->setVisible(true);
                m_tipsLabel->setString(_lang_1("115527", _lang(m_cityInfo.m_bannerInfo.banner).c_str()));
                m_tipsLabel->setPosition(m_bannerNode->getPosition() + ccp(0, bannerFlag->getContentSize().width * bannerFlag->getScaleX() / 2));
            }
        }
    }
}

void TerritoryInfoDetailView::update(float sec) {
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
    //    if (!m_territoryFlag) {
    //        return;
    //    }
    if (WorldController::getInstance()->forceUpdateTerritoryInfo == true) {
        refreshOnce();
        WorldController::getInstance()->forceUpdateTerritoryInfo = false;
    }
    
    double now = GlobalData::shared()->getTimeStamp();
    //设置联盟建筑当前城防值
    switch (m_territoryStat) {
        case 0: {
            if (m_cityInfo.cityType == Tile_allianceArea) {
                double timePass = now - m_cityInfo.m_allianceAreaInfo.startTime;
                double defTemp = m_cityInfo.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_allianceAreaInfo.defenceMax) {
                    currentDef = m_cityInfo.m_allianceAreaInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115305");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_allianceAreaInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
            else if (m_cityInfo.cityType == tile_tower) {
                double timePass = now - m_cityInfo.m_towerInfo.startTime;
                double defTemp = m_cityInfo.m_towerInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_towerInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_towerInfo.defenceMax) {
                    currentDef = m_cityInfo.m_towerInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115305");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_towerInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                double timePass = now - m_cityInfo.m_warehouseInfo.startTime;
                double defTemp = m_cityInfo.m_warehouseInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_warehouseInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_warehouseInfo.defenceMax) {
                    currentDef = m_cityInfo.m_warehouseInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115305");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_warehouseInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                double timePass = now - m_cityInfo.m_bannerInfo.startTime;
                double defTemp = m_cityInfo.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_bannerInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_bannerInfo.defenceMax) {
                    currentDef = m_cityInfo.m_bannerInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115305");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_bannerInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
        }
            break;
        case 1: {
            if (m_cityInfo.cityType == Tile_allianceArea) {
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_tower) {
                auto currentDef = m_cityInfo.m_towerInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_towerInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_wareHouse) {
                auto currentDef = m_cityInfo.m_warehouseInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_warehouseInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                auto currentDef = m_cityInfo.m_bannerInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
        }
            break;
        case 2:
        case 3:{
            if (m_cityInfo.cityType == Tile_allianceArea) {
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defenceMax;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                auto currentDef = m_cityInfo.m_bannerInfo.defenceMax;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
        }
            break;
        case 4: {
            if (m_cityInfo.cityType == Tile_allianceArea) {
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
            else if (m_cityInfo.cityType == tile_banner) {
                auto currentDef = m_cityInfo.m_bannerInfo.defence;
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
            }
        }
            break;
        case 5: {
            if (m_cityInfo.cityType == Tile_allianceArea) {
                double timePass = now - m_cityInfo.m_allianceAreaInfo.startTime;
                double defTemp = m_cityInfo.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_allianceAreaInfo.defenceMax) {
                    currentDef = m_cityInfo.m_allianceAreaInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115310");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_allianceAreaInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
            else if (m_cityInfo.cityType == tile_banner) {
                double timePass = now - m_cityInfo.m_bannerInfo.startTime;
                double defTemp = m_cityInfo.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_bannerInfo.defence + defTemp;
                if (currentDef>=m_cityInfo.m_bannerInfo.defenceMax) {
                    currentDef = m_cityInfo.m_bannerInfo.defenceMax;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
                std::string timeStr = _lang("115310");
                timeStr.append(": ");
                m_timeLabel1->setString(timeStr);
                double timeLeft = m_cityInfo.m_bannerInfo.updateTime - now;
                timeLeft = MAX(0.0f, timeLeft);
                m_timeLabel2->setString(CC_SECTOA(timeLeft));
            }
        }
            break;
        case 6: {
            if (m_cityInfo.cityType == Tile_allianceArea) {
                double timePass = now - m_cityInfo.m_allianceAreaInfo.startTime;
                double defTemp = m_cityInfo.m_allianceAreaInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_allianceAreaInfo.defence - defTemp;
                if (currentDef<=0) {
                    currentDef = 0;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_allianceAreaInfo.defenceMax));
                m_defLabel2->setString(strDef);
                if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    std::string timeStr = _lang("115311");
                    timeStr.append(": ");
                    m_timeLabel1->setString(timeStr);
                    double timeLeft = m_cityInfo.m_allianceAreaInfo.updateTime - now;
                    timeLeft = MAX(0.0f, timeLeft);
                    m_timeLabel2->setString(CC_SECTOA(timeLeft));
                }
                else {
                    std::string timeStr = _lang("115347");
                    timeStr.append(": ");
                    m_timeLabel1->setString(timeStr);
                    double timeLeft = m_cityInfo.m_allianceAreaInfo.updateTime - now;
                    timeLeft = MAX(0.0f, timeLeft);
                    m_timeLabel2->setString(CC_SECTOA(timeLeft));
                }
            }
            else if (m_cityInfo.cityType == tile_banner) {
                double timePass = now - m_cityInfo.m_bannerInfo.startTime;
                double defTemp = m_cityInfo.m_bannerInfo.buildSpeed * timePass;
                auto currentDef = m_cityInfo.m_bannerInfo.defence - defTemp;
                if (currentDef<=0) {
                    currentDef = 0;
                }
                std::string strDef = CC_ITOA(currentDef);
                strDef.append("/");
                strDef.append(CC_ITOA(m_cityInfo.m_bannerInfo.defenceMax));
                m_defLabel2->setString(strDef);
                if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                    std::string timeStr = _lang("115311");
                    timeStr.append(": ");
                    m_timeLabel1->setString(timeStr);
                    double timeLeft = m_cityInfo.m_bannerInfo.updateTime - now;
                    timeLeft = MAX(0.0f, timeLeft);
                    m_timeLabel2->setString(CC_SECTOA(timeLeft));
                }
                else {
                    std::string timeStr = _lang("115347");
                    timeStr.append(": ");
                    m_timeLabel1->setString(timeStr);
                    double timeLeft = m_cityInfo.m_bannerInfo.updateTime - now;
                    timeLeft = MAX(0.0f, timeLeft);
                    m_timeLabel2->setString(CC_SECTOA(timeLeft));
                }
            }
        }
            break;
        default:
            break;
    }
}

void TerritoryInfoDetailView::onCellClick(CCObject* data){
    CCString* uuid = dynamic_cast<CCString*>(data);
    if(uuid){
        int num = m_data->count();
        for (int i=0; i<num; i++) {
            YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(i);
            if(info->getUuid()==uuid->getCString()){
                info->setOpen(!info->getOpen());
                break;
            }
        }
        m_tabView->reloadData();
    }
}

void TerritoryInfoDetailView::qianFanCallBack(CCObject *data)
{
    CCString* uid = dynamic_cast<CCString*>(data);
    if (uid) {
        CCObject* obj = NULL;
        CCARRAY_FOREACH(m_data, obj)
        {
            YuanJunInfo* info = dynamic_cast<YuanJunInfo*>(obj);
            if (info && info->getUid() == uid->getCString())
            {
                m_data->removeObject(obj);
                break;
            }
        }
        bool canUse = true;
        obj = NULL;
        CCARRAY_FOREACH(m_data, obj)
        {
            YuanJunInfo* info = dynamic_cast<YuanJunInfo*>(obj);
            if (info && info->getUid() == GlobalData::shared()->playerInfo.uid)
            {
                canUse = false;
                break;
            }
        }
        int num = m_data->count();
        YuanJunInfo* info = new YuanJunInfo();
        CCArray* array = CCArray::create();
        info->setSoldiers(array);
        info->setCanUse(canUse);
        info->setIndex(2);
        m_data->insertObject(info, num-1);
        //        m_data->addObject(info);
        info->release();
        m_tabView->reloadData();
    }
}

void TerritoryInfoDetailView::onTipBtnClick(CCObject *pSender, CCControlEvent event){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
    //    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");
}

void TerritoryInfoDetailView::onWithdrawClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::marchAlertShow(_lang_1("115344", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryInfoDetailView::secondConfirm)),NULL);
}

void TerritoryInfoDetailView::secondConfirm(){
    if (m_cityInfo.cityType == Tile_allianceArea) {
        YesNoDialog::marchAlertShow(_lang_1("115490", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryInfoDetailView::withdraw)),NULL);
    }
    else
        YesNoDialog::marchAlertShow(_lang_1("115355", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryInfoDetailView::withdraw)),NULL);
}

void TerritoryInfoDetailView::withdraw(){
    if (m_cityInfo.cityType == Tile_allianceArea) {
        string tempTuid = getTuid();
        auto cmd = new WorldAllianceAreaCancelCommand(m_cityInfo.parentCityIndex,tempTuid);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removeAllPopupView();
    }
    else if (m_cityInfo.cityType == tile_tower) {
        string tempTuid = m_cityInfo.m_towerInfo.uid;
        auto cmd = new WorldTowerCancelCommand(tempTuid,m_cityInfo.cityIndex);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removeAllPopupView();
    }
    else if (m_cityInfo.cityType == tile_wareHouse) {
        string tempTuid = m_cityInfo.m_warehouseInfo.uid;
        auto cmd = new WorldTerritoryWarehouseCancelCommand(tempTuid,m_cityInfo.parentCityIndex);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removeAllPopupView();
    }
    else if (m_cityInfo.cityType == tile_banner) {
        string tempTuid = m_cityInfo.m_bannerInfo.uid;
        auto cmd = new WorldTerritoryBannerCancelCommand(tempTuid,m_cityInfo.cityIndex);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

bool TerritoryInfoDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bannerNode", CCNode*, this->m_bannerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statLabel", CCLabelIF*, this->m_statLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel1", CCLabelIF*, this->m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel1", CCLabelIF*, this->m_troopLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel2", CCLabelIF*, this->m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel2", CCLabelIF*, this->m_troopLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    return false;
}

SEL_CCControlHandler TerritoryInfoDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawClick", TerritoryInfoDetailView::onWithdrawClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", TerritoryInfoDetailView::onTipBtnClick);
    return NULL;
}

const int CELL_W = 640;
const int CELL_H = 104;

CCSize TerritoryInfoDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if (obj->getOpen()) {
        int num = obj->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = row*100 + 20;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = row*200;
        }
        if (m_ownerFlag && obj->getUid() != GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+120);
            }
            return CCSize(CELL_W, CELL_H+addH+50);
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+40);
            }
            return CCSize(CELL_W, CELL_H+addH+50);
        }
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(CELL_W, CELL_H);
}

CCSize TerritoryInfoDetailView::cellSizeForTable(CCTableView *table)
{
//    if (CCCommonUtils::isIosAndroidPad()) {
//        return CCSize(1470, 250);
//    }
    return CCSize(CELL_W, CELL_H);
}

CCTableViewCell* TerritoryInfoDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    TerritoryInfoDetailCell* cell = (TerritoryInfoDetailCell*)table->dequeueCell();
    YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,m_stat);
        }else{
            cell = TerritoryInfoDetailCell::create(info,m_infoList,m_stat,m_cityInfo.cityIndex,m_cityInfo.cityType,m_ownerFlag);
        }
    }
    return cell;
}

ssize_t TerritoryInfoDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void TerritoryInfoDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

TerritoryInfoDetailCell *TerritoryInfoDetailCell::create(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type,bool owner){
    TerritoryInfoDetailCell *ret = new TerritoryInfoDetailCell(info,clickNode,stat,targetIndex,type,owner);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryInfoDetailCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void TerritoryInfoDetailCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool TerritoryInfoDetailCell::init(){
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto node = CCBLoadFile("TerritoryInfoDetailCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    setData(m_info,m_stat);
    return true;
}

void TerritoryInfoDetailCell::setData(YuanJunInfo* info,int stat){
    m_stat = stat;
    m_info = info;
    
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_nameTxt->setString(m_info->getName().c_str());
    std::string numStr = _lang("108557");
    numStr.append(":");
    numStr.append(CC_CMDITOA(m_info->getNum()));
    m_armyNum->setString(numStr);
//    m_bgNodee->setVisible(true);
    m_unUseNode->setVisible(false);
    m_moveNode->setVisible(false);
    m_scienceNode->setVisible(false);
    m_resNode->setVisible(false);
    m_renderBg->setVisible(false);
    if(m_info->getIndex()==2){
        switch (m_stat) {
            case 0:
            case 1: {
                if (m_cityType == Tile_allianceArea) {
                    m_tipTxt->setString(_lang("115328"));
                }
                else if (m_cityType == tile_tower) {
                    m_tipTxt->setString(_lang("115408"));
                }
                else if (m_cityType == tile_wareHouse) {
                    m_tipTxt->setString(_lang("115447"));
                }
                else if (m_cityType == tile_banner) {
                    m_tipTxt->setString(_lang("115525"));
                }
            }
                break;
            case 2:
            case 3: {
                if (m_cityType == Tile_allianceArea) {
                    m_tipTxt->setString(_lang("115330"));
                }
                else if (m_cityType == tile_banner) {
                    m_tipTxt->setString(_lang("115528"));
                }
            }
                break;
            case 4:
            case 5: {
                if (m_cityType == Tile_allianceArea) {
                    m_tipTxt->setString(_lang("115329"));
                }
                else if (m_cityType == tile_banner) {
                    m_tipTxt->setString(_lang("115533"));
                }
            }
                break;
            default:
                break;
        }
        m_unUseNode->setVisible(true);
    }else if(m_info->getIndex()==3){
        m_scienceNode->setVisible(true);
//        m_bgNodee->setVisible(false);
        m_scienceTxt->setString(_lang("115146"));
    }else if(m_info->getIndex()==4){
        m_tipTxt->setString(_lang("115403"));
        m_unUseNode->setVisible(true);
        CCCommonUtils::setSpriteGray(m_joinNode, true);
    }else{
        m_moveNode->setVisible(true);
    }
    //    m_soldierNode->removeAllChildrenWithCleanup(true);
    std::string useArmyId = "";
    int num = m_info->getSoldiers()->count();
    int maxCount = 0;
    int count = 0;
    for(int i=0; i<num; i++){
        auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
        count = dic->valueForKey("count")->intValue();
        std::string armyId = dic->valueForKey("armyId")->getCString();
        if(count>maxCount){
            maxCount = count;
            useArmyId = armyId;
        }
    }
    
    const int OPEN_BG_TAG = 10010;
    m_renderBg->getParent()->removeChildByTag(OPEN_BG_TAG);
    
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(m_info->getOpen()){
        m_renderBg->setVisible(true);
        num = m_info->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = 0;
        if (m_owner && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && m_info->getUid() != GlobalData::shared()->playerInfo.uid) {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 120;
            }
            else
                addH = row*100 + 70;
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 40;
            }
            else
                addH = row*100 + 70;
        }
        
        m_moveNode->setPosition(ccp(0, addH));
        m_renderBg->setPosition(ccp(0, addH));
        m_bgNodee->setPosition(ccp(0, addH));
        maxCount = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_renderBg->setContentSize(CCSize(1470, addH));
        }
        else
            m_renderBg->setContentSize(CCSize(CELL_W, addH));
        m_renderBg->removeAllChildrenWithCleanup(true);
        Scale9Sprite* sp = CCLoadSprite::createScale9Sprite("nb_al_members_bg.png");
        sp->setTag(OPEN_BG_TAG);
        sp->setZOrder(-OPEN_BG_TAG);
        sp->setContentSize({CELL_W - 30.0f, addH + 10.0f});
        sp->setPosition({m_renderBg->getPositionX() + CELL_W / 2.0f, m_renderBg->getPositionY() - addH + addH / 2.0f + 15.f});
        m_renderBg->getParent()->addChild(sp);
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
            int rowIndex = i/2;
            int col = i%2;
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(col==0?40:680, m_renderBg->getContentSize().height-220-rowIndex*200));
            }
            else
                cell->setPosition(ccp(col == 0 ? 33 : 327, m_renderBg->getContentSize().height - 122 - rowIndex * 128));
            
            if (rowIndex % 2 == 1)
            {
                cell->m_bg->setScaleY(-1);
            }
            
            m_renderBg->addChild(cell);
            count = dic->valueForKey("count")->intValue();
            std::string armyId = dic->valueForKey("armyId")->getCString();
            if(count>maxCount){
                maxCount = count;
                useArmyId = armyId;
            }
        }
        if (m_owner && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && m_info->getUid() != GlobalData::shared()->playerInfo.uid && m_info->getAllianceId() == GlobalData::shared()->playerInfo.allianceInfo.uid)
        {
            CCControlButton* button = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
            button->setTitleForState(_lang("115157"), CCControlStateNormal);
            button->setTitleForState(_lang("115157"), CCControlStateHighlighted);
            button->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryInfoDetailCell::onQianfanClick), CCControlEventTouchUpInside);
            button->setPreferredSize(CCSizeMake(250, 70));
            button->setAnchorPoint(ccp(0.5, 0.5));
            if (CCCommonUtils::isIosAndroidPad()) {
                button->setScale(1.2f);
                button->setPosition(ccp(735, 60));
            }
            else {
                button->setScale(0.6f);
                button->setPosition(ccp(CELL_W / 2, 30));
            }
            m_renderBg->addChild(button);
        }
    }else{
        m_moveNode->setPosition(ccp(0, 0));
        m_bgNodee->setPosition(ccp(0, 0));
        //        if(!m_info->getCanUse()){
        //            m_tipTxt->setColor({120,120,120});
        //
        //        }else{
        //            m_tipTxt->setColor({169,183,189});
        //        }
//        m_tipTxt->setColor({169,183,189});
        m_renderBg->setVisible(false);
    }
    if(m_info->getGenerals() && m_info->getGenerals()->count()>0){
        std::string head = m_info->getPic();
        if(head==""||head=="0"){
            head = "g044";
        }
        head.append(".png");
        std::string uid = m_info->getUid();
        int picVer = m_info->getpicVer();
        m_icon->removeAllChildrenWithCleanup(true);
        CCSprite* headSpr = CCLoadSprite::createSprite(head.c_str());
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
        if (CCCommonUtils::isUseCustomPic(picVer))
        {
            m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 78, true);
        }
    }else{
        m_icon->removeAllChildrenWithCleanup(true);
        std::string icon = "ico" + useArmyId+ "_small.png";
        CCSprite* headSpr = CCLoadSprite::createSprite(icon.c_str());
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
    }
    m_arrow->setRotation(m_info->getOpen()?270:180);
    
    if(haveTime<=0){
        switch (m_stat) {
            case 0:
                m_statusTxt->setString(_lang("115305"));
                break;
            case 2:
                m_statusTxt->setString(_lang("115346"));
                break;
            case 5:
                m_statusTxt->setString(_lang("115310"));
                break;
            case 6:
                m_statusTxt->setString(_lang("115347"));
                break;
            default:
                break;
        }
    }else{
        m_statusTxt->setString(_lang("115351"));
        this->unschedule(schedule_selector(TerritoryInfoDetailCell::updateTime));
        this->schedule(schedule_selector(TerritoryInfoDetailCell::updateTime),1);
        this->updateTime(0);
    }
}

void TerritoryInfoDetailCell::onQianfanClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    AllianceTerritoryQianfanCommand* cmd = new AllianceTerritoryQianfanCommand(m_info->getUuid());
    cmd->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_INFO_QIANFAN, ccs(m_info->getUid()));
}

void TerritoryInfoDetailCell::updateTime(float _time){
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(haveTime<=0){
        this->unschedule(schedule_selector(TerritoryInfoDetailCell::updateTime));
        TerritoryInfoDetailView* tInfoView = dynamic_cast<TerritoryInfoDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
        if (tInfoView) {
            tInfoView->requestDetail();
        }
        return ;
    }
}
bool TerritoryInfoDetailCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void TerritoryInfoDetailCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && (isTouchInside(m_clickNode, pTouch) || isTouchInside(m_arrow, pTouch)) && fabsf(end.y - m_touchPos.y)<30){
        m_clickFlag = !m_clickFlag;
        if(m_info->getIndex()==2){
            if (!m_info->getCanUse()) {
                unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
                unsigned int max = WorldController::getInstance()->getMaxMarchCount();
                if (current >= max) {
                    WorldController::getInstance()->showMarchAlert(max);
                    return;
                }
                else
                    attendRally();
            }
            else
                attendRally();
        }else if(m_info->getIndex()==3){
            //TODO: guojiang 联盟
            CCCommonUtils::flyHint("", "", _lang("E100008"));
            return;
            
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
        }else if(m_info->getIndex()==4){
            return;//////////////
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_INFO_DETAIL_CELL_CLICK,CCString::create(m_info->getUuid().c_str()));
        }
    }
}

void TerritoryInfoDetailCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void TerritoryInfoDetailCell::attendRally()
{
    WorldController::getInstance()->openMarchDeploy(m_targetIndex,0);
}

bool TerritoryInfoDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodTxt", CCLabelIF*, this->m_woodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodTxt", CCLabelIF*, this->m_foodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironTxt", CCLabelIF*, this->m_ironTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneTxt", CCLabelIF*, this->m_stoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg", CCScale9Sprite*, this->m_renderBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unUseNode", CCNode*, this->m_unUseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceNode", CCNode*, this->m_scienceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNodee", CCSprite*, this->m_bgNodee);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceTxt", CCLabelIF*, this->m_scienceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinNode", CCSprite*, this->m_joinNode);
    return false;
}

SEL_CCControlHandler TerritoryInfoDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
string TerritoryInfoDetailView::getTuid(){
    string backStr = m_cityInfo.m_allianceAreaInfo.tuid;
    if (backStr == "") {
        int parentCityIndex = m_cityInfo.parentCityIndex;
        backStr =   WorldController::getInstance()->getCityInfos()[parentCityIndex].m_allianceAreaInfo.tuid;
    }
    return backStr;
}