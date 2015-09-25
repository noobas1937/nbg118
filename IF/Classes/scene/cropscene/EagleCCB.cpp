//
//  EagleCCB.cpp
//  IF
//
//  Created by fubin on 14-10-8.
//
//

#include "EagleCCB.h"
#include "SceneController.h"
#include "SoundController.h"

using namespace cocos2d;

EagleCCB* EagleCCB::create()
{
    EagleCCB *pRet = new EagleCCB();
    if (pRet && pRet->initEagleCCB())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool EagleCCB::initEagleCCB()
{
    CCBLoadFile("Eagle",this,this);
    return true;
}

void EagleCCB::onEnter() {
    CCNode::onEnter();
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(EagleCCB::onPlayNext));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
}

void EagleCCB::onExit() {
    CCNode::onExit();
}

void EagleCCB::onPlayNext(){
    this->setVisible(false);
    this->scheduleOnce(schedule_selector(EagleCCB::onPlay), (rand()%20)*5);
}

void EagleCCB::onPlay(float _time){
    this->setVisible(true);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler EagleCCB::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler EagleCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool EagleCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    return false;
}