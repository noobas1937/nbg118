//
//  Test1.cpp
//  IF
//
//  Created by xxrdsg on 15-7-16.
//
//

#include "TrainingView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ParticleController.h"
#include "ActivityController.h"
#include "ToolController.h"
#include "TrialCommand.h"
#include "GoldExchangeView.h"
#include "YesNoDialog.h"
#include "SoundController.h"

Training1View* Training1View::create()
{
    auto ret = new Training1View();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Training1View::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(509, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(509, false);
    });
    this->setContentSize(CCSize(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1536, 2048));
    }
    CCBLoadFile("Training1View", this, this);
    auto size = CCDirector::sharedDirector()->getWinSize();
    float dh = size.height - 852;
    if (CCCommonUtils::isIosAndroidPad()) {
        dh = size.height - 2048;
        dh = dh / 2.4;
    }
    m_bg->setPreferredSize(CCSize(size.width, size.height));
    if (dh <= 75) {
        m_buildBg->setPositionY(m_buildBg->getPositionY() - dh);
        m_middleNode->setPositionY(m_middleNode->getPositionY() - dh);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - dh);
    } else {
        m_buildBg->setPositionY(m_buildBg->getPositionY() - 75);
        m_middleNode->setPositionY(m_middleNode->getPositionY() - 75);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - dh);
        m_OKBtn->setPositionY(m_OKBtn->getPositionY() + (dh - 75) / 2);
        float scale = (size.height - 75) / m_buildBg->getContentSize().height;
        if (m_buildBg->getContentSize().height * m_buildBg->getScaleY() < (size.height - 75))
        {
            m_buildBg->setScaleY(scale);
        }
    }
    m_infoLabel->setString(_lang("137526"));
    m_woman->setVisible(true);
    refreshInterface();
    return true;
}

void Training1View::setMyType()
{
//    if (ActivityController::getInstance()->m_monster == "") {
//        m_type = 1;
//    } else {
//        if (ActivityController::getInstance()->m_freeAtk <= 0 ) {//判断是否已经用完攻击次数
//            m_type = 3;
//        } else {
//            m_type = 2;
//        }
//    }
    m_type = ActivityController::getInstance()->getMyTrialState();
}

void Training1View::refreshInterface()
{
    setMyType();
    setButtonTitle();
}

void Training1View::onOKClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_type == 1 || m_type == 5) {
        PopupViewController::getInstance()->addPopupView(TrainingConfirmView::create(1));
    } else if (m_type == 2) {
        m_OKBtn->setEnabled(false);
        showCountDown();
    } else if (m_type == 3 || m_type == 4) {
        PopupViewController::getInstance()->addPopupView(TrainingRwdView::create());
    }
}

void Training1View::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training1View::onGetMsgTrialStartSuc), MSG_TRIAL_START_SUC, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training1View::onGetMsgTrialFininshSuc), MSG_TRIAL_FINISH_SUC, NULL);
    refreshInterface();
    setTitleName(_lang("137525"));
}

void Training1View::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRIAL_FINISH_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRIAL_START_SUC);
    CCNode::onExit();
}

void Training1View::onGetMsgTrialStartSuc(cocos2d::CCObject * obj)
{
    if (m_type != 1 && m_type != 5) return;
    m_OKBtn->setEnabled(false);
    showCountDown();
}

void Training1View::onGetMsgTrialFininshSuc(cocos2d::CCObject * obj)
{
    refreshInterface();
}

void Training1View::showCountDown()
{
    m_countNode->removeAllChildrenWithCleanup(true);
    m_countNode->runAction(CCSequence::create(
                                      CCCallFuncO::create(this, callfuncO_selector(Training1View::setCountLabel), CCString::create("3")),
                                        CCSpawn::create(CCScaleTo::create(0.1, 1.0), CCDelayTime::create(1.0), nullptr),
                                        CCCallFuncO::create(this, callfuncO_selector(Training1View::setCountLabel), CCString::create("2")),
                                        CCSpawn::create(CCScaleTo::create(0.1, 1.0), CCDelayTime::create(1.0), nullptr),
                                        CCCallFuncO::create(this, callfuncO_selector(Training1View::setCountLabel), CCString::create("1")),
                                        CCSpawn::create(CCScaleTo::create(0.1, 1.0), CCDelayTime::create(1.0), nullptr),
                                        CCCallFunc::create(this, callfunc_selector(Training1View::openTraining2View)),
                                        CCFadeOut::create(0.1f),
                                        NULL));
    
}

void Training1View::openTraining2View()
{
    m_countNode->removeAllChildrenWithCleanup(true);
    m_OKBtn->setEnabled(true);
    PopupViewController::getInstance()->addPopupInView(Training2View::create());
}

void Training1View::setCountLabel(cocos2d::CCObject *obj)
{
    m_countNode->removeAllChildrenWithCleanup(true);
    CCString* s = dynamic_cast<CCString*>(obj);
    string name = "training_";
    name += s->getCString();
    name += ".png";
    auto spr = CCLoadSprite::createSprite(name.c_str());
    spr->setAnchorPoint(ccp(0.5, 0.5));
    m_countNode->setScale(3);
    m_countNode->addChild(spr);
}

void Training1View::setButtonTitle()
{
    if (m_type == 1 || m_type == 5)
    {
        CCCommonUtils::setButtonTitle(m_OKBtn, _lang("137527").c_str());
    } else {
        CCCommonUtils::setButtonTitle(m_OKBtn, _lang("137528").c_str());
    }
}

bool Training1View::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBg", CCSprite*, m_buildBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_OKBtn", CCControlButton*, m_OKBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countNode", CCNode*, m_countNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woman", CCSprite*, m_woman);
    return false;
}

SEL_CCControlHandler Training1View::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", Training1View::onOKClick);
    return nullptr;
}

