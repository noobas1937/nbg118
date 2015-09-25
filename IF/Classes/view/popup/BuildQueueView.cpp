//
//  BuildQueueView.cpp
//  IF
//
//  Created by fubin on 14-2-18.
//
//

#include "BuildQueueView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
#include "QueueController.h"
#include "UseCDToolView.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "AllianceWarView.h"
#include "SceneController.h"
#include "ResourceTile.h"
#include "EnemyInfoController.h"
#include "TerritoryTile.h"
#include "WinPointsUseCommand.h"

BuildQueueView* BuildQueueView::create(string itemId, int qid, int type, bool allView,int cType)
{
    auto ret = new BuildQueueView();
    if (ret && ret->init(qid, type,allView,cType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildQueueView::init(int qid, int type,bool allView,int cType)
{
    if (cType==1) {
        CCBLoadFile("MarchQueueCell", this, this);
    }else{
        CCBLoadFile("BuildQueueView", this, this);
    }
    setContentSize(CCSizeMake(110, 59));
    m_qid = qid;
    m_cType = cType;
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    m_nameLabel->setString(info.key.c_str());
    m_icon->removeAllChildren();
    CCSprite* icon = CCSprite::createWithSpriteFrame(CCLoadSprite::loadResource(CCCommonUtils::getQueueIconByType(type).c_str()));
    CCCommonUtils::setSpriteMaxSize(icon, 40);
    m_icon->addChild(icon);
    setBtnStr();
    
    if(info.type != TYPE_MARCH){
        auto barSize = m_bar->getContentSize();
        m_bar->setSpriteFrame(CCLoadSprite::loadResource("exp_progress.png"));
        m_bar->setContentSize(barSize);
    }
    
    onEnterFrame(0);
    return true;
}

void BuildQueueView::setBtnStr(){
    std::string resultStr = _lang("108725");//go home
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    if(info.type == TYPE_MARCH){
        resultStr = _lang("104903");// speed up
    }else if(info.type == TYPE_ARMY_MASS){
        resultStr = _lang("115137"); // view 查看
    }
    string skill_point = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "skill_point");
    int point = atoi(skill_point.c_str());
    if (m_cType==1 && info.type == TYPE_MARCH) {//&&
        CCCommonUtils::setButtonTitle(m_cdBtn, "");
        m_winPointNode->setVisible(true);
        m_btnTxt->setString(resultStr.c_str());
        string itemId = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "item_id");
        auto info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
        string iconStr = "ui_gold.png";
        if (GlobalData::shared()->playerInfo.winPoint>=point) {
            m_winPointTxt->setString(CC_CMDITOA(point));
            iconStr = "icon_liansheng.png";
        }else if (info.getCNT()>0){
            m_winPointTxt->setString(CC_CMDITOA(1));
            iconStr = CCCommonUtils::getIcon(itemId);
        }else{
            m_winPointTxt->setString(CC_CMDITOA(info.price));
        }
        CCSpriteFrame* cf = CCLoadSprite::loadResource(iconStr.c_str());
        m_wIcon->initWithSpriteFrame(cf);
        CCCommonUtils::setSpriteMaxSize(m_wIcon, 30,true);
    }else{
        if(m_cType==1 && m_winPointNode){
            m_winPointNode->setVisible(false);
        }
        CCCommonUtils::setButtonTitle(m_cdBtn, resultStr.c_str());
    }
}

SEL_CCControlHandler BuildQueueView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCDClick", BuildQueueView::onCDClick);
    return NULL;
}

bool BuildQueueView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdBtn", CCControlButton*, this->m_cdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winPointNode", CCNode*, this->m_winPointNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTxt", CCLabelIF*, this->m_btnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winPointTxt", CCLabelIF*, this->m_winPointTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wIcon", CCSprite*, this->m_wIcon);
    return false;
}

void BuildQueueView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BuildQueueView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
    if (m_cType==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildQueueView::refreshBtnData), MSG_QUEUE_ADD, NULL);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildQueueView::refreshBtnData), MSG_QUEUE_REMOVE, NULL);
    }
}

void BuildQueueView::onExit()
{
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(BuildQueueView::onEnterFrame), this);
    if (m_cType==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_ADD);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_REMOVE);
    }
    CCNode::onExit();
}

void BuildQueueView::refreshBtnData(CCObject* param){
    setBtnStr();
}

