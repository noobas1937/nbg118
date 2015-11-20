//
//  GongJian2.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-26.
//
//

#include "GongJian2.h"

bool GongJian2::init(){
//    int level = 0;
//    char buffer[50]= {0};
//    sprintf(buffer, "jian_%d.png", level);
    m_jian = CCLoadSprite::createSprite(m_pic.c_str());
    m_jian->setOpacity(0);
    m_batchNode->addChild(m_jian);
    
    m_jian->setPosition(m_startPoint);
    CCPoint gap = ccpSub(m_startPoint, m_endPoint);
    int len = MAX(abs((int)gap.x),abs((int)gap.y));
    //float time = 1.0*len/400;
    float time = 1.0*len/mSpeed;
    if(time<0.2){
        time = 0.2;
    }
    subX = gap.x/(25*time);
    subY = gap.y/(25*time);
    float angle=CCMathUtils::getAngle(m_startPoint, m_endPoint);
    float angle2 = fabsf(angle);
    //angle = fabsf(angle);
    CCSequence* s1;
    CCSequence* s2;
    //float scale = 0.6;
    float scale = mScale; //a by ljf
    ccBezierConfig  beziercofig;
    if(gap.y<0){//从下往上打
        if(angle2<15){
            angle = 0;
            beziercofig.controlPoint_1 = m_startPoint;
            beziercofig.controlPoint_2 = m_endPoint;
        }else{
            if(gap.x>0){
                beziercofig.controlPoint_1 = ccp(m_startPoint.x+20,m_startPoint.y);
                beziercofig.controlPoint_2 = ccp(m_startPoint.x+20,m_endPoint.y);
            }else{
                beziercofig.controlPoint_1 = ccp(m_startPoint.x-20,m_startPoint.y);
                beziercofig.controlPoint_2 = ccp(m_startPoint.x-20,m_endPoint.y);
            }
            angle = 90-angle;
        }
        s1 = CCSequence::create(CCScaleTo::create(time*0.25, scale),CCScaleTo::create(time*0.75, scale,0.5*scale),NULL);
        s2 = CCSequence::create(CCRotateTo::create(time*0.8, angle),NULL);//
    }else{
        if(angle2>85 && angle2<105){
            m_jian->setFlipY(true);
            if(gap.x>0 && gap.y>0){
                angle = 180+angle;
                angle = 90-angle;
            }else{
                angle = -(90+angle);
            }
            beziercofig.controlPoint_1 = m_startPoint;
            beziercofig.controlPoint_2 = m_endPoint;
        }else{
            if(gap.x>0 && gap.y>0){
                m_jian->setRotation(-150);
                angle = -120;
            }else{
                m_jian->setRotation(70);
                angle = 160;
            }
            beziercofig.controlPoint_1 = ccp(m_endPoint.x,m_startPoint.y);
            beziercofig.controlPoint_2 = ccp(m_endPoint.x-20,m_endPoint.y);
        }
        s1 = CCSequence::create(CCScaleTo::create(time*0.15, scale),CCScaleTo::create(time*0.85, scale,0.5*scale),NULL);
        s2 = CCSequence::create(CCRotateTo::create(time*0.8, angle),NULL);
        
    }
    //CCActionInterval * easeSineIn = CCEaseSineIn::create(CCMoveTo::create(time, m_defSoldier->getCurrentPosition()));
    m_jian->setScale(0.8*scale);
    beziercofig.endPosition = m_endPoint;
//    CCActionInterval *forward = CCBezierTo::create(time, beziercofig);
    CCActionInterval *forward = CCJumpTo::create(time, m_endPoint, 25, 1);
    
    
    CCSequence* s3 = CCSequence::create(forward,CCCallFunc::create(this, callfunc_selector(GongJian2::removeJian)),NULL);
    CCSequence* s4 = CCSequence::create(CCFadeIn::create(time/18.0),CCDelayTime::create(16*time/18.0),CCFadeOut::create(1*time/18.0),NULL);
    CCSpawn* sp = CCSpawn::create(s1,s2,s3,s4,NULL);
    m_jian->runAction(sp);
    return true;
}

void GongJian2::removeJian(){
    m_jian->stopAllActions();
    m_jian->removeFromParentAndCleanup(true);
    m_jian = NULL;
    this->remove();
}

void GongJian2::update(float time){

}

GongJian2* GongJian2::create(CCNode * batchNode,CCPoint startPoint,CCPoint endPoint,int side, string pic, float scale, float speed){
    GongJian2* ret = new GongJian2(batchNode,startPoint,endPoint,side,pic, scale, speed);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
