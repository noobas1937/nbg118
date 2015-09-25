//
//  FlyHint.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-11.
//
//

#include "FlyHint.h"
#include "SceneController.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"
#include "ChatServiceCocos2dx.h"
#include "LotteryController.h"
#include "DynamicResourceController.h"
#include "MailController.h"

static vector<FlyHint*> flyTips;

FlyHint *FlyHint::create(std::string icon, std::string titleText, std::string contentText, float time,float dy,bool useDefaultIcon){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        //两种情况：由java触发，由2dx触发
        if(true){
            ChatServiceCocos2dx::flyHint(icon, titleText, contentText, time, dy, useDefaultIcon);
        }else{
            //scheduleOnce(schedule_selector(FlyHint::showInNativeUI), 0.1f);
        }
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::isChatShowing_fun()) {
        ChatServiceCocos2dx::flyHint(icon, titleText, contentText, time, dy, useDefaultIcon);
        return NULL;
    }
#endif
    FlyHint *ret = new FlyHint(icon, titleText, contentText, time,dy);
    if(ret && ret->init(useDefaultIcon)){
        ret->autorelease();
//        flyTips.push_back(ret);
//        if(!flyTips[0]->m_isShowing){
//            flyTips[0]->show();
//        }
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FlyHint::showInNativeUI(float time){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::flyHint(m_icon, m_title, m_content, m_time, m_dy, m_useDefaultIcon);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::flyHint(m_icon, m_title, m_content, m_time, m_dy, m_useDefaultIcon);
#endif
}

void FlyHint::show(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_ALL_HINT);
    m_isShowing = true;
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->addChild(this,1000,19999);
    
    this->setScaleY(0);
    CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCScaleTo::create(0.2, 1, 1)));
    CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCScaleTo::create(0.4, 1, 0.1)));
    this->runAction(CCSequence::create(
                                       //CCMoveTo::create(0.2, ccp(size.width / 2, size.height - TOP_HEIGHT - 150))
                                       easeInAction
                                       , CCDelayTime::create(2.0)
                                       , easeInAction1
                                       , CCCallFuncO::create(this, callfuncO_selector(FlyHint::removeSelf),NULL)
                                       , NULL
                                       ));
}

void FlyHint::removeAllSelf(CCObject* p)
{
    if (m_mailId != "")
    {
        int posY = this->getPositionY()+120;
        auto size = CCDirector::sharedDirector()->getWinSize();
        if(posY > size.height- 230){
            posY -= 140 * 2;
        }
        this->setPositionY(posY);
        return;
    }
    removeSelf(p);
}

void FlyHint::removeSelf(CCObject* p){
//    flyTips.erase(flyTips.begin());
//    if(flyTips.size()>0){
//        auto hint = flyTips[0];
//        hint->show();
//    }
//    this->release();
    this->removeFromParent();
    PopupViewController::getInstance()->removePopFlyHint(this);
}

bool FlyHint::init(bool useDefaultIcon){
//    PopupBaseView* __tmpNode = PopupViewController::getInstance()->getCurrentPopupView();
//    if (__tmpNode && !__tmpNode->getIsHDPanel())
//    {
//        CCBLoadFile("FlyHintCCB", this, this);
//    }
//    else
//    {
//        CCBLoadFile("FlyHintCCB_HD", this, this);
//    }

    CCBLoadFile("FlyHintCCB", this, this);
    
    m_useDefaultIcon = useDefaultIcon;
    m_isShowing = false;
    m_titleText->setString(m_title.c_str());
    m_contentText->setString(m_content.c_str());
    m_mailId = "";
    if(m_title==""){
        m_contentText->setPositionY(0);
    }else{
        m_contentText->setPositionY(-14);
    }
    DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE, false);
    setCleanFunction([](){
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,true);
    });
    
    if(m_icon!=""){
        CCNode* sprite = CCLoadSprite::createSprite(m_icon.c_str(),useDefaultIcon,CCLoadSpriteType_DEFAULT);
        if (atoi(m_icon.c_str())>=200 && atoi(m_icon.c_str())<=205)
        {
            sprite = LotteryController::shared()->getLabelBMfont(atoi(m_icon.c_str()));
        }
        float sx = 56.4 / sprite->getContentSize().width;
        float sy = 56.4 / sprite->getContentSize().height;
        sprite->setScale(sx > sy ? sy : sx);
        this->m_picContainer->addChild(sprite);
        this->m_flyHeadSpr->setVisible(true);
    }else{
        this->m_flyHeadSpr->setVisible(false);
    }
    
    auto size = CCDirector::sharedDirector()->getWinSize();
    float posY = PopupViewController::getInstance()->getTouchPoint().y+m_dy;
    int gap = 140;
    posY += gap;
    if(posY > size.height- 230){
        posY -= gap * 2;
    }
    
    this->setPosition(ccp(size.width / 2, posY));
    
    PopupViewController::getInstance()->addPopFlyHint(this);
//    show();
    
    return true;
}

void FlyHint::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(true);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FlyHint::removeAllSelf), REMOVE_ALL_HINT, NULL);
}

void FlyHint::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REMOVE_ALL_HINT);
    CCNode::onExit();
}

bool FlyHint::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, this->m_picContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyHeadSpr", CCSprite*, this->m_flyHeadSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);

    return NULL;
}

void FlyHint::setMailId(string mailId){
    m_mailId = mailId;
}

bool FlyHint::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_mailId != "" && isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void FlyHint::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_mailId != "" ) {
//        PopupViewController::getInstance()->removeAllPopupView();
//        MailController::getInstance()->openMailReadPop(m_mailId);
        m_mailId = "";
    }
}
