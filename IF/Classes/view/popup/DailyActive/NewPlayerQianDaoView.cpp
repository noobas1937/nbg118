//
//  NewPlayerQianDaoView.cpp
//  IF
//
//  Created by 付彬 on 15/7/2.
//
//

#include "NewPlayerQianDaoView.h"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ChestUseView.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"

NewPlayerQianDaoView::~NewPlayerQianDaoView(){
}
NewPlayerQianDaoView* NewPlayerQianDaoView::create()
{
    auto ret = new NewPlayerQianDaoView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool NewPlayerQianDaoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    auto tmpCCB = CCBLoadFile("NewPlayerQianDaoView", this, this);
    setContentSize(tmpCCB->getContentSize());
    setTitleName(_lang("106035"));
    GameController::getInstance()->callXCApi("action=newPlayer_7rwd_click");
    
    m_del1Spr->setVisible(false);
    m_del2Spr->setVisible(false);
    m_del3Spr->setVisible(false);
    m_del4Spr->setVisible(false);
    m_del5Spr->setVisible(false);
    m_del6Spr->setVisible(false);
    m_del7Spr->setVisible(false);
    
    m_rd1Label->setString(_lang_1("106036", "1"));
    m_rd2Label->setString(_lang_1("106036", "2"));
    m_rd3Label->setString(_lang_1("106036", "3"));
    m_rd4Label->setString(_lang_1("106036", "4"));
    m_rd5Label->setString(_lang_1("106036", "5"));
    m_rd6Label->setString(_lang_1("106036", "6"));
    m_rd7Label->setString(_lang_1("106036", "7"));
    
//    changeBGHeight(m_buildBG);
    float extH = getExtendHeight();
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("111052").c_str());
    
    auto tbg = CCLoadSprite::loadResource("bg0.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    auto pic = CCLoadSprite::createSprite("bg0.png");
    int maxHeight = CCDirector::sharedDirector()->getWinSize().height  - pic->getContentSize().height+1100;
    int curHeight = -1000;
    while(curHeight < maxHeight)
    {
        auto pic2Left = CCLoadSprite::createSprite("bg0.png");
        pic2Left->setAnchorPoint(ccp(0, 0));
        pic2Left->setPosition(ccp(0, curHeight));
        tBatchNode->addChild(pic2Left);
        auto pic2Right = CCLoadSprite::createSprite("bg0.png");
        pic2Right->setScaleX(-1);
        pic2Right->setAnchorPoint(ccp(1, 0));
        pic2Right->setPosition(ccp(320, curHeight));
        if (CCCommonUtils::isIosAndroidPad())
        {
            pic2Left->setScaleX(2.4f);
            pic2Right->setPosition(ccp(768, curHeight));
            pic2Right->setScaleX(2.4);
        }
        tBatchNode->addChild(pic2Right);
        curHeight += pic2Left->getContentSize().height;
    }
    auto pic1Left = CCLoadSprite::createSprite("bg0.png");
    curHeight = CCDirector::sharedDirector()->getWinSize().height - 300 - pic1Left->getContentSize().height;
    pic1Left->setPosition(ccp(0, curHeight));
    pic1Left->setAnchorPoint(ccp(0, 0));
    tBatchNode->addChild(pic1Left);
    auto pic1Right = CCLoadSprite::createSprite("bg0.png");
    pic1Right->setScaleX(-1);
    pic1Right->setPosition(ccp(320, curHeight));
    if (CCCommonUtils::isIosAndroidPad())
    {
        pic1Left->setScaleX(2.4f);
        pic1Right->setPosition(ccp(768, curHeight));
        pic1Right->setScaleX(2.4);
    }
    pic1Right->setAnchorPoint(ccp(1, 0));
    tBatchNode->addChild(pic1Right);
    tBatchNode->setPosition(0, 0);
    m_bgNode->addChild(tBatchNode);
    if (CCCommonUtils::isIosAndroidPad())
    {
        tBatchNode->setScale(2.4f);
    }
    
    m_vipLabel->setVisible(!GlobalData::shared()->isXiaoMiPlatForm());
    m_vipLabel->setString(_lang("111072"));
    m_tipTxt1->setString(_lang("111073"));
    m_tipTxt2->setString(_lang("106037"));
    int tipTxt2H = m_tipTxt2->getContentSize().height*m_tipTxt2->getOriginScaleY();
    int sumHeight = 640+tipTxt2H;
    
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
    m_infoList->setPositionY(m_infoList->getPositionY()-extH);
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_mainNode->removeFromParent();
    m_mainNode->setPosition(ccp(0, sumHeight-580));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_mainNode->setPosition(ccp(0, sumHeight - 590));
    }
    m_scrollView->addChild(m_mainNode);
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,sumHeight));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - sumHeight));
    m_scrollView->setTouchPriority(Touch_Popup);
    
    m_waitInterface = NULL;
    m_guideKey = "";
    
    refreshData();
    
    return true;
}

void NewPlayerQianDaoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerQianDaoView::retData), MSG_NEW_PLAYER_RD, NULL);
}

void NewPlayerQianDaoView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_PLAYER_RD);
    CCNode::onExit();
}

CCNode* NewPlayerQianDaoView::getGuideNode(string _key)
{
    if (_key == "NEW_reward" && m_rwdBtn->isEnabled()) {
        m_guideKey = _key;
        return m_rwdBtn;
    }else if (_key=="AC_back") {
        return UIComponent::getInstance()->m_popupReturnBtn;
    }
    else {
        return NULL;
    }
}

void NewPlayerQianDaoView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    refreshData();
}

void NewPlayerQianDaoView::refreshData()
{
    int day = PortActController::getInstance()->m_newLoginDay;
    m_titleLabel->setString(_lang_1("106038", CC_ITOA(day)));
    m_leftSpr->setPositionX(-m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 - 10);
    m_rightSpr->setPositionX(m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 + 10);
    
    m_particleNode->removeAllChildren();
    int loginDay = PortActController::getInstance()->m_newLoginDay;
    
    if (PortActController::getInstance()->m_isGetNewRD==1) {
        m_rwdBtn->setEnabled(false);
    }else {
        m_rwdBtn->setEnabled(true);
        
        CCPoint pt = ccp(70, 70);
        if (loginDay==0) {
            m_particleNode->setPosition(m_rd1Node->getPosition()+pt);
        }else if (loginDay==1) {
            m_particleNode->setPosition(m_rd2Node->getPosition()+pt);
        }else if (loginDay==2) {
            m_particleNode->setPosition(m_rd3Node->getPosition()+pt);
        }else if (loginDay==3) {
            m_particleNode->setPosition(m_rd4Node->getPosition()+pt);
        }else if (loginDay==4) {
            m_particleNode->setPosition(m_rd5Node->getPosition()+pt);
        }else if (loginDay==5) {
            m_particleNode->setPosition(m_rd6Node->getPosition()+pt);
        }else if (loginDay==6) {
            m_particleNode->setPosition(m_rd7Node->getPosition()+pt);
        }
        for (int i=1; i<=3; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("LandingChest_%d",i)->getCString());
            if (i==1) {
                particle->setPosition(CCPoint(0, -15));
            }
            m_particleNode->addChild(particle);
        }
    }
    for (int i=1; i<=loginDay; i++) {
        showHaveGetRD(i);
    }
    
}

SEL_CCControlHandler NewPlayerQianDaoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", NewPlayerQianDaoView::onClickRwdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", NewPlayerQianDaoView::onClickTipBtn);
    return NULL;
}

