//
//  CrossServiceTargetView.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/8.
//
//

#include "CrossServiceTargetView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "TipsView.h"
#include "CrossServiceActivityCommand.h"
#include "ActivityRewardView.h"
#include "ActivityController.h"
#include "ActivityHistoryView.h"
#include "CSALeaveCommand.h"
#include "ToolController.h"
#include "ParticleController.h"
#include "CrossServerScoreCommand.h"
#include "QuestController.h"
#include "RewardController.h"
#include "CKFRewardInfoCommand.h"
#include "FunBuildController.h"
#include "AllianceActivityRewardView.h"
#include "ParticleFireAni.h"
#include "RoleInfoView.h"
#include "LianShengView.h"

CrossServiceTargetView *CrossServiceTargetView::create(){
    CrossServiceTargetView *ret = new CrossServiceTargetView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CrossServiceTargetView::onEnter(){
    this->setTitleName(_lang("138000"));
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CrossServiceTargetView::refreshTarget), QUEST_STATE_UPDATE, NULL);
}

void CrossServiceTargetView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    PopupBaseView::onExit();
}

bool CrossServiceTargetView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CrossServiceTargetView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if (!isTouchInside(m_clickArea, pTouch)) {
//        PopupViewController::getInstance()->removePopupView(this);
//        return ;
//    }
}

bool CrossServiceTargetView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(506, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        CCLoadSprite::doResourceByCommonIndex(305, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(101, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(506, false);
            CCLoadSprite::doResourceByCommonIndex(11, false);
            CCLoadSprite::doResourceByCommonIndex(208, false);
            CCLoadSprite::doResourceByCommonIndex(305, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(101, false);
        });
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(0, curHight));
//            curHight += bg->getContentSize().height;
//            tBatchNode->addChild(bg);
//        }
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            tBatchNode->setScaleX(2.4);
//        }
//        this->addChild(tBatchNode);
        auto tmpCCB = CCBLoadFile("CrossServiceTargetView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        int preH = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int addH = m_viewBg->getContentSize().height - preH;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
            addH = addH / 2.4f;
        }
        m_viewBg->setVisible(false);
        
        m_tableNode->setContentSize(CCSize(m_tableNode->getContentSize().width,m_tableNode->getContentSize().height+addH));
        m_tableNode->setPositionY(m_tableNode->getPositionY() - addH);
        
        m_scNode->setContentSize(CCSize(m_scNode->getContentSize().width,m_scNode->getContentSize().height+addH));
        m_scNode->setPositionY(m_scNode->getPositionY() - addH);
        
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
        m_rewardNode->setContentSize(CCSize(m_rewardNode->getContentSize().width,m_rewardNode->getContentSize().height+addH));
        m_rewardNode->setPositionY(m_rewardNode->getPositionY() - addH);
        
        m_tab1->setTouchPriority(1);
        m_tab2->setTouchPriority(1);
        m_tab3->setTouchPriority(1);
        m_tab4->setTouchPriority(1);
        CCCommonUtils::setButtonTitle(m_helpBtn, _lang("138082").c_str());
        m_tabTxt1->setString(_lang("138056"));
        m_tabTxt2->setString(_lang("138025"));
        m_tabTxt3->setString(_lang("138027"));
        m_tabTxt4->setString(_lang("138026"));
        m_targetTip->setString(_lang("138089"));
        
        m_data = CCArray::create();
        m_taskData = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_tableNode->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_tableNode->addChild(m_tabView);
        
        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Default);
        m_scNode->addChild(m_scrollView);
        
        m_introNode->removeFromParent();
        m_introNode->setPositionY(m_introNode->getPositionY()+320);
        m_scrollView->addChild(m_introNode);
        int totalH = 1000;
        m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,totalH));
        m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - totalH));

        m_scrollView2 = CCScrollView::create(m_rewardNode->getContentSize());
        m_scrollView2->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView2->setTouchPriority(Touch_Default);
        m_rewardNode->addChild(m_scrollView2);
        

        
        CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("101313").c_str());
        m_rwdBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
        
        m_actObj = NULL;
        m_score = 0;
        m_desc = "";
        if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
            int num = ActivityController::getInstance()->activityArr->count();
            for (int i=0; i<num; i++) {
                auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                if (temp && temp->type==7) {
                    m_actObj = temp;
                    m_nameTxt->setString(m_actObj->name);
                    m_desc = m_actObj->desc;
                    break;
                }
            }
        }
        this->schedule(schedule_selector(CrossServiceTargetView::updateTime),1);
        this->updateTime(0);
        
        m_scoreInfo = NULL;
        CrossServerScoreCommand* cmd = new CrossServerScoreCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CrossServiceTargetView::getData), NULL));
        cmd->sendAndRelease();
        
        if(ActivityController::getInstance()->getCkfMemberReward()->count()<=0){
            CKFRewardInfoCommand* cmd2 = new CKFRewardInfoCommand();
            cmd2->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CrossServiceTargetView::getData2), NULL));
            cmd2->sendAndRelease();
        }
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_scNode);
        m_waitInterface->setPosition(ccp(m_scNode->getContentSize().width/2, m_scNode->getContentSize().height/2));
        m_tabIndex = 0;
        if (checkHaveCompleteTask()) {
            onClickTab2(NULL,Control::EventType::TOUCH_DOWN);
        }else{
            onClickTab1(NULL,Control::EventType::TOUCH_DOWN);
        }
        ret = true;
    }
    return ret;
}

