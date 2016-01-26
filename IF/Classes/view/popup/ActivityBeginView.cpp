//
//  ActivityBeginView.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "ActivityBeginView.h"
#include "ActivityHistoryView.h"
#include "ActivityRewardView.h"
#include "GetActivityDetailCommand.h"
#include "CCLoadSprite.h"
#include "RewardController.h"
#include "ActivityRankReward.h"
#include "ActivityController.h"
#include "ActivityTipView.h"
#include "TipsView.h"
#include "UIComponent.h"
#include "ActivityEventCommand.h"
#include "GameController.h"

static const cocos2d::ccColor3B COLOR_TYPE_NORMAL = {0x17, 0x48, 0x1B};
static const cocos2d::ccColor3B COLOR_TYPE_SELECTED = {255, 255, 255};

#define SCROLL_W (m_funList->getContentSize().width)
#define CELL_X (m_funList->getContentSize().width / 2)
#define TARGET_NODE_Y (totalH - 205)
#define EARN_NODE_Y (totalH-390-100)
#define RANK_NODE_Y (totalH-630-400+50)

ActivityBeginView *ActivityBeginView::create(){
    ActivityBeginView *ret = new ActivityBeginView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityBeginView::onEnter(){
    PopupBaseView::onEnter();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    this->unschedule(schedule_selector(ActivityBeginView::updateTime));
    this->schedule(schedule_selector(ActivityBeginView::updateTime), 1);
    this->updateTime(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityBeginView::updateEventData), MSG_activity_EVENT_CHANGE, NULL);
}

void ActivityBeginView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_activity_EVENT_CHANGE);
    PopupBaseView::onExit();
}

bool ActivityBeginView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    m_touchPos = pTouch->getLocation();
    m_moveFlag = false;
    if(isTouchInside(m_clickNode, pTouch)){
        m_moveFlag = true;
    }
    if(isTouchInside(m_btnHistory, pTouch)){
        m_btnHistory->setHighlighted(true);
    }else if (isTouchInside(m_btnReward, pTouch)){
        m_btnReward->setHighlighted(true);
    }else if (isTouchInside(m_btnRecord, pTouch)){
        m_btnRecord->setHighlighted(true);
    }
    else if (isTouchInside(m_btnRankReward, pTouch)){
        m_btnRankReward->setHighlighted(true);
    }
    return true;
}

void ActivityBeginView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_btnHistory->setHighlighted(false);
    m_btnReward->setHighlighted(false);
    m_btnRecord->setHighlighted(false);
    m_btnRankReward->setHighlighted(false);
    
    CCPoint pos = pTouch->getLocation();
    if(fabsf(pos.y - m_touchPos.y)>30){
        return ;
    }
    if(isTouchInside(m_btnHistory, pTouch)){
        onClickHistory(NULL,Control::EventType::TOUCH_DOWN);
    }else if (isTouchInside(m_btnReward, pTouch) && m_btnReward->isVisible()){
        onClickReward(NULL,Control::EventType::TOUCH_DOWN);
    }else if (isTouchInside(m_btnRecord, pTouch)){
        onClickRecord(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (isTouchInside(m_btnRankReward, pTouch)){
        onClickRankReward(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (isTouchInside(m_infoBtn, pTouch)){
        onInfoBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (isTouchInside(m_infoBtn2, pTouch)){
        onInfoBtn2Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (isTouchInside(m_infoBtn3, pTouch) && m_infoBtn3->isVisible()){
        onInfoBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }else if (isTouchInside(m_sorceTargetBtn, pTouch)){
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("105844"),kCCTextAlignmentLeft));
    }
}

void ActivityBeginView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_btnHistory, pTouch)){
        m_btnHistory->setHighlighted(true);
    }else if (isTouchInside(m_btnReward, pTouch)){
        m_btnReward->setHighlighted(true);
    }else if (isTouchInside(m_btnRecord, pTouch)){
        m_btnRecord->setHighlighted(true);
    }
    else if (isTouchInside(m_btnRankReward, pTouch)){
        m_btnRankReward->setHighlighted(true);
    }
    CCPoint pos = m_scrollView->getContentOffset();
    //CCLOG("x=%f  y=%f",pos.x,pos.y);
}

void ActivityBeginView::onTab1(CCObject *pSender)
{
    m_btnHistory->setHighlighted(false);
    m_btnReward->setHighlighted(false);
    m_btnRecord->setHighlighted(false);
    m_btnRankReward->setHighlighted(false);
    
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->setContentSize({SCROLL_W, (float)m_totalH});
    m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - m_totalH));
    m_sorceTxt->setColor(COLOR_TYPE_SELECTED);
    m_sorceNum->setColor(COLOR_TYPE_SELECTED);
    m_rankTxt->setColor(COLOR_TYPE_NORMAL);
    m_rankNum->setColor(COLOR_TYPE_NORMAL);
    m_totalRankTxt->setColor(COLOR_TYPE_NORMAL);
    m_totalRankNum->setColor(COLOR_TYPE_NORMAL);
}

