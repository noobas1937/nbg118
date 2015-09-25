//
//  TipsView.cpp
//  IF
//
//  Created by 李锐奇 on 13-11-4.
//
//

#include "TipsView.h"
#include "PopupViewController.h"

TipsView* TipsView::create(string msg,CCTextAlignment align) {
    auto ret = new TipsView();
    if (ret && ret->init(msg,align)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TipsView::init(string msg,CCTextAlignment align) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("TipsNewView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
//        CCCommonUtils::setButtonTitle(m_btnOk, _lang("confirm").c_str());
        m_msgLabel->setAlignment(align);
        m_msgLabel->setString(msg);
        int msgHeight = m_msgLabel->getContentSize().height*m_msgLabel->getOriginScaleY();
//        int sumHeight = msgHeight + m_btnOk->getContentSize().height +100;
//        
//        m_btnOk->setPositionY(m_btnOk->getContentSize().height/2+30 -sumHeight/2);
//        m_msgLabel->setPositionY(sumHeight/2-30-msgHeight/2);
        m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, msgHeight + 50));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, msgHeight + 100));
        }
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if (!isTouchInside(m_buildBG, pTouch)) {
                this->onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
            }
        });
        
        ret = true;
    }
    return ret;
}

void TipsView::onEnter() {
    PopupBaseView::onEnter();
}

void TipsView::onExit() {
    PopupBaseView::onExit();
}

SEL_CCControlHandler TipsView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", TipsView::onClickCloseBtn);
    return NULL;
}

bool TipsView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    return false;
}

void TipsView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}
TipsVipView* TipsVipView::create(string title,string msg) {
    auto ret = new TipsVipView();
    if (ret && ret->init(title,msg)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TipsVipView::init(string title,string msg) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("TipsVIPView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        CCCommonUtils::setButtonTitle(m_btnOk, _lang("confirm").c_str());
        
        m_msgLabel->setString(msg);
        m_msgLabelTitle->setString(title);
        int msgHeight = m_msgLabel->getContentSize().height*m_msgLabel->getOriginScaleY();
        int sumHeight = msgHeight + m_btnOk->getContentSize().height * m_btnOk->getScaleY() + 160;
        if (CCCommonUtils::isIosAndroidPad())
        {
            sumHeight += 80;
        }
        
        m_btnOk->setPositionY(m_btnOk->getContentSize().height * m_btnOk->getScaleY()/2+30 -sumHeight/2);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_btnOk->setPositionY(m_btnOk->getPositionY() + 30);
        }

        int dh = sumHeight-m_buildBG->getContentSize().height;
        m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, sumHeight));
        m_upNode->setPositionY(m_upNode->getPositionY()+dh/2);
        m_msgLabel->setPositionY(m_upNode->getPositionY());
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if (!isTouchInside(m_buildBG, pTouch)) {
                this->onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
            }
        });
        
        ret = true;
    }
    return ret;
}

void TipsVipView::onEnter() {
    PopupBaseView::onEnter();
}

void TipsVipView::onExit() {
    PopupBaseView::onExit();
}

SEL_CCControlHandler TipsVipView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", TipsVipView::onClickCloseBtn);
    return NULL;
}

bool TipsVipView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabelTitle", CCLabelIF*, m_msgLabelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, m_upNode);
    return false;
}

void TipsVipView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}
//
TipsGuideView* TipsGuideView::create(string msg,float dy,CCTextAlignment align) {
    auto ret = new TipsGuideView();
    if (ret && ret->init(msg,dy,align)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TipsGuideView::init(string msg,float dy,CCTextAlignment align) {
    bool ret = false;
    if (PopupBaseView::init()) {
       _modelLayer->setOpacity(0);
        CCBLoadFile("TipsGuideViewCCB",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        float posY=dy;
        if(posY==0){
            posY=size.height/2;
        }
        m_totalNode->setPositionY(posY);
        //        CCCommonUtils::setButtonTitle(m_btnOk, _lang("confirm").c_str());
        m_msgLabel->setAlignment(align);
        m_msgLabel->setString(msg);
        int msgHeight = m_msgLabel->getContentSize().height*m_msgLabel->getOriginScaleY();
        //        int sumHeight = msgHeight + m_btnOk->getContentSize().height +100;
        //
        //        m_btnOk->setPositionY(m_btnOk->getContentSize().height/2+30 -sumHeight/2);
        //        m_msgLabel->setPositionY(sumHeight/2-30-msgHeight/2);
//        m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, msgHeight + 50));
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
//            // detect touch inside panel
//            if (!isTouchInside(m_buildBG, pTouch)) {
                this->onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
//            }
        });
        
        ret = true;
    }
    return ret;
}

void TipsGuideView::onEnter() {
    PopupBaseView::onEnter();
}

void TipsGuideView::onExit() {
    PopupBaseView::onExit();
}

SEL_CCControlHandler TipsGuideView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", TipsView::onClickCloseBtn);
    return NULL;
}

bool TipsGuideView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    return false;
}

void TipsGuideView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}


//

TipsNewView* TipsNewView::create(string title, string msg,CCTextAlignment align, float titleFontSize/*=0.0*/, float infoFontSize/*=0.0*/) {
    auto ret = new TipsNewView();
    if (ret && ret->init(title, msg,align,titleFontSize,infoFontSize)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TipsNewView::init(string title, string msg,CCTextAlignment align,float titleFontSize, float infoFontSize) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("TipsNewView02",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        if (titleFontSize > FLT_MIN)
        {
            m_titleLabel->setFontSize(titleFontSize);
        }
        if (titleFontSize > FLT_MIN)
        {
            m_msgLabel->setFontSize(infoFontSize);
        }
        m_titleLabel->setAlignment(align);
        m_titleLabel->setString(title);
        m_msgLabel->setAlignment(align);
        m_msgLabel->setString(msg);
        
        int titleHight = m_titleLabel->getContentSize().height*m_titleLabel->getOriginScaleY();
        int msgHeight = m_msgLabel->getContentSize().height*m_msgLabel->getOriginScaleY();
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, titleHight+msgHeight+100));
        } else {
            m_buildBG->setContentSize(CCSizeMake(m_buildBG->getContentSize().width, titleHight+msgHeight+50));
        }
        
        
        CCSize bgSize = m_buildBG->getContentSize();
        CCPoint bgPoint = m_buildBG->getPosition();
        if (CCCommonUtils::isIosAndroidPad()) {
            m_titleLabel->setPositionY(bgPoint.y + bgSize.height/2 - titleHight/2 - 20);
            m_buildBG2->setPositionY(m_titleLabel->getPositionY() - titleHight/2 - 10);
            m_msgLabel->setPositionY(m_titleLabel->getPositionY() - titleHight/2 - msgHeight/2 - 20);
        } else {
            m_titleLabel->setPositionY(bgPoint.y + bgSize.height/2 - titleHight/2 - 10);
            m_buildBG2->setPositionY(m_titleLabel->getPositionY() - titleHight/2 - 5);
            m_msgLabel->setPositionY(m_titleLabel->getPositionY() - titleHight/2 - msgHeight/2 - 10);
        }
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            if (!isTouchInside(m_buildBG, pTouch)) {
                this->onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
            }
        });
        
        ret = true;
    }
    return ret;
}

void TipsNewView::onEnter() {
    PopupBaseView::onEnter();
}

void TipsNewView::onExit() {
    PopupBaseView::onExit();
}

SEL_CCControlHandler TipsNewView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", TipsView::onClickCloseBtn);
    return NULL;
}

bool TipsNewView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG2", CCScale9Sprite*, m_buildBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    return false;
}

void TipsNewView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}