bool CrossServiceTargetView::checkHaveCompleteTask(){
    bool flag = false;
    map<std::string, QuestInfo*>::iterator it;
    for (it = QuestController::getInstance()->currentCKFList.begin(); it!=QuestController::getInstance()->currentCKFList.end(); it++) {
        if (it->second->type==3) {
            if (it->second->state==COMPLETE) {
                flag = true;
                break;
            }
        }
    }
    return flag;
}

void CrossServiceTargetView::updateTime(float _time){
    if (m_actObj) {
        double nowTime = GlobalData::shared()->getWorldTime();
        string temp = m_desc;
        if (m_actObj->startTime<nowTime && m_actObj->endTime>nowTime)
        {
            int leftTime = m_actObj->endTime - nowTime;
            int k7 = ActivityController::getInstance()->wb_info_k7;
            if (k7<1) {
                k7 = 1;
            }
            int fTime = leftTime%(k7*60*60);
            m_timeTxt->setString(_lang_1("138030", CC_SECTOA(fTime)));
            temp.append("\n");
            temp.append(_lang_1("138114", CC_SECTOA(leftTime)));
            
            temp.append("\n");
            temp.append(_lang_1("138115", CC_SECTOA(fTime)));
        }
        else if (m_actObj->startTime > nowTime)
        {
            double leftTime = m_actObj->startTime - nowTime;
        }
         m_descTxt->setString(temp);
    }
}