void ActivityBeginView::onTab2(CCObject *pSender)
{
    m_btnHistory->setHighlighted(false);
    m_btnReward->setHighlighted(false);
    m_btnRecord->setHighlighted(false);
    m_btnRankReward->setHighlighted(false);
    
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->setContentOffset(ccp(0,m_funList->getContentSize().height -m_tab2H));
    m_sorceTxt->setColor(COLOR_TYPE_NORMAL);
    m_sorceNum->setColor(COLOR_TYPE_NORMAL);
    m_rankTxt->setColor(COLOR_TYPE_SELECTED);
    m_rankNum->setColor(COLOR_TYPE_SELECTED);
    m_totalRankTxt->setColor(COLOR_TYPE_NORMAL);
    m_totalRankNum->setColor(COLOR_TYPE_NORMAL);
}

void ActivityBeginView::onTab3(CCObject *pSender)
{
    m_btnHistory->setHighlighted(false);
    m_btnReward->setHighlighted(false);
    m_btnRecord->setHighlighted(false);
    m_btnRankReward->setHighlighted(false);
    
    m_sorceTxt->setColor(COLOR_TYPE_NORMAL);
    m_sorceNum->setColor(COLOR_TYPE_NORMAL);
    m_rankTxt->setColor(COLOR_TYPE_NORMAL);
    m_rankNum->setColor(COLOR_TYPE_NORMAL);
    m_totalRankTxt->setColor(COLOR_TYPE_SELECTED);
    m_totalRankNum->setColor(COLOR_TYPE_SELECTED);
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->setContentOffset(ccp(0,m_funList->getContentSize().height - m_tab3H));
}

bool ActivityBeginView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        m_rewards = nullptr;
        
        m_reward1 = nullptr;
        m_reward2 = nullptr;
        m_reward3 = nullptr;
        
//        CCLoadSprite::doResourceByCommonIndex(8, true);
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(8, false);
//        });
        setIsHDPanel(true);
        // this->setModelLayerOpacity(160);
        auto node = CCBLoadFile("ActivityBeginView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(node->getContentSize());
        
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
//        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
//        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        height_offset = add;
        m_funList->setContentSize(CCSize(m_funList->getContentSize().width, m_funList->getContentSize().height + add));
        
        m_scrollView = CCScrollView::create({m_funList->getContentSize().width, m_funList->getContentSize().height});
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Default);
        m_funList->addChild(m_scrollView);
        
        m_trTxt1->setString(_lang("105817"));
        m_trTxt2->setString(_lang("105818"));
        m_trTxt3->setString(_lang("105819"));
        m_rankTitle1->setString(_lang("105821"));
        m_rankTitle2->setString(_lang("105822"));
        m_earnTxt->setString(_lang("105820"));
        m_sorceTxt->setString(_lang("105815"));
        m_rankTxt->setString(_lang("105839"));
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_rankTxt->setMaxScaleXByWidth(150);
            m_totalRankTxt->setMaxScaleXByWidth(150);
        }
        
        m_totalRankTxt->setString(_lang("105840"));
        
        m_lordTxt->setString(_lang("105842"));
        m_targetSorceTxt->setString(_lang("105843"));
        
        CCCommonUtils::setButtonTitle(m_btnRankReward, _lang("105823").c_str());
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("105823").c_str());
        CCCommonUtils::setButtonTitle(m_btnRecord, _lang("105824").c_str());
        CCCommonUtils::setButtonTitle(m_btnHistory, _lang("105836").c_str());
        m_btnHistory->setTouchPriority(Touch_Popup_Item);
        m_btnReward->setTouchPriority(Touch_Popup_Item);
        m_btnRecord->setTouchPriority(Touch_Popup_Item);
        m_btnRankReward->setTouchPriority(Touch_Popup_Item);
        m_infoBtn->setTouchPriority(Touch_Popup_Item);
        m_infoBtn2->setTouchPriority(Touch_Popup_Item);
        m_infoBtn3->setTouchPriority(Touch_Popup_Item);
        m_sorceTargetBtn->setTouchPriority(Touch_Popup_Item);
        
        m_btnHistory->setSwallowsTouches(false);
        m_btnReward->setSwallowsTouches(false);
        m_btnRecord->setSwallowsTouches(false);
        m_btnRankReward->setSwallowsTouches(false);
        m_infoBtn->setSwallowsTouches(false);
        m_infoBtn2->setSwallowsTouches(false);
        m_infoBtn3->setSwallowsTouches(false);
        m_sorceTargetBtn->setSwallowsTouches(false);
        
        m_clipperNode->removeAllChildrenWithCleanup(true);
        
        m_proTimer = CCProgressTimer::create(CCLoadSprite::createSprite("Activities_jindutiao01.png"));//icon_jindutiao.png
        m_proTimer->setType(kCCProgressTimerTypeBar);
        m_proTimer->setMidpoint(ccp(1,0));   // 设置进度方向
        m_proTimer->setBarChangeRate(ccp(0,1));  // 设置进度为水平还是垂直方向
        m_proTimer->setPosition(ccp(0, -18));
        m_proTimer->setAnchorPoint(ccp(0, 0));
        m_proTimer->setPercentage(0);
        m_clipperNode->addChild(m_proTimer);
        m_moveFlag = false;
        
        int totalH = 1590.0;
        m_totalH = totalH;
        
        m_targetNode->removeFromParent();
        m_targetNode->setAnchorPoint(ccp(0, 0));
        m_targetNode->setPosition(ccp(CELL_X, TARGET_NODE_Y));//390
        m_scrollView->addChild(m_targetNode);
        
        m_earnNode->removeFromParent();
        m_earnNode->setAnchorPoint(ccp(0, 0));
        m_earnNode->setPosition(ccp(CELL_X, EARN_NODE_Y));//240
        m_scrollView->addChild(m_earnNode);
        
        m_rankNode->removeFromParent();
        m_rankNode->setAnchorPoint(ccp(0, 0));
        m_rankNode->setPosition(ccp(CELL_X, totalH-630-400+50));//590
        m_scrollView->addChild(m_rankNode);
        
        m_scrollView->setContentSize({SCROLL_W, (float)m_totalH});
        m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - totalH));
        m_scrollView->setPosition(0, -height_offset);
        
        m_loadingIcon = NULL;
        ActivityController::getInstance()->activityRecord(2);
        if(ActivityController::getInstance()->eventInfo.getRankReward()->count()<=0){
            ActivityEventCommand* cmd = new ActivityEventCommand();
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::updateEventData), NULL));
            cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::removeLoadingAni), NULL));
            cmd->sendAndRelease();
        }else{
            GetActivityDetailCommand* cmd = new GetActivityDetailCommand();
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::getServerData), NULL));
            cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::removeLoadingAni), NULL));
            cmd->sendAndRelease();
        }
        GameController::getInstance()->showWaitInterface();
        m_sorceTxt->setColor(COLOR_TYPE_SELECTED);
        m_sorceNum->setColor(COLOR_TYPE_SELECTED);
        m_rankTxt->setColor(COLOR_TYPE_NORMAL);
        m_rankNum->setColor(COLOR_TYPE_NORMAL);
        m_totalRankTxt->setColor(COLOR_TYPE_NORMAL);
        m_totalRankNum->setColor(COLOR_TYPE_NORMAL);
        ret = true;
    }
    return ret;
}

