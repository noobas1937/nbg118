//
//  KingdomNameChangeView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-29.
//
//

#include "KingdomNameChangeView.h"
#include "KingdomNameChangeView.h"
#include "PopupViewController.h"
#include "GameController.h"
#include "ManagerKingdomView.h"
#include "YesNoDialog.h"
#include "Utf8Utils.h"

KingdomNameChangeView *KingdomNameChangeView::create(OfficeInfo* info){
    KingdomNameChangeView *ret = new KingdomNameChangeView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomNameChangeView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingdomNameChangeView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool KingdomNameChangeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void KingdomNameChangeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_clickArea, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool KingdomNameChangeView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCBLoadFile("KingdomNameChangeView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());

        m_kingTxt->setString(_lang("110002").c_str());
        m_tip1->setString(_lang("110029").c_str());
        string tip2 = _lang("110030");
        tip2 += " ";
        tip2 += _lang("110169");
        m_tip2->setString(tip2);
//        m_tip2->setString(_lang("110030"));
        
        auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
        m_inputName = CCEditBox::create(CCSizeMake(440,44),spriteText);
        m_inputName->setMaxLength(10);
        m_inputName->setFontSize(24);
        m_inputName->setFontColor(ccBLACK);
        m_inputName->setReturnType(kKeyboardReturnTypeDone);
        m_inputName->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_inputNode->addChild(m_inputName);
        m_inputName->setPlaceHolder(m_info->kingdomName.c_str());
        
        CCCommonUtils::setButtonTitle(m_nameBtn, _lang("110031").c_str());
        
        m_chenckName = "";
        this->unschedule(schedule_selector(KingdomNameChangeView::checkName));
        this->schedule(schedule_selector(KingdomNameChangeView::checkName));
        this->checkName(0.0);
        
        m_nameBtn->setEnabled(false);
        m_inputName->setEnabled(true);
        
        ret = true;
    }
    return ret;
}

void KingdomNameChangeView::checkName(float time){
    string name = m_inputName->getText();
    int length= Utf8Utils::strlen( name.c_str());
    bool ok = true;
    if(length >10 || length <=0){
        ok = false;
    }
    m_nameBtn->setEnabled(ok);
    m_nameOkNode->setVisible(true);
    m_nameOkNode->getChildByTag(0)->setVisible(!ok);
    m_nameOkNode->getChildByTag(1)->setVisible(ok);

    if(name!="" && m_chenckName!=name && name.length()<11){
        KingdomCheckNameCommand* cmd = new KingdomCheckNameCommand();
        cmd->putParam("name", CCString::create(name));
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomNameChangeView::checkNameCallBack), NULL));
        cmd->sendAndRelease();
        m_chenckName = name;
    }
}

void KingdomNameChangeView::checkNameCallBack(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        bool ok = dic->valueForKey("success")->boolValue();
        m_nameBtn->setEnabled(ok);
        m_nameOkNode->getChildByTag(0)->setVisible(!ok);
        m_nameOkNode->getChildByTag(1)->setVisible(ok);
    }
}

bool KingdomNameChangeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_inputNode", CCNode*, this->m_inputNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameOkNode", CCNode*, this->m_nameOkNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameBtn", CCControlButton*, this->m_nameBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingTxt", CCLabelIFTTF*, this->m_kingTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip1", CCLabelIF*, this->m_tip1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip2", CCLabelIF*, this->m_tip2);
    
    return false;
}


SEL_CCControlHandler KingdomNameChangeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeNameClick", KingdomNameChangeView::onChangeNameClick);
    return NULL;
}


void KingdomNameChangeView::onChangeNameClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::show(_lang_1("110032",m_chenckName.c_str()), CCCallFunc::create(this, callfunc_selector(KingdomNameChangeView::confirmFun1)));
}

void KingdomNameChangeView::confirmFun1(){
    YesNoDialog::show(_lang("110033").c_str(), CCCallFunc::create(this, callfunc_selector(KingdomNameChangeView::confirmFun2)));
}

void KingdomNameChangeView::confirmFun2(){
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(CHANGE_KINGDOM_NAME));
    cmd->putParam("name", CCString::create(m_chenckName));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomNameChangeView::modifyNameSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void KingdomNameChangeView::modifyNameSuccess(CCObject* data){
    string name = m_chenckName;
    if (GlobalData::shared()->playerInfo.isInSelfServer()) {
        name += "#";
        name += CC_ITOA(GlobalData::shared()->playerInfo.selfServerId);
    }
    m_kingTxt->setString(_lang_1("110023",name.c_str()));
    m_info->kingdomName = name;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(KINGDOM_NAME_CHANGE);
    PopupViewController::getInstance()->removePopupView(this);
}