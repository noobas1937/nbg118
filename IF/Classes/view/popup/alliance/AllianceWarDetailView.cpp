//
//  AllianceWarDetailView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-7.
//
//

#include "AllianceWarDetailView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldController.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "CancelAllianceTeamCommand.h"
#include "AllianceFlagPar.h"
#include "AllianceManager.h"
#include "UseCDToolView.h"
#include "WorldCommand.h"
#include "AllianceScienceView.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "NBWorldMapMainCity.hpp"

#define  MSG_ALLIANCE_WAR_DETAIL_CELL_CLICK       "msg_alliance_war_detail_cell_click"

static int teamPlayerNum = 0;
static int teamBattleType = 0;

#include "YuanYunReturnCommand.h"
#include "EnemyInfoController.h"

const int CELL_W = 640;
const int CELL_H = 104;

AllianceWarDetailView::AllianceWarDetailView(AllianceTeamInfo* info):m_info(info),m_isLeaderUser(false){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceWarDetailView::updateArriveTime), MSG_CHANGE_ALLIANCE_TEAM_ARRIVE_TIME, NULL);
};

AllianceWarDetailView::~AllianceWarDetailView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_TEAM_ARRIVE_TIME);
};

AllianceWarDetailView *AllianceWarDetailView::create(AllianceTeamInfo* info){
    AllianceWarDetailView *ret = new AllianceWarDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceWarDetailView::onEnter(){
    this->setTitleName(_lang("115138"));
    PopupBaseView::onEnter();
    
//    m_tabView->reloadData();
//    auto pt = m_tabView->minContainerOffset();
//    m_tabView->setContentOffset(pt);
//    m_tabView->updateCellAtIndex(0);
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceWarDetailView::onCellClick), MSG_ALLIANCE_WAR_DETAIL_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceWarDetailView::qianFanCallBack), MSG_ALLIANCE_QIANFAN, NULL);
}

void AllianceWarDetailView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_WAR_DETAIL_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_QIANFAN);
    PopupBaseView::onExit();
}

bool AllianceWarDetailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_posNode1, pTouch)){
        return true;
    }
    return false;
}

void AllianceWarDetailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_posNode1, pTouch)){
        m_posNode1->stopAllActions();
        CCFadeTo* fade = CCFadeTo::create(0.25, 110);
        CCFadeTo* fade4 = CCFadeTo::create(0.2, 2);
        CCDelayTime* de = CCDelayTime::create(0.05);
        CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(AllianceWarDetailView::goToWorld));
        CCSequence* sc = CCSequence::create(fade,fade4,de,fun,NULL);
        m_posNode1->runAction(sc);
    }
}

void AllianceWarDetailView::qianFanCallBack(cocos2d::CCObject *data)
{
    string uid = dynamic_cast<CCString*>(data)->getCString();
    YuanJunInfo* info = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_data, obj)
    {
        info = dynamic_cast<YuanJunInfo*>(obj);
        if (info && info->getUid() == uid)
        {
            m_data->removeObject(obj);
            break;
        }
    }
    m_tabView->reloadData();
}

void AllianceWarDetailView::goToWorld(){
    int index  = m_info->getTargetId();
    CCPoint pt = WorldController::getPointByIndex(index);
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
        WorldMapView::instance()->openTilePanel(index);
    }else{
        WorldController::getInstance()->openTargetIndex = index;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
}

bool AllianceWarDetailView::init(){
    bool ret = false;
    teamPlayerNum = 0;
    teamBattleType = 0;
    if(PopupBaseView::init()){
        
        CCLoadSprite::doResourceByCommonIndex(204, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        });
        setIsHDPanel(true);
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(0, curHight));
//            if (CCCommonUtils::isIosAndroidPad()) {
//                bg->setScaleX(2.4);
//            }
//            curHight += bg->getContentSize().height;
//            tBatchNode->addChild(bg);
//        }
//        this->addChild(tBatchNode);
        
        auto node = CCBLoadFile("AllianceWarDetailView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
            if(m_info->getAttackUid()==GlobalData::shared()->playerInfo.uid){
                if(m_info->getBattleType()==2){
                    m_bottomNode->setVisible(true);
                }else{
                    m_bottomNode->setVisible(false);
                }
            }else if(m_info->getBattleType()==2){
                m_bottomNode->setVisible(false);
            }else if(m_info->getTargetUid()==GlobalData::shared()->playerInfo.uid){
                m_bottomNode->setVisible(false);
            }else{
                m_bottomNode->setVisible(true);
            }
        }
        else {
            this->setContentSize(node->getContentSize());
            int preHeight = this->m_viewBg->getContentSize().height;
            changeBGHeight(m_viewBg);
            m_viewBg->setVisible(false);
            int dh = m_viewBg->getContentSize().height - preHeight;
            if(m_info->getAttackUid()==GlobalData::shared()->playerInfo.uid){
                if(m_info->getBattleType()==2){
                    dh += 0;
                    m_bottomNode->setVisible(true);
                }else{
                    dh += 100;
                    m_bottomNode->setVisible(false);
                }
            }else if(m_info->getBattleType()==2){
                dh += 100;
                m_bottomNode->setVisible(false);
            }else if(m_info->getTargetUid()==GlobalData::shared()->playerInfo.uid){
                dh += 100;
                m_bottomNode->setVisible(false);
            }else{
                dh += 0;
                m_bottomNode->setVisible(true);
            }
            this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
            m_infoList->setPositionY(m_infoList->getPositionY()-dh);
            
            m_bottomNode->setPositionY(m_bottomNode->getPositionY()-dh);
        }
        