void ActivityBeginView::updateEventData(CCObject* param){
    GetActivityDetailCommand* cmd = new GetActivityDetailCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::getServerData), NULL));
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(ActivityBeginView::removeLoadingAni), NULL));
    cmd->sendAndRelease();
}

void ActivityBeginView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_funList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_funList->addChild(m_loadingIcon,1000000);
    }
}


void ActivityBeginView::removeLoadingAni(CCObject * obj){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void ActivityBeginView::getServerData(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    m_earnNode->removeFromParent();
    m_rankNode->removeFromParent();
    m_targetNode->removeFromParent();
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        int score = dic->valueForKey("score")->intValue();
        int current = ActivityController::getInstance()->eventInfo.curPhase;
        if (current > ActivityController::getInstance()->eventInfo.maxPhase) {
            current = ActivityController::getInstance()->eventInfo.maxPhase;
        }
        ActivityController::getInstance()->eventInfo.currentName = _lang(dic->valueForKey("name")->getCString());
        std::string nameStr = _lang_2("105835",CC_ITOA(current),CC_ITOA(ActivityController::getInstance()->eventInfo.maxPhase));
        nameStr += " ";
        nameStr += _lang(dic->valueForKey("name")->getCString());
        m_titleTxt->setString(nameStr);
        
        for (int i = 0; i < ActivityController::getInstance()->activityArr->count(); ++i)
        {
            ActivityEventObj* obj = (ActivityEventObj*)ActivityController::getInstance()->activityArr->objectAtIndex(i);
            if (obj->name == _lang("105842"))
            {
                m_desc->setString(obj->desc);
            }
        }
        
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_titleTxt->getContentSize().width;
            float s = m_titleTxt->getScaleX();
            if (fw>460) {//防爆框
                m_titleTxt->setScaleX(460/fw*s);
            }
        }
        else
            m_titleTxt->setMaxScaleXByWidth(230);
        
        std::string str  = "";
        if(score==0){
            str.append("~");
        }else{
            str.append(CC_CMDITOA(score));
        }
        m_sorceNum->setString(str);
        
        double readyTime = ActivityController::getInstance()->eventInfo.beginTime - GlobalData::shared()->getWorldTime();
        double endTime = ActivityController::getInstance()->eventInfo.endTime - GlobalData::shared()->getWorldTime() - 1;
        
        int rank = dic->valueForKey("rank")->intValue();
        str  = "";
        if (rank>5000 || (rank ==0 && readyTime<=0 && endTime>=0)) {
            str.append("5000+");
        }else{
            if(rank==0){
               str.append("~");
            }else{
                str.append(CC_CMDITOA(rank));
            }
        }
        m_rankNum->setString(str);
        
        int totalRank = dic->valueForKey("totalRank")->intValue();
        str  = "";
        if (totalRank>5000 || (totalRank ==0 && ActivityController::getInstance()->eventInfo.curPhase!=1) || (totalRank ==0 && ActivityController::getInstance()->eventInfo.curPhase==1 && readyTime<=0 && endTime>=0)) {
            str.append("5000+");
        }else{
            if(totalRank==0){
                str.append("~");
            }else{
                str.append(CC_CMDITOA(totalRank));
            }
        }
        if(ActivityController::getInstance()->eventInfo.getTotalRankReward() && ActivityController::getInstance()->eventInfo.getTotalRankReward()->count()<=0){
            str = "~";
        }
        m_totalRankNum->setString(str);
        
        CCArray* array = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
        CC_SAFE_RELEASE_NULL(m_rewards);
        m_rewards = array;
        CC_SAFE_RETAIN(m_rewards);
        std::string icon = "";
        std::string name = "";
        if(array && array->count()==3){
            auto one = _dict(array->objectAtIndex(0));
            int oneScore = 0;
            int twoScore = 0;
            int threeScore = 0;
            if (one) {
                oneScore = one->valueForKey("score")->intValue();
                m_targetTxt1->setString(CC_CMDITOA(oneScore));
                CCArray* rew = dynamic_cast<CCArray*>(one->objectForKey("reward"));
                m_targetIcon1->removeAllChildrenWithCleanup(true);
                if(rew && rew->count()==1){
                     auto oneReward = _dict(rew->objectAtIndex(0));
                    if(oneReward){
                        int type = oneReward->valueForKey("type")->intValue();
                        int value = oneReward->valueForKey("value")->intValue();
                        if(type==R_GOODS){
                            auto valueObj = _dict(oneReward->objectForKey("value"));
                            int id = valueObj->valueForKey("id")->intValue();
                            name = RewardController::getInstance()->getNameByType(type,id);
                            icon = RewardController::getInstance()->getPicByType(type,id);
                            value = valueObj->valueForKey("num")->intValue();
                        }else{
                            name = RewardController::getInstance()->getNameByType(type,value);
                            icon = RewardController::getInstance()->getPicByType(type,value);
                        }
                        if(score>=oneScore){
                            m_trTxt1->setString(_lang("115216"));
                            m_trTxt1->setPositionY(8);
                            //m_targetTxtBg1->setVisible(false);
                        }else{
                            m_tRewardTxt1->setString(name);
                            if (CCCommonUtils::isIosAndroidPad()) {
                                float fw = m_tRewardTxt1->getContentSize().width;
                                float s = m_tRewardTxt1->getScaleX();
                                if (fw>320) {//防爆框
                                    m_tRewardTxt1->setScaleX(320/fw*s);
                                }
                            }
                            else
                                m_tRewardTxt1->setMaxScaleXByWidth(160);
                        }
                        name = "X";
                        name.append(CC_ITOA(value));
                        m_trNum1->setString(name);
                        CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
                        CCCommonUtils::setSpriteMaxSize(spr, 45);
                        m_targetIcon1->addChild(spr);
                    }
                }
            }
            
            one = _dict(array->objectAtIndex(1));
            if (one) {
                twoScore = one->valueForKey("score")->intValue();
                m_targetTxt2->setString(CC_CMDITOA(twoScore));
                CCArray* rew = dynamic_cast<CCArray*>(one->objectForKey("reward"));
                m_targetIcon2->removeAllChildrenWithCleanup(true);
                if(rew && rew->count()==1){
                    auto oneReward = _dict(rew->objectAtIndex(0));
                    if(oneReward){
                        int type = oneReward->valueForKey("type")->intValue();
                        int value = oneReward->valueForKey("value")->intValue();
                        if(type==R_GOODS){
                            auto valueObj = _dict(oneReward->objectForKey("value"));
                            int id = valueObj->valueForKey("id")->intValue();
                            name = RewardController::getInstance()->getNameByType(type,id);
                            icon = RewardController::getInstance()->getPicByType(type,id);
                            value = valueObj->valueForKey("num")->intValue();
                        }else{
                            name = RewardController::getInstance()->getNameByType(type,value);
                            icon = RewardController::getInstance()->getPicByType(type,value);
                        }
                        if(score>=twoScore){
                            m_trTxt2->setString(_lang("115216"));
                            m_trTxt2->setPositionY(8);
                            //m_targetTxtBg2->setVisible(false);
                        }else{
                            m_tRewardTxt2->setString(name);
                            if (CCCommonUtils::isIosAndroidPad()) {
                                float fw = m_tRewardTxt2->getContentSize().width;
                                float s = m_tRewardTxt2->getScaleX();
                                if (fw>320) {//防爆框
                                    m_tRewardTxt2->setScaleX(320/fw*s);
                                }
                            }
                            else
                                m_tRewardTxt2->setMaxScaleXByWidth(160);
                        }
                        name = "X";
                        name.append(CC_ITOA(value));
                        m_trNum2->setString(name);
                        CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
                        CCCommonUtils::setSpriteMaxSize(spr, 45);
                        m_targetIcon2->addChild(spr);
                    }
                }
            }
            
            one = _dict(array->objectAtIndex(2));
            if (one) {
                threeScore = one->valueForKey("score")->intValue();
                m_targetTxt3->setString(CC_CMDITOA(threeScore));
                CCArray* rew = dynamic_cast<CCArray*>(one->objectForKey("reward"));
                m_targetIcon3->removeAllChildrenWithCleanup(true);
                if(rew && rew->count()==1){
                    auto oneReward = _dict(rew->objectAtIndex(0));
                    if(oneReward){
                        int type = oneReward->valueForKey("type")->intValue();
                        int value = oneReward->valueForKey("value")->intValue();
                        if(type==R_GOODS){
                            auto valueObj = _dict(oneReward->objectForKey("value"));
                            int id = valueObj->valueForKey("id")->intValue();
                            name = RewardController::getInstance()->getNameByType(type,id);
                            icon = RewardController::getInstance()->getPicByType(type,id);
                            value = valueObj->valueForKey("num")->intValue();
                        }else{
                            name = RewardController::getInstance()->getNameByType(type,value);
                            icon = RewardController::getInstance()->getPicByType(type,value);
                        }
                        if(score>=threeScore){
                            m_trTxt3->setString(_lang("115216"));
                            m_trTxt3->setPositionY(8);
                            //m_targetTxtBg3->setVisible(false);
                        }else{
                            m_tRewardTxt3->setString(name);
                            if (CCCommonUtils::isIosAndroidPad()) {
                                float fw = m_tRewardTxt3->getContentSize().width;
                                float s = m_tRewardTxt3->getScaleX();
                                if (fw>320) {//防爆框
                                    m_tRewardTxt3->setScaleX(320/fw*s);
                                }
                            }
                            else
                                m_tRewardTxt3->setMaxScaleXByWidth(160);

                        }
                        name = "X";
                        name.append(CC_ITOA(value));
                        m_trNum3->setString(name);
                        CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
                        CCCommonUtils::setSpriteMaxSize(spr, 45);
                        m_targetIcon3->addChild(spr);
                    }
                }
                float len = 0;
                if(score<=oneScore){
                    len = score*1.0/oneScore;
                    len = len*0.3;
                }else if(score<=twoScore){
                    len = (score-oneScore)*1.0/(twoScore-oneScore);
                    len = len*0.33 +0.33;
                }else{
                    len = (score-twoScore)*1.0/(threeScore-twoScore);
                    len = len*0.33 +0.66;
                }
                len = MIN(1,len);
                len = MAX(0,len);
                int per = len*100;
                m_proTimer->setPercentage(per);
            }
            
            float w = threeScore > 0 ? 532.0 * score / threeScore : 0;
            w = w > 0 ? w : 1;
            w = w > 532 ? 532 : w;
            m_progress1->setContentSize({w, 11});
        }
        
        m_eventIds.clear();
        std::string event = dic->valueForKey("event")->getCString();
        CCCommonUtils::splitString(event,"|", m_eventIds);
        int num = m_eventIds.size();
        m_eNode->removeAllChildrenWithCleanup(true);
        for (int i=0; i<num; i++) {
            ActivityEarnPointCell* cell = ActivityEarnPointCell::create(m_eventIds[i]);
            cell->setPosition(ccp(-590/2, -i*55 - 20));
            m_eNode->addChild(cell);
        }
        
        int hBG = 20+num*55 + 65;
        //m_earnBg->setContentSize(CCSize(540,num*50+20+65));
        showRewards(hBG);
    }
}


