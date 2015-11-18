//
//  DragonBuildBtnsView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuildBtnsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "DragonBuildingController.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "YesNoDialog.h"
#include "DragonBuildingController.h"

#include "DragonScene.h"
#include "DragonBuildView.h"
#include "DragonBuildUpgradeView.h"
#include "DragonBornView.h"
#include "DragonMakeEggView.h"
#include "DragonUpgradeView.h"
#include "DragonFoodMakeView.h"
#include "PropSpeedupView.h"

DragonBuildBtnsView* DragonBuildBtnsView::create()
{
    DragonBuildBtnsView *pRet = new DragonBuildBtnsView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DragonBuildBtnsView::init()
{
    CCBLoadFile("BuildBtnView",this,this);
    m_name1->setString(_lang("102271"));
    m_name2->setString(_lang("102270"));
    m_name1->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_name2->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_name3->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_name4->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_msg5->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_goldLabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_IncreasedGlodTxt->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_name5->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_IncreasedTime->setFntFile(getNBFont(NB_FONT_Bold_Border));
    m_msgV5->setFntFile(getNBFont(NB_FONT_Bold_Border));
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DragonBuildBtnsView::animationFunc));
    return true;
}

void DragonBuildBtnsView::onHide()
{
    m_initEnd = false;
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void DragonBuildBtnsView::onShow(int buildId)
{
    guidKey = "";
    m_toolInfoId = 0;
    m_info = &((*DragonBuildingController::getInstance()->curDragonBuildsInfo)[buildId]);
    m_buildingKey = buildId;
    m_toolNode->setVisible(false);
    m_nameLabel->setString(_lang(m_info->name));
    m_qid = QID_MAX;
    
    m_btn1Node->setPosition(ccp(-52, -10));
    m_btn2Node->setPosition(ccp(-156, 30));
    m_btn3Node->setPosition(ccp(52, -10));
    m_btn4Node->setPosition(ccp(156, 30));
    
    m_btn1->setTouchPriority(Touch_Imperial_City);
    m_btn2->setTouchPriority(Touch_Imperial_City);
    m_btn3->setTouchPriority(Touch_Imperial_City);
    m_btn4->setTouchPriority(Touch_Imperial_City);
    m_btn5->setTouchPriority(Touch_Imperial_City);
    
    int cellCnt = 2;
    buildType = m_buildingKey/1000;
    vector<string> btnIcons;
    if (buildType == DBUILDING_HATCHER) {//龙孵化场
        btnIcons.push_back("affairs_icon.png");
        m_name3->setString(_lang("102281"));
    }
    else if (buildType == DBUILDING_FARM) {//龙农场
        btnIcons.push_back("alliances_trade.png");
        m_name3->setString(_lang("102280"));
    }
    else if (buildType == DBUILDING_HABITAT_1 || buildType == DBUILDING_HABITAT_2 || buildType == DBUILDING_HABITAT_3 || buildType == DBUILDING_HABITAT_4 || buildType == DBUILDING_HABITAT_5 || buildType == DBUILDING_HABITAT_6 || buildType == DBUILDING_HABITAT_7) {//龙栖息地
        btnIcons.push_back("science_icon.png");
        m_name3->setString(_lang("102275"));
    }
    else if (buildType == DBUILDING_CAVE) {//繁殖洞穴
        btnIcons.push_back("icon_forging01.png");
        m_name3->setString(_lang("102349"));
    }
    
    m_btn3Node->setVisible(false);
    m_btn3->setEnabled(false);
    m_IncreasedNode->setVisible(false);
    if (btnIcons.size() >= 1) {
        m_icon3Node->removeAllChildren();
        auto icon = CCLoadSprite::createSprite(btnIcons[0].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_icon3Node->addChild(icon);
        m_btn3Node->setVisible(true);
        m_btn3->setEnabled(true);
        cellCnt=3;
    }
    
    m_btn4Node->setVisible(false);
    m_btn4->setEnabled(false);
    if (btnIcons.size() >= 2) {
        m_icon4Node->removeAllChildren();
        auto icon = CCLoadSprite::createSprite(btnIcons[1].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_btn4Node->setVisible(true);
        m_btn4->setEnabled(true);
        cellCnt=4;
        m_icon4Node->addChild(icon);
    }
    
    m_btn5Node->setVisible(false);
    m_btn5->setEnabled(false);
    
    if (cellCnt==3) {
        m_btn1Node->setPosition(ccp(0, -16));
        m_btn2Node->setPosition(ccp(-107, 6));
        m_btn3Node->setPosition(ccp(107, 6));
    }
    else if (cellCnt==2) {
        m_btn1Node->setPosition(ccp(52, -10));
        m_btn2Node->setPosition(ccp(-52, -10));
    }
    else if (cellCnt==4)
    {
        m_btn2Node->setPosition(ccp(-52, -10));
        m_btn3Node->setPosition(ccp(-156, 30));
        m_btn1Node->setPosition(ccp(52, -10));
        m_btn4Node->setPosition(ccp(156, 30));
    }
    m_btnNum = cellCnt;
    this->setVisible(true);
    onShowInfo();
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

bool DragonBuildBtnsView::onShowInfo()
{
    m_toolNode->setVisible(true);
    bool isTmp = false;
    int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
    if (qType>0){//判断是否是 特殊建筑，并取得是否有正在工作的 队列
        isTmp = true;
        m_qid = QueueController::getInstance()->getMinTimeQidByType(qType);
    }
    
    if (m_info->state == FUN_BUILD_NORMAL)
    {
        if (m_qid == QID_MAX)
        {
            m_toolNode->setPositionY(-93.6+60);
            m_nameNode->setPositionY(250);
            if (false){//小建筑
                m_nameNode->setPositionY(150);
            }
            m_spdNode->setVisible(false);
            m_icon1Node->removeAllChildren();
            auto icon = CCLoadSprite::createSprite("build_up_icon.png");
            CCCommonUtils::setSpriteMaxSize(icon, 90, true);
            m_icon1Node->addChild(icon);
            m_name2->setString(_lang("102270"));
            
            if(buildType == DBUILDING_FARM) {//龙农场
                if (true) {//是否生产过粮食
                    //显示重复种植按钮
                }else {
                    
                }
//                m_btn1Node->setPosition(ccp(0, -16));
//                m_btn2Node->setPosition(ccp(-107, 6));
//                m_btn3Node->setPosition(ccp(107, 6));
//                m_btn3Node->setVisible(true);
//                m_btn3->setEnabled(true);
            }
        }
        else {//建筑正在孵化 或者 生蛋
            m_toolNode->setPositionY(-93.6);
            m_nameNode->setPositionY(250+60);
            if (false){//小建筑
                m_nameNode->setPositionY(150+60);
            }
            m_spdNode->setVisible(true);
            m_icon1Node->removeAllChildren();
            m_name2->setString(_lang("104903"));
            
            if( onUpdateToolBtn(0)) {
                if (buildType == DBUILDING_CAVE || buildType == DBUILDING_HATCHER) {
                    m_btn1Node->setPosition(ccp(-52, -10));
                    m_btn2Node->setPosition(ccp(-156, 30));
                    m_btn3Node->setPosition(ccp(156, 30));
                    m_btn5Node->setPosition(ccp(52, -10));
                }
            }
            else {
                if (buildType == DBUILDING_CAVE || buildType == DBUILDING_HATCHER) {
                    m_btn1Node->setPosition(ccp(0, -16));
                    m_btn2Node->setPosition(ccp(-107, 6));
                    m_btn3Node->setPosition(ccp(107, 6));
                }
            }
        }
    }
    else {//升级 或者 建造中， 显示 秒建筑的 cd按钮
        m_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD, CC_ITOA(m_buildingKey));
        m_toolNode->setPositionY(-93.6);
        m_nameNode->setPositionY(250+60);
        if (false){//小建筑
            m_nameNode->setPositionY(150+60);
        }
        m_spdNode->setVisible(true);
        m_icon1Node->removeAllChildren();
        m_name2->setString(_lang("104903"));
        
        if(buildType == DBUILDING_FARM) {//龙农场
            if (true) {//是否生产过粮食
                //显示重复种植按钮
            }else {
                
            }
        }
        
        if(onUpdateToolBtn(0))
        {
            if(m_btnNum==4) {
                m_btn1Node->setPosition(ccp(-107, 6));
                m_btn2Node->setPosition(ccp(-200, 60));
                m_btn3Node->setPosition(ccp(107, 6));
                m_btn4Node->setPosition(ccp(200, 60));
                
                m_btn5Node->setPosition(ccp(0, -16));
            }
            if(m_btnNum==3) {
                m_btn1Node->setPosition(ccp(-52, -10));
                m_btn2Node->setPosition(ccp(-156, 30));
                m_btn3Node->setPosition(ccp(156, 30));
                m_btn5Node->setPosition(ccp(52, -10));
            }
            if(m_btnNum==2) {
                m_btn1Node->setPosition(ccp(0, -16));
                m_btn2Node->setPosition(ccp(-107, 6));
                m_btn5Node->setPosition(ccp(107, 6));//显示道具加速按钮
            }
        }
    }
    
    onEnterFrame(0);
    
    return true;
}

void DragonBuildBtnsView::animationFunc()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "FadeOut") {
        this->setVisible(false);
    }
    else if (name == "FadeIn") {
        m_initEnd = true;
    }
}

void DragonBuildBtnsView::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DragonBuildBtnsView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Imperial_City+1, true);
}