TrainingConfirmView* TrainingConfirmView::create(int type)
{
    auto ret = new TrainingConfirmView(type);
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TrainingConfirmView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    CCBLoadFile("TrainingConfirmView", this, this);
    
    if (m_type == 1) {
        if (!ActivityController::getInstance()->checkIsToday(ActivityController::getInstance()->m_trialRefTime))
            ActivityController::getInstance()->m_freeTrial = 1;
        if (ActivityController::getInstance()->m_freeTrial > 0) {
            m_leftLabel->setString(_lang("137530"));
            m_leftIcon->setVisible(false);
            m_leftNumLabel->setString("");
        } else {
            m_leftLabel->setString(_lang("137531"));
            m_leftLabel->setPositionY(m_leftLabel->getPositionY() + 10);
            m_leftNumLabel->setString(CC_ITOA(ActivityController::getInstance()->m_newTrialGold));
            m_leftIcon->setVisible(true);
        }
        
        ActivityController::getInstance()->createGoodsIcon(m_iconNode, CC_ITOA(ITEM_TRIAL_TICKET), 50);
        m_numLabel->setString("1");
        m_rightLabel->setString(_lang("137531"));
        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_TRIAL_TICKET);
        if (info.getCNT() <= 0) {
            m_rightBtn->setEnabled(false);
        }
        
        m_infoLabel->setString(_lang("137529"));
    } else {
        if (ActivityController::getInstance()->m_freeAtk > 0) return false;
        m_rightLabel->setPositionY(m_rightLabel->getPositionY() - 10);
        m_iconNode->removeAllChildrenWithCleanup(true);
        m_numLabel->setString("");
        m_rightLabel->setString(_lang("137536"));
        m_leftLabel->setString(_lang("101274"));
        m_leftLabel->setPositionY(m_leftLabel->getPositionY() + 10);
        m_leftIcon->setVisible(true);
        m_leftNumLabel->setString(CC_ITOA(ActivityController::getInstance()->m_moreAtkGold));
        m_infoLabel->setString(_lang_1("137534", CC_ITOA(ActivityController::getInstance()->m_addedAtkCnt)));
        int state = ActivityController::getInstance()->getMyTrialState();
        if (state == 5) {
            m_infoLabel->setString(_lang("137546"));
        }
    }
    
    return true;
}

void TrainingConfirmView::onEnter()
{
    setTouchEnabled(true);
    CCNode::onEnter();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
}

void TrainingConfirmView::onExit()
{
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}


bool TrainingConfirmView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, this->m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftLabel", CCLabelIF*, this->m_leftLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightLabel", CCLabelIF*, this->m_rightLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBtn", CCControlButton*, this->m_leftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBtn", CCControlButton*, this->m_rightBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftIcon", CCSprite*, this->m_leftIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftNumLabel", CCLabelIF*, this->m_leftNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}

SEL_CCControlHandler TrainingConfirmView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickLeftBtn", TrainingConfirmView::onClickLeftBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRightBtn", TrainingConfirmView::onClickRightBtn);
    return nullptr;
}

bool TrainingConfirmView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void TrainingConfirmView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10)
    {
        if (!isTouchInside(m_touchNode, pTouch)) {
            closeSelf();
            if (m_type == 2)
            {
                PopupViewController::getInstance()->removeAllPopupView();
                PopupViewController::getInstance()->addPopupInView(Training1View::create());
                int state = ActivityController::getInstance()->getMyTrialState();
                if (state == 3)
                {
                    PopupViewController::getInstance()->addPopupView(TrainingRwdView::create());
                }
            }
        }
    }
}

void TrainingConfirmView::onClickRightBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
    if (m_type == 1)
    {
        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_TRIAL_TICKET);
        if (info.getCNT() > 0)
        {
            auto cmd = new TrialStartCmd(1);
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
        }
    } else {
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(Training1View::create());
        int state = ActivityController::getInstance()->getMyTrialState();
        if (state == 3)
        {
            PopupViewController::getInstance()->addPopupView(TrainingRwdView::create());
        }
    }
}

void TrainingConfirmView::onClickLeftBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    
    if (m_type == 1) {
        closeSelf();
        if (ActivityController::getInstance()->m_freeTrial > 0) {
            auto cmd = new TrialStartCmd(0);
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
        } else {
            if (GlobalData::shared()->playerInfo.gold >= ActivityController::getInstance()->m_newTrialGold) {
                auto cmd = new TrialStartCmd(2);
                cmd->sendAndRelease();
                GameController::getInstance()->showWaitInterface();
            } else {
                YesNoDialog::gotoPayTips();
            }
        }
    } else {
        if (ActivityController::getInstance()->m_moreAtkGold > GlobalData::shared()->playerInfo.gold)
        {
            YesNoDialog::gotoPayTips();
        } else {
            closeSelf();
            GameController::getInstance()->showWaitInterface();
            auto cmd = new TrialBuyCmd();
            cmd->sendAndRelease();
        }
    }
}

bool Training2View::init()
{
    if (!PopupBaseView::init())
    {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(206, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(206, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
    });
    setContentSize(CCSize(640, 852));
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(1536, 2048));
    }
    CCBLoadFile("Training2View", this, this);
    auto size = CCDirector::sharedDirector()->getWinSize();
    int dh = size.height - 852;
    if (CCCommonUtils::isIosAndroidPad()) {
        dh = size.height - 2048;
        dh = dh / 2.4;
    }
    m_buildBG->setPreferredSize(size);
    m_buildBG->setPositionY(m_buildBG->getPositionY() - dh);
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - dh);
    m_leftBg1->setPreferredSize(CCSize(m_leftBg1->getPreferredSize().width, 506));
    m_leftBg2->setPreferredSize(CCSize(m_leftBg2->getPreferredSize().width, 520 + dh));
    m_rightBg1->setPreferredSize(CCSize(m_rightBg1->getPreferredSize().width, 506));
    m_rightBg2->setPreferredSize(CCSize(m_rightBg2->getPreferredSize().width, 520 + dh));
    m_tNode->setPositionY(m_tNode->getPositionY() - dh);
    m_bNode->setPositionY(m_tNode->getPositionY());
    m_handNode->setPositionY(m_tNode->getPositionY());
    m_midNode1->setPositionY(m_midNode1->getPositionY() - dh);
    m_midNode2->setPositionY(m_midNode2->getPositionY() - dh);
    float scaleX = 640 * 1.0 / m_monsterBg->getContentSize().width;
    float scaleY = (220 + dh) * 1.0 / m_monsterBg->getContentSize().height;
    scaleX = scaleX < 1? 1 : scaleX;
    scaleY = scaleY < 1? 1 : scaleY;
    m_monsterBg->setScaleX(scaleX);
    m_monsterBg->setScaleY(scaleY);
    m_parNode->setPositionY(m_parNode->getPositionY() - dh / 2);
    
    m_clipNode = CCClipNode::create(m_proBar->getContentSize());
    m_clipNode->setAnchorPoint(ccp(0, 0.5));
    m_proBar->getParent()->addChild(m_clipNode);
    
    m_proBar->removeFromParent();
    m_clipNode->addChild(m_proBar);
    m_proBar->setZOrder(0);
    m_proBar->setAnchorPoint(ccp(0, 0));
    m_proBar->setPosition(CCPointZero);
    
    m_clipNode2 = CCClipNode::create(CCSizeZero);
    m_clipNode2->setAnchorPoint(ccp(0, 0.5));
    m_clipNode->getParent()->addChild(m_clipNode2);
    
