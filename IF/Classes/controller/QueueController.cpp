//
//  QueueController.cpp
//  IF
//
//  Created by fubin on 14-2-19.
//
//

#include "QueueController.h"
#include "YesNoDialog.h"
#include "QueueCommand.h"
#include "FunBuildController.h"
#include "DragonBuildingController.h"
#include "FunBuildInfo.h"
#include "ArmyController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ScienceController.h"
#include "../Ext/CCDevice.h"
#include "EquipmentController.h"
#include "SoundController.h"
#include "ImperialScene.h"
#include "SceneController.h"
#include "EquipmentCreateView.h"
#include "PopupViewController.h"
#include "ActivityController.h"
#include "FBUtilies.h"
#include "ToolCreateView.h"

#include "EquipSiteView.h"
#include "EquipmentListView.h"
#include "EquipmentCreateView.h"
#include "EquipNewUseView.h"

static QueueController *_instance = NULL;

QueueController::QueueController()
{
    m_buildType = 0;
    ShowRecAllianceTime = 0;
    m_waitInterface = NULL;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(QueueController::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

QueueController::~QueueController() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(QueueController::onEnterFrame), this);
}

void QueueController::TitanFinishCD(float dt)
{
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(FUN_BUILD_MAIN_CITY_ID));
    string uuid = GlobalData::shared()->allQueuesInfo[qid].uuid;//fusheng 10.13
    QueueFinishCommand* cmd = new QueueFinishCommand(QUEUE_FINISH_COMMAND, uuid);
    
    auto biter = GlobalData::shared()->imperialInfo.find(FUN_BUILD_MAIN_CITY_ID);
    
    if (biter!=GlobalData::shared()->imperialInfo.end()) {
        
        FunBuildController::getInstance()->completeUpOrCreate(FUN_BUILD_MAIN_CITY_ID,false,true);//fusheng 主城数据刷新
        biter->second.state = FUN_BUILD_UPING_END;//fusheng  设置状态 保证正常逻辑
    }
    
    CCLOG("TitanFinishCD");
    cmd->sendAndRelease();
}


void QueueController::onEnterFrame(float dt)
{
    if (ShowRecAllianceTime>0) {
        ShowRecAllianceTime--;
    }
    map<int, QueueInfo>::iterator it;
    auto curTime = GlobalData::shared()->getWorldTime();
    int bnt = 0;
    int qid = 0;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        if((it->second).key != "" && (it->second).key!="999")
        {
            auto finishTime = (it->second).finishTime;
            if (finishTime <= curTime) {
                it->second.canHelp = true;
                if ((it->second).type == TYPE_SCIENCE) {
                    int itemId = atoi((it->second).key.c_str()) / 100;
                    ScienceController::getInstance()->startFinishScience(itemId, (it->second).uuid);
                    startFinishQueue((it->second).qid, false);
                }
                else if(it->second.type == TYPE_FORCE || it->second.type == TYPE_RIDE_SOLDIER || it->second.type == TYPE_BOW_SOLDIER ||it->second.type == TYPE_CAR_SOLDIER){
//                    std::string itemId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
//                    ArmyController::getInstance()->startFinishForce(itemId, (it->second).uuid);
//                    startFinishQueue((it->second).qid, false);
                }
                else if((it->second).type == TYPE_FORT){
//                    std::string itemId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
//                    ArmyController::getInstance()->startFinishFort(itemId, (it->second).uuid);
//                    startFinishQueue((it->second).qid, false);
                }
                else if((it->second).type == TYPE_HOSPITAL){
                    ArmyController::getInstance()->startFinishHospital((it->second).uuid);
                    startFinishQueue((it->second).qid, false);
                }
                else if((it->second).type == TYPE_BUILDING){
                    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
                    {
                        if (it->second.key != "") {
                            auto biter = GlobalData::shared()->imperialInfo.find(atoi(it->second.key.c_str()));
                            if (biter != GlobalData::shared()->imperialInfo.end() && (biter->second.state == FUN_BUILD_CREATE || biter->second.state == FUN_BUILD_UPING)) {
                                auto iter = m_buildQueueInfo.find(it->first);
                                if (iter == m_buildQueueInfo.end()) {
                                    m_buildQueueInfo[it->first] = QueueInfo(it->second);
                                    string para1 = _lang(biter->second.name);
                                    string para2 = string (" Lv") + CC_ITOA(biter->second.level + 1) + string(" ");
                                    CCCommonUtils::flyHint("", "", _lang_2("137480", para1.c_str(), para2.c_str()), 2, 0, true);
                                    SoundController::sharedSound()->playEffects(Music_Sfx_UI_buildfinish);
                                    
                                    //fusheng 在世界场景里  建筑物升级 仅仅只是提示一下
                                    if(it->second.itemId == 400000) //主城发一个
                                    {
                                        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(QueueController::TitanFinishCD), this, 0.5f,0, 1.0f, false);//fusheng 延迟1。5秒  确保数据正常
                                        CCLOG("pre TitanFinishCD");
                                    }
                                } else {
                                    if (it->second.key != iter->second.key || it->second.startTime != iter->second.startTime || it->second.endTime != iter->second.endTime || it->second.finishTime != iter->second.finishTime) {
//                                        m_buildQueueInfo[it->first] = QueueInfo(it->second);
                                        iter->second.qid = it->second.qid;
                                        iter->second.type = it->second.type;
                                        iter->second.itemId = it->second.itemId;
                                        iter->second.canHelp = it->second.canHelp;
                                        iter->second.endTime = it->second.endTime;
                                        iter->second.finishTime = it->second.finishTime;
                                        iter->second.startTime = it->second.startTime;
                                        iter->second.key = it->second.key;
                                        iter->second.uuid = it->second.uuid;
                                        string para1 = _lang(biter->second.name);
                                        string para2 = string (" Lv") + CC_ITOA(biter->second.level + 1) + string(" ");
                                        CCCommonUtils::flyHint("", "", _lang_2("137480", para1.c_str(), para2.c_str()), 2, 0, true);
                                        SoundController::sharedSound()->playEffects(Music_Sfx_UI_buildfinish);
                                        

                                    }
                                }
                            }
                        }
                    }
                }else if((it->second).type == TYPE_MATE){
                }else if((it->second).type == TYPE_DRAGON_BRON){
                }else if((it->second).type == TYPE_DRAGON_EGG){
                }else if((it->second).type == TYPE_DRAGON_BUILD){
                }else if((it->second).type == TYPE_FORGE)
                {
                    PopupBaseView* curView = PopupViewController::getInstance()->getCurrentPopupView();
                    EquipSiteView* siteView = dynamic_cast<EquipSiteView*>(curView);
                    EquipmentListView* listView = dynamic_cast<EquipmentListView*>(curView);
                    EquipmentCreateView* createView = dynamic_cast<EquipmentCreateView*>(curView);
                    EquipNewUseView* newUseView = dynamic_cast<EquipNewUseView*>(curView);
                    if (siteView || listView || createView || newUseView) {
                        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
                        autoHavestEquip((it->second).qid, bid);
                    }
                }
                else {
                    (it->second).key = "";
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
                }
            }
            else
            {
                if((it->second).type == TYPE_BUILDING || (it->second).type == TYPE_DRAGON_BUILD){// && (finishTime-curTime)<=GlobalData::shared()->freeSpdT
                    bnt++;
                    qid = it->first;
                }
                if ( (finishTime-curTime)<=2 ) {
                    cocos2d::extension::CCDevice::cancelNotice((it->second).qid);
                }
            }
        }
    }
    
    //港口礼包及时取消推送
    time_t targetTime = PortActController::getInstance()->m_nextRewardTime;
    time_t nowTime = GlobalData::shared()->getTimeStamp();
    int leftTime = 0;
    leftTime = targetTime - nowTime;
    if (leftTime == 2) {
        cocos2d::extension::CCDevice::cancelNotice(89757);
    } 
