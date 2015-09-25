//
//  AllianceActivityView.cpp
//  IF
//
//  Created by ganxiaohua on 15/2/3.
//
//

#include "AllianceActivityView.h"
#include "PopupViewController.h"
#include "ActivityTipView.h"
#include "TipsView.h"
#include "JoinAllianceView.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "AllianceWarView.h"
#include "AllianceActivityRewardView.h"
#include "HtmlLabelTFNode.h"
#include "AllianceManager.h"
#include "MonsterSiegeStartCommand.h"
#include "MonsterSiegeActivityInfoCommand.h"
#include "ActivityController.h"

AllianceActivityView *AllianceActivityView::create(ActivityEventObj* info){
    AllianceActivityView *ret = new AllianceActivityView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceActivityView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        this->setModelLayerOpacity(160);
        auto cf = CCLoadSprite::getSF("Ativity_iconLogo_3.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(500, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(500, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
            });
        }
        auto node = CCBLoadFile("AllianceActivityTipView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(node->getContentSize());
        m_titleTxt->setString(_lang("133003"));
        CCCommonUtils::setButtonTitle(m_btnCheck, _lang("133005").c_str());
        m_readyNode->setVisible(false);
        
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("133005").c_str());
        CCCommonUtils::setButtonTitle(m_btnBattle, _lang("133030").c_str());
        m_startTip->setString(_lang("133004"));
        m_startNode->setVisible(false);
        
//        m_btnCheck->setTouchPriority(Touch_Popup_Item);
//        m_btnReward->setTouchPriority(Touch_Popup_Item);
//        m_btnBattle->setTouchPriority(Touch_Popup_Item);
//        m_bDetail->setTouchPriority(Touch_Popup_Item);
//        m_bCheckReward->setTouchPriority(Touch_Popup_Item);
        m_btnCheck->setSwallowsTouches(false);
        m_btnClose->setSwallowsTouches(false);
        m_btnReward->setSwallowsTouches(false);
        m_btnBattle->setSwallowsTouches(false);
        m_bCheckReward->setSwallowsTouches(false);
        m_bDetail->setSwallowsTouches(false);
        m_infoBtn->setSwallowsTouches(false);
        
        m_callCount = 5;
        currentTime =  clock()/1000000;
        this->unschedule(schedule_selector(AllianceActivityView::scheduleHandle));
        this->schedule(schedule_selector(AllianceActivityView::scheduleHandle), 1);
        scheduleHandle(0);
        
        MonsterSiegeActivityInfoCommand* cmd = new MonsterSiegeActivityInfoCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceActivityView::getInfoCallBack), NULL));
        cmd->sendAndRelease();
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_loadingNode);
        m_state = 0;
        ret = true;
    }

    
    return ret;
}

void AllianceActivityView::getInfoCallBack(cocos2d::CCObject * param){
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    CCDictionary* dicData = _dict(result->getData());
    ActivityController::getInstance()->aActivityInfo.parse(dicData);
    this->freshView();
}

