//
//  UseCDToolView.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "UseCDToolView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "GeneralManager.h"
#include "ScienceController.h"
#include "QueueController.h"

const float numPerRow = 1.0;
UseCDToolView* UseCDToolView::create(string itemId, int qid, bool timeFlag, double startTime, double endTime){
    UseCDToolView* ret = new UseCDToolView();
    if(ret && ret->init(itemId, qid, timeFlag, startTime, endTime)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseCDToolView::init(string itemId, int qid, bool timeFlag, double startTime, double endTime)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    m_qid = qid;
    m_itemId = itemId;
    m_timeFlag = timeFlag;
    m_startTime = startTime;
    m_endTime = endTime;
    auto tmpCCB = CCBLoadFile("UseCDToolView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        this->setContentSize(tmpCCB->getContentSize());
        
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));

    }
    if (m_qid == CCD_QUEUE_OPEN ) {
        m_progressBarNode->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height+100));
        }
        else
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height+50));
    }
    setTitleName(_lang("104903").c_str());
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    updateInfo();
    
    float oldBgHeight = m_buildBG->getContentSize().height;//fusheng
    changeBGMaxHeight(m_buildBG);
    m_buildBG->setPositionY(m_buildBG->getPositionY()+m_buildBG->getContentSize().height-oldBgHeight);
    
    return true;
}

void UseCDToolView::updateInfo()
{
    m_nameLabel->setString(_lang("102159"));
    m_sumTime = getTotalTime();
    onEnterFrame(0);
    refreashData(NULL);
}

void UseCDToolView::refreashData(CCObject* obj)
{
    m_lastMaxTime = 0;//小于剩余时间的 最大可加速时间
    m_curList.clear();
    bool tmpSt = true;
    if (m_qid == CCD_QUEUE_OPEN ) {
        std::vector<int> tmpVector = ToolController::getInstance()->m_typeTools[ITEM_TYPE_QUEUE];
        int type2 = atoi(m_itemId.c_str());
        for (int i=0; i<tmpVector.size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById(tmpVector[i]);
            if (info.type2 == type2 && (info.price>0 || info.getCNT()>0)) {
                m_curList.push_back(info.itemId);
            }
        }
    }
    else {
        std::vector<int> tmpVector = ToolController::getInstance()->m_typeTools[ITEM_TYPE_SPD];
        if (GlobalData::shared()->allQueuesInfo[m_qid].type == TYPE_BUILDING) {
            m_curList.push_back(0);
        }
        for (int i=0; i<tmpVector.size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById(tmpVector[i]);
            if (m_qid == CCD_MARCH_TYPE) {
                if (info.type2 == 2) {
                    int spdTime = atoi(info.paras[3].c_str());
                    if (info.getCNT()>0) {
                        m_curList.push_back(info.itemId);
                        if (spdTime>=m_curTime) {
                            tmpSt = false;
                        }
                        else {
                            if (spdTime > m_lastMaxTime) {
                                m_lastMaxTime = spdTime;
                            }
                        }
                    }
                    else if (info.price>0) {
                        int spdTime = atoi(info.paras[3].c_str());
                        if (/*spdTime<m_curTime*/m_curTime > 0) {
                            m_curList.push_back(info.itemId);
                            if (spdTime > m_lastMaxTime) {
                                m_lastMaxTime = spdTime;
                            }
                        }
                        else if (tmpSt) {
                            m_curList.push_back(info.itemId);
                            tmpSt = false;
                        }
                    }
                }
            }
            else {
                if (info.type2 == 1) {
                    int spdTime = atoi(info.paras[3].c_str());
                    if (info.getCNT()>0) {
                        m_curList.push_back(info.itemId);
                        if (spdTime>=m_curTime) {
                            tmpSt = false;
                        }
                        else {
                            if (spdTime > m_lastMaxTime) {
                                m_lastMaxTime = spdTime;
                            }
                        }
                    }
                    else if (info.price>0) {
                        if (spdTime<m_curTime) {
                            m_curList.push_back(info.itemId);
                            if (spdTime > m_lastMaxTime) {
                                m_lastMaxTime = spdTime;
                            }
                        }
                        else if (tmpSt) {
                            m_curList.push_back(info.itemId);
                            tmpSt = false;
                        }
                    }
                }
            }
        }
    }
    m_tabView->reloadData();
}

