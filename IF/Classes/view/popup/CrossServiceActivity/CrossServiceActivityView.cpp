//
//  CrossServiceActivityView.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/4.
//
//

#include "CrossServiceActivityView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "TipsView.h"
#include "CrossServiceActivityCommand.h"
#include "CrossServiceRankView.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "CrossServiceTargetView.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ActivityController.h"
#include "ActivityRewardView.h"
#include "CSALeaveCommand.h"
#include "QueueController.h"
#include "CrossServerScoreCommand.h"
#include "ArmyController.h"
#include "BuildingHospitalPopUpView.h"
#include "QuestController.h"

CrossServiceActivityView *CrossServiceActivityView::create(ActivityEventObj* info){
    CrossServiceActivityView *ret = new CrossServiceActivityView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CrossServiceActivityView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void CrossServiceActivityView::onExit(){
    setTouchEnabled(false);
    GameController::getInstance()->removeWaitInterface();
    PopupBaseView::onExit();
}

bool CrossServiceActivityView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CrossServiceActivityView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    auto view = PopupViewController::getInstance()->getCurrentPopupView();
    if(!isTouchInside(m_clickArea, pTouch) && view==this){
        this->closeSelf();
    }
}

bool CrossServiceActivityView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto cf = CCLoadSprite::getSF("Ativity_iconLogo_3.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(500, true);
            CCLoadSprite::doResourceByCommonIndex(504, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            CCLoadSprite::doResourceByCommonIndex(7, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(500, false);
                CCLoadSprite::doResourceByCommonIndex(504, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
                CCLoadSprite::doResourceByCommonIndex(7, false);
            });
        }else{
            CCLoadSprite::doResourceByCommonIndex(504, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(504, false);
            });
        }
        CCBLoadFile("CrossServiceActivity", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        m_titleTxt->setString(m_info->name);
        m_readyTip->setString(m_info->desc);
        if (GlobalData::shared()->firstEnterCrossService) {
            m_tipTxt->setString(_lang("138020"));
            m_icon->setVisible(false);
            m_numTxt->setVisible(false);
        }else{
            m_tipTxt->setString("");
            m_icon->setVisible(true);
            m_numTxt->setVisible(true);
        }
        string temp = "X ";
        temp.append(CC_ITOA(ActivityController::getInstance()->wb_condition_k3));
        m_numTxt->setString(temp);
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(ActivityController::getInstance()->wb_condition_k2);
        if (info.getCNT()<=0) {
            m_numTxt->setColor({255,0,0});
        }
        m_icon->removeAllChildrenWithCleanup(true);
        string iconStr = CCCommonUtils::getIcon(CC_ITOA(ActivityController::getInstance()->wb_condition_k2));
        auto itemIcon = CCLoadSprite::createSprite(iconStr.c_str());
        CCCommonUtils::setSpriteMaxSize(itemIcon, 52);
        m_icon->addChild(itemIcon);
        
        m_btn1->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
        
        CCCommonUtils::setButtonTitle(m_exitBtn, _lang("138061").c_str());
        m_exitBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
        m_exitBtn->setVisible(GlobalData::shared()->isCrossService);
        
        CrossServerScoreCommand* cmd = new CrossServerScoreCommand("simple");
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CrossServiceActivityView::getData), NULL));
        cmd->sendAndRelease();
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_clickArea);
        m_waitInterface->setPosition(m_clickArea->getContentSize().width/2, m_clickArea->getContentSize().height/2);
        this->schedule(schedule_selector(CrossServiceActivityView::updateTime),1);
        this->updateTime(0);
        
        ret = true;
    }
    return ret;
}