//    if (bnt==0 && m_buildType != 1) {
//        m_buildType = 1;
//        UIComponent::getInstance()->updateBuildState(false);
//    }
//    else if (bnt>0 && m_buildType != 2) {
//        m_buildType = 2;
//        UIComponent::getInstance()->updateBuildState(true,qid);
//    }
}

int QueueController::getQueueNumByType(int type)
{
    int ret = 0;
    map<int, QueueInfo>::iterator it;
    auto curTime = GlobalData::shared()->getWorldTime();
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        if((it->second).type==type && (it->second).key != "" && (it->second).key!="999" && (it->second).finishTime > curTime){
            ret++;
        }
    }
    return ret;
}

bool QueueController::checkMarchStatus()
{
    bool flag = false;
    map<string,MarchInfo>::iterator it;
    for(it=WorldController::getInstance()->m_marchInfo.begin();it!=WorldController::getInstance()->m_marchInfo.end();it++){
        if(it->second.ownerType==PlayerSelf){
            if(it->second.targetType==ResourceTile || it->second.targetType==CampTile || it->second.targetType==MonsterTile || it->second.targetType==FieldMonster || it->second.targetType==ActBossTile || it->second.targetType==Tile_allianceArea || it->second.targetType==tile_superMine || it->second.targetType == tile_tower || it->second.targetType == tile_banner){
                continue;
            }
            if(it->second.marchType == MethodScout || it->second.marchType == MethodRally || it->second.marchType == MethodBattle || it->second.marchType == MethodUnion){
                auto curTime = WorldController::getInstance()->getTime();
                auto endTime = it->second.endStamp;
                if(curTime<endTime){//有行军队伍
                    flag = true;
                    break;
                }
            }
        }
    }
    return flag;
}

QueueController* QueueController::getInstance() {
    if (!_instance) {
        _instance = new QueueController();
    }
    return _instance;
}

void QueueController::purgeData() {
    delete _instance;
    _instance = NULL;
}

int QueueController::getQID(int type)
{
    return CCCommonUtils::getQueueSortByType(type) *1000 + (1+type)*100;
}

void QueueController::initQueueData(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("queue"));
    if( arr==NULL ){
        return;
    }
    
    CCDictionary *item = NULL;
//    int type = -1;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int queueType = item->valueForKey("type")->intValue();
        if (queueType == QueueType::TYPE_AFFAIRS) {
            continue;
        }
//        if (type != queueType) {
//            QueueInfo titleInfo = QueueInfo();
//            titleInfo.type = queueType;
//            titleInfo.key = "999";
//            titleInfo.qid = getQID(queueType);
//            type = queueType;
//            GlobalData::shared()->allQueuesInfo[titleInfo.qid] = titleInfo;
//        }

        QueueInfo info = QueueInfo(item);
        if(info.type == 15)//fusheng 泰坦喂食cd  更新titanInfo
        {
            
            if(info.finishTime > GlobalData::shared()->titanInfo.feedcd)//fusheng 因为可能会有好几个type为15的queue 选择时间大的
            {
                GlobalData::shared()->titanInfo.feedcd = info.finishTime;
                GlobalData::shared()->titanInfo.uuid = info.uuid;
            }
        }
        GlobalData::shared()->allQueuesInfo[info.qid] = info;
    }
    
    CCLOG("queue end");
}

void QueueController::initMarchQueueData()
{
    return;
    int qid = getQID(TYPE_MARCH);
    //march title init
    auto &queue = GlobalData::shared()->allQueuesInfo[qid];
    queue.qid = qid;
    queue.type = TYPE_MARCH;
    queue.key = "999";
    //march temp data init
    auto curTime = GlobalData::shared()->getWorldTime();
    qid += 499;
    auto &queueTemp = GlobalData::shared()->allQueuesInfo[qid];
    queueTemp.qid = qid;
    queueTemp.type = TYPE_MARCH;
    queueTemp.key = "";
    queueTemp.finishTime = curTime;
}

void QueueController::initLoginFinishQueues(CCDictionary* dict)
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("finishedQueue"));
    if( arr==NULL ){
        return;
    }
    
    loginFinishQueues.clear();
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        string itemId = item->valueForKey("itemId")->getCString();
        string qUuid = item->valueForKey("qUuid")->getCString();
        loginFinishQueues[qUuid] = itemId;
    }
    
    CCLOG("login finish queue end");
}

int QueueController::getQueueQidByKey(string key)
{
    map<int, QueueInfo>::iterator it;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        if((it->second).key == key)
        {
            return it->first;
        }
    }
    return 0;
}

int QueueController::getQueueQidByUuid(string uuid)
{
    map<int, QueueInfo>::iterator it;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        if((it->second).uuid == uuid)
        {
            return it->first;
        }
    }
    return 0;
}

string QueueController::getLoginFinishQueueUuId(int qType)
{
    string ret = "";
    map<string, string>::iterator it = loginFinishQueues.begin();
    for (; it != loginFinishQueues.end(); it++) {
        int qId = getQueueQidByUuid(it->first);
        if (qId > 0) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[qId];
            if (qInfo.type == qType) {
                ret = it->first;
                break;
            }
        }
    }
    return ret;
}

double QueueController::getFinishTimeByKey(string key)
{
    int qid = getQueueQidByKey(key);
    if (qid > 0)
    {
        return GlobalData::shared()->allQueuesInfo[qid].finishTime;
    }
    return 0;
}

void QueueController::openQueue(CCDictionary* dict)
{
    int qid = dict->valueForKey("qid")->intValue();
    int type = dict->valueForKey("type")->intValue();
    qid = getQID(type) + qid;
    
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end()) {
        QueueInfo info = QueueInfo(dict);
        GlobalData::shared()->allQueuesInfo[info.qid] = info;
    }
    else {
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        if (key == "") {
            addQueueInfo(dict);
        }
        else {
            addQueueInfo(dict, atoi(key.c_str()));
        }
    }
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
}

void QueueController::updateQueueInfo(int qid, double finishTime, int key)
{
    if (GlobalData::shared()->allQueuesInfo.find(qid) != GlobalData::shared()->allQueuesInfo.end()) {
        GlobalData::shared()->allQueuesInfo[qid].key = CC_ITOA(key);
        GlobalData::shared()->allQueuesInfo[qid].finishTime = finishTime;
        if (GlobalData::shared()->allQueuesInfo[qid].type == TYPE_BUILDING) {
            GlobalData::shared()->allQueuesInfo[qid].itemId = key/1000;
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_ADD);
    }
}