std::string UseCDToolView::getNameStr(){
    
    if (m_qid == CCD_MARCH_TYPE || m_qid == CCD_TRAIN_GENERAL_TYPE) {
        return "";
    }
    else {
        auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
        if(info.type == TYPE_BUILDING){
            auto info = FunBuildController::getInstance()->getFunbuildById(atoi(m_itemId.c_str()));
            return _lang(info.name);
        }
        else if(info.type == TYPE_FORCE || info.type == TYPE_FORT || info.type == TYPE_HOSPITAL || info.type == TYPE_AFFAIRS || info.type == TYPE_ALLIANCE_AFFAIRS || info.type == TYPE_RIDE_SOLDIER || info.type == TYPE_BOW_SOLDIER || info.type == TYPE_CAR_SOLDIER){
            int id = atoi(m_itemId.c_str()) / 100;
            return CCCommonUtils::getNameById(CC_ITOA(id));
        }
    }
    return "";
}

int UseCDToolView::getTotalTime(){
    int totalTime = 0;
    if (m_qid == CCD_QUEUE_OPEN) {
        return totalTime;
    }
    if (m_qid == CCD_MARCH_TYPE) {
        if (m_timeFlag) {
            return m_endTime - m_startTime;
        }
        else {
            const MarchInfo& info = WorldController::getInstance()->m_marchInfo[m_itemId];
            return (info.endStamp - info.startStamp) / 1000;
        }
    }else if(m_qid == CCD_TRAIN_GENERAL_TYPE){
        
    }
    else {
        auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
        if(info.type == TYPE_BUILDING){
            auto info = FunBuildController::getInstance()->getFunbuildById(atoi(m_itemId.c_str()));
            if (info.state == FUN_BUILD_DESTROY) {
                totalTime = info.des_time;
            }else{
                totalTime = info.time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
            }
        }
        else{
            totalTime = info.finishTime - info.startTime;
        }
    }
    if(totalTime == 0){
        totalTime = 1;
    }
    return totalTime;
}


void UseCDToolView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UseCDToolView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseCDToolView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseCDToolView::updateArriveTime), MSG_CHANGE_ALLIANCE_TEAM_ARRIVE_TIME, NULL);
}

void UseCDToolView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UseCDToolView::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_TEAM_ARRIVE_TIME);
    CCNode::onExit();
}

void UseCDToolView::updateArriveTime(CCObject* data){
    auto dic = _dict(data);
    if(dic){
        auto temp = dic->valueForKey("et")->doubleValue()/1000;
        if(temp!=0){
            temp = GlobalData::shared()->changeTime(temp);
        }else{
            temp = 0;
        }
        m_endTime = temp;
    }
}

void UseCDToolView::onEnterFrame(float dt)
{
    if (m_qid == CCD_QUEUE_OPEN) {
        return;
    }
    auto curTime = GlobalData::shared()->getWorldTime();
    if (m_qid == CCD_MARCH_TYPE) {
        if (m_timeFlag) {
            auto now = GlobalData::shared()->getWorldTime();
            int curTime = m_endTime - now;
            m_curTime = curTime;
            if (curTime>0) {
                float pro = curTime*1.0/m_sumTime;
                pro = pro>1?1:pro;
                m_bar->setScaleX(1-pro);
                m_timeLabel->setString(CC_SECTOA(curTime));
            } else {
                PopupViewController::getInstance()->removeAllPopupView();
            }
        }
        else {
            const MarchInfo& info = WorldController::getInstance()->m_marchInfo[m_itemId];
            auto now = WorldController::getInstance()->getTime();
            int curTime = (info.endStamp - now)/1000;
            m_curTime = curTime;
            if (curTime>0) {
                float pro = curTime*1.0/m_sumTime;
                pro = pro>1?1:pro;
                m_bar->setScaleX(1-pro);
                m_timeLabel->setString(CC_SECTOA(curTime));
            } else {
                PopupViewController::getInstance()->removeAllPopupView();
            }
        }
    }else if(m_qid == CCD_TRAIN_GENERAL_TYPE){
        m_bar->setScaleX(1-0.2);
        m_timeLabel->setString(CC_SECTOA(1000000));
        map<string,TrainInfo* >::iterator iter = GlobalData::shared()->trainList.find(GlobalData::shared()->m_speedTrainId);
        if(iter!=GlobalData::shared()->trainList.end()){
            int curTime = (iter->second->m_finish - WorldController::getInstance()->getTime())/1000;
            m_curTime = curTime;
            if (curTime>0) {
                float pro = curTime*1.0/m_sumTime;
                pro = pro>1?1:pro;
                m_bar->setScaleX(1-pro);
                m_timeLabel->setString(CC_SECTOA(curTime));
            }
        }
    }
    else {
        m_finishTime = GlobalData::shared()->allQueuesInfo[m_qid].finishTime;
        m_curTime = m_finishTime - curTime;
        if (m_finishTime >= curTime) {
            int tmpTime = m_finishTime-curTime;
            float pro = tmpTime*1.0/m_sumTime;
            pro = pro>1?1:pro;
            m_bar->setScaleX(1-pro);
            m_timeLabel->setString(CC_SECTOA(tmpTime));
        }
        else{
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
    
    if (m_lastMaxTime >= m_curTime) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
    }
}

SEL_CCControlHandler UseCDToolView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool UseCDToolView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBarNode", CCNode*, this->m_progressBarNode);
    return false;
}