void DragonBuildBtnsView::onExit() {
    CCNode::onExit();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DragonBuildBtnsView::onEnterFrame), this);
    setTouchEnabled(false);
}

bool DragonBuildBtnsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (this->isVisible() && isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void DragonBuildBtnsView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void DragonBuildBtnsView::onEnterFrame(float dt)
{
    if (this->isVisible())
    {
        if(m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            lastTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
            m_gold = CCCommonUtils::getGoldByTime(lastTime);
            if (qInfo.type == TYPE_DRAGON_BUILD) {
                if (lastTime <= GlobalData::shared()->freeSpdT) {
                    m_gold = 0;
                    this->setVisible(false);
                }
            }
            if (lastTime<=0 && qInfo.type != TYPE_DRAGON_BUILD) {
                auto layer = DragonScene::instance();
                if(layer) {
                    layer->setUnMoveScence(false);
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
                this->setVisible(false);
            }
            m_goldLabel->setString(CC_ITOA(m_gold));
        }
        else if (m_info->state == FUN_BUILD_NORMAL) {
//            if(!m_btn4->isEnabled() && FunBuildController::getInstance()->getOutPut(m_info->itemId)>0 )
//            {
//                m_btn4->setEnabled(true);
//                m_icon4Node->removeAllChildren();
//                string iconStr = "";
//                if (buildType == FUN_BUILD_FOOD) {
//                    iconStr = CCCommonUtils::getResourceIconByType(Food);
//                }else if (buildType == FUN_BUILD_WOOD) {
//                    iconStr = CCCommonUtils::getResourceIconByType(Wood);
//                }else if (buildType == FUN_BUILD_IRON) {
//                    iconStr = CCCommonUtils::getResourceIconByType(Iron);
//                }else if (buildType == FUN_BUILD_STONE) {
//                    iconStr = CCCommonUtils::getResourceIconByType(Stone);
//                }
//                auto icon = CCLoadSprite::createSprite(iconStr.c_str());
//                CCCommonUtils::setSpriteMaxSize(icon, 90, true);
//                m_icon4Node->addChild(icon);
//            }
        }
        
        if (m_toolInfoId>0 && m_btn5->isEnabled())
        {
            auto& m_toolInfo = ToolController::getInstance()->getToolInfoById(m_toolInfoId);
            if (m_toolInfo.getCNT()<=0)
            {
                if (m_toolInfo.type == ITEM_TYPE_SPD) {//加速道具 没了
                    if(onUpdateToolBtn(0)==true)
                        return;
                    m_btn5Node->setVisible(false);
                    m_btn5->setEnabled(false);
                    if (m_info->state==FUN_BUILD_NORMAL) {//科技
                        m_btn1Node->setPosition(ccp(0, -16));
                        m_btn2Node->setPosition(ccp(-107, 6));
                        m_btn3Node->setPosition(ccp(107, 6));
                    }
                    else {
                        m_btn1Node->setPosition(ccp(52, -10));
                        m_btn2Node->setPosition(ccp(-52, -10));
                        int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
                        if (qType==-1 && (m_info->state==FUN_BUILD_UPING || m_info->state==FUN_BUILD_CREATE)) {
                            if(m_btnNum==4) {
                                m_btn1Node->setPosition(ccp(-52, -10));
                                m_btn2Node->setPosition(ccp(-156, 30));
                                m_btn4Node->setPosition(ccp(156, 30));
                                m_btn3Node->setPosition(ccp(52, -10));
                            }
                            if(m_btnNum==3) {
                                m_btn1Node->setPosition(ccp(0, -16));
                                m_btn2Node->setPosition(ccp(-107, 6));
                                m_btn3Node->setPosition(ccp(107, 6));
                            }
                        }
                    }
                }
                else {//资源加速生产道具没了
                    m_btn5Node->setVisible(false);
                    m_btn5->setEnabled(false);
                    m_btn2Node->setPosition(ccp(-52, -10));
                    m_btn3Node->setPosition(ccp(-156, 30));
                    m_btn1Node->setPosition(ccp(52, -10));
                    m_btn4Node->setPosition(ccp(156, 30));
                }
            }
        }
    }
}

SEL_MenuHandler DragonBuildBtnsView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler DragonBuildBtnsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", DragonBuildBtnsView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", DragonBuildBtnsView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", DragonBuildBtnsView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", DragonBuildBtnsView::onBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn5Click", DragonBuildBtnsView::onBtn5Click);
    return NULL;
}

bool DragonBuildBtnsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldLabel", CCLabelIF*, this->m_goldLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, this->m_btn5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1Node", CCNode*, this->m_btn1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2Node", CCNode*, this->m_btn2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3Node", CCNode*, this->m_btn3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4Node", CCNode*, this->m_btn4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5Node", CCNode*, this->m_btn5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1Node", CCNode*, this->m_icon1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2Node", CCNode*, this->m_icon2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3Node", CCNode*, this->m_icon3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon4Node", CCNode*, this->m_icon4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon5Node", CCNode*, this->m_icon5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolNode", CCNode*, this->m_toolNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdNode", CCNode*, this->m_spdNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1", CCLabelIF*, this->m_name1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2", CCLabelIF*, this->m_name2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3", CCLabelIF*, this->m_name3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4", CCLabelIF*, this->m_name4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5", CCLabelIF*, this->m_name5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg5", CCLabelIF*, this->m_msg5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgV5", CCLabelIF*, this->m_msgV5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedNode", CCNode*, this->m_IncreasedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedGold", CCSprite*, this->m_IncreasedGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedGlodTxt", CCLabelIF*, this->m_IncreasedGlodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedTime", CCLabelIF*, this->m_IncreasedTime);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    return false;
}