void ActivityBeginView::showRewards(int hBG){
    m_rankReward = ActivityController::getInstance()->eventInfo.getRankReward();
    CCArray* rankArr = CCArray::create();
    int offY = 0;
    std::string tempStr = _lang_1("105828","1");
    m_rewardListNode->removeAllChildrenWithCleanup(true);
    if(m_rankReward && m_rankReward->count()>0){
        auto oneDic = _dict(m_rankReward->objectAtIndex(0));
        rankArr = dynamic_cast<CCArray*>(oneDic->objectForKey("reward"));
        ActivityRankReward* rankOne = ActivityRankReward::create(rankArr,tempStr,false,-1,ARR_CELL_1);
        rankOne->setPositionX(-92/2);
        m_rewardListNode->addChild(rankOne);
        offY = -rankOne->getTotalH();
    }
    offY -= 50;
    m_btnRankReward->setPositionY(offY);
    offY -= 90;
    m_titleNode3->setPositionY(offY);
    offY -= 40;
    int rankTwoY = -offY;
    m_totalRankReward = ActivityController::getInstance()->eventInfo.getTotalRankReward();
    if(m_totalRankReward && m_totalRankReward->count()>0){
        auto oneDic = _dict(m_totalRankReward->objectAtIndex(0));
        rankArr = dynamic_cast<CCArray*>(oneDic->objectForKey("reward"));
        ActivityRankReward* rankTwo = ActivityRankReward::create(rankArr,tempStr,false,-1,ARR_CELL_1);
        rankTwo->setPosition(ccp(-92/2, offY));
        m_rewardListNode->addChild(rankTwo);
        offY -= rankTwo->getTotalH();
        m_totalRankTip->setString("");
        m_btnReward->setVisible(true);
    }else{
        m_btnReward->setVisible(false);
        m_totalRankTip->setString(_lang("115240"));
        m_titleNode3->setPositionY(offY+80);
        offY += 80;
    }
    int rankH = 430 - offY;
    //m_rankBg->setContentSize(CCSize(550,rankH-40-160));
    //m_scBG->setContentSize(CCSize(550.0,rankH-rankTwoY-180));
    int totalH = 390 + rankH + hBG + 40 + 60;//1120+100
    
    offY -= 80;
    m_btnReward->setPositionY(offY);
    offY -= 90;
    //m_bottomL2->setPositionY(offY);
    //offY -= 30;
    m_btnRecord->setPositionY(offY);
    offY -= 90;
    m_btnHistory->setPositionY(offY);
    
    m_targetNode->removeFromParent();
    m_targetNode->setAnchorPoint(ccp(0, 0));
    m_targetNode->setPosition(ccp(CELL_X, TARGET_NODE_Y));//390
    m_scrollView->addChild(m_targetNode);
    
    m_earnNode->removeFromParent();
    m_earnNode->setAnchorPoint(ccp(0, 0));
    m_earnNode->setPosition(ccp(CELL_X, EARN_NODE_Y));//240
    m_scrollView->addChild(m_earnNode);
    
    m_rankNode->removeFromParent();
    m_rankNode->setAnchorPoint(ccp(0, 0));
    m_rankNode->setPosition(ccp(CELL_X, totalH-630-hBG+50));//590
    m_scrollView->addChild(m_rankNode);
    
    m_scrollView->setContentSize({SCROLL_W, (float)totalH});
    m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - totalH));
    m_totalH = totalH;
    m_bottomFix->setPositionY(m_scBG->getPositionY()-totalH + 30);
    m_scBG->setContentSize(CCSize(532,totalH-15));
    
    
    CCPoint pos = m_infoBtn2->getPosition();
    CCPoint worldPos = m_infoBtn2->getParent()->convertToWorldSpace(pos);
    CCPoint tabPos = m_scrollView->getContainer()->convertToNodeSpace(worldPos);
    m_tab2H  = tabPos.y + m_infoBtn2->getContentSize().height;
    
    pos = m_infoBtn3->getPosition();
    worldPos = m_infoBtn3->getParent()->convertToWorldSpace(pos);
    tabPos = m_scrollView->getContainer()->convertToNodeSpace(worldPos);
    m_tab3H  = tabPos.y + m_infoBtn3->getContentSize().height;
    
}

