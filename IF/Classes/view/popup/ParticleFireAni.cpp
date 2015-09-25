//
//  ParticleFireAni.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-15.
//
//

#include "ParticleFireAni.h"
#include "ParticleController.h"

ParticleFireAni* ParticleFireAni::create(){
    ParticleFireAni* ret = new ParticleFireAni();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void ParticleFireAni::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
}

void ParticleFireAni::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool ParticleFireAni::init(){
    if (!CCNode::init()) {
        return false;
    }
    
    m_clickNode = CCLoadSprite::createScale9Sprite("8999");
    m_clickNode->setOpacity(2);
    m_clickNode->setContentSize(CCSize(120,120));
    this->addChild(m_clickNode,-100);
    
    m_partileNode = CCNode::create();
    this->addChild(m_partileNode);
    
    m_clickParNode = CCNode::create();
    this->addChild(m_clickParNode);
    
    m_first = true;
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_partileNode->addChild(particle);
    }
    m_touchTime = 0;
    return true;
}

bool ParticleFireAni::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    unsigned long current = clock()/1000;
    if((current-m_touchTime)>200){
        m_touchTime = current;
        return true;
    }
    return false;
}

void ParticleFireAni::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int gapTime = clock()/1000 - m_touchTime;
    if(isTouchInside(m_clickNode, pTouch)){
        std::string file = "FireGrow";
        int rand = GlobalData::shared()->getRand(0,10000000);
        if(rand%2==0 && !m_first && m_partileNode->getChildren().size()>0){
            file = "FireFlicker";
            m_partileNode->removeAllChildrenWithCleanup(true);
        }else{
            int num = m_partileNode->getChildren().size();
            if(num<=0){
                for (int i=1; i<=4; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
                    if(m_first){
                        particle->setScale(1.5);
                    }
                    m_partileNode->addChild(particle);
                }
            }
            m_first = false;
        }
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s_%d",file.c_str(),i)->getCString());
            m_clickParNode->addChild(particle);
        }
    }
}

void ParticleFireAni::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}