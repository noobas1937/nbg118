//
//  MatetialAni.cpp
//  IF
//
//  Created by 李朝辉 on 15-7-31.
//
//

#include "MatetialAni.h"
MatetialAni* MatetialAni::create()
{
    MatetialAni *pRet = new MatetialAni();
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

bool MatetialAni::init()
{
    CCBLoadFile("ComposeMaterial_apart",this,this);
    this->getAnimationManager()->runAnimationsForSequenceNamed("showAni");
    return true;
}

void MatetialAni::onEnter() {
    CCNode::onEnter();
}
void MatetialAni::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler MatetialAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool MatetialAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliance", CCSprite*, this->m_alliance);
    return false;
}