void ActivityBeginView::updateTime(float _time){
    double tempTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->eventInfo.readyTime;
    double readyTime = ActivityController::getInstance()->eventInfo.beginTime - GlobalData::shared()->getWorldTime();
    double endTime = ActivityController::getInstance()->eventInfo.endTime - GlobalData::shared()->getWorldTime() - 1;
    if(readyTime>0){
        m_timeTxt->setString(_lang_1("105804",CC_SECTOA(readyTime)));
    }else if (endTime>0){
        m_timeTxt->setString(_lang_1("105805",CC_SECTOA(endTime)));
    }else{
        this->unschedule(schedule_selector(ActivityBeginView::updateTime));
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void ActivityBeginView::onClickReward(CCObject *pSender, CCControlEvent event){
    CC_SAFE_RELEASE_NULL(m_totalRankRewardForUI);
    m_totalRankRewardForUI = CCArray::create();
    CC_SAFE_RETAIN(m_totalRankRewardForUI);
    
    // [1, 10]
    for (int i = 0; i <= 9 && i < m_totalRankReward->count(); i++)
    {
        auto oneDic = _dict(m_totalRankReward->objectAtIndex(i));
        oneDic->setObject(__String::create(CC_ITOA(i + 1)), "min");
        oneDic->setObject(__String::create(CC_ITOA(i + 1)), "max");
        m_totalRankRewardForUI->addObject(m_totalRankReward->objectAtIndex(i));
    }
    // [11, 19]
    for (int i = 10; i <= 18 && i < m_totalRankReward->count(); i++)
    {
        auto oneDic = _dict(m_totalRankReward->objectAtIndex(i));
        oneDic->setObject(__String::create("11"), "min");
        oneDic->setObject(__String::create("19"), "max");
        m_totalRankRewardForUI->addObject(oneDic);
        break;
    }
    for (int i = 19; i < m_totalRankReward->count(); i++)
    {
        if ((i + 1) % 10 == 0)
        {
            auto oneDic = _dict(m_totalRankReward->objectAtIndex(i));
            oneDic->setObject(__String::create(CC_ITOA(i + 1)), "min");
            oneDic->setObject(__String::create(CC_ITOA(i + 1 + 9)), "max");
            m_totalRankRewardForUI->addObject(oneDic);
        }
    }
    
    PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(m_totalRankRewardForUI, 8));
}

void ActivityBeginView::onClickRankReward(CCObject *pSender, CCControlEvent event){
    CC_SAFE_RELEASE_NULL(m_rankRewardForUI);
    m_rankRewardForUI = CCArray::create();
    CC_SAFE_RETAIN(m_rankRewardForUI);
    
    // [1, 10]
    for (int i = 0; i <= 9 && i < m_rankReward->count(); i++)
    {
        auto oneDic = _dict(m_rankReward->objectAtIndex(i));
        oneDic->setObject(__String::create(CC_ITOA(i + 1)), "min");
        oneDic->setObject(__String::create(CC_ITOA(i + 1)), "max");
        m_rankRewardForUI->addObject(m_rankReward->objectAtIndex(i));
    }
    // [11, 19]
    for (int i = 10; i <= 18 && i < m_rankReward->count(); i++)
    {
        auto oneDic = _dict(m_rankReward->objectAtIndex(i));
        oneDic->setObject(__String::create("11"), "min");
        oneDic->setObject(__String::create("19"), "max");
        m_rankRewardForUI->addObject(oneDic);
        break;
    }
    for (int i = 19; i < m_rankReward->count(); i++)
    {
        if ((i + 1) % 10 == 0)
        {
            auto oneDic = _dict(m_rankReward->objectAtIndex(i));
            oneDic->setObject(__String::create(CC_ITOA(i + 1)), "min");
            oneDic->setObject(__String::create(CC_ITOA(i + 1 + 9)), "max");
            m_rankRewardForUI->addObject(oneDic);
        }
    }
    
    PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(m_rankRewardForUI, 7));
}

