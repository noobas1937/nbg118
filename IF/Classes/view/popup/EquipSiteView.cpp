//
//  EquipSiteView.cpp
//  IF
//
//  Created by fubin on 14-12-29.
//
//

#include "EquipSiteView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "EquipmentCreateView.h"
#include "EquipmentListView.h"
#include "QueueController.h"
#include "SoundController.h"

#define BOOK_NUM_MAX 5

EquipSiteView* EquipSiteView::create(){
    EquipSiteView* ret = new EquipSiteView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipSiteView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(100, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(100, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
    });
    
    setIsHDPanel(true);
    auto bg = CCBLoadFile("EquipSiteView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        setContentSize(bg->getContentSize());
        changeBGHeight(m_buildBG);
        int extH = getExtendHeight();
        
        m_infoNode->setContentSize(CCSize(m_infoNode->getContentSize().width, m_infoNode->getContentSize().height+extH));
        m_infoNode->setPositionY(m_infoNode->getPositionY()-extH);
        m_scrollView = CCScrollView::create(m_infoNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoNode->addChild(m_scrollView);
        //    m_mainNode->removeFromParent();
        //    m_mainNode->setPosition(ccp(320, 570));
        //    m_scrollView->addChild(m_mainNode);
        //    m_scrollView->setContentSize(CCSize(m_infoNode->getContentSize().width,950));
        //    m_scrollView->setContentOffset(ccp(0, m_infoNode->getContentSize().height - 950));
        
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
    }
    m_btn->setSwallowsTouches(false);
    m_equip0Label->setString("");//武器_lang("119014")
    m_equip1Label->setString("");//头盔_lang("119015")
    m_equip2Label->setString("");//铠甲_lang("119016")
    m_equip3Label->setString("");//裤子_lang("119017")
    m_equip4Label->setString("");//鞋_lang("119018")
    m_equip5Label->setString("");//戒指_lang("119019")
    
    m_msgLabel->setString(_lang("119010"));
    CCCommonUtils::setButtonTitle(m_btn, _lang("120153").c_str());
    
    m_site0Btn->setTouchPriority(Touch_Popup_Item);
    m_site1Btn->setTouchPriority(Touch_Popup_Item);
    m_site2Btn->setTouchPriority(Touch_Popup_Item);
    m_site3Btn->setTouchPriority(Touch_Popup_Item);
    m_site4Btn->setTouchPriority(Touch_Popup_Item);
    m_site5Btn->setTouchPriority(Touch_Popup_Item);
    m_btn->setTouchPriority(Touch_Popup_Item);
    
    m_bookDefineOrder[0] = 1;
    m_bookDefineOrder[1] = 3;
    m_bookDefineOrder[2] = 5;
    m_bookDefineOrder[3] = 4;
    m_bookDefineOrder[4] = 2;
    m_bookDefineOrder[5] = 0;
    
    m_bookDefineScale[0] = 0.8;
    m_bookDefineScale[1] = 1.0;
    m_bookDefineScale[2] = 1.2;
    m_bookDefineScale[3] = 1.0;
    m_bookDefineScale[4] = 0.8;
    m_bookDefineScale[5] = 0;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_bookDefinePos[0] = ccp(-380, 250);
        m_bookDefinePos[1] = ccp(-200, 50);
        m_bookDefinePos[2] = ccp(-20, -130);
        m_bookDefinePos[3] = ccp(160, -310);
        m_bookDefinePos[4] = ccp(340, -510);
        m_bookDefinePos[5] = ccp(340, -510);
    }
    else {
        m_bookDefinePos[0] = ccp(-200, -100);
        m_bookDefinePos[1] = ccp(-100, -170);
        m_bookDefinePos[2] = ccp(0, -300);
        m_bookDefinePos[3] = ccp(100, -430);
        m_bookDefinePos[4] = ccp(200, -500);
        m_bookDefinePos[5] = ccp(200, -500);
    }
    
    m_bookDefineColor[0] = 131;
    m_bookDefineColor[1] = 183;
    m_bookDefineColor[2] = 255;
    m_bookDefineColor[3] = 183;
    m_bookDefineColor[4] = 131;
    m_bookDefineColor[5] = 131;
    
    m_site0Node->setPosition(m_bookDefinePos[2]);
    m_site0Node->setZOrder(m_bookDefineOrder[2]);
    m_site0Node->setTag(2);
    m_site0Btn->setEnabled(false);
    m_book0Spr->setColor(ccc3(m_bookDefineColor[2],m_bookDefineColor[2],m_bookDefineColor[2]));
    
    m_site1Node->setPosition(m_bookDefinePos[3]);
    m_site1Node->setZOrder(m_bookDefineOrder[3]);
    m_site1Node->setTag(3);
    m_site1Btn->setEnabled(false);
    m_book1Spr->setColor(ccc3(m_bookDefineColor[3],m_bookDefineColor[3],m_bookDefineColor[3]));
    
    m_site2Node->setPosition(m_bookDefinePos[4]);
    m_site2Node->setZOrder(m_bookDefineOrder[4]);
    m_site2Node->setTag(4);
    m_site2Btn->setEnabled(false);
    m_book2Spr->setColor(ccc3(m_bookDefineColor[4],m_bookDefineColor[4],m_bookDefineColor[4]));
    
    m_site3Node->setPosition(m_bookDefinePos[5]);
    m_site3Node->setZOrder(m_bookDefineOrder[5]);
    m_site3Node->setTag(5);
    m_site3Btn->setEnabled(false);
    m_book3Spr->setColor(ccc3(m_bookDefineColor[5],m_bookDefineColor[5],m_bookDefineColor[5]));
    m_site3Node->setVisible(false);
    
    m_site4Node->setPosition(m_bookDefinePos[0]);
    m_site4Node->setZOrder(m_bookDefineOrder[0]);
    m_site4Node->setTag(0);
    m_site4Btn->setEnabled(false);
    m_book4Spr->setColor(ccc3(m_bookDefineColor[0],m_bookDefineColor[0],m_bookDefineColor[0]));
    
    m_site5Node->setPosition(m_bookDefinePos[1]);
    m_site5Node->setZOrder(m_bookDefineOrder[1]);
    m_site5Node->setTag(1);
    m_site5Btn->setEnabled(false);
    m_book5Spr->setColor(ccc3(m_bookDefineColor[1],m_bookDefineColor[1],m_bookDefineColor[1]));
    
    m_bookParent = m_site0Node->getParent();
    
//    m_totalLength = m_bookDefinePos[0].getDistance(m_bookDefinePos[4]);
    m_totalLength = m_bookDefinePos[1].getDistance(m_bookDefinePos[2]);
    
    m_aniNode->setZOrder(6);
    
    m_weapType = -1;
    m_mateType = -1;
    m_curIdx = 4;
    m_willIdx = 4;
    
    auto pos = m_progrossBar->getPosition();
    m_barClipNode = CCClipNode::create(m_progrossBar->getContentSize().width, m_progrossBar->getContentSize().height);
    m_progrossBar->getParent()->addChild(m_barClipNode);
    m_barClipNode->setAnchorPoint(ccp(0, 0));
    m_progrossBar->retain();
    m_progrossBar->removeFromParent();
    m_barClipNode->addChild(m_progrossBar);
    m_progrossBar->setPosition(0, 0);
    CC_SAFE_RELEASE_NULL(m_progrossBar);
    m_barClipNode->setPosition(pos);
    
    m_isInitEnd = false;
    
    return true;
}

