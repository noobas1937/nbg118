//
//  KingdomMailView.cpp
//  IF
//
//  Created by 张军 on 13-10-11.
//
//

#include "KingdomMailView.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "MailGiftView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "MailSendCommand.h"
#include "AllianceInviteView.h"

void KingdomMailView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomMailView::onCloseEvent), MAIL_SEND_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomMailView::onUpdateStatusEvent), MSG_INPUT_TEXT_EVENT, NULL);
    setTitleName("邮件123");//_lang("105513")
    if(mInited==true){
//        m_nameEditBox->addTargetedDelegate();
        m_contentEditBox->addTargetedDelegate();
    }
    mInited=true;
}

void KingdomMailView::onExit(){
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_SEND_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
    PopupBaseView::onExit();
}

KingdomMailView* KingdomMailView::create(std::string picStr, std::string allStr, std::string nameStr){
    KingdomMailView* ret = new KingdomMailView(picStr, allStr,nameStr);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool KingdomMailView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    auto bg = CCBLoadFile("KingdomMailView", this, this);
    this->setContentSize(bg->getContentSize());
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    this->m_contentText->setString(_lang("105504").c_str());
    
    float canUseH=CCDirector::sharedDirector()->getWinSize().height-TOP_HEIGHT-90;
    float py=m_kingNode->getPositionY()-m_kingNode->getContentSize().height-50;
    this->m_contentText->setPositionY(py);
    py-=150;
    canUseH=canUseH-m_kingNode->getContentSize().height-20-50;
    
    m_contentEditBox = InputFieldMultiLine::create(CCSizeMake(475,200),"UI_Alliance_TextBox.png",24);
    m_contentEditBox->setMaxChars(300);
    m_contentEditBox->setLineNumber(5);
    m_contentEditBox->setFontColor({63,44,27});
    m_contentEditBox->setAnchorPoint(CCPoint(0.5, 1));
    m_contentEditBox->setPosition(ccp(0, py));
    defaultText="defaultText";
    m_contentEditBox->setText(defaultText);
//    m_contentEditBox->setPlaceHolder(defaultText);
    

    this->m_contentContainer->addChild(m_contentEditBox);
    
    float ny1=m_bg->getPositionY();
    m_listNode->setAnchorPoint(CCPoint(0.5, 1.0));
    m_listNode->setPositionY(ny1);
    
    m_iconNode->removeAllChildrenWithCleanup(true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    auto pic = CCLoadSprite::createSprite(m_picStr.c_str());
    m_iconNode->addChild(pic);
    m_allianceTxt->setString(m_allStr);
    m_playerTxt->setString(m_nameStr.c_str());
    
    updateBtnStatus();
    return true;
}

bool KingdomMailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentContainer", CCNode*, this->m_contentContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprMailIcon", CCSprite*, this->m_sprMailIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprDownBorder", CCScale9Sprite*, this->m_sprDownBorder);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingNode", CCNode*, this->m_kingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceTxt", CCLabelIF*, this->m_allianceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerTxt", Label*, this->m_playerTxt);
    
    return false;
}

SEL_CCControlHandler KingdomMailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", KingdomMailView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", KingdomMailView::onSendClick);
    return NULL;
}

void KingdomMailView::onCloseEvent(cocos2d::CCObject *p){
    onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
}

void KingdomMailView::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->goBackPopupView();
}

void KingdomMailView::onSendClick(cocos2d::CCObject *pSender, CCControlEvent event){

    string content = m_contentEditBox->getText();
    string title = content;
    int len = content.length();
//    if(len>30){
//        title =content.substr(0,29);
//    }
//    if(MailController::getInstance()->sendMail("zyx", title, content,"","",false,MAIL_SELF_SEND,"")){
////        PopupViewController::getInstance()->goBackPopupView();
//    }
    
//    std::string uid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "");
//    KingMailSendCommand* cmd = new KingMailSendCommand("6",uid,content);
//    cmd->sendAndRelease();
    
}
void KingdomMailView::updateBtnStatus(){
    if(m_contentEditBox->getText().empty()){
        m_sendBtn->setEnabled(false);
        m_sprMailIcon->setColor({90,85,81});
    }else{
        m_sendBtn->setEnabled(true);
        m_sprMailIcon->setColor(ccWHITE);
    }
}

void KingdomMailView::onUpdateStatusEvent(cocos2d::CCObject *p){
    updateBtnStatus();
}