//    m_monsterNode->setScale(0.7);
    m_fixedWireNode = CCNode::create();
    m_bNode->addChild(m_fixedWireNode, 1);
    m_parAniNode = new CCAniNode();
    m_parNode->addChild(m_parAniNode);
    m_parAniNode->release();
    m_isInAtk = false;
    m_isInTouching = false;
    m_isInCellWiring = false;
    m_isUseBuf = false;
    m_touchedIdxes.clear();
    m_iToPMap.clear();
    m_addedSpr.clear();
    
    m_handSpr = CCLoadSprite::createSprite("UI_hand.png");
    m_handSpr->setAnchorPoint(ccp(0, 0));
    m_handSpr->setRotation(-12);
    m_handSpr->setVisible(false);
    m_handNode->addChild(m_handSpr);
    
    for (int i = 1; i <= 16; ++i) {
        int y = (i - 1) / 4;
        int x = (i - 1) % 4;
        m_iToPMap[i] = ccp(x, y);
    }
    
    refreshInterface();
    return true;
}

bool Training2View::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_proBar", CCSprite*, this->m_proBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bloodLabel", CCLabelIF*, this->m_bloodLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftCntLabel", CCLabelIF*, this->m_leftCntLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg1", CCScale9Sprite*, this->m_rightBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg2", CCScale9Sprite*, this->m_rightBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg1", CCScale9Sprite*, this->m_leftBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg2", CCScale9Sprite*, this->m_leftBg2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_atkBtn", CCControlButton*, this->m_atkBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bufBtn", CCControlButton*, this->m_bufBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, this->m_bNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tNode", CCNode*, this->m_tNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, this->m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterNode", CCNode*, this->m_monsterNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftLabel", CCLabelIF*, this->m_leftLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightLabel", CCLabelIF*, this->m_rightLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNumLabel", CCLabelIF*, this->m_rightNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode1", CCNode*, this->m_midNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode2", CCNode*, this->m_midNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterBg", CCSprite*, this->m_monsterBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handNode", CCNode*, this->m_handNode);
    return false;
}

SEL_CCControlHandler Training2View::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAtkBtn", Training2View::onClickAtkBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBufBtn", Training2View::onClickBufBtn);
    return nullptr;
}

void Training2View::onEnterFrame(float dt)
{
    if (GlobalData::shared()->getTimeStamp() - m_actionTime > 3) {
        playHandAction();
    }
}

vector<int> Training2View::getTwoLegalNeighbor()
{
    vector<int> ret;
    for (int i = 1; i < 16; ++i) {
        int j = i + 1;
        if (isLegalNeighbor(i, j)) {
            ret.push_back(i);
            ret.push_back(j);
            break;
        }
        j = i + 5;
        if (isLegalNeighbor(i, j)) {
            ret.push_back(i);
            ret.push_back(j);
            break;
        }
        j = i + 4;
        if (isLegalNeighbor(i, j)) {
            ret.push_back(i);
            ret.push_back(j);
            break;
        }
    }
    return ret;
}

void Training2View::playHandAction()
{
    if (m_touchedIdxes.size() > 0) {
        return;
    }
    if (GlobalData::shared()->getTimeStamp() - m_actionTime < 3) {
        return;
    }
    if (m_isInAtk || m_isInCellWiring || m_isInTouching) {
        return;
    }
    if (ActivityController::getInstance()->getMyTrialState() != 2) {
        return;
    }
    vector<int> vec = getTwoLegalNeighbor();
    if (vec.size() <= 1) {
        return;
    }
    if (!isLegalNeighbor(vec[0], vec[1])) {
        return;
    }
    m_handSpr->stopAllActions();
    m_handSpr->setScale(0);
    CCPoint p1 = m_tNode->getChildByTag(vec[0])->getPosition();
    CCPoint p2 = m_tNode->getChildByTag(vec[1])->getPosition();
    m_handSpr->setPosition(p1);
    m_handSpr->setVisible(true);
    m_handSpr->setOpacity(255);
    m_handSpr->runAction(CCSequence::create(CCScaleTo::create(0.07 * 5, 1.2),
                                            CCScaleTo::create(0.07 * 3, 0.8),
                                            CCScaleTo::create(0.07 * 3, 1.0),
                                            CCDelayTime::create(0.07 * 10),
                                            CCMoveTo::create(0.07 * 12, p2),
                                            CCDelayTime::create(0.07),
                                            CCFadeOut::create(0.07 * 15),
                                            nullptr));
    m_actionTime = ceil(GlobalData::shared()->getTimeStamp() + 0.07 * 50);
}

void Training2View::hideHandAction()
{
    m_handSpr->setVisible(false);
}

bool Training2View::isIdsValid()
{
    if (m_touchedIdxes.size() <= 0) return false;
    int t = ActivityController::getInstance()->m_screen.at(m_touchedIdxes.at(0) - 1);
    bool ret = true;
    vector<int> vec;
    for (int i = 0; i < m_touchedIdxes.size(); ++i) {
        if (m_touchedIdxes.at(i) > ActivityController::getInstance()->m_screen.size())
        {
            ret = false;
            break;
        }
        int temp = ActivityController::getInstance()->m_screen.at(m_touchedIdxes.at(i) - 1);
        if (temp != t) {
            ret = false;
            break;
        }
        if (i >= 1) {
            if (!isLegalNeighbor(m_touchedIdxes.at(i - 1), m_touchedIdxes.at(i))) {
                ret = false;
                break;
            } else {
                auto it = find(vec.begin(), vec.end(), m_touchedIdxes.at(i));
                if (it != vec.end())
                {
                    ret = false;
                    break;
                }
            }
        }
        vec.push_back(m_touchedIdxes.at(i));
    }
    return ret;
}