unsigned int QueueController::getMarchQueueQid(const string &uuid) {
    auto & info = WorldController::getInstance()->m_marchInfo[uuid];
    return getQID(TYPE_MARCH)+info.marchTag;
}

void QueueController::updateMarchQueueInfo(const string &uuid) {
    addMarchQueueInfo(uuid);
}

void QueueController::addMarchQueueInfo(const string &uuid) {
    auto & info = WorldController::getInstance()->m_marchInfo[uuid];
    if (info.ownerType != PlayerSelf /*||(info.stateType != StateMarch && info.stateType != StateReturn)*/) {
        return;
    }
    unsigned int qid = getMarchQueueQid(uuid);
    auto &queue = GlobalData::shared()->allQueuesInfo[qid];
    queue.qid = qid;
    queue.type = TYPE_MARCH;
    queue.uuid = uuid;
    string str = "";
    queue.finishTime = info.endStamp;
    queue.startTime = info.startStamp;
    auto now = WorldController::getInstance()->getTime();
    
    if (info.stateType == StateMarch) {
        auto point = WorldController::getPointByIndex(info.endPointIndex);
        str = _lang_2("108740",CC_ITOA((int)point.x),CC_ITOA((int)point.y));
        if(info.marchType == MethodRally || info.marchType == MethodUnion){
            queue.startTime = info.startStamp;
            queue.finishTime = info.endStamp;
            str = _lang_2("108740",CC_ITOA((int)point.x),CC_ITOA((int)point.y));
        }
    }
    else if (info.stateType == StateReturn) {
        str = _lang("108572");
    }
    else if (info.stateType == StateOccupy){
        if(info.marchType == MethodUnion || info.marchType == MethodRally){
            if(info.startStamp < now && (info.targetType != Tile_allianceArea || info.targetType != tile_banner)){
                str = _lang("110078");
                queue.startTime = WorldController::getInstance()->getTime();
                queue.finishTime = WorldController::getInstance()->getTime() + 1000000000 + rand() % 10000000;
                queue.type = TYPE_OCCUPY_ALLIANCE;
            }else{
                if(info.targetType == Trebuchet || info.targetType == Throne){
                    str = _lang("110078");
                }else{
                    str = _lang("115136");
                }
                queue.startTime = info.allianceTeamCreateTime;
                queue.finishTime = info.startStamp;
                queue.type = TYPE_ARMY_MASS;
            }
            
            if (info.targetType == Tile_allianceArea) {
                auto timeLeft = info.startStamp - now;
                CCLog("now is %s", CC_ITOA(now));
                CCLOG("timeLeft is %s", CC_ITOA(timeLeft));
                if (timeLeft <= 0){
                    switch (info.territoryStat) {
                        case 0: {
                            str = _lang("115305");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_BUILDING_TERRITORY;
                        }
                            break;
                        case 5: {
                            str = _lang("115310");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_REPAIR_TERRITORY;
                        }
                            break;
                        case 2: {
                            str = _lang("115346");
                            queue.startTime = WorldController::getInstance()->getTime();
                            queue.finishTime = std::numeric_limits<double>::max();
                            queue.type = TYPE_STATION_TERRITORY;
                        }
                            break;
                        case 6: {
                            str = _lang("115347");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_DESTROY_TERRITORY;
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
            else if (info.targetType == tile_banner) {
                auto timeLeft = info.startStamp - now;
                CCLog("now is %s", CC_ITOA(now));
                CCLOG("timeLeft is %s", CC_ITOA(timeLeft));
                if (timeLeft <= 0){
                    switch (info.territoryStat) {
                        case 0: {
                            str = _lang("115305");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_BUILDING_TERRITORY;
                        }
                            break;
                        case 5: {
                            str = _lang("115310");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_REPAIR_TERRITORY;
                        }
                            break;
                        case 2: {
                            str = _lang("115346");
                            queue.startTime = WorldController::getInstance()->getTime();
                            queue.finishTime = std::numeric_limits<double>::max();
                            queue.type = TYPE_STATION_TERRITORY;
                        }
                            break;
                        case 6: {
                            str = _lang("115347");
                            queue.startTime = info.territoryStartTime;
                            queue.finishTime = info.territoryFinishTime;
                            queue.type = TYPE_DESTROY_TERRITORY;
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
            
        }else if(info.targetType == ResourceTile){
            str = _lang("108735");
            queue.startTime = info.digStartTime;
            queue.finishTime = info.getDigEndTime();
            queue.type = TYPE_OCCUPY_RESOURCE;
        }else if (info.targetType == Tile_allianceArea){
             switch (info.territoryStat) {
                case 0: {
                    str = _lang("115305");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_BUILDING_TERRITORY;
                }
                    break;
                case 5: {
                    str = _lang("115310");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_REPAIR_TERRITORY;
                }
                    break;
                case 2: {
                    str = _lang("115346");
                    queue.startTime = WorldController::getInstance()->getTime();
                    queue.finishTime = std::numeric_limits<double>::max();
                    queue.type = TYPE_STATION_TERRITORY;
                }
                    break;
                case 6: {
                    str = _lang("115347");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_DESTROY_TERRITORY;
                }
                    break;
                default:
                    break;
            }
        }else if (info.targetType == tile_superMine){
            switch (info.territoryStat) {
                case 0: {
                    str = _lang("115305");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_BUILDING_TERRITORY;
                }
                    break;
                case 6: {
                    str = _lang("108735");
                    queue.startTime = info.digStartTime;
                    queue.finishTime = info.getDigEndTime();
                    queue.type = TYPE_OCCUPY_TERRITORY_RESOURCE;
                }
                    break;
                default:
                    break;
            }
        }
        else if (info.targetType == tile_tower){
            switch (info.territoryStat) {
                case 0: {
                    str = _lang("115305");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_BUILDING_TERRITORY;
                }
                    break;
                default:
                    break;
            }
        }
        else if (info.targetType == tile_wareHouse) {
            switch (info.territoryStat) {
                case 0: {
                    str = _lang("115305");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_BUILDING_TERRITORY;
                }
                    break;
                default:
                    break;
            }
        }
        else if (info.targetType == tile_banner) {
            switch (info.territoryStat) {
                case 0: {
                    str = _lang("115305");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_BUILDING_TERRITORY;
                }
                    break;
                case 5: {
                    str = _lang("115310");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_REPAIR_TERRITORY;
                }
                    break;
                case 2: {
                    str = _lang("115346");
                    queue.startTime = WorldController::getInstance()->getTime();
                    queue.finishTime = std::numeric_limits<double>::max();
                    queue.type = TYPE_STATION_TERRITORY;
                }
                    break;
                case 6: {
                    str = _lang("115347");
                    queue.startTime = info.territoryStartTime;
                    queue.finishTime = info.territoryFinishTime;
                    queue.type = TYPE_DESTROY_TERRITORY;
                }
                    break;
                default:
                    break;
            }
        }
        else if (info.targetType == MonsterTile){
            str = _lang("108736");
            queue.startTime = info.mazeStartTime;
            queue.finishTime = info.mazeEndTime;
            queue.type = TYPE_OCCUPY_MAZE;
        }else if (info.targetType == CityTile){
            str = _lang("102306");
            queue.startTime = WorldController::getInstance()->getTime();
            queue.finishTime = WorldController::getInstance()->getTime() + 1000000000 + rand() % 10000000;
            queue.type = TYPE_OCCUPY_ALLIANCE;
        }else if(info.targetType == Trebuchet || info.targetType == Throne){
            str = _lang("110078");
            queue.startTime = WorldController::getInstance()->getTime();
            queue.finishTime = std::numeric_limits<double>::max();
            queue.type = TYPE_OCCUPY_CAMP;
        }else{
            str = _lang("108737");
            queue.startTime = WorldController::getInstance()->getTime();
            queue.finishTime = std::numeric_limits<double>::max();
            queue.type = TYPE_OCCUPY_CAMP;
        }
    }
    queue.key = str;
    if(info.isKilled){
        queue.finishTime = 1;
    }

    if (WorldController::getInstance()->m_mainThreadID != pthread_self()) {
        // not in main thread , do not update texture
        WorldController::getInstance()->isRefreshQueue = true;
        return;
    }

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
}

string QueueController::addQueueInfo(CCDictionary* dict, int key)
{
    string ret = "";
    int qid = dict->valueForKey("qid")->intValue();
    int type = dict->valueForKey("type")->intValue();
    qid = getQID(type) + qid;
    
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end() || GlobalData::shared()->allQueuesInfo[qid].uuid == "") {
        if (type != QueueType::TYPE_AFFAIRS) {
            QueueInfo info = QueueInfo(dict);
            GlobalData::shared()->allQueuesInfo[info.qid] = info;
        }
    }
    
    auto tmp = dict->valueForKey("endTime")->doubleValue()/1000;
    if (tmp > 0) {
        GlobalData::shared()->allQueuesInfo[qid].endTime = GlobalData::shared()->changeTime(tmp);
    }
    
    tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
    if (tmp > 0) {
        GlobalData::shared()->allQueuesInfo[qid].finishTime = GlobalData::shared()->changeTime(tmp);
    }
    else {
        GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
    }
    
    tmp = dict->valueForKey("startTime")->doubleValue()/1000;
    if (tmp > 0) {
        GlobalData::shared()->allQueuesInfo[qid].startTime = GlobalData::shared()->changeTime(tmp);
    }
    else {
        GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
    }
    if(dict->objectForKey("isHelped")){
       GlobalData::shared()->allQueuesInfo[qid].canHelp = dict->valueForKey("isHelped")->intValue()==0;
    }
    int curTime = GlobalData::shared()->getWorldTime();
    auto obj = dict->objectForKey("itemObj");
    if (obj) {
        auto autoDict = _dict(obj);
        int itemId = autoDict->valueForKey("itemId")->intValue();
        
        if (type == TYPE_BUILDING || type == TYPE_DRAGON_BUILD) {
            //建筑队列
            if (itemId == 0) {
                GlobalData::shared()->allQueuesInfo[qid].key = "";
            }
            else
            {
                int pos = autoDict->valueForKey("pos")->intValue();
                int tk = 0;
                if (type == TYPE_BUILDING)
                {
                    tk = FunBuildController::getInstance()->getBuildKey(itemId, pos);
                    FunBuildController::getInstance()->pushBuildQueue(autoDict);
                    ret = CC_ITOA(tk);
                    
                    auto& build = FunBuildController::getInstance()->getFunbuildById(tk);
                    int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
                    string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
                    if (build.state == FUN_BUILD_CREATE || build.state==FUN_BUILD_INIT ) {
                        pushQueueNotify(qid, timeLong,  _lang_1("102116", name.c_str()));
                    }
                    else if (build.state == FUN_BUILD_UPING) {
                        pushQueueNotify(qid, timeLong,  _lang_1("102115", name.c_str()));
                    }
                }else
                {
                    tk = DragonBuildingController::getInstance()->getBuildKey(itemId, pos);
                    DragonBuildingController::getInstance()->pushBuildQueue(autoDict);
                    ret = CC_ITOA(tk);
                    
                    auto& build = DragonBuildingController::getInstance()->getFunbuildById(tk);
                    int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
                    string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
                    if (build.state == FUN_BUILD_CREATE || build.state==FUN_BUILD_INIT ) {
                        pushQueueNotify(qid, timeLong,  _lang_1("102116", name.c_str()));
                    }
                    else if (build.state == FUN_BUILD_UPING) {
                        pushQueueNotify(qid, timeLong,  _lang_1("102115", name.c_str()));
                    }
                }
                
                GlobalData::shared()->allQueuesInfo[qid].key = CC_ITOA(tk);
                GlobalData::shared()->allQueuesInfo[qid].itemId = itemId;
                
            }
        } else {
            std::string tk = getKey(itemId, type);
            GlobalData::shared()->allQueuesInfo[qid].key = tk;
            GlobalData::shared()->allQueuesInfo[qid].itemId = itemId;
            GlobalData::shared()->allQueuesInfo[qid].para = 0;
            GlobalData::shared()->allQueuesInfo[qid].allId = "";
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_ADD);
            if(type == TYPE_FORCE || type == TYPE_RIDE_SOLDIER || type == TYPE_BOW_SOLDIER || type == TYPE_CAR_SOLDIER){
                string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
                int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
                pushQueueNotify(qid, timeLong,  _lang_1("102117", name.c_str()));
            }
            if (type == TYPE_FORT) {
                string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
                int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
                pushQueueNotify(qid, timeLong,  _lang_1("102116", name.c_str()));
            }
            if (type == TYPE_SCIENCE) {
                string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
                int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
                pushQueueNotify(qid, timeLong,  _lang_1("105660", name.c_str()));
            }
            if (type == TYPE_FORGE) {
                if (autoDict->objectForKey("cost")) {
                    string cost = autoDict->valueForKey("cost")->getCString();
                    EquipmentController::getInstance()->m_curCreateCost = cost;
                }
            }
            if (type == TYPE_MATE) {
                if(autoDict->valueForKey("para")){
                    GlobalData::shared()->allQueuesInfo[qid].para = autoDict->valueForKey("para")->intValue();
                }
                if(autoDict->valueForKey("allId")){
                    GlobalData::shared()->allQueuesInfo[qid].allId = autoDict->valueForKey("allId")->getCString();
                }
                CCLOG("add create mate queue data!");
            }
            if (type == TYPE_DRAGON_BRON) {
                CCLOG("add create dragon bron queue data!");
            }
            if (type == TYPE_DRAGON_EGG) {
                CCLOG("add create dragon egg queue data!");
            }
        }
    }
    
    if (key!=0) {
        GlobalData::shared()->allQueuesInfo[qid].key = CC_ITOA(key);
        ret = CC_ITOA(key);
    }
    
    return ret;
}
void QueueController::pushQueueNotify(int type, int time, std::string body)
{
    if(CCCommonUtils::isPushNotify(1))
    {
        PushSettingInfo* pushInfo = NULL;
        for (int i=0; i<GlobalData::shared()->pushSettingVector.size(); i++) {
            pushInfo = GlobalData::shared()->pushSettingVector[i];
            if (pushInfo->type == 1) {
                break;
            }
        }
        string sound = "1";
        if (pushInfo) {
            sound = CC_ITOA(pushInfo->sound);
        }
        
        string noticeMark = "";
        if (GlobalData::shared()->allQueuesInfo.find(type) != GlobalData::shared()->allQueuesInfo.end())
        {
            int qtype = GlobalData::shared()->allQueuesInfo[type].type;
            if (qtype == TYPE_BUILDING) {
                noticeMark = "5";
            }else if (qtype == TYPE_SCIENCE) {
                noticeMark = "8";
            }else if (qtype == TYPE_FORCE || qtype == TYPE_RIDE_SOLDIER || qtype == TYPE_BOW_SOLDIER || qtype == TYPE_CAR_SOLDIER) {
                noticeMark = "6";
            }else if (qtype == TYPE_FORT) {
                noticeMark = "7";
            }
        }
        
        cocos2d::extension::CCDevice::pushNotice(type, time,  body, sound, noticeMark);
    }

}
std::string QueueController::getKey(int itemId, int indx){
    int result = itemId * 100 + indx;
    return CC_ITOA(result);
}

int QueueController::getItemId(string key)
{
    int tk = atoi(key.c_str());
    return tk/100;
}

void QueueController::startOpenQueue(string uuid)
{
    QueueOpenCommand* cmd = new QueueOpenCommand(QUEUE_OPEN_COMMAND, uuid);
    cmd->sendAndRelease();
}

void QueueController::endOpenQueue(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        return ;
    }
    openQueue(dict);
}

void QueueController::startCancelQueue(int qid)
{
    if (GlobalData::shared()->allQueuesInfo.find(qid) != GlobalData::shared()->allQueuesInfo.end()) {
        string uuid = GlobalData::shared()->allQueuesInfo[qid].uuid;
        QueueCancelCommand* cmd = new QueueCancelCommand(QUEUE_CANCEL_COMMAND, uuid);
        cmd->sendAndRelease();
    }
}

void QueueController::endCancelQueue(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        return ;
    }
    else {
        //处理相关资源
        auto qDict = _dict(dict->objectForKey("queue"));
        int qid = qDict->valueForKey("qid")->intValue();
        int type = qDict->valueForKey("type")->intValue();
        qid = getQID(type) + qid;
        
        auto resDict = _dict(dict->objectForKey("resource"));
        
        if (dict->objectForKey("goods")) {
            auto arr = dynamic_cast<CCArray*>(dict->objectForKey("goods"));
            CCDictionary* item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                ToolController::getInstance()->pushAddTool(item);
            }
        }
        
        auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
        int itemId = getItemId(qInfo.key);
        
        string tmpTitle = "";
        string tmpMsg = _lang("102288");
        int buildId = 0;
        if (qInfo.type == TYPE_SCIENCE) {
            tmpTitle = _lang("135003");
            buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
            
            int scienceId = getItemId(qInfo.key);
            GlobalData::shared()->scienceInfo.infoMap[scienceId].finishTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_STATE);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
        }
        else if (qInfo.type == TYPE_BUILDING){
            buildId = atoi(qInfo.key.c_str());
            if(FunBuildController::getInstance()->curBuildsInfo->find(buildId) != FunBuildController::getInstance()->curBuildsInfo->end()){
                auto& bInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
                if (bInfo.state == FUN_BUILD_CREATE) {
                    tmpTitle = _lang("135002");
                }else {
                    tmpTitle = _lang("135000");
                }
            }
            FunBuildController::getInstance()->cancelBuildState(buildId);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
        }
        else if (qInfo.type == TYPE_DRAGON_BUILD){
            buildId = atoi(qInfo.key.c_str());
            if(DragonBuildingController::getInstance()->curDragonBuildsInfo->find(buildId) != DragonBuildingController::getInstance()->curDragonBuildsInfo->end()){
                auto& bInfo = DragonBuildingController::getInstance()->getFunbuildById(buildId);
                if (bInfo.state == FUN_BUILD_CREATE) {
                    tmpTitle = _lang("135002");
                }else {
                    tmpTitle = _lang("135000");
                }
            }
            DragonBuildingController::getInstance()->cancelBuildState(buildId);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
        }
        else if (qInfo.type == TYPE_HOSPITAL){
            buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_HOSPITAL);
            tmpTitle = _lang("135004");
            
            ArmyController::getInstance()->cancelTreate();
        }
        else if (qInfo.type==TYPE_FORT || qInfo.type==TYPE_FORCE || qInfo.type==TYPE_RIDE_SOLDIER || qInfo.type==TYPE_BOW_SOLDIER || qInfo.type==TYPE_CAR_SOLDIER)
        {
            cocos2d::extension::CCDevice::cancelNotice(qid);
            if (qInfo.type == TYPE_FORT) {
                GlobalData::shared()->fortList[CC_ITOA(itemId)].finishTime = 0;
            }else {
                GlobalData::shared()->armyList[CC_ITOA(itemId)].finishTime = 0;
            }
            
            tmpTitle = _lang("135001");
            if (qInfo.type == TYPE_FORT){//陷阱
                tmpTitle = _lang("135002");
                buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORT);
            }
            else if (qInfo.type == TYPE_FORCE){//步
                buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_BARRACK1);
            }
            else if (qInfo.type == TYPE_RIDE_SOLDIER){//骑
                buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_BARRACK2);
            }
            else if (qInfo.type == TYPE_BOW_SOLDIER){//弓
                buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_BARRACK3);
            }
            else if (qInfo.type == TYPE_CAR_SOLDIER){//车
                buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_BARRACK4);
            }
        }
        
        auto refreshResource = [](std::string pro, CCDictionary *dict, long &resource, int buildId, std::string tmpTitle, std::string tmpMsg, WorldResourceType type){
            if(dict->objectForKey(pro)){
                int lResource = dict->valueForKey(pro)->intValue();
                if(buildId > 0 && (lResource - resource) > 0){
                    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                    if(layer) {
                        if(lResource > resource) {
                            layer->onFlyOutPut(buildId, (lResource - resource), type);
                        }
                    }
                }
                resource = lResource;
            }
        };
        refreshResource("food", resDict, GlobalData::shared()->resourceInfo.lFood, buildId, tmpTitle, tmpMsg, Food);
        refreshResource("wood", resDict, GlobalData::shared()->resourceInfo.lWood, buildId, tmpTitle, tmpMsg, Wood);
        refreshResource("stone", resDict, GlobalData::shared()->resourceInfo.lStone, buildId, tmpTitle, tmpMsg, Stone);
        refreshResource("iron", resDict, GlobalData::shared()->resourceInfo.lIron, buildId, tmpTitle, tmpMsg, Iron);
        
        if(buildId > 0){
            auto tmpInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
            string iconName = tmpInfo.pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + ".png";
            CCCommonUtils::flyHint(iconName, tmpTitle, tmpMsg);
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        
        startFinishQueue(qid,false);
    }
}

