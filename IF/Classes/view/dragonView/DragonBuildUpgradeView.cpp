//
//  DragonBuildUpgradeView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuildUpgradeView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
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
#include "BatchBuyItemView.h"
#include "ConfirmDialogView.h"
#include "SoundController.h"
#include "CCFlagWaveSprite.h"
#include "DragonBuildingController.h"
#include "DragonScene.h"

#define MSG_BUILD_CELL "msg_build_cell"

DragonBuildUpgradeView* DragonBuildUpgradeView::create(int buildId, int pos){
    DragonBuildUpgradeView* ret = new DragonBuildUpgradeView();
    if(ret && ret->init(buildId, pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonBuildUpgradeView::init(int buildId, int pos)
{
    if (!ArcPopupBaseView::init(TYPE_POS_TO_UP)) {
        return false;
    }
    
    m_isReturn = true;
    m_buildId = buildId;
    m_pos = pos;
    auto tmpCCB = CCBLoadFile("Upgrade",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
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
    
    updateInfo(NULL);
    m_openNum=0;
    
    vector<CCNode*> tmpVec;
    CCCommonUtils::getRGBAProtocols(m_mainNode, &tmpVec);
    for (int i=0; i<tmpVec.size(); i++) {
        CCActionInterval * fadeIn = CCFadeIn::create(0.5);
        tmpVec[i]->runAction(fadeIn);
    }
    
    return true;
}

void DragonBuildUpgradeView::updateInfo(CCObject* obj)
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
        auto& m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
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
    
    if (m_pos>0) {
        m_lvLabel->setString("");
        m_nextLvLabel->setString("");
        m_nextLabel->setString("");
        m_nameLabel->setPositionY(m_nameLabel->getPositionY()+10);
    }
    
    _tmpTime = time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    _resGold = 0;
    
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());
    m_nextMsgLabel->setString(_lang(destip));
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    
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
            auto itemBuild = UpgradeCell::create(6, paramBuild, 0);
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
    
    //没有龙队列，显示条件不足
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD, 1);//_tmpTime, 取空闲队列，并不判断有效期是否够用
    if(QID_MAX == qid)
    {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
        auto itemQueue = UpgradeCell::create(7, Food, qid);
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
    
    if (m_pos<=0) {
        updateParasInfo();
    }
}

void DragonBuildUpgradeView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo(NULL);
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuildUpgradeView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
}

void DragonBuildUpgradeView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    CCNode::onExit();
}

SEL_CCControlHandler DragonBuildUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", DragonBuildUpgradeView::onCreateOrUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", DragonBuildUpgradeView::onInstantClick);
    return NULL;
}

bool DragonBuildUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
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
    return false;
}

CCNode *DragonBuildUpgradeView::getGuideNode(int itemId){
    if((m_buildId==itemId || m_buildId / 1000 == itemId) && m_upBtn->isEnabled()){
        return m_upBtn;
    }
    return NULL;
}

void DragonBuildUpgradeView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_pos <=0) {
        auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.type == DBUILDING_HATCHER && m_info.level==(GlobalData::shared()->shield_base-1)) {
            YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(DragonBuildUpgradeView::showWarningInstantUp)));
        }
        else {
            showWarningInstantUp();
        }
    }
    else {
        showWarningInstantUp();
    }
}
void DragonBuildUpgradeView::showWarningInstantUp(){
    std::string name = "";
    
    if(m_pos <=0){
        auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
    }else{
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
    }
    std::string showString = _lang_1("102498",_lang(name).c_str());
    YesNoDialog::showButtonAndGold(showString.c_str(), CCCallFunc::create(this, callfunc_selector(DragonBuildUpgradeView::onOkInstantUp)),m_btnMsgLabel->getString().c_str(), _tmpGold);
}
void DragonBuildUpgradeView::onOkInstantUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    
    if (m_pos <=0) {
        auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.state == FUN_BUILD_NORMAL && DragonBuildingController::getInstance()->startUpFunBuild(m_buildId,_tmpGold))
        {
            m_isReturn = false;
            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
        }
    }
    else {
        if(DragonBuildingController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _tmpGold))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
        }
    }
}

void DragonBuildUpgradeView::onGotoSpdView()
{
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
    string key = GlobalData::shared()->allQueuesInfo[qid].key;
    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
    int gold = CCCommonUtils::getGoldByTime(tmpTime);
    if (tmpTime<=GlobalData::shared()->freeSpdT) {
        gold = 0;
    }
    DragonBuildingController::getInstance()->costCD(atoi(key.c_str()), "", gold);
}

void DragonBuildUpgradeView::onYesRent()
{
    QueueController::getInstance()->rentQueue(m_qid, m_rentTime);
}