void AllianceActivityView::freshView(float _ft){
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime();
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    double rewardTime = ActivityController::getInstance()->aActivityInfo.rewardTime - GlobalData::shared()->getWorldTime() ;
    if((prevTime<=0 && ActivityController::getInstance()->aActivityInfo.siegeST<=0) || ActivityController::getInstance()->aActivityInfo.activityST<=0){//活动没有开启
        m_readyNode->setVisible(true);
        m_bgNode->setVisible(true);
        m_detailNode->setVisible(false);
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST<=0 && gapTime>0){//活动可以开启
        if(!GlobalData::shared()->playerInfo.isInAlliance()){//没有联盟的
            CCCommonUtils::setButtonTitle(m_btnBattle, _lang("133006").c_str());
        }
        m_readyNode->setVisible(false);
        m_bgNode->setVisible(false);
        m_startNode->setVisible(true);
        m_detailNode->setVisible(false);
        m_gapTime = gapTime;
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && rewardTime>0 && gapEnd<=0){//活动开始,但本次联盟活动已经结束，等发奖
        m_readyNode->setVisible(true);
        m_bgNode->setVisible(true);
        m_detailNode->setVisible(false);
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && gapTime>0 && gapEnd>0){//活动开始
        m_readyNode->setVisible(false);
        m_bgNode->setVisible(false);
        m_detailNode->setVisible(true);
        m_sNode->removeAllChildrenWithCleanup(true);
        CCCommonUtils::setButtonTitle(m_bCheckReward, _lang("133005").c_str());
        CCCommonUtils::setButtonTitle(m_bDetail, _lang("133040").c_str());
        m_bTxt1->setString(_lang_1("133035", CC_ITOA(ActivityController::getInstance()->aActivityInfo.round)));
        m_bTxt2->setString(_lang_1("133036", ""));
        string temp = "X: ";
        temp.append(CC_ITOA(ActivityController::getInstance()->aActivityInfo.posX));
        temp.append(",y: ");
        temp.append(CC_ITOA(ActivityController::getInstance()->aActivityInfo.posY));
        m_bTxt3->setString(temp);
        monsterMap.clear();
        int totalH = m_bTxt2->getPositionY() - 15;
        CCArray* arr = ActivityController::getInstance()->aActivityInfo.getMonsterInfo();
        if (arr && arr->count()>0) {
            int mCount = arr->count();
            CCArray* mArr = CCArray::create();
            for (int i=0; i<mCount; i++) {
                auto dic = _dict(arr->objectAtIndex(i));
                string monsterId = dic->valueForKey("monsterId")->getCString();
                string name = CCCommonUtils::getNameById(monsterId);
                int count = dic->valueForKey("count")->intValue();
                std::map<std::string,int>::iterator mIt = monsterMap.find(name);
                if(mIt != monsterMap.end()){
                    monsterMap[name] = monsterMap[name]+count;
                }else{
                    monsterMap[name] = count;
                }
            }
            std::map<std::string,int>::iterator mIt;
            for (mIt = monsterMap.begin(); mIt!=monsterMap.end(); mIt++) {
                string name = mIt->first;
                int count = mIt->second;
                totalH -= 30;
                CCLabelIF* txt1 = CCLabelIF::create("", 20);
                txt1->setColor({178,178,178});
                txt1->setAnchorPoint(m_bTxt2->getAnchorPoint());
                txt1->setString(name);
                txt1->setPosition(ccp(m_bTxt2->getPositionX(), totalH));
                m_sNode->addChild(txt1);
                
                string temp = "X ";
                temp.append(CC_ITOA(count));
                CCLabelIF* txt2 = CCLabelIF::create("", 20);
                txt2->setColor({236,68,58});
                txt2->setAnchorPoint(ccp(1,0));
                txt2->setAlignment(kCCTextAlignmentLeft);
                txt2->setString(temp);
                txt2->setPosition(ccp(m_bTxt3->getPositionX(), totalH));
                m_sNode->addChild(txt2);
            }
        }
    }else{
        m_readyNode->setVisible(true);
        m_bgNode->setVisible(true);
        m_detailNode->setVisible(false);
    }
    scheduleHandle(0);
}

void AllianceActivityView::scheduleHandle(float _time){
    string temp = "";
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    double rewardTime = ActivityController::getInstance()->aActivityInfo.rewardTime - GlobalData::shared()->getWorldTime();
    unsigned long cTime = clock()/1000000;
    unsigned long cGap = cTime - currentTime;
    if(prevTime>0 && prevTime<3 && cGap>=1 && m_state!=1){
        currentTime = cTime;
        m_state = 1;
    }else if(gapTime>=-2 && gapTime<0 && cGap>=1 && m_state!=5){
        currentTime = cTime;
        m_state = 5;
    }
    if (m_state==1 || m_state==5) {
        this->scheduleOnce(schedule_selector(AllianceActivityView::freshView), 0.4);
    }
    if(ActivityController::getInstance()->aActivityInfo.activityST<=0){
         m_readyTime->setString(_lang("105800"));
         m_readyTip->setString(_lang("133004"));
         m_titleTxt->setString(_lang("133003"));
    }else if(prevTime<=0 && ActivityController::getInstance()->aActivityInfo.siegeST<=0){//活动没有开启
        m_readyTime->setString(_lang_1("105804",CC_SECTOA(-prevTime)));
        m_readyTip->setString(_lang("133004"));
        m_titleTxt->setString(_lang("133003"));
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && rewardTime>=0 && gapEnd<=0){//活动开始,但本联盟活动已经结束，等发奖
        m_readyTime->setString(_lang_1("133082",CC_SECTOA(rewardTime)));
        if(ActivityController::getInstance()->aActivityInfo.userState==2){
            m_readyTip->setString(_lang("133090"));
        }else if(ActivityController::getInstance()->aActivityInfo.userState==0){
            m_readyTip->setString(_lang("133091"));
        }else{
            m_readyTip->setString(_lang("133081"));
        }
        if(gapEnd<=0){
            m_titleTxt->setString(_lang("133003"));
        }else if (gapTime>0) {
            m_titleTxt->setString(_lang_1("105805",CC_SECTOA(gapTime)));
        }else{
            m_titleTxt->setString(_lang("133083"));
        }
        if (rewardTime==0) {
            //发奖结束，重新刷一下
            this->scheduleOnce(schedule_selector(AllianceActivityView::delayCall), 0.8);
        }
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST<=0 && gapTime>0){//活动可以开启
        m_startTxt->setString(_lang_1("105805",CC_SECTOA(gapTime)));
    }else if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && gapTime>0){//活动开始
        m_titleTxt->setString(_lang_1("133033",CC_SECTOA(gapEnd)));
        auto gtime = ActivityController::getInstance()->aActivityInfo.nextRoundTime - GlobalData::shared()->getWorldTime();
        temp = "";
        if(gtime<=0){
            temp = "";//
            if(gapEnd>0 && ActivityController::getInstance()->aActivityInfo.nextRoundTime!=0 && m_callCount>0 && cGap>=2){//活动没有结束，但到下一波怪出怪了，所以再请求下下,波怪的时间  m_callCount 是最多在打开面板后调10次
                m_callCount -= 1;
                currentTime = cTime;
                MonsterSiegeActivityInfoCommand* cmd = new MonsterSiegeActivityInfoCommand();
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceActivityView::getInfoCallBack), NULL));
                cmd->sendAndRelease();
            }
        }else{
            temp = _lang("133037");
            temp.append(_lang_1("133038", CC_SECTOA(gtime)));
        }
        m_bTxt4->setString(temp);
        m_bTxt5->setString(_lang("133034"));
    }else{
        m_readyTip->setString("");
        m_readyTime->setString(_lang("133081"));
        m_titleTxt->setString(_lang("133003"));
    }
}

