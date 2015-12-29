//
//  BuildUpgradeView.cpp
//  IF
//
//  Created by fubin on 14-5-16.
//
//

#include "BuildUpgradeView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "QueueController.h"
#include "UseResToolView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "ScienceListView.h"
#include "BuildMoreInfoView.h"
#include "StoreView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "BatchBuyItemView.h"
#include "ConfirmDialogView.h"
#include "SoundController.h"
#include "CCFlagWaveSprite.h"
#include "DragonBuildingController.h"
#include "DragonScene.h"
#include "ArmyInfo.h"
#include "WorldMapView.h"
#include "GeneralTitanPopupView.h"

#define MSG_BUILD_CELL "msg_build_cell"

BuildUpgradeView* BuildUpgradeView::create(int buildId, int pos){
    BuildUpgradeView* ret = new BuildUpgradeView();
    if(ret && ret->init(buildId, pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildUpgradeView::init(int buildId, int pos)
{
    if (!ArcPopupBaseView::init(TYPE_POS_TO_UP)) {
        return false;
    }
    setIsHDPanel(true);
    m_isReturn = true;
    m_buildId = buildId;
    m_pos = pos;
    auto tmpCCB = CCBLoadFile("Upgrade",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    
    this->getArcModelLayer()->setVisible(false);
    
    int addHeight = getExtendHeight();
    
    auto wSize = Director::getInstance()->getWinSize();
    
    if (wSize.height/wSize.width>1.6) {//fusheng 手机
        auto oldR =m_msg_BG->getBoundingBox();
        
        auto oldS = m_msg_BG->getScaleY();
        
        m_msg_BG->setScaleY(1);
        
        auto newR =m_msg_BG->getBoundingBox();
        
        if (newR.size.height - newR.size.height>addHeight/2) {//fusheng 调整大小后大于infoNode移动距离时 不放缩
            m_msg_BG->setScaleY(oldS);
        }
        else
        {
            BGNode2->setPositionY(BGNode2->getPositionY()-(newR.size.height - oldR.size.height));
            m_buildBG2->setPositionY(m_buildBG2->getPositionY()-(newR.size.height - oldR.size.height));
            m_nbNameNode->setPositionY(m_nbNameNode->getPositionY()-(newR.size.height - oldR.size.height));
            m_info_BG->setPositionY(m_info_BG->getPositionY()-addHeight/2);
            m_infoList->setPositionY(m_infoList->getPositionY()-addHeight/2);

        }
        
    }
    else
    {
        
    }
    
    
//    int oldBgWidth = m_buildBG->getContentSize().width;
//    int oldBgHeight = m_buildBG->getContentSize().height;
//    int newBgHeight = addHeight/2+oldBgHeight;
//    m_buildBG->setContentSize(CCSizeMake(oldBgWidth, newBgHeight));
//    int oldWidth = m_infoList->getContentSize().width;
//    int oldHeight = m_infoList->getContentSize().height;

//    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight/2));
    m_btnNode->setPositionY(m_btnNode->getPositionY()-addHeight);
    
    m_upBtnMsgLabel->setString(_lang("102104").c_str());
    m_btnMsgLabel->setString(_lang("102127").c_str());
    m_inBtnGoldNum->setString("1");
    m_demandLabel->setString(_lang("102130"));
    
    if(m_pos>0) {
        m_upBtnMsgLabel->setString(_lang("102126").c_str());
        m_btnMsgLabel->setString(_lang("102128").c_str());
    }
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    m_itemScrollView = CCScrollView::create(m_iconList->getContentSize());
    m_itemScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    m_itemScrollView->setTouchEnabled(false);
    m_iconList->addChild(m_itemScrollView);
    
    updateInfo(NULL);
    m_openNum=0;
    
//    m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
//    CCActionInterval * moveBy2 = CCMoveBy::create(0.25, ccp(0,200));
    
//    m_mainNode->setScale(0);
//    CCActionInterval * delay = CCDelayTime::create(0);
//    CCActionInterval * scaleTo = CCScaleTo::create(0.25, 1.0);
//    m_mainNode->runAction(CCSequence::create(delay,scaleTo, NULL));
    
    vector<CCNode*> tmpVec;
    CCCommonUtils::getRGBAProtocols(m_mainNode, &tmpVec);
    for (int i=0; i<tmpVec.size(); i++) {
        CCActionInterval * fadeIn = CCFadeIn::create(0.5);
        tmpVec[i]->runAction(fadeIn);
    }
    
//    auto resSp = CCLoadSprite::loadResource("ye.png");
//    auto wareSpr = CCFlagWaveSprite::create(resSp);
//    wareSpr->setPosition(ccp(100, 100));
//    m_infoList->addChild(wareSpr);
    
//    auto wareTest = CCLoadSprite::createSprite("Allance_flay.png");
//    wareTest->setPosition(ccp(250, 250));
//    m_infoList->addChild(wareTest);
//    CCActionInterval * waves =  CCWavesTiles3D::create(50, CCSize(20, 20), 50, 5.0f);
//    wareTest->runAction(waves);
    
    m_unlockLable->setString("");
    m_arrowNode->setVisible(false);
    if (m_pos<=0) {
        updateParasInfo();
    }
    onShowNextUnlockItem();
    
    return true;
}

void BuildUpgradeView::updateInfo(CCObject* obj)
{
    bool isCanUp = true;
    
    string name = "";
    int silver_need = 0;
    int wood_need = 0;
    int time_need = 0;
    int stone_need = 0;
    int food_need = 0;
    int iron_need = 0;
    string tool_need = "";
    string building = "";
    string destip = "";
    int level = 0;
    
    if (m_pos>0) {
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
        silver_need = dictInfo->valueForKey("silver")->intValue();
        wood_need = dictInfo->valueForKey("wood")->intValue();
        time_need = dictInfo->valueForKey("time")->intValue();
        stone_need = dictInfo->valueForKey("stone")->intValue();
        food_need = dictInfo->valueForKey("food")->intValue();
        iron_need = dictInfo->valueForKey("iron")->intValue();
        tool_need = dictInfo->valueForKey("item")->getCString();
        building = dictInfo->valueForKey("building")->getCString();
        destip = dictInfo->valueForKey("description")->getCString();
    }
    else {
        auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
        silver_need = m_info.silver_need;
        wood_need = m_info.wood_need;
        time_need = m_info.time_need;
        stone_need = m_info.stone_need;
        food_need = m_info.food_need;
        iron_need = m_info.iron_need;
        tool_need = m_info.tool_need;
        building = m_info.building;
        level = m_info.level;
//        destip = m_info.destip;
        destip = m_info.tip;
    }
    _tmpTime = time_need;
    m_nameLabel->setString(_lang(name));
    
    string title = _lang_1("102272", CC_ITOA(level));
    if(level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabel->setString(title);
    m_nextLvLabel->setString(CC_ITOA(level+1));
    m_nextLabel->setString(_lang("102176"));
    m_nextLabel->setMaxScaleXByWidth(CCCommonUtils::isIosAndroidPad()? 290.0 : 140.0);
    
    if (m_pos>0) {
        m_lvLabel->setString("");
        m_nextLvLabel->setString("");
        m_nextLabel->setString("");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nameLabel->setPositionY(m_nameLabel->getPositionY() + 19.4);
        } else {
            m_nameLabel->setPositionY(m_nameLabel->getPositionY()+10);
        }
    }
    float tmpEffect = CCCommonUtils::getEffectValueByNum(68);
    _tmpTime = time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    _resGold = 0;
    
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());
    if (m_openNum==0) {
        m_nextMsgLabel->setString(_lang(destip));
    }
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 87;
        curX = 60;
        curY = 50;
    }
    
    m_instantBtn->setEnabled(true);
    m_scrollView->getContainer()->removeAllChildren();
    
    m_lfood = 0;
    m_lwood = 0;
    m_liron = 0;
    m_lstone = 0;
    
    if (stone_need>0) {
        auto item3 = UpgradeCell::create(0, Stone, stone_need);
        item3->setPosition(ccp(curX, curY));
        item3->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        if (!item3->m_isOk) {
//            isCanUp = false;
        }
        
        int tmpSub = stone_need - GlobalData::shared()->resourceInfo.lStone;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Stone, tmpSub);
            m_lstone = tmpSub;
        }
    }
    if (iron_need>0) {
        auto item4 = UpgradeCell::create(0, Iron, iron_need);
        item4->setPosition(ccp(curX, curY));
        item4->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item4);
        curY += _itemH;
        
        if (!item4->m_isOk) {
//            isCanUp = false;
        }
        
        int tmpSub = iron_need - GlobalData::shared()->resourceInfo.lIron;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Iron, tmpSub);
            m_liron = tmpSub;
        }
    }
    if (food_need>0) {
        auto item5 = UpgradeCell::create(0, Food, food_need);
        item5->setPosition(ccp(curX, curY));
        item5->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item5);
        curY += _itemH;
        
        if (!item5->m_isOk) {
//            isCanUp = false;
        }
        
        int tmpSub = food_need - GlobalData::shared()->resourceInfo.lFood;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Food, tmpSub);
            m_lfood = tmpSub;
        }
    }
    
    
    if (wood_need>0) {
        auto item2 = UpgradeCell::create(0, Wood, wood_need);
        item2->setPosition(ccp(curX, curY));
        item2->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        if (!item2->m_isOk) {
//            isCanUp = false;
        }
        
        int tmpSub = wood_need - GlobalData::shared()->resourceInfo.lWood;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Wood, tmpSub);
            m_lwood = tmpSub;
        }
    }
    
    if (tool_need != "") {
        std::vector<string> toolItems;
        CCCommonUtils::splitString(tool_need, "|", toolItems);
        for (int i=0; i<toolItems.size(); i++) {
            string curTool = toolItems[i];
            std::vector<string> item;
            CCCommonUtils::splitString(curTool, ";", item);
            int id = atoi(item[0].c_str());
            int num = atoi(item[1].c_str());
            
            auto itemTool = UpgradeCell::create(1, id, num);
            itemTool->setPosition(ccp(curX, curY));
            itemTool->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemTool);
            curY += _itemH;
            
            if (!itemTool->m_isOk) {
                auto toolInfo = ToolController::getInstance()->getToolInfoById(id);
                _resGold += (num - toolInfo.getCNT()) * toolInfo.price;
//                isCanUp = false;
            }
        }
    }
    
    _tmpGold = _tmpGold + _resGold;
    m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
    
    if (building!="") {
        std::vector<string> buildItems;
        CCCommonUtils::splitString(building, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            string curBuild = buildItems[i];
            std::vector<string> tmpbuildVec;
            CCCommonUtils::splitString(curBuild, ";", tmpbuildVec);
            int paramBuild = atoi(tmpbuildVec[0].c_str())+atoi(tmpbuildVec[1].c_str());
            auto itemBuild = UpgradeCell::create(2, paramBuild, 0);
            itemBuild->setPosition(ccp(curX, curY));
            itemBuild->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemBuild);
            curY += _itemH;
            
            if (!itemBuild->m_isOk) {
                isCanUp = false;
                m_instantBtn->setEnabled(false);
            }
        }
    }
    
    //没有队列，显示条件不足
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, 1);//_tmpTime, 取空闲队列，并不判断有效期是否够用
    if(QID_MAX == qid)
    {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        auto itemQueue = UpgradeCell::create(3, Food, qid);
        itemQueue->setPosition(ccp(curX, curY));
        itemQueue->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(itemQueue);
        curY += _itemH;
        m_qid = qid;
    }
    else {
        m_qid = qid;
    }
    m_upBtn->setEnabled(isCanUp);
    curY -= 23;
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    bool touchable = curY < m_infoList->getContentSize().height?false:true;
    m_scrollView->setTouchEnabled(touchable);
    
}