void ActivityBeginView::onClickRecord(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create());
}

void ActivityBeginView::onInfoBtnClick(CCObject *pSender, CCControlEvent event){
    
    int num = m_eventIds.size();
    std::string tips = "";
    for (int i=0; i<num; i++) {
        std::string point = CCCommonUtils::getPropById(m_eventIds[i], "points");
        tips += _lang_1(CCCommonUtils::getPropById(m_eventIds[i], "tips"),point.c_str());
        tips += "\n";
    }
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips,kCCTextAlignmentLeft));
}

void ActivityBeginView::onInfoBtn2Click(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("105837"),kCCTextAlignmentLeft));
}

void ActivityBeginView::onInfoBtn3Click(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("105838"),kCCTextAlignmentLeft));
}

void ActivityBeginView::onClickHistory(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(1));
}

void ActivityBeginView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void ActivityBeginView::onCloseClick(CCObject *pSender){
    PopupViewController::getInstance()->removePopupView(this);
}

bool ActivityBeginView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sorceTxt", CCLabelIF*, this->m_sorceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sorceNum", CCLabelIF*, this->m_sorceNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankNum", CCLabelIF*, this->m_rankNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalRankTxt", CCLabelIF*, this->m_totalRankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalRankNum", CCLabelIF*, this->m_totalRankNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desc", CCLabelIF*, this->m_desc);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetNode", CCNode*, this->m_targetNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clipperNode", CCNode*, this->m_clipperNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetTxt3", CCLabelIF*, this->m_targetTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetTxt2", CCLabelIF*, this->m_targetTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetTxt1", CCLabelIF*, this->m_targetTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trTxt3", CCLabelIF*, this->m_trTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trTxt2", CCLabelIF*, this->m_trTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trTxt1", CCLabelIF*, this->m_trTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tRewardTxt3", CCLabelIF*, this->m_tRewardTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tRewardTxt2", CCLabelIF*, this->m_tRewardTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tRewardTxt1", CCLabelIF*, this->m_tRewardTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetIcon3", CCNode*, this->m_targetIcon3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetIcon2", CCNode*, this->m_targetIcon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetIcon1", CCNode*, this->m_targetIcon1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"//m_targetTxtBg3", CCNode*, this->//m_targetTxtBg3);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"//m_targetTxtBg2", CCNode*, this->//m_targetTxtBg2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"//m_targetTxtBg1", CCNode*, this->//m_targetTxtBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trNum3", CCLabelIF*, this->m_trNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trNum2", CCLabelIF*, this->m_trNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_trNum1", CCLabelIF*, this->m_trNum1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_earnNode", CCNode*, this->m_earnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_eNode", CCNode*, this->m_eNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_earnBg", CCScale9Sprite*, this->m_earnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_earnTxt", CCLabelIF*, this->m_earnTxt);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTitle1", CCLabelIF*, this->m_rankTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTitle2", CCLabelIF*, this->m_rankTitle2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_funList", CCNode*, this->m_funList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomFix", CCNode*, this->m_bottomFix);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankBg", CCScale9Sprite*, this->m_rankBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnRecord", CCControlButton*, this->m_btnRecord);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnRankReward", CCControlButton*, this->m_btnRankReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnHistory", CCControlButton*, this->m_btnHistory);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn2", CCControlButton*, this->m_infoBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn3", CCControlButton*, this->m_infoBtn3);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scBG", CCScale9Sprite*, this->m_scBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress1", CCLayerGradient*, this->m_progress1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalRankTip", CCLabelIF*, this->m_totalRankTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardListNode", CCNode*, this->m_rewardListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1", CCNode*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2", CCNode*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab3", CCNode*, this->m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_lordTxt", CCLabelIF*, this->m_lordTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetSorceTxt", CCLabelIF*, this->m_targetSorceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sorceTargetBtn", CCControlButton*, this->m_sorceTargetBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode2", CCNode*, this->m_titleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode3", CCNode*, this->m_titleNode3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_step1", CCSprite*, this->m_step1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_step2", CCSprite*, this->m_step2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_step3", CCSprite*, this->m_step3);
    
    return false;
}