//        ParticleFireAni* par = ParticleFireAni::create();
//        m_fireNode1->addChild(par);
        
//        ParticleFireAni* par2 = ParticleFireAni::create();
//        m_fireNode2->addChild(par2);
        
        CCCommonUtils::setButtonTitle(m_jieSanBtn, _lang("115191").c_str());
        CCCommonUtils::setButtonTitle(m_attackBtn, _lang("115193").c_str());
        CCCommonUtils::setButtonTitle(m_supportBtn, _lang("115151").c_str());
        CCCommonUtils::setButtonTitle(m_goHomeBtn, _lang("115157").c_str());

        m_data = CCArray::create();
        CCArray* array = CCArray::create();
       
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        teamBattleType = m_info->getTargetType();
        
        if(m_info->getBattleType()==0 || m_info->getBattleType()==2){
            m_data->addObjectsFromArray(m_info->getMember());
            m_isAttackPer = true;
        }else{
            m_data->addObjectsFromArray(m_info->getReinforce());
            m_isAttackPer = false;
        }
        
        m_cityIconNode->removeAllChildrenWithCleanup(true);
//        CCClipNode* cityClipper = CCClipNode::create(CCSize(136,136));
//        cityClipper->setPosition(ccp(-136/2, -136/2));
//        m_cityIconNode->addChild(cityClipper);
//        
//        CCNode* cityNode = CCNode::create();
//        cityNode->setScale(0.8);
//        cityClipper->addChild(cityNode);
        int level = m_info->getTargetCityLevel();
//        int mapIndex = 3;
//        while (mapIndex >= 0) {
//            auto arr = WorldMapView::getCityPicArr(mapIndex, level, false);
//            int posX = _tile_width / 2;
//            int posY = _tile_height / 2;
//            if(mapIndex == 0 || mapIndex == 2){
//                posX = _tile_width;
//            }
//            if(mapIndex == 3){
//                posX = _tile_width + posX;
//            }
//            if(mapIndex == 1 || mapIndex == 3){
//                posY = _tile_height;
//            }
//            if(mapIndex == 2){
//                posY = _tile_height + posY;
//            }
//            CCObject *obj = NULL;
//            CCARRAY_FOREACH(arr, obj){
//                std::string picName = _dict(obj)->valueForKey("pic")->getCString();
//                int addX = _dict(obj)->valueForKey("x")->intValue();
//                int addY = _dict(obj)->valueForKey("y")->intValue();
//                auto sprite = CCLoadSprite::createSprite(picName.c_str());
//                sprite->setAnchorPoint(ccp(0, 0));
//                cityNode->addChild(sprite);
//                sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY) + ccp(-_tile_width/1.6 - 5,-_tile_height/1.6));
//            }
//            mapIndex--;
//        }
        
//        auto island = NBWorldMapMainCity::getMainCityIslandImage(0, _tile_width / 2, _tile_height / 2);
//        if (island)
//        {
//            m_cityIconNode->addChild(island);
//        }
        auto house = NBWorldMapMainCity::getMainCity(0, level, -1);
        house->setAnchorPoint({.5, .5});
        house->setPosition({0, 0});
        m_cityIconNode->addChild(house);
        
        m_targetTitle->setString(_lang("115224"));
        m_teamTitle->setString(_lang("115223"));
        int textW = m_teamTitle->getContentSize().width;
        m_renSpr->setPositionX(textW*m_teamTitle->getOriginScaleX()+m_teamTitle->getPositionX()+15);
        m_txtNum1->setPositionX(m_renSpr->getPositionX() + 20);
        

        int current = m_info->getCurrSoldiers();
        std::string numStr = "";
        if(m_info->getBattleType()==0||m_info->getBattleType()==2){
            double hTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
            if(hTime<=0){
                int count = m_data->count();
                current = 0;
                for (int i=0; i<count; i++) {
                    auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
                    double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                    if(hTime<=0 && arraivalTime > 0 && !yuan->getIsLeader()){
                        continue;
                    }
                    current += yuan->getNum();
                }
            }
            numStr = _lang("108557");
            numStr.append(":");
            numStr.append(CC_CMDITOA(current));
            numStr.append("/");
            numStr.append(CC_CMDITOA(m_info->getMaxSoldiers()));
            m_armyNum->setString(numStr);
        }else{
            numStr = _lang("108557");
            numStr.append(":");
            numStr.append(CC_CMDITOA(m_info->getReinfoceCount()));
            numStr.append("/");
            numStr.append(CC_CMDITOA(m_info->getreinforceMax()));
            m_armyNum->setString(numStr);
        }
        
        int num = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX));
        bool flag = true;
        if(m_info->getAttackUid()!=GlobalData::shared()->playerInfo.uid && m_info->getBattleType()==0){ //打
            m_attackBtn->setVisible(true);
        }else if(m_data->count()<num && m_info->getAttackUid()!=GlobalData::shared()->playerInfo.uid && m_info->getBattleType()==2){//组队进攻
            int count = m_data->count();
            for (int i=0; i<count; i++) {
                auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
                if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
                    flag = false;
                    break;
                }
            }
        }else if(m_info->getAttackUid()==GlobalData::shared()->playerInfo.uid){
            flag = false;
        }else{
            
        }
        if(m_info->getBattleType()==2){
            teamPlayerNum = m_data->count();
            int gapNum = num - m_data->count();
            gapNum = MAX(gapNum,0);
            YuanJunInfo* info = NULL;
            for (int i=0; i<gapNum; i++) {
                YuanJunInfo* info = new YuanJunInfo();
                info->setTeamId(m_info->getTeamId());
                info->setTeamSoldier(m_info->getMaxSoldiers() - m_info->getCurrSoldiers());
                info->setArrivalTime(m_info->getWaitTime());
                info->setUid(m_info->getAttackUid());
                info->setIsLeader(false);
                info->setCanUse(flag);
                array = CCArray::create();
                info->setSoldiers(array);
                info->setIndex(2);
                info->setNum(m_info->getAttackId());//m_info->getTargetId()
                m_data->addObject(info);
                CC_SAFE_RELEASE(info);
                
            }
            info = new YuanJunInfo();
            info->setTeamId(m_info->getTeamId());
            info->setTeamSoldier(m_info->getMaxSoldiers() - m_info->getCurrSoldiers());
            info->setArrivalTime(m_info->getWaitTime());
            info->setIsLeader(false);
            array = CCArray::create();
            info->setSoldiers(array);
            info->setIndex(3);
            info->setNum(m_info->getTargetId());
            m_data->addObject(info);
            CC_SAFE_RELEASE(info);
        }
        m_jieSanBtn->setVisible(false);
        m_attackBtn->setVisible(false);
        m_supportBtn->setVisible(false);
        m_goHomeBtn->setVisible(false);
        int type = m_info->getBattleType();//0 个人进攻， 1 个人防守，2 组队进攻 3 组队防守
        CCArray* armys = CCArray::create();
        
