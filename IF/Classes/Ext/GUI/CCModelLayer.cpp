//
//  CCModelLayer.cpp
//  ccgoe
//
//  Created by 谢文杰 on 12-11-27.
//
//
#include "CCModelLayer.h"
#include "CCBExtension.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

bool isNodeVisible(CCNode *node) {
    if (!node->getParent()) {
        return node->isVisible();
    }
    if (!node->isVisible()) {
        return false;
    } else {
        return isNodeVisible(node->getParent());
    }
}

bool CCModelLayer::init()
{
    if (CCLayer::init()) {
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setSwallowsTouches(true);
        setTouchEnabled(true);
        _touchEnabled=true;
        return true;
    }
    return false;
}

//void CCModelLayer::registerWithTouchDispatcher(void)
//{
//    cocos2d:://CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_bTouchPriority, true);
//}

void CCModelLayer::setTouchPriority(int priority) {
    if(m_bTouchPriority != priority) {
        m_bTouchPriority = priority;
        
		if( _touchEnabled) {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

bool CCModelLayer::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(!isNodeVisible(this) || (m_bRestraintTouchInside && !isTouchInside(this, pTouch)))
        return false;

    return true;
}

bool CCModelLayerColor::init()
{
    if (CCLayerColor::init()) {
        setSwallowsTouches(true);
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setTouchEnabled(true);
        return true;
    }
    return false;
}

//void CCModelLayerColor::registerWithTouchDispatcher(void)
//{
//    cocos2d:://CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_bTouchPriority, true);
//}
void CCModelLayerColor::onEnter()
{
    CCNode::onEnter();
}

void CCModelLayerColor::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}
void CCModelLayerColor::setTouchPriority(int priority) {
    if(m_bTouchPriority != priority) {
        m_bTouchPriority = priority;
        
		if( _touchEnabled) {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

void CCModelLayerColor::setTouchFunction(std::function<void(cocos2d::CCTouch *pTouch)> function) {
    m_function = function;
}

bool CCModelLayerColor::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(!isNodeVisible(this) || (m_bRestraintTouchInside && !isTouchInside(this, pTouch)))
        return false;
    if(m_throughBegan)
    {
        m_bTouchThrough = m_throughBegan(pTouch,pEvent);
    }
    return true;
}

void CCModelLayerColor::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(m_bTouchThrough)
    {
        m_throughEnd(pTouch,pEvent);
    }
    if (m_function) {
        m_function(pTouch);
    }
}

NS_CC_EXT_END