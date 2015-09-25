//
//  FlySystemUpdateHint.cpp
//  IF
//
//  Created by liweiyu on 14-9-15.
//
//

#include "FlySystemUpdateHint.h"
#include "SceneController.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"
#include "FlyRollingText.h"
#include "AllianceScienceView.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "ChatView.h"
#include "ChatController.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "ChatServiceCocos2dx.h"

FlySystemUpdateHint *FlySystemUpdateHint::create(double countDown, bool isLogin,string tip,FlyHintType type,string icon){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        //两种情况：由java触发，由2dx触发
        if(true){
            double curTime = GlobalData::shared()->getWorldTime();
            double countDownTime = GlobalData::shared()->changeTime(countDown/1000);
            bool isFlyHintLogin=false;
            if(type==FLY_HINT_LOGIN)
                isFlyHintLogin=true;
            ChatServiceCocos2dx::flySystemUpdateHint(floor( countDownTime - curTime ), isFlyHintLogin,isLogin, tip, icon);
        }else{
//            scheduleOnce(schedule_selector(FlySystemUpdateHint::showInNativeUI), 0.1f);
        }
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::isChatShowing_fun()){
        if(true){
            double curTime = GlobalData::shared()->getWorldTime();
            double countDownTime = GlobalData::shared()->changeTime(countDown/1000);
            bool isFlyHintLogin=false;
            if(type==FLY_HINT_LOGIN)
                isFlyHintLogin=true;
            ChatServiceCocos2dx::flySystemUpdateHint(floor( countDownTime - curTime ), isFlyHintLogin,isLogin, tip, icon);
        }else{
        
        }
        
        return NULL;
    }
#endif
    FlySystemUpdateHint *ret = new FlySystemUpdateHint(countDown, isLogin,tip,type,icon);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FlySystemUpdateHint::showInNativeUI(float time){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bool isFlyHintLogin=false;
    if(m_type==FLY_HINT_LOGIN)
        isFlyHintLogin=true;
    ChatServiceCocos2dx::flySystemUpdateHint(m_countDown, isFlyHintLogin, m_isLogin, m_tip, m_icon);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool isFlyHintLogin=false;
    if(m_type==FLY_HINT_LOGIN)
    isFlyHintLogin=true;
    ChatServiceCocos2dx::flySystemUpdateHint(m_countDown, isFlyHintLogin, m_isLogin, m_tip, m_icon);
#endif
}

void FlySystemUpdateHint::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FlySystemUpdateHint::removeSelf), "remove_hint", NULL);
//    m_time = m_countDown;
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    this->schedule(schedule_selector(FlySystemUpdateHint::loop), 1);
}

void FlySystemUpdateHint::onExit(){
    this->unschedule(schedule_selector(FlySystemUpdateHint::loop));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "remove_hint");
    setTouchEnabled(false);
    CCNode::onExit();
}

bool FlySystemUpdateHint::init()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("remove_hint");
    
    CCBLoadFile("FlySystemUpdateHint", this, this);
//    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
//    layer->addChild(this,1000);
//    auto size = CCDirector::sharedDirector()->getWinSize();
//    this->setPosition(ccp(size.width / 2, size.height-210));
    