//        m_clipper = CCClipNode::create(CCSize(416,30));
//        m_clipper->setPosition(ccp(0, 0));
//        m_clipperNode->addChild(m_clipper);
        std::string flagIcon = "";
        std::string leader = "";
        std::string posInfo = "";
        m_statusTxt->setFntFile(getNBFont(NB_FONT_Bold_Border));
        if(m_info->getMySide()==1){
            flagIcon = "Allance_team_jindutiao01.png";
//            m_statusTxt->setColor({255,211,0});
//            m_txtNum1->setColor({244,36,0});
            m_battleTxt->setColor({244,36,0});
            if(m_info->getAttackAAbb()!=""){
                leader.append("(");
                leader.append(m_info->getAttackAAbb());
                leader.append(")");
            }
            leader += m_info->getAttackName();
            m_battleTxt->setString(_lang("108582"));
//            m_attNode->setVisible(true);
//            m_defNode->setVisible(false);
        }else{
            flagIcon = "Allance_team_jindutiao02.png";
//            m_statusTxt->setColor({117,211,255});
//            m_txtNum1->setColor({0,162,196});
            m_battleTxt->setColor({0,162,196});
            if(m_info->getTargetAAbbr()!=""){
                leader.append("(");
                leader.append(m_info->getTargetAAbbr());
                leader.append(")");
            }
            leader += m_info->getTargetName();
            m_battleTxt->setString(_lang("115221"));
//            m_attNode->setVisible(false);
//            m_defNode->setVisible(true);
        }
        CCPoint pt = WorldController::getPointByIndex(m_info->getTargetId());
        posInfo = "";
        posInfo.append(CC_ITOA(pt.x));
        posInfo.append(",");
        posInfo.append(CC_ITOA(pt.y));
        m_targetPosTxt->setString(posInfo);
        m_leader115222Txt->setString(_lang("115222").c_str());
        m_leaderTxt->setString(leader.c_str());
        std::string pName = m_info->getTargetName();
        
        if (m_info->getTargetType() == ActBossTile) {
            CCLoadSprite::doResourceByCommonIndex(206, true);
            pName = _lang("111300");
            auto bossSpr =  CCLoadSprite::createSprite("scws.png",true,CCLoadSpriteType_MONSTERLAYERLITTLE);
            bossSpr->setScale(1.4);
            int bsW = bossSpr->getContentSize().width*bossSpr->getScale();
            bossSpr->setPosition(ccp(bsW/2, bsW/2));
            m_cityIconNode->addChild(bossSpr);
        }
        if (m_info->getTerritoryFlag() == "true") {
            CCLoadSprite::doResourceByCommonIndex(504, true);
            pName = m_info->getTerritoryName();
            if (m_info->getTerritoryType() == "tower") {
                auto territorySpr = CCLoadSprite::createSprite("territory_building_lv1.png");
                territorySpr->setScale(1.25);
                territorySpr->setAnchorPoint(ccp(0, 0));
                m_cityIconNode->addChild(territorySpr);
            }
            else if (m_info->getTerritoryType() == "flag") {
                auto territorySpr = CCLoadSprite::createSprite("territory_tower0.png");
                territorySpr->setScale(1.25);
                territorySpr->setAnchorPoint(ccp(0, 0));
                m_cityIconNode->addChild(territorySpr);
            }
        }
        
        if(pName.length()>13){
            pName = CCCommonUtils::subStrByUtf8(pName,0,12);
            pName.append("...");
        }
        m_teamTargetTxt->setString(pName);
        
