//
//  TerritoryResourceDetailView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/18.
//
//

#include "TerritoryResourceDetailView.h"
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
#define  MSG_TERRITORY_RES_DETAIL_CELL_CLICK       "msg_territory_res_detail_cell_click"
#define  MSG_TERRITORY_RES_QIANFAN             "msg_territory_res_qianfan"
TerritoryResourceDetailView::TerritoryResourceDetailView(WorldCityInfo& info):m_cityInfo(info){
};

TerritoryResourceDetailView::~TerritoryResourceDetailView(){
};

TerritoryResourceDetailView *TerritoryResourceDetailView::create(WorldCityInfo& info){
    TerritoryResourceDetailView *ret = new TerritoryResourceDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryResourceDetailView::onEnter(){
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    m_tabView->reloadData();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryResourceDetailView::onCellClick), MSG_TERRITORY_RES_DETAIL_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryResourceDetailView::qianFanCallBack), MSG_TERRITORY_RES_QIANFAN, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void TerritoryResourceDetailView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_RES_DETAIL_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_RES_QIANFAN);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool TerritoryResourceDetailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryResourceDetailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId) && isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=3) {
        CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
    }
}

bool TerritoryResourceDetailView::init(){
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        setIsHDPanel(true);
        m_data = CCArray::create();
        
        requestDetail();
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
        addLoadingAni();
        
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
        
//        ParticleFireAni* par = ParticleFireAni::create();//fusheng 报错
//        m_fireNode1->addChild(par);
//        
//        ParticleFireAni* par2 = ParticleFireAni::create();
//        m_fireNode2->addChild(par2);
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        //        m_titleTxt->setString(_lang("115331"));
        std::string picStr = "";
        switch (m_cityInfo.m_superMineInfo.type) {
            case Food:
                picStr = "territory_resource_food.png";
                break;
            case Wood:
                picStr = "territory_resource_wood.png";
                break;
            case Iron:
                picStr = "territory_resource_iron.png";
            case Stone:
                picStr = "territory_resource_stone.png";
            default:
                break;
        }
        auto picSprite = CCLoadSprite::createSprite(picStr.c_str());
        if (picSprite) {
            if (CCCommonUtils::isIosAndroidPad()) {
                picSprite->setScale(1.5);
            }
            m_picNode->addChild(picSprite);
        }
        
        if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
            removeLoadingAni();
            m_territoryFlag = false;
            refreshView();
        }
        m_isInUpdate = false;
        return true;
    }
    
    return false;
}

void TerritoryResourceDetailView::requestDetail() {
    m_isInUpdate = true;
    unsigned index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        auto cmd = new AllianceTerritoryDetailCommand(index);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryResourceDetailView::onDetailCallback), nullptr));
        cmd->sendAndRelease();
    }
}