//    this->setPosition(ccp(230, 185));
//    this->setPosition(ccp(size.width / 2, 185));
    
    double curTime = GlobalData::shared()->getWorldTime();
    m_countDown = GlobalData::shared()->changeTime(m_countDown/1000);
    m_time = (long)floor( m_countDown - curTime );
    string delayTime = CC_ITOA(m_time/60);
    //delayTime.append("m");

    if(m_type==FLY_HINT_KING && m_icon==""){
        string temp = "";
        int num = 2;
        for (int i=0; i<num; i++) {
            temp.append(m_tip);
            if(i!=1){
                temp.append("          ");
            }
        }
        m_tip = temp;
    }
    m_rollText = FlyRollingText::create(m_tip.c_str(), 320, 24, ccc3(245,233,215), 60, 1,m_icon);
    m_rollTime = m_rollText->getRollingTime();
    this->setScaleY(0);
    one = CCSequence::create(CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::showUIQuestNode))
                             ,CCScaleTo::create(0.2, 1, 1)
                             , CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::rollText))
                             , CCDelayTime::create(m_rollTime)
                             , CCScaleTo::create(0.4, 1, 0)
                             , CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::hideUIQuestNode))
                             , NULL
                             );
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        one = CCSequence::create(CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::showUIQuestNode))
                                 ,CCScaleTo::create(0.2, 2, 2)
                                 , CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::rollText))
                                 , CCDelayTime::create(m_rollTime)
                                 , CCScaleTo::create(0.4, 2, 0)
                                 , CCCallFunc::create(this, callfunc_selector(FlySystemUpdateHint::hideUIQuestNode))
                                 , NULL);
    }
    
    return true;
}
float FlySystemUpdateHint::getRollTime(){
    return m_rollTime + 0.6;
}
void FlySystemUpdateHint::rollText(){
    m_container->removeAllChildren();
    if(m_type==FLY_HINT_LOGIN){
        if(m_time+1>=60){
            string delayTime = CC_ITOA((m_time+1)/60);
            //delayTime.append("m");
            FlyRollingText* rollText = FlyRollingText::create(CCString::createWithFormat("%s",_lang_1("105324", delayTime.c_str()))->getCString(), 320, 24, ccc3(245,233,215), 60, 1);
            m_container->addChild(rollText);
        }
        else if(m_time+1<60){
            string delayTime = CC_ITOA((m_time+1));
            //delayTime.append("s");
            FlyRollingText* rollText = FlyRollingText::create(CCString::createWithFormat("%s",_lang_1("105325", delayTime.c_str()))->getCString(), 320, 24, ccc3(245,233,215), 60, 1);
            m_container->addChild(rollText);
        }
    }else{
        FlyRollingText* rollText = FlyRollingText::create(m_tip.c_str(), 320, 24, ccc3(245,233,215), 60, 1,m_icon);
        m_container->addChild(rollText);
    }
}

void FlySystemUpdateHint::loop(float _time){
//    CCLOG("TTTTTTTTTTT %s %f",CC_SECTOA(m_time),m_countDown);
    if(m_time <= 0){
        removeSelf(NULL);
        return;
    }
    if(m_type==FLY_HINT_LOGIN && (m_time == 900 || m_time == 600 || m_time == 300 || m_time == 60)){
        this->stopAllActions();
        this->runAction(one);
//        this->getAnimationManager()->runAnimationsForSequenceNamed("flash");
    }
    else if(m_isLogin)
    {
        m_isLogin = false;
        this->stopAllActions();
        this->runAction(one);
        if(m_type==FLY_HINT_LOGIN){
            this->getAnimationManager()->runAnimationsForSequenceNamed("flash");   
        }
    }

    m_time -= 1;
}

void FlySystemUpdateHint::removeSelf(CCObject* obj){
    this->removeFromParent();
}

void FlySystemUpdateHint::showUIQuestNode()
{
    return;
    UIComponent::getInstance()->showUIQuestNode(false);
}

void FlySystemUpdateHint::hideUIQuestNode()
{
    return;
    UIComponent::getInstance()->showUIQuestNode(true);
}

bool FlySystemUpdateHint::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, this->m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    return NULL;
}

bool FlySystemUpdateHint::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return false;
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void FlySystemUpdateHint::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_clickArea, pTouch) && m_type == FLY_HINT_KING){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        int index  = WorldController::getInstance()->getIndexByPoint(ccp(600,600));
        CCPoint pt = WorldController::getPointByIndex(index);
        PopupViewController::getInstance()->removeAllPopupView();
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
            WorldMapView::instance()->openTilePanel(index);
        }else{
            WorldController::getInstance()->openTargetIndex = index;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
    }else if(isTouchInside(m_clickArea, pTouch) && m_type == FLY_HINT_ALLIANCE_INVITE){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if(GlobalData::shared()->android_native_chat == 1 ){
            if(!ChatServiceCocos2dx::isChatShowing)
            {
                ChatServiceCocos2dx::m_channelType = CHANNEL_TYPE_COUNTRY;
//                ChatServiceCocos2dx::m_isInMailDialog=false;
//                if(ChatController::getInstance()->isFirstRequestCountry)
//                    ChatController::getInstance()->sendRequestChat(CHAT_COUNTRY);
                ChatServiceCocos2dx::showChatActivityFrom2dx();
            }
        }else{
            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COUNTRY),false);
        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun()){
                ChatServiceCocos2dx::m_channelType = CHANNEL_TYPE_COUNTRY;
                ChatServiceCocos2dx::m_isInMailDialog=false;
                if(ChatController::getInstance()->isFirstRequestCountry)
                    ChatController::getInstance()->sendRequestChat(CHAT_COUNTRY);
                ChatServiceCocos2dx::showChatIOSFrom2dx();
            }
        }else{
            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COUNTRY),false);
        }
#else
        //关闭cocos2d 聊天入口
        //PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COUNTRY),false);
#endif
    }
}

void FlySystemUpdateHint::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    //    m_moveFlag = true;
}
