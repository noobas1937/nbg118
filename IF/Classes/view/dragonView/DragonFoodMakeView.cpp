//
//  DragonFoodMakeView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

//#include "DragonFoodMakeView.h"
//#include "PopupViewController.h"
//#include "ParticleController.h"
//#include "ToolController.h"
//#include "QueueController.h"
//#include "GuideController.h"
//
//DragonFoodMakeView* DragonFoodMakeView::create(){
//    DragonFoodMakeView* ret = new DragonFoodMakeView();
//    if(ret && ret->init()){
//        ret->autorelease();
//    }else{
//        CC_SAFE_DELETE(ret);
//    }
//    return ret;
//}
//
//bool DragonFoodMakeView::init()
//{
//    if (!PopupBaseView::init()) {
//        return false;
//    }
//    
//    auto bg = CCBLoadFile("DragonFoodMakeView",this,this);
//    setContentSize(bg->getContentSize());
//    
//    setTitleName(_lang("119008"));
//    
//    return true;
//}
//
//void DragonFoodMakeView::onEnter(){
//    CCNode::onEnter();
//}
//
//void DragonFoodMakeView::onExit(){
//    CCNode::onExit();
//}
//
//SEL_CCControlHandler DragonFoodMakeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
//{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", DragonFoodMakeView::onOKClick);
//    return NULL;
//}
//
//bool DragonFoodMakeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
//{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
//    return false;
//}
//
//void DragonFoodMakeView::onOKClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
//{
//    CCLOG(" OOOOOOOOOO ok");
//}

//***************************************************************************************

#include "DragonFoodMakeView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "CCThreadManager.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "UseToolView.h"
#include "SoundController.h"
#include "HelpshiftCocos2dx.h"
#include "VipUtil.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "FunBuild.h"
#include "GuideController.h"

DragonFoodMakeView* DragonFoodMakeView::create(){
    DragonFoodMakeView* ret = new DragonFoodMakeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonFoodMakeView::init()
{
    if (!ArcPopupBaseView::init(TYPE_POS_MID)) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(4, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    
    startPt = ccp(0, 0);
    auto bg = CCBLoadFile("DragonFoodMakeView",this,this);
    int extH = getExtendHeight();
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_flyNode->setVisible(false);
    
    int tmpPoxY = 70;
    if (extH/4 < tmpPoxY) {
        tmpPoxY = extH/4;
    }
    m_makeNode->setPositionY(m_makeNode->getPositionY()-tmpPoxY);
    
    m_buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WORKSHOP);
    
    m_toolNodeVec.push_back(m_tool1Node);
    m_toolNodeVec.push_back(m_tool2Node);
    m_toolNodeVec.push_back(m_tool3Node);
    m_toolNodeVec.push_back(m_tool4Node);
    m_toolNodeVec.push_back(m_tool5Node);
    m_toolNodeVec.push_back(m_tool6Node);
    m_toolNodeVec.push_back(m_tool7Node);
    m_toolNodeVec.push_back(m_tool8Node);
    m_toolNodeVec.push_back(m_tool9Node);
    m_toolNodeVec.push_back(m_tool10Node);
    m_toolNodeVec.push_back(m_tool11Node);
    m_toolNodeVec.push_back(m_tool12Node);
    
    m_makeTools = ToolController::getInstance()->m_makeTools;
    for (int i=0; i<m_makeTools.size(); i++)
    {
        if (i<m_toolNodeVec.size())
        {
            CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
            string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_makeTools[i]));
            auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
            icon->setPosition(ccp(58, 58));
            CCCommonUtils::setSpriteMaxSize(icon, 90);
            m_toolNodeVec[i]->addChild(icon);
        }
    }
    m_tipNode->setVisible(false);
    m_spdIcon->setVisible(false);
    
    m_sumGold = 99999;
    m_oldSumTime = 1;
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int TmpItemId = QueueController::getInstance()->getItemId(qInfo.key);
        m_curCreatingItemId = TmpItemId;
        m_qid = tqid;
        
        CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_curCreatingItemId));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(icon, 120);
        m_curIconNode->addChild(icon);
        
        CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
        CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
        CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
        icon->runAction(repeatForever);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_curCreatingItemId);
        m_sumGold = toolInfo.crt_gold;
        m_oldSumTime = toolInfo.crt_cd;
    }
    
    onEnterFrame(0);
    _openNum = 0;
    m_curOptIdx = -1;
    m_willIconPath = "";
    m_waitInterface = NULL;
    return true;
}

