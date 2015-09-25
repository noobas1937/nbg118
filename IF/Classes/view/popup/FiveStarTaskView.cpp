//
//  FiveStarTaskView.cpp
//  IF
//
//  Created by xingxiaorui on 14-11-5.
//
//

#include "FiveStarTaskView.h"
#include "SetAccountView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "LoginServiceView.h"
#include "NetController.h"
#include "FBUtilies.h"
#include "../../Ext/CCDevice.h"
#include "UserBindCommand.h"
#include "SuggestionView.h"
#include "PopupViewController.h"
#include "SettingPopUpView.h"
#include "RegisterView.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "WorldController.h"
#include "QuestRewardCommand.h"


FiveStarTaskView* FiveStarTaskView::create() {
    auto ret = new FiveStarTaskView();
    if (ret && ret->init()){
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FiveStarTaskView::init() {
    bool ret = false;
    if (PopupBaseView::init()) {
        CCBLoadFile("FiveStarTask",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        m_titleLabel->setString(_lang("2000212"));
        m_infoLabel->setString(_lang("2000213"));
        CCCommonUtils::setButtonTitle(m_goBtn, _lang("2000214").c_str());
        ret = true;
    }
    return ret;
}

void FiveStarTaskView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void FiveStarTaskView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void FiveStarTaskView::onGoClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    FSTaskCommand * cmd = new FSTaskCommand(1);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removePopupView(this);
    GameController::getInstance()->updateVersion();
}

SEL_CCControlHandler FiveStarTaskView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoClick", FiveStarTaskView::onGoClick);
    return NULL;
}

bool FiveStarTaskView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goBtn", CCControlButton*, m_goBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    return false;
}

bool FiveStarTaskView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void FiveStarTaskView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}
