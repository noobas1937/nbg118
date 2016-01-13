//
//  PopupViewController.cpp
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#include "PopupViewController.h"
#include "SceneController.h"
#include "TransitionCallback.h"
#include "UIComponent.h"
#include "WorldMapView.h"
#include "ArcPopupBaseView.h"
#include "UserUpgradeView.h"
#include "MailPopUpView.h"
#include "StoreView.h"
#include "UseCDToolView.h"
#include "UseResToolView.h"
#include "MinimapView.h"
#include "ServerListPopUpView.h"
#include "SacrificePopUpView.h"
#include "ChatServiceCocos2dx.h"
#include "MerchantView.h"
#include "LotteryActView.h"
#include "LotteryAct2View.h"
#include "ServerListPopUpView.h"
#include "RepayView.h"
#include "MailController.h"
#include "LongJingStoreView.h"
#include "GeneralTitanPopupView.h"
#include "TitanUpgradeView.h"
#include "MailSystemListPopUp.h"

static PopupViewController *_instance = NULL;
static int _view_count = 0;

PopupViewController::~PopupViewController() {
    for (auto stack : m_stack) {
        CC_SAFE_RELEASE_NULL(stack.second);
    }
    m_stack.clear();
    for (auto gbstack : m_gobackStack) {
        CC_SAFE_RELEASE_NULL(gbstack.second);
    }
    m_gobackStack.clear();
}