bool NewPlayerQianDaoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftSpr", CCSprite*, this->m_leftSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightSpr", CCSprite*, this->m_rightSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd1Node", CCNode*, this->m_rd1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd2Node", CCNode*, this->m_rd2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd3Node", CCNode*, this->m_rd3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd4Node", CCNode*, this->m_rd4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd5Node", CCNode*, this->m_rd5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd6Node", CCNode*, this->m_rd6Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd7Node", CCNode*, this->m_rd7Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del1Spr", CCSprite*, this->m_del1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del2Spr", CCSprite*, this->m_del2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del3Spr", CCSprite*, this->m_del3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del4Spr", CCSprite*, this->m_del4Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del5Spr", CCSprite*, this->m_del5Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del6Spr", CCSprite*, this->m_del6Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del7Spr", CCSprite*, this->m_del7Spr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd1Label", CCLabelIF*, this->m_rd1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd2Label", CCLabelIF*, this->m_rd2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd3Label", CCLabelIF*, this->m_rd3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd4Label", CCLabelIF*, this->m_rd4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd5Label", CCLabelIF*, this->m_rd5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd6Label", CCLabelIF*, this->m_rd6Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd7Label", CCLabelIF*, this->m_rd7Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIF*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    return false;
}

void NewPlayerQianDaoView::onClickRwdBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_rwdBtn);
    if (m_guideKey!="") {
        PortActController::getInstance()->startGetNewPlayerRD(GUIDE_NEW_RD_ED);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("NEW_reward"));
    }else {
        PortActController::getInstance()->startGetNewPlayerRD();
    }
}

void NewPlayerQianDaoView::onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    string tips = _lang("106039") + "\n" + _lang("106040") + "\n" + _lang("106041") + "\n" + _lang("106042") + "\n" + _lang("106043");
    if (GlobalData::shared()->isXiaoMiPlatForm()) {
        tips = _lang("106039") + "\n" + _lang("106040") + "\n" + _lang("106041") + "\n" + _lang("106042");
    }
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
}

bool NewPlayerQianDaoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_infoList, pTouch)){
    }
    m_startY = pTouch->getLocation().y;
    return true;
}

void NewPlayerQianDaoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY - m_startY) >= 20) {
        return;
    }
    if (isTouchInside(m_rd1Node, pTouch)) {
        clickShowRD(1);
    }else if (isTouchInside(m_rd2Node, pTouch)) {
        clickShowRD(2);
    }else if (isTouchInside(m_rd3Node, pTouch)) {
        clickShowRD(3);
    }else if (isTouchInside(m_rd4Node, pTouch)) {
        clickShowRD(4);
    }else if (isTouchInside(m_rd5Node, pTouch)) {
        clickShowRD(5);
    }else if (isTouchInside(m_rd6Node, pTouch)) {
        clickShowRD(6);
    }else if (isTouchInside(m_rd7Node, pTouch)) {
        clickShowRD(7);
    }
}