void BuildUpgradeView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo(NULL);
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildUpgradeView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void BuildUpgradeView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool BuildUpgradeView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_x = m_itemScrollView->getContentOffset().x;
    return true;
}
void BuildUpgradeView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_arrowNode->isVisible()) {
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        int num = 0;
        if(dx > 10){
            num = 1;
        }else if (dx < -10) {
            num = -1;
        }
        int newOffx = m_x+num*m_unlockItemWidth;
        if (newOffx < -(m_sumItemWidth-m_unlockItemWidth)) {
            newOffx = -(m_sumItemWidth-m_unlockItemWidth);
        }else if (newOffx > 0) {
            newOffx = 0;
        }
        m_itemScrollView->setContentOffset(ccp(newOffx, 0), true);
        
        int offx = m_itemScrollView->getContentOffset().x;
        if (m_unlockItemWidth/2 < abs(offx) && !m_lArrow->isVisible()) {
            m_lArrow->setVisible(true);
        }
        if ( m_sumItemWidth-abs(offx) < m_unlockItemWidth*1.5  && m_rArrow->isVisible()) {
            m_rArrow->setVisible(false);
        }
        if ( abs(offx) <= m_unlockItemWidth/2  && m_lArrow->isVisible()) {
            m_lArrow->setVisible(false);
        }
        if ( m_sumItemWidth-abs(offx) >= m_unlockItemWidth*1.5  && !m_rArrow->isVisible()) {
            m_rArrow->setVisible(true);
        }
    }
}
void BuildUpgradeView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return;
    if (m_arrowNode->isVisible()) {
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        if(dx > 10 || dx < -10)
        {
            int offX = m_itemScrollView->getContentOffset().x + dx;
            int min = -(m_sumItemWidth - m_unlockItemWidth) ;
            int max = 0;
            if(offX > max){
                offX = max;
            }else if(offX < min){
                offX = min;
            }
            m_itemScrollView->setContentOffset(ccp(offX, 0), false);
        }
        
        int offx = m_itemScrollView->getContentOffset().x;
        if (m_unlockItemWidth/2 < abs(offx) && !m_lArrow->isVisible()) {
            m_lArrow->setVisible(true);
        }
        
        if ( m_sumItemWidth-abs(offx) < m_unlockItemWidth*1.5  && m_rArrow->isVisible()) {
            m_rArrow->setVisible(false);
        }
        
        if ( abs(offx) <= m_unlockItemWidth/2  && m_lArrow->isVisible()) {
            m_lArrow->setVisible(false);
        }
        
        if ( m_sumItemWidth-abs(offx) >= m_unlockItemWidth*1.5  && !m_rArrow->isVisible()) {
            m_rArrow->setVisible(true);
        }
    }
}

SEL_CCControlHandler BuildUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", BuildUpgradeView::onCreateOrUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", BuildUpgradeView::onInstantClick);
    return NULL;
}

bool BuildUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nbNameNode", CCNode*, this->m_nbNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG2", CCScale9Sprite*, this->m_buildBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "BGNode2", CCNode*, this->BGNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg_BG", CCSprite*, this->m_msg_BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info_BG", CCScale9Sprite*, this->m_info_BG);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnNode", CCNode*, this->m_inBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, this->m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, this->m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextMsgLabel", CCLabelIF*, this->m_nextMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_demandLabel", CCLabelIF*, this->m_demandLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para1Node", CCNode*, this->m_para1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para1Name", CCLabelIF*, this->m_para1Name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para1Num", CCLabelIF*, this->m_para1Num);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_next1Num", CCLabelIF*, this->m_next1Num);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para2Node", CCNode*, this->m_para2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para2Name", CCLabelIF*, this->m_para2Name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para2Num", CCLabelIF*, this->m_para2Num);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_next2Num", CCLabelIF*, this->m_next2Num);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para3Node", CCNode*, this->m_para3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para3Name", CCLabelIF*, this->m_para3Name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para3Num", CCLabelIF*, this->m_para3Num);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_next3Num", CCLabelIF*, this->m_next3Num);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para4Node", CCNode*, this->m_para4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para4Name", CCLabelIF*, this->m_para4Name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para4Num", CCLabelIF*, this->m_para4Num);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_next4Num", CCLabelIF*, this->m_next4Num);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconList", CCNode*, this->m_iconList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unlockLable", CCLabelIF*, this->m_unlockLable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowNode", CCNode*, this->m_arrowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rArrow", CCSprite*, this->m_rArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lArrow", CCSprite*, this->m_lArrow);
    return false;
}