SEL_CCControlHandler ActivityBeginView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRankReward", ActivityBeginView::onClickRankReward);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickReward", ActivityBeginView::onClickReward);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRecord", ActivityBeginView::onClickRecord);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoBtnClick", ActivityBeginView::onInfoBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoBtn2Click", ActivityBeginView::onInfoBtn2Click);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoBtn3Click", ActivityBeginView::onInfoBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", ActivityBeginView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStep1", ActivityBeginView::onStep1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStep2", ActivityBeginView::onStep2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStep3", ActivityBeginView::onStep3);
    
    return NULL;
}

cocos2d::SEL_MenuHandler ActivityBeginView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCloseClick", ActivityBeginView::onCloseClick);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTab1", ActivityBeginView::onTab1);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTab2", ActivityBeginView::onTab2);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTab3", ActivityBeginView::onTab3);
    
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onStep1", ActivityBeginView::onTab1);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onStep2", ActivityBeginView::onTab1);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onStep3", ActivityBeginView::onTab1);
    
    return NULL;
}

void ActivityBeginView::onStep1(CCObject *pSender, CCControlEvent event)
{
    if (!m_rewards || m_rewards->count() < 1) return;
    
    auto one = _dict(m_rewards->objectAtIndex(0));
    if (!one) return;
    
    CC_SAFE_RELEASE_NULL(m_reward1);
    m_reward1 = CCArray::create(one, NULL);
    CC_SAFE_RETAIN(m_reward1);
    
    PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(m_reward1, 6));
}