void Training2View::onClickAtkBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_touchedIdxes.size() <= 0) return;
    if (!isIdsValid()) return;
    int state = ActivityController::getInstance()->getMyTrialState();
    if (state != 2) return;
    m_atkBtn->setEnabled(false);
    m_bufBtn->setEnabled(false);
    bool isLastMonsterWillBeKilled = false;
    float t = 0.5f;
    if (ActivityController::getInstance()->getKilledMonsterCnt() == ActivityController::getInstance()->getAllMonsterCnt() - 1)
    {
        int atkType = ActivityController::getInstance()->m_screen.at(m_touchedIdxes.at(0) - 1);
        int totalHarm = ActivityController::getInstance()->m_atkHarm.at(atkType) * m_touchedIdxes.size();
        if (m_isUseBuf) {
            totalHarm = totalHarm * ActivityController::getInstance()->m_bigStrenMul;
        }
        if (totalHarm >= ActivityController::getInstance()->m_blood)
        {
            isLastMonsterWillBeKilled = true;
        }
    }
    showAtkParByType(ActivityController::getInstance()->m_screen.at(m_touchedIdxes.at(0) - 1), isLastMonsterWillBeKilled);
    string totals = CC_ITOA((int)(m_touchedIdxes.size()));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_START_ATK, CCString::create(totals));
    m_isInAtk = true;
    m_mainNode->runAction(CCSequence::create(
                                             CCEaseSineIn::create(CCScaleTo::create(0.1, 1.2)),
                                             CCEaseSineIn::create(CCScaleTo::create(0.1, 1.0)),
                                             CCEaseSineIn::create(CCScaleTo::create(0.1, 1.2)),
                                             CCEaseSineIn::create(CCScaleTo::create(0.1, 1.0)),
                                             nullptr
                                             ));
    m_actionTime = GlobalData::shared()->getTimeStamp();
}

void Training2View::onClickBufBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_isUseBuf) return;
    m_actionTime = GlobalData::shared()->getTimeStamp();
    if (ActivityController::getInstance()->m_freeAtk <= 0)
    {
        m_bufBtn->setEnabled(false);
        m_atkBtn->setEnabled(false);
        return;
    }
    if (GlobalData::shared()->playerInfo.gold >= ActivityController::getInstance()->m_bigStrengthGold)
    {
        m_isUseBuf = true;
        m_bufBtn->setEnabled(false);
        refreshBlood();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_START_USE_BUF);
    } else {
        YesNoDialog::gotoPayTips();
    }
}

void Training2View::showAtkParByType(int type, bool isLastKilled)
{
    //0 普通攻击；1 雷电攻击；2 火球攻击
    m_parAniNode->removeAllChildrenWithCleanup(true);
    int picCnt = 7;
    int loop = 1;
    if (m_isUseBuf) {
        loop = 3;
    }
    string name = "ActTrialsAtkNormal_%d.png";
    if (type == 0)
    {
        name = "ActTrialsAtkNormal_%d.png";
        picCnt = 11;
    }
    else if (type == 1)
    {
        name = "ActTrialsAtkLighting_%d.png";
    }
    else if (type == 2)
    {
        name = "ActTrailsAtkFire_%d.png";
    }
    for (int i = 1; i <= loop; ++i)
    {
        auto spr = CCSprite::create();
        m_parAniNode->addChild(spr);
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(spr, "spr");
        dic->setObject(CCString::create(name), "name");
        dic->setObject(CCString::create(CC_ITOA(picCnt)), "picCnt");
        dic->setObject(CCInteger::create(type), "type");
        if (isLastKilled && i == loop)
        {
            dic->setObject(CCFloat::create(0.5), "dt");
            dic->setObject(CCBoolean::create(true), "isLastLoop");
            
        } else {
            dic->setObject(CCFloat::create(0.07), "dt");
            if (i == loop) {
                dic->setObject(CCBoolean::create(true), "isLastLoop");
            }
        }
        spr->runAction(CCSequence::create(CCDelayTime::create(picCnt * 0.07 * (i - 1)),
                                          CCCallFuncO::create(this, callfuncO_selector(Training2View::makeOnceEffectSpr), dic),
                                          nullptr));
    }
}

void Training2View::makeOnceEffectSpr(cocos2d::CCObject *obj)
{
    auto dic = _dict(obj);
    auto spr = dynamic_cast<CCSprite*>(dic->objectForKey("spr"));
    auto cdt = dynamic_cast<CCFloat*>(dic->objectForKey("dt"));
    int picCnt = dic->valueForKey("picCnt")->intValue();
    float dt = cdt->getValue();
    auto ctype = dynamic_cast<CCInteger*>(dic->objectForKey("type"));
    int type = ctype->getValue();
    CCCommonUtils::makeOnceEffectSpr(spr, dic->valueForKey("name")->getCString(), picCnt, 0, dt);
    if (type == 0) {
        spr->setBlendFunc({GL_ONE, GL_ONE});
    }
    bool isLastLoop = false;
    if (dic->objectForKey("isLastLoop")) {
        auto ccbool = dynamic_cast<CCBoolean*>(dic->objectForKey("isLastLoop"));
        isLastLoop = ccbool->getValue();
    }
    spr->setScale(4);
    spr->setAnchorPoint(ccp(0.5, 0.5));
    if (isLastLoop) {
        string ids = "";
        for (int i = 0; i < m_touchedIdxes.size(); ++i)
        {
            ids += CC_ITOA(m_touchedIdxes[i]);
            if (i < m_touchedIdxes.size() - 1) {
                ids += "|";
            }
        }
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(CCString::create(ids), "ids");
        dic->setObject(CCBoolean::create(m_isUseBuf),"gold");
        spr->runAction(CCSequence::create(CCDelayTime::create(picCnt * dt),
                                          CCCallFuncO::create(this, callfuncO_selector(Training2View::sendAtkCmd), dic),
                                          CCFadeOut::create(0.1),
                                          nullptr));
    } else {
        srand (time(NULL));
        int randomIndex = rand() % 200;
        randomIndex -= 100;
        spr->setPositionX(randomIndex);
        randomIndex = rand() % 100;
        randomIndex -= 50;
        spr->setPositionY(randomIndex);
        spr->runAction(CCSequence::create(CCDelayTime::create(picCnt * dt),
                                          CCFadeOut::create(0.1),
                                          nullptr));
    }
    
}

void Training2View::removeAtkPar()
{
    m_parAniNode->removeAllChildrenWithCleanup(true);
}

void Training2View::sendAtkCmd(cocos2d::CCObject *obj)
{
    auto dic = _dict(obj);
    auto cmd = new TrialAtkCmd(dic);
    cmd->sendAndRelease();
    ActivityController::getInstance()->m_isInAtkCmdSending = true;
}

bool Training2View::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_actionTime = GlobalData::shared()->getTimeStamp();
    if (m_isInAtk) {
        return false;
    }
    m_isInTouching = true;
    if (!m_isInCellWiring) {
        m_touchedIdxes.clear();
        removeAllFixedWire();
        refreshBlood();
    }
    return true;
}
void Training2View::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    m_actionTime = GlobalData::shared()->getTimeStamp();
//    refreshMovingWire(pTouch);
}
void Training2View::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_isInTouching = false;
    m_isInCellWiring = false;
    m_actionTime = GlobalData::shared()->getTimeStamp();