void BuildQueueView::onEnterFrame(float dt)
{
    auto it = GlobalData::shared()->allQueuesInfo.find(m_qid);
    if (it == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    auto &info = it->second;
    double now = WorldController::getInstance()->getTime();
        
    if (now > it->second.finishTime) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REFRESH_UPDATE);
        //return;
    }
    
    std::string timeStr = "";
    float s = 0.0f;
    double totalTime = 0;
    double leftTime = 0;
    
    auto marchIt = WorldController::getInstance()->m_marchInfo.find(it->second.uuid);
    if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
        if(info.type == TYPE_OCCUPY_CAMP || info.type == TYPE_OCCUPY_ALLIANCE || info.type == TYPE_STATION_TERRITORY){//援助、帐篷和驻守没有时间,显示地点
            if(info.type == TYPE_OCCUPY_CAMP && (*marchIt).second.treasureMapTime>0 && (*marchIt).second.treasureMapFinishTime>0 && (*marchIt).second.treasureMapFinishTime > now){
                totalTime = (*marchIt).second.treasureMapTime * 1000;
                leftTime = (*marchIt).second.treasureMapFinishTime - now;
            }else{
                auto pt = WorldController::getInstance()->getPointByIndex(marchIt->second.endPointIndex);
                timeStr = std::string("") + CC_ITOA(pt.x) + "," + CC_ITOA(pt.y);
            }
        }
        else if(info.type == TYPE_MARCH){
            totalTime = info.finishTime - info.startTime;
            leftTime = info.finishTime - now;
        }
        else if(info.type == TYPE_OCCUPY_RESOURCE){
            totalTime = marchIt->second.troopMax / marchIt->second.digSpeed * 1000;
            leftTime = totalTime + marchIt->second.digStartTime - now;
        }
        else if(info.type == TYPE_OCCUPY_MAZE){
            totalTime = marchIt->second.mazeEndTime - marchIt->second.mazeStartTime;
            leftTime = marchIt->second.mazeEndTime - now;
        }
        else if(info.type == TYPE_ARMY_MASS){
            totalTime = info.finishTime - info.startTime;
            leftTime = info.finishTime - now;
        }
        else if (info.type == TYPE_BUILDING_TERRITORY || info.type == TYPE_REPAIR_TERRITORY || info.type == TYPE_DESTROY_TERRITORY){
            totalTime = marchIt->second.territoryFinishTime - marchIt->second.territoryStartTime;
            leftTime = marchIt->second.territoryFinishTime - now;
        }
        else if (info.type == TYPE_OCCUPY_TERRITORY_RESOURCE){
            totalTime = marchIt->second.troopMax / marchIt->second.digSpeed * 1000;
            leftTime = totalTime + marchIt->second.digStartTime - now;
        }
        else{
            totalTime = info.finishTime - info.startTime;
            leftTime = info.finishTime - now;
        }
    }
    
    if(timeStr == ""){
        totalTime = MAX(1, totalTime);
        leftTime = MAX(0, leftTime);
        leftTime = MIN(totalTime, leftTime);
        timeStr = CC_SECTOA(leftTime / 1000);
        s = 1.0 - leftTime * 1.0 / totalTime;
        s = MAX(0.0f, s);
        s = MIN(1.0f, s);
    }
    m_bar->setScaleX(s);
    m_timeLabel->setString(timeStr);
    string temp = info.key;
    if (m_cType==1 && info.type == TYPE_MARCH) {//&&
        m_timeLabel->setString("");
        temp.append(" (");
        temp.append(timeStr);
        temp.append(")");
        m_nameLabel->setString(temp.c_str());
        m_nameLabel->setPositionY(-21);
        if (m_cType==1 && m_winPointNode) {
            m_winPointNode->setVisible(true);
        }
    }else{
        m_nameLabel->setString(temp.c_str());
        m_nameLabel->setPositionY(-11);
        if (m_cType==1 && m_winPointNode) {
             m_winPointNode->setVisible(false);
        }
    }
}

void BuildQueueView::onCDClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    if(m_cType==1 && info.type == TYPE_MARCH){
        string itemId = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "item_id");
        auto &itemInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
        string skill_point = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "skill_point");
        int point = atoi(skill_point.c_str());
        if (GlobalData::shared()->playerInfo.winPoint>=point) {
            WinPointsUseCommand* cmd = new WinPointsUseCommand(CC_ITOA(ITEM_SKILL_SPEED));
            cmd->putParam("uuid", CCString::create(info.uuid));
            cmd->sendAndRelease();
        }else if (itemInfo.getCNT()>0){
            auto tmpDict = CCDictionary::create();
            tmpDict->setObject(CCString::create(info.uuid), "marchId");
            tmpDict->setObject(CCString::create(itemInfo.uuid), "itemId");
            WorldMapView::instance()->afterMarchSpeedUp(tmpDict);
            CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(itemId), _lang("104908"), _lang_1("104909", itemInfo.getName().c_str()));
        }else{
            YesNoDialog::showButtonAndGold( _lang("138153").c_str() , CCCallFunc::create(this, callfunc_selector(BuildQueueView::onYes)), _lang("104906").c_str(), itemInfo.price);
        }
        return ;
    }
    if (info.type == TYPE_MARCH) {
        PopupViewController::getInstance()->addPopupInView(UseCDToolView::create(info.uuid, CCD_MARCH_TYPE));
    }
    else if(info.type == TYPE_ARMY_MASS){
        PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
    }
    else if(info.type == TYPE_OCCUPY_RESOURCE || info.type == TYPE_OCCUPY_MAZE || info.type == TYPE_OCCUPY_CAMP || info.type == TYPE_OCCUPY_ALLIANCE || info.type == TYPE_BUILDING_TERRITORY || info.type == TYPE_REPAIR_TERRITORY || info.type == TYPE_STATION_TERRITORY || info.type == TYPE_DESTROY_TERRITORY || info.type == TYPE_OCCUPY_TERRITORY_RESOURCE){
        if(info.type == TYPE_OCCUPY_CAMP){
            auto marchIt = WorldController::getInstance()->m_marchInfo.find(info.uuid);
            if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
                if((*marchIt).second.treasureMapFinishTime>0 && (*marchIt).second.treasureMapTime>0 && (*marchIt).second.treasureMapFinishTime > WorldController::getInstance()->getTime()){
                    YesNoDialog::show( _lang("111503").c_str() , CCCallFunc::create(this, callfunc_selector(BuildQueueView::confirmReturn)), 0);
                    return;
                }
            }
        }
        YesNoDialog::show( _lang("110098").c_str() , CCCallFunc::create(this, callfunc_selector(BuildQueueView::confirmReturn)), 0);
    }
}

