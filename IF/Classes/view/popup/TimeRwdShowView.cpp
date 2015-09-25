//
//  TimeRwdShowView.cpp
//  IF
//
//  Created by xxrdsg on 15-3-24.
//
//

#include "TimeRwdShowView.h"
#include "ActivityController.h"
#include "ToolController.h"
#include "PopupViewController.h"
#include "AllianceManager.h"

TimeRwdShowView* TimeRwdShowView::create()
{
    TimeRwdShowView* ret = new TimeRwdShowView();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TimeRwdShowView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    CCBLoadFile("TimeRwdShowView", this, this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    string rewardInfo = PortActController::getInstance()->m_rewardInfo;
    if (rewardInfo == "")
        return false;
    vector<string> vec;
    CCCommonUtils::splitString(rewardInfo, "|", vec);
    if (vec.size() == 0) {
        return false;
    }
    rewardInfo = vec[0];
    vec.clear();
    CCCommonUtils::splitString(rewardInfo, ",", vec);
    if (vec[0] != "goods") {
        return false;
    }
    int num = atoi(vec[2].c_str());
    string pic = "";
    string name = "";
    string des = "";
    string bg = "";
    string toolId = vec[1];
    
    pic = CCCommonUtils::getIcon(toolId);
    name = CCCommonUtils::getNameById(toolId);
    if (num > 1) {
        name.append("   X");
        name.append(CC_ITOA(num));
    }
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolId.c_str()));
    des = toolInfo.des;
    bg = CCCommonUtils::getToolBgByColor(toolInfo.color);
    
    m_nameLabel->setString(name);
    m_desLabel->setString(_lang(des));
    auto bgSpr = CCLoadSprite::createSprite(bg.c_str());
    auto picSpr = CCLoadSprite::createSprite(pic.c_str());
    CCCommonUtils::setSpriteMaxSize(picSpr, 100,true);
    CCCommonUtils::setSpriteMaxSize(bgSpr, 107,true);
    m_bgNode->addChild(bgSpr);
    m_picNode->addChild(picSpr);
    
//    CCCommonUtils::setButtonTitle(m_btnOk, _lang("105036").c_str());
    m_btnTitle->setString(_lang("105036").c_str());
    if (CCCommonUtils::isIosAndroidPad())
    {
        float fw = m_btnTitle->getContentSize().width;
        if (fw>440.0) {//防爆框
            setScaleX(440.0/fw * 0.5);
        }
    } else {
        m_btnTitle->setMaxScaleXByWidth(220.0);
    }
    
    m_waitInterface = NULL;
    return true;
}

void TimeRwdShowView::onClickOkBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_btnOk);
    AllianceManager::getInstance()->showRecAlliance();
    PortActController::getInstance()->startTimeRwd();
}

void TimeRwdShowView::afterGetRwd(CCObject* p)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void TimeRwdShowView::onEnter()
{
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TimeRwdShowView::afterGetRwd), MSG_NEW_TIME_RWD_END, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void TimeRwdShowView::onExit()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool TimeRwdShowView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void TimeRwdShowView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler TimeRwdShowView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOkBtn", TimeRwdShowView::onClickOkBtn);
    return NULL;
}

bool TimeRwdShowView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTitle", CCLabelIF*, m_btnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, m_btnOk);
    return false;
}