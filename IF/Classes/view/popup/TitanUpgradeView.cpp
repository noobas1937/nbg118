//
//  TitanUpgradeView.cpp
//  IF
//
//  Created by fusheng on 15-9-17.
//
//

#include "TitanUpgradeView.h"

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
#include "TitanInView.h"


#define MSG_BUILD_CELL "msg_build_cell"

#include "NBCommonUtils.h"
using namespace NBCommonUtils;

TitanUpgradeView* TitanUpgradeView::create(int buildId,int titanId, int pos){
    TitanUpgradeView* ret = new TitanUpgradeView();
    if(ret && ret->init(buildId, pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TitanUpgradeView::init(int buildId, int pos)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) //fusheng 在世界场景里 加载Imperial_22.plist
    {
        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,22, true);
    }
    setIsHDPanel(true);
    m_isReturn = true;
    m_buildId = buildId;
    m_pos = pos;
    auto tmpCCB = CCBLoadFile("UpgradeTitan",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->m_BackGroud->setContentSize(winSize);
    //    int addHeight = getExtendHeight();
 
    //    int newBgHeight = addHeight/2+oldBgHeight;

    //    int oldWidth = m_infoList->getContentSize().width;
    //    int oldHeight = m_infoList->getContentSize().height;
    //    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight/2);
    //    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight/2));
    //    m_btnNode->setPositionY(m_btnNode->getPositionY()-addHeight/2);
    
    m_upBtnMsgLabel->setString(_lang("102104").c_str());
    m_btnMsgLabel->setString(_lang("102127").c_str());
    m_inBtnGoldNum->setString("1");

    
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
    
//    updateInfo(NULL);
    updateTitanInfo(nullptr);
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
    m_titanPosInView->addChild( TitanInView::create());
    

    return true;
}

void TitanUpgradeView::updateTitanInfo(CCObject* obj)
{
    if(GlobalData::shared()->titanInfo.level<GlobalData::shared()->MaxBuildLv) {//fusheng 没有满级
        ArmyInfo titanInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId]; //fusheng map中通过id获得泰坦属性 泰坦满记属性
        ArmyInfo nextTitanInfo = GlobalData::shared()->armyList[ CC_ITOA(CCString::create(GlobalData::shared()->titanInfo.titanId)->intValue()+1)];
        m_titanCurrentZDL->setString(CC_ITOA(titanInfo.power));
        m_titanCurrentZDLInNext->setString(CC_ITOA(nextTitanInfo.power));
        m_titanZDLAddValue->setString(CCString::createWithFormat("+%.1f",nextTitanInfo.power-titanInfo.power)->getCString());
        
        nextLevelNode->setVisible(true);
        
        currentLevelNode->setPosition(70.0,64);
    }
    else
    {
        ArmyInfo titanInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
        m_titanCurrentZDL->setString(CC_ITOA(titanInfo.power));
        
        nextLevelNode->setVisible(false);
        
        currentLevelNode->setPosition(70.0,204);
    }
    ;
    updateInfo(nullptr);
    
}

void TitanUpgradeView::updateInfo(CCObject* obj)
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

//    string title = _lang_1("102272", CC_ITOA(level));
    string title = _lang_1("102272", " ");
    if(level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabelPre->setString(title);
    
    m_lvLabel->setString(CC_ITOA(level));
    vector<cocos2d::CCLabelIF *> labels;
    labels.push_back(m_lvLabelPre);
    labels.push_back(m_lvLabel);
    arrangeLabel(labels);
    
//    string nextLv = _lang_1("102272", );
    m_nextLvLabelPre->setString(title);
    m_nextLvLabel->setString(CC_ITOA(level+1));

    labels.clear();
    labels.push_back(m_nextLvLabelPre);
    labels.push_back(m_nextLvLabel);
    arrangeLabel(labels);
    
    float tmpEffect = CCCommonUtils::getEffectValueByNum(68);
    _tmpTime = time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    _resGold = 0;
    
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());

    
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
    
    //fusheng 泰坦喂食状态
    auto item_titanStatus = UpgradeCell::create(8, 0, 0);
    item_titanStatus->setPosition(ccp(curX, curY));
    item_titanStatus->setTouchNode(m_infoList,m_buildId);
    m_scrollView->addChild(item_titanStatus);
    curY += _itemH;

    
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
    
    //fusheng 添加喂食度判断
    if(GlobalData::shared()->titanInfo.exp<GlobalData::shared()->titanInfo.nextExp)
    {
        m_instantBtn->setEnabled(false);
        m_upBtn->setEnabled(false);
    }
    else
    {
        
    }
    
    
}

void TitanUpgradeView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo(NULL);
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanUpgradeView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanUpgradeView::updateTitanInfo), MSG_TITAN_INFORMATION_RESET, NULL);
    setTouchEnabled(true);
    int sid = SceneController::getInstance()->currentSceneId;
    
   
    
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void TitanUpgradeView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_INFORMATION_RESET);
    
    setTouchEnabled(false);
    
