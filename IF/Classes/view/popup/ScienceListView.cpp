//
//  ScienceListView.cpp
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#include "ScienceListView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ScienceController.h"
#include "ScienceView.h"
#include "ParticleController.h"
#include "QueueController.h"
#include "UserUpgradeView.h"
#include "CCThreadManager.h"

ScienceListView* ScienceListView::create(int buildId){
    ScienceListView* ret = new ScienceListView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ScienceListView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });
    
    m_buildId = buildId;
    
    m_smokeLayer = CCLayer::create();
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -100;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 1));
//        if (CCCommonUtils::isIosAndroidPad()) {
//            bg->setScale(2.4);
//        }
////        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        bg->runAction(CCFadeIn::create(0.5));
//        tBatchNode->addChild(bg);
//    }
//    this->addChild(tBatchNode);
    
    this->addChild(m_smokeLayer);

    
    CCBLoadFile("ScienceListView",this,this);
   
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    m_name1BG->setVisible(false);
    m_name2BG->setVisible(false);
    m_name3BG->setVisible(false);
    m_name4BG->setVisible(false);
    
    int addHeight = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY() + addHeight / 3);
//    m_sceinceNode->setPositionY(m_sceinceNode->getPositionY()+ addHeight / 3 * 2);
    m_sceinceNode->setPositionY(m_sceinceNode->getPositionY()+ addHeight);//fusheng   顶到头
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    m_title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
    m_smokeLayer->setPosition(m_mainNode->getPosition());
    if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
        m_title += _lang("102183");
    }
    setTitleName(m_title.c_str());
    
    m_desLabel->setString(_lang("121990"));
    
//    for (int i=1; i<=4; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode1->addChild(particle);
//        
//        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode2->addChild(particle1);
//    }
    CCCommonUtils::setButtonTitle(m_btn, _lang("120153").c_str());
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationCallback));
//    addBGPic();
    
    auto pos = m_progrossBar->getPosition();
    m_barClipNode = CCClipNode::create(m_progrossBar->getContentSize().width, m_progrossBar->getContentSize().height);
    m_progrossBar->getParent()->addChild(m_barClipNode);
    m_barClipNode->setAnchorPoint(ccp(0, 0));
    m_progrossBar->retain();
    m_progrossBar->removeFromParent();
    m_barClipNode->addChild(m_progrossBar);
    m_progrossBar->setPosition(0, 0);
    CC_SAFE_RELEASE_NULL(m_progrossBar);
    m_barClipNode->setPosition(pos);
    
    m_timeText->setZOrder(1);
    update(1.0f);
    
    
    changeBGMaxHeight(m_buildBG);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this](Touch* touch, Event* event)
    {
       
        if (isTouchInside(m_buildBG, touch)) {
            return true;
        }
        return false;
    };
    
    listener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_buildBG);
    
    return true;
}

void ScienceListView::addBGPic(){
    int totalH = 0;
    auto size = CCDirector::sharedDirector()->getWinSize();
    while (totalH < size.height) {
//        auto sprite = CCLoadSprite::createSprite("kji_bg.png");
        auto sprite = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(55, 0.77, 1.0), 0, 1.04, 4.4);
        this->m_bgContainer->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 0));
        sprite->setPosition(ccp(0, totalH));
        totalH += sprite->getContentSize().height;
    }
}

void ScienceListView::updateInfo()
{
    m_dataCnt = ScienceController::getInstance()->m_scienceType.size();
    for (int i=0; i<m_dataCnt; i++) {
        int type = ScienceController::getInstance()->m_scienceType[i];
        string posStr = CCCommonUtils::getPropById(CC_ITOA(type), "position");
        int pos = atoi(posStr.c_str());
        if (pos>=1 && pos<=4) {
            string picName = CCCommonUtils::getIcon(CC_ITOA(type));
            string sName = CCCommonUtils::getNameById(CC_ITOA(type));
            updateScienceType(pos, sName, picName);
            m_posTypes[pos] = type;
        }
    }
    
    m_name1BG->setVisible(true);
    m_name2BG->setVisible(true);
    m_name3BG->setVisible(true);
    m_name4BG->setVisible(true);
}

