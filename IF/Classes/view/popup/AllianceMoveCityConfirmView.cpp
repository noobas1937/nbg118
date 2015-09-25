//
//  AllianceMoveCityConfirmView.cpp
//  IF
//
//  Created by xxrdsg on 15-7-2.
//
//

#include "AllianceMoveCityConfirmView.h"
#include "ToolController.h"
#include "AlPointFindCommand.h"

AllianceMoveCityConfirmView* AllianceMoveCityConfirmView::create()
{
    auto ret = new AllianceMoveCityConfirmView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceMoveCityConfirmView::init()
{
    if (!PopupBaseView::init())
        return false;
    setIsHDPanel(true);
    if (!CCBLoadFile("AllianceMoveCityView", this, this)) {
        return false;
    }
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_desLabel->setString(_lang("115422"));
    CCCommonUtils::setButtonTitle(m_terMoveBtn, _lang("115423").c_str());
    CCCommonUtils::setButtonTitle(m_headMoveBtn, _lang("115424").c_str());
    return true;
}

void AllianceMoveCityConfirmView::onClickTerMoveBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_ALLIANCE_CITY_MOVE);
    if (info.getCNT() <= 0) {
        closeSelf();
        return;
    }
    AlPointFindCommand* cmd = new AlPointFindCommand(1);
    cmd->sendAndRelease();
    closeSelf();
}
void AllianceMoveCityConfirmView::onClickHeadMoveBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_ALLIANCE_CITY_MOVE);
    if (info.getCNT() <= 0) {
        closeSelf();
        return;
    }
    AlPointFindCommand* cmd = new AlPointFindCommand(0);
    cmd->sendAndRelease();
    closeSelf();
}

void AllianceMoveCityConfirmView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
}

void AllianceMoveCityConfirmView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

cocos2d::SEL_MenuHandler AllianceMoveCityConfirmView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}
SEL_CCControlHandler AllianceMoveCityConfirmView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTerMoveBtn", AllianceMoveCityConfirmView::onClickTerMoveBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHeadMoveBtn", AllianceMoveCityConfirmView::onClickHeadMoveBtn);
    return NULL;
}
bool AllianceMoveCityConfirmView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char* pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_terMoveBtn", CCControlButton*, this->m_terMoveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headMoveBtn", CCControlButton*, this->m_headMoveBtn);
    return false;
}

bool AllianceMoveCityConfirmView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void AllianceMoveCityConfirmView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch)) {
        closeSelf();
    }
}