void NewPlayerQianDaoView::showHaveGetRD(int day)
{
    CCNode* curNode = NULL;
    CCSprite* curSpr = NULL;
    if (day == 1) {
        curNode = m_rd1Node;
        curSpr = m_del1Spr;
    }else if (day == 2) {
        curNode = m_rd2Node;
        curSpr = m_del2Spr;
    }else if (day == 3) {
        curNode = m_rd3Node;
        curSpr = m_del3Spr;
    }else if (day == 4) {
        curNode = m_rd4Node;
        curSpr = m_del4Spr;
    }else if (day == 5) {
        curNode = m_rd5Node;
        curSpr = m_del5Spr;
    }else if (day == 6) {
        curNode = m_rd6Node;
        curSpr = m_del6Spr;
    }else if (day == 7) {
        curNode = m_rd7Node;
        curSpr = m_del7Spr;
    }else {
        return;
    }
    auto & tmpArr = curNode->getChildren();
    if (true) {
        for (int i=0; i<tmpArr.size(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(tmpArr.at(i));
            if (spr) {
                CCCommonUtils::setSpriteGray(spr, true);
            }
        }
    }
    CCCommonUtils::setSpriteGray(curSpr, false);
    curSpr->setVisible(true);
}

void NewPlayerQianDaoView::clickShowRD(int day)
{
    if(PortActController::getInstance()->m_newPalyerRDMap.find(day) != PortActController::getInstance()->m_newPalyerRDMap.end() )
    {
        auto dict = CCDictionary::create();
        auto itemEffectObj = CCDictionary::create();
        CCArray* tmpArr = PortActController::getInstance()->m_newPalyerRDMap[day];
        
        auto newArr = CCArray::createWithArray(tmpArr);
        
        itemEffectObj->setObject(newArr, "reward");
        auto tmparray = CCArray::create();
        dict->setObject(itemEffectObj, "itemEffectObj");
        dict->setObject(CCString::create(CC_ITOA(day)), "day");
        PopupViewController::getInstance()->addPopupView(ChestRDView::create(dict),false,false);
    }
}


//////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////
NewPlayerQianDaoPopView::~NewPlayerQianDaoPopView(){
}
NewPlayerQianDaoPopView* NewPlayerQianDaoPopView::create()
{
    auto ret = new NewPlayerQianDaoPopView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool NewPlayerQianDaoPopView::init()
{
    if (!CCNode::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    auto tmpCCB = CCBLoadFile("NewPlayerQianDaoPanel", this, this);
    setContentSize(tmpCCB->getContentSize());
//    setTitleName(_lang("106035"));
    GameController::getInstance()->callXCApi("action=newPlayer_7rwd_pop");
    
    m_del1Spr->setVisible(false);
    m_del2Spr->setVisible(false);
    m_del3Spr->setVisible(false);
    m_del4Spr->setVisible(false);
    m_del5Spr->setVisible(false);
    m_del6Spr->setVisible(false);
    m_del7Spr->setVisible(false);
    
    m_rd1Label->setString(_lang_1("106036", "1"));
    m_rd2Label->setString(_lang_1("106036", "2"));
    m_rd3Label->setString(_lang_1("106036", "3"));
    m_rd4Label->setString(_lang_1("106036", "4"));
    m_rd5Label->setString(_lang_1("106036", "5"));
    m_rd6Label->setString(_lang_1("106036", "6"));
    m_rd7Label->setString(_lang_1("106036", "7"));
    
    //    changeBGHeight(m_buildBG);
//    float extH = getExtendHeight();
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("111052").c_str());
    
//    auto tbg = CCLoadSprite::loadResource("bg0.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    auto pic = CCLoadSprite::createSprite("bg0.png");
//    int maxHeight = CCDirector::sharedDirector()->getWinSize().height  - pic->getContentSize().height+1100;
//    int curHeight = -1000;
//    while(curHeight < maxHeight)
//    {
//        auto pic2Left = CCLoadSprite::createSprite("bg0.png");
//        pic2Left->setAnchorPoint(ccp(0, 0));
//        pic2Left->setPosition(ccp(0, curHeight));
//        tBatchNode->addChild(pic2Left);
//        auto pic2Right = CCLoadSprite::createSprite("bg0.png");
//        pic2Right->setScaleX(-1);
//        pic2Right->setAnchorPoint(ccp(1, 0));
//        pic2Right->setPosition(ccp(320, curHeight));
//        tBatchNode->addChild(pic2Right);
//        curHeight += pic2Left->getContentSize().height;
//    }
//    auto pic1Left = CCLoadSprite::createSprite("bg0.png");
//    curHeight = CCDirector::sharedDirector()->getWinSize().height - 300 - pic1Left->getContentSize().height;
//    pic1Left->setPosition(ccp(0, curHeight));
//    pic1Left->setAnchorPoint(ccp(0, 0));
//    tBatchNode->addChild(pic1Left);
//    auto pic1Right = CCLoadSprite::createSprite("bg0.png");
//    pic1Right->setScaleX(-1);
//    pic1Right->setPosition(ccp(320, curHeight));
//    pic1Right->setAnchorPoint(ccp(1, 0));
//    tBatchNode->addChild(pic1Right);
//    tBatchNode->setPosition(0, 0);
//    m_bgNode->addChild(tBatchNode);
    
//    m_vipLabel->setString(_lang("111072"));
//    m_tipTxt1->setString(_lang("111073"));
//    m_tipTxt2->setString(_lang("106037"));
//    int tipTxt2H = m_tipTxt2->getContentSize().height*m_tipTxt2->getOriginScaleY();
//    int sumHeight = 920+tipTxt2H;
    
//    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height));
//    m_infoList->setPositionY(m_infoList->getPositionY());
//    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
//    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
//    m_infoList->addChild(m_scrollView);
//    m_mainNode->removeFromParent();
//    m_mainNode->setPosition(ccp(0, sumHeight-580));
//    m_scrollView->addChild(m_mainNode);
//    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,sumHeight));
//    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - sumHeight));
//    
    m_waitInterface = NULL;
    
    refreshData();
    
    return true;
}

void NewPlayerQianDaoPopView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerQianDaoPopView::retData), MSG_NEW_PLAYER_RD, NULL);
}

