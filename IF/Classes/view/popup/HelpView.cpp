//
//  HelpView.cpp
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#include "HelpView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"

HelpView* HelpView::create(){
    HelpView* ret = new HelpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HelpView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    auto tmpCCB = CCBLoadFile("BuildMoreInfoView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    CCLOG("old x, y: %f, %f", m_infoList->getPositionX(), m_infoList->getPositionY());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
//    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCLOG("cur x, y: %f, %f", m_infoList->getPositionX(), m_infoList->getPositionY());
    
    m_webView = CCWebView::create(ccp(0, 0), m_infoList->getContentSize(), m_infoList);
    m_webView->loadUrl("http://news.qq.com/a/20140408/012675.htm");
    
    return true;
}

void HelpView::onEnter(){
    CCNode::onEnter();
}

void HelpView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler HelpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillBtnClick", UserUpgradeView::onSkillBtnClick);
    return NULL;
}

bool HelpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return false;
}