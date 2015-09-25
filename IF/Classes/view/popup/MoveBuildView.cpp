//
//  MoveBuildView.cpp
//  IF
//
//  Created by fubin on 15/2/5.
//
//

#include "MoveBuildView.h"
#include "FunBuildController.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "YesNoDialog.h"

MoveBuildView* MoveBuildView::create() {
    auto ret = new MoveBuildView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MoveBuildView::init() {
    bool ret = false;
    if (PopupBaseView::init()) {
        setCleanFunction([](){
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        CCBLoadFile("MoveBuildView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(ITEM_MOVE_BUILD);
        m_btnMsgLabel->setString(_lang("104082"));
        m_toolMsgLabel->setString(_lang("104082"));
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(ITEM_MOVE_BUILD));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
        CCCommonUtils::setSpriteMaxSize(icon, 60);
        m_iconNode->addChild(icon);
        
        m_inBtnGoldNum->setString(CC_ITOA(toolInfo.price));
        
        if (toolInfo.getCNT() > 0) {
            m_gold = 0;
            m_goldNode->setVisible(false);
            m_toolNode->setVisible(true);
        }else {
            m_gold = toolInfo.price;
            m_goldNode->setVisible(true);
            m_toolNode->setVisible(false);
            if ( m_gold>GlobalData::shared()->playerInfo.gold ) {
                m_inBtnGoldNum->setColor(ccRED);
            }
        }
        m_infoLabel->setString(_lang("102369"));
        
        ret = true;
    }
    return ret;
}

void MoveBuildView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void MoveBuildView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void MoveBuildView::onOKClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_gold>0 && m_gold>GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        UIComponent::getInstance()->onCancelMoveBuild();
    }else {
        FunBuildController::getInstance()->StartMoveBuild(m_gold);
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler MoveBuildView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", MoveBuildView::onOKClick);
    return NULL;
}

bool MoveBuildView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", CCNode*, m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolNode", CCNode*, m_toolNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolMsgLabel", CCLabelIF*, m_toolMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}

bool MoveBuildView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void MoveBuildView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    FunBuildController::getInstance()->CancelLastSetPos();
    PopupViewController::getInstance()->removePopupView(this);
}