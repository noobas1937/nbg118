//
//  AlertAddAllianceView.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-7.
//
//

#include "AlertAddAllianceView.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GuideController.h"
#include "PopupViewController.h"
#include "CreateAllianceView.h"
#include "JoinAllianceView.h"
#include "GetAllianceListCommand.h"
#include "SoundController.h"

AlertAddAllianceView *AlertAddAllianceView::create(){
    AlertAddAllianceView *ret = new AlertAddAllianceView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AlertAddAllianceView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -80, true);
    //UIComponent::getInstance()->showPopupView(2);
    //UIComponent::getInstance()->hideReturnBtn();
}

void AlertAddAllianceView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AlertAddAllianceView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        this->setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(7, false);
//        });
        
        auto node = CCBLoadFile("AlertAddAllianceView", this, this);
        this->setContentSize(node->getContentSize());
        
//        m_titleText->setString(_lang("115001"));
//        m_txt1->setString(_lang("115002"));
//        m_txt2->setString(_lang("115003"));
//        m_txt3->setString(_lang("115004"));
//        m_txt4->setString(_lang("115005"));
//        m_txt5->setString(_lang("115006"));
        m_txt1->setString(_lang("115168"));
        m_txt2->setString(_lang_1("115169", "200"));
        m_txt3->setString(_lang("115170"));
        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115020").c_str());
        m_btnJoin->setSwallowsTouches(false);
        ret = true;
    }
    return ret;
}

bool AlertAddAllianceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelBMFont*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    return false;
}

bool AlertAddAllianceView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AlertAddAllianceView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_btnJoin, pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        onClickClose(NULL,Control::EventType::TOUCH_DOWN);
    } else if (!isTouchInside(m_clickNode, pTouch)) {
        this->onClickClose(NULL,Control::EventType::TOUCH_DOWN);
    }
    
}

SEL_CCControlHandler AlertAddAllianceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoin", AlertAddAllianceView::onClickClose);
    return NULL;
}

void AlertAddAllianceView::onClickClose(CCObject * pSender, Control::EventType pCCControlEvent) {
    this->closeSelf();
}
