//
//  NpcTalkView.cpp
//  IF
//
//  Created by fubin on 14-11-11.
//
//

#include "NpcTalkView.h"
#include "GlobalData.h"
#include "PopupViewController.h"
#include "ImperialScene.h"
#include "SceneController.h"

NpcTalkView *NpcTalkView::create(std::string dialogs) {
    auto ret = new NpcTalkView(dialogs);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NpcTalkView::init() {
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        auto node = CCBLoadFile("NpcTalkView",this,this);
        this->setContentSize(node->getContentSize());
        m_iconNode->setPositionX(m_iconNode->getPositionX()-300);
        m_txtNode->setPositionX(m_txtNode->getPositionX()+300);
        refreshWord();
    }
    return true;
}

bool NpcTalkView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, m_nameText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, m_txtNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    return false;
}

SEL_CCControlHandler NpcTalkView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", NewPlotView::onCloseClick);
    return NULL;
}

void NpcTalkView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void NpcTalkView::refreshWord()
{
    m_contentText->setString(m_dialogs);
    
//    m_iconNode->removeAllChildren();
//    std::string body = GlobalData::shared()->playerInfo.getBodyPic();
//    std::string bodyIndex = body.substr(2,2);
//    int bindex = atoi(bodyIndex.c_str());
//    int index = bindex>26?2:1;
//    CCLoadSprite::doResourceByGeneralIndex(index, true);
//    setCleanFunction([index](){
//        CCLoadSprite::doResourceByGeneralIndex(index, false);
//    });
//    
//    auto spr = CCLoadSprite::createSprite(body.c_str());
//    CCCommonUtils::setSpriteMaxSize(spr, 350, true);
//    spr->setAnchorPoint(ccp(0, 0));
//    m_iconNode->addChild(spr);
}

void NpcTalkView::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
    m_iconNode->runAction(CCMoveBy::create(0.2, ccp(300,0)));
    m_txtNode->runAction(CCMoveBy::create(0.2, ccp(-300,0)));
//    scheduleOnce(schedule_selector(NpcTalkView::onClose), 5);
}

void NpcTalkView::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void NpcTalkView::onClose()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer) {
        layer->reSetTalkTime();
    }
    PopupViewController::getInstance()->removePopupView(this);
}

bool NpcTalkView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}

void NpcTalkView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
}