void TerritoryResourceDetailView::onDetailCallback(cocos2d::CCObject *obj) {
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
        m_cityInfo.m_superMineInfo.trstat = (AAreaState)info->valueForKey("stat")->intValue();
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
        m_cityInfo.m_superMineInfo.trdef = info->valueForKey("curDef")->intValue();
    }
    if (info->objectForKey("maxDef")) {
        m_cityInfo.m_superMineInfo.trdefmax = info->valueForKey("maxDef")->intValue();
    }
    if (info->objectForKey("updateTime")) {
        m_cityInfo.m_superMineInfo.trutime = info->valueForKey("updateTime")->doubleValue() / 1000;
    }
    if (info->objectForKey("startTime")) {
        if (m_cityInfo.m_superMineInfo.trstat == 0 || m_cityInfo.m_superMineInfo.trstat == 1) {
            m_cityInfo.m_superMineInfo.trstime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
        else if (m_cityInfo.m_superMineInfo.trstat == 6 || m_cityInfo.m_superMineInfo.trstat == 3 || m_cityInfo.m_superMineInfo.trstat == 4) {
            m_cityInfo.m_superMineInfo.digStartTime = info->valueForKey("startTime")->doubleValue() / 1000;
        }
    }
    if (info->objectForKey("buildingSpd")) {
        if (m_cityInfo.m_superMineInfo.trstat == 0 || m_cityInfo.m_superMineInfo.trstat == 1) {
            m_cityInfo.m_superMineInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
        else if (m_cityInfo.m_superMineInfo.trstat == 6 || m_cityInfo.m_superMineInfo.trstat == 3 || m_cityInfo.m_superMineInfo.trstat == 4) {
            m_cityInfo.m_superMineInfo.digSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
        }
    }
    if (info->objectForKey("name")) {
        m_cityInfo.m_aArea_nickname = info->valueForKey("name")->getCString();
    }
    if (!info->objectForKey("name")) {
        switch (m_cityInfo.m_superMineInfo.type) {
            case Food:
                m_cityInfo.m_aArea_nickname = _lang("115376");
                break;
            case Wood:
                m_cityInfo.m_aArea_nickname = _lang("115377");
                break;
            case Iron:
                m_cityInfo.m_aArea_nickname = _lang("115378");
                break;
            case Stone:
                m_cityInfo.m_aArea_nickname = _lang("115379");
                break;
            default:
                break;
        }
    }
    if (info->objectForKey("allianceAbbr")) {
        m_cityInfo.m_aArea_simpleName = info->valueForKey("allianceAbbr")->getCString();
    }
    if (info->objectForKey("allianceId")) {
        std::string allianceId = info->valueForKey("allianceId")->getCString();
        if (allianceId.compare(GlobalData::shared()->playerInfo.allianceInfo.uid)==0) {
            m_ownerFlag = true;
        }
        else
            m_ownerFlag = false;
    }
    
    m_data->removeAllObjects();
    m_territoryFlag = true;
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
                    m_marchId = info->getUuid();
                    auto &marchInfo = WorldController::getInstance()->m_marchInfo[m_marchId];
                    m_digStartTime = marchInfo.digStartTime / 1000;
                    m_digStartNum = marchInfo.digStartNum;
                    m_digSpeed = marchInfo.digSpeed;
                    m_troopMax = marchInfo.troopMax;
                    m_changeCollectSpdTime = marchInfo.changeCollectSpdTime / 1000;
                }
                info->release();
            }
        }
    }
    
    if (tempFlag) {
        m_digStartTime = 0;
        m_digStartNum = 0;
        m_digSpeed = 0;
        m_troopMax = 0;
        m_changeCollectSpdTime = 0;
    }
    
    if (m_cityInfo.m_superMineInfo.trstat == 1 || m_cityInfo.m_superMineInfo.trstat == 0) {
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
    else if (m_cityInfo.m_superMineInfo.trstat == 3 || m_cityInfo.m_superMineInfo.trstat == 4) {
        if (m_data->count() == 0) {
            YuanJunInfo* info = new YuanJunInfo();
            CCArray* array = CCArray::create();
            info->setSoldiers(array);
            info->setCanUse(true);
            info->setIndex(2);
            m_data->addObject(info);
            info->release();

        }
    }

    refreshView();
}