void AllianceActivityView::delayCall(float _time){
    MonsterSiegeActivityInfoCommand* cmd = new MonsterSiegeActivityInfoCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceActivityView::getInfoCallBack), NULL));
    cmd->sendAndRelease();
}

void AllianceActivityView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   // //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void AllianceActivityView::onExit(){
    GameController::getInstance()->removeWaitInterface();
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceActivityView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceActivityView::onCheckClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    //PopupViewController::getInstance()->addPopupView(ActivityTipView::create(m_info));
    PopupViewController::getInstance()->addPopupView(AllianceActivityRewardView::create());
}

void AllianceActivityView::onCheckReward(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(AllianceActivityRewardView::create());
}

void AllianceActivityView::onBattleDetail(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(AllianceWarView::create(1));
}

void AllianceActivityView::onBattleClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        if (GlobalData::shared()->playerInfo.allianceInfo.rank>=4) {//AllianceManager::getInstance()->checkRight(OPEN_ALLIANCE_ACTIVITY, GlobalData::shared()->playerInfo.allianceInfo.rank
            YesNoDialog* dialog = YesNoDialog::show(_lang("133273").c_str(), CCCallFunc::create(this, callfunc_selector(AllianceActivityView::confirmFun)));
        }else{
           PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("133031"),kCCTextAlignmentLeft));
        }
    }else{
        PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
        PopupViewController::getInstance()->removePopupView(this);
    }
    
}

void AllianceActivityView::confirmFun(){
    MonsterSiegeStartCommand* cmd = new MonsterSiegeStartCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceActivityView::battleCMDCallBack), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void AllianceActivityView::battleCMDCallBack(cocos2d::CCObject * param){
    CCLOG("battleCMDCallBack");
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    CCDictionary* dicData = _dict(result->getData());
    if(dicData && dicData->objectForKey("pointId")){
        int m_worldIndex = dicData->valueForKey("pointId")->intValue();
        CCPoint pt = WorldController::getPointByIndex(m_worldIndex);
        PopupViewController::getInstance()->removeAllPopupView();
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance()){
            WorldMapView::instance()->gotoTilePoint(pt);
            WorldMapView::instance()->openTilePanel(m_worldIndex);
        }else{
            WorldController::getInstance()->openTargetIndex = m_worldIndex;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_worldIndex);
        }
    }else{
        CCLog("MonsterSiegeStartCommand error");
    }
}

bool AllianceActivityView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_btnCheck, pTouch) && m_readyNode->isVisible()){
        m_btnCheck->setHighlighted(true);
    }else if(isTouchInside(m_btnReward, pTouch) && m_startNode->isVisible()){
        m_btnReward->setHighlighted(true);
    }else if(isTouchInside(m_btnBattle, pTouch) && m_startNode->isVisible()){
        m_btnBattle->setHighlighted(true);
    }else if(isTouchInside(m_bDetail, pTouch) && m_detailNode->isVisible()){
        m_bDetail->setHighlighted(true);
    }else if(isTouchInside(m_bCheckReward, pTouch) && m_detailNode->isVisible()){
        m_bCheckReward->setHighlighted(true);
    }
    return true;
}