CCNode *BuildUpgradeView::getGuideNode(int itemId){
    if((m_buildId==itemId || m_buildId / 1000 == itemId) && m_upBtn->isEnabled()){
        return m_upBtn;
    }
    return NULL;
}

void BuildUpgradeView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
            YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::showWarningInstantUp)));
        }
        else {
            showWarningInstantUp();
        }
    }
    else {
        showWarningInstantUp();
    }
}
void BuildUpgradeView::showWarningInstantUp(){
    std::string name = "";
    
    if(m_pos <=0){
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
    }else{
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
    }
    std::string showString = _lang_1("102498",_lang(name).c_str());
    YesNoDialog::showButtonAndGold(showString.c_str(), CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::onOkInstantUp)),m_btnMsgLabel->getString().c_str(), _tmpGold);
}
void BuildUpgradeView::onOkInstantUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    
    for (int j = 1; j<willUnlockItems.size(); j++)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
    }
    CCUserDefault::sharedUserDefault()->flush();

    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(m_buildId,_tmpGold))
        {
            m_isReturn = false;
            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
        }
    }
    else {
        if(FunBuildController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _tmpGold))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
        }
    }
}

void BuildUpgradeView::onGotoSpdView()
{
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
    string key = GlobalData::shared()->allQueuesInfo[qid].key;
    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
    int gold = CCCommonUtils::getGoldByTime(tmpTime);
    if (tmpTime<=GlobalData::shared()->freeSpdT) {
        gold = 0;
    }
    FunBuildController::getInstance()->costCD(atoi(key.c_str()), "", gold);
}

void BuildUpgradeView::onYesRent()
{
    QueueController::getInstance()->rentQueue(m_qid, m_rentTime);
}

void BuildUpgradeView::onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, _tmpTime);
    if(QID_MAX == qid)
    {
        bool st = true;
        if (m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            if (qInfo.endTime!=0) {
                //可租用的队列
                int lastTime = qInfo.endTime - GlobalData::shared()->getWorldTime();
                if (lastTime < _tmpTime) {
                    lastTime = MAX(0, lastTime);
                    int needTime = _tmpTime - lastTime;
                    const int baseTime = FunBuildController::getInstance()->building_base_k8;
                    int needDay = ceil(needTime*1.0/86400);
                    int minDay = baseTime / 86400 ;
                    needDay = MAX(needDay, minDay);
                    int day = 0;
                    if(needDay%minDay!=0){
                        int tmp = floor(needDay*1.0/minDay);
                        day = tmp+1;
                        needDay = day * minDay;
                    }else{
                        day = needDay/minDay;
                    }
                    needTime = day * baseTime;
                    int needCost = day * FunBuildController::getInstance()->building_base_k9;
                    m_rentTime = needTime;
                    st = false;
                    std::string showTip = _lang_1("102297", CC_ITOA(needDay));
                    std::string t2 = "";
                    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
                    if(m_info.level > 0){
                        t2 = _lang_1("102502",_lang(m_info.name).c_str());
                    }else{
                        t2 = _lang_1("102503",_lang(m_info.name).c_str());
                    }
                    YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
                }
            }
        }
        
//        qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
//        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
//        YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::onGotoSpdView)), tmpTime, _lang("104903").c_str());
        
        if (st) {
            CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
        }
    }
    else {
        if (m_pos <=0) {
            auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
            if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
                YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::onOkUp)));
            }
            else {
                onOkUp();
            }
        }
        else {
            onOkUp();
        }
    }
}

void BuildUpgradeView::onOkUp()
{
    if (_resGold>0) {
        string _dialog = "102312";//升级提示
        string _btnDialog = "102104";
        if (m_pos > 0) {
            _dialog = "102313";//建造提示
            _btnDialog = "102126";
        }
//        YesNoDialog::showButtonAndGold( _lang_1(_dialog, CC_ITOA(_resGold)) , CCCallFunc::create(this, callfunc_selector(BuildUpgradeView::onLastUp)), _lang("").c_str(),_resGold);
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                   ,this,callfunc_selector(BuildUpgradeView::onLastUp)
                                                                                   , _lang(_dialog), _lang(_btnDialog)),false);
    }
    else {
        onLastUp();
    }
}

void BuildUpgradeView::onLastUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    for (int j = 1; j<willUnlockItems.size(); j++)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
    }
    CCUserDefault::sharedUserDefault()->flush();

    bool isForce = false;
    if (_resGold>0) {
        isForce = true;
    }
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(m_buildId, _resGold, isForce))
        {
            m_isReturn = false;
            PopupViewController::getInstance()->removeAllPopupView();
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->showBuildBtns(m_buildId);
        }
    }
    else {
        if(FunBuildController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _resGold, isForce))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}

