//
//  FlyCBuildView.cpp
//  IF
//
//  Created by fubin on 14-9-22.
//
//

#include "FlyCBuildView.h"

using namespace cocos2d;

FlyCBuildView* FlyCBuildView::create()
{
    FlyCBuildView *pRet = new FlyCBuildView();
    if (pRet && pRet->initFlyCBuildView())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FlyCBuildView::initFlyCBuildView()
{
    string ccbName = "FlyCBuildView";
    CCBLoadFile(ccbName.c_str(),this,this);
    
    m_expName->setString(_lang("107509"));
    m_powerName->setString(_lang("102163"));
    
    for (int i=1; i<=2; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UIGlowLoop_%d",i)->getCString());
        m_expNode->addChild(particle);
    }
    for (int i=1; i<=2; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UIGlowLoop_%d",i)->getCString());
        m_powerNode->addChild(particle);
    }
    m_powerValue->setString("+999");
    m_expValue->setString("+999");
    this->setVisible(false);
    
    return true;
}

void FlyCBuildView::onEnter() {
    CCNode::onEnter();
}

void FlyCBuildView::onExit() {
    CCNode::onExit();
}

void FlyCBuildView::onShowPowerInfo(int power)
{
    m_expItemNode->setVisible(false);
    m_powerItemNode->setVisible(true);
    
    string tmpStr = "+";
    if (power > -0.000001) {
        m_powerValue->setString(tmpStr + CC_CMDITOA(power));
    } else {
        m_powerValue->setString(CC_CMDITOA(power));
    }
    m_powerItemNode->setPositionY(0);
    this->setVisible(true);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    m_particleNode->removeAllChildren();
    this->scheduleOnce(schedule_selector(FlyCBuildView::onPlayBoom), 0.4);
}

void FlyCBuildView::onShowInfo(int exp, int power)
{
    m_expItemNode->setVisible(false);
    m_powerItemNode->setVisible(false);
    
    string tmpStr = "+";
    if (exp > 0) {
        m_expItemNode->setVisible(true);
        m_expValue->setString(tmpStr+CC_CMDITOA(exp));
        if (power>0) {
            m_expItemNode->setPositionY(30);
        }else {
            m_expItemNode->setPositionY(0);
        }
    }
    
    if (power > 0) {
        m_powerItemNode->setVisible(true);
        m_powerValue->setString(tmpStr+CC_CMDITOA(power));
        if (exp>0) {
            m_powerItemNode->setPositionY(-30);
        }else {
            m_powerItemNode->setPositionY(0);
        }
    }
    
    this->setVisible(true);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    
    m_particleNode->removeAllChildren();
    this->scheduleOnce(schedule_selector(FlyCBuildView::onPlayBoom), 0.4);
}

void FlyCBuildView::onPlayBoom(float _time)
{
    for (int i = 1; i<= 8; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Construction_%d",i)->getCString());
        m_particleNode->addChild(particle);
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler FlyCBuildView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler FlyCBuildView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool FlyCBuildView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_expItemNode", CCNode*, this->m_expItemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerItemNode", CCNode*, this->m_powerItemNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_expNode", CCNode*, this->m_expNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerNode", CCNode*, this->m_powerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_particleNode", CCNode*, this->m_particleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_expName", CCLabelIF*, this->m_expName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_expValue", CCLabelIF*, this->m_expValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerName", CCLabelIF*, this->m_powerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerValue", CCLabelIF*, this->m_powerValue);
    return false;
}