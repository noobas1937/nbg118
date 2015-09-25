//
//  CCSliderBar.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-23.
//
//

#include "CCSliderBar.h"

CCSliderBar* CCSliderBar::createSlider(CCScale9Sprite* backgSprite, CCSprite* pogressSprite, CCSprite* thumbSprite){
    CCSliderBar* slider = new CCSliderBar();
    CCSprite* spr = CCSprite::create();
    spr->setContentSize(backgSprite->getContentSize());
    slider->initWithSprites(spr, pogressSprite, thumbSprite);
    slider->addChild(backgSprite,-1000);
    slider->m_bgWidth = backgSprite->getContentSize().width;
    slider->autorelease();
    return slider;
}

void CCSliderBar::needsLayout(){
    if (NULL == this->getThumbSprite() || NULL == this->getBackgroundSprite() || NULL == this->getProgressSprite())
    {
        return;
    }
    // Update thumb position for new value
    float percent               = (_value - _minimumValue) / (_maximumValue - _minimumValue);
    
    CCPoint pos                 = this->getThumbSprite()->getPosition();
    pos.x                       = percent * (m_bgWidth-m_limitMoveValue*2)+m_limitMoveValue;
    this->getThumbSprite()->setPosition(pos);
    this->getSelectedThumbSprite()->setPosition(pos);
    this->getProgressSprite()->setScaleX(percent*m_maxScaleX);
}

void CCSliderBar::setProgressScaleX(float scaleX){
    m_maxScaleX = scaleX;
    this->needsLayout();
}

bool CCSliderBar::onTouchBegan(CCTouch* touch, CCEvent* pEvent)
{
    if (!isTouchInside(touch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    m_touchEnd = false;
    CCPoint location = locationFromTouch(touch);
    sliderBegan(location);
    return true;
}

void CCSliderBar::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchEnd = true;
    sliderEnded(CCPointZero);
}

float CCSliderBar::valueForLocation(CCPoint location)
{
    float percent = (location.x-m_limitMoveValue)/ (getBackgroundSprite()->getContentSize().width-m_limitMoveValue*2);
    return MAX(MIN(_minimumValue + percent * (_maximumValue - _minimumValue), _maximumAllowedValue), _minimumAllowedValue);
}