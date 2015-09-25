//
//  DragonInfoView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonInfoView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "GuideController.h"

DragonInfoView* DragonInfoView::create(){
    DragonInfoView* ret = new DragonInfoView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonInfoView::init()
{
    if (!ArcPopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(203, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(203, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    
    CCBLoadFile("DragonInfoView",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    int extH = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY()+extH/2);
    
    m_nameTxt->setString("m_nameTxt");
    m_outputTxt->setString("m_outputTxt");
    m_detailTxt->setString("m_detailTxt");
    m_haveTxt->setString("m_haveTxt");
    m_countTxt->setString("m_countTxt");
    
    return true;
}

void DragonInfoView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->hideReturnBtn();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void DragonInfoView::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler DragonInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", DragonInfoView::onOKClick);
    return NULL;
}

bool DragonInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconSpr", CCSprite*, this->m_iconSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starSpr", CCSprite*, this->m_starSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attributeNode", CCNode*, this->m_attributeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attributeSpr", CCSprite*, this->m_attributeSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputNode", CCNode*, this->m_outputNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputTxt", CCLabelIF*, this->m_outputTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputSpr", CCSprite*, this->m_outputSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_detailTxt", CCLabelIF*, this->m_detailTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countTxt", CCLabelIF*, this->m_countTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_haveTxt", CCLabelIF*, this->m_haveTxt);

    return false;
}

bool DragonInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch))
    {
        return true;
    }
    return false;
}

void DragonInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch))
    {
        onCloseclick(NULL, CCControlEventTouchUpInside);
    }
}

void DragonInfoView::onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void DragonInfoView::onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCLOG(" OOOOOOOOOO ok");
}