void QueueController::startCCDQueue(int qid, string itemUuid, bool isFinish, int isGold,string mark,int from, string guideStep,int itmCnt)
{
    if (GlobalData::shared()->allQueuesInfo.find(qid) != GlobalData::shared()->allQueuesInfo.end()) {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemUuid.c_str()));
        string qUuid = GlobalData::shared()->allQueuesInfo[qid].uuid;
      
        QueueCCDCommand* cmd = new QueueCCDCommand(QUEUE_CCD_COMMAND, qUuid, toolInfo.uuid, isGold);
        if(mark!="")
        {
            cmd->putParam("mark", CCString::create(mark));
        }
        if (guideStep!="") {
            cmd->putParam("guideStep", CCString::create(guideStep));
        }
        if(itmCnt>0){
            cmd->putParam("itemCount", CCInteger::create(itmCnt));
        }else if(isGold==0){
            cmd->putParam("itemCount", CCInteger::create(1));
        }
        cmd->sendAndRelease();
       
        if( isFinish ) {
            startFinishQueue(qid, false);
        }
        if (isGold>0) {
            int spend = isGold/50 + (isGold%50==0?0:1);
            if (spend>10) {
                spend = 10;
            }
            FBUtilies::appEventSpeedUp(FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level, from, spend);
        }
    }
}

