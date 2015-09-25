//
//  DragonBattleJoinView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#include "DragonBattleJoinView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattleWarListView.h"
#include "DragonManageView.h"
#include "AgainstInfoView.h"
#include "DragonHistoryView.h"
#include "DragonBattleApplyCommand.h"
#include "DragonBattleInfoCommand.h"

DragonBattleJoinView *DragonBattleJoinView::create(){
    DragonBattleJoinView *ret = new DragonBattleJoinView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonBattleJoinView::onEnter(){
    this->setTitleName(_lang("140001"));
    PopupBaseView::onEnter();
}

void DragonBattleJoinView::onExit(){
    PopupBaseView::onExit();
}

bool DragonBattleJoinView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonBattleJoinView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool DragonBattleJoinView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(512, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        CCLoadSprite::doResourceByCommonIndex(506, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(512, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(506, false);
        });
        auto node = CCBLoadFile("DragonBattleJoinView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        m_fixbg->setScaleY((add+646.0*0.6)/646.0);
        m_moveNode->setPositionY(m_moveNode->getPositionY() - add/2);
        
        m_battleTile->setString(_lang("140001"));
        m_battleDesc->setString(_lang("140002"));
        m_startTime->setString(_lang("140003"));
        m_endTime->setString(_lang("140004"));
        
        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140005").c_str());
        CCCommonUtils::setButtonTitle(m_helpBtn, _lang("140006").c_str());
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("140007").c_str());
        CCCommonUtils::setButtonTitle(m_infoBtn, _lang("140010").c_str());
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("140043").c_str());
        int state = 2;
        if (state==0) {
            m_infoBtn->setVisible(false);
            m_manageBtn->setVisible(false);
        }else if(state==1){
            m_infoBtn->setVisible(false);
            m_joinBtn->setVisible(false);
        }else {
            
        }
        DragonBattleInfoCommand* cmd = new DragonBattleInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattleJoinView::getData), NULL));
        cmd->sendAndRelease();
        
        ret = true;
    }
    return ret;
}

void DragonBattleJoinView::getData(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){

    }
}

void DragonBattleJoinView::onClickJoin(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->playerInfo.allianceInfo.rank==5){
        if (true) {
            PopupViewController::getInstance()->addPopupInView(DragonBattleWarListView::create());
        }else{
            DragonBattleApplyCommand* cmd = new DragonBattleApplyCommand();
        }
    }else{
        YesNoDialog::show(_lang("140106").c_str(), NULL);
    }
}

void DragonBattleJoinView::onClickInfo(CCObject *pSender, CCControlEvent event){
   PopupViewController::getInstance()->addPopupView(AgainstInfoView::create());
}

void DragonBattleJoinView::onHistoryClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonHistoryView::create());
}

void DragonBattleJoinView::onClickHelp(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45209");
}

void DragonBattleJoinView::onManageClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonManageView::create());
}

bool DragonBattleJoinView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinBtn", CCControlButton*, this->m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_manageBtn", CCControlButton*, this->m_manageBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleTile", CCLabelIF*, this->m_battleTile);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleDesc", CCLabelIF*, this->m_battleDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_startTime", CCLabelIF*, this->m_startTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_endTime", CCLabelIF*, this->m_endTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fixbg", CCSprite*, this->m_fixbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    return false;
}
SEL_CCControlHandler DragonBattleJoinView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickJoin", DragonBattleJoinView::onClickJoin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickInfo", DragonBattleJoinView::onClickInfo);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHistoryClick", DragonBattleJoinView::onHistoryClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelp", DragonBattleJoinView::onClickHelp);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageClick", DragonBattleJoinView::onManageClick);
    
    return NULL;
}