void BuildUpgradeView::onShowNextUnlockItem()
{
//    return;
    int itemId = m_buildId;
    if (m_pos<=0) {
        itemId = m_buildId / 1000 ;
    }
    if (itemId<=0) {
        return;
    }
    string keys = "unlock_item";
    
    string msgInfo = CCCommonUtils::getPropById(CC_ITOA(itemId), keys);
//    if (itemId == FUN_BUILD_SCIENE) {
//        msgInfo = "1,700100,700200,730000|2,710000,711400,711500,730100,730200,730300,730400|3,720000|4,720100,720200,730500|5,700300,730600,730700,730800,730900|6,720300,720400,731000|7,700400,700500,710300|8,731100,731200,731300,731400|9,731500|10,700600,710500,720500,720600,720700|11,720800,731600|12,700700,710600|13,731700,731800,731900,732000|14,710400,732100|15,700800,711600,711700,720900|16,700900,701000,721000,732200,732300,732400,732500|17,732600|18,701200,701300,710700,732700,732800,732900,733000|19,721100,721200,733100|20,711000,733200|21,701400|22,701500,701600,711200,711300,721400,733300,733400,733500,733600|24,701700,733700|25,701800,721500,721600,721700,733800,733900,734000,734100|26,701900,711100|27,702000,702100,721800,734200|28,734300,734400,734500,734600|30,734700";
//    }
//    else if (itemId == FUN_BUILD_BARRACK1) {
//        msgInfo = "4,107001|7,107002|10,107003|13,107004|16,107005|19,107006|22,107007|26,107008|30,107009";
//    }
//    else if (itemId == FUN_BUILD_BARRACK2) {
//        msgInfo = "4,107101|7,107102|10,107103|13,107104|16,107105|19,107106|22,107107|26,107108|30,107109";
//    }
//    else if (itemId == FUN_BUILD_BARRACK3) {
//        msgInfo = "4,107201|7,107202|10,107203|13,107204|16,107205|19,107206|22,107207|26,107208|30,107209";
//    }
//    else if (itemId == FUN_BUILD_BARRACK4) {
//        msgInfo = "4,107301|7,107302|10,107303|13,107304|16,107305|19,107306|22,107307|26,107308|30,107309";
//    }
//    else if (itemId == FUN_BUILD_FORT) {
//        msgInfo = "2,107910|3,107920|6,107901|8,107911|10,107921|12,107902|14,107912|16,107922|18,107903|20,107913|22,107923|24,107904|27,107914|30,107924";
//    }
//    else if (itemId == FUN_BUILD_MAIN) {
//        msgInfo = "2,410000|3,425000|4,416000,403000,411000|5,426000,429000|6,401000,402000|7,407000|8,428000|10,414000|15,412000";
//    }
    
    if (msgInfo=="") {
        return;
    }
    
    int nextLv = 1;
    if (m_pos<=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        nextLv = m_info.level+1;
    }
    vector<string> msgItemVec;
    vector<string> finalVec;
    vector<string> nextVec;
    CCCommonUtils::splitString(msgInfo, "|", msgItemVec);
    for (int i=0; i<msgItemVec.size(); i++)
    {
        finalVec.clear();
        CCCommonUtils::splitString(msgItemVec[i], ",", finalVec);
        if (finalVec.size()>0 && finalVec[0] == CC_ITOA(nextLv)) {
            break;
        }
        
        if (atoi(finalVec[0].c_str()) > nextLv)
        {
            if (nextVec.size() == 0) {
                nextVec = finalVec;
            }
            else {
                if (atoi(nextVec[0].c_str()) > atoi(finalVec[0].c_str())) {
                    nextVec = finalVec;
                }
            }
        }
        
        if (i == msgItemVec.size()-1) {
            finalVec.clear();
        }
    }
    
    if (finalVec.size()<=0) {
        finalVec = nextVec;
        if (finalVec.size()>0) {
            m_unlockLable->setString(_lang_1("106048", finalVec[0].c_str() ));
        }
    }else {
        m_unlockLable->setString(_lang("106047"));
        willUnlockItems = finalVec;
    }
    
    if (finalVec.size()<=0) {
        return;
    }
    
    if (itemId == FUN_BUILD_SCIENE) {
        CCLoadSprite::doResourceByCommonIndex(5, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(5, false);
        });
    }
    else if (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2 || itemId == FUN_BUILD_BARRACK3 || itemId == FUN_BUILD_BARRACK4 || itemId ==FUN_BUILD_FORT) {
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
        });
    }
    else {
        CCLoadSprite::doResourceByCommonIndex(101, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(101, false);
        });
    }
    
    m_para1Node->setVisible(false);
    m_para2Node->setVisible(false);
    m_para3Node->setVisible(false);
    m_para4Node->setVisible(false);
    m_nextMsgLabel->setVisible(false);
    
    m_itemScrollView->getContainer()->removeAllChildren();
    
    int cellWidth = 180;
    if (CCCommonUtils::isIosAndroidPad())
    {
        cellWidth = 180 * 2.32;
    }
    int startX = 0;
    if (finalVec.size() == 2) {
        startX = (180-cellWidth)/2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            startX = 0;
        }
    }
    int posY = 10;
    for (int j = 1; j<finalVec.size(); j++)
    {
        auto unLockItemCell = UnLockItemCell::create(itemId, finalVec[j], m_iconList);
        unLockItemCell->setPosition(ccp((j-1)*cellWidth + startX, posY));
        m_itemScrollView->addChild(unLockItemCell);
    }
    if (finalVec.size()==2) {
        m_itemScrollView->setTouchEnabled(false);
    }else if (finalVec.size()>2) {
        m_arrowNode->setVisible(true);
        m_rArrow->setVisible(true);
        m_lArrow->setVisible(false);
    }
    m_unlockItemWidth = cellWidth;
    m_sumItemWidth = (finalVec.size()-1)*cellWidth;
    m_itemScrollView->setContentSize(CCSize((finalVec.size()-1)*cellWidth  + startX ,m_iconList->getContentSize().height));
}

void BuildUpgradeView::updateParasInfo()
{
    auto getAddStr = [](string curPara, string nextPara, bool isPersent, bool isAdd){
        string ret = "";
        if (isAdd) {
            ret = "+";
        }
        float add1 = atoi(nextPara.c_str()) - atoi(curPara.c_str());
        ret = ret+CC_ITOA(add1);
        if (isPersent) {
            ret = ret+"%";
        }
        return ret;
    };
    
    m_para1Node->setVisible(false);
    m_para2Node->setVisible(false);
    m_para3Node->setVisible(false);
    m_para4Node->setVisible(false);
    
    int tmpCnt = 0;
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_info.type == FUN_BUILD_WOOD || m_info.type == FUN_BUILD_FOOD || m_info.type == FUN_BUILD_IRON || m_info.type == FUN_BUILD_STONE) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=2 && m_info.para.size()>=2) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102122"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang_1("102123",""));
            m_para2Num->setString(m_info.para[2]);
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
            m_next2Num->setString(next2Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 2;
        }
    }
    else if (m_info.type == FUN_BUILD_MAIN) {
//        string nextParas = m_info.nextLevelParas;
//        vector<string> nextVec;
//        CCCommonUtils::splitString(nextParas, ",", nextVec);
//        if (nextVec.size()>=1 && m_info.para.size()>=1) {
//            m_para1Node->setVisible(true);
//            m_para1Name->setString(_lang("102206"));
//            m_para1Num->setString(m_info.para[1]);
//            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
//            m_next1Num->setString(next1Msg);
//            m_nextMsgLabel->setString("");
//            tmpCnt = 1;
//        }
    }
    else if (m_info.type == FUN_BUILD_BARRACK) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=2 && m_info.para.size()>=2) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102213"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang("102264"));
            m_para2Num->setString(m_info.para[2]+"%");
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], true, true);
            m_next2Num->setString(next2Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 2;
        }
    }
    else if (m_info.type == FUN_BUILD_HOSPITAL) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=1 && m_info.para.size()>=1) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102186"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 1;
        }
    }
    else if (m_info.type == FUN_BUILD_WALL) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=2 && m_info.para.size()>=2) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102185"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang("102206"));
            m_para2Num->setString(m_info.para[2]);
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
            m_next2Num->setString(next2Msg);
            
            m_nextMsgLabel->setString("");
            tmpCnt = 2;
        }
    }
    else if (m_info.type == FUN_BUILD_TRAINFIELD) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=1 && m_info.para.size()>=1) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102228"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 1;
        }
    }
    else if (m_info.type == FUN_BUILD_SMITHY) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=1 && m_info.para.size()>=1) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102311"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 1;
        }
    }
    else if (m_info.type == FUN_BUILD_STABLE) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=3 && m_info.para.size()>=3) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102218"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang("102221"));
            m_para2Num->setString(m_info.para[2]);
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
            m_next2Num->setString(next2Msg);
            
            m_para3Node->setVisible(true);
            m_para3Name->setString(_lang("102258"));
            m_para3Num->setString(m_info.para[3]);
            string next3Msg = getAddStr(m_info.para[3], nextVec[2], false, true);
            m_next3Num->setString(next3Msg);
            
            m_nextMsgLabel->setString("");
            tmpCnt = 3;
        }
    }
    else if (m_info.type == FUN_BUILD_TAVERN) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=2 && m_info.para.size()>=2) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102219"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang("102220"));
            m_para2Num->setString(m_info.para[2]);
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, false);
            m_next2Num->setString(next2Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 2;
        }
    }
    else if (m_info.type == FUN_BUILD_FORGE) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=2 && m_info.para.size()>=2) {
            m_para1Node->setVisible(true);
            m_para1Name->setString(_lang("102362"));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], true, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_para2Name->setString(_lang("102363"));
            m_para2Num->setString(m_info.para[2]);
            string next2Msg = getAddStr(m_info.para[2], nextVec[1], true, true);
            m_next2Num->setString(next2Msg);
            m_nextMsgLabel->setString("");
            tmpCnt = 2;
        }
    }
    else if (m_info.type == FUN_BUILD_CELLAR) {
        string nextParas = m_info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        if (nextVec.size()>=4 && m_info.para.size()>=4) {
            m_para1Node->setVisible(true);//wood
            m_para1Name->setString(CCCommonUtils::getResourceNameByType(Wood));
            m_para1Num->setString(m_info.para[1]);
            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
            m_next1Num->setString(next1Msg);
            
            m_para2Node->setVisible(true);//food
            m_para2Name->setString(CCCommonUtils::getResourceNameByType(Food));
            m_para2Num->setString(m_info.para[4]);
            string next2Msg = getAddStr(m_info.para[4], nextVec[3], false, true);
            m_next2Num->setString(next2Msg);
            tmpCnt = 2;
            
            int mlv = FunBuildController::getInstance()->getMainCityLv();
            if (mlv>=FunBuildController::getInstance()->building_base_k3) {
                m_para3Node->setVisible(true);//iron
                m_para3Name->setString(CCCommonUtils::getResourceNameByType(Iron));
                m_para3Num->setString(m_info.para[3]);
                string next3Msg = getAddStr(m_info.para[3], nextVec[2], false, true);
                m_next3Num->setString(next3Msg);
                tmpCnt++;
            }
            
            if (mlv>=FunBuildController::getInstance()->building_base_k4) {
                m_para4Node->setVisible(true);
                m_para4Name->setString(CCCommonUtils::getResourceNameByType(Stone));
                m_para4Num->setString(m_info.para[2]);
                string next4Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
                m_next4Num->setString(next4Msg);
                tmpCnt++;
            }
            m_nextMsgLabel->setString("");
        }
    }
    else if (m_info.type == FUN_BUILD_ARROW_TOWER) {
        vector<string> vecItems;
        CCCommonUtils::splitString(m_info.information, "|", vecItems);
        string curAtk = "";
        string nextAtk = "";
        string curSpd = "";
        string nextSpd = "";
        
        if (m_info.level < vecItems.size()) {
            string curMsg = vecItems[m_info.level];
            string nextMsg = vecItems[m_info.level+1];
            vector<string> cellItems;
            vector<string> cellNextItems;
            CCCommonUtils::splitString(curMsg, ";", cellItems);
            CCCommonUtils::splitString(nextMsg, ";", cellNextItems);
            if (cellItems.size()>=3 && cellNextItems.size()>=3) {
                string atkStr = cellItems[1];
                string spdStr = cellItems[2];
                string nextAtkStr = cellNextItems[1];
                string nextSpdStr = cellNextItems[2];
                
                vector<string> tinyItem1;
                CCCommonUtils::splitString(atkStr, ",", tinyItem1);
                curAtk = tinyItem1[1];
                vector<string> nexttinyItem1;
                CCCommonUtils::splitString(nextAtkStr, ",", nexttinyItem1);
                nextAtk = nexttinyItem1[1];
                
                vector<string> tinyItem2;
                CCCommonUtils::splitString(spdStr, ",", tinyItem2);
                curSpd = tinyItem2[1];
                vector<string> nexttinyItem2;
                CCCommonUtils::splitString(nextSpdStr, ",", nexttinyItem2);
                nextSpd = nexttinyItem2[1];
            }
        }
        
        m_para1Node->setVisible(true);
        m_para1Name->setString(_lang("102259"));
        m_para1Num->setString(curAtk);
        string next1Msg = getAddStr(curAtk, nextAtk, false, true);
        m_next1Num->setString(next1Msg);
        
        m_para2Node->setVisible(true);
        m_para2Name->setString(_lang("102260"));
        m_para2Num->setString(curSpd);
        string next2Msg = getAddStr(curSpd, nextSpd, true, true);
        m_next2Num->setString(next2Msg);
        m_nextMsgLabel->setString("");
        tmpCnt = 2;
    }
    
    if (tmpCnt==1) {
        m_para1Node->setPositionY(23);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_para1Node->setPositionY(70);
        }
    }
    else if (tmpCnt==2) {
        m_para1Node->setPositionY(45);
        m_para2Node->setPositionY(0);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_para1Node->setPositionY(120);
            m_para2Node->setPositionY(0);
        }
    }
    else if (tmpCnt==3) {
        m_para1Node->setPositionY(68);
        m_para2Node->setPositionY(23);
        m_para3Node->setPositionY(-22);
        m_para4Node->setPositionY(-22);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_para1Node->setPositionY(176);
            m_para2Node->setPositionY(42);
            m_para3Node->setPositionY(-92);
            m_para4Node->setPositionY(-92);
        }
    }
}

