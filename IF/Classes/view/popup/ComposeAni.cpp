//
//  ComposeAni.cpp
//  IF
//
//  Created by 李朝辉 on 15-7-31.
//
//

#include "ComposeAni.h"

ComposeAni* ComposeAni::create()
{
    ComposeAni *pRet = new ComposeAni();
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

bool ComposeAni::init()
{
    CCBLoadFile("ComposeMaterial_compo",this,this);
    this->getAnimationManager()->runAnimationsForSequenceNamed("showAni");
    return true;
}

void ComposeAni::onEnter() {
    CCNode::onEnter();
}
void ComposeAni::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler ComposeAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ComposeAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliance", CCSprite*, this->m_alliance);
    return false;
}
