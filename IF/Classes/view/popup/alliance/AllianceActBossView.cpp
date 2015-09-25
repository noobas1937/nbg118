//
//  AllianceActBossView.cpp
//  IF
//
//  Created by 付彬 on 15/4/27.
//
//

#include "AllianceActBossView.h"
#include "PopupViewController.h"
#include "ActivityTipView.h"
#include "TipsView.h"
#include "JoinAllianceView.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "AllianceWarView.h"
#include "AllianceActivityRewardView.h"
#include "HtmlLabelTFNode.h"
#include "AllianceManager.h"
#include "MonsterSiegeStartCommand.h"
#include "MonsterSiegeActivityInfoCommand.h"
#include "ActivityController.h"
#include "WorldActivityCommand.h"

AllianceActBossView *AllianceActBossView::create(ActivityEventObj* info){
    AllianceActBossView *ret = new AllianceActBossView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceActBossView::init()
{
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        });
        this->setModelLayerOpacity(160);
        auto node = CCBLoadFile("AllianceActBossView", this, this);
        this->setContentSize(node->getContentSize());
        m_titleTxt->setString(_lang("137444"));
        
        CCCommonUtils::setButtonTitle(m_btnLook, _lang("137462").c_str());
        CCCommonUtils::setButtonTitle(m_btnBattle, _lang("137447").c_str());
        CCCommonUtils::setButtonTitle(m_btnAddAlliance, _lang("137448").c_str());
        
        m_readyNode->setVisible(false);
        m_startTip->setString(_lang("137443"));
        m_startNode->setVisible(false);
        
        this->unschedule(schedule_selector(AllianceActBossView::scheduleHandle));
        this->schedule(schedule_selector(AllianceActBossView::scheduleHandle), 1);
        scheduleHandle(0);
        if (m_monsterSp->getParent()) {
            m_monsterSp->setVisible(false);
            auto bossSpr =  CCLoadSprite::createSprite("scws_bust.png",true,CCLoadSpriteType_MONSTERLAYERBUST);
            bossSpr->setPosition(m_monsterSp->getPosition());
            m_monsterSp->getParent()->addChild(bossSpr);
        }else{
            
        }
        ret = true;
    }
    return ret;
}

void AllianceActBossView::scheduleHandle(float _time)
{
    int curTime = GlobalData::shared()->getWorldTime();
    
    if (curTime < m_info->startTime) {
        //活动未开始
        m_readyNode->setVisible(true);
        m_startNode->setVisible(false);
        int lastTime = m_info->startTime - curTime;
        m_readyTime->setString( _lang_2("137445", _lang("137444").c_str(), CC_SECTOA(lastTime)) );
    }else {
        m_readyNode->setVisible(false);
        m_startNode->setVisible(true);
        
        if(!GlobalData::shared()->playerInfo.isInAlliance())
        {//没有联盟的
            m_btnAddAlliance->setVisible(true);
            m_btnBattle->setVisible(false);
            m_btnLook->setVisible(false);
        }
        else
        {
            m_btnAddAlliance->setVisible(false);
            m_btnBattle->setVisible(true);
            m_btnLook->setVisible(true);
        }
        
        if (curTime > m_info->endTime) {
            //活动结束
        }else {
//            int lastTime = m_info->endTime - curTime;
//            m_readyTime->setString( _lang_2("137445", "", CC_SECTOA(lastTime)) );
        }
    }
}

void AllianceActBossView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
}

void AllianceActBossView::onExit(){
    if (m_waitInterface) {
        m_waitInterface->removeFromParent();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceActBossView::goToWorld(){
    int m_worldIndex = WorldController::getIndexByPoint(ccp(ActivityController::getInstance()->aActivityInfo.posX, ActivityController::getInstance()->aActivityInfo.posY));
    CCPoint pt = WorldController::getPointByIndex(m_worldIndex);
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance()){
        WorldMapView::instance()->gotoTilePoint(pt);
        WorldMapView::instance()->openTilePanel(m_worldIndex);
    }else{
        WorldController::getInstance()->openTargetIndex = m_worldIndex;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_worldIndex);
    }
}

bool AllianceActBossView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}
void AllianceActBossView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_clickArea, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler AllianceActBossView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", AllianceActBossView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddAlliance", AllianceActBossView::onAddAlliance);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJijie", AllianceActBossView::onJijie);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLookTroops", AllianceActBossView::onLookTroops);
    return NULL;
}

bool AllianceActBossView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyNode", CCNode*, this->m_readyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyTime", CCLabelIF*, this->m_readyTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyTip", CCLabelIF*, this->m_readyTip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startNode", CCNode*, this->m_startNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLook", CCControlButton*, this->m_btnLook);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnBattle", CCControlButton*, this->m_btnBattle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAddAlliance", CCControlButton*, this->m_btnAddAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startTxt", CCLabelIF*, this->m_startTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startTip", CCLabelIF*, this->m_startTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingNode", CCNode*, this->m_loadingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterSp", CCSprite*, this->m_monsterSp);
    
    return false;
}

void AllianceActBossView::onAddAlliance(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}
void AllianceActBossView::onJijie(CCObject * pSender, Control::EventType pCCControlEvent)
{
    int curTime = GlobalData::shared()->getWorldTime();
    if (curTime > m_info->endTime) {
        //活动结束
        CCCommonUtils::flyHint("", "", _lang_1("133048", _lang("137444").c_str()));
        return;
    }
    
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_btnBattle);
    GetACTBossCommand* cmd = new GetACTBossCommand();
    cmd->sendAndRelease();
}
void AllianceActBossView::onLookTroops(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
}

void AllianceActBossView::onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}