PopupViewController* PopupViewController::getInstance() {
    if (!_instance) {
        _instance = new PopupViewController();
        _instance->m_currentId = -1;
        _instance->CanPopPushView = true;
        _instance->m_coverSprite = TouchSprite::create();
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(_instance, callfuncO_selector(PopupViewController::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);

    }
    return _instance;
}

void PopupViewController::popupCloseEvent(cocos2d::CCObject *params)
{
	auto _paramsPrt = dynamic_cast<CCInteger*>(params);
	if (!_paramsPrt) {
		return;
	}
	int _id = _paramsPrt->getValue();
	map<int, PopupBaseView*>::iterator itGB;
	if (m_gobackStack.end() != (itGB = m_gobackStack.find(_id))) {
		CCLOG("popupCloseEvent");
		m_gobackStack.erase(itGB);
	}
}

int PopupViewController::addPopupView(PopupBaseView *view, bool useAnimation,bool needLayout) {
    if (!view) {
        return -1;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ChatServiceCocos2dx::disableChatInputView();
#endif
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto viewSize = view->getContentSize();
    
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    if (needLayout) {
        view->setAnchorPoint(CCPoint(0.5, 0.5));
        view->setPosition(winSize.width / 2, winSize.height / 2);
    }
    view->setUseAnimation(useAnimation);

//    if (viewSize.width > 300 && viewSize.height > 600) {
//        if (!dynamic_cast<LoginFirstStartView*>(view) && !dynamic_cast<LoginFirstView*>(view)) {
//            view->setScale(MIN(1.00, winSize.height/852));
//        }
//    }
    m_stack.insert(pair<int, PopupBaseView*> (++_view_count,view));
    view->_id = _view_count;
    view->setInFlag(false);
    //useAnimation = false;
    m_currentId = _view_count;
    view->retain();
    popupLayer->addChild(view);
    if(!useAnimation){
        view->addToLayer();
    }else{
        view->addToLayer(m_coverSprite->m_clickPos.x, m_coverSprite->m_clickPos.y);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_IN);
    if (GlobalData::shared()->isInitFinish) {
        UIComponent::getInstance()->QuestPrcTimes = 0;
    }
    return _view_count;
}
int PopupViewController::addPopupInViewWithAnim(PopupBaseView *view, bool needLayout) {
    if (!view) {
        return -1;
    }
    if(m_isPlayingInAnim){
        return -1;
    }
    m_isPlayingInAnim = true;
    
    auto world = WorldMapView::instance();
    if (world && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) {
        world->updateGUI(true);
    }
    auto ArcPop = dynamic_cast<ArcPopupBaseView*>(view);
    MailSystemListPopUp* mailSystemListPopUp = nullptr;
    if (ArcPop) {
        UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    } else {
        auto mailwrite = dynamic_cast<MailPopUpView*>(view);
        if(mailwrite){
            UIComponent::getInstance()->showPopupView(UIPopupViewType_Mail);
        }else{
            mailSystemListPopUp = dynamic_cast<MailSystemListPopUp*>(view);
            UIComponent::getInstance()->showPopupView();
        }
    }
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
    
    if (needLayout) {
        view->setAnchorPoint(CCPoint(0, 1));
        view->setPosition(0, winSize.height);
        view->setModelLayerPosition(ccp(0, 852- winSize.height));
        if (CCCommonUtils::isIosAndroidPad())
        {
            view->setModelLayerPosition(ccp(0, 2048 - winSize.height));
        }
        if(ArcPop) {
            view->setModelLayerPosition(ccp(0, 0));
        }
    }
    view->setModelLayerTouchPriority(Touch_Popup);
    view->setInFlag(true);
    m_stack.insert(pair<int, PopupBaseView*> (++_view_count,view));
    view->_id = _view_count;
    
    addGoBackViewWithAnim(true);
    
    m_currentId = _view_count;
    view->retain();
    popupLayer->addChild(view);
    view->setUseAnimation(false);
    view->setOpenAnimation(true);
    if (mailSystemListPopUp)
    {
        mailSystemListPopUp->setPositionX(0);
    }
    else
    {
        view->addToLayer();
    }
    
    return _view_count;
}
void PopupViewController::addGoBackViewWithAnim(bool isCloseAnim)
{
    if (m_currentId == -1) {
        m_isPlayingInAnim = false;
        return ;
    }

    map<int,PopupBaseView*>::iterator it = m_stack.find(m_currentId);
    if (m_stack.end()!= it && it->second->getInFlag()) {
        m_currentInView = it->second;
        if(isCloseAnim){
            float x = m_currentInView->getPositionX()-100;
            float y = m_currentInView->getPositionY();
            //this->setPositionX(this->getPositionX()+this->getContentSize().width);
            CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
            m_currentInView->runAction(CCSequence::create(
                                               CCEaseSineOut::create(userMove)
                                               , CCCallFunc::create(this, callfunc_selector(PopupViewController::whenPalyPopInAnim))
                                               , NULL
                                               ));
        }

    }

}

void PopupViewController::addGoBackViewFromAndroidWithAnim(bool isCloseAnim)
{
    if (m_currentId == -1) {
        return ;
    }
    
    map<int,PopupBaseView*>::iterator it = m_stack.find(m_currentId);
    if (m_stack.end()!= it && it->second->getInFlag()) {
        m_currentInView = it->second;
        if(isCloseAnim){
            float x = m_currentInView->getPositionX()-100;
            float y = m_currentInView->getPositionY();
            //this->setPositionX(this->getPositionX()+this->getContentSize().width);
            CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
            m_currentInView->runAction(CCEaseSineOut::create(userMove));
        }
    }
}


//动画播放完毕，更新站内信息
void PopupViewController::whenPalyPopInAnim(){

    map<int,PopupBaseView*>::iterator it = m_stack.find(m_currentInView->_id);
    if (m_stack.end()!= it && m_currentInView->getInFlag()) {
        it->second->retain();
        it->second->getParent()->removeChild(it->second,false);
        int _currentId = m_currentInView->_id;
        m_stack.erase(it);

        m_gobackStack.insert(pair<int, PopupBaseView*> (std::numeric_limits<int>::max() - _currentId,m_currentInView));
    }
    
    m_isPlayingInAnim = false;
}

int PopupViewController::addPopupInView(PopupBaseView *view, bool needLayout, bool useAnimation, bool openAnimation, bool isHD) {
    if (!view) {
        return -1;
    }
    
    UIComponent::getInstance()->resetOriginPos();
    
    addGoBackStack(openAnimation);
    auto world = WorldMapView::instance();
    if (world && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) {
        world->updateGUI(true);
    }
    auto ArcPop = dynamic_cast<ArcPopupBaseView*>(view);
    if (ArcPop) {
        UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    } else {
        UIPopupViewType type = UIPopupViewType_NONE;
        auto typeview = dynamic_cast<MailPopUpView*>(view);
        if(typeview){
            type = UIPopupViewType_Mail;
        }
        auto storeView = dynamic_cast<StoreView*>(view);
        if(storeView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useCDToolView = dynamic_cast<UseCDToolView*>(view);
        if(useCDToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useToolView = dynamic_cast<UseToolView*>(view);
        if(useToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useResToolView = dynamic_cast<UseResToolView*>(view);
        if(useResToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto minimap = dynamic_cast<MinimapView*>(view);
        if(minimap){
            type = UIPopupViewType_Minimap_ServerList;
        }
        auto server = dynamic_cast<ServerListPopUpView*>(view);
        if(server){
            type = UIPopupViewType_Minimap_ServerList;
        }
        auto sacrificePopUpView = dynamic_cast<SacrificePopUpView*>(view);
        if(sacrificePopUpView){
            type = UIPopupViewType_Sacrifice;
        }
//        auto christmasActView = dynamic_cast<ChristmasAndNewYearView*>(view);
//        if (christmasActView) {
//            type = 4;
//        }
        auto merchantView = dynamic_cast<MerchantView*>(view);
        if(merchantView){
            type = UIPopupViewType_Merchant;
        }
        auto repayView = dynamic_cast<RepayView*>(view);
        if(repayView){
            type = UIPopupViewType_Repay_Lottery;//0;
        }
        auto lotteryView = dynamic_cast<LotteryActView*>(view);
        if (lotteryView)
        {
            type = UIPopupViewType_Repay_Lottery;
        }
        auto lotteryView2 = dynamic_cast<LotteryAct2View*>(view);
        if (lotteryView2)
        {
            type = UIPopupViewType_Repay_Lottery;
        }
//        auto longJingView = dynamic_cast<LongJingStoreView*>(view);
//        if (longJingView)
//        {
//            type = 10;
//        }
        auto titanView = dynamic_cast<GeneralTitanPopupView*>(view); //fusheng 添加titanView类型
        if (titanView)
        {
            type = UIPopupViewType_GeneralTitan;
        }
        auto titanUpgradeView = dynamic_cast<TitanUpgradeView*>(view); //fusheng
        if (titanUpgradeView)
        {
            type = UIPopupViewType_ArcPop_TitanUpgrade;
        }
        
        UIComponent::getInstance()->showPopupView(type);
    }
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);

    if (needLayout) {
        view->setAnchorPoint(CCPoint(0, 1));
        view->setPosition(0, winSize.height);
        view->setModelLayerPosition(ccp(0, 852- winSize.height));
        if (CCCommonUtils::isIosAndroidPad())
        {
            view->setModelLayerPosition(ccp(0, 2048 - winSize.height));
        }
        if(ArcPop) {
            view->setModelLayerPosition(ccp(0, 0));
        }
    }
    view->setModelLayerTouchPriority(Touch_Popup);
    view->setInFlag(true);
    m_stack.insert(pair<int, PopupBaseView*> (++_view_count,view));
    view->_id = _view_count;
    m_currentId = _view_count;
    view->retain();
    popupLayer->addChild(view);
    view->setUseAnimation(false);
    view->setOpenAnimation(false);

    useAnimation = false;

    if(!useAnimation){
        view->addToLayer();
    }else{
        view->addToLayer(m_coverSprite->m_clickPos.x, m_coverSprite->m_clickPos.y);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_IN);
    if (GlobalData::shared()->isInitFinish) {
        UIComponent::getInstance()->QuestPrcTimes = 0;
    }
    return _view_count;
}

void PopupViewController::addGoBackStack(bool isCloseAnim)
{
    if (m_currentId == -1) {
        return ;
    }
    CCLOG("Max int: %d,m_currentID: %d",std::numeric_limits<int>::max(),m_currentId);
    map<int,PopupBaseView*>::iterator it = m_stack.find(m_currentId);
    if (m_stack.end()!= it && it->second->getInFlag()) {
        (it->second)->setUseAnimation(false);
        (it->second)->retain();

        it->second->getParent()->removeChild(it->second,false);

        int _currentId = m_currentId;
        auto _addStackView = it->second;
        m_stack.erase(it);
        if (!m_stack.empty()) {
            auto it = m_stack.end();
            m_currentId = (--it)->first;
        } else {
            m_currentId = -1;
        }
        m_gobackStack.insert(pair<int, PopupBaseView*> (std::numeric_limits<int>::max() - _currentId,_addStackView));
    }
    else
    {
        int _currId = -1;
        for(map<int, PopupBaseView*>::reverse_iterator it = m_stack.rbegin();it!=m_stack.rend();++it)
        {
            if (it->second->getInFlag()) {
                _currId = it->first;
                break;
            }
        }
        if (_currId != -1) {
            map<int,PopupBaseView*>::iterator it = m_stack.find(_currId);
            (it->second)->setUseAnimation(false);
            (it->second)->retain();
            it->second->getParent()->removeChild(it->second,false);
            auto _addStackView = it->second;
            m_stack.erase(it);
            if (!m_stack.empty()) {
                auto it = m_stack.end();
                m_currentId = (--it)->first;
            } else {
                m_currentId = -1;
            }

            m_gobackStack.insert(pair<int, PopupBaseView*> (std::numeric_limits<int>::max() - _currId,_addStackView));
        }
    }
}


int PopupViewController::goBackPopupViewWithAnim(PopupBaseView *removeView,PopupBaseView *gobackView){
    if (!gobackView) {
        return -1;
    }
    m_isPlayingInAnim = true;
    auto ArcPop = dynamic_cast<ArcPopupBaseView*>(gobackView);
    MailPopUpView* mailwrite = nullptr;
    MailSystemListPopUp* mailSys = nullptr;
    if (ArcPop) {
        UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    } else {
        mailwrite = dynamic_cast<MailPopUpView*>(gobackView);
        if(mailwrite){
            UIComponent::getInstance()->showPopupView(UIPopupViewType_Mail);
        }else{
            mailSys = dynamic_cast<MailSystemListPopUp*>(gobackView);
            UIComponent::getInstance()->showPopupView();
        }
    }
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
   // m_stack.insert(pair<int, PopupBaseView*> (++_view_count,gobackView));
   // gobackView->_id = _view_count;
   // m_currentId = _view_count;
    gobackView->setUseAnimation(false);
    popupLayer->addChild(gobackView);
    //gobackView->release();
    
    if (mailwrite || mailSys)
    {
        gobackView->setPositionX(0);
    }
    else
    {
        gobackView->setReturnPlayAnim();
    }
    
    removeView->setZOrder(1);
    float x = removeView->getPositionX()+removeView->getContentSize().width;
    float y = removeView->getPositionY();
    //  this->setPositionX(this->getPositionX()+this->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.2f, ccp(x,y));
    removeView->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(PopupViewController::whenPalyBackInAnim))
                                       , NULL
                                       ));
    
    
    
    return _view_count;
}

void PopupViewController::whenPalyBackInAnim(){
    map<int,PopupBaseView*>::iterator it = m_gobackStack.begin();
    auto tmpView = getPopupView(m_currentId);
    PopupBaseView* gobackView = it->second;
   // removePopupView(tmpView);
    int viewID = tmpView->_id;
    map<int, PopupBaseView*>::iterator itRemove;
    if (m_stack.end() != (itRemove = m_stack.find(viewID))) {
        m_stack.erase(itRemove);
        if (tmpView->getParent()) {
            tmpView->getParent()->removeChild(tmpView);
        }
        tmpView->removeFromLayer(false);
        CC_SAFE_RELEASE_NULL(tmpView);
        if (!m_stack.empty()) {
            auto it = m_stack.end();
            m_currentId = (--it)->first;
        } else {
            m_currentId = -1;
        }
    }
    viewID = std::numeric_limits<int>::max() - viewID;
    map<int, PopupBaseView*>::iterator itGB;
    if (m_gobackStack.end() != (itGB = m_gobackStack.find(viewID))) {
        m_gobackStack.erase(itGB);
    }
    m_gobackStack.erase(it);
    
    m_stack.insert(pair<int, PopupBaseView*> (++_view_count,gobackView));
    gobackView->_id = _view_count;
    CC_SAFE_RELEASE_NULL(gobackView);
    m_currentId = _view_count;
    m_isPlayingInAnim = false;

}
int PopupViewController::goBackPopupView(PopupBaseView *removeView /*=NULL*/)
{
    CCLOGFUNCF("");
    if(m_isPlayingInAnim){//等待进场动画播放完毕，才响应回退
        return -1;
    }
    CCLOGFUNCF("1 currentId %d",m_currentId);
    if (!m_gobackStack.empty()) {
         CCLOGFUNCF("2");
//        bool isPlay = false;
        map<int,PopupBaseView*>::iterator it = m_gobackStack.begin();
//        if (!removeView) {
//            removeLastPopupView();
//        }
//        else
//        {
//            PopupBaseView* view = NULL;
//            do{
//                view = getPopupView(m_currentId);
//                removePopupView(view);
//            }while (view != removeView && m_stack.size() > 0);
//        }
        PopupBaseView* gobackView = it->second;
        if (m_currentId != -1) {
            auto tmpView = getPopupView(m_currentId);
            if (tmpView) {
                if(tmpView->m_bopenAnim){
                    goBackPopupViewWithAnim(tmpView,gobackView);
                    return -1;
                }
                if ( gobackView && tmpView->_id == gobackView->_id) {
                    CCLOG("error error!");
                    return -1;
                }

                removePopupView(tmpView);
            }
        }

        m_gobackStack.erase(it);
        if (!gobackView) {
            return -1;
        }
        
        if(CCCommonUtils::isIosAndroidPad() && gobackView)
        {
            CCCommonUtils::setIsHDViewPort(gobackView->m_isHDPanel);
        }
        
        auto ArcPop = dynamic_cast<ArcPopupBaseView*>(gobackView);
        if (ArcPop) {
            UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
        } else {
            auto mailwrite = dynamic_cast<MailPopUpView*>(gobackView);
            auto sacrificePopUpView = dynamic_cast<SacrificePopUpView*>(gobackView);
            auto merchantView = dynamic_cast<MerchantView*>(gobackView);
            auto serverListPopupView = dynamic_cast<ServerListPopUpView*>(gobackView);
            auto TitanPopupView = dynamic_cast<GeneralTitanPopupView*>(gobackView);
            auto titanUpgradeView = dynamic_cast<TitanUpgradeView*>(gobackView); //fusheng

//            JoinAllianceView
            if(mailwrite){
                UIComponent::getInstance()->showPopupView(UIPopupViewType_Mail);
            }else if(sacrificePopUpView){
                UIComponent::getInstance()->showPopupView(UIPopupViewType_Sacrifice);
            }else if(merchantView){
                UIComponent::getInstance()->showPopupView(UIPopupViewType_Merchant);
            }else if (serverListPopupView){
                UIComponent::getInstance()->showPopupView(UIPopupViewType_Minimap_ServerList);
            }else if (TitanPopupView){ //fusheng 泰坦type 11
                UIComponent::getInstance()->showPopupView(UIPopupViewType_GeneralTitan);
            }else if (titanUpgradeView){ //fusheng 泰坦升级界面 1
                UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
            }else{
                
                UIComponent::getInstance()->showPopupView();
            }
        }
        
        auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
        m_stack.insert(pair<int, PopupBaseView*> (++_view_count,gobackView));
        gobackView->_id = _view_count;
        m_currentId = _view_count;
        gobackView->setUseAnimation(false);
        popupLayer->addChild(gobackView);
//        if(isPlay){
//            gobackView->setReturnPlayAnim();
//        }
        CC_SAFE_RELEASE_NULL(gobackView);
        return m_currentId;
    }
    else
    {
        CCLOGFUNCF("3");
        PopupBaseView* popUp = getPopupView(m_currentId);
        auto longJingStore = dynamic_cast<LongJingStoreView*>(popUp);
        if(longJingStore){
            UIComponent::getInstance()->showPopupView(UIPopupViewType_NONE);
        }
        
        if(popUp && popUp->getIsSystemMail() && MailController::getInstance()->getIsNewMailListEnable())
            removeLastSystemMailPopupView();
        else
            removeLastPopupView();
        
//        popUp = getPopupView(m_currentId);
//        if(CCCommonUtils::isIosAndroidPad() && popUp &&  popUp->m_isHDPanel)
//        {
//            CCCommonUtils::setIsHDViewPort(popUp->m_isHDPanel);
//        }
    }
    return -1;
}


void PopupViewController::removePopupView(PopupBaseView *view,bool clare,bool isNewMailListPopup) {
    if (!view) {
        return;
    }
    if(m_isPlayingInAnim){
        return;
    }
    int viewID = view->_id;
    map<int, PopupBaseView*>::iterator it;
    if (m_stack.end() != (it = m_stack.find(viewID))) {
        m_stack.erase(it);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(isNewMailListPopup)
            JNIScheduleObject::getInstance()->goBackToNewMailList();
#endif
        view->removeFromLayer(false,isNewMailListPopup);
        CC_SAFE_RELEASE_NULL(view);
        if (!m_stack.empty()) {
            auto it = m_stack.end();
            m_currentId = (--it)->first;
            PopupBaseView* __popUp = getPopupView(m_currentId);
            if(CCCommonUtils::isIosAndroidPad() && __popUp &&  __popUp->m_isHDPanel)
            {
                CCCommonUtils::setIsHDViewPort(__popUp->m_isHDPanel);
            }
        } else {
            if(CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
            {
                CCCommonUtils::setIsHDViewPort(true);
            }
            m_currentId = -1;
            if(clare) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(!isNewMailListPopup)
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
#else
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
#endif
            }
        }
    }
    viewID = std::numeric_limits<int>::max() - viewID;
    map<int, PopupBaseView*>::iterator itGB;
    if (m_gobackStack.end() != (itGB = m_gobackStack.find(viewID))) {
        m_gobackStack.erase(itGB);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(!isNewMailListPopup)
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
#else
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
#endif
    
    
    if(m_gobackStack.empty() && m_stack.empty()) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_REMOVE_TMPBUILD);
        showPushPop();
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PopupViewController::doResourceClear), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PopupViewController::doResourceClear), this, 0.1, 1, 0.0f, false);
}

