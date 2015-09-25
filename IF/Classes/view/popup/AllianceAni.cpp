//
//  AllianceAni.cpp
//  IF
//
//  Created by liweiyu on 14-8-14.
//
//

#include "AllianceAni.h"

AllianceAni* AllianceAni::create()
{
    AllianceAni *pRet = new AllianceAni();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool AllianceAni::init()
{
    CCBLoadFile("AllianceBtnAni",this,this);
    this->getAnimationManager()->runAnimationsForSequenceNamed("showAni");
    return true;
}

void AllianceAni::onEnter() {
    CCNode::onEnter();
}
void AllianceAni::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler AllianceAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool AllianceAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliance", CCSprite*, this->m_alliance);
    return false;
}