void ScienceListView::update(float time){
    bool flag = false;
    auto &map = GlobalData::shared()->scienceInfo.infoMap;
    for (auto it = map.begin(); it != map.end(); it++) {
        if(it->second.finishTime > GlobalData::shared()->getWorldTime()){
            flag = true;
            m_btn->setVisible(true);
            m_hintText->setString("");
            std::string name = it->second.getName();
            std::string description = it->second.getDescription();
            m_nameText->setString(name);
            m_descriptionText->setString(description);
            m_progressBG->setVisible(true);
            m_progrossBar->setVisible(true);
            int time = it->second.finishTime - GlobalData::shared()->getWorldTime();
            m_timeText->setString(CCCommonUtils::timeLeftToCountDown(time));
            int sumTime = it->second.time_need;
            int totalW = 342;
            if(time > sumTime){
                time = sumTime;
            }
            float w = totalW - time * 1.0  * totalW / sumTime;
            m_barClipNode->setContentSize(CCSize(w, m_progrossBar->getContentSize().height));
            break;
        }
    }
    if(!flag){
        m_progressBG->setVisible(false);
        m_progrossBar->setVisible(false);
        m_nameText->setString("");
        m_timeText->setString("");
        m_descriptionText->setString("");
        m_hintText->setString(_lang("120154"));
        m_btn->setVisible(false);
        m_timeSpr->setVisible(false);
    }
}

void ScienceListView::AnimationCallback()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    string name = getAnimationManager()->getLastCompletedSequenceName();
    if(name == "FadeIn") {
        
        playCircleGlow();
        updateInfo();
        
        
        Size winSize = CCDirector::getInstance()->getWinSize();
        
        if (winSize.height/winSize.width < 1.6) {
            m_magicCircle->runAction(Sequence::createWithTwoActions( CCScaleTo::create(0.3, 1),CallFunc::create([this]{
                m_magicCircle->setScale(1);
                
            })));
            
            m_node1->runAction(CCMoveBy::create(0.3, Vec2(20,-20)));
            
            m_node2->runAction(CCMoveBy::create(0.3, Vec2(-20,-20)));
            
            m_node3->runAction(CCMoveBy::create(0.3, Vec2(20,20)));
            
            m_node4->runAction(CCMoveBy::create(0.3, Vec2(-20,20)));
            
            
            this->runAction(Sequence::createWithTwoActions(CCDelayTime::create(0.35), CallFunc::create([this]{
                m_isInit = true;
                
            })));

        }
        else
        {
            m_isInit = true;
        }
        
        
//        int addHeight = getExtendHeight();
//        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addHeight/2);
    }
}

void ScienceListView::playClose(int type, bool force)
{
    if(m_posTypes.find(type) == m_posTypes.end() && !force) {
        return;
    }
    m_clickType = type;
    stopCircleGlow(NULL);
    
    m_name1BG->setVisible(false);
    m_name2BG->setVisible(false);
    m_name3BG->setVisible(false);
    m_name4BG->setVisible(false);
    
    m_name1Label->setString("");
    m_name2Label->setString("");
    m_name3Label->setString("");
    m_name4Label->setString("");
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationClose));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void ScienceListView::AnimationClose()
{
    if (m_clickType == 0) {
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(ScienceListView::closeView),NULL);
    }
    else {
        if (m_clickType>=1 && m_clickType<=4) {
            int type = m_posTypes[m_clickType];
            PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, type));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("SC1_%d", type));
        }
    }
}

void ScienceListView::closeView(CCObject* params)
{
    PopupViewController::getInstance()->goBackPopupView();
}