void UseCDToolView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
}

cocos2d::CCSize UseCDToolView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1476, 290);
    }
    return CCSize(610, 160);
}

CCTableViewCell* UseCDToolView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    UseToolCell* cell = (UseToolCell*)table->dequeueGrid();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx], m_itemId, m_qid);
        }else{
            cell = UseToolCell::create(m_curList[idx], m_itemId, m_qid);
        }
    }
    return cell;
}

ssize_t UseCDToolView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_curList.size() / numPerRow);
    return num;
}

unsigned int UseCDToolView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

////

UseToolCell* UseToolCell::create(int itemId, string objId, int qid)
{
    auto ret = new UseToolCell();
    if (ret && ret->init(itemId, objId, qid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseToolCell::init(int itemId, string objId, int qid)
{
    bool ret = true;
    m_waitInterFace = NULL;
    CCBLoadFile("UseToolCell",this,this);
    setData(itemId, objId, qid);
    return ret;
}

void UseToolCell::setData(int itemId, string objId, int qid)
{
    if(m_waitInterFace){
        m_waitInterFace->remove();
        m_waitInterFace = NULL;
    }

    m_itemId = itemId;
    m_objId = objId;
    m_qid = qid;
    
    m_picNode->removeAllChildren();
    m_lockNode->removeAllChildren();
    m_des2Label->setString("");
    if (m_itemId == 0) {
        m_numLabel->setString("");
        m_nameLabel->setString(_lang("103669").c_str());
        m_des2Label->setString(_lang("103671").c_str());
        m_buyNode->setVisible(false);
        CCCommonUtils::setButtonTitle(m_useBtn, _lang("103672").c_str());
        CCCommonUtils::setButtonSprite(m_useBtn, "btn_green4.png");
        
        m_useBtn->setVisible(true);
        m_buyNode->setVisible(false);
        m_buyBtn->setEnabled(false);
        int curTime = GlobalData::shared()->getWorldTime();
        int m_finishTime = GlobalData::shared()->allQueuesInfo[m_qid].finishTime;
        int tmpTime = m_finishTime-curTime;
        if (tmpTime < GlobalData::shared()->freeSpdT) {
            m_useBtn->setEnabled(true);
            m_desLabel->setString("");
        }
        else {
            auto picLock = CCLoadSprite::createSprite("iron_lock.png");
            picLock->setScale(0.5);
            m_lockNode->addChild(picLock);
            m_useBtn->setEnabled(false);
            m_desLabel->setString(_lang_1("103670", CC_SECTOA(tmpTime-GlobalData::shared()->freeSpdT)));
        }
        
        auto pic = CCLoadSprite::createSprite("free_icon.png");
        m_picNode->addChild(pic);
    }
    else {
        ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
        
        m_nameLabel->setString(toolInfo.getName());
        m_desLabel->setString(_lang(toolInfo.des).c_str());
        m_numLabel->setString(CC_ITOA(toolInfo.getCNT()));
        m_price = toolInfo.price;
        
        CCCommonUtils::createGoodsIcon(toolInfo.itemId, m_picNode, CCSize(80, 80));
        
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 80);
//            m_picNode->addChild(iconBg);
//            
//            CCCommonUtils::setSpriteMaxSize(pic, 80);
//            m_picNode->addChild(pic);
//        }
        
        m_inBtnGoldNum->setString(CC_CMDITOA(m_price));
        CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
        m_btnMsgLabel->setString(_lang("104906").c_str());
        
        if (toolInfo.getCNT()>0) {
            m_buyNode->setVisible(false);
            m_buyBtn->setEnabled(false);
            m_useBtn->setVisible(true);
            m_useBtn->setEnabled(true);
        }
        else {
            m_buyNode->setVisible(true);
            m_buyBtn->setEnabled(true);
            m_useBtn->setVisible(false);
            m_useBtn->setEnabled(false);
        }
    }
}

void UseToolCell::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UseToolCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void UseToolCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UseToolCell::onEnterFrame), this);
    CCNode::onExit();
}

