//
//  IFShakeGuideLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#include "IFShakeGuideLayer.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#define  modelWidth (CCCommonUtils::isIosAndroidPad()? 1536 : 670)
IFShakeGuideLayer* IFShakeGuideLayer::create()
{
    IFShakeGuideLayer *pRet = new IFShakeGuideLayer();
    if (pRet && pRet->initData())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IFShakeGuideLayer::initData()
{
    this->setVisible(false);
    m_defaultWidth = 140;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_defaultWidth = 240;
    }
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    
    CCBLoadFile("shakeGuideLayer",this,this);
    string infoStr("");
    int guideTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(MSG_SHAKEGUIDETIME, 0);
    
    string shakePicStr = GlobalData::shared()->shakePicStr;
    vector<string> picVec ;
    CCCommonUtils::splitString(shakePicStr, "|", picVec);
    
    int count = picVec.size();
    int index = -1;
    if(count > 0 ){
        index = rand() % count;
    }
    if (index != -1) {
        m_headImgNode = HFHeadImgNode::create();
        m_headImgNode->initHeadImgUrl2(m_head, CCCommonUtils::getShakePicUrl(picVec[index]), 1.0f, 100, true);
        CCLOG("shakelog: picUrl%s",CCCommonUtils::getShakePicUrl(picVec[index]).c_str());
    }
    schedule(schedule_selector(IFShakeGuideLayer::monitorPic), 0.5);
    m_startTime  = WorldController::getInstance()->getTime()/1000;
    if (guideTime == 0) {
        infoStr = _lang("113100");
    }else if (guideTime == 1){
        infoStr = _lang("113108");
    }else if (guideTime == 2){
        infoStr = _lang("113103");
    }else{
        int randContainerArr[] = {113100,113101,113102,113103,113108,113109};
        int randNUm = rand() % 6;
        CCLOG("shakelog: randnum  %d",randNUm);
        infoStr = _lang(CC_ITOA(randContainerArr[randNUm]));
    }
    m_hintText->setString(infoStr.c_str());
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKEGUIDETIME, ++guideTime);

    m_bgNode->setVisible(false);
    m_bgNode->retain();
    m_bgNode->removeFromParent();
    m_clipNode = CCClipNode::create(modelWidth, 150);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_clipNode = CCClipNode::create(modelWidth, 320);
    }
    m_clipNode->setAnchorPoint(ccp(0, 0));
    m_clipNode->addChild(m_bgNode);
    m_bgNode->release();
    this->addChild(m_clipNode);
    return true;
}

void IFShakeGuideLayer::onEnter() {
    setTouchEnabled(true);
    CCNode::onEnter();
}
void IFShakeGuideLayer::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool IFShakeGuideLayer::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if (!WorldController::getInstance()->isInWorld) {
        return false;
    }
    if(WorldMapView::instance()->isMiniMapModel()){
        return false;
    }
    if(isTouchInside(UIComponent::getInstance()->m_miniBG, pTouch)){
        return true;
    }
    if(isTouchInside(WorldMapView::instance()->m_directionIcon, pTouch) && WorldMapView::instance()->m_directionIcon->isVisible()){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_coordBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_faveBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_skillBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_goHomeBtn, pTouch) && !GlobalData::shared()->playerInfo.isInSelfServer()){
        return true;
    }
    return false;
}
void IFShakeGuideLayer::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    
}
SEL_CCControlHandler IFShakeGuideLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool IFShakeGuideLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    return false;
}

void IFShakeGuideLayer::removeOneself(float t){
    this->removeFromParent();
}

void IFShakeGuideLayer::onGameTick(float time){
    int height = m_clipNode->getContentSize().height;
    m_defaultWidth = m_defaultWidth + 100;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_defaultWidth += 100;
    }
    if(m_defaultWidth >= modelWidth){
        unschedule(schedule_selector(IFShakeGuideLayer::onGameTick));
        scheduleOnce(schedule_selector(IFShakeGuideLayer::removeOneself), 10.0);
        return;
    }
    m_clipNode->setContentSize(CCSizeMake(m_defaultWidth, height));
    m_bgNode->setVisible(true);
}

bool IFShakeGuideLayer::isPicBack(){
if(m_head->getChildrenCount() > 0)
    {
        return true;
    }
else{
        return false;
    }
}
void IFShakeGuideLayer::monitorPic(float time){
    if (isPicBack()) {
        this->setVisible(true);
        unschedule(schedule_selector(IFShakeGuideLayer::monitorPic));
        schedule(schedule_selector(IFShakeGuideLayer::onGameTick), 0.01);
    }
    else{
        if(WorldController::getInstance()->getTime() / 1000 - m_startTime > 10 ){
            unschedule(schedule_selector(IFShakeGuideLayer::monitorPic));
            CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKETIME, -1);
            this->removeFromParent();
        }
    }
    
}