void ScienceListView::playBtnSmoke(float _time)
{
    return;
    auto btnSmoke1 = ParticleController::createParticle("ScienceSmoke");
    auto btnSmoke2 = ParticleController::createParticle("ScienceSmoke");
    auto btnSmoke3 = ParticleController::createParticle("ScienceSmoke");
    auto btnSmoke4 = ParticleController::createParticle("ScienceSmoke");
    auto btnSmoke5 = ParticleController::createParticle("ScienceSmoke");
    
    btnSmoke1->setPosition(m_node1->getPosition());
    btnSmoke2->setPosition(m_node2->getPosition());
    btnSmoke3->setPosition(m_node3->getPosition());
    btnSmoke4->setPosition(m_node4->getPosition());
    btnSmoke5->setPosition(m_node5->getPosition());
    
    addParticleToBatch(btnSmoke1);
    addParticleToBatch(btnSmoke2);
    addParticleToBatch(btnSmoke3);
    addParticleToBatch(btnSmoke4);
    addParticleToBatch(btnSmoke5);
}

void ScienceListView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_smokeLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void ScienceListView::onEnter()
{
    CCNode::onEnter();

    m_name1Label->setString("");
    m_name2Label->setString("");
    m_name3Label->setString("");
    m_name4Label->setString("");
    CCLoadSprite::doResourceByCommonIndex(4, true);
    
    m_clickType = -1;
    setTitleName(m_title.c_str());
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    scheduleOnce(schedule_selector(ScienceListView::playBtnSmoke), 0.5f);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceListView::stopCircleGlow), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void ScienceListView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_smokeLayer->removeAllChildren();
    m_parVec.clear();
    if (false) {
        stopCircleGlow(NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    }
    
    m_isInit = false;
//    CCLoadSprite::doResourceByCommonIndex(4, false);
    CCNode::onExit();
}

SEL_CCControlHandler ScienceListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", ScienceListView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", ScienceListView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", ScienceListView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", ScienceListView::onBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn5Click", ScienceListView::onBtn5Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClearCDClick", ScienceListView::onClearCDClick);
    return NULL;
}

bool ScienceListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_magicCircle", CCSprite*, this->m_magicCircle);

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1Node", CCNode*, this->m_icon1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2Node", CCNode*, this->m_icon2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3Node", CCNode*, this->m_icon3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon4Node", CCNode*, this->m_icon4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon5Node", CCNode*, this->m_icon5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Label", CCLabelIF*, this->m_name1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2Label", CCLabelIF*, this->m_name2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3Label", CCLabelIF*, this->m_name3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4Label", CCLabelIF*, this->m_name4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5Label", CCLabelIF*, this->m_name5Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, this->m_btn5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", CCNode*, this->m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", CCNode*, this->m_node5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crNode1", CCNode*, this->m_crNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crNode2", CCNode*, this->m_crNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crNode3", CCNode*, this->m_crNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crNode4", CCNode*, this->m_crNode4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1BG", CCScale9Sprite*, this->m_name1BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2BG", CCScale9Sprite*, this->m_name2BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3BG", CCScale9Sprite*, this->m_name3BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4BG", CCScale9Sprite*, this->m_name4BG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sceinceNode", CCNode*, this->m_sceinceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBG", CCScale9Sprite*, this->m_progressBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progrossBar", CCScale9Sprite*, this->m_progrossBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeSpr", CCSprite*, this->m_timeSpr);


    return false;
}

CCNode *ScienceListView::getGuideNode(std::string sid)
{
    if (m_isInit) {
        string posStr = CCCommonUtils::getPropById(sid, "position");
        int pos = atoi(posStr.c_str());
        if (pos==1) {
            return m_btn1;
        }
        else if (pos==2) {
            return m_btn2;
        }
        else if (pos==3) {
            return m_btn3;
        }
        else if (pos==4) {
            return m_btn4;
        }
    }
    return NULL;
}

