//
//  DragonBornView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBornView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "GuideController.h"

DragonBornView* DragonBornView::create(){
    DragonBornView* ret = new DragonBornView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonBornView::init()
{
    if (!ArcPopupBaseView::init()) {
        return false;
    }
    
    auto bg = CCBLoadFile("DragonBornView",this,this);
    setContentSize(bg->getContentSize());
    
//    setTitleName(_lang("119008"));
    m_attributeTxt->setString("m_attributeTxt");
    m_nameTxt->setString("m_nameTxt");
    m_levelTxt->setString("m_levelTxt");
    m_outputTxt->setString("m_outputTxt");
    m_outputValueTxt->setString("m_outputValueTxt");
    CCCommonUtils::setButtonTitle(m_sellBtn, "m_sellBtn");
    CCCommonUtils::setButtonTitle(m_moveBtn, "m_moveBtn");
    
    return true;
}

void DragonBornView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
}

void DragonBornView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler DragonBornView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", DragonBornView::onOKClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSellClick", DragonBornView::onSellClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoveClick", DragonBornView::onMoveClick);
    return NULL;
}

bool DragonBornView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attributeTxt", CCLabelIF*, this->m_attributeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputTxt", CCLabelIF*, this->m_outputTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputValueTxt", CCLabelIF*, this->m_outputValueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonNode", CCNode*, this->m_dragonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starSpr", CCSprite*, this->m_starSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sellBtn", CCControlButton*, this->m_sellBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveBtn", CCControlButton*, this->m_moveBtn);
    return false;
}

void DragonBornView::onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCLOG(" OOOOOOOOOO ok");
}


void DragonBornView::onSellClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent){
    
}
void DragonBornView::onMoveClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent){
    
}