void CrossServiceTargetView::getData(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    m_scoreInfo = _dict(result->getData());
    if (m_scoreInfo && m_scoreInfo->objectForKey("userScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("userScore"));
        CCArray* arr = dynamic_cast<CCArray*>(dic->objectForKey("rankList"));
        if (arr) {
            for (int i=0; i<arr->count(); i++) {
                auto info = _dict(arr->objectAtIndex(i));
                info->setObject(CCString::create(CC_ITOA(i)), "rank");
            }
        }
    }
    if (m_scoreInfo && m_scoreInfo->objectForKey("kingdomScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("kingdomScore"));
        CCArray* arr2 = dynamic_cast<CCArray*>(dic->objectForKey("rankList"));
        if (arr2) {
            for (int i=0; i<arr2->count(); i++) {
                auto info = _dict(arr2->objectAtIndex(i));
                info->setObject(CCString::create(CC_ITOA(i)), "rank");
            }
        }
    }
}

void CrossServiceTargetView::getData2(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    auto ckfReward = _dict(dic->objectForKey("ckfReward"));
    CCArray* members = dynamic_cast<CCArray*>(ckfReward->objectForKey("member"));
    if (members) {
        ActivityController::getInstance()->getCkfMemberReward()->addObjectsFromArray(members);
    }
    CCArray* kingdom = dynamic_cast<CCArray*>(ckfReward->objectForKey("kingdom"));
    if (kingdom) {
        for (int i=0; i<kingdom->count(); i++) {
            auto oneRank = _dict(kingdom->objectAtIndex(i));
            if(oneRank->objectForKey("effect")){
                CCArray* rewards = dynamic_cast<CCArray*>(oneRank->objectForKey("reward"));
                if (!rewards) {
                    rewards = CCArray::create();
                    oneRank->setObject(rewards, "reward");
                }
                CCDictionary* eReward = CCDictionary::create();
                eReward->setObject(CCString::create(CC_ITOA(R_EFFECT)), "type");
                eReward->setObject(oneRank->objectForKey("effect"), "value");
                rewards->insertObject(eReward, 0);
            }
        }
        ActivityController::getInstance()->getCkfKingdomReward()->addObjectsFromArray(kingdom);
    }
}

void CrossServiceTargetView::refreshTarget(CCObject *pSender){
    if (m_tabIndex==1) {
        this->unschedule(schedule_selector(CrossServiceTargetView::delayFresh));
        this->schedule(schedule_selector(CrossServiceTargetView::delayFresh), 0.5);
    }
}

void CrossServiceTargetView::delayFresh(float _time){
    this->unschedule(schedule_selector(CrossServiceTargetView::delayFresh));
    this->freshScrollView();
}

void CrossServiceTargetView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void CrossServiceTargetView::onClickRwdBtn(CCObject *pSender, CCControlEvent event){
    m_rwdBtn->setEnabled(false);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Change");
    QuestController::getInstance()->getReward(itemId);
}

void CrossServiceTargetView::onLianShengClick(CCObject *pSender, CCControlEvent event){
    if(ActivityController::getInstance()->checkLianShengOpen()){
        PopupViewController::getInstance()->addPopupView(LianShengView::create());
    }else{
        CCCommonUtils::flyHint("", "", _lang("E100008"));
    }
}

void CrossServiceTargetView::onClickTab1(CCObject *pSender, CCControlEvent event){
    CCLOG("onClickTab1");
    m_tabIndex = 0;
    m_tab1->setEnabled(false);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(true);
    m_introNode->setVisible(true);
    m_scNode->setVisible(true);
    m_rankNode->setVisible(false);
    m_targetNode->setVisible(false);
    m_tabTxt1->setColor({255,206,87});
    m_tabTxt2->setColor({104,88,73});
    m_tabTxt3->setColor({104,88,73});
    m_tabTxt4->setColor({104,88,73});
    freshScrollView();
}

void CrossServiceTargetView::onClickTab2(CCObject *pSender, CCControlEvent event){
    CCLOG("onClickTab2");
    m_tabIndex = 1;
    this->freshScrollView();
    m_scNode->setVisible(true);
    m_rankNode->setVisible(false);
    m_introNode->setVisible(false);
    m_targetNode->setVisible(true);
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(false);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(true);
    m_tabTxt1->setColor({104,88,73});
    m_tabTxt2->setColor({255,206,87});
    m_tabTxt3->setColor({104,88,73});
    m_tabTxt4->setColor({104,88,73});
    
    m_fireNode1->removeAllChildrenWithCleanup(true);
    ParticleFireAni* par = ParticleFireAni::create();
    m_fireNode1->addChild(par);
    
    m_fireNode2->removeAllChildrenWithCleanup(true);
    ParticleFireAni* par2 = ParticleFireAni::create();
    m_fireNode2->addChild(par2);
}

void CrossServiceTargetView::onClickTab3(CCObject *pSender, CCControlEvent event){
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(false);
    m_tab4->setEnabled(true);
    m_tabTxt1->setColor({104,88,73});
    m_tabTxt2->setColor({104,88,73});
    m_tabTxt3->setColor({255,206,87});
    m_tabTxt4->setColor({104,88,73});
    
    m_scNode->setVisible(false);
    m_rankNode->setVisible(true);
    m_introNode->setVisible(false);
    m_targetNode->setVisible(false);
    m_tabIndex = 2;
    m_data->removeAllObjects();
    m_scoreNode->stopAllActions();
    if (m_scoreInfo && m_scoreInfo->objectForKey("kingdomScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("kingdomScore"));
        CCArray* arr = dynamic_cast<CCArray*>(dic->objectForKey("rankList"));
        if (arr) {
            m_data->addObjectsFromArray(arr);
        }
        CCDictionary* btnInfo = CCDictionary::create();
        btnInfo->setObject(CCString::create(CC_ITOA(-1)), "rank");
        m_data->addObject(btnInfo);
        m_tabView->reloadData();
        
        m_kingdomScore = dic->valueForKey("kingdomScore")->doubleValue();
        int rank = dic->valueForKey("kingdomRank")->intValue();
        int mRank = dic->valueForKey("kingdomMaxRank")->intValue();
        
        m_txt1->setString(_lang_1("138031", ""));
        m_txt2->setString(_lang_1("138032", ""));
        m_txt3->setString(_lang_1("138033", ""));
        
        m_txt2Value->setString(CC_CMDITOAL(rank));
        m_txt3Value->setString(CC_CMDITOAL(mRank));
        
        double lastKingdomScore = 0.0;
        string strV = CCUserDefault::sharedUserDefault()->getStringForKey("crossServerKingdomScore", "");
        if (strV!="") {
            lastKingdomScore = atof(strV.c_str());
        }
        if (lastKingdomScore>=m_kingdomScore) {
            lastKingdomScore = m_kingdomScore;
        }
        m_txt1Value->setString(CC_CMDITOAD(lastKingdomScore));
        
        m_score = lastKingdomScore;
        m_perScore = (m_kingdomScore - lastKingdomScore)/15;
        if (m_perScore<=1) {
            m_perScore = 1;
        }
        CCActionInterval * scale1 = CCScaleTo::create(0.08, 1.2);
        CCActionInterval * scale2 = CCScaleTo::create(0.06, 1.1);
        CCActionInterval * repeat = CCRepeatForever::create(CCSequence::create(scale1, scale2, NULL));
        m_scoreNode->runAction(repeat);
        this->unschedule(schedule_selector(CrossServiceTargetView::updateScore));
        this->schedule(schedule_selector(CrossServiceTargetView::updateScore));
        CCUserDefault::sharedUserDefault()->setStringForKey("crossServerKingdomScore", CC_ITOA(m_kingdomScore));
        CCUserDefault::sharedUserDefault()->flush();
    }
}

void CrossServiceTargetView::onClickTab4(CCObject *pSender, CCControlEvent event){
    CCLOG("onClickTab4");
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(false);
    m_tabTxt1->setColor({104,88,73});
    m_tabTxt2->setColor({104,88,73});
    m_tabTxt3->setColor({104,88,73});
    m_tabTxt4->setColor({255,206,87});
    
    m_scNode->setVisible(false);
    m_rankNode->setVisible(true);
    m_introNode->setVisible(false);
    m_tabIndex = 3;
    m_data->removeAllObjects();
    m_scNode->setVisible(false);
    m_rankNode->setVisible(true);
    m_introNode->setVisible(false);
    m_targetNode->setVisible(false);
    m_tabIndex = 1;
    m_data->removeAllObjects();
    m_scoreNode->stopAllActions();
    if (m_scoreInfo && m_scoreInfo->objectForKey("userScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("userScore"));
        CCArray* arr = dynamic_cast<CCArray*>(dic->objectForKey("rankList"));
        if (arr) {
            m_data->addObjectsFromArray(arr);
        }
        CCDictionary* btnInfo = CCDictionary::create();
        btnInfo->setObject(CCString::create(CC_ITOA(-1)), "rank");
        m_data->addObject(btnInfo);
        m_tabView->reloadData();
        
        m_userScore = dic->valueForKey("userScore")->doubleValue();
        int userRank = dic->valueForKey("userRank")->intValue();
        int userMaxRank = dic->valueForKey("userMaxRank")->intValue();
        
        m_txt1->setString(_lang_1("138034", ""));
        m_txt2->setString(_lang_1("138035", ""));
        m_txt3->setString(_lang_1("138036", ""));
        m_txt2Value->setString(CC_CMDITOAL(userRank));
        m_txt3Value->setString(CC_CMDITOAL(userMaxRank));
        
        double lastUserScore = 0.0;
        string strTime = CCUserDefault::sharedUserDefault()->getStringForKey("crossServerUserScore", "");
        if (strTime!="") {
            lastUserScore = atof(strTime.c_str());
        }
        if (lastUserScore>=m_userScore) {
            lastUserScore = m_userScore;
        }
        m_txt1Value->setString(CC_CMDITOAD(lastUserScore));
        
        m_score = lastUserScore;
        m_perScore = (m_userScore - lastUserScore)/15;
        if (m_perScore<=1) {
            m_perScore = 1;
        }
        CCActionInterval * scale1 = CCScaleTo::create(0.08, 1.2);
        CCActionInterval * scale2 = CCScaleTo::create(0.06, 1.1);
        CCActionInterval * repeat = CCRepeatForever::create(CCSequence::create(scale1, scale2, NULL));
        m_scoreNode->runAction(repeat);
        this->unschedule(schedule_selector(CrossServiceTargetView::updateScore));
        this->schedule(schedule_selector(CrossServiceTargetView::updateScore));
        
        CCUserDefault::sharedUserDefault()->setStringForKey("crossServerUserScore", CC_ITOA(m_userScore));
        CCUserDefault::sharedUserDefault()->flush();
    }
}

void CrossServiceTargetView::updateScore(float _time){
    m_score = m_score + m_perScore;
    if (m_tabIndex==2) {
        if (m_score>=m_kingdomScore) {
            m_score = m_kingdomScore;
            this->unschedule(schedule_selector(CrossServiceTargetView::updateScore));
            m_scoreNode->stopAllActions();
            m_scoreNode->setScale(1.0);
        }
    }else{
        if (m_score>=m_userScore) {
            m_score = m_userScore;
            this->unschedule(schedule_selector(CrossServiceTargetView::updateScore));
            m_scoreNode->stopAllActions();
            m_scoreNode->setScale(1.0);
        }
    }
    m_txt1Value->setString(CC_CMDITOAD(m_score));
}

void CrossServiceTargetView::onClickHelpBtn(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45209");
}

void CrossServiceTargetView::onInfoClick(CCObject *pSender, CCControlEvent event){
    string str = _lang("138055");
    str.append("\n");
    int soldierLv = (FunBuildController::getInstance()->getMainCityLv()-1)/3+1;
    if (soldierLv>10) {
        soldierLv = 10;
    }
    for (int i=1; i<=10; i++) {
        int N = i - soldierLv;
        float factor = 1.0;
        for (int k=1; k<=6; k++) {
            string key = string("k") + CC_ITOA(k);
            string value = ActivityController::getInstance()->wbScoreMap[key];
            std::vector<std::string> arr1;
            CCCommonUtils::splitString(value,"|",arr1);
            if (arr1.size()==2) {
                std::vector<std::string> arr2;
                CCCommonUtils::splitString(arr1[0],";",arr2);
                if (arr2.size()==2) {
                    int x1 = atoi(arr2[0].c_str());
                    int x2 = atoi(arr2[1].c_str());
                    if (N>=x2 && N<=x1) {
                        factor = (100 + atoi(arr1[1].c_str()))*1.0/100;
                        break;
                    }
                }
            }
        }
        int baseScore = atoi(ActivityController::getInstance()->wbScoreMap["k9"].c_str());
        int score = factor*baseScore;
        str.append(_lang_2("138069", CC_ITOA(i), CC_ITOA(score)));
        str.append("\n");
    }
    str.append(_lang("138070"));
    PopupViewController::getInstance()->addPopupView(TipsView::create(str,kCCTextAlignmentLeft));
}

void CrossServiceTargetView::freshScrollView(){
    int totalH = 0;
    if (m_tabIndex==0) {
    }else if(m_tabIndex==1){
        m_taskData->removeAllObjects();
        map<std::string, QuestInfo*>::iterator it;
        CCArray* temp1 = CCArray::create();
        CCArray* temp2 = CCArray::create();
        for (it = QuestController::getInstance()->currentCKFList.begin(); it!=QuestController::getInstance()->currentCKFList.end(); it++) {
            if (it->second->type==3) {
                if (it->second->state==COMPLETE) {
                    temp1->addObject(it->second);
                }else{
                    temp2->addObject(it->second);
                }
            }
        }
        int num = temp1->count();
        for (int i=0; i<num; i++) {
            for (int j=i; j<num-1; j++) {
                QuestInfo* info = dynamic_cast<QuestInfo*>(temp1->objectAtIndex(j));
                QuestInfo* info2 = dynamic_cast<QuestInfo*>(temp1->objectAtIndex(j+1));
                if (info->currPhase>info2->currPhase) {
                    temp1->swap(j, j+1);
                }
            }
        }
        m_taskData->addObjectsFromArray(temp1);
        m_taskData->addObjectsFromArray(temp2);
        if (m_taskData->count()>0) {
            QuestInfo* info = dynamic_cast<QuestInfo*>(m_taskData->objectAtIndex(0));
            itemId = info->itemId;
            string tempName = _lang_3("138028", CC_ITOA(info->currPhase), CC_ITOA(info->maxPhase), info->name.c_str());
            m_taskNameTxt->setString(tempName);
            int currValue = info->curValue;
            if (currValue>=info->maxValue) {
                currValue = info->maxValue;
            }
            string per = CC_CMDITOAD(currValue);
            per.append("/");
            per.append(CC_CMDITOAD(info->maxValue));
            m_perTxt->setString(per.c_str());
            float perV = info->curValue/(1.0*info->maxValue);
            if (perV>=1) {
                perV = 1.0;
            }
            m_perSpr->setContentSize(CCSize(perV*500,26));
            m_helpTxt->setString(_lang("138107"));
            m_helpTip->setString(info->description);
            m_rewardTxt->setString(_lang("138108"));
            m_tastTxt->setString(info->name);
            
            m_taskIcon->removeAllChildrenWithCleanup(true);
            auto pic = CCLoadSprite::createSprite(info->icon1.c_str());
            m_taskIcon->addChild(pic);
            CCSprite* pic1 =NULL;
            if(info->icon1!=""){
                pic1 = CCLoadSprite::createSprite(info->icon1.c_str());
                m_taskIcon->addChild(pic1);
            }
            if(info->icon2!=""){
                CCSprite* pic = CCLoadSprite::createSprite(info->icon2.c_str());
                m_taskIcon->addChild(pic);
                if(pic1!=NULL){
                    pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
                }
            }
            int rewardNum = 0;
            if(info->reward){
                m_scrollView2->getContainer()->removeAllChildrenWithCleanup(true);
                int scH = m_rewardNode->getContentSize().height;
                rewardNum = info->reward->count();
                int gapY = 90;
                 int rewardH = rewardNum*gapY;
                for (int i=0; i<rewardNum; i++) {
                    auto dic = _dict(info->reward->objectAtIndex(i));
                    AllianceActivityRewardCell* cell = AllianceActivityRewardCell::create(dic);
                    rewardH -= gapY;
                    cell->setPosition(ccp(25,rewardH));
                    m_scrollView2->addChild(cell);
                }
                rewardH = rewardNum*gapY;
                m_scrollView2->setContentSize(CCSize(m_rewardNode->getContentSize().width,rewardH));
                m_scrollView2->setContentOffset(ccp(0, m_rewardNode->getContentSize().height - rewardH));
            }
            m_rwdBtn->setEnabled(info->state==COMPLETE);
        }
        totalH = 1000;
        if (m_taskData->count()<=0) {
            m_targetTip->setVisible(true);
        }else{
            m_targetTip->setVisible(false);
        }
    }
}

bool CrossServiceTargetView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1", CCControlButton*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2", CCControlButton*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab3", CCControlButton*, this->m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab4", CCControlButton*, this->m_tab4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tableNode", CCNode*, this->m_tableNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabNode1", CCNode*, this->m_tabNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabRewardNode1", CCNode*, this->m_tabRewardNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_introNode", CCNode*, this->m_introNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabTxt1", CCLabelIF*, this->m_tabTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabTxt2", CCLabelIF*, this->m_tabTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabTxt3", CCLabelIF*, this->m_tabTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabTxt4", CCLabelIF*, this->m_tabTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1Value", CCLabelIF*, this->m_txt1Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2Value", CCLabelIF*, this->m_txt2Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3Value", CCLabelIF*, this->m_txt3Value);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_parNode", CCNode*, this->m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scoreNode", CCNode*, this->m_scoreNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetTip", CCLabelIF*, this->m_targetTip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetNode", CCNode*, this->m_targetNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_perTxt", CCLabelIF*, this->m_perTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_taskNameTxt", CCLabelIF*, this->m_taskNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTxt", CCLabelIF*, this->m_rewardTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpTip", CCLabelIF*, this->m_helpTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tastTxt", CCLabelIF*, this->m_tastTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpTxt", CCLabelIF*, this->m_helpTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_perSpr", CCScale9Sprite*, this->m_perSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_taskIcon", CCNode*, this->m_taskIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    return false;
}

SEL_CCControlHandler CrossServiceTargetView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", CrossServiceTargetView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab1", CrossServiceTargetView::onClickTab1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab2", CrossServiceTargetView::onClickTab2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab3", CrossServiceTargetView::onClickTab3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab4", CrossServiceTargetView::onClickTab4);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", CrossServiceTargetView::onClickHelpBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoClick", CrossServiceTargetView::onInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", CrossServiceTargetView::onClickRwdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLianShengClick", CrossServiceTargetView::onLianShengClick);
    return NULL;
}

CCSize CrossServiceTargetView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    int h = 70;
    CCDictionary* dic = _dict(m_data->objectAtIndex(idx));
    int rank = dic->valueForKey("rank")->intValue();
    if (rank<3) {
        h = 105;
    }
    return CCSize(640, h);
}

CCSize CrossServiceTargetView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 70);
}