void ScienceListView::updateScienceType(int pos, string name, string pic)
{
    auto picSpr = CCLoadSprite::createSprite(pic.c_str());
    if (pos == 1) {
        m_icon1Node->removeAllChildren();
        m_name1Label->setString(name);
        CCCommonUtils::setSpriteGray(picSpr, true);
        m_icon1Node->addChild(picSpr);
    }
    else if (pos == 2) {
        m_icon2Node->removeAllChildren();
        m_name2Label->setString(name);
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_name2Label->getContentSize().width;
            float s = m_name2Label->getScaleX();
            if (fw>220) {//防爆框
                m_name2Label->setScaleX(220/fw*s);
            }
        }
        else
            m_name2Label->setMaxScaleXByWidth(135);
        CCCommonUtils::setSpriteGray(picSpr, true);
        m_icon2Node->addChild(picSpr);
    }
    else if (pos == 3) {
        m_icon3Node->removeAllChildren();
        m_name3Label->setString(name);
        CCCommonUtils::setSpriteGray(picSpr, true);
        m_icon3Node->addChild(picSpr);
    }
    else if (pos == 4) {
        m_icon4Node->removeAllChildren();
        m_name4Label->setString(name);
        m_icon4Node->addChild(picSpr);
    }
    else if (pos == 5) {
        m_icon5Node->removeAllChildren();
        m_name5Label->setString(name);
        m_icon5Node->addChild(picSpr);
    }
    
    //fusheng 添加一个fadeIn效果
    if(picSpr->getContentSize().width!=2 || picSpr->getContentSize().height!=2 )
    {
        picSpr->setOpacity(0);
        
        picSpr->runAction(CCFadeIn::create(0.3));
    }
}

void ScienceListView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //fusheng 屏蔽功能
    CCCommonUtils::flyHint("", "", _lang("E100008"));
    return;
    playClose(1);
//    int type = m_posTypes[1];
//    PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, type));
}

void ScienceListView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    UserUpgradeView *pop = UserUpgradeView::create();
//    if (PopupViewController::getInstance()->pushPop(pop)) {
//        PopupViewController::getInstance()->addPopupView(pop);
//    }
    //fusheng 屏蔽功能
    CCCommonUtils::flyHint("", "", _lang("E100008"));
    return;
    playClose(2);
}

void ScienceListView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //fusheng 屏蔽功能
    CCCommonUtils::flyHint("", "", _lang("E100008"));
    return;
    playClose(3);
}

void ScienceListView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(4);
}

void ScienceListView::onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    return;
//    int type = m_posTypes[5];
//    PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, type));
}

void ScienceListView::onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto &map = GlobalData::shared()->scienceInfo.infoMap;
    for (auto it = map.begin(); it != map.end(); it++) {
        if(it->second.finishTime > GlobalData::shared()->getWorldTime()){
            PopupViewController::getInstance()->addPopupView(SciencePopupView::create(it->second.scienceId));
            return;
        }
    }
}


void ScienceListView::playCircleGlow()
{
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
    if (qid != QID_MAX) {
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int sid = QueueController::getInstance()->getItemId(key);
        auto& info = GlobalData::shared()->scienceInfo.infoMap[sid];
        int sType = info.scienceType;
        string posStr = CCCommonUtils::getPropById(CC_ITOA(sType), "position");
        int pos = atoi(posStr.c_str());
        
        CCLoadSprite::doResourceByCommonIndex(4, true);
        auto circle = CCLoadSprite::createSprite("IconGlow.png");
//        circle->setScale(2.0);
        CCActionInterval * rotateto1 = CCRotateTo::create(4, -180);
        CCActionInterval * rotateto2 = CCRotateTo::create(4, -360);
        CCActionInterval * rotateto3 = CCRotateTo::create(0, 0);
        circle->runAction(CCRepeatForever::create(CCSequence::create(rotateto1, rotateto2, rotateto3, NULL)));
        circle->runAction(CCFadeIn::create(0.5));
        circle->setTag(99);
        if (pos==1) {
            m_crNode1->addChild(circle);
        }
        if (pos==2) {
            m_crNode2->addChild(circle);
        }
        if (pos==3) {
            m_crNode3->addChild(circle);
        }
        if (pos==4) {
            m_crNode4->addChild(circle);
        }
        
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
    else {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
}

void ScienceListView::stopCircleGlow(CCObject* obj)
{
    m_crNode1->removeChildByTag(99);
    m_crNode2->removeChildByTag(99);
    m_crNode3->removeChildByTag(99);
    m_crNode4->removeChildByTag(99);
}