//
//  BuildQueueCell.cpp
//  IF
//
//  Created by fubin on 14-7-31.
//
//

#include "BuildQueueCell.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "QuestInfo.h"
#include "QuestController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "FBUtilies.h"

using namespace cocos2d;

BuildQueueCell* BuildQueueCell::create(int qid)
{
    BuildQueueCell *pRet = new BuildQueueCell();
    if (pRet && pRet->initBuildQueueCell(qid))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool BuildQueueCell::initBuildQueueCell(int qid)
{
    m_qid = qid;
    m_particleNode = CCNode::create();
    
    
    CCBLoadFile("BuildQueueCell",this,this);
    this->m_handLabel->setString(_lang("102285"));
    m_infoNode->setVisible(false);
    m_info2Node->setVisible(false);
    m_hand2Label->setString(_lang("102298"));
    
    string hammerPic = "hammer2.png";
    if (m_qid == BUILD_QUEUE_1ST) {
        hammerPic = "hammer1.png";
    }
    auto pic = CCLoadSprite::createSprite(hammerPic.c_str());
    pic->setScale(1.5);
    m_hammerNode->addChild(pic);
    
    m_timeLabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
    
    m_proTimer = CCProgressTimer::create(CCLoadSprite::createSprite("icon_jindutiao.png"));
    m_proTimer->setType(kCCProgressTimerTypeRadial);
    m_proTimer->setPercentage(0);
    m_proNode->addChild(m_proTimer);
    m_flyHintFlag = false;
    
    this->addChild(m_particleNode);
    
    m_curState = Queue_ST_INIT;
    this->getAnimationManager()->runAnimationsForSequenceNamed("Lock");
    
    onEnterFrame(0.0);
    this->schedule(schedule_selector(BuildQueueCell::onEnterFrame));
    return true;
}

void BuildQueueCell::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}
void BuildQueueCell::onExit() {
    m_iconNode->removeAllChildren();
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler BuildQueueCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool BuildQueueCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if(0 == strcmp(pMemberVariableName, "m_proNode"))
    {
        CCLOG("a");
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_proNode", CCNode*, this->m_proNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hammerNode", CCNode*, this->m_hammerNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeLabel", CCLabelIF*, this->m_freeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handLabel", CCLabelIF*, this->m_handLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handBG1", CCScale9Sprite*, this->m_handBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handBG2", CCSprite*, this->m_handBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handBG3", CCSprite*, this->m_handBG3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Node", CCNode*, this->m_info2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hand2Label", CCLabelIF*, this->m_hand2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hand3Label", CCLabelIF*, this->m_hand3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hand2BG1", CCScale9Sprite*, this->m_hand2BG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hand2BG3", CCSprite*, this->m_hand2BG3);
    return false;
}

void BuildQueueCell::onAddParticle()
{
    for (int i=1; i<=2; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("QueueGlow_%d",i)->getCString());
        m_particleNode->addChild(particle);
    }
}

void BuildQueueCell::onShowInfo()
{
    return;
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_handBG1->runAction(CCRepeatForever::create(sequ));
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_handBG2->runAction(CCRepeatForever::create(sequ));
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_handBG3->runAction(CCRepeatForever::create(sequ));
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_handLabel->runAction(CCRepeatForever::create(sequ));
    }
}

void BuildQueueCell::onShow2Info()
{
    m_info2Node->setVisible(true);
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_hand2BG1->runAction(sequ);
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_hand2Label->runAction(sequ);
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_hand2BG3->runAction(sequ);
    }
    if (1) {
        CCFadeIn* fadeIn = CCFadeIn::create(0.5);
        CCDelayTime* delt1 = CCDelayTime::create(3);
        CCFadeOut* fadeOut = CCFadeOut::create(0.5);
        CCDelayTime* delt2 = CCDelayTime::create(15);
        CCSequence* sequ = CCSequence::create(fadeIn, delt1, fadeOut, delt2, NULL);
        m_hand3Label->runAction(sequ);
    }
}

void BuildQueueCell::setQid(int qid)
{
    if (qid>0 && qid!=QID_MAX) {
        m_qid = qid;
        m_iconNode->removeAllChildren();
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        int itemId = atoi(qInfo.key.c_str())/1000;
        string picName = CCCommonUtils::getPropById(CC_ITOA(itemId), "pic");
        if(GlobalData::shared()->contryResType==0){
            picName = picName +".png";
        }
        else{
            picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        }
        auto icon = CCLoadSprite::createSprite(picName.c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 80);
        m_iconNode->addChild(icon);
    }
}

