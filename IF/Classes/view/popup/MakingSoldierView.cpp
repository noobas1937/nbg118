//
//  MakingSoldierView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-22.
//
//

#include "MakingSoldierView.h"
#include "QueueController.h"
#include "PopupViewController.h"
#include "ProductionSoldiersView.h"
#include "FunBuildController.h"
#include "ParticleController.h"

MakingSoldierView* MakingSoldierView::create(std::string armyId,QueueInfo* info,int buildingId){
    MakingSoldierView* ret = new MakingSoldierView(armyId,info,buildingId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool MakingSoldierView::init()
{
    if (!ArcPopupBaseView::init(TYPE_POS_MID_UP)) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(4, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    CCBLoadFile("MakingSoldierView",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_haveTime = m_info->finishTime - m_info->startTime;
    this->schedule(schedule_selector(MakingSoldierView::updateTime), 1);
    this->updateTime();
    m_btnLabel->setString(_lang("104903").c_str());
    
    bool isFort = m_buildingId/1000 == FUN_BUILD_FORT;
    m_resIndex = -1;
    std::string desc = "";
    ArmyInfo* armyInfo = NULL;
    if(isFort){
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(m_armyId);
        if(it!=GlobalData::shared()->fortList.end() && it->second.finishTime!=0){
            desc = it->second.getDesc();
            armyInfo = &it->second;
        }
    }else{
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_armyId);
        if(it!=GlobalData::shared()->armyList.end() && it->second.finishTime!=0){
            desc = it->second.getDesc();
            armyInfo = &it->second;
        }
    }
    if(armyInfo==NULL){
        CCLOG("error have no making soldier");
        return false;
    }
    m_desLabel->setString(desc.c_str());
    
    CCPoint pos = ccp(0,168);
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        pos = ccp(0,93);
    }
    if(!isFort){
        m_resIndex = m_buildingId/1000000 - 423 + 200;
        CCLoadSprite::doResourceByCommonIndex(m_resIndex, true);
    }
    CCSprite* pic = CCLoadSprite::createSprite(armyInfo->getBodyIcon().c_str());
    pic->setPosition(pos);
    m_soldierNode->addChild(pic);
    if(m_buildingId/1000==FUN_BUILD_BARRACK4){
         pic->setPosition(ccp(-60,130));
    }
    m_soldierBg->setVisible(m_buildingId/1000!=FUN_BUILD_BARRACK4);
    if(m_resIndex!=-1){
        CCLoadSprite::doResourceByCommonIndex(m_resIndex, false);
    }
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        std::string particleName = "WeaponsFire_Wood";
        for (int i=2; i <= 6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s_%d",particleName.c_str(),i)->getCString());
            particle->setPosition(0, 65);
            m_soldierNode->addChild(particle);
        }
    }
    return true;
}


void MakingSoldierView::onEnter(){
    CCNode::onEnter();
}

void MakingSoldierView::onExit(){
    CCNode::onExit();
}

void MakingSoldierView::updateTime(float t){
    int current = m_info->finishTime - m_info->getTime();
    if(current<=0){
        this->unschedule(schedule_selector(MakingSoldierView::updateTime));
        current = 0;
        m_killCDBtn->setEnabled(false);
        this->closeHandle();
        return ;
    }
    float len = current*1.0/m_haveTime;
    if(len>1) len = 1;
    m_timeProgress->setContentSize(CCSize((1-len)*436,14.0));
    m_inBtnGoldNum->setString(CC_ITOA(getGoldByTime(current)));
    m_timeTxt->setString(CC_SECTOA(current));
}

int MakingSoldierView::getGoldByTime(int time){
    if(time<=0) return 0;
    int k1 = GlobalData::shared()->cd_gold["k1"];
    int k2 = GlobalData::shared()->cd_gold["k2"];
    float tmp = time/3600.0;
    float p1 = k2/100.0;
    int goldcost = time*1.0/(k1*powf(tmp, p1));
    goldcost = MAX(goldcost,1);
    return goldcost;
}

void MakingSoldierView::killCDClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int current = m_info->finishTime - m_info->getTime();
    QueueController::getInstance()->startCCDQueue(m_info->qid,"",false,getGoldByTime(current),"",2);
    this->closeHandle();
}

void MakingSoldierView::closeHandle(){
    int id = m_buildingId;
    int viewId = PopupViewController::getInstance()->goBackPopupView(this);
    if(viewId==-1){
        PopupViewController::getInstance()->addPopupInView(ProductionSoldiersView::create(id));
    }
}

SEL_CCControlHandler MakingSoldierView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "killCDClick", MakingSoldierView::killCDClick);
    return NULL;
}

bool MakingSoldierView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killCDBtn", CCControlButton*, this->m_killCDBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, this->m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeProgress", CCScale9Sprite*, this->m_timeProgress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierBg", CCScale9Sprite*, this->m_soldierBg);
    
    return false;
}