void EquipSiteView::onEnter(){
    CCNode::onEnter();
    m_isInitEnd = false;
    UIComponent::getInstance()->showEquipOrBagBtn(1);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(EquipSiteView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
    setTitleName(_lang("119008"));
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    
    m_site0Node->setScale(m_bookDefineScale[m_site0Node->getTag()]);
    m_site1Node->setScale(m_bookDefineScale[m_site1Node->getTag()]);
    m_site2Node->setScale(m_bookDefineScale[m_site2Node->getTag()]);
    m_site3Node->setScale(m_bookDefineScale[m_site3Node->getTag()]);
    m_site4Node->setScale(m_bookDefineScale[m_site4Node->getTag()]);
    m_site5Node->setScale(m_bookDefineScale[m_site5Node->getTag()]);
    
    m_leftBg->setOpacity(255);
    m_rightBg->setOpacity(255);
    m_buildBG->setOpacity(255);
    
    m_onAction = 0;
    
//    m_site5Node->setScale(1.0);
    
    onRefreshStat();
    
    startToMoveInit();
}

void EquipSiteView::onExit(){
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(EquipSiteView::onEnterFrame), this);
    CCNode::onExit();
}

CCNode* EquipSiteView::getGuideNode(string _key)
{
    if (m_isInitEnd && m_willIdx != -1) {
        return m_guideNode;
    }
    return NULL;
}