//    refreshMovingWire(pTouch);
}

void Training2View::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void Training2View::refreshFixedWire()
{
    
}

void Training2View::removeAllFixedWire()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRAINING_CELL_ALL_REMOVED);
    m_touchedIdxes.clear();
    m_fixedWireNode->removeAllChildrenWithCleanup(true);
    m_addedSpr.clear();
}

void Training2View::addOneFixedWire()
{
    if (m_touchedIdxes.size() >= 2) {
        auto it = m_touchedIdxes.end();
        --it;
        int endIdx = *it--;
        int startIdx = *it;
        CCPoint endPt = m_tNode->getChildByTag(endIdx)->getPosition();
        CCPoint startPt = m_tNode->getChildByTag(startIdx)->getPosition();
        CCPoint deltaPt = ccpSub(endPt, startPt);
        float dis = deltaPt.getLength();
        float angleRadians = deltaPt.getAngle();
        float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
        auto spr = CCLoadSprite::createScale9Sprite("training_line1.png");
        spr->setAnchorPoint(ccp(0,0.5));
        spr->setPreferredSize(CCSize(dis, 20));
        spr->setPosition(startPt);
        spr->setRotation(-angleDegrees);
        m_fixedWireNode->addChild(spr);
        m_addedSpr.push_back(spr);
        auto dic = CCDictionary::create();
        dic->setObject(CCString::createWithFormat("%d", *m_touchedIdxes.rbegin()) , "idx");
        dic->setObject(CCString::createWithFormat("%d", (int)(m_touchedIdxes.size())), "pos");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRAINING_CELL_ADDED, dic);
    }
}

void Training2View::removeFixedWireToIdx(int idx)
{
    if (!isHasTouched(idx)) {
        return;
    }
    auto it = m_touchedIdxes.rbegin();
    int cnt = 0;
    while (it != m_touchedIdxes.rend()) {
        if (*it != idx) {
            cnt++;
        } else {
            break;
        }
        ++it;
    }
    if (cnt == 0) {
        return;
    }
    while (cnt > 0) {
        m_fixedWireNode->removeChild(*(m_addedSpr.rbegin()), true);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRAINING_CELL_REMOVED, CCInteger::create(*m_touchedIdxes.rbegin()));
        m_addedSpr.pop_back();
        m_touchedIdxes.pop_back();
        cnt--;
    }
}

void Training2View::refreshCells()
{
    m_tNode->removeAllChildrenWithCleanup(true);
    CCPoint s(0, 132 * 3);
    for (int i = 1; i <= 16; ++i) {
        auto cell = TrainingCell::create(i);
        cell->setPosition(s.x + m_iToPMap[i].x * 146, s.y - m_iToPMap[i].y * 132);
        cell->setTag(i);
        m_tNode->addChild(cell);
    }
}

void Training2View::refreshBtnState()
{
    if (ActivityController::getInstance()->m_freeAtk <= 0) {
        m_atkBtn->setEnabled(false);
        m_bufBtn->setEnabled(false);
    } else {
        m_atkBtn->setEnabled(true);
        m_bufBtn->setEnabled(true);
    }
    m_leftLabel->setString(_lang("103600"));
    m_rightLabel->setString(_lang("137533"));
    m_rightNumLabel->setString(CC_ITOA(ActivityController::getInstance()->m_bigStrengthGold));
}

void Training2View::refreshMonster()
{
    m_leftCntLabel->setString(_lang_1("137532", CC_ITOA(ActivityController::getInstance()->m_freeAtk)));
    string blood = CC_ITOA(ActivityController::getInstance()->m_blood);
    blood += "/";
    blood += CC_ITOA(ActivityController::getInstance()->m_maxBlood);
    m_bloodLabel->setString(blood);
    refreshBlood();
//    float pro = ActivityController::getInstance()->m_blood * 1.0f / ActivityController::getInstance()->m_maxBlood;
//    m_clipNode->setContentSize(CCSize(pro * m_proBar->getContentSize().width, m_proBar->getContentSize().height));
    m_monsterNode->removeAllChildrenWithCleanup(true);
    {
        string name = CCCommonUtils::getPropById(ActivityController::getInstance()->m_monster, "monster") + "_bust.png";
        auto sprite  = CCLoadSprite::createSprite(name.c_str(),true,CCLoadSpriteType_MONSTERLAYERBUST);
        if (sprite) {
            m_monsterNode->addChild(sprite);
            sprite->setAnchorPoint(ccp(0.5, 1));
            auto size = CCDirector::sharedDirector()->getWinSize();
            int dh = size.height - 852;
            if (CCCommonUtils::isIosAndroidPad())
            {
                dh = size.height - 2048;
                dh = dh / 2.4;
            }
            float totalH = 168 + dh + 30;
            float scale = totalH / sprite->getContentSize().height;
            sprite->setScale(scale);
        }
    }
}

void Training2View::refreshBlood()
{
    float rPro = 0;
    int totalHarm = 0;
    if (m_touchedIdxes.size() > 0) {
        int idx = m_touchedIdxes.at(0);
        int t = ActivityController::getInstance()->m_screen.at(idx - 1);
        int harm = ActivityController::getInstance()->m_atkHarm.at(t);
        totalHarm = harm * m_touchedIdxes.size();
        if (m_isUseBuf)
        {
            totalHarm = totalHarm * ActivityController::getInstance()->m_bigStrenMul;
        }
        if (totalHarm > ActivityController::getInstance()->m_blood)
            totalHarm = ActivityController::getInstance()->m_blood;
        rPro = totalHarm * 1.0f / ActivityController::getInstance()->m_maxBlood;
        rPro = rPro < 0? 0 : rPro;
        
    }
    float pro = (ActivityController::getInstance()->m_blood - totalHarm) * 1.0f / ActivityController::getInstance()->m_maxBlood;
    m_clipNode->setContentSize(CCSize(pro * m_proBar->getContentSize().width * m_proBar->getScaleX(), m_proBar->getContentSize().height * m_proBar->getScaleY()));
    
    m_clipNode2->setPositionX(m_clipNode->getContentSize().width);
    m_clipNode2->setContentSize(CCSize(rPro * m_proBar->getContentSize().width * m_proBar->getScaleX(), m_proBar->getContentSize().height * m_proBar->getScaleY()));
    if (!m_clipNode2->getChildByTag(123)) {
        auto spr = CCLoadSprite::createSprite("training_xutiao1.png");
        spr->setAnchorPoint(ccp(0, 0));
        spr->setColor({0, 255, 255});
        spr->setScaleX(m_proBar->getScaleX());
        spr->setScaleY(m_proBar->getScaleY());
        spr->setTag(123);
        spr->setBlendFunc({GL_ONE, GL_ONE});
        m_clipNode2->addChild(spr);
    }
    auto spr = dynamic_cast<CCSprite*>(m_clipNode2->getChildByTag(123));
    spr->setPosition(ccp(-m_clipNode->getContentSize().width, 0));
}

