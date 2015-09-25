//
//  WaitInterface.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "CCModelLayer.h"
#include "WaitInterface.h"
#include "CCLoadSprite.h"
WaitInterface::~WaitInterface(void)
{
//    CCLOGTIME();
}

WaitInterface::WaitInterface(CCPoint point,CCSize size,int touchMaskOpacity)
:m_point(point)
,m_size(size)
,m_touchMaskOpacity(touchMaskOpacity)

{
//    CCLOGTIME();
}

WaitInterface* WaitInterface::show(CCNode * parent,CCPoint point,CCSize size,int touchMaskOpacity /*= 64*/,float delayTime/*=0.0f*/)
{
    WaitInterface* pRet=new WaitInterface(point,size,touchMaskOpacity);
    if (pRet && pRet->init() && parent)
    {
        pRet->autorelease();
        pRet->m_touchMaskOpacity=touchMaskOpacity;
        pRet->createTouchMask(touchMaskOpacity);
        pRet->delayAni(delayTime);
        parent->addChild(pRet);
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void WaitInterface::remove()
{
    this->removeFromParentAndCleanup(true);
}

void WaitInterface::delayAni(float delay)
{
    createLoadingAni();
    if(fullBtn)
        fullBtn->setVisible(false);
    sprAni->setVisible(false);
    scheduleOnce(schedule_selector(WaitInterface::doShowAni), delay);
}
//延迟显示出来
void WaitInterface::doShowAni(float t)
{
    sprAni->setVisible(true);
    if(fullBtn)
        fullBtn->setVisible(true);
}
//背景
void WaitInterface::createTouchMask(int opacity)
{
    if (fullBtn) {
        fullBtn->removeFromParentAndCleanup(true);
        fullBtn=NULL;
    }
    if (opacity<0)
        return;
    fullBtn = CCModelLayerColor::create();
    fullBtn->setContentSize(m_size);
    fullBtn->setPosition(ccp(m_point.x-m_size.width/2, m_point.y-m_size.height/2));
    fullBtn->setColor(ccBLACK);
    fullBtn->setTouchPriority(0);
    fullBtn->setOpacity(opacity);
    addChild(fullBtn);
}

//动画
void WaitInterface::createLoadingAni()
{
    if (sprAni)
        sprAni->removeFromParentAndCleanup(true);
//    sprAni = CCSprite::create();
//    sprAni->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize screenSize = m_size;
//    sprAni->setPosition(ccp(m_point.x, m_point.y));
//    sprAni->setScale(0.5);
//    int size = 3;
//    CCArray* myArray = new CCArray();
//    CCSpriteFrame *myframe[size];
//    for (int i=0; i<size; i++) {
//        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat("loading_%d.png", i+1)->getCString());
//        myArray->addObject(myframe[i]);
//    }
//    sprAni->setDisplayFrame(myframe[0]);
//    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, 0.1f);
//    CCAnimate* animate = CCAnimate::create(animation);
//    myArray->release();
//    sprAni->stopAllActions();
//    sprAni->runAction(CCRepeatForever::create(animate));
//    addChild(sprAni);
    
    sprAni = CCLoadSprite::createSprite("loading_1.png");
    auto loadingSize = sprAni->getContentSize();
    //    m_loadingIcon->runAction(createMarchAni(AniLoading));
    sprAni->setPosition(ccp(m_point.x, m_point.y));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    sprAni->runAction(rotateForever);
    
    addChild(sprAni);
}


