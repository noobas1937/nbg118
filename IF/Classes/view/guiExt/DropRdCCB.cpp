//
//  DropRdCCB.cpp
//  IF
//
//  Created by fubin on 14-10-14.
//
//

#include "DropRdCCB.h"
#include "SoundController.h"
#include "CCMathUtils.h"

using namespace cocos2d;

DropRdCCB* DropRdCCB::create(int type)
{
    DropRdCCB *pRet = new DropRdCCB();
    if (pRet && pRet->initDropRdCCB(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DropRdCCB::initDropRdCCB(int type)
{
    m_dropNodes = CCArray::create();
    m_dropSprs = CCArray::create();
    string ccbName = "DropRdCCB";
    CCBLoadFile(ccbName.c_str(),this,this);
    
    string picName = "";
    musicName = "";
    cellName = "";
    int sprCnt = 5;
    float dt = 0.1;
    if (type == R_GOLD) {
        picName = "ActivitiesCoin";
        cellName = "ActivitiesCoin";
        sprCnt = 7;
        musicName = Music_Sfx_button_click_reward;
        dt = 0.05;
    }
    else if (type == R_WOOD) {
        picName = "Wood";
        cellName = "wood";
        musicName = Music_Sfx_UI_woodreward;
    }
    else if (type == R_FOOD) {
        picName = "Food";
        cellName = "food";
        musicName = Music_Sfx_UI_foodreward;
    }
    else if (type == R_IRON) {
        picName = "Iron";
        cellName = "Iron";
        musicName = Music_Sfx_city_harvest_iron;
    }
    else if (type == R_STONE) {
        picName = "Stone";
        cellName = "Stone";
        musicName = Music_Sfx_city_harvest_mithril;
    }
    
    if (picName == "") {
        return true;
    }
    
//    for (int i=0; i<m_dropNodes->count(); i++) {
//        CCNode* curNode = dynamic_cast<CCNode*>(m_dropNodes->objectAtIndex(i));
//        auto pic = CCSprite::create();
//        CCCommonUtils::makeEffectSpr(pic, picName+"_%d.png", sprCnt, dt, i%sprCnt);
//        CCCommonUtils::setSpriteMaxSize(pic, 70, true);
//        
//        auto pic = CCLoadSprite::createSprite(picName.c_str());
//        m_dropSprs->addObject(pic);
//        curNode->addChild(pic);
//    }
    
    return true;
}

void DropRdCCB::onEnter() {
    CCNode::onEnter();
//    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DropRdCCB::onPlayEnd));
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Drop");
    onPlayDrop();
    this->scheduleOnce(schedule_selector(DropRdCCB::onDelete), 3);
    this->schedule(schedule_selector(DropRdCCB::onPlayMusic), 0.2, 0, 0);
}

void DropRdCCB::onExit() {
    CCNode::onExit();
}

void DropRdCCB::onPlayEnd()
{
    for (int i=0; i<m_dropSprs->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_dropSprs->objectAtIndex(i));
        if (spr) {
            spr->runAction(CCFadeOut::create(1));
        }
    }
    this->scheduleOnce(schedule_selector(DropRdCCB::onDelete), 1);
}

void DropRdCCB::onPlayMusic(float _time)
{
    if (musicName != "") {
        SoundController::sharedSound()->playEffects(musicName.c_str());
    }
}

void DropRdCCB::onDelete(float _time)
{
    m_dropSprs->removeAllObjects();
    m_dropNodes->removeAllObjects();
    this->removeFromParent();
}

void DropRdCCB::onDropOneCell(CCObject* obj)
{
    int idx = dynamic_cast<CCInteger*>(obj)->getValue();
    if (idx < m_ptArray.size()) {
        spine::SkeletonAnimation* skAni = spine::SkeletonAnimation::createWithFile("Spine/World/DropRdCCB.json","Imperial/Imperial_30.atlas",1.0);
        int dx = CCMathUtils::getRandomInt(0, 640);
        
        int dy = CCMathUtils::getRandomInt(0, 100);
        skAni->setPosition(m_ptArray[idx]+ccp(0, 600));
        if (CCCommonUtils::isIosAndroidPad())
        {
            skAni->setPosition(m_ptArray[idx]+ccp(0, 800));
        }
        this->addChild(skAni);
        skAni->setAnimation(0, cellName.c_str(), false);
        
    }
}

void DropRdCCB::onPlayDrop()
{
    m_ptArray.clear();
    int constPt = 320;
    if (CCCommonUtils::isIosAndroidPad())
    {
//        constPt = 768;
    }
    int tmpPtx = constPt;
    int tmpPty = 500;
    for (int i=1; i<=7; i++) {
        tmpPtx = constPt - 40*(i-1);
        tmpPty = 100 - 10*(i-1);
        for (int j=0; j<i; j++) {
            int offx = CCMathUtils::getRandomInt(0, 40)-20;
            int offy = CCMathUtils::getRandomInt(0, 40)-20;
            m_ptArray.push_back(ccp(tmpPtx+j*80+offx, tmpPty+offy));
        }
    }
    Vector<FiniteTimeAction*> arrayOfActions;
    //auto arrayOfActions = CCArray::create();
    for (int i=0; i<m_ptArray.size(); i++) {
        auto func = CCCallFuncO::create(this, callfuncO_selector(DropRdCCB::onDropOneCell), CCInteger::create(i));
        int t = CCMathUtils::getRandomInt(0, 1);
        CCActionInterval * delayT = CCDelayTime::create(0.1*t);
        arrayOfActions.pushBack(func);
        arrayOfActions.pushBack(delayT);
    }
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    this->runAction(sequenceAction);
    return;
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler DropRdCCB::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler DropRdCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool DropRdCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode",10) == 0) {
        pNode->removeAllChildren();
        m_dropNodes->addObject(pNode);
        return true;
    }
    return false;
}