void PopupViewController::doResourceClear(float time){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PopupViewController::doResourceClear), this);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//void PopupViewController::removePopupView(int _id) {
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(POPUP_VIEW_CLOSE);
//    map<int, PopupBaseView*>::iterator it;
//    if (m_stack.end() != (it = m_stack.find(_id))) {
//        m_stack.erase(it);
//        if (m_stack.empty()) {
//            m_currentId = -1;
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
//        }
//        it->second->removeFromLayer();
//        it->second->release();
//        if (!m_stack.empty()) {
//            auto it = m_stack.end();
//            m_currentId = (--it)->first;
//        }
//    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
//}

void PopupViewController::removeLastPopupView() {
    if (m_currentId == -1) {
        return;
    }
    removePopupView(getPopupView(m_currentId));
}

void PopupViewController::removeLastSystemMailPopupView() {
    if (m_currentId == -1) {
        return;
    }
    removePopupView(getPopupView(m_currentId),true,true);
}

void PopupViewController::removeAllPopupView() {
    if (m_currentId == -1) {
        return;
    }
    m_currentId = -1;
    for(map<int, PopupBaseView*>::iterator it = m_stack.begin();it!=m_stack.end();++it)
    {
        it->second->removeFromLayer(false);
        CC_SAFE_RELEASE_NULL(it->second);
    }
    m_stack.clear();
    for(map<int, PopupBaseView*>::iterator it = m_gobackStack.begin();it!=m_gobackStack.end();++it)
    {
        it->second->removeFromLayer(false);
        CC_SAFE_RELEASE_NULL(it->second);
        CC_SAFE_RELEASE_NULL(it->second);
    }

    m_gobackStack.clear();
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setIsHDViewPort(true);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_REMOVE_TMPBUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
    showPushPop();
}