//        CCScale9Sprite* icon = CCLoadSprite::createScale9Sprite(flagIcon.c_str());
//        icon->setInsetBottom(2);
//        icon->setInsetLeft(30);
//        icon->setInsetRight(30);
//        icon->setInsetBottom(2);
//        icon->setAnchorPoint(ccp(0, 0));
//        icon->setContentSize(CCSize(416,30));
//        m_clipper->addChild(icon);
        
        m_teamArmys = CCArray::create();
        
        std::string nameStr = "";
        int numSoldier = 0;
        int offX = 0;
        switch (type) {
            case 0:
            {
                nameStr = "";
                if(m_info->getAttackAAbb()!=""){
                    nameStr = "(";
                    nameStr.append(m_info->getAttackAAbb());
                    nameStr.append(") ");
                    nameStr.append(m_info->getAttackName());
                }else{
                    nameStr.append(m_info->getAttackName());
                }

                int numSoldier = m_info->getMember()->count();
                m_txtNum1->setString(CC_ITOA(numSoldier));
                offX = 58;
                m_attackBtn->setVisible(true);
            }
                
                break;
            case 1:
            {
                nameStr = "";
                if(m_info->getTargetAAbbr()!=""){
                    nameStr = "(";
                    nameStr.append(m_info->getTargetAAbbr());
                    nameStr.append(") ");
                    nameStr.append(m_info->getTargetName());
                }else{
                    nameStr.append(m_info->getTargetName());
                }
                numSoldier = m_info->getReinforce()->count()+1;
                m_txtNum1->setString(CC_ITOA(numSoldier));
                                
                YuanJunInfo* self = new YuanJunInfo();
                self->setPic(m_info->getTargetPic());
                self->setUid(GlobalData::shared()->playerInfo.uid);
                self->setpicVer(GlobalData::shared()->playerInfo.picVer);
                armys->addObject(self);
                armys->addObjectsFromArray(m_info->getReinforce());
                CC_SAFE_RELEASE(self);
                int count = m_data->count();
                bool haveSelf = false;
                for (int i=0; i<count; i++) {
                    auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
                    if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
                        haveSelf = true;
                        break;
                    }
                }
                if(haveSelf && m_info->getTargetUid()!=GlobalData::shared()->playerInfo.uid){
                    m_goHomeBtn->setVisible(true);
                }else if(m_info->getTargetUid()!=GlobalData::shared()->playerInfo.uid){
                    m_supportBtn->setVisible(true);
                }
            }
                
                break;
            case 2:
            {
                nameStr = "";
                if(m_info->getAttackAAbb()!=""){
                    nameStr = "(";
                    nameStr.append(m_info->getAttackAAbb());
                    nameStr.append(") ");
                    nameStr.append(m_info->getAttackName());
                }else{
                    nameStr.append(m_info->getAttackName());
                }
                int num = m_info->getMember()->count();
                int useNum = 0;
                double hTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
                for (int i=0; i<num; i++) {
                    YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                    double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                    if(hTime<=0 && arraivalTime > 0 && !yuan->getIsLeader()){
                        continue;
                    }
                    useNum += 1;
                }
                m_txtNum1->setString(CC_ITOA(useNum));
                m_jieSanBtn->setVisible(true);
                m_jieSanBtn->setEnabled(hTime>0);
            }
                
                break;
            case 3:
            {
                nameStr = "";
                if(m_info->getTargetAAbbr()!=""){
                    nameStr = "(";
                    nameStr.append(m_info->getTargetAAbbr());
                    nameStr.append(") ");
                    nameStr.append(m_info->getTargetName());
                }else{
                    nameStr.append(m_info->getTargetName());
                }
                numSoldier = m_info->getReinforce()->count()+1;
                m_txtNum1->setString(CC_ITOA(numSoldier));
                
                int count = m_data->count();
                bool haveSelf = false;
                for (int i=0; i<count; i++) {
                    auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
                    if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
                        haveSelf = true;
                        break;
                    }
                }
                if(haveSelf && m_info->getTargetUid()!=GlobalData::shared()->playerInfo.uid){
                    m_goHomeBtn->setVisible(true);
                }else if(m_info->getTargetUid()!=GlobalData::shared()->playerInfo.uid){
                    m_supportBtn->setVisible(true);
                }
            }
                
                break;
        }
        m_refreshMarch = false;
        m_statusTxt->setString("");
        double haveTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
        double march = m_info->getMarchTime() - GlobalData::shared()->getWorldTime();
        if(haveTime>0 || march>0){
            this->unschedule(schedule_selector(AllianceWarDetailView::updateTime));
            this->schedule(schedule_selector(AllianceWarDetailView::updateTime), 1);
            this->updateTime(0);
        }
        ret = true;
    }
    if (m_info->getBattleType() == 2)
    {
        m_isLeaderUser = (m_info->getAttackUid()==GlobalData::shared()->playerInfo.uid);
    }
    
    m_tabView->reloadData();
    
    return ret;
}