void TerritoryResourceDetailView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void TerritoryResourceDetailView::addLoadingAni(){
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

void TerritoryResourceDetailView::refreshView() {
    m_territoryStat = m_cityInfo.m_superMineInfo.trstat;
    
    if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {
        m_withdrawBtn->setEnabled(true);
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
            m_tipsLabel->setString(_lang("115321"));
            std::string strDef = CC_ITOA(m_cityInfo.m_superMineInfo.trdef);
            strDef.append("/");
            strDef.append(CC_ITOA(m_cityInfo.m_superMineInfo.trdefmax));
            m_defLabel1->setString(_lang_1("115345",""));
            m_defLabel2->setString(strDef);
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
            m_tipsLabel->setString(_lang_1("115479", m_cityInfo.m_aArea_nickname.c_str()));
            std::string strDef = CC_ITOA(m_cityInfo.m_superMineInfo.trdef);
            strDef.append("/");
            strDef.append(CC_ITOA(m_cityInfo.m_superMineInfo.trdefmax));
            m_defLabel1->setString(_lang_1("115345",""));
            m_defLabel2->setString(strDef);
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
        case 4:
        case 3: {
            strStat = _lang("115381");
            m_statLabel->setString(strStat);
            std::string strDef = CC_CMDITOA(double(m_cityInfo.m_superMineInfo.sum));
            m_defLabel1->setString(_lang("115382"));
            m_defLabel2->setString(strDef);
            m_tipsLabel->setString(_lang("115481"));
            m_timeLabel1->setString(_lang_1("115491",""));
            int speed = 0;
            m_timeLabel2->setString(CC_CMDITOA(0));
            m_speedAddText->setVisible(false);
            m_troopLabel1->setString(_lang_1("108868", ""));
            m_troopLabel2->setString(CC_CMDITOA(0));
            m_stat = 4;
        }
            break;
        case 6: {
            strStat = _lang("115380");
            m_statLabel->setString(strStat);
            std::string strDef = CC_CMDITOA(double(m_cityInfo.m_superMineInfo.sum));
            m_defLabel1->setString(_lang("115382"));
            m_defLabel2->setString(strDef);
            m_tipsLabel->setString(_lang("115481"));
            m_timeLabel1->setString(_lang_1("115491",""));
            int speed = round(m_digSpeed * 3600);
            int addSpeed = round(speed - speed / (1.0 + CCCommonUtils::getEffectValueByNum(65) / 100.0));
            m_timeLabel2->setString(CC_CMDITOA(speed - addSpeed));
            if(addSpeed != 0){
                m_speedAddText->setVisible(true);
                m_speedAddText->setString(std::string("+ ") + CC_CMDITOA(addSpeed) + "/h");
                m_speedAddText->setColor({52,200,13});
                float sizeWidth = m_timeLabel2->getContentSize().width;
                m_speedAddText->setPositionX(m_timeLabel2->getPositionX() + sizeWidth * m_timeLabel2->getOriginScaleX() + 10);
            }
            else
                m_speedAddText->setVisible(false);
            m_troopLabel1->setString(_lang_1("108868", ""));
            m_troopLabel2->setString(CC_CMDITOA(m_digStartNum));
            m_stat = 6;
        }
            break;
        default:
            break;
    }
    
    m_tabView->reloadData();
}

void TerritoryResourceDetailView::refreshOnce() {
    m_territoryStat = m_cityInfo.m_superMineInfo.trstat;
    
    m_statLabel->setColor({243,69,0});
    std::string strStat = "";//状态
    switch (m_territoryStat) {
        case 0: {
            strStat =  _lang("115305");
            m_statLabel->setString(strStat);
            m_tipsLabel->setString(_lang("115321"));
            std::string strDef = CC_ITOA(m_cityInfo.m_superMineInfo.trdef);
            strDef.append("/");
            strDef.append(CC_ITOA(m_cityInfo.m_superMineInfo.trdefmax));
            m_defLabel1->setString(_lang_1("115345",""));
            m_defLabel2->setString(strDef);
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
            m_tipsLabel->setString(_lang_1("115479", m_cityInfo.m_aArea_nickname.c_str()));
            std::string strDef = CC_ITOA(m_cityInfo.m_superMineInfo.trdef);
            strDef.append("/");
            strDef.append(CC_ITOA(m_cityInfo.m_superMineInfo.trdefmax));
            m_defLabel1->setString(_lang_1("115345",""));
            m_defLabel2->setString(strDef);
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
        case 4:
        case 3: {
            strStat = _lang("115381");
            m_statLabel->setString(strStat);
            std::string strDef = CC_CMDITOA(double(m_cityInfo.m_superMineInfo.sum));
            m_defLabel1->setString(_lang("115382"));
            m_defLabel2->setString(strDef);
            m_tipsLabel->setString(_lang("114481"));
            m_timeLabel1->setString(_lang_1("108869",""));
            int speed = 0;
            m_timeLabel2->setString(CC_CMDITOA(0));
            m_speedAddText->setVisible(false);
            m_troopLabel1->setString(_lang_1("108868", ""));
            m_troopLabel2->setString(CC_CMDITOA(0));
            m_stat = 4;
        }
            break;
        case 6: {
            strStat = _lang("115380");
            m_statLabel->setString(strStat);
            std::string strDef = CC_CMDITOA(double(m_cityInfo.m_superMineInfo.sum));
            m_defLabel1->setString(_lang("115382"));
            m_defLabel2->setString(strDef);
            m_tipsLabel->setString(_lang("114481"));
            m_timeLabel1->setString(_lang_1("108869",""));
            int speed = round(m_digSpeed * 3600);
            int addSpeed = round(speed - speed / (1.0 + CCCommonUtils::getEffectValueByNum(65) / 100.0));
            m_timeLabel2->setString(CC_CMDITOA(speed - addSpeed));
            if(addSpeed != 0){
                m_speedAddText->setVisible(true);
                m_speedAddText->setString(std::string("+ ") + CC_CMDITOA(addSpeed) + "/h");
                m_speedAddText->setColor({52,200,13});
                float sizeWidth = m_timeLabel2->getContentSize().width;
                m_speedAddText->setPositionX(m_timeLabel2->getPositionX() + sizeWidth * m_timeLabel2->getOriginScaleX() + 10);
            }
            else
                m_speedAddText->setVisible(false);
            m_troopLabel1->setString(_lang_1("108868", ""));
            m_troopLabel2->setString(CC_CMDITOA(m_digStartNum));
            m_stat = 6;
        }
            break;
        default:
            break;
    }
}

void TerritoryResourceDetailView::update(float sec) {
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

    if (WorldController::getInstance()->forceUpdateTerritoryInfo == true) {
        refreshOnce();
        WorldController::getInstance()->forceUpdateTerritoryInfo = false;
    }
    
    double now = GlobalData::shared()->getTimeStamp();
    //设置联盟建筑当前城防值
    switch (m_territoryStat) {
        case 0: {
            double timePass = now - m_cityInfo.m_superMineInfo.trstime;
            double defTemp = m_cityInfo.m_superMineInfo.buildSpeed * timePass;
            auto currentDef = m_cityInfo.m_superMineInfo.trdef + defTemp;
            if (currentDef>=m_cityInfo.m_superMineInfo.trdefmax) {
                currentDef = m_cityInfo.m_superMineInfo.trdefmax;
            }
            std::string strDef = CC_ITOA(currentDef);
            strDef.append("/");
            strDef.append(CC_ITOA(m_cityInfo.m_superMineInfo.trdefmax));
            m_defLabel2->setString(strDef);
            std::string timeStr = _lang("115305");
            timeStr.append(": ");
            m_timeLabel1->setString(timeStr);
            double timeLeft = m_cityInfo.m_superMineInfo.trutime - now;
            timeLeft = MAX(0.0f, timeLeft);
            m_timeLabel2->setString(CC_SECTOA(timeLeft));
        }
            break;
        case 6: {
            auto time = now - m_digStartTime;
            int currentNum = m_digSpeed * time;
            if(m_changeCollectSpdTime != 0){
                time = now - m_changeCollectSpdTime;
                currentNum = m_digStartNum + m_digSpeed * time;
            }
            currentNum = MAX(0.0f, currentNum);
            currentNum = MIN(m_troopMax, currentNum);
            m_troopLabel2->setString(CC_CMDITOA(currentNum));
            double timePass = now - m_cityInfo.m_superMineInfo.digStartTime;
            double resTemp = m_cityInfo.m_superMineInfo.digSpeed * timePass;
            double resLeft = m_cityInfo.m_superMineInfo.sum - resTemp;
            resLeft = MAX(0.0f, resLeft);
            m_defLabel2->setString(CC_CMDITOA(double(resLeft)).c_str());
        }
            break;
        default:
            break;
    }
}

void TerritoryResourceDetailView::onCellClick(CCObject* data){
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

void TerritoryResourceDetailView::qianFanCallBack(CCObject *data)
{
    CCString* uid = dynamic_cast<CCString*>(data);
    if (uid && m_cityInfo.m_superMineInfo.trstat == 0) {
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
        m_data->insertObject(info, num-2);
        //        m_data->addObject(info);
        info->release();
        m_tabView->reloadData();
    }
}

void TerritoryResourceDetailView::onTipBtnClick(CCObject *pSender, CCControlEvent event){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
    //    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");
}

void TerritoryResourceDetailView::onWithdrawClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::marchAlertShow(_lang_1("115344", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryResourceDetailView::secondConfirm)),NULL);
}

void TerritoryResourceDetailView::secondConfirm(){
    YesNoDialog::marchAlertShow(_lang_1("115355", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryResourceDetailView::withdraw)),NULL);
}

