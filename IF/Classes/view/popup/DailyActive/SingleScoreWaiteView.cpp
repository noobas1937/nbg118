//
//  SingleScoreWaiteView.cpp
//  IF
//
//  Created by 付彬 on 15/11/19.
//
//

#include "SingleScoreWaiteView.hpp"
#include "PopupViewController.h"
#include "SceneController.h"
#include "TipsView.h"
#include "YesNoDialog.h"
#include "ActivityController.h"

SingleScoreWaiteView *SingleScoreWaiteView::create(ActivityEventObj* info){
    SingleScoreWaiteView *ret = new SingleScoreWaiteView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SingleScoreWaiteView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto cf = CCLoadSprite::getSF("Ativity_iconLogo_3.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(500, true);
            CCLoadSprite::doResourceByCommonIndex(504, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            CCLoadSprite::doResourceByCommonIndex(7, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(500, false);
                CCLoadSprite::doResourceByCommonIndex(504, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
                CCLoadSprite::doResourceByCommonIndex(7, false);
            });
        }else{
            CCLoadSprite::doResourceByCommonIndex(504, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(504, false);
            });
        }
        CCBLoadFile("SingleScoreWaitView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        m_titleTxt->setString(m_info->name);
        m_readyTip->setString(m_info->desc);
        
        this->schedule(schedule_selector(SingleScoreWaiteView::updateTime),1);
        this->updateTime(0);
        ret = true;
    }
    return ret;
}

void SingleScoreWaiteView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void SingleScoreWaiteView::onExit(){
    setTouchEnabled(false);
    GameController::getInstance()->removeWaitInterface();
    PopupBaseView::onExit();
}

bool SingleScoreWaiteView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void SingleScoreWaiteView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    auto view = PopupViewController::getInstance()->getCurrentPopupView();
    if(!isTouchInside(m_clickArea, pTouch) && view==this){
        this->closeSelf();
    }
}

void SingleScoreWaiteView::updateTime(float _time)
{
    auto nowTime = GlobalData::shared()->getWorldTime();
    if(m_info->startTime==0){
        m_readyTime->setString(_lang("105800"));
        m_readyTime->setPositionY(234);
        m_readyTip->setPositionY(300);
        
    }else if (m_info->startTime > nowTime)
    {
        double leftTime = m_info->startTime - nowTime;
        m_readyTime->setString(_lang_1("105804", CC_SECTOA(leftTime)));
        m_readyTime->setPositionY(234);
        m_readyTip->setPositionY(300);
    }else{
        m_readyTime->setString(_lang("133083"));
    }
}

void SingleScoreWaiteView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void SingleScoreWaiteView::onInfoClick(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
//    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("150264"),kCCTextAlignmentLeft));
}

bool SingleScoreWaiteView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_readyTime", CCLabelIF*, this->m_readyTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_readyTip", CCLabelIF*, this->m_readyTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    return false;
}

SEL_CCControlHandler SingleScoreWaiteView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", SingleScoreWaiteView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoClick", SingleScoreWaiteView::onInfoClick);
    return NULL;
}