/////////////////////////

UpgradeCell* UpgradeCell::create(int type, int itemId, int value, bool isShow)
{
    auto ret = new UpgradeCell();
    if (ret && ret->init(type, itemId, value, isShow)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UpgradeCell::init(int type, int itemId, int value, bool isShow)
{
    CCBLoadFile("ResUpCell02",this,this);
    m_type = type;
    m_itemId = itemId;
    m_value = value;
    tmpNum = 0;
    m_isShow = isShow;
    refresh();
    bool ret = true;
    return ret;
}

void UpgradeCell::refresh()
{
    string tmpCellName = "";
    int tmpCellValue = 0;
    int tmpSumValue = 0;
    m_isOk = false;
    m_value2Label->setString("");
    
    string picName = "";
    if (m_type == 0) {//资源
        picName = CCCommonUtils::getResourceIconByType(m_itemId);
        tmpCellName = CCCommonUtils::getResourceNameByType(m_itemId);
        tmpCellValue = m_value;
        m_isOk = CCCommonUtils::isEnoughResourceByType(m_itemId, m_value);
        tmpSumValue = CCCommonUtils::getCurResourceByType(m_itemId);
    }
    else if (m_type == 1) {//道具
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        tmpCellName = toolInfo.getName();//_lang(toolInfo.name);
        tmpCellValue = m_value;
        picName = toolInfo.icon + ".png";
        tmpSumValue = toolInfo.getCNT();
        if (tmpSumValue >= tmpCellValue) {
            m_isOk = true;
        }
    }
    else if (m_type == 2) {//建筑
        int buildId = m_itemId;
        int lv = buildId%1000;
        buildId = buildId-lv;
        auto buildDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildId));
        string name = buildDict->valueForKey("name")->getCString();
        tmpCellName = _lang(name);
        tmpCellValue = lv;
        if (FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)) {
            m_isOk = true;
        }
        picName = buildDict->valueForKey("pic")->getCString();
        if(GlobalData::shared()->contryResType==0){
            picName = picName +".png";
        }
        else{
            picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        }
        if (m_isShow) {
            picName = "build_icon.png";
        }
        if(!m_isOk) {
            m_gotoBuildId = FunBuildController::getInstance()->getMaxLvBuildByType(buildId);
        }
    }
    else if (m_type == 3) {//没有空闲队列
        picName = "icon_time.png";
        string key = GlobalData::shared()->allQueuesInfo[m_value].key;
        int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
        tmpTime = GlobalData::shared()->allQueuesInfo[m_value].finishTime - GlobalData::shared()->getWorldTime();
        
        if (qType == TYPE_BUILDING) {
            auto& bInfo = FunBuildController::getInstance()->getFunbuildById(atoi(key.c_str()));
            if (bInfo.state == FUN_BUILD_UPING) {
                tmpCellName = _lang_1("102273", _lang(bInfo.name).c_str());
            }
            else if (bInfo.state == FUN_BUILD_CREATE) {
                tmpCellName = _lang_1("102274", _lang(bInfo.name).c_str());
            }
        }
        else if (qType == TYPE_SCIENCE) {
            int scId = QueueController::getInstance()->getItemId(key);
            string scName = CCCommonUtils::getNameById(CC_ITOA(scId));
            tmpCellName = _lang_1("102273", scName.c_str());
        }
        
        tmpCellValue = m_value;
        m_isOk = false;
    }
    else if (m_type == 4) {
        if (m_itemId == R_POWER) {
            picName = "icon_combat.png";
        }
        else if (m_itemId == R_EXP) {
            picName = "icon_exp.png";
        }
        tmpCellName = "";
        tmpCellValue = m_value;
        m_isOk = false;
    }
    else if (m_type == 5) {
        auto &scienceMap = GlobalData::shared()->scienceInfo.infoMap;
        picName = "book.png";
        tmpCellValue = m_value;
        
        string name = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
        tmpCellName = _lang_2("121989", name.c_str(), CC_ITOA(m_value));
        m_isOk = true;
        if (scienceMap.find(m_itemId) == scienceMap.end() || scienceMap[m_itemId].scienceLv < m_value ) {
            m_isOk = false;
        }
    }
    else if (m_type == 6) {//龙建筑
        int buildId = m_itemId;
        int lv = buildId%1000;
        buildId = buildId-lv;
        auto buildDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildId));
        string name = buildDict->valueForKey("name")->getCString();
        tmpCellName = _lang(name);
        tmpCellValue = lv;
        if (DragonBuildingController::getInstance()->isExistBuildByTypeLv(buildId, lv) || FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)) {
            m_isOk = true;
        }
        picName = buildDict->valueForKey("pic")->getCString();