void TerritoryResourceDetailView::withdraw(){
    auto cmd = new WorldTerritoryResCancelCommand(m_cityInfo.m_superMineInfo.truid,m_cityInfo.parentCityIndex,m_cityInfo.m_superMineInfo.type);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

bool TerritoryResourceDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statLabel", CCLabelIF*, this->m_statLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel1", CCLabelIF*, this->m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel1", CCLabelIF*, this->m_troopLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel2", CCLabelIF*, this->m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedAddText", CCLabelIF*, this->m_speedAddText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel2", CCLabelIF*, this->m_troopLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    return false;
}

SEL_CCControlHandler TerritoryResourceDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawClick", TerritoryResourceDetailView::onWithdrawClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", TerritoryResourceDetailView::onTipBtnClick);
    return NULL;
}

CCSize TerritoryResourceDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if (obj->getOpen()) {
        int num = obj->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = row*100;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = row*200;
        }
        if (m_ownerFlag && obj->getUid() != GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && m_cityInfo.m_superMineInfo.trstat == 0) {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+120);
            }
            return CCSize(620, 125+addH+60);
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+40);
            }
            return CCSize(620, 125+addH+20);
        }
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCSize TerritoryResourceDetailView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCTableViewCell* TerritoryResourceDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    TerritoryResourceDetailCell* cell = (TerritoryResourceDetailCell*)table->dequeueCell();
    YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,m_stat);
        }else{
            cell = TerritoryResourceDetailCell::create(info,m_infoList,m_stat,m_cityInfo.cityIndex,m_cityInfo.m_superMineInfo.type,m_ownerFlag);
        }
    }
    return cell;
}