CCTableViewCell* CrossServiceTargetView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    ActivityRankCell* cell = (ActivityRankCell*)table->dequeueCell();
    CCDictionary* dic = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(dic,m_tabIndex);
        }else{
            cell = ActivityRankCell::create(dic,m_tableNode,m_tabIndex);
        }
    }
    return cell;
}

ssize_t CrossServiceTargetView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void CrossServiceTargetView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

//
ActivityTargetCell *ActivityTargetCell::create(QuestInfo* info,CCNode* clickArea){
    ActivityTargetCell *ret = new ActivityTargetCell(info,clickArea);
    if(ret && ret->init()){
    ret->autorelease();
    }else{
    CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityTargetCell::init(){
    auto node = CCBLoadFile("ActivityTargetCell", this, this);
    this->setContentSize(node->getContentSize());
    m_rwdBtn->setTouchPriority(3);
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("101313").c_str());
    string tempName = _lang_3("138028", CC_ITOA(m_info->currPhase), CC_ITOA(m_info->maxPhase), m_info->name.c_str());
    m_nameTxt->setString(tempName);
    
    int currValue = m_info->curValue;
    if (currValue >=m_info->maxValue) {
        currValue = m_info->maxValue;
    }
    string per = CC_ITOA(currValue);
    per.append("/");
    per.append(CC_ITOA(m_info->maxValue));
    m_perTxt->setString(per.c_str());
    float perV = m_info->curValue/(1.0*m_info->maxValue);
    if (perV>=1) {
        perV = 1.0;
    }
    m_perSpr->setContentSize(CCSize(perV*500,26));
    m_rwdBtn->setVisible(false);
    int rewardNum = 0;
    if(m_info->reward){
        rewardNum = m_info->reward->count();
        for (int i=0; i<rewardNum; i++) {
            auto dic = _dict(m_info->reward->objectAtIndex(i));
            TargetRewardCell* cell = TargetRewardCell::create(dic,m_clickArea);
            cell->setPosition(ccp(115*i,  -1));
            m_rewardNode->addChild(cell);
        }
    }
    m_rewardNode->setPositionX(-rewardNum*115/2 + 320 + 61);
    m_gray->setVisible(false);
    m_rwdBtn->setVisible(false);
    m_okSpr->setVisible(false);
    m_rewardNode->setVisible(true);
    if (m_info->state==ACCEPT) {
        //setSpriteGray(node);
        m_rewardNode->setVisible(true);
    }else if(m_info->state==COMPLETE){
        m_rwdBtn->setVisible(true);
        m_rewardNode->setVisible(false);
    }else if(m_info->state==REWARD){
        m_okSpr->setVisible(true);
    }else{
        setSpriteGray(node);
    }

    return true;
}

void ActivityTargetCell::setSpriteGray(CCNode* node){
    if (node==NULL) {
        return;
    }
    auto& arr = node->getChildren();
    if (arr.size()>0) {
        int num = arr.size();
        for (int i=0; i<num; i++) {
            CCNode* node1 = dynamic_cast<CCNode*>(arr.at(i));
            CCSprite* ch1 = dynamic_cast<CCSprite*>(arr.at(i));
            if (ch1) {
                CCCommonUtils::setSpriteGray(ch1, true);
                if (ch1->getChildrenCount()>1) {
                    setSpriteGray(ch1);
                }
            }else {
                if (node1 && node1->getChildrenCount()>1) {
                    setSpriteGray(node1);
                }
            }
        }
    }
}

void ActivityTargetCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setSwallowsTouches(false);
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void ActivityTargetCell::onExit(){
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool ActivityTargetCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_clickArea && m_clickArea->isVisible() && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    return false;
}

void ActivityTargetCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
    if(isTouchInside(m_rwdBtn, pTouch) && m_rwdBtn->isVisible() && m_rwdBtn->isEnabled()){
        CCLog("m_rwdBtn");
        m_rwdBtn->setEnabled(false);
        QuestController::getInstance()->getReward(m_info->itemId);
        //PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->allianceKill,2));
    }
}

