//
//  SpriteSheetAni.cpp
//  IF
//
//  Created by ganxiaohua on 13-8-29.
//
//

#include "SpriteSheetAni.h"
using namespace cocos2d;

bool SpriteSheetAni::init(){
    if ( !CCSprite::init() )
    {
        return false;
    }
    return true;
}

void SpriteSheetAni::onExit() {
    stopAllActions();
    CCSprite::onExit();
}

SpriteSheetAni* SpriteSheetAni::create(const char* aniName,int loop,bool endRemove,int type,float delayPerUnit,int totalFrame)
{
    SpriteSheetAni *pRet = new SpriteSheetAni(aniName,loop,endRemove,type,delayPerUnit,totalFrame);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        pRet->createAnimation();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void SpriteSheetAni::createAnimation()
{
    if(mAniName!=NULL){
        Vector<AnimationFrame*> pArray;
        for(int i=0; i<=mtotalFrame; ++i)
        {
            char buffer[50]= {0};
            sprintf(buffer, "%s%d.png", mAniName,i);
            CCSpriteFrame *pSpriteFrame = CCLoadSprite::getSF(buffer);
            CC_BREAK_IF(!pSpriteFrame);
            CCAnimationFrame *pAnimationFrame = new CCAnimationFrame();
            pAnimationFrame->initWithSpriteFrame(pSpriteFrame, 0.3f,ValueMap());
            pArray.pushBack(pAnimationFrame);
            pAnimationFrame->release();
        }
        CCAnimation *pAnimation = CCAnimation::create(pArray, 0.3f,1);
        pAnimation->setDelayPerUnit(mdelayPerUnit);
        pAnimation->setLoops(mLoop);
        pAnimation->setRestoreOriginalFrame(false);
        
        this->setAnimation(pAnimation);
        this->setAnimationEnd(CCCallFuncN::create(this, callfuncN_selector(SpriteSheetAni::onAnimationEnd)));
    }
}

//void SpriteSheetAni::setAF(CCArray* arrayAF){
//    marrayAF = arrayAF;
//    CCAnimation *pAnimation = CCAnimation::create(arrayAF, 0.3f,1);
//    pAnimation->setDelayPerUnit(mdelayPerUnit);
//    pAnimation->setLoops(mLoop);
//    pAnimation->setRestoreOriginalFrame(false);
//    
//    this->setAnimation(pAnimation);
//    this->setAnimationEnd(CCCallFuncN::create(this, callfuncN_selector(SpriteSheetAni::onAnimationEnd)));
//}

void SpriteSheetAni::play(float delayTime)
{
    CC_ASSERT(this->manimation);
    this->runAction(CCSequence::create(CCDelayTime::create(delayTime),CCAnimate::create(this->manimation), this->manimationEnd, NULL));
}

void SpriteSheetAni::onAnimationEnd(cocos2d::CCNode* node)
{
    if(mendRemove)
    {
        CCNode* parent = this->getParent();
        if( NULL != parent )
        {
            parent->removeChild(this, true);
        }
    }
    this->callback();
}

void SpriteSheetAni::callback() {
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
}

void SpriteSheetAni::setCallback(cocos2d::CCObject *target, SEL_CallFunc func) {
    m_callbackTarget = target;
    m_callbackFunc = func;
}