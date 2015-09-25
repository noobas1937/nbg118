//
//  ButtonLightEffect.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/2/10.
//
//

#include "ButtonLightEffect.h"

ButtonLightEffect *ButtonLightEffect::create(CCSize &size, ccColor3B color, bool autoPlay,float delayTime,int type){
    ButtonLightEffect *ret = new ButtonLightEffect();
    if(ret && ret->init(size,color,autoPlay,delayTime,type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ButtonLightEffect::init(CCSize &size, ccColor3B color, bool autoPlay,float delayTime,int type){
    if(!CCNode::init())
        return false;
    CCLoadSprite::doResourceByCommonIndex(102, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(102, false);
    });
    m_delayTime = delayTime;
    m_effBound = size - CCSize(20,0);
    string spName = "ButtonLight_6.png";
    if (type == LOTTERY)
    {
        spName = "ButtonLight_7.png";
    }
    m_effSpr = CCLoadSprite::createSprite(spName.c_str());
    m_effSpr->setAnchorPoint(ccp(0, 0.5));
    CCSize effSize = m_effSpr->getContentSize();
    float fScale = m_effBound.height / effSize.height;
    if (type == LOTTERY)
    {
        float scalex = m_effBound.width / effSize.width;
//        m_effSpr->setScaleY(-fScale);
//        m_effSpr->setScaleX(scalex);
        m_effSpr->setOpacity(100);
    }
    else
    {
        m_effSpr->setScale(fScale);
    }
    
    m_effSpr->setOpacity(200);
    setEffColor(color);
    m_clipNode = CCClipNode::create(m_effBound.width, m_effBound.height);
    m_clipNode->setAnchorPoint(CCPointZero);
    m_clipNode->addChild(m_effSpr);
    m_effSpr->setPosition(ccp(-m_effBound.width, m_effBound.height*0.5));
    m_clipNode->setPosition(ccp(-m_effBound.width*0.5,-m_effBound.height*0.5));
    this->addChild(m_clipNode);
    
    m_playTime = m_effBound.width / 400;
    
    if(autoPlay){
        playEffect();
    }else{
        if(delayTime>0){
            m_effSpr->runAction(CCSequence::create(CCDelayTime::create(m_delayTime),CCCallFuncO::create(this, callfuncO_selector(ButtonLightEffect::loopEffect),NULL),NULL));
        }
    }
    return true;
}
void ButtonLightEffect::stopEffect(){
    if(m_effSpr){
        m_effSpr->stopAllActions();
    }
}
void ButtonLightEffect::playEffect(){
    if(m_effSpr){
        CCMoveTo *moveTo = CCMoveTo::create(m_playTime, ccp(m_effBound.width, m_effBound.height*0.5));
        m_effSpr->runAction(CCSequence::create(moveTo,CCDelayTime::create(m_delayTime),CCCallFuncO::create(this, callfuncO_selector(ButtonLightEffect::loopEffect),NULL),NULL));
    }
}
void ButtonLightEffect::loopEffect(CCObject *ccObj){
    if(m_effSpr){
        m_effSpr->stopAllActions();
        m_effSpr->setPosition(ccp(-m_effBound.width, m_effBound.height*0.5));
        playEffect();
    }
}
void ButtonLightEffect::setBlendFunc(ccBlendFunc blendFunc){
    if(m_effSpr){
        m_effSpr->setBlendFunc(blendFunc);
    }
}
ccColor3B ButtonLightEffect::getEffColor(void){
    return m_effColor;
}
void ButtonLightEffect::setEffColor(ccColor3B color){
    m_effColor = color;
    if(m_effSpr){
        m_effSpr->setColor(color);
    }
}