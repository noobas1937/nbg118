//
//  CrossServiceRankView.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/5.
//
//

#include "CrossServiceRankView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "TipsView.h"
#include "CrossServiceActivityCommand.h"
#include "ActivityController.h"
#include "AllianceActivityRewardView.h"
#include "ActivityHistoryView.h"
#include "ActivityRewardView.h"
#include "FunBuildController.h"
#include "CrossServerScoreCommand.h"
#include "CKFRewardInfoCommand.h"

CrossServiceRankView *CrossServiceRankView::create(ActivityEventObj* info){
    CrossServiceRankView *ret = new CrossServiceRankView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CrossServiceRankView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void CrossServiceRankView::onExit(){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool CrossServiceRankView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCBLoadFile("CrossServiceRankView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_titleTxt->setString(m_info->name);
        m_tab1Txt1->setString(_lang("138004"));
        m_tab2Txt1->setString(_lang("138005"));
        m_tipTxt2->setString(_lang("133029"));

        CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("138016").c_str());
        CCCommonUtils::setButtonTitle(m_kingdomBtn, _lang("138015").c_str());
        CCCommonUtils::setButtonTitle(m_playerBtn, _lang("138017").c_str());
        m_rewardBtn->setTouchPriority(2);
        m_kingdomBtn->setTouchPriority(2);
        m_playerBtn->setTouchPriority(2);
        m_infoBtn->setTouchPriority(2);
        m_btnClose->setTouchPriority(2);
        
        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(0);
        m_scNode->addChild(m_scrollView);
        
        m_tabNode1->removeFromParent();
        m_tabNode1->setPosition(ccp(275, -200 + 570));
        m_scrollView->addChild(m_tabNode1);
        
        m_btnNode->removeFromParent();
        this->schedule(schedule_selector(CrossServiceRankView::updateTime),1);
        this->updateTime(0);
        m_tab1Txt1->setColor({252,192,129});
        m_tab2Txt1->setColor({128,78,35});
        
        m_tab = 0;
        m_userScore = ActivityController::getInstance()->m_userScore;
        m_userRank = ActivityController::getInstance()->m_userRank;
        m_kingdomScore = ActivityController::getInstance()->m_kingdomScore;
        m_kingdomRank = ActivityController::getInstance()->m_kingdomRank;
        m_btnClose->setSwallowsTouches(false);
        m_infoBtn->setSwallowsTouches(false);
        m_rewardBtn->setSwallowsTouches(false);
        m_kingdomBtn->setSwallowsTouches(false);
        m_playerBtn->setSwallowsTouches(false);
        if(ActivityController::getInstance()->getCkfMemberReward()->count()<=0){
            CKFRewardInfoCommand* cmd2 = new CKFRewardInfoCommand();
            cmd2->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CrossServiceRankView::getData2), NULL));
            cmd2->sendAndRelease();
            m_waitInterface = GameController::getInstance()->showWaitInterface(m_scNode);
            m_waitInterface->setPosition(m_scNode->getContentSize().width/2, m_scNode->getContentSize().height/2);
        }
        refreshData(0);
        ret = true;
    }
    return ret;
}

bool CrossServiceRankView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (!isTouchInside(m_clickArea, pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
        return false;
    }
    return true;
}

void CrossServiceRankView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_btnClose, pTouch)){
        onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_infoBtn, pTouch)){
        onInfoClick(NULL,Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_tab1, pTouch)){
        refreshData(0);
    }else if (isTouchInside(m_tab2, pTouch)) {
        refreshData(1);
    }else if(isTouchInside(m_rewardBtn, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        if (m_tab==0) {
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->getCkfKingdomReward(),4));
        }else{
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->getCkfMemberReward(),5));
        }
    }else if(isTouchInside(m_kingdomBtn, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(5));
        CCLOG("m_kingdomBtn");
    }else if(isTouchInside(m_playerBtn, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        CCLOG("m_playerBtn");
        PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(4));
    }
}