void Training2View::refreshInterface()
{
    m_actionTime = GlobalData::shared()->getTimeStamp();
    refreshCells();
    refreshBtnState();
    refreshMonster();
}

void Training2View::onEnter()
{
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    m_actionTime = GlobalData::shared()->getTimeStamp();
    CCNode::onEnter();
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgCellTouchBegan), MSG_CELL_TOUCH_BEGAN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgCellTouchMoved), MSG_CELL_TOUCH_MOVED, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgCellTouchEnded), MSG_CELL_TOUCH_ENDED, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgGetAtkData), MSG_GET_ATK_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgTrialBuySuc), MSG_TRIAL_BUY_SUC, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Training2View::onGetMsgCellBoom), MSG_CELL_BOOM, NULL);
    
    m_mainNode->setScale(1.2);
    m_mainNode->runAction(CCEaseSineIn::create(CCScaleTo::create(0.1, 1.0)));
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Training2View::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}
void Training2View::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CELL_TOUCH_BEGAN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CELL_TOUCH_MOVED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CELL_TOUCH_ENDED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_ATK_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRIAL_BUY_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CELL_BOOM);
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Training2View::onEnterFrame), this);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    setTouchEnabled(false);
    CCNode::onExit();
}

void Training2View::onGetMsgCellBoom(cocos2d::CCObject *obj)
{
    CCInteger* cint = dynamic_cast<CCInteger*>(obj);
    int pos = cint->getValue();
    if (pos <= 0) {
        return;
    }
    if (pos > m_addedSpr.size()) {
        m_addedSpr.clear();
        return;
    }
    if (m_addedSpr.size() <= 0) {
        return;
    }
    m_fixedWireNode->removeChild(m_addedSpr.at(pos - 1), true);
}

void Training2View::onGetMsgCellTouchBegan(cocos2d::CCObject *obj)
{
    if (m_touchedIdxes.size() > 0) {
        m_touchedIdxes.clear();
        removeAllFixedWire();
    }
    CCInteger* ccIdx = dynamic_cast<CCInteger*>(obj);
    int idx = ccIdx->getValue();
    m_isInCellWiring = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRAINING_CELL_START_LINING, CCInteger::create(ActivityController::getInstance()->m_screen.at(idx - 1)));
    m_touchedIdxes.push_back(idx);
    auto dic = CCDictionary::create();
    dic->setObject(CCString::create(CC_ITOA(idx)), "idx");
    dic->setObject(CCString::create(CC_ITOA(1)), "pos");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRAINING_CELL_ADDED, dic);
    refreshBlood();
    hideHandAction();
}

void Training2View::onGetMsgCellTouchMoved(cocos2d::CCObject *obj)
{
    if (m_touchedIdxes.size() <= 0 || !m_isInCellWiring) {
        return;
    }
    int idx = m_touchedIdxes[m_touchedIdxes.size() - 1];
    CCDictionary* dic = dynamic_cast<CCDictionary*>(obj);
    CCTouch* pTouch = dynamic_cast<CCTouch*>(dic->objectForKey("pTouch"));
    CCInteger* ccIdx = dynamic_cast<CCInteger*>(dic->objectForKey("m_idx"));
    int myIdx = ccIdx->getValue();
    if (!isHasTouched(myIdx)) {
        if (isLegalNeighbor(idx, myIdx)) {
            m_touchedIdxes.push_back(myIdx);
            addOneFixedWire();
            refreshBlood();
        }
    } else {
        removeFixedWireToIdx(myIdx);
        refreshBlood();
    }
    
}

bool Training2View::isHasTouched(int idx)
{
    auto it = find(m_touchedIdxes.begin(), m_touchedIdxes.end(), idx);
    if (it == m_touchedIdxes.end()) {
        return false;
    } else {
        return true;
    }
}

bool Training2View::isLegalNeighbor(int tarIdx, int myIdx)
{
    if (m_iToPMap.find(tarIdx) == m_iToPMap.end()) {
        return false;
    }
    if (m_iToPMap.find(myIdx) == m_iToPMap.end()) {
        return false;
    }
    if (ActivityController::getInstance()->m_screen.at(tarIdx - 1) != ActivityController::getInstance()->m_screen.at(myIdx - 1))
    {
        return false;
    }
    CCPoint tarPt = m_iToPMap[tarIdx];
    CCPoint myPt = m_iToPMap[myIdx];
    if (ccpDistance(tarPt, myPt) < 1.5 && ccpDistance(tarPt, myPt) > 0.5) {
        return true;
    } else {
        return false;
    }
}

void Training2View::onGetMsgCellTouchEnded(cocos2d::CCObject *obj)
{
    
}

void Training2View::onGetMsgGetAtkData(cocos2d::CCObject *obj)
{
    m_isInAtk = false;
    m_touchedIdxes.clear();
    m_fixedWireNode->removeAllChildrenWithCleanup(true);
    m_addedSpr.clear();
    m_isInCellWiring = false;
    m_isInTouching = false;
    m_isUseBuf = false;
    refreshInterface();
    auto cint = dynamic_cast<CCInteger*>(obj);
    if (cint) {
        int db = cint->getValue();
        if (db > 0) {
            string dblood = "-";
            dblood += CC_ITOA(db);
            auto label = CCLabelIFBMFont::create(dblood.c_str(), "pve_fnt_boss.fnt");
            label->setFontSize(35);
            label->setAnchorPoint(CCPoint(0.5, 0.5));
            label->setPosition(ccp(160, 0));
            m_parAniNode->addChild(label);
            label->runAction(CCSequence::create(CCDelayTime::create(1),
                                                CCFadeOut::create(0.5),
                                                nullptr));
        }
    }
    
    int state = ActivityController::getInstance()->getMyTrialState();
    if (state == 3 || state == 5) {
        PopupViewController::getInstance()->addPopupView(TrainingConfirmView::create(2));
    } else if (state == 4) {
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(Training1View::create());
        PopupViewController::getInstance()->addPopupView(TrainingRwdView::create());
    }
}

void Training2View::onGetMsgTrialBuySuc(cocos2d::CCObject *obj)
{
    m_touchedIdxes.clear();
    m_fixedWireNode->removeAllChildrenWithCleanup(true);
    m_addedSpr.clear();
    m_isInCellWiring = false;
    m_isInTouching = false;
    m_isUseBuf = false;
    refreshInterface();
}