void NewPlayerQianDaoPopView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_PLAYER_RD);
    CCNode::onExit();
}

void NewPlayerQianDaoPopView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    refreshData();
}

void NewPlayerQianDaoPopView::refreshData()
{
    int day = PortActController::getInstance()->m_newLoginDay;
    m_titleLabel->setString(_lang_1("106038", CC_ITOA(day)));
    m_leftSpr->setPositionX(-m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 - 10);
    m_rightSpr->setPositionX(m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 + 10);
    
    m_particleNode->removeAllChildren();
    int loginDay = PortActController::getInstance()->m_newLoginDay;
    
    if (PortActController::getInstance()->m_isGetNewRD==1) {
        m_rwdBtn->setEnabled(false);
    }else {
        m_rwdBtn->setEnabled(true);
        
        CCPoint pt = ccp(70, 70);
        if (loginDay==0) {
            m_particleNode->setPosition(m_rd1Node->getPosition()+pt);
        }else if (loginDay==1) {
            m_particleNode->setPosition(m_rd2Node->getPosition()+pt);
        }else if (loginDay==2) {
            m_particleNode->setPosition(m_rd3Node->getPosition()+pt);
        }else if (loginDay==3) {
            m_particleNode->setPosition(m_rd4Node->getPosition()+pt);
        }else if (loginDay==4) {
            m_particleNode->setPosition(m_rd5Node->getPosition()+pt);
        }else if (loginDay==5) {
            m_particleNode->setPosition(m_rd6Node->getPosition()+pt);
        }else if (loginDay==6) {
            m_particleNode->setPosition(m_rd7Node->getPosition()+pt);
        }
        for (int i=1; i<=3; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("LandingChest_%d",i)->getCString());
            if (i==1) {
                particle->setPosition(CCPoint(0, -15));
            }
            m_particleNode->addChild(particle);
        }
    }
    for (int i=1; i<=loginDay; i++) {
        showHaveGetRD(i);
    }
    
}

SEL_CCControlHandler NewPlayerQianDaoPopView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", NewPlayerQianDaoPopView::onClickRwdBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", NewPlayerQianDaoPopView::onClickTipBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", NewPlayerQianDaoPopView::onCloseBtnClick);
    return NULL;
}
bool NewPlayerQianDaoPopView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftSpr", CCSprite*, this->m_leftSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightSpr", CCSprite*, this->m_rightSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd1Node", CCNode*, this->m_rd1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd2Node", CCNode*, this->m_rd2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd3Node", CCNode*, this->m_rd3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd4Node", CCNode*, this->m_rd4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd5Node", CCNode*, this->m_rd5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd6Node", CCNode*, this->m_rd6Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd7Node", CCNode*, this->m_rd7Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del1Spr", CCSprite*, this->m_del1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del2Spr", CCSprite*, this->m_del2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del3Spr", CCSprite*, this->m_del3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del4Spr", CCSprite*, this->m_del4Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del5Spr", CCSprite*, this->m_del5Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del6Spr", CCSprite*, this->m_del6Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_del7Spr", CCSprite*, this->m_del7Spr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd1Label", CCLabelIF*, this->m_rd1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd2Label", CCLabelIF*, this->m_rd2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd3Label", CCLabelIF*, this->m_rd3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd4Label", CCLabelIF*, this->m_rd4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd5Label", CCLabelIF*, this->m_rd5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd6Label", CCLabelIF*, this->m_rd6Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rd7Label", CCLabelIF*, this->m_rd7Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIF*, this->m_vipLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}