void DragonBuildBtnsView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    if (m_info->state == FUN_BUILD_NORMAL) {
        if(m_qid == QID_MAX) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
            auto layer = DragonScene::instance();
            if(layer) {
                layer->setUnMoveScence(false);
                layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
            }
            PopupViewController::getInstance()->addPopupInView(DragonBuildUpgradeView::create(m_buildingKey));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("BU_%d_up", buildType));
        }
        else {
            if(PopupViewController::getInstance()->CanPopPushView) {
                YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(DragonBuildBtnsView::onCostGoldBack)), lastTime, _lang("104903").c_str(), false);
            }
            else {
                if (m_gold > 0) {
                    if (GlobalData::shared()->playerInfo.gold < m_gold) {
                        YesNoDialog::gotoPayTips();
                        return;
                    }
                }
                QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
            }
        }
    }
    else {
        if(PopupViewController::getInstance()->CanPopPushView && m_gold>0) {
            YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(DragonBuildBtnsView::onCostGoldBack)), lastTime, _lang("104903").c_str());
        }
        else {
            DragonBuildingController::getInstance()->costCD(m_buildingKey, "", m_gold);
        }
    }
}

void DragonBuildBtnsView::onCostGoldBack()
{
    if (m_info->state == FUN_BUILD_NORMAL) {
        if (m_gold > 0) {
            if (GlobalData::shared()->playerInfo.gold < m_gold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
    }
    else {
        DragonBuildingController::getInstance()->costCD(m_buildingKey, "", m_gold);
    }
}

void DragonBuildBtnsView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
    auto layer = DragonScene::instance();
    if(layer) {
        layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
    }
    PopupViewController::getInstance()->addPopupInView(DragonBuildView::create(m_buildingKey), true, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("BU_%d_detail", buildType));
}

void DragonBuildBtnsView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    
    if (buildType == DBUILDING_HATCHER) {
        PopupViewController::getInstance()->addPopupInView(DragonBornView::create());
    }else if (buildType == DBUILDING_CAVE) {
        PopupViewController::getInstance()->addPopupInView(DragonMakeEggView::create());
    }else if (buildType == DBUILDING_FARM) {
        PopupViewController::getInstance()->addPopupInView(DragonFoodMakeView::create());
    }else if (buildType==DBUILDING_HABITAT_1 || buildType==DBUILDING_HABITAT_2 || buildType==DBUILDING_HABITAT_3 || buildType==DBUILDING_HABITAT_4 || buildType==DBUILDING_HABITAT_5 || buildType==DBUILDING_HABITAT_6 || buildType==DBUILDING_HABITAT_7) {
        //收取龙产的资源
        CCLOG("shou qu long chan de zi yuan");
    }
}