void DragonFoodMakeView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DragonFoodMakeView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonFoodMakeView::updateCrtRet), MATETOOL_CREATE_END, NULL);
    
    _openNum++;
    if (_openNum>1 && m_qid == QID_MAX) {
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    
//    string key = CCUserDefault::sharedUserDefault()->getStringForKey("firstOpenDragonFoodMakeView", "");
//    if (key == "") {
//        CCUserDefault::sharedUserDefault()->setStringForKey("firstOpenDragonFoodMakeView", "3120100");
//        GuideController::share()->setGuide("3120100");
//    }
}

void DragonFoodMakeView::onExit(){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GlobalData::shared()->isBind = false;
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DragonFoodMakeView::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MATETOOL_CREATE_END);
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler DragonFoodMakeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool DragonFoodMakeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_makeNode", CCNode*, this->m_makeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_make2Node", CCNode*, this->m_make2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curIconNode", CCNode*, this->m_curIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lastTime", CCLabelIF*, this->m_lastTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdIcon", CCSprite*, this->m_spdIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", CCNode*, this->m_tipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBg", CCSprite*, this->m_tipBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipName", CCLabelIF*, this->m_tipName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTime", CCLabelIF*, this->m_tipTime);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNode", CCNode*, this->m_flyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool1Node", CCNode*, this->m_tool1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool2Node", CCNode*, this->m_tool2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool3Node", CCNode*, this->m_tool3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool4Node", CCNode*, this->m_tool4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool5Node", CCNode*, this->m_tool5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool6Node", CCNode*, this->m_tool6Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool7Node", CCNode*, this->m_tool7Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool8Node", CCNode*, this->m_tool8Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool9Node", CCNode*, this->m_tool9Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool10Node", CCNode*, this->m_tool10Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool11Node", CCNode*, this->m_tool11Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool12Node", CCNode*, this->m_tool12Node);
    
    return false;
}

void DragonFoodMakeView::onOKCreateMate()
{
    if (m_qid == QID_MAX)
    {
        m_waitInterface= GameController::getInstance()->showWaitInterface(m_touchNode);
        m_waitInterface->setPosition(ccp(m_touchNode->getContentSize().width/2, m_touchNode->getContentSize().height/2));
        int itemId = m_makeTools[m_curOptIdx];
        //        ToolController::getInstance()->flyToolReward(itemId);
//        ToolController::getInstance()->startMakeTool(itemId);
    }
    else
    {
        CCCommonUtils::flyHint("", "", _lang("111401"));
    }
}

void DragonFoodMakeView::onCostGoldBack()
{
    if (m_qid != QID_MAX) {
        if (_tmpGold > 0) {
            if (GlobalData::shared()->playerInfo.gold < _tmpGold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, _tmpGold,"",5);
    }
}

void DragonFoodMakeView::updateCrtRet(CCObject* p)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    //    this->getAnimationManager()->runAnimationsForSequenceNamed("Start");
    
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int itemId = QueueController::getInstance()->getItemId(qInfo.key);
        m_curCreatingItemId = itemId;
        m_qid = tqid;
        
        m_curIconNode->removeAllChildren();
        CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_curCreatingItemId));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(icon, 120, true);
        m_curIconNode->addChild(icon);
        
        CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
        CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
        CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
        icon->runAction(repeatForever);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_curCreatingItemId);
        m_sumGold = toolInfo.crt_gold;
        m_oldSumTime = toolInfo.crt_cd;
    }
    onEnterFrame(0);
}

bool DragonFoodMakeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    startPt = pTouch->getLocation();
    for (int i=0; i<m_toolNodeVec.size(); i++) {
        if ( isTouchInside(m_toolNodeVec[i], pTouch) ) {
            refreshFlyNode(i);
            break;
        }
    }
    
    return true;
}

void DragonFoodMakeView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    auto curPt = pTouch->getLocation();
    auto flyPos = m_flyNode->getParent()->convertTouchToNodeSpace(pTouch);
    m_flyNode->setPosition(flyPos);
    if (m_qid == QID_MAX && m_flyNode->isVisible()) {
        if (isTouchInside(m_makeNode, pTouch) || isTouchInside(m_make2Node, pTouch)) {
            onShowWillMate(true);
        }else {
            onShowWillMate(false);
        }
    }
}

void DragonFoodMakeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_willIconPath = "";
    m_tipNode->setVisible(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
    auto endPt = pTouch->getLocation();
    
    if (m_flyNode->isVisible())
    {
        if (isTouchInside(m_makeNode, pTouch) || isTouchInside(m_make2Node, pTouch))
        {
            onOKCreateMate();
        }
        m_flyNode->setVisible(false);
        if (m_curOptIdx>=0 && m_curOptIdx<m_toolNodeVec.size()) {
            m_toolNodeVec[m_curOptIdx]->setVisible(true);
        }
    }
    else
    {
        if (isTouchInside(m_makeNode, pTouch) || isTouchInside(m_touchNode, pTouch))
        {
            if (m_qid != QID_MAX && isTouchInside(m_makeNode, pTouch))
            {
                YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(DragonFoodMakeView::onCostGoldBack)), m_curTime, _lang("104903").c_str(), false, m_sumTime, m_sumGold);
            }
            
        }else {
            //            this->closeSelf();
            playClose();
        }
    }
}

