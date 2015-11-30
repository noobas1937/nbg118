//
//  ChangeNickNameView.cpp
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#include "ChangeNickNameView.h"
#include "PopupViewController.h"
#include "ChangeNickNameCommand.h"
#include "ToolController.h"
#include "CheckNickNameCommand.h"
#include "ToolController.h"
#include "BuyPicCommand.h"
#include "ChangePicCommand.h"
#include "UseToolView.h"
#include "Utf8Utils.h"
#include "YesNoDialog.h"

#define MIN_NAME_CHAR 3
#define MAX_NAME_CHAR 16
void ChangeNickNameView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangeNickNameView::onEditTextEvent), MSG_INPUT_TEXT_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangeNickNameView::onInputFieldCloseEvent), INPUTFIELD_CLOSE_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangeNickNameView::resetBtnState), MSG_CHANGE_NAME_BACK, NULL);
}

void ChangeNickNameView::onExit(){
    //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_NAME_BACK);
    CCNode::onExit();
}

void ChangeNickNameView::resetBtnState(CCObject* obj){
    m_okBtn->setEnabled(true);
}

ChangeNickNameView* ChangeNickNameView::create(string goodsUUID){
    ChangeNickNameView* ret = new ChangeNickNameView();
    if(ret && ret->init(goodsUUID)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ChangeNickNameView::init(string goodsUUID){
    bool ret = false;
    if(!PopupBaseView::init()){
        return ret;
    }
    setIsHDPanel(true);
    m_goodsUUID = goodsUUID;
    CCBLoadFile("ChangeNickName", this, this);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    
    CCCommonUtils::setButtonTitle(this->m_okBtn, _lang("105109").c_str());
    m_nameLabel->setString(_lang("105220"));
//    m_desLabel->setString(_lang("105222"));
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("105221").c_str());
    m_msgLabel->setString(_lang("105223"));
    
    auto editSize = m_nameNode->getContentSize();
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_editBox = InputFieldMultiLine::create(editSize, "nb_editBG.png", 80);
    }
    else
        m_editBox = InputFieldMultiLine::create(editSize, "nb_editBG.png", 30);
    m_editBox->setAddH(5);
    m_editBox->setAnchorPoint(ccp(0,0));
    nameMaxLen=MAX_NAME_CHAR;
    m_editBox->setMaxChars(nameMaxLen);
    m_editBox->setLineNumber(1);
    m_editBox->setFontColor(ccWHITE);
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(0);
    m_editBox->setcalCharLen(true);
    m_editBox->setOnlySingleLine(true);
    std::string nameTip = "";
    nameTip.append(_lang("105214").c_str());
    m_editBox->setPlaceHolder(nameTip.c_str());
    m_nameNode->addChild(m_editBox);
    
    m_editBox->getBG()->setCapInsets(Rect(22,21,1,1));
    
    m_msgLabel->setString(_lang_1("105224", CC_ITOA(MAX_NAME_CHAR)));

    m_okBtn->setEnabled(false);
    m_isExist = false;
    m_lastName = "";
    
    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_RENAME);
    if(info.getCNT() > 0){
        m_btnText->setVisible(false);
        m_goldNum->setVisible(false);
        m_goldIcon->setVisible(false);
    }else{
        CCCommonUtils::setButtonTitle(this->m_okBtn, "");
        m_btnText->setString(_lang("101263").c_str());
        int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_RENAME));
        m_goldNum->setString(CC_ITOA(gold));
    }
    
    this->unschedule(schedule_selector(ChangeNickNameView::chechNameFun));
    this->schedule(schedule_selector(ChangeNickNameView::chechNameFun));
    this->chechNameFun(0.0);
    
    ret = true;
    return ret;
}

bool ChangeNickNameView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void ChangeNickNameView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_bg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ChangeNickNameView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nickNameTitle", CCLabelIF*, this->m_nickNameTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameOkNode", CCNode*, this->m_nameOkNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);

    return true;
}

SEL_CCControlHandler ChangeNickNameView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkBtnClick", ChangeNickNameView::onOkBtnClick);

    return NULL;
}

void ChangeNickNameView::onOkBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    string nickname = m_editBox->getText();
    if(nickname.length()<3){
        CCCommonUtils::flyText(_lang("105223").c_str());
    }else{
        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_RENAME);
        if(info.getCNT() <= 0){
            int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_RENAME));
            YesNoDialog::show( _lang_1("105219", CC_ITOA(gold)) , CCCallFunc::create(this, callfunc_selector(ChangeNickNameView::onOKCostGold)));
        }else{
            string nickname = m_editBox->getText();
            ToolController::getInstance()->useItemChnageName(ITEM_RENAME, nickname);
            m_okBtn->setEnabled(false);
        }
    }
}