//        if(GlobalData::shared()->contryResType==0){
//            picName = picName +".png";
//        }
//        else{
//            
//        }
        picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        if (m_isShow) {
            picName = "build_icon.png";
        }
        if(!m_isOk) {
            m_gotoBuildId = DragonBuildingController::getInstance()->getMaxLvBuildByType(buildId);
        }
    }
    else if (m_type == 7) {//没有空闲龙相关队列
        picName = "icon_time.png";
        string key = GlobalData::shared()->allQueuesInfo[m_value].key;
        int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
        tmpTime = GlobalData::shared()->allQueuesInfo[m_value].finishTime - GlobalData::shared()->getWorldTime();
        
        if(qType == TYPE_DRAGON_BUILD) {
            auto& bInfo = DragonBuildingController::getInstance()->getFunbuildById(atoi(key.c_str()));
            if (bInfo.state == FUN_BUILD_UPING) {
                tmpCellName = _lang_1("102273", _lang(bInfo.name).c_str());
            }
            else if (bInfo.state == FUN_BUILD_CREATE) {
                tmpCellName = _lang_1("102274", _lang(bInfo.name).c_str());
            }
        }
        tmpCellValue = m_value;
        m_isOk = false;
    }else if (m_type == 8) {//fusheng 泰坦喂食
        picName = "zy_shiwu.png";
        
        tmpCellName = CCString::createWithFormat("%d/%d",GlobalData::shared()->titanInfo.exp,GlobalData::shared()->titanInfo.nextExp)->getCString();
        
        if(GlobalData::shared()->titanInfo.exp<GlobalData::shared()->titanInfo.nextExp)
        {
            
            m_isOk = false;
        }
        else
        {
            m_isOk = true;
        }

        

    }

    
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if(pic)
    {
        if (m_type == 2 || m_type == 6) {
            if(m_isShow) {
                CCCommonUtils::setSpriteMaxSize(pic, 60, true);
            }
            else {
                CCCommonUtils::setSpriteMaxSize(pic, 40, true);
            }
        }
        else if (m_type == 5) {
            CCCommonUtils::setSpriteMaxSize(pic, 60, true);
        } else {
            CCCommonUtils::setSpriteMaxSize(pic, 40, true);
        }
        m_picNode->addChild(pic);
    }
    
    m_nameLabel->setColor(Color3B(196,207,255));
    m_valueLabel->setColor(Color3B(196,207,255));
    
    m_nameLabel->setString(tmpCellName.c_str());
    if (m_type == 2) {
        string tmpBuildName = tmpCellName + " " + _lang_1("102272", CC_CMDITOA(tmpCellValue).c_str());
        m_valueLabel->setString("");
        m_nameLabel->setString(tmpBuildName.c_str());
        m_nameLabel->setPositionX(m_nameLabel->getPositionX()+0);
    }
    else if (m_type == 6) {
        string tmpBuildName = tmpCellName + " " + _lang_1("102272", CC_CMDITOA(tmpCellValue).c_str());
        m_valueLabel->setString("");
        m_nameLabel->setString(tmpBuildName.c_str());
        m_nameLabel->setPositionX(m_nameLabel->getPositionX()+0);
    }
    else if (m_type == 3) {
        m_tmpName = tmpCellName;
        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        m_valueLabel->setString("");
        
    }
    else if (m_type == 7) {
        m_tmpName = tmpCellName;
        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        m_valueLabel->setString("");
    }
    else if (m_type == 0) {
        if(m_isShow) {
            
            if (!m_isOk) {
                m_nameLabel->setColor(Color3B(255,84,84));
                
                m_nameLabel->setFontSize(16);//fusheng 换字体大小
                
                m_valueLabel->setFontSize(16);
            }
            else
            {
                m_nameLabel->setFontSize(20);
                
                m_valueLabel->setFontSize(20);
            }
            
            m_nameLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            m_valueLabel->setString(CCString::createWithFormat("/%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            
            m_valueLabel->setPositionX(m_nameLabel->getPositionX()+m_nameLabel->getContentSize().width*m_nameLabel->getOriginScaleX());
        }
        else {
            m_nameLabel->setString("");
            m_valueLabel->setString(CCString::createWithFormat("%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            if (!m_isOk) {
                m_valueLabel->setColor(Color3B(255,84,84));
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX());
        }
    }
    else if (m_type == 4) {
        m_valueLabel->setString("");
        m_nameLabel->setString(CCString::createWithFormat("+%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
    }
    else if (m_type == 5) {
        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_valueLabel->setString("");
    }
    else if (m_type == 8) {//fusheng 泰坦
//        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        if (!m_isOk) {
            m_nameLabel->setColor(Color3B(255,84,84));
       
        }
        else
        {
            m_nameLabel->setColor(Color3B(196,207,255));

        }

        m_valueLabel->setString("");
    }
    else {
        m_valueLabel->setString(CC_CMDITOA(tmpCellValue).c_str());
        if (m_type==1) {
            m_valueLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            m_value2Label->setString(CCString::createWithFormat("/%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            m_value2Label->setPositionX(m_valueLabel->getPositionX()+m_valueLabel->getContentSize().width*m_valueLabel->getOriginScaleX());
        }
    }
    
    m_btn->setVisible(false);
    m_btnLabel->setString("");
    if (m_isOk) {
        m_yesSprite->setVisible(true);
        m_noSprite->setVisible(false);
        
        if ((m_type == 3||m_type == 0) && m_isShow) {
            m_moveNode->setPositionX(0);
        }
    }
    else {
        m_yesSprite->setVisible(false);
        m_noSprite->setVisible(true);
        if (m_type == 0) {
//            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("102153"));
            if (m_isShow) {//fusheng 不显示按钮
//                m_btn->setVisible(false);
//                m_btnLabel->setString("");
                if (!m_isOk) {
                    m_moveNode->setPositionX(-90);
                }
            }
        }
        else if (m_type == 1) {
//            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("102153"));
        }
        else if (m_type == 2 && m_gotoBuildId > 0) {
//            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("103658"));
            if (m_isShow) {
                m_btn->setVisible(false);
                m_btnLabel->setString("");
            }
        }
        else if (m_type == 3) {
//            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("104903"));
            
            if (m_isShow) {
                m_moveNode->setPositionX(-90);
            }
        }
        else if (m_type == 7) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("104903"));
        }
        else if (m_type == 6 && m_gotoBuildId > 0) {
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("103658"));
            if (m_isShow) {
                m_btn->setVisible(false);
                m_btnLabel->setString("");
            }
        }
        else if (m_type == 8) {
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("500002"));
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            float ls = m_btnLabel->getScaleX();
            float fw = getContentSize().width;
            if (fw>240) {//防爆框
                m_btnLabel->setScaleX(240/fw * ls);
            }
        } else {
            m_btnLabel->setMaxScaleXByWidth(100);
        }
        
        if(m_type == 1 || m_type == 2 || m_type == 3 || m_type == 5 || m_type == 6 || m_type == 7) {
            m_nameLabel->setColor(Color3B(255,84,84));
            m_valueLabel->setColor(Color3B(255,84,84));
        }
    }
    
    if (m_type == 4) {
        m_yesSprite->setVisible(false);
        m_noSprite->setVisible(false);
    }
}

void UpgradeCell::setTouchNode(CCNode* node, int buildId, int parentType)
{
    m_touchNode = node;
    m_buildId = buildId;
    m_parentType = parentType;
    
}

void UpgradeCell::setCreateBuildInfo(int buildId, int pos)
{
    m_createInfo = CC_ITOA(buildId);
    m_createInfo = m_createInfo+"|"+CC_ITOA(pos);
}

void UpgradeCell::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UpgradeCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UpgradeCell::rectCallBack), MSG_BUILD_CELL, NULL);
    if (tmpNum>0) {
        refresh();
    }
    tmpNum++;
}

void UpgradeCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UpgradeCell::onEnterFrame), this);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUILD_CELL);
    CCNode::onExit();
}

void UpgradeCell::onEnterFrame(float dt)
{
    if (m_type == 0) {
        bool t_isOk = CCCommonUtils::isEnoughResourceByType(m_itemId, m_value);
        int tmpSumValue = CCCommonUtils::getCurResourceByType(m_itemId);
        if (m_isShow) {
            m_nameLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            if (!t_isOk) {
                m_nameLabel->setColor(Color3B(255,84,84));
            }
            else {
                m_nameLabel->setColor(Color3B(196,207,255));
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX()+m_nameLabel->getContentSize().width*m_nameLabel->getOriginScaleX());
        }
        else {
            m_nameLabel->setString("");
            if (!t_isOk) {
                m_valueLabel->setColor(Color3B(255,84,84));
            }
            else {
                m_valueLabel->setColor(Color3B(196,207,255));
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX());
        }
        if (m_isOk ^ t_isOk) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_BUILD_UPGRADE);
        }
        m_isOk = t_isOk;
    }
    
    if(m_type == 3) {
        tmpTime--;
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        if (tmpTime<=0) {
            this->removeFromParent();
        }
    }
    if(m_type == 7) {
        tmpTime--;
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        if (tmpTime<=0) {
            this->removeFromParent();
        }
    }
}

bool UpgradeCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_btn->isVisible() && (m_type==0||m_type==1||m_type==2||m_type==3 || m_type == 6 || m_type == 7 || m_type == 8) && m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_touchBtn,pTouch)) {
        m_btn->setScale(1.1);
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        return true;
    }
    return false;
}

void UpgradeCell::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_btn->setScale(1.0);
}

void UpgradeCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_btn->setScale(1.0);
    if (1) {//m_btn->isVisible() && (m_type==0||m_type==1||m_type==2||m_type==3) && m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_touchBtn,pTouch)
        //        m_touchNode->setVisible(false);
        if (m_type == 1) {
            PopupViewController::getInstance()->addPopupInView(BatchBuyItemView::create(m_itemId, m_value));
        }
        else if (m_type == 2 && m_gotoBuildId>0) {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
                if(m_gotoBuildId == FUN_BUILD_MAIN_CITY_ID)
                {
                    
                    PopupViewController::getInstance()->removeAllPopupView();
                    PopupViewController::getInstance()->addPopupInView(GeneralTitanPopupView::create());//fusheng 点击龙 goto 打开龙界面
                }
                else
                {
                    layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
                    PopupViewController::getInstance()->removeAllPopupView();
                }
                
            }
            else
            {
                if (m_buildId == FUN_BUILD_MAIN_CITY_ID) {//fusheng 从world里 点击goto按钮
//                    UIComponent::getInstance()->OnHomeBackBtnClick(NULL, CCControlEvent::TOUCH_DOWN);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        CCCommonUtils::setIsHDViewPort(true);
                    }
                    //    SoundController::sharedSound()->playEffects(Music_ui_world);
                    
                    PopupViewController::getInstance()->removeAllPopupView();
                    
                    auto world = WorldMapView::instance();
                    
                    if (world) {
                        
                        world->leaveWorld();
                        
                    }
                    
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                           
                                                                                           , CCString::createWithFormat("UI_world_back"));
                    FunBuildController::getInstance()->willMoveToBuildItemID = m_gotoBuildId;
                }
            }
            
            
        }
        else if (m_type == 6 && m_gotoBuildId>0) {
            auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
                layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
                PopupViewController::getInstance()->removeAllPopupView();
            }
        }
        else if (m_type == 3) {
            if (m_parentType==0) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(UpgradeCell::spdCallBack)), tmpTime, _lang("104903").c_str());
            }
            else if (m_parentType==1) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("121986").c_str() , CCCallFunc::create(this, callfunc_selector(UpgradeCell::spdCallBack)), tmpTime, _lang("104903").c_str(), false);
            }
        }
        else if (m_type == 7) {
            int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
            if (qType == TYPE_DRAGON_BUILD) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(UpgradeCell::spdCallBackDragon)), tmpTime, _lang("104903").c_str());
            }
        }
        else if (m_type == 8) {
            
//            PopupViewController::getInstance()->goBackPopupView();
            
            auto view = PopupViewController::getInstance()->getCurrentPopupView();
            
            GeneralTitanPopupView*  generalTitanPopupView = dynamic_cast<GeneralTitanPopupView*>(view);
            
            if (generalTitanPopupView) {
                
                generalTitanPopupView->setGuideFeed(true);
            }
            
            
        }
        else {
            if(m_buildId!=0){
                if (m_parentType == 0) {
                    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, CC_ITOA(m_buildId), RES_BUILD_UP_VIEW));
                }
                else if (m_parentType == 1) {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CLOSE);
                    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, CC_ITOA(m_buildId), RES_SCIENCE_VIEW));
                }
            }
            else {
                PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, m_createInfo, RES_BUILD_CREATE_VIEW));
            }
        }
    }
}

SEL_CCControlHandler UpgradeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", BuildCell::onCreateOrUpClick);
    return NULL;
}

bool UpgradeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_value2Label", CCLabelIF*, m_value2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yesSprite", CCSprite*, m_yesSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noSprite", CCSprite*, m_noSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCScale9Sprite*, m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn", CCNode*, m_touchBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, m_moveNode);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rectPic", CCScale9Sprite*, m_rectPic);
    return false;
}

void UpgradeCell::spdCallBackDragon()
{
    int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
    if (qType == TYPE_DRAGON_BUILD) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        int gold = CCCommonUtils::getGoldByTime(tmpTime);
        if (tmpTime<=GlobalData::shared()->freeSpdT) {
            gold = 0;
        }
        if (DragonBuildingController::getInstance()->costCD(atoi(key.c_str()), "", gold))
        {
            this->removeFromParent();
        }
    }
}

void UpgradeCell::spdCallBack()
{
    if (m_parentType == 0) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        int gold = CCCommonUtils::getGoldByTime(tmpTime);
        if (tmpTime<=GlobalData::shared()->freeSpdT) {
            gold = 0;
        }
        if(FunBuildController::getInstance()->costCD(atoi(key.c_str()), "", gold))
        {
            this->removeFromParent();
        }
    }
    else if (m_parentType == 1) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        int gold = CCCommonUtils::getGoldByTime(tmpTime);
        if( gold <= GlobalData::shared()->playerInfo.gold ){
            QueueController::getInstance()->startCCDQueue(qid, "", false, gold,"",1);
            this->removeFromParent();
        }
        else {
            YesNoDialog::gotoPayTips();
        }
    }
}

void UpgradeCell::rectCallBack(CCObject* obj)
{
    if(m_type==3 || m_type == 7) {
        m_rectPic->setVisible(true);
        CCActionInterval* fadeout = CCFadeOut::create(0.4);
        CCActionInterval* fadein = CCFadeIn::create(0.4);
        CCSequence* seq = CCSequence::create(fadein, fadeout, NULL);
        m_rectPic->runAction(CCRepeat::create(seq, 2));
    }
}