void QueueController::endCCDQueue(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        return;
    }
    int qid = dict->valueForKey("qid")->intValue();
    int type = dict->valueForKey("type")->intValue();
    qid = getQID(type) + qid;
    
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    
    auto time = dict->valueForKey("updateTime")->doubleValue()/1000;
    if (time>0) {
        time = GlobalData::shared()->changeTime(time);
    }
    GlobalData::shared()->allQueuesInfo[qid].finishTime = time;
    
    auto startTime = dict->valueForKey("startTime")->doubleValue()/1000;
    if (startTime>0) {
        startTime = GlobalData::shared()->changeTime(startTime);
    }
    GlobalData::shared()->allQueuesInfo[qid].startTime = startTime;
    
    if (dict->objectForKey("remainGold")) {
        int tmpInt = dict->valueForKey("remainGold")->intValue();
        UIComponent::getInstance()->updateGold(tmpInt);
    }
    
    string key = GlobalData::shared()->allQueuesInfo[qid].key;
    
    bool finished = false;
    if (dict->objectForKey("finished")) {
        finished = dict->valueForKey("finished")->boolValue();
    }
    
    if (finished) { //fusheng 通知泰坦数据更新
        if(type == 15)
        {
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_SPEED_UP_COMPLETE,CCString::create("feed"));
            
            //fusheng android 没办法
            GlobalData::shared()->titanInfo.feedcdfix = 0;
            
            string puid = GlobalData::shared()->playerInfo.uid;
            
            CCUserDefault::sharedUserDefault()->setIntegerForKey((puid+"feedcdfix").c_str(), 0);
            

            
        }
        if(GlobalData::shared()->allQueuesInfo[qid].itemId == 400000)
        {
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_SPEED_UP_COMPLETE,CCString::create("upgrade"));//fusheng 泰坦秒cd 也认为是这个
        }
       

    }
    
    if(key == ""){
        return;
    }
 
   
    
    auto curTime = GlobalData::shared()->getWorldTime();
    if (finished) {//time <= curTime
        GlobalData::shared()->allQueuesInfo[qid].canHelp = true;
        cocos2d::extension::CCDevice::cancelNotice(qid);
        if(type == TYPE_FORCE || type == TYPE_RIDE_SOLDIER || type == TYPE_BOW_SOLDIER || type == TYPE_CAR_SOLDIER || type == TYPE_FORT){
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
            return ;
        }else if ( type != TYPE_FORGE && type != TYPE_MATE && type != TYPE_DRAGON_BRON && type != TYPE_DRAGON_EGG ){
            GlobalData::shared()->allQueuesInfo[qid].key = "";
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
        }
        
        if (type == TYPE_BUILDING) {
//            auto info = FunBuildController::getInstance()->getFunbuildById(atoi(key.c_str()));
//            info.updateTime = 0;
            int itemId = atoi(key.c_str())/1000;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("BU_%d_cd", itemId));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE,CCInteger::create(qid));
        }
        else if (type == TYPE_DRAGON_BUILD) {
            int itemId = atoi(key.c_str())/1000;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE,CCInteger::create(qid));
        }
        // tao.yu 这个判断在上面return了，神奇的代码