void AllianceWarDetailView::updateTime(float _time){
    double haveTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
    double march = m_info->getMarchTime() - GlobalData::shared()->getWorldTime();
    float len = 0;
    double totalTime = 0;
    if(haveTime==0){
        m_refreshMarch = false;
    }
    m_jieSanBtn->setEnabled(haveTime>0);
    if(haveTime>0){
        m_statusTxt->getParent()->setVisible(true);
        totalTime = (m_info->getWaitTime() - m_info->getCreateTime());
        len = haveTime/totalTime;
        len = MAX(len,0);
        len = MIN(len,1);
//        m_clipper->setContentSize(CCSize((1-len)*416,30));
        
        m_progress->setPreferredSize(CCSize(175 * len, 30));
        m_progress->setVisible(175 * len > 10);
        
        m_statusXXXTxt->setString(_lang_1("115130", ""));
        m_statusTxt->setString(CC_SECTOA((int)haveTime));
    }else if(march>0){
        m_statusTxt->getParent()->setVisible(true);
        totalTime = (m_info->getMarchTime() - m_info->getWaitTime());
        len = march/totalTime;
        len = MAX(len,0);
        len = MIN(len,1);
//        m_clipper->setContentSize(CCSize((1-len)*416,30));
        
        m_progress->setPreferredSize(CCSize(175 * len, 30));
        m_progress->setVisible(175 * len > 10);
        
        m_statusXXXTxt->setString(_lang_1("115131", ""));
        m_statusTxt->setString(CC_SECTOA((int)march));
        
    }else{
        m_statusTxt->getParent()->setVisible(false);
//        m_clipper->setContentSize(CCSize(416,30));
        this->unschedule(schedule_selector(AllianceWarDetailView::updateTime));
        this->closeSelf();
        return ;
    }
    if(!m_refreshMarch && haveTime<=0){
        m_refreshMarch = true;
        int num = m_data->count();
        for(int i=0;i<num;i++){
            YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(i);
            double arraivalTime = (info->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
            if(arraivalTime > 0 && info->getIndex()!=2 && info->getIndex()!=3 && !info->getIsLeader() && m_isAttackPer){
                m_data->removeObjectAtIndex(i);
                i--;
                num--;
            }
        }
        if(m_info->getBattleType()==2){
            CCLoadSprite::doResourceByCommonIndex(204, true);
            CCLoadSprite::doResourceByCommonIndex(7, true);
            int num = m_info->getMember()->count();
            int useNum = 0;
            int current = 0;
            for (int i=0; i<num; i++) {
                YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                if(arraivalTime > 0 && !yuan->getIsLeader()){
                    continue;
                }
                current += yuan->getNum();
                useNum += 1;
            }
            m_txtNum1->setString(CC_ITOA(useNum));
        
            std::string numStr = _lang("108557");
            numStr.append(":");
            numStr.append(CC_CMDITOA(current));
            numStr.append("/");
            numStr.append(CC_CMDITOA(m_info->getMaxSoldiers()));
            m_armyNum->setString(numStr);
            
            int gapNum = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX)) - m_data->count();
            gapNum = MAX(gapNum,0);
            for (int i=0; i<gapNum; i++) {
                YuanJunInfo* info = new YuanJunInfo();
                info->setTeamId(m_info->getTeamId());
                info->setTeamSoldier(m_info->getMaxSoldiers() - m_info->getCurrSoldiers());
                info->setArrivalTime(m_info->getWaitTime());
                info->setUid(m_info->getAttackUid());
                info->setIsLeader(false);
                CCArray* array = CCArray::create();
                info->setSoldiers(array);
                info->setIndex(2);
                info->setNum(m_info->getTargetId());
                m_data->addObject(info);
                CC_SAFE_RELEASE(info);
                if(m_data->count()>=2){
                    m_data->swap(m_data->count()-2, m_data->count()-1);
                }
            }
        }
        m_tabView->reloadData();
    }
}

void AllianceWarDetailView::onCellClick(CCObject* data){
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

void AllianceWarDetailView::updateArriveTime(CCObject* data){
    auto dic = _dict(data);
    if(dic){
        auto temp = dic->valueForKey("et")->doubleValue()/1000;
        if(temp!=0){
            temp = GlobalData::shared()->changeTime(temp);
        }else{
            temp = 0;
        }
        std::string uuid = dic->valueForKey("uuid")->getCString();
        int num = m_data->count();
        for (int i=0; i<num; i++) {
            YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(i);
            if(info->getUuid()==uuid){
                info->setArrivalTime(temp);
                break;
            }
        }
        m_tabView->reloadData();
    }
}

void AllianceWarDetailView::jieSanAlliacne(CCObject *pSender, CCControlEvent event)
{
    if (teamBattleType == ActBossTile) {
        YesNoDialog::show( _lang("137458").c_str() , CCCallFunc::create(this, callfunc_selector(AllianceWarDetailView::okJieSan)));
    }else {
        okJieSan();
    }
}

void AllianceWarDetailView::okJieSan()
{
    CancelAllianceTeamCommand* cmd = new CancelAllianceTeamCommand(m_info->getTeamId());
    cmd ->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

void AllianceWarDetailView::onAttackClick(CCObject *pSender, CCControlEvent event){
    AllianceManager::getInstance()->onAttackView(m_info->getTargetId());
}

void AllianceWarDetailView::onSupportClick(CCObject *pSender, CCControlEvent event){
    int count = m_data->count();
    bool haveSelf = false;
    for (int i=0; i<count; i++) {
        auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
        if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
            haveSelf = true;
            break;
        }
    }
    if(haveSelf){
        CCCommonUtils::flyHint("", "", _lang("115214"));
    }else{
       AllianceManager::getInstance()->openYuanYunView(m_info->getTargetId());
    }
}

void AllianceWarDetailView::onGoHomeClick(CCObject *pSender, CCControlEvent event){
    std::string uuid = "";
    int count = m_data->count();
    for (int i=0; i<count; i++) {
        auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
        if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
            uuid = yuan->getUuid();
            break;
        }
    }
    if(uuid!="" && m_info->getTargetUid() != GlobalData::shared()->playerInfo.uid){
        auto cmd = new WorldMarchCancelCommand(uuid,"");
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceWarDetailView::sendBackArmy), NULL));
        cmd->sendAndRelease();
    }
}