void DragonBuildBtnsView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    m_toolNode->setVisible(false);
    onHideShadow();
    if (buildType == DBUILDING_FARM) {
        CCLOG("chong fu zhong zhi!");
    }
}

void DragonBuildBtnsView::onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    
    if (m_toolInfoId<=0) {
        return;
    }
    auto& m_toolInfo = ToolController::getInstance()->getToolInfoById(m_toolInfoId);
    
    if (m_toolInfo.type == ITEM_TYPE_SPD) {//加速道具
        if(m_qid != QID_MAX) {
            PropSpeedupView::show(getSPDTypeByBuildingState(), m_buildingKey, m_qid);
        }
    }
    else {//资源加速生产道具
//        m_info->effectTime = GlobalData::shared()->getWorldTime()+24*60*60-1;
//        BuildingAddOutPutCommand* cmd = new BuildingAddOutPutCommand(m_info->uuid,m_buildingKey, m_toolInfo.uuid);
//        cmd->sendAndRelease();
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
//        int tCnt = m_toolInfo.getCNT() - 1;
//        m_toolInfo.setCNT(tCnt);
//        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(m_toolInfo.itemId)), "", _lang_1("104909",m_toolInfo.getName().c_str()));
    }
    
    m_toolNode->setVisible(false);
    onHideShadow();
}

