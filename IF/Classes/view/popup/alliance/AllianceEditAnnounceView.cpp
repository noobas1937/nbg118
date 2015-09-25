//
//  AllianceEditAnnounceView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-20.
//
//
#include "AllianceEditAnnounceView.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GuideController.h"
#include "PopupViewController.h"
#include "CreateAllianceView.h"
#include "JoinAllianceView.h"
#include "GetAllianceListCommand.h"
#include "ChangeAllianceAttributesCommand.h"

AllianceEditAnnounceView *AllianceEditAnnounceView::create(std::string content,bool edit){
    AllianceEditAnnounceView *ret = new AllianceEditAnnounceView(content,edit);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceEditAnnounceView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void AllianceEditAnnounceView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AllianceEditAnnounceView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        auto node = CCBLoadFile("AllianceEditAnnounceView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_contentEditBox = NULL;
        m_announceTxt->setString(m_content.c_str());
        m_btnSave->setVisible(m_canEdit);
        CCCommonUtils::setButtonTitle(m_btnSave, _lang("115035").c_str());
        CCCommonUtils::setButtonTitle(m_btnEdit, _lang("115020").c_str());
        
        if(m_canEdit && m_contentEditBox==NULL){
            m_contentEditBox = InputFieldMultiLine::create(CCSizeMake(560,250),"Alliance_r1234_frame.png",24);
            m_contentEditBox->setMaxChars(199);
            m_contentEditBox->setLineNumber(10);
            m_contentEditBox->setPosition(ccp(0, 0));
            m_contentEditBox->setFontColor({255,255,255});
            m_contentEditBox->setEnabled(m_canEdit);
            m_contentEditBox->setSwallowsTouches(false);
            m_textNode->addChild(m_contentEditBox);
        }
        m_first = true;
        ret = true;
    }
    return ret;
}

bool AllianceEditAnnounceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnEdit", CCControlButton*, this->m_btnEdit);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnSave", CCControlButton*, this->m_btnSave);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textNode", CCNode*, this->m_textNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceTxt", CCLabelIFTTF*, this->m_announceTxt);
    return false;
}

bool AllianceEditAnnounceView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceEditAnnounceView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_textNode, pTouch)) {
        if(m_first && m_canEdit && m_contentEditBox!=NULL){
            m_contentEditBox->setText(GlobalData::shared()->playerInfo.allianceInfo.announce);
            m_announceTxt->setString("");
            m_first = false;
        }
    }
    if (!isTouchInside(m_bg, pTouch)) {
        this->onClickClose(NULL,Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_btnSave, pTouch)){
        //this->onSaveClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}

void AllianceEditAnnounceView::onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent){
    std::string content = m_contentEditBox->getText();
    if(content==GlobalData::shared()->playerInfo.allianceInfo.announce || m_first){
        this->closeSelf();
    }else{
        ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
        command->putParam("announce", CCString::create(content));
        command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceEditAnnounceView::success), NULL));
        command->sendAndRelease();
        m_btnSave->setEnabled(false);
    }
}

void AllianceEditAnnounceView::success(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL || result->getData()==NULL) {
        return ;
    }
    auto dic = _dict(result->getData());
    if(dic){
        string announce = dic->valueForKey("announce")->getCString();
        GlobalData::shared()->playerInfo.allianceInfo.announce = announce;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_ANNOUNCE);
        this->closeSelf();
        CCCommonUtils::flyHint("", "", _lang("115205"));
    }
}

SEL_CCControlHandler AllianceEditAnnounceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSaveClick", AllianceEditAnnounceView::onSaveClick);
    return NULL;
}

void AllianceEditAnnounceView::onClickClose(CCObject * pSender, Control::EventType pCCControlEvent) {
    this->closeSelf();
}