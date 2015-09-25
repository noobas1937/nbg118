//
//  AllianceActivityRewardView.cpp
//  IF
//
//  Created by ganxiaohua on 15/3/2.
//
//

#include "AllianceActivityRewardView.h"
#include "PopupViewController.h"
#include "ActivityTipView.h"
#include "TipsView.h"
#include "JoinAllianceView.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "AllianceWarView.h"
#include "RewardController.h"
#include "MonsterSiegeRewardInfoCommand.h"
#include "ActivityController.h"
#include "ActivityRewardView.h"
#include "ActivityHistoryView.h"

static const char* roman[30] = {"I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII","XIII","XIV","XV","XVI","XVII","XVIII","XIX","XX", "XXI","XXII","XXIII","XXIV","XXV","XXVI","XXVII","XXVIII","XXIX","XXX"};
static const cocos2d::ccColor3B COLOR_TAB_NORMAL = {140,154,166};
static const cocos2d::ccColor3B COLOR_TAB_SELECTED = {213,233,247};

AllianceActivityRewardView *AllianceActivityRewardView::create(){
    AllianceActivityRewardView *ret = new AllianceActivityRewardView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceActivityRewardView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto node = CCBLoadFile("AllianceActivityRewardView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(node->getContentSize());
        m_titleTxt->setString(_lang("133003"));
        
        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(0);
        m_scNode->addChild(m_scrollView);
        
        m_tab1Txt1->setString(_lang("133017"));
        m_tab2Txt1->setString(_lang("133009"));
        m_tab3Txt1->setString(_lang("133063"));
        m_tipTxt->setString(_lang("133029"));
        
        m_tabNode1->removeFromParent();
        m_tabNode1->setPosition(ccp(275, -200 + 570));
        m_scrollView->addChild(m_tabNode1);

        m_btnClose->setTouchPriority(0);
        m_rewardBtn->setTouchPriority(2);
        m_playerBtn->setTouchPriority(2);
        m_allianceBtn->setTouchPriority(2);
        m_infoBtn->setSwallowsTouches(false);
        m_rewardBtn->setSwallowsTouches(false);
        m_playerBtn->setSwallowsTouches(false);
        m_allianceBtn->setSwallowsTouches(false);
        
        m_btnNode->removeFromParent();
        CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("105823").c_str());
        CCCommonUtils::setButtonTitle(m_playerBtn, _lang("133085").c_str());
        CCCommonUtils::setButtonTitle(m_allianceBtn, _lang("133086").c_str());
        
        
        this->schedule(schedule_selector(AllianceActivityRewardView::scheduleHandle),1);
        m_haveTime = 100000;
        scheduleHandle(0);
        if(ActivityController::getInstance()->allianceKill && ActivityController::getInstance()->allianceKill->count()<=0){
            MonsterSiegeRewardInfoCommand* cmd = new MonsterSiegeRewardInfoCommand();
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceActivityRewardView::callBack), NULL));
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
        }else{
            showRewardByTab(1);
        }
        ret = true;
    }
    
    
    return ret;
}

void AllianceActivityRewardView::callBack(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    CCLOG("MonsterSiegeRewardInfoCommand");
    NetResult* result = dynamic_cast<NetResult*>(param);
    CCDictionary* dicData = _dict(result->getData());
    if(dicData){
        if(dicData->objectForKey("msReward")){
            CCDictionary* msReward = _dict(dicData->objectForKey("msReward"));
            if(msReward){
                CCArray* allianceKill = dynamic_cast<CCArray*>(msReward->objectForKey("allianceKill"));
                CCArray* gole = dynamic_cast<CCArray*>(msReward->objectForKey("gole"));
                CCArray* memberKill = dynamic_cast<CCArray*>(msReward->objectForKey("memberKill"));
                if(allianceKill){
                    ActivityController::getInstance()->allianceKill->addObjectsFromArray(allianceKill);
                }
                if(gole){
                    ActivityController::getInstance()->gole->addObjectsFromArray(gole);
                }
                if(memberKill){
                    ActivityController::getInstance()->memberKill->addObjectsFromArray(memberKill);
                }
            }
        }
    }
    showRewardByTab(1);
    scheduleHandle(0);
}

