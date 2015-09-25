//
//  AllianceApplyInfoView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/5.
//
//

#include "AllianceApplyInfoView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleFireAni.h"
#include "AllianceApplyInfoSendCommand.h"
AllianceApplyInfoView *AllianceApplyInfoView::create(std::string allianceId){
    AllianceApplyInfoView *ret = new AllianceApplyInfoView(allianceId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceApplyInfoView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
}

void AllianceApplyInfoView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool AllianceApplyInfoView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        auto node = CCBLoadFile("JoinAllianceInfoView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_contentEditBox = NULL;
        CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105302").c_str());
        CCCommonUtils::setButtonTitle(m_nosendBtn, _lang("115505").c_str());
        m_titleLabel->setString(_lang("115022"));
        m_titleLabel->setColor({255,213,144});
        m_tipsLabel->setString(_lang("115501"));
        m_tipsLabel->setColor({58,42,19});
        
        if(m_contentEditBox==NULL){
            m_contentEditBox = InputFieldMultiLine::create(CCSizeMake(445,300),"lianmeng_di_7.png",18);
            m_contentEditBox->setMaxChars(199);
            m_contentEditBox->setLineNumber(10);
            m_contentEditBox->setPosition(ccp(0, 0));
            m_contentEditBox->setFontColor({201,182,140});
            m_contentEditBox->setEnabled(true);
            m_contentEditBox->setSwallowsTouches(false);
            m_messageNode->addChild(m_contentEditBox);
        }
        m_sendBtn->setEnabled(false);

        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        ret = true;
    }
    return ret;
}

void AllianceApplyInfoView::checkInput(float t){
    string message = m_contentEditBox->getText();
    bool flag = true;
    if(message==""){
        m_sendBtn->setEnabled(false);
    }else{
        m_sendBtn->setEnabled(true);
    }
}
bool AllianceApplyInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nosendBtn", CCControlButton*, this->m_nosendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_messageNode", CCNode*, this->m_messageNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    return false;
}

bool AllianceApplyInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceApplyInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_messageNode, pTouch)) {
        if(m_contentEditBox!=NULL){
            this->unschedule(schedule_selector(AllianceApplyInfoView::checkInput));
            this->schedule(schedule_selector(AllianceApplyInfoView::checkInput));
        }
    }
    if (!isTouchInside(m_messageNode, pTouch)) {
        this->unschedule(schedule_selector(AllianceApplyInfoView::checkInput));
    }
}

void AllianceApplyInfoView::onSendClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    std::string content = m_contentEditBox->getText();
    AllianceApplyInfoSendCommand* command = new AllianceApplyInfoSendCommand(m_allianceId, content);
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceApplyInfoView::onShowTips), NULL));
    command->sendAndRelease();
    closeSelf();
}

void AllianceApplyInfoView::onShowTips(cocos2d::CCObject *data){
    CCCommonUtils::flyHint("", "", _lang("115513"), 3, 0, true);
}

SEL_CCControlHandler AllianceApplyInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", AllianceApplyInfoView::onSendClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNoSendClick", AllianceApplyInfoView::onNoSendClick);
    return NULL;
}

void AllianceApplyInfoView::onNoSendClick(CCObject * pSender, CCControlEvent pCCControlEvent) {
    std::string content = "";
    AllianceApplyInfoSendCommand* command = new AllianceApplyInfoSendCommand(m_allianceId, content);
    command->sendAndRelease();
    closeSelf();
}