void AllianceWarDetailView::sendBackArmy(CCObject* obj){
    int count = m_data->count();
    for (int i=0; i<count; i++) {
        auto yuan = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
        if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
            m_data->removeObjectAtIndex(i);
            break;
        }
    }
    count = m_info->getReinforce()->count();
    for (int i=0; i<count; i++) {
        auto yuan = dynamic_cast<YuanJunInfo*>(m_info->getReinforce()->objectAtIndex(i));
        if(yuan->getUid()==GlobalData::shared()->playerInfo.uid){
            m_info->getReinforce()->removeObjectAtIndex(i);
            break;
        }
    }
    int offX = 0;
    CCArray* armys = CCArray::create();
    YuanJunInfo* self = new YuanJunInfo();
    self->setPic(m_info->getTargetPic());
    self->setUid(GlobalData::shared()->playerInfo.uid);
    self->setpicVer(GlobalData::shared()->playerInfo.picVer);
    armys->addObject(self);
    armys->addObjectsFromArray(m_info->getReinforce());
    CC_SAFE_RELEASE(self);
    int num = armys->count();
    offX = 58*num;
    int numSoldier = m_info->getReinforce()->count()+1;
    m_txtNum1->setString(CC_ITOA(numSoldier));
    m_tabView->reloadData();
    m_goHomeBtn->setVisible(false);
    m_supportBtn->setVisible(true);
}

bool AllianceWarDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusXXXTxt", CCLabelIF*, this->m_statusXXXTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_jieSanBtn", CCControlButton*, this->m_jieSanBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_attackBtn", CCControlButton*, this->m_attackBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_supportBtn", CCControlButton*, this->m_supportBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goHomeBtn", CCControlButton*, this->m_goHomeBtn);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clipperNode", CCNode*, this->m_clipperNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txtNum1", CCLabelIF*, this->m_txtNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_posNode1", CCNode*, this->m_posNode1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleTxt", CCLabelIF*, this->m_battleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetTitle", CCLabelIF*, this->m_targetTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetPosTxt", CCLabelIF*, this->m_targetPosTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_teamTargetTxt", CCLabelIF*, this->m_teamTargetTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_teamTitle", CCLabelIF*, this->m_teamTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leader115222Txt", CCLabelIFTTF*, this->m_leader115222Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cityIconNode", CCNode*, this->m_cityIconNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defNode", CCNode*, this->m_defNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_attNode", CCNode*, this->m_attNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renSpr", CCSprite*, this->m_renSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    return false;
}

SEL_CCControlHandler AllianceWarDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "jieSanAlliacne", AllianceWarDetailView::jieSanAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAttackClick", AllianceWarDetailView::onAttackClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSupportClick", AllianceWarDetailView::onSupportClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoHomeClick", AllianceWarDetailView::onGoHomeClick);
    return NULL;
}

CCSize AllianceWarDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
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
        if (obj->getUid()!=GlobalData::shared()->playerInfo.uid && !obj->getIsLeader() && m_isLeaderUser && m_info->getBattleType()==2)
        {
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

CCSize AllianceWarDetailView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1450, 283);
    }
    return CCSize(CELL_W, CELL_H);
}

CCTableViewCell* AllianceWarDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceTeamDetailCell* cell = (AllianceTeamDetailCell*)table->dequeueCell();
    YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,m_isLeaderUser,m_info->getBattleType(), m_info->getTeamId());
        }else{
            cell = AllianceTeamDetailCell::create(info,m_infoList,m_isLeaderUser,m_info->getBattleType(), m_info->getTeamId());
        }
    }
    return cell;
}

ssize_t AllianceWarDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void AllianceWarDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

#pragma mark -