void UseToolCell::onEnterFrame(float dt)
{
    if(m_itemId == 0) {
        int curTime = GlobalData::shared()->getWorldTime();
        int m_finishTime = GlobalData::shared()->allQueuesInfo[m_qid].finishTime;
        int tmpTime = m_finishTime-curTime;
        if (tmpTime <= GlobalData::shared()->freeSpdT) {
            m_desLabel->setString("");
            m_lockNode->removeAllChildren();
            m_useBtn->setEnabled(true);
        }
        else {
            m_desLabel->setString(_lang_1("103670", CC_SECTOA(tmpTime-GlobalData::shared()->freeSpdT)));
        }
    }
}

SEL_CCControlHandler UseToolCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", UseToolCell::onClickUseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", UseToolCell::onClickBuyBtn);
    return NULL;
}

void UseToolCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_qid == CCD_MARCH_TYPE) {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (toolInfo.getCNT() <= 0) {
            YesNoDialog::show( _lang("101320").c_str() , CCCallFunc::create(this, callfunc_selector(UseToolCell::onYes)), m_price);
        }
        else {
            int tCNT = toolInfo.getCNT() - 1;
            toolInfo.setCNT(tCNT);
            m_numLabel->setString(CC_ITOA(toolInfo.getCNT()));

            auto tmpDict = CCDictionary::create();
            std::string marchId = m_objId;
            auto marchIt = WorldController::getInstance()->m_marchInfo.find(marchId);
            if(marchIt != WorldController::getInstance()->m_marchInfo.end()
               && marchIt->second.marchType == MethodRally
               && marchIt->second.teamUid != ""
               && marchIt->second.stateType == StateMarch){
                marchId = marchIt->second.teamUid;
            }
            tmpDict->setObject(CCString::create(marchId), "marchId");
            tmpDict->setObject(CCString::create(toolInfo.uuid), "itemId");
            WorldMapView::instance()->afterMarchSpeedUp(tmpDict);
            
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(m_itemId)), _lang("104908"), _lang_1("104909", toolInfo.getName().c_str()));
        }
    }else if(m_qid == CCD_TRAIN_GENERAL_TYPE){
        map<string,TrainInfo* >::iterator iter = GlobalData::shared()->trainList.find(GlobalData::shared()->m_speedTrainId);
        if(iter!=GlobalData::shared()->trainList.end()){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
//            GeneralManager::getInstance()->speedTrainGeneral(iter->second->m_trainId,info.uuid);
//            PopupViewController::getInstance()->addPopupInView(GeneralTrainView::create());
        }
    }
    else if(m_qid == CCD_QUEUE_OPEN){
        ToolController::getInstance()->useTool(m_itemId,1,true);
        PopupViewController::getInstance()->goBackPopupView();
    }
    else {
        auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
        
        string toolItemId = CC_ITOA(m_itemId);
        if (m_itemId==0) {
            toolItemId = "";
        }
        
        if(info.type == TYPE_BUILDING){
            if(FunBuildController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId))
            {
                PopupViewController::getInstance()->goBackPopupView();
            }
            else
            {
                onYes();
            }
        }else if(info.type == TYPE_FORCE || info.type == TYPE_FORT || info.type == TYPE_HOSPITAL || info.type == TYPE_RIDE_SOLDIER || info.type == TYPE_BOW_SOLDIER || info.type == TYPE_CAR_SOLDIER){
            if(ArmyController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId))
            {
                PopupViewController::getInstance()->goBackPopupView();
            }
            else
            {
                onYes();
            }
        }else if(info.type == TYPE_AFFAIRS || info.type == TYPE_ALLIANCE_AFFAIRS){
//            if(AffairsController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId))
//            {
//                PopupViewController::getInstance()->goBackPopupView();
//            }
//            else
//            {
//                onYes();
//            }
        }
        else if(info.type == TYPE_SCIENCE){
            if(ScienceController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId))
            {
                PopupViewController::getInstance()->goBackPopupView();
            }
            else
            {
                onYes();
            }
        }
        else if(info.type == TYPE_FORGE || info.type == TYPE_MATE){
            auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolItemId.c_str()));
            if (toolInfo.getCNT() <= 0) {
                CCCommonUtils::flyText(_lang("102198"));
                onYes();
            }else {
                int toolCNT = toolInfo.getCNT() - 1;
                toolInfo.setCNT(toolCNT);
                QueueController::getInstance()->startCCDQueue(info.qid, toolItemId,false,0,"",5);
                PopupViewController::getInstance()->goBackPopupView();
            }
        }
    }
    
    if (m_itemId>0) {
        ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (toolInfo.getCNT()<=0) {
            if (toolInfo.price>0) {
                m_buyNode->setVisible(true);
                m_buyBtn->setEnabled(true);
            }
            else {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
            }
            m_useBtn->setVisible(false);
            m_useBtn->setEnabled(false);
        }
    }
}

