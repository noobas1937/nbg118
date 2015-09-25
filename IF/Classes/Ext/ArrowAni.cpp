//
//  ArrowAni.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-8.
//
//

#include "ArrowAni.h"

bool ArrowAni::init(){
    m_rect = CCLoadSprite::createScale9Sprite("rect.png");
    this->addChild(m_rect);
    m_arrow = CCLoadSprite::createSprite("guide_arrow_new.png");
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        m_arrow->setScale(2.f);
    }
    this->addChild(m_arrow);

    return true;
}

void ArrowAni::play(CCPoint pos, int w, int h, int d){
    m_direction = d;
    if(m_direction < UP){
        m_direction = UP;
    }
    m_pos = pos;
   
    if(w == 0){
        w = 100;
    }
    if(h == 0){
        h = 100;
    }
    m_arrow->stopAllActions();
    m_rect->setContentSize(CCSize(w, h));
    m_rect->cocos2d::CCNode::setPosition(pos.x + w / 2, pos.y + h / 2);
    float  time = 0.5f;
    int offset = 50;
    CCMoveTo* move1 = NULL;
    CCMoveTo* move2 = NULL;
 
    switch(m_direction){
        case UP:
            m_arrow->setRotation(0);
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.y = m_pos.y-m_arrow->getContentSize().height/2;
            }
            m_arrow->setPosition(ccp(m_pos.x + w / 2, m_pos.y - m_arrow->getContentSize().height / 2 - offset));
            move1= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y - m_arrow->getContentSize().height / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y - m_arrow->getContentSize().height / 2 - offset));
            
            break;
        case DOWN:
            m_arrow->setRotation(180);
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.y = m_pos.y+m_arrow->getContentSize().height/2;
            }
            m_arrow->setPosition(ccp(m_pos.x + w / 2, m_pos.y + m_arrow->getContentSize().height / 2 + offset + h));
            move1= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y + m_arrow->getContentSize().height / 2 + h));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y + m_arrow->getContentSize().height / 2 + offset + h));
            
            break;
        case LEFT:
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.x = m_pos.x+m_arrow->getContentSize().height/2;
            }
            m_arrow->setPosition(ccp(m_pos.x + w / 2 + m_arrow->getContentSize().width, m_pos.y + h / 2));
            move1= CCMoveTo::create(time, ccp(m_pos.x + offset + w / 2 + m_arrow->getContentSize().width, m_pos.y + h / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2 + m_arrow->getContentSize().width, m_pos.y + h / 2));
            m_arrow->setRotation(-90);
            break;
        case RIGHT:
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.x = m_pos.x-m_arrow->getContentSize().height/2;
            }
            m_arrow->setRotation(90);
            m_arrow->setPosition(ccp(m_pos.x - m_arrow->getContentSize().width / 2, m_pos.y + h / 2));
            move1= CCMoveTo::create(time, ccp(m_pos.x - offset - m_arrow->getContentSize().width / 2, m_pos.y + h / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x - m_arrow->getContentSize().width / 2, m_pos.y + h / 2));
            break;
    }
    if(move1 && move2) {
        m_arrow->setVisible(true);
        m_arrow->runAction(CCRepeatForever::create(CCSequence::create(move1,move2,NULL)));
    }
    else {
        m_arrow->setVisible(false);
    }
}

void ArrowAni::stop(){
    m_arrow->stopAllActions();
}

ArrowAni* ArrowAni::create(float delayPerUnit){
    ArrowAni *pRet = new ArrowAni(delayPerUnit);
    if (pRet && pRet->init())
    {
//        pRet->initWithSpriteFrame(CCLoadSprite::loadResource("guide_arrow_new.png"));
        
//        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}