void NewPlayerQianDaoPopView::onClickRwdBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_rwdBtn);
    PortActController::getInstance()->startGetNewPlayerRD();
}

//void NewPlayerQianDaoPopView::onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
//{
//    string tips = _lang("106039") + "\n" + _lang("106040") + "\n" + _lang("106041") + "\n" + _lang("106042") + "\n" + _lang("106043");
//    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
//}
void NewPlayerQianDaoPopView::onCloseBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    PopupViewController::getInstance()->removeAllPopupView();
}
bool NewPlayerQianDaoPopView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_infoList, pTouch)){
//    }
//    m_startY = pTouch->getLocation().y;
    return true;
}

void NewPlayerQianDaoPopView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    int endY = pTouch->getLocation().y;
//    if (abs(endY - m_startY) >= 20) {
//        return;
//    }
    if (isTouchInside(m_rd1Node, pTouch)) {
        clickShowRD(1);
    }else if (isTouchInside(m_rd2Node, pTouch)) {
        clickShowRD(2);
    }else if (isTouchInside(m_rd3Node, pTouch)) {
        clickShowRD(3);
    }else if (isTouchInside(m_rd4Node, pTouch)) {
        clickShowRD(4);
    }else if (isTouchInside(m_rd5Node, pTouch)) {
        clickShowRD(5);
    }else if (isTouchInside(m_rd6Node, pTouch)) {
        clickShowRD(6);
    }else if (isTouchInside(m_rd7Node, pTouch)) {
        clickShowRD(7);
    }else if(!isTouchInside(m_touchNode, pTouch)){
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void NewPlayerQianDaoPopView::showHaveGetRD(int day)
{
    CCNode* curNode = NULL;
    CCSprite* curSpr = NULL;
    if (day == 1) {
        curNode = m_rd1Node;
        curSpr = m_del1Spr;
    }else if (day == 2) {
        curNode = m_rd2Node;
        curSpr = m_del2Spr;
    }else if (day == 3) {
        curNode = m_rd3Node;
        curSpr = m_del3Spr;
    }else if (day == 4) {
        curNode = m_rd4Node;
        curSpr = m_del4Spr;
    }else if (day == 5) {
        curNode = m_rd5Node;
        curSpr = m_del5Spr;
    }else if (day == 6) {
        curNode = m_rd6Node;
        curSpr = m_del6Spr;
    }else if (day == 7) {
        curNode = m_rd7Node;
        curSpr = m_del7Spr;
    }else {
        return;
    }
    
    auto & tmpArr = curNode->getChildren();
    if (true) {
        for (int i=0; i<tmpArr.size(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(tmpArr.at(i));
            if (spr) {
                CCCommonUtils::setSpriteGray(spr, true);
            }
        }
    }
    CCCommonUtils::setSpriteGray(curSpr, false);
    curSpr->setVisible(true);
}

void NewPlayerQianDaoPopView::clickShowRD(int day)
{
    if(PortActController::getInstance()->m_newPalyerRDMap.find(day) != PortActController::getInstance()->m_newPalyerRDMap.end() )
    {
        auto dict = CCDictionary::create();
        auto itemEffectObj = CCDictionary::create();
        CCArray* tmpArr = PortActController::getInstance()->m_newPalyerRDMap[day];
        
        auto newArr = CCArray::createWithArray(tmpArr);
        
        itemEffectObj->setObject(newArr, "reward");
        auto tmparray = CCArray::create();
        dict->setObject(itemEffectObj, "itemEffectObj");
        dict->setObject(CCString::create(CC_ITOA(day)), "day");
        PopupViewController::getInstance()->addPopupView(ChestRDView::create(dict),false,false);
    }
}