//    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
//    {
//        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,22, true);
//    }
    CCNode::onExit();
}

bool TitanUpgradeView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_x = m_itemScrollView->getContentOffset().x;
    return true;
}
void TitanUpgradeView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
void TitanUpgradeView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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

SEL_CCControlHandler TitanUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", TitanUpgradeView::onCreateOrUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", TitanUpgradeView::onInstantClick);
    return NULL;
}

bool TitanUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanCurrentZDL", CCLabelIF*, this->m_titanCurrentZDL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanCurrentZDLInNext", CCLabelIF*, this->m_titanCurrentZDLInNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanZDLAddValue", CCLabelIF*, this->m_titanZDLAddValue);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanPosInView", CCNode*, this->m_titanPosInView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnNode", CCNode*, this->m_inBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "currentLevelNode", CCNode*, this->currentLevelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "nextLevelNode", CCNode*, this->nextLevelNode);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabelPre", CCLabelIF*, this->m_lvLabelPre);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, this->m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabelPre", CCLabelIF*, this->m_nextLvLabelPre);
   
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    

    

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconList", CCNode*, this->m_iconList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unlockLable", CCLabelIF*, this->m_unlockLable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowNode", CCNode*, this->m_arrowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rArrow", CCSprite*, this->m_rArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lArrow", CCSprite*, this->m_lArrow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BackGroud", CCScale9Sprite*, this->m_BackGroud);
    return false;
}

CCNode *TitanUpgradeView::getGuideNode(string _key){
    if(_key=="TitanUp_End" && m_upBtn->isEnabled()){
        return m_upBtn;
    }
    return NULL;
}

void TitanUpgradeView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
            YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::showWarningInstantUp)));
        }
        else {
            showWarningInstantUp();
        }
    }
    else {
        showWarningInstantUp();
    }
}
void TitanUpgradeView::showWarningInstantUp(){
    std::string name = "";
    
    if(m_pos <=0){
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
    }else{
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
    }
//    std::string showString = _lang_1("102498",_lang(name).c_str());
    std::string showString = _lang("500008");
    
    YesNoDialog::showButtonAndGold(showString.c_str(), CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onOkInstantUp)),m_btnMsgLabel->getString().c_str(), _tmpGold);
}
void TitanUpgradeView::onOkInstantUp()
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
            
            
            //fusheng 如果在世界里的话  更新主城数据
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
            {
                auto biter = GlobalData::shared()->imperialInfo.find(400000000);
                
                if (biter!=GlobalData::shared()->imperialInfo.end()) {
                    
                    FunBuildController::getInstance()->completeUpOrCreate(400000000);//fusheng 主城数据刷新
                    biter->second.state = FUN_BUILD_UPING_END;//fusheng  设置状态 保证正常逻辑
                }
            }
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

void TitanUpgradeView::onGotoSpdView()
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

void TitanUpgradeView::onYesRent()
{
    QueueController::getInstance()->rentQueue(m_qid, m_rentTime);
}

void TitanUpgradeView::onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
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
                    YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
                }
            }
        }
        
        //        qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        //        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        //        YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onGotoSpdView)), tmpTime, _lang("104903").c_str());
        
        if (st) {
            CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
        }
    }
    else {
        if (m_pos <=0) {
            auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
            if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
                YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onOkUp)));
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

void TitanUpgradeView::onOkUp()
{
    if (_resGold>0) {
        string _dialog = "102312";//升级提示
        string _btnDialog = "102104";
        if (m_pos > 0) {
            _dialog = "102313";//建造提示
            _btnDialog = "102126";
        }
        //        YesNoDialog::showButtonAndGold( _lang_1(_dialog, CC_ITOA(_resGold)) , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onLastUp)), _lang("").c_str(),_resGold);
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                   ,this,callfunc_selector(TitanUpgradeView::onLastUp)
                                                                                   , _lang(_dialog), _lang(_btnDialog)),false);
    }
    else {
        onLastUp();
    }
}

void TitanUpgradeView::onLastUp()
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
//            PopupViewController::getInstance()->removeAllPopupView();
//            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//            if(layer)
//                layer->showBuildBtns(m_buildId);
            
            if(isForce)//fusheng 如果是强制建立  退出所有弹出窗
                PopupViewController::getInstance()->removeAllPopupView();
            else
                PopupViewController::getInstance()->goBackPopupView();
        }
    }
    else {
        if(FunBuildController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _resGold, isForce))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if(layer)
                layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("TitanUp_End"));
}

void TitanUpgradeView::onShowNextUnlockItem()
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
    else if (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2 || itemId == FUN_BUILD_BARRACK3 || itemId == FUN_BUILD_BARRACK4) {
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

void TitanUpgradeView::updateParasInfo()
{
}
