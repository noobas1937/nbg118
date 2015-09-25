//
//  EquipFinishView.cpp
//  IF
//
//  Created by fubin on 15-1-26.
//
//

#include "EquipFinishView.h"
#include "EquipmentCreateView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "QueueController.h"
#include "CCThreadManager.h"
#include "EquipNewUseView.h"
#include "CountryChatCommand.h"

EquipFinishView* EquipFinishView::create(int itemId){
    EquipFinishView* ret = new EquipFinishView();
    if(ret && ret->init(itemId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipFinishView::init(int itemId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(207, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(207, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_EQUIP);
    });
    
    m_itemId = itemId;
    
    auto bg = CCBLoadFile("EquipFinishView",this,this);
//    setContentSize(bg->getContentSize());
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    m_popLayer = CCLayer::create();
    m_playNode = CCNode::create();
    
    CCCommonUtils::setButtonTitle(m_gotoBtn, _lang("119036").c_str());
    setTitleName(_lang("119012"));
    
    m_titleLabel->setString(_lang("107502"));
    m_msgLabel->setString(_lang("119035"));
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    string strName = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
    m_nameLabel->setString(strName);
    
    string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
    auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bgIcon, 120, true);
    m_iconNode->addChild(bgIcon);
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 120, true);
    m_iconNode->addChild(icon);
    
    m_iconNode->setVisible(false);
    
    onRefreshEquipInfo(eInfo.color);
    m_weapType = eInfo.site;
    m_curPopType = -1;
    
    m_isShare = CCUserDefault::sharedUserDefault()->getBoolForKey("equip_finish_share", true);
    if (m_isShare) {
//        auto checkSpr = CCLoadSprite::createSprite("share_check_box_checked.png");
        m_checkBox->setVisible(true);
    }else{
        m_checkBox->setVisible(false);
    }
    m_shareLab->setString(_lang("111661"));
    
    string sw=GlobalData::shared()->equipShare_control;
    m_finishShareNode->setVisible(false);
    if (sw==""||sw=="0") {
        m_finishShareNode->setVisible(false);
    }else if (sw=="1"){
        m_finishShareNode->setVisible(true);
    }else {
        if (CCCommonUtils::checkVersion(sw)){
            m_finishShareNode->setVisible(true);
        }else{
            m_finishShareNode->setVisible(false);
        }
    }
    return true;
}

void EquipFinishView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    m_initEnd = true;
    if (m_curPopType == 1) {
        m_initEnd = true;
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        showPopParticle(1);
    }else {
        m_initEnd = false;
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(EquipFinishView::animationFunc));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Open");
        showPopParticle(1);
    }
}

void EquipFinishView::onExit(){
    if (m_isShare && m_finishShareNode->isVisible()) {
        string auid = GlobalData::shared()->playerInfo.allianceInfo.uid;
        if(auid!=""){
            auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            if (eInfo.color>=2) {
                string strName = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
                string msg = _lang_1("111660", strName.c_str());
                CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_EQUIP_SHARE);
                cmd->putParam("equipId", CCLong::create(m_itemId));
                cmd->sendAndRelease();
            }
        }
        
    }

    setTouchEnabled(false);
    CCNode::onExit();
}

void EquipFinishView::animationFunc()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "Open") {
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
//        showPopParticle(1);
        m_initEnd = true;
    }
}

SEL_CCControlHandler EquipFinishView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGoToBtn", EquipFinishView::onClickGoToBtn);
    return NULL;
}

bool EquipFinishView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoBtn", CCControlButton*, this->m_gotoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popNode", CCNode*, this->m_popNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, this->m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg4Label", CCLabelIF*, this->m_msg4Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkBox", CCSprite*, this->m_checkBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shareLab", CCLabelIF*, this->m_shareLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_finishShareNode", CCNode*, this->m_finishShareNode);
    
    return false;
}

void EquipFinishView::setCurPopType(int type)
{
    m_curPopType = type; // 1 穿装备面板
}

void EquipFinishView::onClickGoToBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_curPopType == m_weapType) {
        PopupViewController::getInstance()->removePopupView(this);
    }else {
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(EquipNewUseView::create(m_weapType));
    }
    return;
}

bool EquipFinishView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void EquipFinishView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch) && m_initEnd) {
        PopupViewController::getInstance()->removePopupView(this);
    }else if (isTouchInside(m_checkBox, pTouch)) {
        if (m_isShare) {//share_check_box.png//share_check_box_checked.png
            m_isShare = false;
            CCUserDefault::sharedUserDefault()->setBoolForKey("equip_finish_share", false);
            m_checkBox->setVisible(false);
        }else{
            m_isShare = true;
            CCUserDefault::sharedUserDefault()->setBoolForKey("equip_finish_share", true);
            m_checkBox->setVisible(true);
        }
    }
    
}