void EquipSiteView::startToMoveInit()
{
    m_flag0Spr->setVisible(false);
    m_flag1Spr->setVisible(false);
    m_flag2Spr->setVisible(false);
    m_flag3Spr->setVisible(false);
    m_flag4Spr->setVisible(false);
    m_flag5Spr->setVisible(false);
    
    m_willIdx = 4;
    if (m_willIdx == -1 || m_willIdx == m_curIdx) {
        m_isInitEnd = true;
        return;
    }
    return;
    
    vector<int> idVec = EquipmentController::getInstance()->CheckEquipSiteMake();
    for (int i=0; i<idVec.size(); i++) {
//        if (idVec[i] == 0) {
//            m_flag0Spr->setVisible(true);
//        }else if (idVec[i] == 1) {
//            m_flag1Spr->setVisible(true);
//        }else if (idVec[i] == 2) {
//            m_flag2Spr->setVisible(true);
//        }else if (idVec[i] == 3) {
//            m_flag3Spr->setVisible(true);
//        }else if (idVec[i] == 4) {
//            m_flag4Spr->setVisible(true);
//        }else if (idVec[i] == 5) {
//            m_flag5Spr->setVisible(true);
//        }
    }
    
    m_willIdx = -1;
    if (idVec.size()>0) {
        m_willIdx = idVec[0];
    }
    if (m_willIdx == -1 || m_willIdx == m_curIdx) {
        m_isInitEnd = true;
        return;
    }
    
    m_moveSum = abs(m_willIdx - m_curIdx);
    if (m_moveSum > 3) {
        m_moveSum = 6-m_moveSum;
    }
    
    int tmpWill1 = m_curIdx+m_moveSum;
    tmpWill1 = tmpWill1>5?(tmpWill1-6):tmpWill1;
    if (tmpWill1 == m_willIdx) {
        m_movedPercent = -1;
    }
    else {
        int tmpWill2 = m_curIdx-m_moveSum;
        tmpWill2 = tmpWill2<0?(tmpWill2+6):tmpWill2;
        if(tmpWill2 == m_willIdx) {
            m_movedPercent = 1;
        }else {
            return;
        }
    }
    
    if(m_willIdx != m_curIdx) {
        this->schedule(schedule_selector(EquipSiteView::moveToIdx), 0.5);
        moveToIdx(0);
    }
}

void EquipSiteView::moveToIdx(float dt)
{
//    m_curIdx += m_movedPercent;
//    m_curIdx = m_curIdx>5?0:m_curIdx;
//    m_curIdx = m_curIdx<0?5:m_curIdx;
    move(1);
    if (m_curIdx == m_willIdx ) {
        this->unschedule(schedule_selector(EquipSiteView::moveToIdx));
        this->scheduleOnce(schedule_selector(EquipSiteView::initToEnd), 0.5);
    }
}

void EquipSiteView::initToEnd(float dt)
{
    m_isInitEnd = true;
}

void EquipSiteView::onRefreshStat()
{
    m_qid = QID_MAX;
    m_itemId = 0;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid!=QID_MAX) {
        m_qid = tqid;
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        m_barNode->setVisible(true);
        m_msgLabel->setVisible(false);
        int itemId = QueueController::getInstance()->getItemId(qInfo.key);
        m_itemId = itemId;
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        string strName = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
        m_nameLabel->setString(strName);
        
        onEnterFrame(0);
    }
    else {
        m_barNode->setVisible(false);
        m_msgLabel->setVisible(true);
    }
}

void EquipSiteView::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if(qInfo.startTime==0){
        qInfo.startTime = tmpCurTime;
    }
    int m_sumTime = qInfo.finishTime - qInfo.startTime;
    int m_curTime = qInfo.finishTime - tmpCurTime;
    m_curTime = MAX(0, m_curTime);
    m_sumTime = MAX(1, m_sumTime);
    if (qInfo.finishTime<=0) {
        m_curTime = 0;
        m_sumTime = 1;
    }
    
    float pro = m_curTime*1.0/m_sumTime;
    pro = pro>1?1:pro;
