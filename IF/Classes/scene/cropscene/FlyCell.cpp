//
//  FlyCell.cpp
//  IF
//
//  Created by fubin on 14-7-31.
//
//

#include "FlyCell.h"
#include "UIComponent.h"

using namespace cocos2d;

FlyCell* FlyCell::create(CCPoint spt, CCPoint ept, int type, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration)
{
    FlyCell *pRet = new FlyCell();
    if (pRet && pRet->initFlyCell(spt,ept,type, picPath,picCnt,batchNode, duration))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FlyCell::initFlyCell(CCPoint spt, CCPoint ept, int type, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration)
{
    m_batchNode = batchNode;
    m_type = type;
    m_Spr = CCSprite::create();
    if(m_type==Gold){
        m_Spr->initWithSpriteFrame(CCLoadSprite::loadResource(picPath.c_str()));
    }else{
        string curPath = picPath+"_%d.png";
        CCCommonUtils::makeEffectSpr(m_Spr, curPath, picCnt, 0.07);
    }
    m_Spr->setPosition(spt);
    m_Spr->setScale(0.9);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_Spr->setScale(1.5);
    }
    
    CCPoint gap = ccpSub(ept, spt);
    float len = ccpLength(gap);
    float useTime = len*1.0/500;
    if (CCCommonUtils::isIosAndroidPad())
    {
        useTime = len * 1.0 / 1000;
    }
    
    int td = duration*10/2;
    if (td%2==0) {
        td = -td;
    }
    
    ccBezierConfig  beziercofig;
    beziercofig.controlPoint_1 = ccp(spt.x+20*td,spt.y);
    beziercofig.controlPoint_2 = ept;
    beziercofig.endPosition = ept;
    CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
    
    m_Spr->setOpacity(0);
    CCActionInterval * delatTime = CCDelayTime::create(duration);
    CCActionInterval * fadeIn = CCFadeIn::create(0);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(FlyCell::onFlyCellDelete));
    m_batchNode->addChild(m_Spr);
    
    m_Spr->runAction(CCSequence::create(delatTime, fadeIn, forward, funcall, NULL));
    return true;
}
FlyCell* FlyCell::createCircle(CCPoint spt, CCPoint ept, string picPath, int picCnt, CCSpriteBatchNode* batchNode, float duration, float param1,float param2)
{
    FlyCell *pRet = new FlyCell();
    if (pRet && pRet->initCircleFly(spt,ept, picPath, picCnt,batchNode, duration,param1,param2))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
bool FlyCell::initCircleFly(cocos2d::CCPoint spt, cocos2d::CCPoint ept, string picPath, int picCnt, cocos2d::CCSpriteBatchNode *batchNode, float duration, float param1,float param2){
    m_batchNode = batchNode;
    m_type = Wood;
    m_Spr = CCSprite::create();
    string curPath = picPath+"%d.png";
    CCCommonUtils::makeEffectSpr(m_Spr, curPath, picCnt, 0.07);
    m_Spr->setPosition(spt);
    m_Spr->setScale(0.5);
    
    CCPoint gap = ccpSub(ept, spt);
    float len = ccpLength(gap);
    float useTime = len*1.0/250;

    float tmpX = spt.x + (ept.x - spt.x) * 0.7;
    float tmpY = spt.y + (ept.y - spt.y) * 0.7;

    ccBezierConfig  beziercofig0;
    beziercofig0.controlPoint_1 = ccp(spt.x+param1,spt.y);
    beziercofig0.controlPoint_2 = ccp(tmpX + param1 * 0.3, tmpY + param1 * 0.1);
    beziercofig0.endPosition = ccp(tmpX + param1 * 0.3, tmpY + param1 * 0.1);
    CCActionInterval *forward0 = CCBezierTo::create(useTime*0.5, beziercofig0);
    
    
    ccBezierConfig  beziercofig;
    beziercofig.controlPoint_1 = ccp(tmpX+param2,tmpY);
    beziercofig.controlPoint_2 = ept;
    beziercofig.endPosition = ept;
    CCActionInterval *forward = CCBezierTo::create(useTime*0.5, beziercofig);
    
    m_Spr->setOpacity(0);
    CCActionInterval * delatTime = CCDelayTime::create(duration);
    CCActionInterval * fadeIn = CCFadeIn::create(0.1);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(FlyCell::onFlyCellDelete));
    m_batchNode->addChild(m_Spr);
    
    m_Spr->runAction(CCSequence::create(delatTime, fadeIn, forward0, forward, funcall, NULL));
    return true;
}
FlyCell* FlyCell::createCycleLight(string picPath,float height,int idx,int cnt,CCSpriteBatchNode* batchNode,ccColor3B &color,CCPoint spt){
    FlyCell *pRet = new FlyCell();
    if (pRet && pRet->initCycleLight(picPath,height,idx,cnt,batchNode,color,spt)){
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
bool FlyCell::initCycleLight(string picPath,float height,int idx,int cnt,CCSpriteBatchNode* batchNode,ccColor3B &color,CCPoint spt){
    if(picPath.empty())
        return false;
    m_batchNode = batchNode;
    m_Spr = CCLoadSprite::createSprite(picPath.c_str());
    m_Spr->setColor(color);
    m_batchNode->addChild(m_Spr);
    m_Spr->setPosition(spt);
    m_cycleH = height;
    m_cycleIDX = idx;
    m_cycleCNT = cnt;
    m_cyclePoint = spt;
    onFlyCellCycle(CCBool::create(true));
    return true;
}

void FlyCell::onEnter() {
    CCNode::onEnter();
}
void FlyCell::onExit() {
    CCNode::onExit();
}

void FlyCell::onFlyCellDelete(){
    UIComponent::getInstance()->playIconByType(m_type);
    m_batchNode->removeChild(m_Spr, true);
    this->removeFromParent();
}
void FlyCell::onFlyCellCycle(CCObject *ccObj){
    m_Spr->stopAllActions();
    float duration = m_cycleH / 10;
    if(ccObj != NULL){
        m_Spr->setPosition(ccp(m_cyclePoint.x,m_cyclePoint.y+m_cycleH * m_cycleIDX/m_cycleCNT));
        duration = (duration/m_cycleCNT)*(m_cycleCNT - m_cycleIDX);
    }else{
        m_Spr->setPosition(m_cyclePoint);
    }

    auto fadeIn = CCFadeIn::create(duration*0.5);
    auto fadeOut = CCFadeOut::create(duration*0.5);
    CCCallFuncO *funcall = CCCallFuncO::create(this, callfuncO_selector(FlyCell::onFlyCellCycle), NULL);
    CCSequence *sequence1 = CCSequence::create(fadeIn,fadeOut,NULL);
    CCSequence *sequence2 = CCSequence::create(CCMoveTo::create(duration, CCPoint(m_cyclePoint.x, m_cyclePoint.y+m_cycleH)),funcall,NULL);
    m_Spr->runAction(CCSpawn::createWithTwoActions(sequence1, sequence2));
}