int PopupViewController::getGoBackViewCount()
{
    return m_gobackStack.size();
}

int PopupViewController::getCurrViewCount()
{
    return m_stack.size();
}

PopupBaseView* PopupViewController::getCurrentPopupView(){
    return getPopupView(m_currentId);
}
PopupBaseView* PopupViewController::getPopupView(int _id) {
    if (m_currentId == -1) {
        return NULL;
    }
    map<int, PopupBaseView*>::iterator it;
    if (m_stack.end() != (it = m_stack.find(_id))) {
        return it->second;
    }
    return NULL;
}

bool PopupViewController::pushPop(PopupBaseView *view, bool force)
{
    if (!view) {
        return false;
    }
    if(m_stack.size()>0 || !CanPopPushView || force) {
        view->retain();
        m_pushPopStack.push_back(view);
        return false;
    }
    else {
        return true;
    }
}

void PopupViewController::showPushPop()
{
    if(!CanPopPushView) {
        return;
    }
    
    if (m_pushPopStack.size()>0) {
        bool st = true;
        vector<PopupBaseView*>::iterator it = m_pushPopStack.begin();
        for (; it!=m_pushPopStack.end(); it++) {
            UserUpgradeView* upView = dynamic_cast<UserUpgradeView*>(*it);
            if (upView) {
                addPopupView(upView);
                CC_SAFE_RELEASE_NULL(upView);
                m_pushPopStack.erase(it);
                st = false;
                break;
            }
        }
        if (st) {
            addPopupView(m_pushPopStack[0]);
            CC_SAFE_RELEASE_NULL(m_pushPopStack[0]);
            m_pushPopStack.erase(m_pushPopStack.begin());
        }
    }
}