void CrossServiceActivityView::getData(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto m_scoreInfo = _dict(result->getData());
    if (m_scoreInfo && m_scoreInfo->objectForKey("userScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("userScore"));
        ActivityController::getInstance()->m_userScore = dic->valueForKey("userScore")->doubleValue();
        ActivityController::getInstance()->m_userRank = dic->valueForKey("userRank")->intValue();
        int userMaxRank = dic->valueForKey("userMaxRank")->intValue();
        m_txt1->setString(_lang_1("138101", CC_CMDITOAL(userMaxRank).c_str()));
        m_txt3->setString(_lang_1("138034", CC_CMDITOAD(ActivityController::getInstance()->m_userScore).c_str()));
    }
    if (m_scoreInfo && m_scoreInfo->objectForKey("kingdomScore")) {
        auto dic = _dict(m_scoreInfo->objectForKey("kingdomScore"));
        ActivityController::getInstance()->m_kingdomScore = dic->valueForKey("kingdomScore")->doubleValue();
        ActivityController::getInstance()->m_kingdomRank = dic->valueForKey("kingdomRank")->intValue();
        m_txt2->setString(_lang_1("138102", CC_CMDITOAL(ActivityController::getInstance()->m_kingdomRank).c_str()));
    }
    if (ActivityController::getInstance()->m_userRank==0) {
        m_txt1->setString("");
        m_txt2->setPositionY(m_txt2->getPositionY()+15);
        m_txt3->setPositionY(m_txt3->getPositionY()+15);
    }
    auto nowTime = GlobalData::shared()->getWorldTime();
    if(ActivityController::getInstance()->m_userScore==0 && m_info->endTime>nowTime){
        m_txt3->setString(_lang("138104"));
    }else{
        if (GlobalData::shared()->isCrossService) {
            map<std::string, QuestInfo*>::iterator it;
            CCArray* temp1 = CCArray::create();
            for (it = QuestController::getInstance()->currentCKFList.begin(); it!=QuestController::getInstance()->currentCKFList.end(); it++) {
                if (it->second->type==3) {
                    if (it->second->state==COMPLETE) {
                        temp1->addObject(it->second);
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
            if (num>0) {
                QuestInfo* info = dynamic_cast<QuestInfo*>(temp1->objectAtIndex(0));
                m_txt3->setString(_lang_2("138104",CC_ITOA(info->currPhase),CC_ITOA(info->maxPhase)));
            }
        }
    }
}

void CrossServiceActivityView::updateTime(float _time){
    auto nowTime = GlobalData::shared()->getWorldTime();
    if (m_info->startTime<nowTime && m_info->endTime>nowTime)
    {
        double leftTime = m_info->endTime - nowTime;
        m_readyTime->setString(_lang_1("105805", CC_SECTOA(leftTime)));
        if (GlobalData::shared()->isCrossService) {
            CCCommonUtils::setButtonTitle(m_btn1, _lang("138003").c_str());
            m_requireNode->setVisible(false);
        }else{
            CCCommonUtils::setButtonTitle(m_btn1, _lang("138019").c_str());
            m_btn1->setPositionX(320);
            m_requireNode->setVisible(true);
        }
        m_exitBtn->setVisible(GlobalData::shared()->isCrossService);
        m_btn1->setVisible(true);
    }else if(m_info->startTime==0){
        m_readyTime->setString(_lang("105800"));
        m_exitBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_btn1, _lang("138003").c_str());
        m_btn1->setPositionX(320);
        m_requireNode->setVisible(false);
        m_infoNode->setVisible(false);
        m_readyTime->setPositionY(234);
        m_readyTip->setPositionY(300);
    }else if (m_info->startTime > nowTime)
    {
        double leftTime = m_info->startTime - nowTime;
        m_readyTime->setString(_lang_1("105804", CC_SECTOA(leftTime)));
        m_exitBtn->setVisible(false);
        m_requireNode->setVisible(false);
        m_infoNode->setVisible(false);
        m_readyTime->setPositionY(234);
        m_readyTip->setPositionY(300);
        CCCommonUtils::setButtonTitle(m_btn1, _lang("138003").c_str());
        if(GlobalData::shared()->isCrossService){
            m_exitBtn->setVisible(true);
            m_btn1->setVisible(true);
        }else{
            m_btn1->setPositionX(320);
        }
    }else{
        m_requireNode->setVisible(false);
        m_exitBtn->setVisible(false);
        m_btn1->setVisible(true);
        m_readyTime->setString(_lang("133083"));
        CCCommonUtils::setButtonTitle(m_btn1, _lang("138003").c_str());
        if(GlobalData::shared()->isCrossService){
            m_exitBtn->setVisible(true);
            m_btn1->setVisible(true);
        }else{
            m_btn1->setPositionX(320);
        }
    }
}

void CrossServiceActivityView::onClickBtn1(CCObject *pSender, CCControlEvent event){
    auto nowTime = GlobalData::shared()->getWorldTime();
    if(m_info->startTime==0 || m_info->startTime > nowTime || m_info->endTime < nowTime){
        PopupViewController::getInstance()->addPopupView(CrossServiceRankView::create(m_info));
        return ;
    }
    if (GlobalData::shared()->isCrossService) {
        PopupViewController::getInstance()->addPopupView(CrossServiceRankView::create(m_info));
        return ;
    }
    if (FunBuildController::getInstance()->getMainCityLv()<ActivityController::getInstance()->wb_condition_k1) {
        YesNoDialog::show(_lang_1("138021",CC_ITOA(ActivityController::getInstance()->wb_condition_k1)), NULL);
        return;
    }
    if (!GlobalData::shared()->isCrossService && (ArmyController::getInstance()->getTotalDead()>0 || QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0)) {
        YesNoDialog* dialog = YesNoDialog::show(_lang("138110").c_str(), CCCallFunc::create(this, callfunc_selector(CrossServiceActivityView::healSoldier)));
        CCCommonUtils::setButtonTitle(dialog->m_btnOk, _lang("138113").c_str());
        return ;
    }
    string itemUUid = "";
    string itemName = "";
    if (GlobalData::shared()->firstEnterCrossService) {
        itemUUid = "";
    }else{
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(ActivityController::getInstance()->wb_condition_k2);
        itemName = info.getName();
        if (info.getCNT()>0) {
            itemUUid = info.uuid;
        }else{
            YesNoDialog::show(_lang("138022").c_str(), NULL);
            return;
        }
    }
    if (GlobalData::shared()->firstEnterCrossService) {
        enterCKF();
    }else{
        YesNoDialog* dialog = YesNoDialog::show(_lang_1("138090",itemName.c_str()), CCCallFunc::create(this, callfunc_selector(CrossServiceActivityView::enterCKF)));
    }
}

void CrossServiceActivityView::healSoldier(){
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_HOSPITAL);
    if (buildId<=0) {
        return ;
    }
    if (QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0) {
        int m_qType = CCCommonUtils::getQueueTypeByBuildType(buildId/1000);
        if (m_qType != -1) {
            m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType);
        }
        auto it = GlobalData::shared()->allQueuesInfo.find(m_qid);
        if (it!=GlobalData::shared()->allQueuesInfo.end()) {
            double lastTime = it->second.finishTime - it->second.startTime;
            m_gold = CCCommonUtils::getGoldByTime(lastTime);
            if(PopupViewController::getInstance()->CanPopPushView) {
                YesNoDialog::showTime( _lang("138111").c_str() , CCCallFunc::create(this, callfunc_selector(CrossServiceActivityView::onCostGoldBack)), lastTime, _lang("104903").c_str(), false);
            }
            else {
                if (m_gold > 0) {
                    if (GlobalData::shared()->playerInfo.gold < m_gold) {
                        YesNoDialog::gotoPayTips();
                        return;
                    }
                }
                QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
            }
            return ;
        }
    }else{
        auto& info = FunBuildController::getInstance()->getFunbuildById(buildId);
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(BuildingHospitalPopUpView::create(info));
    }
}

void CrossServiceActivityView::onCostGoldBack(){
    if (m_gold > 0) {
        if (GlobalData::shared()->playerInfo.gold < m_gold) {
            YesNoDialog::gotoPayTips();
            return;
        }
    }
    QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
}

void CrossServiceActivityView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void CrossServiceActivityView::onInfoClick(CCObject *pSender, CCControlEvent event){
//    string temp = _lang("138002");
//    temp.append("\n");
//    temp.append("\n");
//    PopupViewController::getInstance()->addPopupView(TipsView::create(temp,kCCTextAlignmentLeft));
    CCCommonUtils::showHelpShiftSingleFAQ("45209");
}

void CrossServiceActivityView::onClickExitBtn(CCObject *pSender, CCControlEvent event){
    string name = CCCommonUtils::getNameById(CC_ITOA(ActivityController::getInstance()->wb_condition_k1));
    YesNoDialog* dialog = YesNoDialog::show(_lang_1("138091",name.c_str()), CCCallFunc::create(this, callfunc_selector(CrossServiceActivityView::leaveCKF)));
}

void CrossServiceActivityView::leaveCKF(){
    m_exitBtn->setEnabled(false);
    CSALeaveCommand* cmd = new CSALeaveCommand();
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void CrossServiceActivityView::enterCKF(){
    string itemUUID = "";
    if (!GlobalData::shared()->firstEnterCrossService) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(ActivityController::getInstance()->wb_condition_k2);
        itemUUID = info.uuid;
    }
    CrossServiceActivityCommand* cmd = new CrossServiceActivityCommand(itemUUID);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CrossServiceActivityView::getData), NULL));
    cmd->sendAndRelease();
    m_btn1->setEnabled(false);
    GameController::getInstance()->showWaitInterface();
}

bool CrossServiceActivityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnClose", CCControlButton*, this->m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_readyTime", CCLabelIF*, this->m_readyTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_readyTip", CCLabelIF*, this->m_readyTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_requireNode", CCNode*, this->m_requireNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_exitBtn", CCControlButton*, this->m_exitBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoNode", CCNode*, this->m_infoNode);
    return false;
}

SEL_CCControlHandler CrossServiceActivityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn1", CrossServiceActivityView::onClickBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", CrossServiceActivityView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoClick", CrossServiceActivityView::onInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickExitBtn", CrossServiceActivityView::onClickExitBtn);
    return NULL;
}