void DragonFoodMakeView::refreshFlyNode(int idx)
{
    if (idx>=m_makeTools.size() || idx>=m_toolNodeVec.size()) {
        return;
    }
    
    int itemId = m_makeTools[idx];
    m_flyNode->removeAllChildren();
    m_flyNode->setVisible(true);
    m_curOptIdx = idx;
    
    m_tipBg->setFlipX(false);
    if (idx == 0) {
        m_tipNode->setPosition(ccp(-30, 365));
    }else if (idx == 1) {
        m_tipNode->setPosition(ccp(-126, 308));
    }else if (idx == 2) {
        m_tipNode->setPosition(ccp(-170, 205));
    }else if (idx == 3) {
        m_tipNode->setPosition(ccp(-170, 93));
    }else if (idx == 4) {
        m_tipNode->setPosition(ccp(-124, -12));
    }else if (idx == 5) {
        m_tipNode->setPosition(ccp(-26, -73));
    }else if (idx == 6) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(30, 365));
    }else if (idx == 7) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(126, 308));
    }else if (idx == 8) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(170, 205));
    }else if (idx == 9) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(170, 93));
    }else if (idx == 10) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(124, -12));
    }else if (idx == 11) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(26, -73));
    }
    
    auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    
    string iconPath = "";
    CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
    string toolId = CC_ITOA(itemId);
    iconPath = CCCommonUtils::getIcon(toolId);
    m_willIconPath = iconPath;
    auto flyIcon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
    CCCommonUtils::setSpriteMaxSize(flyIcon, 160, true);
    m_flyNode->setPosition(m_toolNodeVec[idx]->getPosition());
    m_flyNode->addChild(flyIcon);
    
    m_toolNodeVec[idx]->setVisible(false);
    
    m_tipName->setString( CCCommonUtils::getNameById(toolId) );
    m_tipTime->setString( CC_SECTOA(toolInfo.crt_cd) );
    
    m_tipNode->setVisible(true);
}

void DragonFoodMakeView::onShowWillMate(bool show)
{
    if (m_qid == QID_MAX)
    {
        if (show && m_willIconPath!="")
        {
            if (m_curIconNode->getChildrenCount() <= 0) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
            }
            m_curIconNode->removeAllChildren();
            CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
            auto icon = CCLoadSprite::createSprite(m_willIconPath.c_str(),true,loadType);
            CCCommonUtils::setSpriteMaxSize(icon, 120);
            CCCommonUtils::setSpriteGray(icon, true);
            m_curIconNode->addChild(icon);
        }
        else {
            if (m_curIconNode->getChildrenCount() > 0) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
            }
            m_curIconNode->removeAllChildren();
        }
    }
}

void DragonFoodMakeView::flyEnd()
{
    m_flyNode->removeAllChildren();
    m_flyNode->setScale(1.0);
}

void DragonFoodMakeView::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if(qInfo.startTime==0){
        qInfo.startTime = tmpCurTime;
    }
    m_sumTime = qInfo.finishTime - qInfo.startTime;
    m_curTime = qInfo.finishTime - tmpCurTime;
    m_curTime = MAX(0, m_curTime);
    m_sumTime = MAX(1, m_sumTime);
    
    if (m_curTime>0) {
        m_spdIcon->setVisible(true);
        if (qInfo.finishTime<=0) {
            m_curTime = 0;
            m_sumTime = 1;
        }
        m_lastTime->setString(CC_SECTOA(m_curTime));
        _tmpGold = m_sumGold*(m_curTime*1.0/m_oldSumTime)+0.5;
        _tmpGold = MAX(1, _tmpGold);
    }
    else {
        QueueController::getInstance()->autoHavestMateTool(m_qid, m_buildId);
        m_spdIcon->setVisible(false);
        m_lastTime->setString("");
        m_curIconNode->removeAllChildren();
        m_qid = QID_MAX;
        //        this->getAnimationManager()->runAnimationsForSequenceNamed("End");
    }
}

void DragonFoodMakeView::playClose()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DragonFoodMakeView::AnimationClose));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void DragonFoodMakeView::AnimationClose()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(DragonFoodMakeView::closeView),NULL);
}

void DragonFoodMakeView::closeView(CCObject* params)
{
    //    PopupViewController::getInstance()->goBackPopupView();
    this->closeSelf();
}

void DragonFoodMakeView::AnimationFadeIn()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name=="FadeIn" && m_qid!=QID_MAX) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
}