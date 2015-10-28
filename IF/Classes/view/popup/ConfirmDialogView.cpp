//
//  ConfirmDialogView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-4.
//
//

#include "ConfirmDialogView.h"
#include "PopupViewController.h"


ConfirmDialogView* ConfirmDialogView::create(int food,int wood,int iron,int stone,int money,int gold,CCObject* callTarget,SEL_CallFunc callunc, string tips, string btnName){
    ConfirmDialogView* ret = new ConfirmDialogView(food,wood,iron,stone,money,gold,callTarget,callunc, tips, btnName);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool ConfirmDialogView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCBLoadFile("ConfirmDialog",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    setTitleName(_lang(""));
    m_useGoldTxt->setString(CC_ITOA(m_gold));
    m_btnTxt->setString(_lang("102131").c_str());
    m_tipTxt->setString(_lang("102177").c_str());
    m_needTip->setString(_lang("102178").c_str());
    if (m_tips != "") {
        m_tipTxt->setString(m_tips);
    }
    if (m_btnName != "") {
        m_btnTxt->setString(m_btnName);
    }
    
    //CCCommonUtils::setButtonTitle(m_btnOk, _lang("confirm").c_str());
    int offx = 0;
    if(m_wood>0){
        CCSprite* spr = CCLoadSprite::createSprite("ui_wood.png");
        spr->setPosition(ccp(offx, 0));
        m_resNode->addChild(spr);
        CCLabelIF* label = CCLabelIF::create();
        label->setString(CC_ITOA_K(m_wood));
        label->setColor(ccBLACK);
        label->setFontSize(22);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setPosition(ccp(offx+5+spr->getContentSize().width, 0));//fusheng
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.5);
            label->setFontSize(44);
        }
        m_resNode->addChild(label);
        offx +=65;
    }
    if(m_stone>0){
        CCSprite* spr = CCLoadSprite::createSprite("ui_stone.png");
        spr->setPosition(ccp(offx, 0));
        m_resNode->addChild(spr);
        CCLabelIF* label = CCLabelIF::create();
        label->setString(CC_ITOA_K(m_stone));
        label->setColor(ccBLACK);
        label->setFontSize(22);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setPosition(ccp(offx+5+spr->getContentSize().width, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.5);
            label->setFontSize(44);
        }
        m_resNode->addChild(label);
        offx +=65;
    }
    if(m_iron>0){
        CCSprite* spr = CCLoadSprite::createSprite("ui_iron.png");
        spr->setPosition(ccp(offx, 0));
        m_resNode->addChild(spr);
        CCLabelIF* label = CCLabelIF::create();;
        label->setString(CC_ITOA_K(m_iron));
        label->setColor(ccBLACK);
        label->setFontSize(22);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setPosition(ccp(offx+5+spr->getContentSize().width, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.5);
            label->setFontSize(44);
        }
        m_resNode->addChild(label);
        offx +=65;
    }
    if(m_food>0){
        CCSprite* spr = CCLoadSprite::createSprite("ui_food.png");
        spr->setPosition(ccp(offx, 0));
        m_resNode->addChild(spr);
        CCLabelIF* label = CCLabelIF::create();
        label->setString(CC_ITOA_K(m_food));
        label->setColor(ccBLACK);
        label->setFontSize(22);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setPosition(ccp(offx+5+spr->getContentSize().width, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.5);
            label->setFontSize(44);
        }
        m_resNode->addChild(label);
        offx +=65;
    }
    if(m_money>0){
        CCSprite* spr = CCLoadSprite::createSprite("ui_money.png");
        spr->setPosition(ccp(offx, 0));
        m_resNode->addChild(spr);
        CCLabelIF* label = CCLabelIF::create();
        label->setString(CC_ITOA_K(m_money));
        label->setColor(ccBLACK);
        label->setFontSize(22);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setPosition(ccp(offx+5+spr->getContentSize().width, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.5);
            label->setFontSize(44);
        }
        m_resNode->addChild(label);
        offx +=65;
    }
    return true;
}

void ConfirmDialogView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void ConfirmDialogView::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool ConfirmDialogView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_bg, pTouch)) {
        return false;
    }
    keyPressedBtnCancel(NULL,Control::EventType::TOUCH_DOWN);
    return true;
}

void ConfirmDialogView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void ConfirmDialogView::keyPressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void ConfirmDialogView::keyPressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler ConfirmDialogView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", ConfirmDialogView::keyPressedBtnOk);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", ConfirmDialogView::keyPressedBtnCancel);
    return NULL;
}

bool ConfirmDialogView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needTip", CCLabelIF*, this->m_needTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTxt", CCLabelIF*, this->m_btnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useGoldTxt", CCLabelIF*, this->m_useGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    return false;
}