//        else if (type == TYPE_FORCE || type == TYPE_RIDE_SOLDIER || type == TYPE_BOW_SOLDIER || type == TYPE_CAR_SOLDIER) {
//            std::string itemId = CC_ITOA(atoi(key.c_str()) / 100);
//            GlobalData::shared()->armyList[itemId].setEndTime(0);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ARMY_DATA);
//        }
//        else if (type == TYPE_FORT) {
//            std::string itemId = CC_ITOA(atoi(key.c_str()) / 100);
//            GlobalData::shared()->fortList[itemId].setEndTime(0);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ARMY_DATA);
//        }
        else if (type == TYPE_HOSPITAL) {
            ArmyController::getInstance()->setTreatEndTime(0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
        }
//        else if (type == TYPE_AFFAIRS) {
//            AffairsController::getInstance()->changeCDTime(GlobalData::shared()->getWorldTime() - 1, NORMAL_AFFAIR);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
//        }
//        else if (type == TYPE_ALLIANCE_AFFAIRS) {
//            AffairsController::getInstance()->changeCDTime(GlobalData::shared()->getWorldTime() - 1, LEAGUE_AFFAIR);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
//        }
        else if (type == TYPE_SCIENCE) {
            auto scienceDict = _dict(dict->objectForKey("itemObj"));
            ScienceController::getInstance()->retFinishScience(scienceDict);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
        }
        else if (type == TYPE_FORGE) {
//            auto equipDict = _dict(dict->objectForKey("itemObj"));
//            EquipmentController::getInstance()->retFinishCrtEquip(equipDict);
            
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
            
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            EquipmentCreateView *curView = dynamic_cast<EquipmentCreateView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",bid,1));
            }else {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",bid,0));
            }
        }else if (type == TYPE_MATE) {
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WORKSHOP);
            ToolCreateView *curView = dynamic_cast<ToolCreateView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(curView && curView->isAniComplete()){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",bid,1));
            }else {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",bid,0));
            }
        }else if (type == TYPE_DRAGON_BRON) {
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
        }else if (type == TYPE_DRAGON_EGG) {
            GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
            GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
        }
    }
    else {
        if (type == TYPE_BUILDING || type == TYPE_DRAGON_BUILD) {
            int buildState = 0;
            string name = "";
            if (type == TYPE_BUILDING) {
                auto& info = FunBuildController::getInstance()->getFunbuildById(atoi(key.c_str()));
                info.updateTime = time;
                buildState = info.state;
                name = info.name;
            }else if (type == TYPE_DRAGON_BUILD){
                auto& info = DragonBuildingController::getInstance()->getFunbuildById(atoi(key.c_str()));
                info.updateTime = time;
                buildState = info.state;
                name = info.name;
            }
            if (buildState == FUN_BUILD_UPING) {
                cocos2d::extension::CCDevice::cancelNotice(qid);
                pushQueueNotify(qid, (time-curTime),  _lang_1("102115", _lang(name).c_str()));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
            }
            else if (buildState == FUN_BUILD_CREATE || buildState == FUN_BUILD_INIT) {
                cocos2d::extension::CCDevice::cancelNotice(qid);
                pushQueueNotify(qid, (time-curTime),  _lang_1("102116", _lang(name).c_str()));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
            }
        }
        else if (type == TYPE_FORCE || type == TYPE_RIDE_SOLDIER || type == TYPE_BOW_SOLDIER || type == TYPE_CAR_SOLDIER) {
            std::string itemId = CC_ITOA(atoi(key.c_str()) / 100);
            GlobalData::shared()->armyList[itemId].setEndTime(time);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
            string name = CCCommonUtils::getNameById(itemId);
            int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
            pushQueueNotify(qid, timeLong,  _lang_1("102117", name.c_str()));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }
        else if (type == TYPE_FORT) {
            std::string itemId = CC_ITOA(atoi(key.c_str()) / 100);
            GlobalData::shared()->fortList[itemId].setEndTime(time);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
            string name = CCCommonUtils::getNameById(itemId);
            int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
            pushQueueNotify(qid, timeLong,  _lang_1("102116", name.c_str()));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }
        else if (type == TYPE_HOSPITAL) {
            ArmyController::getInstance()->setTreatEndTime(time);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }
//        else if (type == TYPE_AFFAIRS) {
//            AffairsController::getInstance()->changeCDTime(time, NORMAL_AFFAIR);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
//        }
//        else if (type == TYPE_ALLIANCE_AFFAIRS) {
//            AffairsController::getInstance()->changeCDTime(time, LEAGUE_AFFAIR);
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
//        }
        else if (type == TYPE_SCIENCE) {
            int itemId = atoi(key.c_str()) / 100;
            GlobalData::shared()->scienceInfo.infoMap[itemId].finishTime = time;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
            
            cocos2d::extension::CCDevice::cancelNotice(qid);
            int timeLong = GlobalData::shared()->allQueuesInfo[qid].finishTime - curTime;
            string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
            pushQueueNotify(qid, timeLong,  _lang_1("105660", name.c_str()));
        }
        else if (type == TYPE_FORGE) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }else if (type == TYPE_MATE) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }else if (type == TYPE_DRAGON_EGG) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }else if (type == TYPE_DRAGON_BRON) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_TIME_UPDATE);
        }
    }
}