//    m_progrossBar->setScaleX(1-pro);
    m_timeText->setString(CC_SECTOA(m_curTime));
    
    int totalW = 342;
    float w = totalW - pro*totalW;
    m_barClipNode->setContentSize(CCSize(w, m_progrossBar->getContentSize().height));
    
    if (m_curTime<=0) {
        m_qid = QID_MAX;
        m_barNode->setVisible(false);
        m_msgLabel->setVisible(true);
    }
}

SEL_CCControlHandler EquipSiteView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite0Btn", EquipSiteView::onClickSite0Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite1Btn", EquipSiteView::onClickSite1Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite2Btn", EquipSiteView::onClickSite2Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite3Btn", EquipSiteView::onClickSite3Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite4Btn", EquipSiteView::onClickSite4Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite5Btn", EquipSiteView::onClickSite5Btn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClearCDClick", EquipSiteView::onClearCDClick);
    return NULL;
}

bool EquipSiteView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, this->m_barNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progrossBar", CCScale9Sprite*, this->m_progrossBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode", CCNode*, this->m_guideNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site0Btn", CCControlButton*, this->m_site0Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site1Btn", CCControlButton*, this->m_site1Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site2Btn", CCControlButton*, this->m_site2Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site3Btn", CCControlButton*, this->m_site3Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site4Btn", CCControlButton*, this->m_site4Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site5Btn", CCControlButton*, this->m_site5Btn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book0Spr", CCSprite*, this->m_book0Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book1Spr", CCSprite*, this->m_book1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book2Spr", CCSprite*, this->m_book2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book3Spr", CCSprite*, this->m_book3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book4Spr", CCSprite*, this->m_book4Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_book5Spr", CCSprite*, this->m_book5Spr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip0Label", CCLabelIF*, this->m_equip0Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip1Label", CCLabelIF*, this->m_equip1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip2Label", CCLabelIF*, this->m_equip2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip3Label", CCLabelIF*, this->m_equip3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip4Label", CCLabelIF*, this->m_equip4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equip5Label", CCLabelIF*, this->m_equip5Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag0Spr", CCSprite*, this->m_flag0Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag1Spr", CCSprite*, this->m_flag1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag2Spr", CCSprite*, this->m_flag2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag3Spr", CCSprite*, this->m_flag3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag4Spr", CCSprite*, this->m_flag4Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flag5Spr", CCSprite*, this->m_flag5Spr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site0Node", CCNode*, this->m_site0Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site1Node", CCNode*, this->m_site1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site2Node", CCNode*, this->m_site2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site3Node", CCNode*, this->m_site3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site4Node", CCNode*, this->m_site4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_site5Node", CCNode*, this->m_site5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniNode", CCNode*, this->m_aniNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg", CCSprite*, this->m_leftBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg", CCSprite*, this->m_rightBg);
    
    return false;
}

void EquipSiteView::onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_itemId>0) {
        PopupViewController::getInstance()->addPopupInView(EquipmentCreateView::create(m_itemId));
    }
}

void EquipSiteView::onClickSite0Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(0);
}
void EquipSiteView::onClickSite1Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(1);
}
void EquipSiteView::onClickSite2Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(2);
}
void EquipSiteView::onClickSite3Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(3);
}
void EquipSiteView::onClickSite4Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(4);
}
void EquipSiteView::onClickSite5Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onPlayClick(5);
}

