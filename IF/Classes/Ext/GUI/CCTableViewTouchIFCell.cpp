//
//  CCTableViewTouchIFCell.cpp
//  IF
//
//  Created by 童悦 on 15/7/2.
//
//

#include "CCTableViewTouchIFCell.h"
CCTableViewTouchIFCell::CCTableViewTouchIFCell():_touchListener(nullptr),_touchEnabled(false)
{
    _swallowsTouches = false;
}

void CCTableViewTouchIFCell::setSwallowsTouches(bool swallowsTouches)
{
    if (_swallowsTouches != swallowsTouches)
    {
        _swallowsTouches = swallowsTouches;
        
        if( _touchEnabled)
        {
            setTouchEnabled(false);
            setTouchEnabled(true);
        }
    }
}

void CCTableViewTouchIFCell::setTouchEnabled(bool enabled)
{
    if (_touchEnabled != enabled)
    {
        _touchEnabled = enabled;
        if (enabled)
        {
            if (_touchListener != nullptr)
                return;
                // Register Touch Event
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(_swallowsTouches);
            listener->onTouchBegan = CC_CALLBACK_2(CCTableViewTouchIFCell::onTouchBegan, this);
            listener->onTouchMoved = CC_CALLBACK_2(CCTableViewTouchIFCell::onTouchMoved, this);
            listener->onTouchEnded = CC_CALLBACK_2(CCTableViewTouchIFCell::onTouchEnded, this);
            listener->onTouchCancelled = CC_CALLBACK_2(CCTableViewTouchIFCell::onTouchCancelled, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
            _touchListener = listener;
            
        }
        else
        {
            _eventDispatcher->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }
    }
}

bool CCTableViewTouchIFCell::onTouchBegan(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
    CCASSERT(false, "Layer#ccTouchBegan override me");
    return true;
}

void CCTableViewTouchIFCell::onTouchMoved(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}

void CCTableViewTouchIFCell::onTouchEnded(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}

void CCTableViewTouchIFCell::onTouchCancelled(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}