/////-------TrainingCell

TrainingCell* TrainingCell::create(int idx)
{
    auto ret = new TrainingCell(idx);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TrainingCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    
    m_bgSpr = CCLoadSprite::createSprite("training_kuang.png");
    m_bgSpr->setAnchorPoint(ccp(0.5, 0.5));
    m_bgSpr->setScale(0.9);
    this->addChild(m_bgSpr, 1);
    
    m_iconNode = CCNode::create();
    this->addChild(m_iconNode, 2);
    
    m_parNode = CCNode::create();
    m_parNode->setScale(0.85);
    m_parNode->setPositionY(3);
    this->addChild(m_parNode, 3);
    
    m_touchNode = CCNode::create();
    m_touchNode->setContentSize(CCSize(90, 90));
    m_touchNode->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(m_touchNode, 4);
    if (ActivityController::getInstance()->m_freeAtk <= 0) {
        m_bCanClick = false;
    }
    refreshSpr();
    
    return true;
}

void TrainingCell::onEnter()
{
    setSwallowsTouches(false);
    CCNode::onEnter();
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgCellAdded), MSG_TRAINING_CELL_ADDED, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgCellRemoved), MSG_TRAINING_CELL_REMOVED, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgCellAllRemoved), MSG_TRAINING_CELL_ALL_REMOVED, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgCellStartLining), MSG_TRAINING_CELL_START_LINING, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgStartUseBuf), MSG_START_USE_BUF, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TrainingCell::onGetMsgStartAtk), MSG_START_ATK, NULL);
}

void TrainingCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRAINING_CELL_ADDED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRAINING_CELL_REMOVED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRAINING_CELL_ALL_REMOVED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRAINING_CELL_START_LINING);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_START_USE_BUF);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_START_ATK);
    
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool TrainingCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch) && m_bCanClick) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CELL_TOUCH_BEGAN, CCInteger::create(m_idx));
        this->setScale(0.8);
    } else {
        this->setScale(1);
    }
    return true;
}

void TrainingCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch) && m_bCanClick) {
        auto dic = CCDictionary::create();
        dic->setObject(pTouch, "pTouch");
        dic->setObject(CCInteger::create(m_idx), "m_idx");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CELL_TOUCH_MOVED, dic);
        this->setScale(0.8);
    } else {
        this->setScale(1);
    }
}

void TrainingCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    this->setScale(1);
    if (isTouchInside(m_touchNode, pTouch) && m_bCanClick) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CELL_TOUCH_ENDED, CCInteger::create(m_idx));
    }
}


void TrainingCell::onGetMsgCellAdded(cocos2d::CCObject *obj)
{
    auto dic = _dict(obj);
    int idx = dic->valueForKey("idx")->intValue();
    if (m_idx == idx) {
        m_addedPos = dic->valueForKey("pos")->intValue();
        removeParticle();
        if (m_isInBuf) {
            showSpePar();
        } else {
            showParByType();
        }
    }
}

void TrainingCell::onGetMsgCellRemoved(cocos2d::CCObject *obj)
{
    CCInteger* ccIdx = dynamic_cast<CCInteger*>(obj);
    int idx = ccIdx->getValue();
    if (m_idx == idx) {
        m_addedPos = 0;
        removeParticle();
    }
}

void TrainingCell::onGetMsgCellAllRemoved(cocos2d::CCObject *obj)
{
    m_addedPos = 0;
    removeParticle();
    m_bCanClick = true;
    setCellState();
}

void TrainingCell::onGetMsgCellStartLining(cocos2d::CCObject *obj)
{
    CCInteger* ccType = dynamic_cast<CCInteger*>(obj);
    int t = ccType->getValue();
    if (ActivityController::getInstance()->m_screen.at(m_idx - 1) != t) {
        m_bCanClick = false;
        setCellState();
    } else {
        m_bCanClick = true;
        setCellState();
    }
}

void TrainingCell::onGetMsgStartUseBuf(CCObject* obj)
{
    m_isInBuf = true;
    if (m_bCanClick && m_addedPos > 0) {
        removeParticle();
        showSpePar();
    }
}

void TrainingCell::onGetMsgStartAtk(cocos2d::CCObject *obj)
{
    removeParticle();
    m_bCanClick = false;
    if(m_addedPos == 0) {
        setCellState();
    } else {
        auto cs = dynamic_cast<CCString*>(obj);
        int totalCnt = cs->intValue();
        float dt = 0;
        if (totalCnt > 1) {
            dt = (0.5 - 0.1) / (totalCnt - 1);
        }
        dt = dt > 0.1? 0.1 : dt;
        this->runAction(CCSequence::create(CCDelayTime::create(dt * (m_addedPos - 1)),
                                           CCCallFunc::create(this, callfunc_selector(TrainingCell::showBoomPar)),
                                           CCDelayTime::create(0.1f),
                                           CCCallFunc::create(this, callfunc_selector(TrainingCell::setCellState)),
                                           nullptr
                                           ));
    }
}

void TrainingCell::showParByType()
{
    m_parNode->removeAllChildrenWithCleanup(true);
    if (ActivityController::getInstance()->m_screen.at(m_idx - 1) == 0) {
        auto par = ParticleController::createParticle("Smeltingtest_Attack_2");
        m_parNode->addChild(par);
    } else if (ActivityController::getInstance()->m_screen.at(m_idx - 1) == 1) {
        auto par1 = ParticleController::createParticle("Smeltingtest_lighting_1");
        auto par2 = ParticleController::createParticle("Smeltingtest_lighting_2");
        m_parNode->addChild(par1);
        m_parNode->addChild(par2);
    } else {
        auto par1 = ParticleController::createParticle("Smeltingtest_fire_1");
        auto par2 = ParticleController::createParticle("Smeltingtest_fire_2");
        m_parNode->addChild(par1);
        m_parNode->addChild(par2);
    }
}

void TrainingCell::showSpePar()
{
    removeParticle();
    auto par1 = ParticleController::createParticle("Smeltingtest_Bom_1");
    auto par2 = ParticleController::createParticle("Smeltingtest_Bom_2");
    m_parNode->addChild(par1);
    m_parNode->addChild(par2);
}

void TrainingCell::showBoomPar()
{
    removeParticle();
    for (int i = 1; i <= 5; ++i) {
        string name = "SmeltUI_end_";
        name += CC_ITOA(i);
        auto par = ParticleController::createParticle(name);
        m_parNode->addChild(par);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CELL_BOOM, CCInteger::create(m_addedPos));
}