AllianceTeamDetailCell *AllianceTeamDetailCell::create(YuanJunInfo* info,CCNode* clickNode,bool isLeaderUser,int battleType,string teamId){
    AllianceTeamDetailCell *ret = new AllianceTeamDetailCell(info,clickNode,isLeaderUser,battleType,teamId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceTeamDetailCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void AllianceTeamDetailCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceTeamDetailCell::init(){
    auto node = CCBLoadFile("AllianceTeamDetailCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    CCCommonUtils::setButtonTitle(m_speedBtn, _lang("108558").c_str());
    CCCommonUtils::setButtonTitle(m_goHomeBtn, _lang("115192").c_str());
    CCCommonUtils::setButtonTitle(m_daJieBtn, _lang("115191").c_str());
    setData(m_info,m_isLeaderUser,m_battleType,m_teamId);
    return true;
}

void AllianceTeamDetailCell::setData(YuanJunInfo* info,bool isLeaderUser,int battleType,string teamId){
    m_info = info;
    m_isLeaderUser = isLeaderUser;
    m_battleType = battleType;
    m_teamId = teamId;
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_nameTxt->setString(m_info->getName().c_str());
    std::string numStr = _lang("108557");
    numStr.append(":");
    numStr.append(CC_CMDITOA(m_info->getNum()));
    m_armyNum->setString(numStr);
    m_unUseNode->setVisible(false);
    m_moveNode->setVisible(false);
    m_scienceNode->setVisible(false);
    if(m_info->getIndex()==2){
        m_unUseNode->setVisible(true);
        m_tipTxt->setString(_lang("115145"));
    }else if(m_info->getIndex()==3){
        m_scienceNode->setVisible(true);
        m_scienceTxt->setString(_lang("115146"));
    }else{
        m_moveNode->setVisible(true);
    }
    m_soldierNode->removeAllChildrenWithCleanup(true);
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
    double totalTime = (m_info->getArrivalTime() - m_info->getStartTime());
    if(m_info->getOpen()){
        num = m_info->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = row * 100 + 70;
//        if (CCCommonUtils::isIosAndroidPad()) {
//            addH = row*200 + 144;
//        }
        if (m_info->getUid()!=GlobalData::shared()->playerInfo.uid && !m_info->getIsLeader() && m_isLeaderUser && m_battleType==2)
        {
//            if (CCCommonUtils::isIosAndroidPad()) {
//                addH+=192;
//            }
//            else
//                addH += 80;
        }
        if(CCCommonUtils::isIosAndroidPad()){
            m_moveNode->setPosition(ccp(0, addH));
            m_soldierNode->setPosition(ccp(0, addH));
        }
        else{
            m_moveNode->setPosition(ccp(0, addH));
            m_soldierNode->setPosition(ccp(0, addH));
        }
        maxCount = 0;
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
            int rowIndex = i/2;
            int col = i%2;
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(col==0?40:680, -220-rowIndex*200));
            }
            else
//                cell->setPosition(ccp(col==0?10:330, -100-rowIndex*100));
                cell->setPosition(ccp(col == 0 ? 33 : 327, - 122 - rowIndex * 128));
            
            cell->m_bg->setScaleY(rowIndex % 2 == 0 ? 1 : -1);
            
            m_soldierNode->addChild(cell);
            count = dic->valueForKey("count")->intValue();
            std::string armyId = dic->valueForKey("armyId")->getCString();
            if(count>maxCount){
                maxCount = count;
                useArmyId = armyId;
            }
        }
        
        if (m_info->getUid()!=GlobalData::shared()->playerInfo.uid && !m_info->getIsLeader() && m_isLeaderUser && m_battleType==2)
        {
            CCControlButton* button = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
            button->setTitleForState(_lang("115157"), CCControlStateNormal);
            button->setTitleForState(_lang("115157"), CCControlStateHighlighted);
            button->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceTeamDetailCell::onQianfanClick), CCControlEventTouchUpInside);
            
            button->setPreferredSize(CCSizeMake(250, 70));
            button->setAnchorPoint(ccp(0.5, 0.5));
            if (CCCommonUtils::isIosAndroidPad()) {
                button->setScale(1.44f);
                button->setPosition(ccp(725, -addH+200));
            }
            else {
                button->setScale(0.6f);
                button->setPosition(ccp(300, -addH+100));
            }
            m_soldierNode->addChild(button);
        }
        
        // m_renderBg->initWithSpriteFrame(CCLoadSprite::loadResource("Allance_team_Members.png"));
        m_renderBg->setAnchorPoint(CCPoint(0,0));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_renderBg->setContentSize(CCSize(1430,283+addH));
        }
        else
            m_renderBg->setContentSize(CCSize(CELL_W, CELL_H+addH));
        
        m_renderBg->removeAllChildrenWithCleanup(true);
        Scale9Sprite* sp = CCLoadSprite::createScale9Sprite("nb_al_members_bg.png");
        sp->setTag(OPEN_BG_TAG);
        sp->setZOrder(-OPEN_BG_TAG);
        sp->setContentSize({CELL_W - 30.0f, row * 118 + 50.0f});
        sp->setPosition({m_renderBg->getPositionX() + CELL_W / 2.0f, m_renderBg->getPositionY() + m_renderBg->getContentSize().height / 2 - CELL_H / 2 + 20});
        m_renderBg->getParent()->addChild(sp);
        
        m_funNode->setVisible(true);
    }else{
        m_funNode->setVisible(false);
        m_moveNode->setPosition(ccp(0, 0));
        m_soldierNode->setPosition(ccp(0, 0));
        if((haveTime<=0 && (m_info->getIndex()==2))||!m_info->getCanUse()){
            // m_renderBg->initWithSpriteFrame(CCLoadSprite::loadResource("Allance_team_Members_grey.png"));
            if(haveTime<=0 && m_info->getIndex()==2){
                m_unUseNode->setVisible(false);
                m_scienceNode->setVisible(true);
                m_scienceTxt->setString(_lang("115219"));
            }
            m_tipTxt->setString(_lang("115145"));
//            m_tipTxt->setColor({120,120,120});
            
        }else{
            // m_renderBg->initWithSpriteFrame(CCLoadSprite::loadResource("Allance_team_Members.png"));
//            m_tipTxt->setColor({169,183,189});
        }
        m_renderBg->setAnchorPoint(CCPoint(0,0));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_renderBg->setContentSize(CCSize(1430,283));
        }
        else
            m_renderBg->setContentSize(CCSize(CELL_W, CELL_H));
    }
    if(m_info->getGenerals() && m_info->getGenerals()->count()>0){
        std::string head = m_info->getPic();
        head.append(".png");
        m_icon->removeAllChildrenWithCleanup(true);
        CCSprite* headSpr = CCLoadSprite::createSprite(head.c_str());
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
        if (CCCommonUtils::isUseCustomPic(m_info->getpicVer()))
        {
            m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(m_info->getUid(), m_info->getpicVer()), 1.0f, 78, true);
        }
    }else{
        m_icon->removeAllChildrenWithCleanup(true);
        std::string icon = "ico" + useArmyId+ "_small.png";
        CCSprite* headSpr = CCLoadSprite::createSprite(icon.c_str());
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
    }
    m_arrow->setRotation(m_info->getOpen()?270:180);
    
    if(haveTime>0 && !m_info->getIsLeader()){
        m_statusTxt->setString(_lang("115136"));
        std::string time= _lang_1("115131", CC_SECTOA(haveTime));
        m_proTxt->setString(time);
        if(m_info->getOpen()){
           m_funNode->setVisible(true);
        }
        float len = (totalTime-haveTime)*1.0/totalTime;
        len = MAX(len,1.0);
        m_progress->setContentSize(CCSize(340.0*len,22));
        this->unschedule(schedule_selector(AllianceTeamDetailCell::updateTime));
        this->schedule(schedule_selector(AllianceTeamDetailCell::updateTime),1);
        this->updateTime(0);
    }else{
        m_proTxt->setString(_lang("115144"));
        m_statusTxt->setString(_lang("115144"));
        m_funNode->setVisible(false);
    }
}