int PopupViewController::addPopupFromAndroidInView(PopupBaseView *view, bool needLayout) {
    if (!view) {
        return -1;
    }
    addGoBackStack(true);
    auto world = WorldMapView::instance();
    if (world && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) {
        world->updateGUI(true);
    }
    auto ArcPop = dynamic_cast<ArcPopupBaseView*>(view);
    if (ArcPop) {
        UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    } else {
        UIPopupViewType type = UIPopupViewType_NONE;
        auto typeview = dynamic_cast<MailPopUpView*>(view);
        if(typeview){
            type = UIPopupViewType_Mail;
        }
        auto storeView = dynamic_cast<StoreView*>(view);
        if(storeView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useCDToolView = dynamic_cast<UseCDToolView*>(view);
        if(useCDToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useToolView = dynamic_cast<UseToolView*>(view);
        if(useToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto useResToolView = dynamic_cast<UseResToolView*>(view);
        if(useResToolView){
            type = UIPopupViewType_Tool_Store;
        }
        auto minimap = dynamic_cast<MinimapView*>(view);
        if(minimap){
            type = UIPopupViewType_Minimap_ServerList;
        }
        auto server = dynamic_cast<ServerListPopUpView*>(view);
        if(server){
            type = UIPopupViewType_Minimap_ServerList;
        }
        auto sacrificePopUpView = dynamic_cast<SacrificePopUpView*>(view);
        if(sacrificePopUpView){
            type = UIPopupViewType_Sacrifice;
        }
        //        auto christmasActView = dynamic_cast<ChristmasAndNewYearView*>(view);
        //        if (christmasActView) {
        //            type = 4;
        //        }
        auto merchantView = dynamic_cast<MerchantView*>(view);
        if(merchantView){
            type = UIPopupViewType_Merchant;
        }
        auto repayView = dynamic_cast<RepayView*>(view);
        if(repayView){
            type = UIPopupViewType_Repay_Lottery;//0;
        }
        auto lotteryView = dynamic_cast<LotteryActView*>(view);
        if (lotteryView)
        {
            type = UIPopupViewType_Repay_Lottery;
        }
        auto lotteryView2 = dynamic_cast<LotteryAct2View*>(view);
        if (lotteryView2)
        {
            type = UIPopupViewType_Repay_Lottery;
        }
        
        UIComponent::getInstance()->showPopupView(type, false);
    }
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
    
    if (needLayout) {
        view->setAnchorPoint(CCPoint(0, 1));
        view->setPosition(0, winSize.height);
        view->setModelLayerPosition(ccp(0, 852- winSize.height));
        if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
        {
            view->setModelLayerPosition(ccp(0, DEF_HEIGHT_HD- winSize.height));
        }
        if(ArcPop) {
            view->setModelLayerPosition(ccp(0, 0));
        }
    }
    view->setModelLayerTouchPriority(Touch_Popup);
    view->setInFlag(true);
    m_stack.insert(pair<int, PopupBaseView*> (++_view_count,view));
    view->_id = _view_count;
    
    m_currentId = _view_count;
    view->retain();
    popupLayer->addChild(view);
    view->setUseAnimation(false);
    view->setOpenAnimation(false);
    view->addToLayer();
    return _view_count;
}

CCPoint PopupViewController::getTouchPoint(){
    return m_coverSprite->m_clickPos;
}

void PopupViewController::setTouchPos(CCPoint& p){
    m_coverSprite->m_clickPos = p;
}

void PopupViewController::addPopFlyHint(FlyHint* flyhint)
{
    if (m_flyHintStack.size()>0) {
        for (int i=m_flyHintStack.size()-1; i>=0; i--) {
            if (m_flyHintStack[i]->m_content == flyhint->m_content) {
                return;
            }
        }
    }
    
    flyhint->retain();
    m_flyHintStack.push_back(flyhint);
    if (m_flyHintStack.size() == 1) {
        flyhint->show();
    }
}

void PopupViewController::removePopFlyHint(FlyHint* flyhint)
{
    if (m_flyHintStack.size() > 0) {
        CC_SAFE_RELEASE_NULL(m_flyHintStack[0]);
        m_flyHintStack.erase(m_flyHintStack.begin());
    }
    
    if (m_flyHintStack.size() > 0) {
        m_flyHintStack[0]->show();
    }
}

//----
bool TouchSprite::init(){
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    this->setAnchorPoint(ccp(0, 0));
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POP_TOUCH);
    layer->addChild(this);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(TouchSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchSprite::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchSprite::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -2);
    return true;
}

void TouchSprite::onEnter(){
    CCNode::onEnter();
    
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

void TouchSprite::onExit(){
    CCNode::onExit();
}

bool TouchSprite::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TouchSprite::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void TouchSprite::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_clickPos = pTouch->getLocation();
}