void AllianceActivityView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_btnCheck->setHighlighted(false);
    m_btnReward->setHighlighted(false);
    m_btnBattle->setHighlighted(false);
    m_bDetail->setHighlighted(false);
    m_bCheckReward->setHighlighted(false);
    
    if(!isTouchInside(m_clickArea, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }else if(isTouchInside(m_infoBtn, pTouch)){
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("133092"),kCCTextAlignmentLeft));
    }else{
        if(ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<30){
            if(isTouchInside(m_btnClose, pTouch)){
                onCloseClick(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_btnCheck, pTouch) && m_readyNode->isVisible()){
                onCheckClick(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_btnReward, pTouch) && m_startNode->isVisible()){
                onCheckReward(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_btnBattle, pTouch) && m_startNode->isVisible()){
                onBattleClick(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_bDetail, pTouch) && m_detailNode->isVisible()){
                onBattleDetail(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_bCheckReward, pTouch) && m_detailNode->isVisible()){
                onCheckReward(NULL,CCControlEventTouchUpInside);
            }else if(isTouchInside(m_coordinateNode, pTouch) && m_detailNode->isVisible()){
                m_coordinateNode->stopAllActions();
                CCFadeTo* fade = CCFadeTo::create(0.25, 110);
                CCFadeTo* fade4 = CCFadeTo::create(0.2, 2);
                CCDelayTime* de = CCDelayTime::create(0.05);
                CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(AllianceActivityView::goToWorld));
                CCSequence* sc = CCSequence::create(fade,fade4,de,fun,NULL);
                m_coordinateNode->runAction(sc);
            }
        }
    }
}

void AllianceActivityView::goToWorld(){
    int m_worldIndex = WorldController::getIndexByPoint(ccp(ActivityController::getInstance()->aActivityInfo.posX, ActivityController::getInstance()->aActivityInfo.posY));
    CCPoint pt = WorldController::getPointByIndex(m_worldIndex);
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance()){
        WorldMapView::instance()->gotoTilePoint(pt);
        WorldMapView::instance()->openTilePanel(m_worldIndex);
    }else{
        WorldController::getInstance()->openTargetIndex = m_worldIndex;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_worldIndex);
    }
}

void AllianceActivityView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_btnCheck, pTouch) && m_readyNode->isVisible()){
        m_btnCheck->setHighlighted(true);
    }else if(isTouchInside(m_btnReward, pTouch) && m_startNode->isVisible()){
        m_btnReward->setHighlighted(true);
    }else if(isTouchInside(m_btnBattle, pTouch) && m_startNode->isVisible()){
        m_btnBattle->setHighlighted(true);
    }else if(isTouchInside(m_bDetail, pTouch) && m_detailNode->isVisible()){
        m_bDetail->setHighlighted(true);
    }else if(isTouchInside(m_bCheckReward, pTouch) && m_detailNode->isVisible()){
        m_bCheckReward->setHighlighted(true);
    }
}

SEL_CCControlHandler AllianceActivityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", AllianceActivityView::onCloseClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCheckClick", AllianceActivityView::onCheckClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCheckReward", AllianceActivityView::onCheckReward);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBattleClick", AllianceActivityView::onBattleClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBattleDetail", AllianceActivityView::onBattleDetail);
    return NULL;
}

bool AllianceActivityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyNode", CCNode*, this->m_readyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCheck", CCControlButton*, this->m_btnCheck);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyTime", CCLabelIF*, this->m_readyTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_readyTip", CCLabelIF*, this->m_readyTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnClose", CCControlButton*, this->m_btnClose);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startNode", CCNode*, this->m_startNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnBattle", CCControlButton*, this->m_btnBattle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startTxt", CCLabelIF*, this->m_startTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startTip", CCLabelIF*, this->m_startTip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_detailNode", CCNode*, this->m_detailNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bCheckReward", CCControlButton*, this->m_bCheckReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bDetail", CCControlButton*, this->m_bDetail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt1", CCLabelIF*, this->m_bTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt2", CCLabelIF*, this->m_bTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt3", CCLabelIF*, this->m_bTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt4", CCLabelIF*, this->m_bTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt5", CCLabelIF*, this->m_bTxt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sNode", CCNode*, this->m_sNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordinateNode", CCNode*, this->m_coordinateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingNode", CCNode*, this->m_loadingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn", CCControlButton*, this->m_infoBtn);
    return false;
}