void CrossServiceRankView::refreshData(int tab){
    m_tab = tab;
    CCArray* array = NULL;
    if (m_tab==0) {
        m_tab1Txt1->setColor({252,192,129});
        m_tab2Txt1->setColor({128,78,35});
        m_txt1->setString(_lang_1("138008",CC_CMDITOAD(m_kingdomScore).c_str()));
        m_txt2->setString(_lang_1("138009",CC_CMDITOAD(m_kingdomRank).c_str()));
        m_txt3->setString(_lang_1("138010","1"));
        m_tipTxt1->setString(_lang("138006"));
        array = ActivityController::getInstance()->getCkfKingdomReward();
    }else{
        m_tab2Txt1->setColor({252,192,129});
        m_tab1Txt1->setColor({128,78,35});
        m_txt1->setString(_lang_1("138012",CC_CMDITOAD(m_userScore).c_str()));
        m_txt2->setString(_lang_1("138013",CC_CMDITOAL(m_userRank).c_str()));
        m_txt3->setString(_lang_1("138011","1"));
        m_tipTxt1->setString(_lang("138005"));
        array = ActivityController::getInstance()->getCkfMemberReward();
    }
    m_btnNode->removeFromParent();
    m_tabRewardNode1->removeAllChildrenWithCleanup(true);
    int num = array->count();
    int rank = 0;
    if(num>1) num = 1;
    int totalH = 0;
    for(int i=0;i<num;i++){
        CCDictionary* oneRank = _dict(array->objectAtIndex(i));
        string temp = _lang("133009");
        temp.append(" ");
        rank = oneRank->valueForKey("rank")->intValue();
        temp.append(CC_ITOA(rank+1));
//        CCLabelIF* txt = CCLabelIF::create("", 20);
//        txt->setAnchorPoint(ccp(0, 0.5));
//        txt->setPosition(ccp(0, totalH));
//        txt->setColor({108,255,253});
//        txt->setString(temp);
//        m_tabRewardNode1->addChild(txt);
        
        totalH = -25;
        CCArray* arr = dynamic_cast<CCArray*>(oneRank->objectForKey("reward"));
        if(arr && arr->count()>0){
            int numReward = arr->count();
            CCScale9Sprite* rewardBg = CCLoadSprite::createScale9Sprite("kuafuzhan-3.png");
            rewardBg->setAnchorPoint(ccp(0, 1));
            rewardBg->setContentSize(CCSize(543,84*numReward));
            rewardBg->setPositionY(totalH+80);
            m_tabRewardNode1->addChild(rewardBg);
            if(arr){
                for(int j=0;j<numReward;j++){
                    CCDictionary* reward = _dict(arr->objectAtIndex(j));
                    AllianceActivityRewardCell* cell = AllianceActivityRewardCell::create(reward);
                    cell->setPosition(ccp(10,totalH));
                    m_tabRewardNode1->addChild(cell);
                    totalH -= 83;
                }
            }
            totalH += 30;
        }
    }
    
    totalH -= 10;
    m_btnNode->setPosition(ccp(267.0, totalH));
    m_tabRewardNode1->addChild(m_btnNode);
    totalH -= 300;
    
    totalH = -totalH + 300;
    m_tabNode1->setPosition(ccp(275, -200 + totalH));
    m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,totalH));
    m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - totalH));
}

void CrossServiceRankView::updateTime(float _time){
    auto nowTime = GlobalData::shared()->getWorldTime();
    if (m_info->startTime<nowTime && m_info->endTime>nowTime)
    {
        double leftTime = m_info->endTime - nowTime;
        m_timeTxt->setString(_lang_1("105805", CC_SECTOA(leftTime)));
    }else if(m_info->startTime==0){
        m_timeTxt->setString(_lang("105800"));
    }else if (m_info->startTime > nowTime)
    {
        double leftTime = m_info->startTime - nowTime;
        m_timeTxt->setString(_lang_1("105804", CC_SECTOA(leftTime)));
    }else{
        m_timeTxt->setString(_lang("133083"));
    }
}

void CrossServiceRankView::getData2(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
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
    refreshData(m_tab);
}

void CrossServiceRankView::onCheckClick(CCObject *pSender, CCControlEvent event){
    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
}

void CrossServiceRankView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void CrossServiceRankView::onInfoClick(CCObject *pSender, CCControlEvent event){
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

bool CrossServiceRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1Txt1", CCLabelIF*, this->m_tab1Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2Txt1", CCLabelIF*, this->m_tab2Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1", CCNode*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2", CCNode*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingdomBtn", CCControlButton*, this->m_kingdomBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerBtn", CCControlButton*, this->m_playerBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabRewardNode1", CCNode*, this->m_tabRewardNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tabNode1", CCNode*, this->m_tabNode1);
    return false;
}

SEL_CCControlHandler CrossServiceRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

    return NULL;
}