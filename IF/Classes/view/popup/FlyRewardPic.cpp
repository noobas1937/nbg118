//
//  FlyRewardPic.cpp
//  IF
//
//  Created by lifangkai on 14-8-12.
//
//

#include "FlyRewardPic.h"
#include "UIComponent.h"

using namespace cocos2d;

FlyRewardPic* FlyRewardPic::create(string type,CCPoint spt,string picPath,float delaytime=0.3)
{
    FlyRewardPic *pRet = new FlyRewardPic(type,spt,picPath,delaytime);
    if (pRet && pRet->init(type,spt,picPath,delaytime))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
void FlyRewardPic::addFlyRewardAnim(string type, cocos2d::CCPoint spt, string picPath,float delaytime){
    auto node = UIComponent::getInstance();
    auto layer = node->getChildByTag(90001);
    if(layer==NULL){
        CCModelLayerColor* m_modalLayer = CCModelLayerColor::create();
        m_modalLayer->setOpacity(200);
        m_modalLayer->setColor(ccBLACK);
        m_modalLayer->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_modalLayer->setAnchorPoint(ccp(0, 0));
        node->addChild(m_modalLayer,10001,90001);
        layer = m_modalLayer;
    }
    //转换坐标
    //CCPoint localP = layer->convertToNodeSpaceAR(spt);
    FlyRewardPic* flyRewardPic = FlyRewardPic::create(type, spt, picPath,delaytime);
    node->addChild(flyRewardPic,10002);
    //flyRewardPic->setPosition(spt);
}
bool FlyRewardPic::init(string type,CCPoint spt,string picPath,float delaytime)
{
    if(type=="goods"){
        m_endpoint = ccp(100,100);
    }
    m_Spr= CCLoadSprite::createSprite(picPath.c_str());
    m_Spr->setPosition(spt);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_Spr->setScale(1.5);
    }
    m_Spr->setAnchorPoint(ccp(0.5,0.5));
    this->addChild(m_Spr);
    playScaleAnim();
  
    return true;
}
void FlyRewardPic::playScaleAnim(){
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(FlyRewardPic::playFlyAnim));
    if (CCCommonUtils::isIosAndroidPad()) {
        CCScaleTo* iconRotate = CCScaleTo::create(0.125f, 1.5*2);
        CCScaleTo* iconRotate1 = CCScaleTo::create(0.125f, 1.2*2);
        CCScaleTo* iconRotate2 = CCScaleTo::create(0.125f, 1.4*2);
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.125f, 1.2*2);
        CCSequence* action  =CCSequence::create(iconRotate, iconRotate1,iconRotate2,iconRotate3,funcall,NULL);
        m_Spr->runAction(action);
        CCPoint pos = ccp(m_Spr->getPositionX(), m_Spr->getPositionY()+100);
        CCMoveTo* moveToAction = CCMoveTo::create(0.125f, pos);
        m_Spr->runAction(moveToAction);
    }
    else {
        CCScaleTo* iconRotate = CCScaleTo::create(0.125f, 1.5);
        CCScaleTo* iconRotate1 = CCScaleTo::create(0.125f, 1.2);
        CCScaleTo* iconRotate2 = CCScaleTo::create(0.125f, 1.4);
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.125f, 1.2);
        CCSequence* action  =CCSequence::create(iconRotate, iconRotate1,iconRotate2,iconRotate3,funcall,NULL);
        m_Spr->runAction(action);
        CCPoint pos = ccp(m_Spr->getPositionX(), m_Spr->getPositionY()+100);
        CCMoveTo* moveToAction = CCMoveTo::create(0.125f, pos);
        m_Spr->runAction(moveToAction);
    }
}
void FlyRewardPic::playFlyAnim(){
    //转还坐标
    auto node = UIComponent::getInstance();
    CCPoint p = node->m_store->getPosition();
    CCDirector::sharedDirector()->convertToGL(p);
    CCPoint p2 = node->m_store->getParent()->convertToWorldSpace(p);
    // m_endpoint = node->convertToNodeSpaceAR(p2);
    m_endpoint = this->convertToNodeSpace(p2);
    
    CCPoint stp = m_Spr->getPosition();
    CCDirector::sharedDirector()->convertToGL(stp);
    CCPoint p3 = m_Spr->getParent()->convertToWorldSpace(stp);
    //CCPoint startPoint = node->convertToNodeSpaceAR(p3);
    CCPoint startPoint = p3;
    
    int num = rand() % 3+1;
    float useTime = 1.0;
//    ccBezierConfig  beziercofig;
//    beziercofig.controlPoint_1 = startPoint;
//    beziercofig.controlPoint_2 = startPoint+ccp((m_endpoint.x-startPoint.x)/num+startPoint.x, (m_endpoint.y-startPoint.y)/num+startPoint.y);
//    beziercofig.endPosition = m_endpoint;
//    CCActionInterval *bzAction = CCBezierTo::create(useTime, beziercofig);
    CCMoveTo* moveToAction = CCMoveTo::create(0.5f, m_endpoint);
    CCActionInterval * delatTime = CCDelayTime::create(m_delayTime);
    CCActionInterval * fadeIn = CCFadeIn::create(0);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(FlyRewardPic::onFlyRewardDelete));
    
    m_Spr->runAction(CCSequence::create(delatTime, fadeIn,moveToAction, funcall, NULL));
    CCScaleTo* iconRotate = CCScaleTo::create(0.5f, 0.1);
    
    CCActionInterval * delatTime1 = CCDelayTime::create(m_delayTime);
    CCSequence* action1  =CCSequence::create(delatTime1, iconRotate,NULL);
    m_Spr->runAction(action1);
}
void FlyRewardPic::onEnter() {
    CCNode::onEnter();
}
void FlyRewardPic::onExit() {
    CCNode::onExit();
}

void FlyRewardPic::onFlyRewardDelete()
{
//    UIComponent::getInstance()->playIconByType(m_type);
//    m_batchNode->removeChild(m_Spr, true);
    this->removeFromParent();
//    this->setVisible(false);
    auto node = UIComponent::getInstance();
    node->removeChildByTag(90001);
   // auto layer = node->getChildByTag(9000);
}