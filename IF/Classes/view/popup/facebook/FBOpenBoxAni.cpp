//
//  FBOpenBoxAni.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-20.
//
//

#include "FBOpenBoxAni.h"
#include "FBOpenBoxAni.h"
#include "SoundController.h"
#include "ParticleController.h"

using namespace cocos2d;

FBOpenBoxAni* FBOpenBoxAni::create(int type)
{
    FBOpenBoxAni *pRet = new FBOpenBoxAni();
    if (pRet && pRet->initFBOpenBoxAni(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FBOpenBoxAni::initFBOpenBoxAni(int type)
{
    CCBLoadFile("BaozangCoin",this,this);
    m_type = type;
    return true;
}

void FBOpenBoxAni::onEnter() {
    CCNode::onEnter();
    this->getAnimationManager()->runAnimationsForSequenceNamed("Coin");
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(FBOpenBoxAni::onDelete));
    //this->scheduleOnce(schedule_selector(FBOpenBoxAni::onPlayEnd), rand);
    switch (m_type) {
        case 1:
            m_oneNode->setVisible(true);
            break;
        case 2:
            m_twoNode->setVisible(true);
            break;
        case 5:
            m_fiveNode->setVisible(true);
            break;
        case 10:
            m_tenNode->setVisible(true);
            break;
        default:
            m_oneNode->setVisible(true);
            break;
    }
    if(m_type>=5){
        m_mutiNode->setVisible(true);
    }
}

void FBOpenBoxAni::onExit() {
    CCNode::onExit();
}

void FBOpenBoxAni::onPlayEnd()
{
    this->schedule(schedule_selector(FBOpenBoxAni::onPlayMusic), 0.2, 8, 0);
    //this->scheduleOnce(schedule_selector(FBOpenBoxAni::onDelete), 1);
}

void FBOpenBoxAni::onPlayMusic(float _time)
{
    if (musicName != "") {
        SoundController::sharedSound()->playEffects(musicName.c_str());
    }
}

void FBOpenBoxAni::onDelete()
{
    this->removeFromParent();
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler FBOpenBoxAni::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler FBOpenBoxAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool FBOpenBoxAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oneNode", CCNode*, this->m_oneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_twoNode", CCNode*, this->m_twoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fiveNode", CCNode*, this->m_fiveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tenNode", CCNode*, this->m_tenNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mutiNode", CCNode*, this->m_mutiNode);
    return true;
}
