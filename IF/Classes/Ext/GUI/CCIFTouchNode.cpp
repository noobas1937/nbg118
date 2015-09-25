//
//  CCIFTouchNode.cpp
//  IF
//
//  Created by 童悦 on 15/8/4.
//
//

#include "CCIFTouchNode.h"
//
//  CCTableViewTouchIFCell.cpp
//  IF
//
//  Created by 童悦 on 15/7/2.
//
//

#include "CCIFTouchNode.h"
CCIFTouchNode::CCIFTouchNode():_touchListener(nullptr),_touchEnabled(false)
{
    _swallowsTouches = true;
}

void CCIFTouchNode::setSwallowsTouches(bool swallowsTouches)
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

void CCIFTouchNode::setTouchEnabled(bool enabled)
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
            listener->onTouchBegan = CC_CALLBACK_2(CCIFTouchNode::onTouchBegan, this);
            listener->onTouchMoved = CC_CALLBACK_2(CCIFTouchNode::onTouchMoved, this);
            listener->onTouchEnded = CC_CALLBACK_2(CCIFTouchNode::onTouchEnded, this);
            listener->onTouchCancelled = CC_CALLBACK_2(CCIFTouchNode::onTouchCancelled, this);
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

bool CCIFTouchNode::onTouchBegan(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
    CCASSERT(false, "Layer#ccTouchBegan override me");
    return true;
}

void CCIFTouchNode::onTouchMoved(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}
void CCIFTouchNode::onRetainTouchEnded(Touch *touch, Event *unused_event)
{
    //this->retain();
    onTouchEnded(touch,unused_event);
    //this->release();
}
void CCIFTouchNode::onTouchEnded(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}

void CCIFTouchNode::onTouchCancelled(Touch *touch, Event *event)
{
    CC_UNUSED_PARAM(event);
}