bool EquipSiteView::move(int distance) {
    if (m_onAction==0) {
        if (m_movedPercent > 0.2) {
            for (int i = 5; i >=0 ; --i) {
                CCNode* bookNode;
                switch (i) {
                    case 0: bookNode = m_site0Node; break;
                    case 1: bookNode = m_site1Node; break;
                    case 2: bookNode = m_site2Node; break;
                    case 3: bookNode = m_site3Node; break;
                    case 4: bookNode = m_site4Node; break;
                    case 5: bookNode = m_site5Node; break;
                        
                    default:
                        break;
                }
                int oldposid = bookNode->getTag();
                int posid = oldposid+1;
                if (oldposid == 5) posid = 0;
                m_bookNodesMove2Posid[i] = posid; //标记目标位置
                if (posid == 0) {
                    bookNode->setPosition(m_bookDefinePos[0]);
                    bookNode->setVisible(true);
                }
                CCSpawn* spwan = CCSpawn::create(CCSequence::create(CCMoveTo::create(0.3, m_bookDefinePos[posid]), CCCallFuncN::create(this, callfuncN_selector(EquipSiteView::moveend)), NULL),
                                                 CCScaleTo::create(0.3, m_bookDefineScale[posid]),
                                                 NULL);
                m_onAction++;
                bookNode->runAction(spwan);
                
                CCSprite* spr = nodeid2Sprite(i);
                spr->runAction(CCTintTo::create(0.3, m_bookDefineColor[posid], m_bookDefineColor[posid], m_bookDefineColor[posid]));
            }
            m_curIdx -= 1;
            if (m_curIdx<0) {
                m_curIdx = 5;
            }
            this->scheduleOnce(schedule_selector(EquipSiteView::onPlaySound), 0.2f);
            return true;
        } else if (m_movedPercent < -0.2) {
            for (int i = 5; i >=0 ; --i) {
                CCNode* bookNode;
                switch (i) {
                    case 0: bookNode = m_site0Node; break;
                    case 1: bookNode = m_site1Node; break;
                    case 2: bookNode = m_site2Node; break;
                    case 3: bookNode = m_site3Node; break;
                    case 4: bookNode = m_site4Node; break;
                    case 5: bookNode = m_site5Node; break;
                        
                    default:
                        break;
                }
                int oldposid = bookNode->getTag();
                int posid = oldposid-1;
                if (oldposid == 0) posid = 5;
                m_bookNodesMove2Posid[i] = posid; //标记目标位置
                if (posid == 4) {
                    bookNode->setPosition(m_bookDefinePos[4]);
                    bookNode->setVisible(true);
                }
                CCSpawn* spwan;
                if (posid == 5) {
                    spwan = CCSpawn::create(CCSequence::create(CCMoveTo::create(0.3, m_bookDefinePos[0]), CCCallFuncN::create       (this, callfuncN_selector(EquipSiteView::moveend)), NULL),
                                            CCScaleTo::create(0.3, m_bookDefineScale[posid]),
                                            NULL);

                } else {
                    spwan = CCSpawn::create(CCSequence::create(CCMoveTo::create(0.3, m_bookDefinePos[posid]), CCCallFuncN::create(this, callfuncN_selector(EquipSiteView::moveend)), NULL),
                                                 CCScaleTo::create(0.3, m_bookDefineScale[posid]),
                                                 NULL);
                }
                m_onAction++;
                bookNode->runAction(spwan);
                
                CCSprite* spr = nodeid2Sprite(i);
                spr->runAction(CCTintTo::create(0.3, m_bookDefineColor[posid], m_bookDefineColor[posid], m_bookDefineColor[posid]));
            }
            m_curIdx += 1;
            if (m_curIdx>5) {
                m_curIdx = 0;
            }
            this->scheduleOnce(schedule_selector(EquipSiteView::onPlaySound), 0.3f);
        }
        return true;
    }
    return false;
}

int EquipSiteView::node2Nodeid(CCNode* node) {
    int nodeid = 0;
    if (node==m_site0Node) {
        nodeid = 0;
    } else if (node==m_site1Node) {
        nodeid = 1;
    } else if (node==m_site2Node) {
        nodeid = 2;
    } else if (node==m_site3Node) {
        nodeid = 3;
    } else if (node==m_site4Node) {
        nodeid = 4;
    } else if (node==m_site5Node) {
        nodeid = 5;
    }
    return nodeid;
}

CCControlButton* EquipSiteView::nodeid2Button(int nodeid) {
    CCControlButton* testbtn = m_site0Btn;
    switch (nodeid) {
        case 0: testbtn = m_site0Btn; break;
        case 1: testbtn = m_site1Btn; break;
        case 2: testbtn = m_site2Btn; break;
        case 3: testbtn = m_site3Btn; break;
        case 4: testbtn = m_site4Btn; break;
        case 5: testbtn = m_site5Btn; break;
            
        default:
            break;
    }
    return testbtn;
}

CCSprite* EquipSiteView::nodeid2Sprite(int nodeid)
{
    CCSprite* retSpr = m_book0Spr;
    switch (nodeid) {
        case 0: retSpr = m_book0Spr; break;
        case 1: retSpr = m_book1Spr; break;
        case 2: retSpr = m_book2Spr; break;
        case 3: retSpr = m_book3Spr; break;
        case 4: retSpr = m_book4Spr; break;
        case 5: retSpr = m_book5Spr; break;
            
        default:
            break;
    }
    return retSpr;
}

