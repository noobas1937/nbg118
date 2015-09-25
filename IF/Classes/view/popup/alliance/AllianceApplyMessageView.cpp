//
//  AllianceApplyMessageView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/6.
//
//

#include "AllianceApplyMessageView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleFireAni.h"
#include "AllianceApplyInfoSendCommand.h"

/////////////
AllianceApplyMessageView *AllianceApplyMessageView::create(std::string content){
    AllianceApplyMessageView *ret = new AllianceApplyMessageView(content);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceApplyMessageView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void AllianceApplyMessageView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

bool AllianceApplyMessageView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        auto node = CCBLoadFile("JoinAllianceInfoView02", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_messageLabel->setString(m_content);
        m_messageLabel->setColor({201,182,140});
        ret = true;
    }
    return ret;
}

bool AllianceApplyMessageView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_messageLabel", CCLabelIF*, this->m_messageLabel);
    return false;
}

bool AllianceApplyMessageView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceApplyMessageView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_bgNode, pTouch)) {
        closeSelf();
    }
}

SEL_CCControlHandler AllianceApplyMessageView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