void AllianceActivityRewardView::scheduleHandle(float _time){
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    double rewardTime = ActivityController::getInstance()->aActivityInfo.rewardTime - GlobalData::shared()->getWorldTime();
    if(ActivityController::getInstance()->aActivityInfo.activityST<=0){
        m_timeTxt->setString(_lang("105800"));
    }else if(prevTime<=0 && ActivityController::getInstance()->aActivityInfo.siegeST<=0){//活动没有开启
        m_timeTxt->setString(_lang_1("105804",CC_SECTOA(-prevTime)));
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST<=0 && gapTime>0){//活动可以开启
        m_timeTxt->setString(_lang_1("105805",CC_SECTOA(gapTime)));
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && rewardTime>=0 && gapEnd<=0){//活动开始,但本联盟活动已经结束，等发奖
        if (rewardTime>=0) {
            m_timeTxt->setString(_lang_1("133082",CC_SECTOA(rewardTime)));
        }else{
            m_timeTxt->setString(_lang("133003"));
        }
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && gapTime>0){//活动开始
        m_timeTxt->setString(_lang_1("105805",CC_SECTOA(gapTime)));
    }else{
        m_timeTxt->setString(_lang("133083"));
    }
}

void AllianceActivityRewardView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void AllianceActivityRewardView::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceActivityRewardView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
}

bool AllianceActivityRewardView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void AllianceActivityRewardView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_tab1, pTouch)){
        showRewardByTab(1);
    }else if (isTouchInside(m_tab2, pTouch)) {
        showRewardByTab(2);
    }else if (isTouchInside(m_tab3, pTouch)) {
        showRewardByTab(3);
    }else if(isTouchInside(m_scNode, pTouch) && isTouchInside(m_infoBtn, pTouch)){
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("133015"),kCCTextAlignmentLeft));
    }else if(!isTouchInside(m_clickArea, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        PopupViewController::getInstance()->removePopupView(this);
    }else if(m_btnNode->getParent() && isTouchInside(m_rewardBtn, pTouch) && isTouchInside(m_clickArea, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        if (m_tap==2) {
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->allianceKill,2));
        }else{
            PopupViewController::getInstance()->addPopupView(ActivityRewardView::create(ActivityController::getInstance()->memberKill,3));
        }
    }else if(m_btnNode->getParent() && isTouchInside(m_playerBtn, pTouch) && isTouchInside(m_clickArea, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(2));
    }else if(m_btnNode->getParent() && isTouchInside(m_allianceBtn, pTouch) && isTouchInside(m_clickArea, pTouch) && ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
        PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(3));
    }
    
}

