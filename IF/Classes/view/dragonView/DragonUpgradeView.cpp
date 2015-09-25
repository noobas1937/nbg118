//
//  DragonUpgradeView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonUpgradeView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "GuideController.h"

DragonUpgradeView* DragonUpgradeView::create(){
    DragonUpgradeView* ret = new DragonUpgradeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonUpgradeView::init()
{
    if (!ArcPopupBaseView::init()) {
        return false;
    }
    
    auto bg = CCBLoadFile("DragonUpgradeView",this,this);
    setContentSize(bg->getContentSize());
    
//    setTitleName(_lang("119008"));
    m_attributeTxt->setString("m_attributeTxt");
    m_nameTxt->setString("m_nameTxt");
    m_levelTxt->setString("m_levelTxt");
    m_outputTxt->setString("m_outputTxt");
    m_outputValueTxt->setString("m_outputValueTxt");
    m_consumemTxt->setString("m_consumemTxt");
    m_allFoodTxt->setString("m_allFoodTxt");
    CCCommonUtils::setButtonTitle(m_skillBtn, "m_skillBtn");
    CCCommonUtils::setButtonTitle(m_sellBtn, "m_sellBtn");
    CCCommonUtils::setButtonTitle(m_moveBtn, "m_moveBtn");
    CCCommonUtils::setButtonTitle(m_feedBtn1, "m_feedBtn1");
    CCCommonUtils::setButtonTitle(m_feedBtn2, "m_feedBtn2");
    
    return true;
}

void DragonUpgradeView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
}

void DragonUpgradeView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler DragonUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", DragonUpgradeView::onOKClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillClick", DragonUpgradeView::onSkillClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSellClick", DragonUpgradeView::onSellClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoveClick", DragonUpgradeView::onMoveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFeed1Click", DragonUpgradeView::onFeed1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFeed2Click", DragonUpgradeView::onFeed2Click);
    return NULL;
}

bool DragonUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, this->m_OKBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attributeTxt", CCLabelIF*, this->m_attributeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputTxt", CCLabelIF*, this->m_outputTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_outputValueTxt", CCLabelIF*, this->m_outputValueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_consumemTxt", CCLabelIF*, this->m_consumemTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allFoodTxt", CCLabelIF*, this->m_allFoodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonNode", CCNode*, this->m_dragonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode2", CCNode*, this->m_btnNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starSpr", CCSprite*, this->m_starSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodSpr", CCSprite*, this->m_foodSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillBtn", CCControlButton*, this->m_skillBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sellBtn", CCControlButton*, this->m_sellBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveBtn", CCControlButton*, this->m_moveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedBtn1", CCControlButton*, this->m_feedBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedBtn2", CCControlButton*, this->m_feedBtn2);

    return false;
}

void DragonUpgradeView::onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCLOG(" OOOOOOOOOO ok");
}

void DragonUpgradeView::onSkillClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
}
void DragonUpgradeView::onSellClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
}
void DragonUpgradeView::onMoveClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
}
void DragonUpgradeView::onFeed1Click(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
}
void DragonUpgradeView::onFeed2Click(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
}