void ActivityTargetCell::setData(){

}

void ActivityTargetCell::onClickRwdBtn(CCObject *pSender, CCControlEvent event){
    
}

bool ActivityTargetCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gray", CCNode*, this->m_gray);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_perSpr", CCScale9Sprite*, this->m_perSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_perTxt", CCLabelIF*, this->m_perTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okSpr", CCNode*, this->m_okSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    return false;
}

SEL_CCControlHandler ActivityTargetCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", ActivityTargetCell::onClickRwdBtn);
    return NULL;
}

//
ActivityRankCell *ActivityRankCell::create(CCDictionary* info,CCNode* clickArea,int type){
    ActivityRankCell *ret = new ActivityRankCell(info,clickArea,type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityRankCell::init(){
    auto node = CCBLoadFile("ActivityRankCell", this, this);
    this->setContentSize(node->getContentSize());
    m_btn1->setTouchPriority(2);
    m_btn1->setSwallowsTouches(false);
    m_btn2->setTouchPriority(2);
    m_btn2->setSwallowsTouches(false);
    CCCommonUtils::setButtonTitle(m_btn1, _lang("138052").c_str());
    CCCommonUtils::setButtonTitle(m_btn2, _lang("138003").c_str());
    m_headImgNode = HFHeadImgNode::create();
    this->setData(m_info,m_type);
    return true;
}

void ActivityRankCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}


void ActivityRankCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void ActivityRankCell::setData(CCDictionary* info,int type){
    m_info = info;
    m_type = type;
    if (!m_info || !m_info->objectForKey("rank")) {
        return ;
    }
    int rank = m_info->valueForKey("rank")->intValue();
    int serverId = m_info->valueForKey("serverId")->intValue();
    double score = m_info->valueForKey("score")->doubleValue();
    string abbr = m_info->valueForKey("abbr")->getCString();
    string uid = m_info->valueForKey("uid")->getCString();
    string name = m_info->valueForKey("name")->getCString();
    string pic = m_info->valueForKey("pic")->getCString();
    if (pic=="" && m_type==2) {
        pic = "UN";
    }
    pic.append(".png");
    string kingdomName = m_info->valueForKey("kingdom")->getCString();
    int pivVer = m_info->valueForKey("pivVer")->intValue();
    if (kingdomName=="") {
        kingdomName = _lang("138027");
        kingdomName.append("#");
        kingdomName.append(CC_ITOA(serverId));
    }
    m_icon->removeAllChildrenWithCleanup(true);
    m_aniNode->removeAllChildrenWithCleanup(true);
    m_nameTxt1->setColor({232,209,141});
    m_scoreTxt2->setColor({232,209,141});
    m_scoreTxt1->setColor({232,209,141});
    m_clickNode->setContentSize(CCSize(640,rank<3?75:50));
    if (rank==-1) {
        m_top3Node->setVisible(false);
        m_normalNode->setVisible(false);
        m_btnNode->setVisible(true);
    }else if (rank<3) {
        m_top3Node->setVisible(true);
        m_normalNode->setVisible(false);
        m_btnNode->setVisible(false);
        string temp = "";
        if (abbr!="") {
            temp.append("(");
            temp.append(abbr);
            temp.append(")");
        }
        temp.append(name);
        m_nameTxt2->setString(temp.c_str());
        m_scoreTxt2->setString(CC_CMDITOAD(score));
        if (m_type==2) {
            m_nameTxt2->setString(kingdomName.c_str());
            m_sTxt->setString("");
            m_nameTxt2->setPositionY(m_scoreTxt2->getPositionY());
        }else{
            m_sTxt->setString(kingdomName.c_str());
            m_nameTxt2->setPositionY(82.1);
        }
        CCLoadSprite::doResourceByCommonIndex(7, true);
        string rankStr = CCString::createWithFormat("Alliance_Ranking%d.png",rank+2)->getCString();
        auto cf = CCLoadSprite::loadResource(rankStr.c_str());
        if (cf) {
            m_numspr->initWithSpriteFrame(cf);
        }
        auto picSpr = CCLoadSprite::createSprite(pic.c_str());
        CCCommonUtils::setSpriteMaxSize(picSpr, 61,true);
        m_icon->addChild(picSpr);
        if (CCCommonUtils::isUseCustomPic(pivVer) && m_type!=2) {
            m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(uid, pivVer), 1.0f, 61, true);
        }
        RankCellAni* ani = RankCellAni::create(rank);
        ani->setPositionY(16);
        m_aniNode->addChild(ani);
        
    }else{
        m_top3Node->setVisible(false);
        m_normalNode->setVisible(true);
        m_btnNode->setVisible(false);
        string temp = "";
        if (abbr!="") {
            temp.append("(");
            temp.append(abbr);
            temp.append(")");
        }
        temp.append(name);
        temp.append(" #");
        temp.append(CC_ITOA(serverId));
        if (m_type==2) {
            temp = kingdomName;
            temp.append("#");
            temp.append(CC_ITOA(serverId));
        }
        m_nameTxt1->setString(temp.c_str());
        m_scoreTxt1->setString(CC_CMDITOAD(score));
        m_numTxt->setString(CC_ITOA(rank+1));
        if (uid==GlobalData::shared()->playerInfo.uid) {
            m_nodeBg2->setVisible(true);
            m_nodeBg1->setVisible(false);
            m_nameTxt1->setColor({204,255,162});
            m_scoreTxt1->setColor({204,255,162});
        }else{
            m_nodeBg1->setVisible(true);
            m_nodeBg2->setVisible(false);
        }
        
    }
}