void UseToolCell::onYes()
{
    m_waitInterFace = GameController::getInstance()->showWaitInterface(m_buyBtn);
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(UseToolCell::onUseTool)),0,true,false,"UseCDToolView");
}

void UseToolCell::onUseTool()
{
    if(m_waitInterFace){
        m_waitInterFace->remove();
        m_waitInterFace = NULL;
    }
    if (m_qid == CCD_MARCH_TYPE) {
        auto tmpDict = CCDictionary::create();
        std::string marchId = m_objId;
        auto marchIt = WorldController::getInstance()->m_marchInfo.find(marchId);
        if(marchIt != WorldController::getInstance()->m_marchInfo.end() && marchIt->second.marchType == MethodRally && marchIt->second.teamUid != ""){
            marchId = marchIt->second.teamUid;
        }

        tmpDict->setObject(CCString::create(marchId), "marchId");
        tmpDict->setObject(CCString::create(ToolController::getInstance()->getToolInfoById(m_itemId).uuid), "itemId");
        ToolController::getInstance()->useTool(m_itemId);
        WorldMapView::instance()->afterMarchSpeedUp(tmpDict);
    }else if(m_qid == CCD_TRAIN_GENERAL_TYPE){
        
    }
    else if(m_qid == CCD_QUEUE_OPEN){
        ToolController::getInstance()->useTool(m_itemId,1,true);
        PopupViewController::getInstance()->goBackPopupView();
    }
    else {
        auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
        
        string toolItemId = CC_ITOA(m_itemId);
        if (m_itemId==0) {
            toolItemId = "";
        }
        
        if(info.type == TYPE_BUILDING){
            FunBuildController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId);
        }else if(info.type == TYPE_FORCE || info.type == TYPE_FORT || info.type == TYPE_HOSPITAL || info.type == TYPE_RIDE_SOLDIER || info.type == TYPE_BOW_SOLDIER || info.type == TYPE_CAR_SOLDIER){
            ArmyController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId);
        }else if(info.type == TYPE_AFFAIRS){
//            AffairsController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId);
        }else if(info.type == TYPE_SCIENCE){
            ScienceController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId);
        }else if(info.type == TYPE_FORGE || info.type == TYPE_MATE){
            QueueController::getInstance()->startCCDQueue(info.qid, toolItemId,false, 0,"",5);
        }else if(info.type == TYPE_ALLIANCE_AFFAIRS){
//            AffairsController::getInstance()->costCD(atoi(m_objId.c_str()), toolItemId);
        }
    }
    
    if (m_itemId>0) {
        ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (toolInfo.getCNT()<=0) {
            if (toolInfo.price>0) {
                m_buyNode->setVisible(true);
                m_buyBtn->setEnabled(true);
            }
            else {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
            }
            m_useBtn->setVisible(false);
            m_useBtn->setEnabled(false);
        }
    }
}

void UseToolCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_qid == CCD_MARCH_TYPE) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);//获取新手迁城道具
        if (info.getCNT()>0) {
            onYes();
        }else{
            YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(UseToolCell::onYes)), _lang("104906").c_str(), info.price);
        }
        
    }else{
        onYes();
    }
}

bool UseToolCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_des2Label", CCLabelIF*, m_des2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, m_lockNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    return false;
}