void TrainingCell::removeParticle()
{
    m_parNode->removeAllChildrenWithCleanup(true);
}

void TrainingCell::setCellState()
{
    CCSprite* spr = dynamic_cast<CCSprite*>(m_iconNode->getChildByTag(1));
    if (m_bCanClick) {
        CCCommonUtils::setSpriteGray(m_bgSpr, false);
        if (spr) {
            CCCommonUtils::setSpriteGray(spr, false);
        }
    } else {
        CCCommonUtils::setSpriteGray(m_bgSpr, true);
        if (spr) {
            CCCommonUtils::setSpriteGray(spr, true);
        }
        removeParticle();
    }
}

void TrainingCell::refreshSpr()
{
    m_iconNode->removeAllChildrenWithCleanup(true);
    int t = ActivityController::getInstance()->m_screen.at(m_idx - 1);
    string sprName = "";
    if (t == 0)
    {
        sprName = "training_putong.png";
    } else if (t == 1)
    {
        sprName = "training_shandian.png";
    } else if (t == 2)
    {
        sprName = "training_huoqiu.png";
    }
    auto spr = CCLoadSprite::createSprite(sprName.c_str());
    spr->setTag(1);
    m_iconNode->addChild(spr);
    setCellState();
}

bool TrainingRwdView::init()
{
    if (!PopupBaseView::init()) return false;
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
    int extH = this->getExtendHeight();
    CCBLoadFile("DailyActiveBoxInfoView", this, this);
    m_mainNode->setPositionY(m_mainNode->getPositionY() + extH/2);
    
    m_congratuTTF->setString(_lang("107502"));
    m_titleTTF->setString("");
    
    int state = ActivityController::getInstance()->getMyTrialState();
    if (state != 3 && state != 4) {
        return false;
    }
    int cnt = ActivityController::getInstance()->getKilledMonsterCnt();
    if (state == 4)
    {
        m_rwdInfoLabel->setString(_lang("137538"));
    } else {
        m_rwdInfoLabel->setString(_lang_1("137537", CC_ITOA(cnt)));
    }
    
    int typeCnt = 0;
    if (ActivityController::getInstance()->m_monsterDiv.size() < 3)
    {
        return false;
    }
    if (cnt <= ActivityController::getInstance()->m_monsterDiv[0]) {
        typeCnt = 1;
    } else if (cnt <= ActivityController::getInstance()->m_monsterDiv[0] + ActivityController::getInstance()->m_monsterDiv[1]) {
        typeCnt = 2;
    } else {
        typeCnt = 3;
    }
    float dh = (340 - typeCnt * 110) * 1.0f / (typeCnt + 1);
    for (int i = 1; i <= typeCnt; ++i)
    {
        auto cell = TrainingRwdCell::create(i, cnt);
        cell->setPosition(ccp(0, (typeCnt - i) * 110 + (typeCnt - i + 1) * dh));
        m_listNode->addChild(cell);
    }
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("137539").c_str());
    addParticle();
    return true;
}

void TrainingRwdView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}
void TrainingRwdView::onExit()
{
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool TrainingRwdView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void TrainingRwdView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
void TrainingRwdView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10)
    {
        if (!isTouchInside(m_touchNode, pTouch)) {
            closeSelf();
        }
    }
}

bool TrainingRwdView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratureNode", CCNode*, this->m_congratureNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode1", CCNode*, this->m_particleNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode2", CCNode*, this->m_particleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratuTTF", CCLabelIF*, this->m_congratuTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdInfoLabel", CCLabelIF*, this->m_rwdInfoLabel);
    return false;
}
SEL_CCControlHandler TrainingRwdView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", TrainingRwdView::onOkClick);
    return nullptr;
}

void TrainingRwdView::onOkClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
    auto cmd = new TrialFinishCmd();
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void TrainingRwdView::addParticle()
{
    for (int i=1; i<4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("DailyActivity_%d",i)->getCString());
        //        particle->setPosition(CCPoint(40, 15));
        m_particleNode1->addChild(particle);
    }
    for (int i=4; i<7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("DailyActivity_%d",i)->getCString());
        //        particle->setPosition(CCPoint(40, 15));
        m_particleNode2->addChild(particle);
    }
    this->schedule(schedule_selector(TrainingRwdView::playGiftAnim), 0.2f, 5, 0.0f);
    SoundController::sharedSound()->playEffects(Music_Daily_Effect);
}

void TrainingRwdView::playGiftAnim(float dt)
{
    auto newBatch = ParticleController::createParticleBatch();
    for (int i=1; i<5; i++) {
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
        newBatch->addChild(praticle1);
    }
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    int maxX = size.width;
    int maxY = size.height;
    int x = rand() % maxX;
    int y = rand() % maxY;
    this->addChild(newBatch);
    newBatch->setPosition(x, y);
}

TrainingRwdCell* TrainingRwdCell::create(int type, int cnt)
{
    auto ret = new TrainingRwdCell(type, cnt);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool TrainingRwdCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    CCBLoadFile("TrainingRwdCell", this, this);
    int num = 0;
    int itemId = 0;
    if (m_type == 1)
    {
        if (m_totalCnt >= ActivityController::getInstance()->m_monsterDiv[0]) {
            num = ActivityController::getInstance()->m_monsterDiv[0];
        } else {
            num = m_totalCnt;
        }
        itemId = ITEM_TRIAL_RWD_NORMAL;
    } else if (m_type == 2){
        if (m_totalCnt >= ActivityController::getInstance()->m_monsterDiv[0] + ActivityController::getInstance()->m_monsterDiv[1]) {
            num = ActivityController::getInstance()->m_monsterDiv[1];
        } else {
            num = m_totalCnt - ActivityController::getInstance()->m_monsterDiv[0] ;
        }
        itemId = ITEM_TRIAL_RWD_SILVER;
    } else {
        num = m_totalCnt - ActivityController::getInstance()->m_monsterDiv[0] - ActivityController::getInstance()->m_monsterDiv[1];
        itemId = ITEM_TRIAL_RWD_GOLD;
    }
    string mul = "×";
    mul += CC_ITOA(num);
    m_numLabel->setString(mul);
    
    //test
//    itemId = ITEM_ALLIANCE_CITY_MOVE;
    
    auto& info = ToolController::getInstance()->getToolInfoById(itemId);
    m_infoLabel->setString(info.getName());
    ActivityController::getInstance()->createGoodsIcon(m_iconNode, CC_ITOA(itemId), 100);
    
    return true;
}

bool TrainingRwdCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, this->m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    return false;
}