void DragonBuildBtnsView::onHideShadow(bool st)
{
    this->setVisible(false);
    auto layer = DragonScene::instance();
    if(layer) {
        layer->stopShadow(m_buildingKey,NULL,NULL);
        if (st) {
            layer->onResetLastBuildId();
        }
    }
}

void DragonBuildBtnsView::onHideBtn()
{
    m_toolNode->setVisible(false);
    onHideShadow(false);
}

CCNode* DragonBuildBtnsView::getGuideNode(string _key)
{
    if (m_initEnd && this->isVisible() && m_toolNode->isVisible() && m_toolNode->getScale()>=1.0) {
        if(_key=="up") {
            guidKey = _key;
            return m_btn1;
        }
        else if(_key=="detail") {
            guidKey = _key;
            return m_btn2;
        }
        else if(_key=="cd") {
            if (m_btn1Node->isVisible()) {
                guidKey = _key;
                return m_btn1;
            }
            else {
                return NULL;
            }
        }
        else if(_key=="soldier") {
            guidKey = _key;
            return m_btn3;
        }
        else if(_key=="science") {
            guidKey = _key;
            return m_btn3;
        }
        else if(_key=="forge") {
            guidKey = _key;
            return m_btn3;
        }
    }
    return NULL;
}

void DragonBuildBtnsView::onGuidClick()
{
    if(guidKey=="up") {
        guidKey = "";
        onBtn1Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="detail") {
        guidKey = "";
        onBtn2Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="cd") {
        guidKey = "";
        onBtn1Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="soldier") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="science") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="forge") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
}
int DragonBuildBtnsView::getSPDTypeByBuildingState(){
    int speedType = 0;
    switch (m_info->state) {
        case FUN_BUILD_NORMAL:{
            switch (buildType) {
                case FUN_BUILD_BARRACK1:
                case FUN_BUILD_BARRACK2:
                case FUN_BUILD_BARRACK3:
                case FUN_BUILD_BARRACK4:{
                    speedType = ItemSpdMenu_Soldier;
                    break;
                }
                case FUN_BUILD_FORT:{
                    speedType = ItemSpdMenu_Trap;
                    break;
                }
                case FUN_BUILD_SCIENE:{
                    speedType = ItemSpdMenu_Science;
                    break;
                }
                case FUN_BUILD_HOSPITAL:{
                    speedType = ItemSpdMenu_Heal;
                    break;
                }
                case FUN_BUILD_FORGE:{
                    speedType = ItemSpdMenu_DuanZao;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case FUN_BUILD_UPING:
        case FUN_BUILD_CREATE:
        case FUN_BUILD_INIT:{
            speedType = ItemSpdMenu_City;
            break;
        }
        default:
            break;
    }
    return speedType;
}
bool DragonBuildBtnsView::onUpdateToolBtn(int type)
{
    int toolId = 0;
    if (type == 0) {//加速
        toolId = ToolController::getInstance()->getSPDItem(getSPDTypeByBuildingState());
    }else if (type == FUN_BUILD_WOOD) {//木
        toolId = 200027;
    }else if (type == FUN_BUILD_FOOD) {//粮
        toolId = 200028;
    }else if (type == FUN_BUILD_IRON) {//铁
        toolId = 200029;
    }else if (type == FUN_BUILD_STONE) {//石
        toolId = 200030;
    }else {
        return false;
    }
    if(toolId <=0 )
        return false;
    
    auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolId);
    if (toolInfo.getCNT()<=0) {
        return false;
    }
    
    m_toolInfoId = toolId;
    m_icon5Node->removeAllChildren();
    auto icon = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolId)).c_str());
    CCCommonUtils::setSpriteMaxSize(icon, 90, true);
    m_icon5Node->addChild(icon);
    m_btn5Node->setVisible(true);
    m_btn5->setEnabled(true);
    if (toolInfo.type == ITEM_TYPE_SPD) {
        m_name5->setString("");
        m_msg5->setString(_lang("104903"));
        //m_msgV5->setString("1"+_lang("105591"));
        m_msgV5->setString("");
    }
    else {
        m_name5->setString(_lang_1("104907", CC_ITOA(toolInfo.getCNT())));
        m_msg5->setString(_lang("102293"));
        m_msgV5->setString("1d");
    }
    
    return true;
}