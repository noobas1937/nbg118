//
//  AlertAddAllianceViewRecommend.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-7.
//
//

#include "AlertAddAllianceViewRecommend.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GuideController.h"
#include "PopupViewController.h"
#include "CreateAllianceView.h"
#include "JoinAllianceView.h"
#include "GetAllianceListCommand.h"
#include "SoundController.h"
//begin a by ljf
#include "AllianceFlagPar.h"
#include "ApplyAllianceCommand.h"
#include "ChatController.h"
//end a by ljf

AlertAddAllianceViewRecommend *AlertAddAllianceViewRecommend::create(AllianceInfo* info){
    AlertAddAllianceViewRecommend *ret = new AlertAddAllianceViewRecommend(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AlertAddAllianceViewRecommend::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -80, true);
    //UIComponent::getInstance()->showPopupView(2);
    //UIComponent::getInstance()->hideReturnBtn();
}

void AlertAddAllianceViewRecommend::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AlertAddAllianceViewRecommend::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        this->setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(206, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
        
        auto node = CCBLoadFile("AlertAddAllianceViewRecommend", this, this);
        this->setContentSize(node->getContentSize());
        
//        m_titleText->setString(_lang("115001"));
//        m_txt1->setString(_lang("115002"));
//        m_txt2->setString(_lang("115003"));
//        m_txt3->setString(_lang("115004"));
//        m_txt4->setString(_lang("115005"));
//        m_txt5->setString(_lang("115006"));
        
//        m_txt1->setString(_lang("115168"));
//        m_txt2->setString(_lang_1("115169", "200"));
//        m_txt3->setString(_lang("115170"));
        
        m_txt1->setString(_lang("500035"));
        m_txt2->setString(_lang("500026"));
        //m_txt3->setString(_lang("500026"));
        
        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115020").c_str());
        m_btnJoin->setSwallowsTouches(false);
        
        m_btnJoin->setTitleColorForState({0x8c, 0x51, 0x17}, cocos2d::extension::Control::State::SELECTED);
        m_btnJoin->setTitleColorForState({0x8c, 0x51, 0x17}, cocos2d::extension::Control::State::NORMAL);
        m_btnJoin->setTitleColorForState({0x8c, 0x51, 0x17}, cocos2d::extension::Control::State::HIGH_LIGHTED);
        m_btnJoin->setTitleColorForState({0x8c, 0x51, 0x17}, cocos2d::extension::Control::State::DISABLED);
        
        //begin a by ljf
        if(m_info!=NULL){
            std::string str = "(";
            str.append(m_info->shortName.c_str());
            str.append(") ");
            str.append(m_info->name.c_str());
            m_allianceName->setString(str.c_str());
            str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(m_info->getSelfAllianceNum()));
            m_memberNum->setString(str);
            str = _lang_1("115015", m_info->leader.c_str());
            m_allianceLeader->setString(str.c_str());
            str = _lang_1("115017", CC_CMDITOAL(m_info->totalForce).c_str());
            m_battleNum->setString(str);

            string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
            m_language->setString(_lang(lang).c_str());

            m_allianceIcon->removeAllChildren();
            AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
            m_allianceIcon->addChild(flag);

            m_title->setString(_lang("115001"));
            m_gemNum->setString("X200"); //应从配置文件里面读,ljf
            //m_gemNum->setString(_lang("103672").c_str());
        }
        //end a by ljf
        ret = true;
    }
    return ret;
}

bool AlertAddAllianceViewRecommend::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    //begin a by ljf
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gemNum", CCLabelIF*, this->m_gemNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceName", CCLabelIF*, this->m_allianceName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIF*, this->m_language);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceLeader", CCLabelIF*, this->m_allianceLeader);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_memberNum", CCLabelIF*, this->m_memberNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battleNum", CCLabelIF*, this->m_battleNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    //end a by ljf
    return false;
}

bool AlertAddAllianceViewRecommend::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AlertAddAllianceViewRecommend::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_btnJoin, pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        onClickJoin(NULL,Control::EventType::TOUCH_DOWN);
    } else if (!isTouchInside(m_clickNode, pTouch)) {
        this->onClickClose(NULL,Control::EventType::TOUCH_DOWN);
    }
    
}

SEL_CCControlHandler AlertAddAllianceViewRecommend::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoin", AlertAddAllianceViewRecommend::onClickJoin);
    return NULL;
}

void AlertAddAllianceViewRecommend::onClickClose(CCObject * pSender, Control::EventType pCCControlEvent)
{
    this->closeSelf();
}

void AlertAddAllianceViewRecommend::onClickJoin(CCObject * pSender, Control::EventType pCCControlEvent) {
    
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    //ApplyAllianceCommand* cmd = new ApplyAllianceCommand("d74fc57c6f354dc39f22d6cd3c8d2159");
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AlertAddAllianceViewRecommend::createSuccess), NULL));
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(AlertAddAllianceViewRecommend::createFail), NULL));
    cmd->sendAndRelease();
    
    /*
    int n = rand() % 2;
    if(n == 0)
    {
        createFail(nullptr);
    }
    if(n == 1)
    {
        createSuccess(nullptr);
    }
    */
    //this->closeSelf();
}


void AlertAddAllianceViewRecommend::createSuccess(CCObject* obj){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        //PopupViewController::getInstance()->removeAllPopupView();
        //PopupViewController::getInstance()->removeLastPopupView();
        this->closeSelf();
        CCCommonUtils::setIsHDViewPort(true);
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
    }
    //发聊天消息，加入联盟
    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
    //PopupViewController::getInstance()->removeAllPopupView();
    //AllianceManager::getInstance()->addAnnounceEvent();
    //PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
}

void AlertAddAllianceViewRecommend::createFail(CCObject* obj)
{
    //PopupViewController::getInstance()->removeAllPopupView();
    //PopupViewController::getInstance()->removeLastPopupView();
    this->closeSelf();
}