bool ActivityRankCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_clickArea && m_clickArea->getParent() && m_clickArea->getParent()->isVisible() && isTouchInside(m_clickArea, pTouch) && isTouchInside(m_clickNode, pTouch)) {
        return true;
    }
    return false;
}

void ActivityRankCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
    if (isTouchInside(m_btn1, pTouch) && m_btnNode->isVisible()) {
        CCLog("m_btn1");
        if (m_type==2) {
            PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(5));
        }else{
            PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(4));
        }
    }else if(isTouchInside(m_btn2, pTouch) && m_btnNode->isVisible()){
        CCLog("m_btn2");
        if (m_type==2) {
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->ActivityController::getInstance()->getCkfKingdomReward(),4));
        }else{
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->ActivityController::getInstance()->getCkfMemberReward(),5));
        }

    }else if(isTouchInside(m_clickNode, pTouch)){
        if (m_type!=2) {
            string uid = m_info->valueForKey("uid")->getCString();
            RoleInfoView::createInfoByUid(uid);
        }
    }
}

bool ActivityRankCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_top3Node", CCNode*, this->m_top3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr", CCSprite*, this->m_numspr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt2", CCLabelIF*, this->m_nameTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTxt", CCLabelIF*, this->m_sTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreTxt2", CCLabelIF*, this->m_scoreTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_normalNode", CCNode*, this->m_normalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreTxt1", CCLabelIF*, this->m_scoreTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt1", CCLabelIF*, this->m_nameTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniNode", CCNode*, this->m_aniNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeBg1", CCNode*, this->m_nodeBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeBg2", CCNode*, this->m_nodeBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    return false;
}