void AllianceActivityRewardView::showRewardByTab(int tab){
    m_tap = tab;
    string strTemp = "";
    m_scrollView->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->getContainer()->unscheduleAllSelectors();
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,570));
    m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 570));
    int rank = 0;
    m_btnNode->removeFromParent();
    if(m_tap==1){
        int myKill = ActivityController::getInstance()->aActivityInfo.userKill;
        int aKill = ActivityController::getInstance()->aActivityInfo.allKill;
        m_tab1Txt1->setColor(COLOR_TAB_SELECTED);
        m_tab2Txt1->setColor(COLOR_TAB_NORMAL);
        m_tab3Txt1->setColor(COLOR_TAB_NORMAL);
        m_tipTxt1->setString(_lang("133017"));
        strTemp = _lang("133007");
        strTemp.append(" ");
        strTemp.append(CC_CMDITOA(myKill));
        m_tipTxt2->setString(strTemp);
        strTemp = _lang("133008");
        strTemp.append(" ");
        strTemp.append(CC_CMDITOA(aKill));
        m_tipTxt3->setString(strTemp);
        m_tipTxt4->setString("");
        m_tabRewardNode1->removeAllChildrenWithCleanup(true);
        int totalH = 60;
        int num = ActivityController::getInstance()->gole->count();
        for(int i=0;i<num;i++){
            CCDictionary* oneRank = _dict(ActivityController::getInstance()->gole->objectAtIndex(i));
            int tempKill = oneRank->valueForKey("memberKill")->intValue();
            int tempAKill = oneRank->valueForKey("allianceKill")->intValue();
            string temp = "";
            rank = oneRank->valueForKey("rank")->intValue();
            if(rank<30 && rank>=0){
                temp.append(roman[rank]);
            }else{
                temp.append(CC_ITOA(rank+1));
            }
            CCLabelIF* txt = CCLabelIF::create("", 20);
            txt->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txt->setPosition(ccp(0, totalH));
            txt->setColor({108,255,253});
            txt->setString(_lang_1("133019",temp.c_str()));
            m_tabRewardNode1->addChild(txt);
            
            totalH -= 30;
            CCLabelIF* txt1 = CCLabelIF::create("", 20);
            txt1->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txt1->setPosition(ccp(0, totalH));
            txt1->setAlignment(kCCTextAlignmentLeft);
            txt1->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            temp = _lang("133018");
            temp.append(" ");
            temp.append(CC_ITOA(tempKill));
            txt1->setString(temp);
            CCSprite* spr = NULL;
            bool getFlag = true;
            if (myKill>=tempKill) {
                txt1->setColor({255,255,255});
                spr = CCLoadSprite::createSprite("green_yes.png");
            }else{
                getFlag = false;
                txt1->setColor({255,0,0});
                spr = CCLoadSprite::createSprite("red_no.png");
            }
            m_tabRewardNode1->addChild(txt1);
            spr->setScale(0.6);
            int sprX = MIN(txt1->getContentSize().width+30,520);
            spr->setPosition(ccp(sprX, totalH+10));
            m_tabRewardNode1->addChild(spr);
            
            totalH -= 30;
            CCLabelIF* txt2 = CCLabelIF::create("", 20);
            txt2->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txt2->setPosition(ccp(0, totalH));
            temp = _lang("133027");
            temp.append(" ");
            temp.append(CC_ITOA(tempAKill));
            txt2->setString(temp);
            if (aKill>=tempAKill) {
                txt2->setColor({255,255,255});
                spr = CCLoadSprite::createSprite("green_yes.png");
            }else{
                getFlag = false;
                txt2->setColor({255,0,0});
                spr = CCLoadSprite::createSprite("red_no.png");
            }
            m_tabRewardNode1->addChild(txt2);
            spr->setScale(0.6);
            sprX = MIN(txt2->getContentSize().width+30,520);
            spr->setPosition(ccp(sprX, totalH+10));
            m_tabRewardNode1->addChild(spr);
            
            CCLabelIF* txtGet = CCLabelIF::create("", 20);
            txtGet->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txtGet->setAlignment(kCCTextAlignmentLeft);
            int offX = MIN(txt->getContentSize().width+30,350);
            txtGet->setPosition(ccp(offX, txt->getPositionY()));
            txtGet->setColor({108,255,253});
            m_tabRewardNode1->addChild(txtGet);
            if(getFlag){
                txtGet->setString(_lang("133087"));
            }else{
                txtGet->setString(_lang("133088"));
            }
            totalH -= 90;
            CCArray* arr = dynamic_cast<CCArray*>(oneRank->objectForKey("reward"));
            if(arr && arr->count()>0){
                int numReward = arr->count();
                CCScale9Sprite* rewardBg = CCLoadSprite::createScale9Sprite("Activity_down-1.png");
                rewardBg->setAnchorPoint(ccp(0, 1));
                rewardBg->setContentSize(CCSize(543,84*(numReward)));
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
        totalH = -totalH + 300;
        m_tabNode1->setPosition(ccp(275, -200 + totalH));
        m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,totalH));
        m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - totalH));
        m_selectBg->setPosition(ccp(-182.9,243));
        
    }else if(m_tap==2){
        m_tab1Txt1->setColor(COLOR_TAB_NORMAL);
        m_tab2Txt1->setColor(COLOR_TAB_SELECTED);
        m_tab3Txt1->setColor(COLOR_TAB_NORMAL);
        m_tipTxt1->setString(_lang("133026"));
        strTemp = _lang("133008");
        strTemp.append(" ");
        strTemp.append(CC_CMDITOA(ActivityController::getInstance()->aActivityInfo.allKill));
        m_tipTxt2->setString(strTemp);
        strTemp = _lang("133028");
        strTemp.append(" ");
        if(ActivityController::getInstance()->aActivityInfo.allRank<=0){
            double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
            if (prevTime<=0) {
                strTemp.append(_lang("133089"));
            }else{
                strTemp.append("100+");
            }
        }else{
            strTemp.append(CC_CMDITOA(ActivityController::getInstance()->aActivityInfo.allRank));
        }
        m_tipTxt3->setString(strTemp);
        m_tipTxt4->setString("");
        m_tabRewardNode1->removeAllChildrenWithCleanup(true);
        int num = ActivityController::getInstance()->allianceKill->count();
        if(num>1) num = 1;
        int totalH = 60;
        for(int i=0;i<num;i++){
            CCDictionary* oneRank = _dict(ActivityController::getInstance()->allianceKill->objectAtIndex(i));
            string temp = _lang("133009");
            temp.append(" ");
            //temp.append(roman[i]);
            rank = oneRank->valueForKey("rank")->intValue();
//            if(rank<20 && rank>=0){
//                temp.append(roman[rank]);
//            }else{
//                temp.append(CC_ITOA(rank+1));
//            }
            temp.append(CC_ITOA(rank+1));
            CCLabelIF* txt = CCLabelIF::create("", 20);
            txt->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txt->setPosition(ccp(0, totalH));
            txt->setColor({108,255,253});
            txt->setString(temp);
            m_tabRewardNode1->addChild(txt);
            
            totalH -= 90;
            CCArray* arr = dynamic_cast<CCArray*>(oneRank->objectForKey("reward"));
            if(arr && arr->count()>0){
                int numReward = arr->count();
                CCScale9Sprite* rewardBg = CCLoadSprite::createScale9Sprite("Activity_down-1.png");
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
        
        m_btnNode->setPosition(ccp(267.0, totalH));
        m_tabRewardNode1->addChild(m_btnNode);
        totalH -= 300;
        
        totalH = -totalH + 300;
        m_tabNode1->setPosition(ccp(275, -200 + totalH));
        m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,totalH));
        m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - totalH));
        m_selectBg->setPosition(ccp(3.0,243));
    }else{
        m_tab1Txt1->setColor(COLOR_TAB_NORMAL);
        m_tab2Txt1->setColor(COLOR_TAB_NORMAL);
        m_tab3Txt1->setColor(COLOR_TAB_SELECTED);
        m_tipTxt1->setString(_lang("133062"));
        strTemp = _lang("133007");
        strTemp.append(" ");
        strTemp.append(CC_CMDITOA(ActivityController::getInstance()->aActivityInfo.userKill));
        m_tipTxt2->setString(strTemp);
        strTemp = _lang("133010");
        strTemp.append(" ");
        if(ActivityController::getInstance()->aActivityInfo.userRank<=0){
            double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
            if (prevTime<=0) {
                strTemp.append(_lang("133089"));
            }else{
                strTemp.append("5000+");
            }
        }else{
            strTemp.append(CC_CMDITOA(ActivityController::getInstance()->aActivityInfo.userRank));
        }
        m_tipTxt3->setString(strTemp);
        m_tipTxt4->setString("");
        m_tabRewardNode1->removeAllChildrenWithCleanup(true);
        int totalH = 60;
        int num = ActivityController::getInstance()->memberKill->count();
        if(num>1) num = 1;
        for(int i=0;i<num;i++){
            CCDictionary* oneRank = _dict(ActivityController::getInstance()->memberKill->objectAtIndex(i));
            int tempRank = oneRank->valueForKey("rank")->intValue();
            string temp = _lang("133011");
            temp.append(" ");
            temp.append(CC_ITOA(tempRank+1));
            CCLabelIF* txt = CCLabelIF::create("", 20);
            txt->setAnchorPoint(m_tipTxt4->getAnchorPoint());
            txt->setPosition(ccp(0, totalH));
            txt->setColor({108,255,253});
            txt->setString(temp);
            m_tabRewardNode1->addChild(txt);
            
            totalH -= 90;
            CCArray* arr = dynamic_cast<CCArray*>(oneRank->objectForKey("reward"));
            if(arr && arr->count()>0){
                int numReward = arr->count();
                CCScale9Sprite* rewardBg = CCLoadSprite::createScale9Sprite("Activity_down-1.png");
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
        
        m_btnNode->setPosition(ccp(267.0, totalH));
        m_tabRewardNode1->addChild(m_btnNode);
        totalH -= 300;
        
        totalH = -totalH + 300;
        m_tabNode1->setPosition(ccp(275, -200 + totalH));
        m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,totalH));
        m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - totalH));
        m_selectBg->setPosition(ccp(188.6,243));
    }
}