void ActivityBeginView::onStep2(CCObject *pSender, CCControlEvent event)
{
    if (!m_rewards || m_rewards->count() < 2) return;
    
    auto one = _dict(m_rewards->objectAtIndex(1));
    if (!one) return;
    
    CC_SAFE_RELEASE_NULL(m_reward2);
    m_reward2 = CCArray::create(one, NULL);
    CC_SAFE_RETAIN(m_reward2);
    
    PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(m_reward2, 6));
}

void ActivityBeginView::onStep3(CCObject *pSender, CCControlEvent event)
{
    if (!m_rewards || m_rewards->count() < 3) return;
    
    auto one = _dict(m_rewards->objectAtIndex(2));
    if (!one) return;
    
    CC_SAFE_RELEASE_NULL(m_reward3);
    m_reward3 = CCArray::create(one, NULL);
    CC_SAFE_RETAIN(m_reward3);
    
    PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(m_reward3, 6));
}

ActivityEarnPointCell *ActivityEarnPointCell::create(std::string id){
    ActivityEarnPointCell *ret = new ActivityEarnPointCell(id);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityEarnPointCell::onEnter(){
    CCNode::onEnter();
}

void ActivityEarnPointCell::onExit(){
    CCNode::onExit();
}

bool ActivityEarnPointCell::init(){
    auto bg = CCBLoadFile("ActivityEarnPointCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    std::string name = CCCommonUtils::getNameById(m_id);
    m_nameTxt->setString(name);
    if (CCCommonUtils::isIosAndroidPad()) {
        float fw = m_nameTxt->getContentSize().width;
        float s = m_nameTxt->getScaleX();
        if (fw>1000) {//防爆框
            m_nameTxt->setScaleX(1000/fw*s);
        }
    }
    else
        m_nameTxt->setMaxScaleXByWidth(500);
    
    std::string point = CCCommonUtils::getPropById(m_id, "points");
    m_pointTxt->setString(point);
    return true;
}


bool ActivityEarnPointCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pointTxt", CCLabelIF*, this->m_pointTxt);
    return false;
}

SEL_CCControlHandler ActivityEarnPointCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}