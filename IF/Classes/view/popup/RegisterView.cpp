//
//  RegisterView.cpp
//  IF
//
//  Created by zhaohf on 14-4-10.
//
//

#include "RegisterView.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCSafeNotificationCenter.h"
#include "UserBindCommand.h"

RegisterView* RegisterView::create(){
    RegisterView* ret = new RegisterView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RegisterView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("RegisterDialog",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("confirm").c_str());
    
    // email
    auto sprite9 = CCLoadSprite::createScale9Sprite("num_text_bg.png");
    m_editBoxEmail = CCEditBox::create(CCSizeMake(320,60),sprite9);
    m_editBoxEmail->setAnchorPoint(ccp(0,0.5));
    m_editBoxEmail->setFontSize(20);
    m_editBoxEmail->setPlaceHolder("");
    m_editBoxEmail->setMaxLength(512);
    m_editBoxEmail->setFontColor(ccc3(255, 255, 255));
    m_editBoxEmail->setPosition(ccp(70, 300));
    m_editBoxEmail->setReturnType(kKeyboardReturnTypeDone);
    m_editBoxEmail->setInputMode(EditBox::InputMode::EMAIL_ADDRESS);
    m_subNode->addChild(m_editBoxEmail);
    
    // password
    auto sprite9p = CCLoadSprite::createScale9Sprite("num_text_bg.png");
    m_editBoxPassword = CCEditBox::create(CCSizeMake(320,60),sprite9p);
    m_editBoxPassword->setAnchorPoint(ccp(0,0.5));
    m_editBoxPassword->setFontSize(20);
    m_editBoxPassword->setPlaceHolder("");
    m_editBoxPassword->setMaxLength(512);
    m_editBoxPassword->setFontColor(ccc3(255, 255, 255));
    m_editBoxPassword->setPosition(ccp(70, 200));
    m_editBoxPassword->setReturnType(kKeyboardReturnTypeDone);
    m_editBoxPassword->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_subNode->addChild(m_editBoxPassword);
    
    // password2
    auto sprite9p1 = CCLoadSprite::createScale9Sprite("num_text_bg.png");
    m_editBoxPassword1 = CCEditBox::create(CCSizeMake(320,60),sprite9p1);
    m_editBoxPassword1->setAnchorPoint(ccp(0,0.5));
    m_editBoxPassword1->setFontSize(20);
    m_editBoxPassword1->setPlaceHolder("");
    m_editBoxPassword1->setMaxLength(512);
    m_editBoxPassword1->setFontColor(ccc3(255, 255, 255));
    m_editBoxPassword1->setPosition(ccp(70, 100));
    m_editBoxPassword1->setReturnType(kKeyboardReturnTypeDone);
    m_editBoxPassword1->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_subNode->addChild(m_editBoxPassword1);
    
    return true;
}
void RegisterView::onEnter()
{
    CCNode::onEnter();
}

void RegisterView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler RegisterView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", RegisterView::onCloseView);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", RegisterView::onOk);
    return NULL;
}

bool RegisterView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    return false;
}
void RegisterView::onOk(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    string p = m_editBoxPassword->getText();
    string p1 = m_editBoxPassword1->getText();
    if (p != p1) {
        CCCommonUtils::flyText("password error!");
        return;
    }
    string mail = m_editBoxEmail->getText();
    CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, mail);
    CCUserDefault::sharedUserDefault()->flush();
  //  UserBindCommand* cmd = new UserBindCommand("","","",mail,p);
  //  cmd->sendAndRelease();
    PopupViewController::getInstance()->removePopupView(this);
}

void RegisterView::onCloseView(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}