SEL_CCControlHandler ActivityRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickHandle", ActivityRankCell::clickHandle);
    return NULL;
}

//
RankCellAni* RankCellAni::create(int rank)
{
    RankCellAni *pRet = new RankCellAni(rank);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool RankCellAni::init()
{
    CCBLoadFile("RankCellAni",this,this);
    m_ani1->setVisible(m_rank==0);
    m_ani2->setVisible(m_rank==1);
    m_ani3->setVisible(m_rank==2);
    auto particle = ParticleController::createParticle("ActivityRankCell");
    particle->setPositionX(320);
    this->addChild(particle);
    
    return true;
}

void RankCellAni::onEnter() {
    CCNode::onEnter();
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
}
void RankCellAni::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler RankCellAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool RankCellAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ani1", CCNode*, this->m_ani1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ani2", CCNode*, this->m_ani2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ani3", CCNode*, this->m_ani3);
    return false;
}

//
TargetRewardCell *TargetRewardCell::create(CCDictionary* info,CCNode* clickArea){
    TargetRewardCell *ret = new TargetRewardCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TargetRewardCell::init(){
    
    m_bg = CCLoadSprite::createSprite("Items_icon_kuang.png");
    m_bg->setPosition(ccp(0,  -3.5));
    CCCommonUtils::setSpriteMaxSize(m_bg, 149*0.55,true);
    this->addChild(m_bg);
    string icon = "";
    string numStr = "";
    int type = m_info->valueForKey("type")->intValue();
    if (type == R_GOODS){
        auto value = _dict(m_info->objectForKey("value"));
        if (value){
            m_itemId = value->valueForKey("id")->intValue();
            auto &tool = ToolController::getInstance()->getToolInfoById(m_itemId);
            icon = RewardController::getInstance()->getPicByType(type, value->valueForKey("id")->intValue()).c_str();
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(tool.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 82,true);
            iconBg->setPosition(ccp(-249.5, -3.5));
            numStr = value->valueForKey("num")->getCString();
        }
    }else{
        icon = RewardController::getInstance()->getPicByType(type,0);
        numStr = m_info->valueForKey("value")->getCString();
    }
    auto pic = CCLoadSprite::createSprite(icon.c_str());
    pic->setPosition(ccp(0,  3));
    CCCommonUtils::setSpriteMaxSize(pic, 62,true);
    this->addChild(pic);
    
    auto txtBg = CCLoadSprite::createScale9Sprite("technology_11.png");//CCLoadSprite::createScale9Sprite("black_frame.png");
    txtBg->setInsetBottom(5);
    txtBg->setInsetTop(5);
    txtBg->setInsetLeft(5);
    txtBg->setInsetRight(5);
    txtBg->setContentSize(CCSize(85,22));
    txtBg->setPosition(ccp(0,  -32));
    this->addChild(txtBg);
    
    CCLabelIF* label = CCLabelIF::create("",20);
    label->setAlignment(kCCTextAlignmentRight);
    label->setString(numStr);
    label->setAnchorPoint(ccp(1, 0));
    label->setPosition(ccp(40,  -41));
    this->addChild(label);
    
    return true;
}


void TargetRewardCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void TargetRewardCell::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool TargetRewardCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_clickArea && m_clickArea->isVisible() && isTouchInside(m_clickArea, pTouch)) {
        m_startPoint=pTouch->getStartLocation();
        if(isTouchInside(m_bg,pTouch)){
            this->unschedule(schedule_selector(TargetRewardCell::setLeft));
            this->schedule(schedule_selector(TargetRewardCell::setLeft),0.2);
        }
        return true;
    }
    return false;
}

void TargetRewardCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    this->unschedule(schedule_selector(TargetRewardCell::setLeft));
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->removeAllChildren();
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
}

void TargetRewardCell::setLeft(float _time){
    this->unschedule(schedule_selector(TargetRewardCell::setLeft));
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    string name = info.getName();
    string desc = _lang(info.des);
    if(layer->getChildrenCount()<=0 && name!=""){
        CCNode* node = CCNode::create();
        if (m_startPoint.x>320) {
            m_startPoint.x = m_startPoint.x - 180;
        }else{
            m_startPoint.x = m_startPoint.x + 180;
        }
        m_startPoint.y = m_startPoint.y - 70;
        node->setPosition(m_startPoint);
        layer->addChild(node);
        auto bg = CCLoadSprite::createScale9Sprite("Items_tips2.png");
        bg->setContentSize(CCSize(295,149));
        node->addChild(bg);
        
        auto line = CCLoadSprite::createScale9Sprite("Items_tips3.png");
        line->setPositionY(35);
        line->setContentSize(CCSize(278,2));
        node->addChild(line);
        
        CCLabelIF* label = CCLabelIF::create("",20);
        label->setAlignment(kCCTextAlignmentRight);
        label->setString(name);
        label->setColor({234,207,164});
        label->setPosition(ccp(0,  56));
        node->addChild(label);
        
        CCLabelIF* label2 = CCLabelIF::create("",20);
        label2->setAlignment(kCCTextAlignmentLeft);
        label2->setDimensions(CCSize(270,80));
        label2->setString(desc);
        label2->setColor({234,207,164});
        label2->setAnchorPoint(ccp(0,1));
        label2->setPosition(ccp(-133.6,  22));
        node->addChild(label2);
    }
}
