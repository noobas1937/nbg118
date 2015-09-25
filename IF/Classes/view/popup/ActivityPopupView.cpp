//
//  ActivityPopupView.cpp
//  IF
//
//  Created by xxrdsg on 15-7-9.
//
//

#include "ActivityPopupView.h"
#include "ActivityController.h"
#include "SceneContainer.h"
#include "ImperialScene.h"
#include "FunBuildController.h"
#include "UIComponent.h"
#include "ActPopTimesCommand.h"

ActivityPopupView* ActivityPopupView::create(string itemId)
{
    auto ret = new ActivityPopupView(itemId);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityPopupView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    auto _rootNode = CCBLoadFile("ActivityHistoryView02", this, this);
    if (CCCommonUtils::isIosAndroidPad())
    {
        _rootNode->setScale(1.8f);
    }
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    m_obj = ActivityController::getInstance()->getActObj(m_itemId);
    if (!m_obj) {
        return false;
    }
    CCCommonUtils::setButtonTitle(m_jumpBtn, _lang("115018").c_str());
    m_titleLabel->setString(_lang("137516"));
    m_nameLabel->setString(m_obj->name);
    if (m_obj->story == "") {
        m_storyLabel->setString(_lang(m_obj->desc_info));
    } else {
        m_storyLabel->setString(_lang(m_obj->story));
    }
    auto listSize = m_listNode->getContentSize();
    auto labelSize = CCSize(m_storyLabel->getContentSize().width * m_storyLabel->getOriginScaleX(), m_storyLabel->getContentSize().height * m_storyLabel->getOriginScaleY());
    if (labelSize.height > listSize.height) {
        auto scrollView = CCScrollView::create(listSize);
        scrollView->setDirection(kCCScrollViewDirectionVertical);
        scrollView->setAnchorPoint(ccp(0, 0));
        scrollView->setContentSize(CCSize(listSize.width, labelSize.height));
        m_labelNode->getParent()->removeChild(m_labelNode, false);
        m_labelNode->setPosition(ccp(0, 0));
        scrollView->addChild(m_labelNode);
        scrollView->setPosition(ccp(0, 0));
        m_listNode->addChild(scrollView);
        scrollView->setContentOffset(ccp(0, listSize.height - scrollView->getContentSize().height));
    } else {
        m_labelNode->setPosition(ccp(0, listSize.height / 2 - labelSize.height / 2));
    }
    return true;
}

void ActivityPopupView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    GameController::getInstance()->callXCApi("action=activity_popup");
    auto cmd = new ActPopTimesCommand(GlobalData::shared()->popupTimes);
    cmd->sendAndRelease();
}

void ActivityPopupView::onExit()
{
    setTouchEnabled(false);
   // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool ActivityPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storyLabel", CCLabelIF*, m_storyLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpBtn", CCControlButton*, m_jumpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labelNode", CCNode*, m_labelNode);
    return false;
}

SEL_CCControlHandler ActivityPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickJumpBtn", ActivityPopupView::onClickJumpBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", ActivityPopupView::onCloseBtnClick);
    return nullptr;
}

bool ActivityPopupView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void ActivityPopupView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch)) {
        closeSelf();
    }
}

void ActivityPopupView::onClickJumpBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
    GameController::getInstance()->callXCApi("action=activity_popup_click");
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN)
    {
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer)
        {
            layer->onMoveToSpeBuildAndPlay(SPE_BUILD_HD);
            UIComponent::getInstance()->hideQuestRect();
        }
    }
}

void ActivityPopupView::onCloseBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
}