void QueueController::startFinishQueue(int qid, bool isSend)
{
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    string uuid = GlobalData::shared()->allQueuesInfo[qid].uuid;
    GlobalData::shared()->allQueuesInfo[qid].finishTime = 0;
    GlobalData::shared()->allQueuesInfo[qid].startTime = 0;
    GlobalData::shared()->allQueuesInfo[qid].key = "";
    GlobalData::shared()->allQueuesInfo[qid].canHelp = true;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE,CCInteger::create(qid));
    
    if(GlobalData::shared()->allQueuesInfo[qid].type == TYPE_BUILDING || GlobalData::shared()->allQueuesInfo[qid].type == TYPE_DRAGON_BUILD) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_BUILD_UPGRADE);
    }
    
    if(isSend) {
        QueueFinishCommand* cmd = new QueueFinishCommand(QUEUE_FINISH_COMMAND, uuid);
        cmd->sendAndRelease();
    }
}

void QueueController::removeQueue(int qid)
{
    map<int, QueueInfo>::iterator it = GlobalData::shared()->allQueuesInfo.find(qid);

    if (it == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    GlobalData::shared()->allQueuesInfo.erase(it);
    it->second.release();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
}

void QueueController::removeQueueByType(int type){
    map<int, QueueInfo>::iterator it;
//    vector<int> vector;
    for(it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++){
        if(it->second.type == type){
//            vector.push_back(it->first);
            it->second.finishTime = 0;
        }
    }
//    int i = 0;
//    while(i < vector.size()){
//        int index = vector[i];
//        removeQueue(index);
//        i++;
//    }
//    vector.clear();
}

void QueueController::endFinishQueue(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        return;
    }
    int qid = dict->valueForKey("qid")->intValue();
    int type = dict->valueForKey("type")->intValue();
    qid = getQID(type) + qid;
    
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    if(type == 15)
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_SPEED_UP_COMPLETE,CCString::create("feed"));//fusheng feedCD完成
        
        //fusheng android 没办法
        GlobalData::shared()->titanInfo.feedcdfix = 0;
        
        string puid = GlobalData::shared()->playerInfo.uid;
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey((puid+"feedcdfix").c_str(), 0);
        
    }
    
    if(GlobalData::shared()->allQueuesInfo[qid].itemId == 400000)
    {
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_SPEED_UP_COMPLETE,CCString::create("upgrade"));//fusheng 泰坦秒cd 也认为是这个
    }
    cocos2d::extension::CCDevice::cancelNotice(qid);
    if (type == TYPE_FORGE) {
        auto equipDict = _dict(dict->objectForKey("itemObj"));
//        EquipmentController::getInstance()->retFinishCrtEquip(equipDict);
    }else if (type == TYPE_MATE) {
        CCLOG("finish create mate tool");
    }else if (type == TYPE_DRAGON_BRON) {
        CCLOG("finish bron a dragon");
    }else if (type == TYPE_DRAGON_EGG) {
        CCLOG("finish create a dragon egg");
    }
}

int QueueController::canMakeItemByType(int type, int time_need)
{
    int ret = QID_MAX;
    auto curTime = GlobalData::shared()->getWorldTime();
    map<int, QueueInfo>::iterator it;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
//        CCLOG("for value type = %d, key = %s time = %f",(it->second).type,(it->second).key.c_str(),(it->second).finishTime );
        if((it->second).type == type && (it->second).key != "999" && ( (it->second).key == "" || (it->second).finishTime<=curTime))
        {
//            CCLOG("qid = %d",(it->second).qid);
            if ((it->second).endTime==0 || ((it->second).endTime - GlobalData::shared()->getWorldTime())>=time_need ) {
                if ((it->second).qid < ret) {
//                    CCLOG("inner qid = %d",(it->second).qid);
                    ret = (it->second).qid;
                }
            }
        }
    }
//    CCLOG("return value %d",ret);
    return ret;
}

int QueueController::getMinTimeQidByType(int type, string key)
{
    int ret = QID_MAX;
    double minTime = -1;
    auto curTime = GlobalData::shared()->getWorldTime();
    map<int, QueueInfo>::iterator it;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        if((it->second).type == type && (it->second).key != "999" && ( (it->second).key != "" || (it->second).finishTime>curTime))
        {
            if (key == "") {
                if (minTime<0 || minTime > (it->second).finishTime ) {
                    minTime = (it->second).finishTime;
                    ret = it->first;
                }
            }
            else if (key == (it->second).key) {
                return it->first;
            }
        }
    }
    return ret;
}

