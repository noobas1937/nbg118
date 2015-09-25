//
//  IFDockWalloperNode.cpp
//  IF
//
//  Created by xxrdsg on 15-4-13.
//
//

#include "IFDockWalloperNode.h"
#include "ActivityController.h"

IFDockWalloperNode* IFDockWalloperNode::create()
{
    IFDockWalloperNode* ret = new IFDockWalloperNode();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool IFDockWalloperNode::init()
{
    p1 = ccp(2236, 1590);
    p2 = ccp(2173, 1560);
    p3 = ccp(2106, 1600);
    
    char strJsonFileName[256] = {0};
    char strAtlasFilename[256] = {0};
    sprintf(strJsonFileName, "Spine/Imperial/worker.json");
    sprintf(strAtlasFilename, "Imperial/Imperial_30.atlas");
    if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName) ||
        !CCFileUtils::sharedFileUtils()->isFileExist(strAtlasFilename))
    {
        return false;
    }
    animationObj = NULL;
    animationObj = new IFSkeletonAnimation(strJsonFileName,strAtlasFilename);
    if (!animationObj) {
        return false;
    }
    animationObj->setVisibleStop(false);
    animationObj->setScale(1.0);
    addChild(animationObj, 2);
    animationObj->setPosition(p1);
    CCSprite* spr = CCLoadSprite::createSprite("imperialShadow.png");
    spr->setScale(0.3);
    spr->setOpacity(170);
    spr->setAnchorPoint(ccp(0.5, 0.2));
    animationObj->addChild(spr, -1);
    
    startLoop(NULL);
    return true;
}

void IFDockWalloperNode::startLoop(CCObject* obj)
{
    if (m_inLoop) {
        return;
    }
    this->setVisible(true);
    m_inLoop = true;
    CCArray* arrayOfActions = CCArray::create();
    CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(IFDockWalloperNode::setDockWalloperState), CCInteger::create(0));
    CCMoveTo* mv1 = CCMoveTo::create(7.0, p2);
    CCCallFuncO* fun2 = CCCallFuncO::create(this, callfuncO_selector(IFDockWalloperNode::setDockWalloperState), CCInteger::create(1));
    CCMoveTo* mv2 = CCMoveTo::create(7.0, p3);
    CCCallFuncO* fun3 = CCCallFuncO::create(this, callfuncO_selector(IFDockWalloperNode::setDockWalloperState), CCInteger::create(2));
    CCDelayTime* mv3 = CCDelayTime::create(1.0);
    CCCallFuncO* fun4 = CCCallFuncO::create(this, callfuncO_selector(IFDockWalloperNode::setDockWalloperState), CCInteger::create(3));
    CCMoveTo* mv4 = CCMoveTo::create(6.0, p2);
    CCCallFuncO* fun5 = CCCallFuncO::create(this, callfuncO_selector(IFDockWalloperNode::setDockWalloperState), CCInteger::create(4));
    CCMoveTo* mv5 = CCMoveTo::create(6.0, p1);
    CCSequence* seq = CCSequence::create(fun1, mv1, fun2, mv2, fun3, mv3, fun4, mv4, fun5, mv5, NULL);
    animationObj->runAction(CCRepeatForever::create(seq));
}

void IFDockWalloperNode::setDockWalloperState(CCObject* obj)
{
    int state = dynamic_cast<CCInteger*>(obj)->getValue();
    setDockWalloperState(state);
}

IFSkeletonAnimation* IFDockWalloperNode::setDockWalloperState(int state)
{
    m_state = state;

    string aniName = "";
    switch (state) {
        case leftAwayShip:
            aniName = "walk_1";
            break;
            
        case upAwayShip:
            aniName = "walk_2";
            break;
            
        case unLoad:
            aniName = "banyun";
            break;
            
        case downTowardsShip:
            aniName = "walk_3";
            break;
            
        case rightTowardsShip:
            aniName = "walk_4";
            break;
        
        default:
            break;
    }
    
    animationObj->setToSetupPose();
    spTrackEntry* entry = animationObj->setAnimation(0, aniName.c_str(), true);
    animationObj->setTimeScale(entry->endTime/1.0f);
    
    if (m_state == 0) {
        if (animationObj) {
            if (PortActController::getInstance()->isCanRwd()) {
                animationObj->stopAllActions();
                this->setVisible(false);
                m_inLoop = false;
            }
        }
    }
    
    return animationObj;
}

IFDockWalloperNode::~IFDockWalloperNode()
{
    CC_SAFE_RELEASE_NULL(animationObj);
}

IFDockWalloperNode::IFDockWalloperNode():
m_inLoop(false)
{
    
}

void IFDockWalloperNode::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IFDockWalloperNode::startLoop), MSG_NEW_TIME_RWD_END, NULL);
}

void IFDockWalloperNode::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    CCNode::onExit();
}