void BuildQueueCell::onEnterFrame(float _time)
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int curTime = GlobalData::shared()->getWorldTime();
    if (m_curState != Queue_ST_LOCK && qInfo.endTime !=0 && qInfo.endTime <= curTime) { //转为未开启队列状态
        m_curState = Queue_ST_LOCK;
        onAddParticle();
        this->getAnimationManager()->runAnimationsForSequenceNamed("Lock");
        removeIntroTip();
    }
    else {
        if (qInfo.endTime==0 || qInfo.endTime > curTime) {//空闲或者工作中的状态
            int lastTime = 0;
            if (qInfo.endTime > 0) {
                lastTime = qInfo.endTime - curTime;//剩余有效时间
            }
            int m_sumTime = qInfo.finishTime - qInfo.startTime;
            m_curTime = qInfo.finishTime - curTime;
            
            if (m_curState != Queue_ST_IDLE && m_curTime<=0) { //转为空闲状态
                removeIntroTip();
                if (m_curState == Queue_ST_LOCK) {
                    m_particleNode->removeAllChildren();
                    m_hand3Label->setString(CC_SECTOA(lastTime));
                    onShow2Info();//刚租的时候提示
                }
                m_curState = Queue_ST_IDLE;
                m_isFree = false;
                m_infoNode->setVisible(false);
                m_handBG1->stopAllActions();
                m_handBG2->stopAllActions();
                m_handBG3->stopAllActions();
                m_handLabel->stopAllActions();
                m_proTimer->setPercentage(0);
                
                m_freeLabel->setString("");
                m_hammerNode->removeAllChildren();
                string hammerPic = "hammer2.png";
                if (m_qid == BUILD_QUEUE_1ST) {
                    hammerPic = "hammer1.png";
                }
                auto pic = CCLoadSprite::createSprite(hammerPic.c_str());
                pic->setScale(1.5);
                m_hammerNode->addChild(pic);
                m_proTimer->setSprite(CCLoadSprite::createSprite("icon_jindutiao.png"));
                
                this->getAnimationManager()->runAnimationsForSequenceNamed("Untitled");
            }
            else {
                if (m_curTime>0) {//工作中状态
                    if (m_curState != Queue_ST_WORK) { //转为工作中的状态
                        m_curState = Queue_ST_WORK;
                        setQid(m_qid);
                        this->getAnimationManager()->runAnimationsForSequenceNamed("Uping");
                    }
                    m_timeLabel->setString(CC_SECTOA(m_curTime));
                    int pro = m_curTime*100.0/m_sumTime;
                    m_proTimer->setPercentage(100-pro);
                    
                    if (!m_isFree && m_curTime<=GlobalData::shared()->freeSpdT) {
                        m_isFree = true;
//                        m_infoNode->setVisible(true);
                        m_hammerNode->removeAllChildren();
                        m_freeLabel->setString(_lang("103672"));
                        if(m_freeLabel->getContentSize().width>100){
                            m_freeLabel->setFontSize(18);
                        }
                        
                        this->getAnimationManager()->runAnimationsForSequenceNamed("Free");
                        m_proTimer->setSprite(CCLoadSprite::createSprite("icon_jindutiao1.png"));
                        m_proTimer->setPercentage(m_proTimer->getPercentage());
                        onShowInfo();
                        removeIntroTip();
                    }else if(m_isFree == false){
                        if(m_curTime<600){
                            addIntroTip();
                        }else{
                            removeIntroTip();
                        }
                    }
                }
                else {
                    removeIntroTip();
                    //空闲状态
                    if (lastTime>0) {
                        if (lastTime == 3600*24 || lastTime == 3600 || lastTime == 600) {
                            m_hand3Label->setString(CC_SECTOA(lastTime));
                            onShow2Info();
                        }
                    }
                }
            }
        }
        else {
            //未开启状态
        }
    }
}

bool BuildQueueCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(this->getParent()){
        if (!this->getParent()->isVisible() ) {
            return false;
        }
    }
    if (isTouchInside(m_touchNode, pTouch) || (m_infoNode->isVisible() && isTouchInside(m_handBG1, pTouch)&& m_handBG1->getOpacity()>5)) {
        return true;
    }
    return false;
}

void BuildQueueCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
    if(m_curState == Queue_ST_LOCK) {
        YesNoDialog::showButtonAndGold(_lang("102296").c_str() , CCCallFunc::create(this, callfunc_selector(BuildQueueCell::onYesOpen)), _lang("102174").c_str(),FunBuildController::getInstance()->building_base_k9);
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (m_curState == Queue_ST_IDLE && qInfo.endTime>0) {
        int lastTime = qInfo.endTime - GlobalData::shared()->getWorldTime();
        if (lastTime>0) {
            m_hand3Label->setString(CC_SECTOA(lastTime));
            onShow2Info();
        }
    }
    
    if (m_qid != QID_MAX && m_curTime>0) {
        string key = GlobalData::shared()->allQueuesInfo[m_qid].key;
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if (layer) {
            layer->onMoveToBuildAndOpen(atoi(key.c_str()), TYPE_POS_CLICK, 0.25, true);
        }
        if (m_isFree) {
            FunBuildController::getInstance()->costCD(atoi(key.c_str()), "", 0);
        }
    }
    else {
        QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
        if (info) {
            if (QuestController::getInstance()->goToQuestTarget(info) ) {
                //跳转不提示
            }else {
                flyHint();
            }
        }
        else {
            flyHint();
        }
    }
}

void BuildQueueCell::onYesOpen()
{
    QueueController::getInstance()->rentQueue(m_qid, FunBuildController::getInstance()->building_base_k8, true);
    FBUtilies::appEventHireWorker(FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level);
}

void BuildQueueCell::flyHint(){
    if(m_flyHintFlag){
        return;
    }
    CCCommonUtils::flyHint("", "", _lang("102295"));
    
    m_flyHintFlag = true;
    this->scheduleOnce(schedule_selector(BuildQueueCell::resetHintFlag), 1.6);
}

void BuildQueueCell::resetHintFlag(float _time){
    m_flyHintFlag = false;
}

void BuildQueueCell::addIntroTip(){
    if(FunBuildController::getInstance()->getMainCityLv()<5){
        removeIntroTip();
        return;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if(playerInfo.vipEndTime>GlobalData::shared()->getWorldTime()){//vip有效,倒计时
        removeIntroTip();
        return;
    }
    if(!m_introTip){
        m_introTip = AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Right,_lang("103046"), 360 , 0);
        addChild(m_introTip);
        m_introTip->setPositionX(60);
//        m_introTip->setScale(0.8);
        m_introTip->setAnimationAndPlay(3,5);
    }
}
void BuildQueueCell::removeIntroTip(){
    if(m_introTip){
        m_introTip->stopAllActions();
        m_introTip->removeFromParent();
        m_introTip=NULL;
    }
}