void AllianceTeamDetailCell::onQianfanClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    YuanYunReturnCommand* cmd = new YuanYunReturnCommand(m_info->getUuid());
    cmd->putParam("teamId", ccs(m_teamId));
    cmd->sendAndRelease();
    vector<EnemyInfo>::iterator it = EnemyInfoController::getInstance()->m_enemyInfos.begin();
    for (; it != EnemyInfoController::getInstance()->m_enemyInfos.end(); it++)
    {
        if (it->uuid == m_info->getUuid())
        {
            EnemyInfoController::getInstance()->m_enemyInfos.erase(it);
            break;
        }
    }
    WorldController::getInstance()->forceUPdate = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_QIANFAN, ccs(m_info->getUid()));
}

void AllianceTeamDetailCell::updateTime(float _time){
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    double totalTime = (m_info->getArrivalTime() - m_info->getStartTime());
    if(haveTime<=0){
        this->unschedule(schedule_selector(AllianceTeamDetailCell::updateTime));
        m_proTxt->setString(_lang("115144"));
        m_statusTxt->setString(_lang("115144"));
        m_funNode->setVisible(false);
        return ;
    }
    std::string time= _lang_1("115131", CC_SECTOA(haveTime));
    m_proTxt->setString(time);
    float len = haveTime*1.0/totalTime;
    len = MIN(len,1.0);
    len = MAX(len, 0);
    m_progress->setContentSize(CCSize(340.0*(1-len),22));
}

void AllianceTeamDetailCell::onSpeedClick(CCObject *pSender, CCControlEvent event){
    if(!m_funNode->isVisible()){
        return ;
    }
    PopupViewController::getInstance()->addPopupInView(UseCDToolView::create(m_info->getUuid(),CCD_MARCH_TYPE,true,m_info->getStartTime(),m_info->getArrivalTime()));
}

void AllianceTeamDetailCell::onGoHomeClick(CCObject *pSender, CCControlEvent event){
    
}

bool AllianceTeamDetailCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
       return true;
    }
    return false;
}

void AllianceTeamDetailCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && fabsf(end.y - m_touchPos.y)<30){
        m_clickFlag = !m_clickFlag;
        if(isTouchInside(m_speedBtn, pTouch) && m_funNode->isVisible()){
            return ;
        }
        
        if(m_info->getIndex()==2){
            double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
            if (m_info->getUid()==GlobalData::shared()->playerInfo.uid) {
                CCCommonUtils::flyHint("", "", _lang("115212"));
            }else if(!m_info->getCanUse()){
                CCCommonUtils::flyHint("", "", _lang("115213"));
            }else if(haveTime>0){
                // 攻击方，并且有防护罩
                auto now = WorldController::getInstance()->getTime();
                auto dialog = dynamic_cast<AllianceWarDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
                bool isAttackPer = false;
                if(dialog){
                    isAttackPer = dialog->getIsAttackPer();
                }
                // m_info->getMySide()==1
                if(GlobalData::shared()->playerInfo.protectTimeStamp > now && isAttackPer)
                {
                    YesNoDialog::show( _lang("101438").c_str() , CCCallFunc::create(this, callfunc_selector(AllianceTeamDetailCell::attendRally)));
                }
                else
                {
                    attendRally();
                }
            }else{
                //todo
                //CCCommonUtils::flyHint("", "", _lang("115144"));
            }
        }else if(m_info->getIndex()==3){
            //TODO: guojiang 联盟
            CCCommonUtils::flyHint("", "", _lang("E100008"));
            return;
            
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_WAR_DETAIL_CELL_CLICK,CCString::create(m_info->getUuid().c_str()));
        }
    }
}

void AllianceTeamDetailCell::attendRally()
{
    if (teamPlayerNum>=GlobalData::shared()->worldConfig.bossLimitNum && teamBattleType == ActBossTile) {
        CCCommonUtils::flyHint("", "", _lang_1("137464", CC_ITOA(GlobalData::shared()->worldConfig.bossLimitNum)));
        return;
    }
    WorldController::getInstance()->openMarchDeploy(m_info->getNum(),1,m_info->getTeamSoldier(),MethodUnion,-1,m_info->getTeamId(),teamBattleType);
}
void AllianceTeamDetailCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool AllianceTeamDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg", CCScale9Sprite*, this->m_renderBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedBtn", CCControlButton*, this->m_speedBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unUseNode", CCNode*, this->m_unUseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_funNode", CCNode*, this->m_funNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goHomeBtn", CCControlButton*, this->m_goHomeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_daJieBtn", CCControlButton*, this->m_daJieBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_proTxt", CCLabelIF*, this->m_proTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceNode", CCNode*, this->m_scienceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceTxt", CCLabelIF*, this->m_scienceTxt);
    return false;
}

SEL_CCControlHandler AllianceTeamDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpeedClick", AllianceTeamDetailCell::onSpeedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoHomeClick", AllianceTeamDetailCell::onGoHomeClick);
    return NULL;
}