ssize_t TerritoryResourceDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void TerritoryResourceDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

TerritoryResourceDetailCell *TerritoryResourceDetailCell::create(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldResourceType type,bool owner){
    TerritoryResourceDetailCell *ret = new TerritoryResourceDetailCell(info,clickNode,stat,targetIndex,type,owner);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryResourceDetailCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void TerritoryResourceDetailCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool TerritoryResourceDetailCell::init(){
    auto node = CCBLoadFile("TerritoryInfoDetailCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    setData(m_info,m_stat);
    return true;
}

void TerritoryResourceDetailCell::setData(YuanJunInfo* info,int stat){
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
    if(m_info->getIndex()==2){
        std::string resName = "";
        switch (m_resType) {
            case Food:
                resName = _lang("115376");
                break;
            case Wood:
                resName = _lang("115377");
                break;
            case Iron:
                resName = _lang("115378");
                break;
            case Stone:
                resName = _lang("115379");
                break;
            default:
                break;
        }
        switch (m_stat) {
            case 0:
            case 1:
                m_tipTxt->setString(_lang_1("115480", resName.c_str()));
                break;
            case 3:
            case 4:
                m_tipTxt->setString(_lang_1("115482", resName.c_str()));
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
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(m_info->getOpen()){
        num = m_info->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = 0;
        if (m_owner && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && m_info->getUid() != GlobalData::shared()->playerInfo.uid) {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 120;
            }
            else
                addH = row*100 + 60;
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 40;
            }
            else
                addH = row*100 + 20;
        }
        
        m_moveNode->setPosition(ccp(0, addH));
        m_renderBg->setPosition(ccp(0, addH));
        m_bgNodee->setPosition(ccp(0, addH));
        maxCount = 0;
        m_renderBg->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_renderBg->setContentSize(CCSize(1470, addH));
        }
        else
            m_renderBg->setContentSize(CCSize(620,addH));
        m_renderBg->removeAllChildrenWithCleanup(true);
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
            int rowIndex = i/2;
            int col = i%2;
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(col==0?40:680, m_renderBg->getContentSize().height-220-rowIndex*200));
            }
            else
                cell->setPosition(ccp(col==0?20:340, m_renderBg->getContentSize().height-110-rowIndex*100));
            m_renderBg->addChild(cell);
            count = dic->valueForKey("count")->intValue();
            std::string armyId = dic->valueForKey("armyId")->getCString();
            if(count>maxCount){
                maxCount = count;
                useArmyId = armyId;
            }
        }
        if (m_owner && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && m_info->getUid() != GlobalData::shared()->playerInfo.uid && m_stat == 0) {
            CCControlButton* button = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
            button->setTitleForState(_lang("115157"), CCControlStateNormal);
            button->setTitleForState(_lang("115157"), CCControlStateHighlighted);
            button->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryResourceDetailCell::onQianfanClick), CCControlEventTouchUpInside);
            
            button->setPreferredSize(CCSizeMake(250, 70));
            button->setAnchorPoint(ccp(0.5, 0.5));
            if (CCCommonUtils::isIosAndroidPad()) {
                button->setScale(1.2);
                button->setPosition(ccp(735, 60));
            }
            else {
                button->setScale(0.6f);
                button->setPosition(ccp(310, 30));
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
        m_tipTxt->setColor({169,183,189});
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
            case 6:
                m_statusTxt->setString(_lang("115380"));
                break;
            default:
                break;
        }
    }else{
        m_statusTxt->setString(_lang("115351"));
        this->unschedule(schedule_selector(TerritoryResourceDetailCell::updateTime));
        this->schedule(schedule_selector(TerritoryResourceDetailCell::updateTime),1);
        this->updateTime(0);
    }
}