void ChangeNickNameView::onOKCostGold()
{
    int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_RENAME));
    if (gold > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return;
    }
    
    string nickname = m_editBox->getText();
    ToolController::getInstance()->useItemChnageName(ITEM_RENAME, nickname);
    m_okBtn->setEnabled(false);
}

void ChangeNickNameView::onEditTextEvent(cocos2d::CCObject *params)
{
    chechNameFun(0);
}

void ChangeNickNameView::onInputFieldCloseEvent(cocos2d::CCObject *params)
{
    std::string str = m_editBox->getText();
    int length= str.length();
    
    bool isOK = true;
    if(str!="" && str[0]=='(')
    {
        isOK = false;
        m_msgLabel->setString(_lang("113995"));
    }
    else if(length<MIN_NAME_CHAR)
    {
        isOK = false;
        m_msgLabel->setString(_lang("105223"));
        
        if(str == "")
        {
            string nameTip = "";
            nameTip.append(_lang("105214").c_str());
            m_editBox->setPlaceHolder(nameTip.c_str());
        }
    }
    else if(length>nameMaxLen)
    {
        isOK = false;
        m_msgLabel->setString(_lang("113988"));
    }
    else if(m_isExist)
    {
        isOK = false;
        m_msgLabel->setString(_lang("105251"));
    }
    
    m_okBtn->setEnabled(isOK);
    m_nameOkNode->setVisible(true);
    m_nameOkNode->getChildByTag(0)->setVisible(!isOK);
    m_nameOkNode->getChildByTag(1)->setVisible(isOK);
    
    if(!isOK)
    {
        m_msgLabel->setColor(ccRED);
    }
    else
    {
        int lastNum = nameMaxLen-length;
        m_msgLabel->setString(_lang_1("105224", CC_ITOA(lastNum) ));
        m_msgLabel->setColor(ccc3(109,107,100));
    }
}

void ChangeNickNameView::chechNameFun(float _time)
{
    
    std::string str = m_editBox->getText();
    int length= str.length();

    int lastNum = nameMaxLen-length;
    if(lastNum < 0)
    {
        lastNum = 0;
    }
    m_msgLabel->setString(_lang_1("105224", CC_ITOA(lastNum) ));
    m_msgLabel->setColor(ccc3(109,107,100));
    
//    m_nameOkNode->setVisible(true);
//    m_msgLabel->setVisible(false);
    
//    if(str==""){//|| length < MIN_NAME_CHAR|| m_lastName==str 
//        m_msgLabel->setString(_lang_1("105224", CC_ITOA(MAX_NAME_CHAR)));
//        m_msgLabel->setVisible(true);
//        return ;
//    }
    
//    if (length >= 1 && str[0]=='(') {
//        m_msgLabel->setVisible(true);
//        m_msgLabel->setString(_lang("113995"));
//        return;
//    }
    
    if(m_lastName==str)
    {
        return;
    }
    m_lastName = str;
    CheckNickNameCommand* command = new CheckNickNameCommand(m_editBox->getText());
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChangeNickNameView::checkSuccess), NULL));
    command->sendAndRelease();
    CCLOG("send name=%s",m_editBox->getText().c_str());
}

void ChangeNickNameView::checkSuccess(CCObject* data)
{
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCDictionary* dictData = _dict(result->getData());
    int exist = dictData->valueForKey("exist")->intValue();
    int length= dictData->valueForKey("length")->intValue();

    int lastNum = MAX_NAME_CHAR-length;
    if(lastNum < 0)
    {
        lastNum = 0;
    }
    m_msgLabel->setString(_lang_1("105224", CC_ITOA(lastNum) ));
    m_msgLabel->setColor(ccc3(109,107,100));
    m_msgLabel->setVisible(true);

    nameMaxLen=lastNum+m_editBox->getText().length();
    m_editBox->setMaxChars(nameMaxLen);
    
    bool ok = (exist != -1 && length >= MIN_NAME_CHAR && length <= MAX_NAME_CHAR);
    
    m_isExist = false;
    
    if(ok)
    {
    }
    else if (exist == -1)
    {
        m_isExist = true;
    }
    this->onInputFieldCloseEvent(NULL);
}