void BuildQueueView::onYes(){
    string itemId = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "item_id");
    ToolController::getInstance()->buyTool(atoi(itemId.c_str()), 1, CCCallFunc::create(this, callfunc_selector(BuildQueueView::onUseTool)),0,true,false,"UseCDToolView");
}

void BuildQueueView::onUseTool(){
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    std::string marchId = info.uuid;
    auto marchIt = WorldController::getInstance()->m_marchInfo.find(marchId);
    if(marchIt != WorldController::getInstance()->m_marchInfo.end() && marchIt->second.marchType == MethodRally && marchIt->second.teamUid != ""){
        marchId = marchIt->second.teamUid;
    }
    string iId = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_SPEED), "item_id");
    int itemId = atoi(iId.c_str());
    auto tmpDict = CCDictionary::create();
    tmpDict->setObject(CCString::create(marchId), "marchId");
    tmpDict->setObject(CCString::create(ToolController::getInstance()->getToolInfoById(itemId).uuid), "itemId");
    ToolController::getInstance()->useTool(itemId);
    WorldMapView::instance()->afterMarchSpeedUp(tmpDict);
}

void BuildQueueView::confirmReturn(){
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    auto marchIt = WorldController::getInstance()->m_marchInfo.find(info.uuid);
    if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
        
        vector<EnemyInfo>::iterator enemyIt= EnemyInfoController::getInstance()->m_enemyInfos.begin();
        for(auto &enemyInfo : EnemyInfoController::getInstance()->m_enemyInfos)
        {
            for(auto &marchInfo : WorldController::getInstance()->m_marchInfo)
            {
                if(enemyInfo.uuid == marchInfo.second.uuid && marchInfo.second.endPointIndex == marchIt->second.endPointIndex)
                {
                    EnemyInfoController::getInstance()->m_enemyInfos.erase(enemyIt);
                    break;
                }
            }
            enemyIt++;
        }
        
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(marchIt->second.uuid), "marchId");
        WorldMapView::instance()->afterMarchCancel(dict);
    }
}

bool BuildQueueView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch)&&hasVisibleParents(this)) {
        return true;
    }
    return false;
}

void BuildQueueView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!WorldMapView::instance()) {
        return;
    }
    auto &info = GlobalData::shared()->allQueuesInfo[m_qid];
    if (isTouchInside(m_bg, pTouch)) {
        if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
            return;
        }
        auto marchIt = WorldController::getInstance()->m_marchInfo.find(info.uuid);
        if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
            std::string uid = info.uuid;
            auto pt = WorldController::getPointByIndex(marchIt->second.endPointIndex);
            if(info.type == TYPE_MARCH){
                auto realPt = WorldMapView::instance()->getMarchPoint(marchIt->second);
                if(marchIt->second.marchType == MethodRally){
                    auto teamMarchIt = WorldController::getInstance()->m_marchInfo.find(marchIt->second.teamUid);
                    if(marchIt != WorldController::getInstance()->m_marchInfo.end() && teamMarchIt->second.uuid != info.uuid){
                        realPt = WorldMapView::instance()->getMarchPoint(teamMarchIt->second);
                        uid = teamMarchIt->second.uuid;
                    }
                }
                if (realPt.equals(ccp(-1, -1))) {
                    return;
                }
                pt = WorldMapView::instance()->m_map->getTilePointByViewPoint(realPt);
            }else{
                
            }
            
            if(info.type == TYPE_MARCH){
                WorldMapView::instance()->showTroopInfo(uid);
            }
            else if(info.type == TYPE_OCCUPY_RESOURCE)
            {
                //WorldMapView::instance()->setNeedOpenInfoView(true);
                WorldController::getInstance()->openTargetIndex = marchIt->second.endPointIndex;
//                WorldMapView::instance()->autoAddResourceView(marchIt->second);
            }
            else if (info.type == TYPE_BUILDING_TERRITORY || info.type == TYPE_REPAIR_TERRITORY || info.type == TYPE_STATION_TERRITORY || info.type == TYPE_DESTROY_TERRITORY || info.type == TYPE_OCCUPY_TERRITORY_RESOURCE) {
                WorldController::getInstance()->openTargetIndex = marchIt->second.endPointIndex;
            }
            WorldMapView::instance()->gotoTilePoint(pt, GlobalData::shared()->playerInfo.selfServerId);
        }
    }
}