void EquipFinishView::onRefreshEquipInfo(int color)
{
//    if (color == WHITE) {
//        m_msg1Label->setColor(ccc3(177, 172, 168));
//        m_msg2Label->setColor(ccc3(177, 172, 168));
//        m_msg3Label->setColor(ccc3(177, 172, 168));
//        m_msg4Label->setColor(ccc3(177, 172, 168));
//    }else if (color == GREEN) {
//        m_msg1Label->setColor(ccc3(109, 188, 15));
//        m_msg2Label->setColor(ccc3(109, 188, 15));
//        m_msg3Label->setColor(ccc3(109, 188, 15));
//        m_msg4Label->setColor(ccc3(109, 188, 15));
//    }else if (color == BLUE) {
//        m_msg1Label->setColor(ccc3(25, 175, 197));
//        m_msg2Label->setColor(ccc3(25, 175, 197));
//        m_msg3Label->setColor(ccc3(25, 175, 197));
//        m_msg4Label->setColor(ccc3(25, 175, 197));
//    }else if (color == PURPLE) {
//        m_msg1Label->setColor(ccc3(142, 55, 203));
//        m_msg2Label->setColor(ccc3(142, 55, 203));
//        m_msg3Label->setColor(ccc3(142, 55, 203));
//        m_msg4Label->setColor(ccc3(142, 55, 203));
//    }else if (color == ORANGE) {
//        m_msg1Label->setColor(ccc3(204, 127, 44));
//        m_msg2Label->setColor(ccc3(204, 127, 44));
//        m_msg3Label->setColor(ccc3(204, 127, 44));
//        m_msg4Label->setColor(ccc3(204, 127, 44));
//    }else if (color == GOLDEN) {
//        m_msg1Label->setColor(ccc3(198, 176, 28));
//        m_msg2Label->setColor(ccc3(198, 176, 28));
//        m_msg3Label->setColor(ccc3(198, 176, 28));
//        m_msg4Label->setColor(ccc3(198, 176, 28));
//    }
    
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    for (int i=1; i<=4; i++)
    {
        string tmpInfo = "";
        if (eInfo.showDias.find(i) != eInfo.showDias.end()) {
            string dialog = eInfo.showDias[i];
            if (dialog!="" && eInfo.values.find(i)!=eInfo.values.end()) {
                tmpInfo = tmpInfo + _lang(dialog)+" +"+CC_ITOA_MONEY(eInfo.values[i])+"%";
            }
        }
        
        if (i==1) {
            m_msg1Label->setString(tmpInfo);
            m_msg1Label->setMaxScaleXByWidth(520);
        }else if (i==2) {
            m_msg2Label->setString(tmpInfo);
            m_msg2Label->setMaxScaleXByWidth(520);
        }else if (i==3) {
            m_msg3Label->setString(tmpInfo);
            m_msg3Label->setMaxScaleXByWidth(520);
        }else if (i==4) {
            m_msg4Label->setString(tmpInfo);
            m_msg4Label->setMaxScaleXByWidth(520);
        }
    }
}

void EquipFinishView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        auto &batch = m_parPopVec[batchCount];
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
    m_popLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parPopVec.push_back(newBatch);
}

void EquipFinishView::showPopParticle(float dt)
{
    removePopParticle(0);
    m_popNode->removeAllChildren();
    m_popNode->addChild(m_popLayer);
    m_popNode->addChild(m_playNode);
    
    playBombEff();
    return;

    string strEquipTmp = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
    m_popLayer->addChild(iconEquipTmp);
    
    iconEquipTmp->setScale(0.1);
    CCFiniteTimeAction *pAction1 = CCSequence::create(CCScaleTo::create(0.35, 1.5),CCScaleTo::create(0.15, 1.2),NULL);
    iconEquipTmp->runAction(CCSequence::create(pAction1, NULL));
    
    this->scheduleOnce(schedule_selector(EquipFinishView::removePopParticle), 1);
}

void EquipFinishView::playParticle()
{
    int curColor = -1;
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    curColor = eInfo.color;
    string tmpStart = "MallBag_";
    int count = 3;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if (curColor == WHITE) {
            particle->setStartColor(ccc4f(1, 1, 1, 1));
        }else if (curColor == GREEN) {
            particle->setStartColor(ccc4f(0, 1, 0, 1));
        }else if (curColor == BLUE) {
            particle->setStartColor(ccc4f(0, 1, 1, 1));
        }else if (curColor == PURPLE) {
            particle->setStartColor(ccc4f(1, 0, 1, 1));
        }else if (curColor == ORANGE) {
            particle->setStartColor(ccc4f(1, 0.5, 0, 1));
        }else if (curColor == GOLDEN) {
            particle->setStartColor(ccc4f(1, 1, 0, 1));
        }
        addPopParticleToBatch(particle);
    }
//    m_popLayer->setScale(1.5);
}

void EquipFinishView::playBombEff()
{
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    
    auto playBg = CCLoadSprite::createSprite("Lord_icon_box.png");
    playBg->setScale(1.4);
    m_playNode->addChild(playBg);
    
    string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
    auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bgIcon, 120, true);
    m_playNode->addChild(bgIcon);
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 120, true);
    m_playNode->addChild(icon);
    
    CCActionInterval * rotateTo1 = CCRotateTo::create(0.1,180);
    CCActionInterval * rotateTo2 = CCRotateTo::create(0.1,360);
    CCActionInterval * rotateTo3 = CCRotateTo::create(0,0);
    CCActionInterval * repeat = CCRepeat::create(CCSequence::create(rotateTo1,rotateTo2,rotateTo3,NULL), 3);
    
    CCActionInterval * scaleTo0 = CCScaleTo::create(0.6,1.5);
    CCFiniteTimeAction * spawn0 =CCSpawn::create(repeat,scaleTo0,NULL);
    
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipFinishView::playParticle));
    CCActionInterval * delayT = CCDelayTime::create(1);
    CCActionInterval * scaleTo1 = CCScaleTo::create(0.2,1.0);
    
    m_playNode->setScale(0);
    m_playNode->runAction(CCSequence::create(spawn0, funcall, delayT, scaleTo1, NULL));
}

void EquipFinishView::removePopParticle(float dt)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
//    m_popNode->removeAllChildren();
}