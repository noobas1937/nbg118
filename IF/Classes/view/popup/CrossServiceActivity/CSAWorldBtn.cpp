//
//  CSAWorldBtn.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/11.
//
//

#include "CSAWorldBtn.h"
#include "ParticleController.h"
#include "ActivityController.h"
#include "QuestController.h"
#include "WorldController.h"

CSAWorldBtn *CSAWorldBtn::create(){
    CSAWorldBtn *ret = new CSAWorldBtn();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CSAWorldBtn::init(){
    auto node = CCBLoadFile("CSAWorldBtn", this, this);
    this->setContentSize(node->getContentSize());
    m_parNode = CCNode::create();
    m_parNode->setScale(0.8);
    this->addChild(m_parNode);
    m_actObj = NULL;
    m_state = 3;
    m_startTime = 0;
    m_endTime = 0;
    if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
        int num = ActivityController::getInstance()->activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
            if (temp && temp->type==7) {
                m_actObj = temp;
                m_startTime = m_actObj->startTime;
                m_endTime = m_actObj->endTime;
                break;
            }
        }
    }
    
    updateState(3);
    this->schedule(schedule_selector(CSAWorldBtn::updateTime), 1);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Turn");
    
    return true;
}

void CSAWorldBtn::updateTime(float _time){
    m_pNode->setVisible(false);
    double nowTime = GlobalData::shared()->getWorldTime();
    if (m_startTime<nowTime && m_endTime>nowTime)
    {
        int leftTime = m_endTime - nowTime;
        int k7 = ActivityController::getInstance()->wb_info_k7;
        if (k7<1) {
            k7 = 1;
        }
        int fTime = leftTime%(k7*60*60);
        m_timeTxt->setString(CC_SECTOA(fTime));
    }else{
        m_timeTxt->setString("");
    }
    if (ActivityController::getInstance()->ckfProtectTime!=0 && ActivityController::getInstance()->ckfProtectTime>nowTime && GlobalData::shared()->playerInfo.currentServerId > CROSS_SERVER_START_INDEX) {
        int pTime = ActivityController::getInstance()->ckfProtectTime - nowTime;
        m_pTipTxt->setString(_lang_1("138080", CC_SECTOA(pTime)));
        m_pNode->setVisible(true);
    }
    map<std::string, QuestInfo*>::iterator it;
    int state = m_state;
    for (it = QuestController::getInstance()->currentCKFList.begin(); it!=QuestController::getInstance()->currentCKFList.end(); it++) {
        if (it->second->type==3) {
            if (it->second->state==COMPLETE) {
                state = 2;
                break;
            }else{
                state = 3;
            }
        }
    }
    if(QuestController::getInstance()->currentCKFList.size()==0 && state==2){
        state = 3;
    }
    if (state!=m_state) {
        updateState(state);
    }
}

//0 为升级，1为降级，2为目标
void CSAWorldBtn::updateState(int state){
    m_state = state;
    CCParticleSystemQuad* particle = NULL;
    m_parNode->removeAllChildrenWithCleanup(true);
    if (state==0) {
        m_icon->initWithSpriteFrame(CCLoadSprite::loadResource("csa_icon_02.png"));
        particle = ParticleController::createParticle(CCString::createWithFormat("RankingRaise_%d",0)->getCString());
        particle->setPositionY(-40);
        m_parNode->addChild(particle);
        for (int i=1; i<6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("RankingRaise_%d",i)->getCString());
            particle->setPositionY(-16);
            m_parNode->addChild(particle);
        }
    }else if(state==1){
        m_icon->initWithSpriteFrame(CCLoadSprite::loadResource("csa_icon_02.png"));
        particle = ParticleController::createParticle(CCString::createWithFormat("RankingDown_%d",0)->getCString());
        particle->setPositionY(50);
        m_parNode->addChild(particle);
        for (int i=1; i<6; i++) {
            particle = ParticleController::createParticle(CCString::createWithFormat("RankingDown_%d",i)->getCString());
            particle->setPositionY(-16);
            m_parNode->addChild(particle);
        }
    }else if(state==2){
        m_icon->initWithSpriteFrame(CCLoadSprite::loadResource("csa_icon_03.png"));
        for (int i=1; i<4; i++) {
            particle = ParticleController::createParticle(CCString::createWithFormat("RankingTarget_%d",i)->getCString());
            m_parNode->addChild(particle);
        }
    }else{
        m_icon->initWithSpriteFrame(CCLoadSprite::loadResource("csa_icon_01.png"));
    }
}

void CSAWorldBtn::onEnter(){
    CCNode::onEnter();
}

void CSAWorldBtn::onExit(){
    CCNode::onExit();
}

bool CSAWorldBtn::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCSprite*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNode", CCNode*, this->m_pNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pTipTxt", CCLabelIF*, this->m_pTipTxt);
    
    return false;
}

SEL_CCControlHandler CSAWorldBtn::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}