UpgradeTitleCell* UpgradeTitleCell::create(string msg)
{
    auto ret = new UpgradeTitleCell();
    if (ret && ret->init(msg)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UpgradeTitleCell::init(string msg)
{
    CCBLoadFile("ResUpTitleCell02",this,this);
    m_titleLabel->setString(msg.c_str());
    bool ret = true;
    return ret;
}

void UpgradeTitleCell::onEnter() {
    CCNode::onEnter();
}

void UpgradeTitleCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler UpgradeTitleCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool UpgradeTitleCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    return false;
}

/////////////////////////

UnLockItemCell* UnLockItemCell::create(int type, string itemId, CCNode* node)
{
    auto ret = new UnLockItemCell();
    if (ret && ret->init(type, itemId, node)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UnLockItemCell::init(int type, string itemId, CCNode* node)
{
    int sizeWidth = 180;
    int sizeHeight = 120;
    if (CCCommonUtils::isIosAndroidPad())
    {
        sizeWidth = 410;
        sizeHeight = 273;
    }
    setContentSize(CCSize(sizeWidth, sizeHeight));
    m_type = type;
    m_itemId = itemId;
    m_touchNode = node;
    
    m_iconPath = "";
    CCSprite* bg = nullptr;//fusheng 背景
    if (m_type == FUN_BUILD_MAIN) {
        m_iconPath = CCCommonUtils::getPropById(m_itemId, "pic");
        m_iconPath = m_iconPath+"_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        if (atoi(m_itemId.c_str()) == FUN_BUILD_STABLE) {
            m_iconPath = "quest_Embassy_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_BARRACK1) {
            m_iconPath = "quest_Barrack_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_FORGE) {
            m_iconPath = "quest_Blacksmith_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_BARRACK4) {
            m_iconPath = "quest_Chariot Plant_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_SCIENE) {
            m_iconPath = "quest_College_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_CELLAR) {
            m_iconPath = "quest_Depot_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_TRAINFIELD) {
            m_iconPath = "quest_Drill Grounds_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_FORT) {
            m_iconPath = "quest_Fortress_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_SMITHY) {
            m_iconPath = "quest_Hall of War_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_HOSPITAL) {
            m_iconPath = "quest_Hospital_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_IRON) {
            m_iconPath = "quest_Iron Mine_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_TAVERN) {
            m_iconPath = "quest_Market_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_BARRACK) {
            m_iconPath = "quest_Military Tent_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_STONE) {
            m_iconPath = "quest_Mithril Mine_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_BARRACK3) {
            m_iconPath = "quest_Range_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_BARRACK2) {
            m_iconPath = "quest_Stable_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_ARROW_TOWER) {
            m_iconPath = "quest_Turret_icon1.png";
        }else if (atoi(m_itemId.c_str()) == FUN_BUILD_SACRIFICE) {
            m_iconPath = "quest_Wishing well_icon1.png";
        }
    }
//    else if (m_type == FUN_BUILD_BARRACK1 || m_type == FUN_BUILD_BARRACK2 || m_type == FUN_BUILD_BARRACK3 || m_type == FUN_BUILD_BARRACK4) {
    else if (m_type == FUN_BUILD_BARRACK1 || m_type == FUN_BUILD_BARRACK2 || m_type == FUN_BUILD_BARRACK3 || m_type == FUN_BUILD_BARRACK4 ) {
        auto& aInfo = GlobalData::shared()->armyList[m_itemId];
        m_iconPath = aInfo.getHeadIcon();
        
        bg = CCLoadSprite::createSprite("zb_bp_xuanzhong.png");
        bg->setPosition(ccp(sizeWidth/2, sizeHeight/2));
//        CCCommonUtils::setSpriteMaxSize(bg, 125, true);
//        if (CCCommonUtils::isIosAndroidPad()) {
////            CCCommonUtils::setSpriteMaxSize(bg, 273, true);
//        }
        this->addChild(bg);
    }
    else if (m_type == FUN_BUILD_FORT)
    {
        m_iconPath = CCCommonUtils::getPropById(m_itemId, "icon").append("_small.png");
        
        bg = CCLoadSprite::createSprite("zb_bp_xuanzhong.png");
        bg->setPosition(ccp(sizeWidth/2, sizeHeight/2));
//        CCCommonUtils::setSpriteMaxSize(bg, 125, true);
//        if (CCCommonUtils::isIosAndroidPad()) {
////            CCCommonUtils::setSpriteMaxSize(bg, 273, true);
//        }
        this->addChild(bg);

    }
    else {
        m_iconPath = CCCommonUtils::getIcon(m_itemId);
    }
    auto icon = CCLoadSprite::createSprite(m_iconPath.c_str());
    CCCommonUtils::setSpriteMaxSize(icon, 120, true);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 273, true);
    }
    icon->setPosition(ccp(sizeWidth/2, sizeHeight/2));
    this->addChild(icon);
    
    if(bg)
    {
        bg->setScale(icon->getScale());
    }
    
    return true;
}

void UnLockItemCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void UnLockItemCell::onExit() {
     setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool UnLockItemCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(this,pTouch)) {
        startX = pTouch->getLocation().x;
        return true;
    }
    return false;
}

void UnLockItemCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int curX = pTouch->getLocation().x;
    if (abs(curX - startX) > 10) {
        return;
    }
    string name = CCCommonUtils::getNameById(m_itemId);
    string des = _lang( CCCommonUtils::getPropById(m_itemId, "description") );
    PopupViewController::getInstance()->addPopupView(UnlockItemInfoView::create(name, des, m_iconPath));
    CCLOG("show item info");
}

///ItemInfoShow
UnlockItemInfoView* UnlockItemInfoView::create(string name, string des, string iconPath)
{
    auto ret = new UnlockItemInfoView();
    if (ret && ret->init(name, des, iconPath)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UnlockItemInfoView::init(string name, string des, string iconPath)
{
    bool ret = false;
    if (!PopupBaseView::init()) {
        return ret;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    CCBLoadFile("qiandaoTips", this, this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    auto picSpr = CCLoadSprite::createSprite(iconPath.c_str());
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setSpriteMaxSize(picSpr, 130*2, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(picSpr, 130, true);
    m_picNode->addChild(picSpr);
    m_nameLabel->setString(name);
    m_desLabel->setString(des);
    
    auto resSp = CCLoadSprite::loadResource("UI-tishikuang2.png");
    auto batchNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
    m_bcNode->addChild(batchNode);
    
    int limitHight = 30;
    int desHeight = m_desLabel->getContentSize().height*m_desLabel->getOriginScaleY();
    if (desHeight > limitHight) {
        int extH = 0;
        int i=0;
        while (desHeight > limitHight+extH)
        {
            auto sp1 = CCLoadSprite::createSprite("UI-tishikuang2.png");
            sp1->setAnchorPoint(ccp(1.0, 1.0));
            sp1->setPosition(ccp(1, -55-i*35));
            auto sp2 = CCLoadSprite::createSprite("UI-tishikuang2.png");
            sp2->setAnchorPoint(ccp(0, 1.0));
            sp2->setPosition(ccp(1, -55-i*35));
            
            batchNode->addChild(sp1);
            batchNode->addChild(sp2);
            extH = i*35;
            i++;
        }
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            batchNode->setScaleX(2.f);
//        }
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
        m_mainNode->setPositionY(m_mainNode->getPositionY()+extH/2);
    }
    
    ret = true;
    return ret;
}

void UnlockItemInfoView::onEnter()
{
    PopupBaseView::onEnter();
    setSwallowsTouches(false);
    setTouchEnabled(true);
}

void UnlockItemInfoView::onExit()
{
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool UnlockItemInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void UnlockItemInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler UnlockItemInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool UnlockItemInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bcNode", CCNode*, m_bcNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    return false;
}