void TerritoryResourceDetailCell::onQianfanClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    AllianceTerritoryQianfanCommand* cmd = new AllianceTerritoryQianfanCommand(m_info->getUuid());
    cmd->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_RES_QIANFAN, ccs(m_info->getUid()));
}

void TerritoryResourceDetailCell::updateTime(float _time){
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(haveTime<=0){
        this->unschedule(schedule_selector(TerritoryResourceDetailCell::updateTime));
        TerritoryResourceDetailView* tInfoView = dynamic_cast<TerritoryResourceDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
        if (tInfoView) {
            tInfoView->requestDetail();
        }
        return ;
    }
}
bool TerritoryResourceDetailCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void TerritoryResourceDetailCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_clickNode, pTouch) && fabsf(end.y - m_touchPos.y)<30){
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
        }else if(m_info->getIndex()==3)
        {
            //TODO: guojiang 联盟
            CCCommonUtils::flyHint("", "", _lang("E100008"));
            return;
            
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
        }
        else if(m_info->getIndex()==4){
            return;//////////////
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_RES_DETAIL_CELL_CLICK,CCString::create(m_info->getUuid().c_str()));
        }
    }
}

void TerritoryResourceDetailCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void TerritoryResourceDetailCell::attendRally()
{
    WorldController::getInstance()->openMarchDeploy(m_targetIndex,0);
}

bool TerritoryResourceDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
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

SEL_CCControlHandler TerritoryResourceDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}