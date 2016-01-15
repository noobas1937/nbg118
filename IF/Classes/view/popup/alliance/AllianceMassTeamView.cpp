//
//  AllianceMassTeamView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-7.
//
//

#include "AllianceMassTeamView.h"
#include "PopupViewController.h"
#include "AllianceManager.h"
#include "AllianceWarDetailView.h"
#include "AllianceWarView.h"
#include "WorldController.h"
#include "ActivityController.h"

AllianceMassTeamView *AllianceMassTeamView::create(int pointId){
    AllianceMassTeamView *ret = new AllianceMassTeamView(pointId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceMassTeamView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115904"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceMassTeamView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AllianceMassTeamView::init(){
    bool ret = false;

    
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        setIsHDPanel(true);
        auto node = CCBLoadFile("AllianceMassTeamView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(node->getContentSize());//fusheng edit

        this->setModelLayerOpacity(0);
        //changeBGHeight(m_viewBg);
        
        m_infoTxt->setString(_lang("115132"));
        std::string str = CC_ITOA(GlobalData::shared()->world_rally_k1);
        if (GlobalData::shared()->isCrossService && ActivityController::getInstance()->checkUseCKFMassTime()) {
            str = CC_ITOA(GlobalData::shared()->world_rally_k5);
        }
        str.append(_lang("115133").c_str());
        m_txt1->setString(str);
        CCCommonUtils::setButtonTitle(m_btn1, str.c_str());
        str = CC_ITOA(GlobalData::shared()->world_rally_k2);
        if (GlobalData::shared()->isCrossService && ActivityController::getInstance()->checkUseCKFMassTime()) {
            str = CC_ITOA(GlobalData::shared()->world_rally_k6);
        }
        str.append(_lang("115133").c_str());
        m_txt2->setString(str);
        CCCommonUtils::setButtonTitle(m_btn2, str.c_str());
        str = CC_ITOA(GlobalData::shared()->world_rally_k3);
        if (GlobalData::shared()->isCrossService && ActivityController::getInstance()->checkUseCKFMassTime()) {
            str = CC_ITOA(GlobalData::shared()->world_rally_k7);
        }
        str.append(_lang("115133").c_str());
        m_txt3->setString(str);
        CCCommonUtils::setButtonTitle(m_btn3, str.c_str());
        str = CC_ITOA(GlobalData::shared()->world_rally_k4);
        if (GlobalData::shared()->isCrossService && ActivityController::getInstance()->checkUseCKFMassTime()) {
            str = CC_ITOA(GlobalData::shared()->world_rally_k8);
        }
        str.append(_lang("115133").c_str());
        m_txt4->setString(str);
        CCCommonUtils::setButtonTitle(m_btn4, str.c_str());
        m_hideNode->setVisible(false);
        
        m_descTxt->setString(_lang("115134"));

        CCCommonUtils::setButtonTitle(m_btnCancel, _lang("cancel_btn_label").c_str());
        CCCommonUtils::setButtonTitle(m_btnConfirm, _lang("confirm").c_str());
        
        m_index = 0;
        ret = true;
    }
    return ret;
}

bool AllianceMassTeamView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg1", CCSprite*, this->m_viewBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnConfirm", CCControlButton*, this->m_btnConfirm);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box1", CCNode*, this->m_box1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box2", CCNode*, this->m_box2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box3", CCNode*, this->m_box3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box4", CCNode*, this->m_box4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCNode*, this->m_selectSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hideNode", CCNode*, this->m_hideNode);
    return false;
}

bool AllianceMassTeamView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    return true;
}

void AllianceMassTeamView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_viewBg1,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
        return ;
    }
    if (isTouchInside(m_box1,pTouch)) {
        m_selectSpr->setPosition(m_box1->getPosition());//ccp(-227,174)
        m_index = 0;
    }else if(isTouchInside(m_box2,pTouch)){
        m_selectSpr->setPosition(m_box2->getPosition());//ccp(80,171)
        m_index = 1;
    }else if(isTouchInside(m_box3,pTouch)){
        m_selectSpr->setPosition(m_box3->getPosition());//ccp(-219,88)
        m_index = 2;
    }else if(isTouchInside(m_box4,pTouch)){
        m_selectSpr->setPosition(m_box4->getPosition());//ccp(80,90)
        m_index = 3;
    }
}

void AllianceMassTeamView::onClickConfirm(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
    WorldController::getInstance()->openMarchDeploy(m_pointId,1,0,MethodRally,m_index+1);
}

void AllianceMassTeamView::onClickCancel(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceMassTeamView::onClickBtn1(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    PopupViewController::getInstance()->removePopupView(this);
    WorldController::getInstance()->openMarchDeploy(m_pointId,1,0,MethodRally,1);
}
void AllianceMassTeamView::onClickBtn2(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    PopupViewController::getInstance()->removePopupView(this);
    WorldController::getInstance()->openMarchDeploy(m_pointId,1,0,MethodRally,2);
}
void AllianceMassTeamView::onClickBtn3(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    PopupViewController::getInstance()->removePopupView(this);
    WorldController::getInstance()->openMarchDeploy(m_pointId,1,0,MethodRally,3);
}
void AllianceMassTeamView::onClickBtn4(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    PopupViewController::getInstance()->removePopupView(this);
    WorldController::getInstance()->openMarchDeploy(m_pointId,1,0,MethodRally,4);
}

SEL_CCControlHandler AllianceMassTeamView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickConfirm", AllianceMassTeamView::onClickConfirm);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancel", AllianceMassTeamView::onClickCancel);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn1", AllianceMassTeamView::onClickBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn2", AllianceMassTeamView::onClickBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn3", AllianceMassTeamView::onClickBtn3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn4", AllianceMassTeamView::onClickBtn4);
    
    return NULL;
}