void QueueController::collectSolider(int qid,int buildingKey){
    map<int, QueueInfo>::iterator it =  GlobalData::shared()->allQueuesInfo.find(qid);
    if(it==GlobalData::shared()->allQueuesInfo.end()) return ;
    if (it->second.type == TYPE_BOW_SOLDIER) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_bowman);
    } else if (it->second.type == TYPE_RIDE_SOLDIER) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_cavalryman);
    } else if (it->second.type == TYPE_FORCE) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_infantry);
    } else if (it->second.type == TYPE_CAR_SOLDIER) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_chariot);
    } else {
        SoundController::sharedSound()->playEffects(Music_Sfx_harvest_soldier);
    }
    if(it->second.type == TYPE_FORCE || it->second.type == TYPE_RIDE_SOLDIER || it->second.type == TYPE_BOW_SOLDIER ||it->second.type == TYPE_CAR_SOLDIER){
        std::string itemId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
        ArmyController::getInstance()->startFinishForce(itemId, (it->second).uuid,buildingKey);
        startFinishQueue((it->second).qid, false);
    }else if((it->second).type == TYPE_FORT){
        std::string itemId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
        ArmyController::getInstance()->startFinishFort(itemId, (it->second).uuid);
        startFinishQueue((it->second).qid, false);
    }
}

int QueueController::getCanRecQidByType(int type)
{
    int ret = QID_MAX;
    map<int, QueueInfo>::iterator it;
    for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
        bool flag = false;
        double gapTime = GlobalData::shared()->getWorldTime() - it->second.finishTime + 1;
        if(it->second.finishTime<=0){
            flag = true;
        }else if (gapTime>=0){
            flag = true;
        }
        if(it->second.type==1){
//            CCLOG("getCanRecQidByType gapTime=%d  it->second.qid=%d it->first=%d key=%s",gapTime,it->second.qid,it->first,it->second.key.c_str());
        }
        if((it->second).type == type && (it->second).key != "999" && (it->second).key != "" && flag)
        {
            ret = it->first;
            break;
        }
    }
    return ret;
}

void QueueController::rentQueue(int qid, int time, bool oneCmdQueue)
{
    int tmpNum = ceil(time*1.0/FunBuildController::getInstance()->building_base_k8);
    int sumGold = tmpNum*FunBuildController::getInstance()->building_base_k9;
    int sumTime = tmpNum*FunBuildController::getInstance()->building_base_k8;
    if (GlobalData::shared()->playerInfo.gold < sumGold) {
        YesNoDialog::gotoPayTips();
        return;
    }
    
    if (GlobalData::shared()->allQueuesInfo.find(qid) == GlobalData::shared()->allQueuesInfo.end()) {
        return;
    }
    string uuid = GlobalData::shared()->allQueuesInfo[qid].uuid;
    m_waitInterface= GameController::getInstance()->showWaitInterface(UIComponent::getInstance()->m_buildNode);
    m_waitInterface->setPosition(50, -70);
    QueueRentCommand* cmd = new QueueRentCommand(uuid, sumTime);
    if(oneCmdQueue){
        cmd->setCheckType(CHECK_CMD_PARAM);
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(uuid), "qUUID");
        cmd->setCheckDict(dict);
    }
    cmd->sendAndRelease();
}

void QueueController::ResetNoticeParam(int open, int sound)
{
    int curTime = GlobalData::shared()->getWorldTime();
    map<int, QueueInfo>::iterator it = GlobalData::shared()->allQueuesInfo.begin();
    for (; it!= GlobalData::shared()->allQueuesInfo.end(); it++)
    {
        cocos2d::extension::CCDevice::cancelNotice(it->first);
        auto& qInfo = it->second;
        if (open == 1 && qInfo.key!="" && qInfo.finishTime>(curTime+60))
        {
            int qid = it->first;
            if (qInfo.type == TYPE_BUILDING || qInfo.type == TYPE_DRAGON_BUILD) {
                string name = "";
                int bState = 0;
                if (qInfo.type == TYPE_BUILDING) {
                    auto& info = FunBuildController::getInstance()->getFunbuildById(atoi(qInfo.key.c_str()));
                    bState = info.state;
                    name = info.name;
                }else {
                    auto& info = DragonBuildingController::getInstance()->getFunbuildById(atoi(qInfo.key.c_str()));
                    bState = info.state;
                    name = info.name;
                }
                if (bState == FUN_BUILD_UPING) {
                    int timeLong = qInfo.finishTime - curTime;
                    cocos2d::extension::CCDevice::pushNotice(qid, timeLong,  _lang_1("102115", _lang(name).c_str()), CC_ITOA(sound), "5");
                }
                else if (bState == FUN_BUILD_CREATE || bState == FUN_BUILD_INIT) {
                    int timeLong = qInfo.finishTime - curTime;
                    cocos2d::extension::CCDevice::pushNotice(qid, timeLong,  _lang_1("102116", _lang(name).c_str()), CC_ITOA(sound), "5");
                }
            }
            else if (qInfo.type == TYPE_SCIENCE) {
                int timeLong = qInfo.finishTime - curTime;
                string name = CCCommonUtils::getNameById(CC_ITOA(qInfo.itemId));
                cocos2d::extension::CCDevice::pushNotice(qid, timeLong,  _lang_1("105660",name.c_str()), CC_ITOA(sound), "8");
            }
            else if (qInfo.type == TYPE_FORCE || qInfo.type == TYPE_RIDE_SOLDIER || qInfo.type == TYPE_BOW_SOLDIER || qInfo.type == TYPE_CAR_SOLDIER) {
                std::string itemId = CC_ITOA(atoi(qInfo.key.c_str()) / 100);
                string name = CCCommonUtils::getNameById(itemId);
                int timeLong = qInfo.finishTime - curTime;
                cocos2d::extension::CCDevice::pushNotice(qid, timeLong,  _lang_1("102117", name.c_str()), CC_ITOA(sound), "6");
            }
            else if (qInfo.type == TYPE_FORT) {
                std::string itemId = CC_ITOA(atoi(qInfo.key.c_str()) / 100);
                string name = CCCommonUtils::getNameById(itemId);
                int timeLong = qInfo.finishTime - curTime;
                cocos2d::extension::CCDevice::pushNotice(qid, timeLong,  _lang_1("102116", name.c_str()), CC_ITOA(sound), "7");
            }
        }
    }
}

void QueueController::autoHavestEquip(int qid, int buildingKey)
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
    if (qInfo.key != "") {
        EquipmentController::getInstance()->startFinishCrtEquip(qInfo.uuid);
//        QueueController::getInstance()->startFinishQueue(qInfo.qid, false);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(buildingKey));
    }
}

void QueueController::autoHavestMateTool(int qid, int buildingKey)
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
    if (qInfo.key != "") {
        ToolController::getInstance()->getMakeTool(qInfo.uuid);
        QueueController::getInstance()->startFinishQueue(qInfo.qid, false);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(buildingKey));
    }
}

void QueueController::refreshMateQueue(int buildingKey){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(buildingKey));
    QueueRefreshCommand* cmd = new QueueRefreshCommand(QUEUE_MATE_REFRESH_COMMAND);
    cmd->sendAndRelease();
}

void QueueController::endRefreshMateQueue(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        return;
    }
    if (dict->valueForKey("updateTime")->longValue()>0) {
        string key = QueueController::getInstance()->addQueueInfo(dict);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MATETOOL_CREATE_END);
    
}
