//
//  SuggestionView.cpp
//  IF
//
//  Created by fubin on 13-11-14.
//
//

#include "SuggestionView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "SuggestionCommand.h"
#include "SettingPopUpView.h"

SuggestionView* SuggestionView::create(){
    SuggestionView* ret = new SuggestionView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

SuggestionView::~SuggestionView()
{
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_more)));
}

bool SuggestionView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("SuggestionView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105302").c_str());
    CCCommonUtils::setButtonTitleColor(m_sendBtn, ccc3(253, 187, 9));
    setTitleName(_lang("105202").c_str());
    
    //m_titleName1->setString(_lang("105206"));
    m_titleName1->setString("");
    m_tips->setString(_lang("105203"));
    float oldHeight = getBGHeight() - m_BG->getContentSize().height;
    changeBGHeight(m_BG);
    m_infoBG->setContentSize(CCSize(m_infoBG->getContentSize().width,m_infoBG->getContentSize().height+oldHeight));
    m_editNode->setPositionY(m_editNode->getPositionY()-oldHeight);
    int _editHeight = getBGHeight()-300;
    m_editBox = CCEditText::create(CCSizeMake(610,_editHeight),"queue_bg1.png",true,false);
    //auto sprite9 = CCLoadSprite::createScale9Sprite("chat_edit.png");
   // m_editBox = CCEditBox::create(CCSizeMake(480,50),sprite9);
    m_editBox->setAnchorPoint(ccp(0,0.5));
    m_editBox->setFontSize(24);
   // m_editBox->setPlaceHolder(_lang("105205").c_str());
    m_editBox->setMaxLength(1024);
    m_editBox->setFontColor(ccc3(122, 66, 0));
    m_editBox->setReturnType(kKeyboardReturnTypeSend);
    m_editBox->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
//    m_editBox->setBeforeInputCallBack(CCCallFuncO::create(this, callfuncO_selector(SuggestionView::setBeforeShowEditText), NULL));
    m_editBox->setPosition(0, _editHeight/2);
    m_infoNode->addChild(m_editBox);
//    m_editBox->setText(_lang("105205").c_str());
//    m_infoLabel =  CCLabelIF::create("", 22, CCSizeMake(550, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
//    if (LocalController::shared()->getLanguageFileName() != "zh_CN") {
//        this->m_infoLabel->setFntFile("Arial_Bold.fnt");
//    }
    
   // m_infoLabel->setAnchorPoint(ccp(0,0.5));
   // m_infoLabel->setColor(ccc3(56, 8, 0));
   // m_infoNode->addChild(m_infoLabel);
    
    return true;
}
void SuggestionView::setBeforeShowEditText(){
    string curStr = m_editBox->getText();
    if (curStr==_lang("105205")) {
        m_editBox->setText("");
        // m_infoLabel->setString(curStr.c_str());
    }
}
void SuggestionView::onEnter()
{
    CCNode::onEnter();
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SuggestionView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void SuggestionView::onExit()
{
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SuggestionView::onEnterFrame), this);
    CCNode::onExit();
}

//void SuggestionView::onEnterFrame(float dt)
//{
//    string curStr = m_editBox->getText();
//    if (curStr==_lang("105205")) {
//        //m_editBox->setText("");
//       // m_infoLabel->setString(curStr.c_str());
//    }
//}

SEL_CCControlHandler SuggestionView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPostBtnClick", SuggestionView::onPostBtnClick);
    return NULL;
}

bool SuggestionView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tips", CCLabelIF*, this->m_tips);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleName1", CCLabelIF*, this->m_titleName1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BG", CCScale9Sprite*, this->m_BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBG", CCScale9Sprite*, this->m_infoBG);
    return false;
}
void SuggestionView::onPostBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    string msg = m_editBox->getText();
    
    string tmp = "";
    tmp = tmp + msg;
    int begin = 0;
    begin = tmp.find(" ",begin);
    while(begin != -1){
        tmp.replace(begin, 1, "");
        begin = tmp.find(" ",begin);
    }

    if (tmp.length()==0||msg==_lang("105205")) {
        CCCommonUtils::flyText(_lang("105206"));
       // PopupViewController::getInstance()->removePopupView(this);
        return;
    }
    
    SuggestionCommand *cmd = new SuggestionCommand(msg);
    cmd->sendAndRelease();
    m_editBox->setText("");
   // m_infoLabel->setString("");
    CCCommonUtils::flyText(_lang("105204"));
}