void DragonBuildUpgradeView::onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD, _tmpTime);
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
                    const int baseTime = DragonBuildingController::getInstance()->building_base_k8;
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
                    int needCost = day * DragonBuildingController::getInstance()->building_base_k9;
                    m_rentTime = needTime;
                    st = false;
                    std::string showTip = _lang_1("102297", CC_ITOA(needDay));
                    std::string t2 = "";
                    auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
                    if(m_info.level > 0){
                        t2 = _lang_1("102502",_lang(m_info.name).c_str());
                    }else{
                        t2 = _lang_1("102503",_lang(m_info.name).c_str());
                    }
                    YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(DragonBuildUpgradeView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
                }
            }
        }
        
        if (st) {
            CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
        }
    }
    else {
        if (m_pos <=0) {
            auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
            if (m_info.type == DBUILDING_HATCHER && m_info.level==(GlobalData::shared()->shield_base-1)) {
                YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(DragonBuildUpgradeView::onOkUp)));
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

void DragonBuildUpgradeView::onOkUp()
{
    if (_resGold>0) {
        string _dialog = "102312";//升级提示
        string _btnDialog = "102104";
        if (m_pos > 0) {
            _dialog = "102313";//建造提示
            _btnDialog = "102126";
        }
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                   ,this,callfunc_selector(DragonBuildUpgradeView::onLastUp)
                                                                                   , _lang(_dialog), _lang(_btnDialog)),false);
    }
    else {
        onLastUp();
    }
}

void DragonBuildUpgradeView::onLastUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    
    bool isForce = false;
    if (_resGold>0) {
        isForce = true;
    }
    if (m_pos <=0) {
        auto m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.state == FUN_BUILD_NORMAL && DragonBuildingController::getInstance()->startUpFunBuild(m_buildId, _resGold, isForce))
        {
            m_isReturn = false;
            PopupViewController::getInstance()->removeAllPopupView();
            auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->showBuildBtns(m_buildId);
        }
    }
    else {
        if(DragonBuildingController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _resGold, isForce))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}

void DragonBuildUpgradeView::updateParasInfo()
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
    auto& m_info = DragonBuildingController::getInstance()->getFunbuildById(m_buildId);
//    if (m_info.type == FUN_BUILD_WOOD || m_info.type == FUN_BUILD_FOOD || m_info.type == FUN_BUILD_IRON || m_info.type == FUN_BUILD_STONE) {
//        string nextParas = m_info.nextLevelParas;
//        vector<string> nextVec;
//        CCCommonUtils::splitString(nextParas, ",", nextVec);
//        if (nextVec.size()>=2 && m_info.para.size()>=2) {
//            m_para1Node->setVisible(true);
//            m_para1Name->setString(_lang("102122"));
//            m_para1Num->setString(m_info.para[1]);
//            string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
//            m_next1Num->setString(next1Msg);
//            
//            m_para2Node->setVisible(true);
//            m_para2Name->setString(_lang_1("102123",""));
//            m_para2Num->setString(m_info.para[2]);
//            string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
//            m_next2Num->setString(next2Msg);
//            m_nextMsgLabel->setString("");
//            tmpCnt = 2;
//        }
//    }
//    else if (m_info.type == FUN_BUILD_FORGE) {
//        string nextParas = m_info.nextLevelParas;
//        vector<string> nextVec;
//        CCCommonUtils::splitString(nextParas, ",", nextVec);
//        if (nextVec.size()>=2 && m_info.para.size()>=2) {
//            m_para1Node->setVisible(true);
//            m_para1Name->setString(_lang("102362"));
//            m_para1Num->setString(m_info.para[1]);
//            string next1Msg = getAddStr(m_info.para[1], nextVec[0], true, true);
//            m_next1Num->setString(next1Msg);
//            
//            m_para2Node->setVisible(true);
//            m_para2Name->setString(_lang("102363"));
//            m_para2Num->setString(m_info.para[2]);
//            string next2Msg = getAddStr(m_info.para[2], nextVec[1], true, true);
//            m_next2Num->setString(next2Msg);
//            m_nextMsgLabel->setString("");
//            tmpCnt = 2;
//        }
//    }
    
    if (tmpCnt==1) {
        m_para1Node->setPositionY(23);
    }
    else if (tmpCnt==2) {
        m_para1Node->setPositionY(45);
        m_para2Node->setPositionY(0);
    }
    else if (tmpCnt==3) {
        m_para1Node->setPositionY(68);
        m_para2Node->setPositionY(23);
        m_para3Node->setPositionY(-22);
        m_para4Node->setPositionY(-22);
    }
}