void AllianceActivityRewardView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

SEL_CCControlHandler AllianceActivityRewardView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", AllianceActivityRewardView::onCloseClick);
    return NULL;
}

bool AllianceActivityRewardView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab1", CCNode*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab2", CCNode*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab3", CCNode*, this->m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab1Txt1", CCLabelIF*, this->m_tab1Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab2Txt1", CCLabelIF*, this->m_tab2Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab3Txt1", CCLabelIF*, this->m_tab3Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabNode1", CCNode*, this->m_tabNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabRewardNode1", CCNode*, this->m_tabRewardNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt3", CCLabelIF*, this->m_tipTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt4", CCLabelIF*, this->m_tipTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectBg", CCNode*, this->m_selectBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerBtn", CCControlButton*, this->m_playerBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBtn", CCControlButton*, this->m_allianceBtn);
    return false;
}

AllianceActivityRewardCell *AllianceActivityRewardCell::create(CCDictionary* info){
    AllianceActivityRewardCell *ret = new AllianceActivityRewardCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceActivityRewardCell::onEnter(){
    CCNode::onEnter();
}

void AllianceActivityRewardCell::onExit(){
    CCNode::onExit();
}

bool AllianceActivityRewardCell::init(){
    auto bg = CCBLoadFile("ActivityRewardCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_nameTxt->setFontSize(20);
    m_numTxt->setFontSize(20);
    m_nameTxt->setColor({213,233,247});
    m_numTxt->setColor({213,233,247});
    
    std::string name = "";
    std::string icon = "";
    int type = m_info->valueForKey("type")->intValue();
    int value = m_info->valueForKey("value")->intValue();
    if(type==R_GOODS){
        auto valueObj = _dict(m_info->objectForKey("value"));
        int id = valueObj->valueForKey("id")->intValue();
        name = RewardController::getInstance()->getNameByType(type,id);
        icon = RewardController::getInstance()->getPicByType(type,id);
        value = valueObj->valueForKey("num")->intValue();
    }else if(type==R_EFFECT){
        name = CCCommonUtils::getNameById(CC_ITOA(value));
        if (name=="") {
            switch (value) {
                case 502600:
                    name = _lang("138074");
                    break;
                case 502601:
                    name = _lang("138075");
                    break;
                case 502602:
                    name = _lang("138076");
                    break;
            }
        }
        icon = CCCommonUtils::getIcon(CC_ITOA(value));
        value = 1;
    }else{
        name = RewardController::getInstance()->getNameByType(type,value);
        icon = RewardController::getInstance()->getPicByType(type,value);
    }
    m_nameTxt->setString(name);
    name = "X";
    name.append(CC_CMDITOA(value));
    if (type==R_EFFECT) {
        name = "";
    }
    m_numTxt->setString(name);
    m_numTxt->setPositionX(m_numTxt->getPositionX()+60);
    
    CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
    CCCommonUtils::setSpriteMaxSize(spr, 45);
    m_iconNode->addChild(spr);
    return true;
}

bool AllianceActivityRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    return false;
}

SEL_CCControlHandler AllianceActivityRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