void EquipSiteView::moveend(CCNode* node) {
    int nodeid = node2Nodeid(node);
    int posid = m_bookNodesMove2Posid[nodeid];
    node->setZOrder(m_bookDefineOrder[posid]);
    node->setTag(posid);
    if (posid == 5) {
        node->setVisible(false);
    }
    m_onAction--;
}

bool EquipSiteView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_onAction>0) {
        return false;
    }
    m_startY = pTouch->getLocation().y;
    
    CCNode* topNode = m_site0Node->getParent()->getChildByTag(2);
    int nodeid = node2Nodeid(topNode);
    CCControlButton* testbtn = nodeid2Button(nodeid);

    if (isTouchInside(testbtn, pTouch)) {
        testbtn->onTouchBegan(pTouch, pEvent);
    }
    
    if (isTouchInside(m_btn, pTouch)) {
        m_btn->onTouchBegan(pTouch, pEvent);
    }
    
    m_startTouchPoint = m_bookParent->convertTouchToNodeSpace(pTouch);
    m_nowTouchPoint = m_startTouchPoint;
    m_CanMove = true;
    
    return true;
}

void EquipSiteView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    m_nowTouchPoint = m_bookParent->convertTouchToNodeSpace(pTouch);
    m_movedPercent = m_nowTouchPoint.getDistance(m_startTouchPoint) / m_totalLength;
    if (m_nowTouchPoint.y > m_startTouchPoint.y) m_movedPercent = -m_movedPercent;
    
    if (m_CanMove && (m_movedPercent>0.2 || m_movedPercent<-0.2)) {
        m_CanMove = false;
        move(1);
    }
    
//    move(1);
//    if (m_onAction==0) {
//        m_startTouchPoint = m_nowTouchPoint;
//    }
}

void EquipSiteView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_CanMove = true;
    int endY = pTouch->getLocation().y;
    if (abs(endY - m_startY) >= 20) {
        return;
    }
    
    CCNode* topNode = m_site0Node->getParent()->getChildByTag(2);
    int nodeid = node2Nodeid(topNode);
    CCControlButton* testbtn = nodeid2Button(nodeid);
    
    if (isTouchInside(testbtn, pTouch)) {
        testbtn->onTouchBegan(pTouch, pEvent);
        onPlayClick(nodeid);
    }
    
    if (isTouchInside(m_btn, pTouch) && m_barNode->isVisible()) {
        m_btn->onTouchEnded(pTouch, pEvent);
        onClearCDClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}

void EquipSiteView::onPlayClick(int site)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_scroll);
    m_weapType = site;
    if (site == 0) {
        m_aniNode->setPosition(m_site0Node->getPosition());
    }else if (site == 1) {
        m_aniNode->setPosition(m_site1Node->getPosition());
    }else if (site == 2) {
        m_aniNode->setPosition(m_site2Node->getPosition());
    }else if (site == 3) {
        m_aniNode->setPosition(m_site3Node->getPosition());
    }else if (site == 4) {
        m_aniNode->setPosition(m_site4Node->getPosition());
    }else if (site == 5) {
        m_aniNode->setPosition(m_site5Node->getPosition());
    }
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(EquipSiteView::onPlayClickEnd));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Click");
    
    m_site0Node->runAction(CCScaleTo::create(0.3, 0));
    m_site1Node->runAction(CCScaleTo::create(0.3, 0));
    m_site2Node->runAction(CCScaleTo::create(0.3, 0));
    m_site3Node->runAction(CCScaleTo::create(0.3, 0));
    m_site4Node->runAction(CCScaleTo::create(0.3, 0));
    m_site5Node->runAction(CCScaleTo::create(0.3, 0));
    
    
}

void EquipSiteView::onPlayClickEnd()
{
    string aniName = this->getAnimationManager()->getLastCompletedSequenceName();
    if (aniName == "Click") {
        PopupViewController::getInstance()->addPopupInView(EquipmentListView::create(m_weapType));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("FR1_1"));
    }
}

void EquipSiteView::onPlaySound(float dt)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_slide);
}