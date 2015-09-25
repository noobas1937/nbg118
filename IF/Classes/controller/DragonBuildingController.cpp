//
//  DragonBuildingController.cpp
//  IF
//
//  Created by 张军 on 15/6/5.
//
//


#include "DragonBuildingController.h"
#include "LocalController.h"
#include "UIComponent.h"
#include "FunBuildCommand.h"
#include "FunBuildAllCommand.h"
#include "FunBuildInfo.h"
#include "SceneController.h"
#include "../Ext/CCDevice.h"
#include "QueueController.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "GuideController.h"
#include "QuestController.h"
#include "ArmyController.h"
#include "VipUtil.h"
#include "ImperialScene.h"
#include "WorldController.h"
#include "CCMathUtils.h"
#include "AppLibHelper.h"
#include "MoveBuildView.h"
#include "PopupViewController.h"
#include "EvaluationView.h"
#include "DailyActiveController.h"

static DragonBuildingController *_instance = NULL;

DragonBuildingController::DragonBuildingController()
{
    curFunBuildId = 0;
    isGetAllFunBuilds = false;
    oldScale = 1.0;
    oldx = 99999;
    oldy = 99999;
    willOpenOrUpId = 0;
    isFirst = false;
    isError = false;
    addTime = 0;
    
//    WoodOutPreSed = 0;
    WoodOdd = 0;
//    FoodOutPreSed = 0;
//    curFoodOutPreSed = 0;
    FoodOdd = 0;
//    StoneOutPreSed = 0;
    StoneOdd = 0;
//    IronOutPreSed = 0;
    IronOdd = 0;
//    SliverOutPreSed = 0;
    SliverOdd = 0;
    
    lastAddWood = 0;
    lastAddStone = 0;
    lastAddIron = 0;
    lastAddFood = 0;
    
    dataConfig_freeSpdT = 0;
    
    building_base_k1 = 0;
    
    OpenMoveBuildStat = false;
    willMoveToBuildType = 0;
    
    //初始化DragonBuildingController
    curDragonBuildsInfo = &(GlobalData::shared()->dragonBuildingInfo);
    curUpDragonBuildsInfo = &(GlobalData::shared()->UpDragonBuildsInfo);
    //地块
    auto posictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("dragon_postition");
    if(posictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(posictInfo, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            int pos = dictInfo->valueForKey("position")->intValue();
            string value = dictInfo->valueForKey("building")->getCString();
            posToTypeMap[pos] = value;
            
            auto tileInfo = TileInfo(dictInfo);
            m_tileMap[pos] = tileInfo;
        }
    }
    
    //龙食物
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("dragon_food");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            int foodId=dictInfo->valueForKey("id")->intValue();
            dragonFoodIds.push_back(foodId);
        }
    }
}

DragonBuildingController::~DragonBuildingController() {
}

DragonBuildingController* DragonBuildingController::getInstance() {
    if (!_instance) {
        _instance = new DragonBuildingController();
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DragonBuildingController::updatePreSed), _instance, 1.0,kCCRepeatForever, 0.0f, false);
    }
    return _instance;
}

void DragonBuildingController::purgeData() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DragonBuildingController::updatePreSed), _instance);
    delete _instance;
    _instance = NULL;
}

string DragonBuildingController::getBuildByPos(int pos)
{
    string ret = "";
    if (posToTypeMap.find(pos) != posToTypeMap.end()) {
        ret = posToTypeMap[pos];
    }
    return ret;
}

int DragonBuildingController::getBuildKey(int type, int pos)
{
    return type*1000+pos;
}

void DragonBuildingController::getDataFromServer()
{
//    FunBuildAllCommand * cmd=new FunBuildAllCommand(PALACE_SHOW_COMMAND);
//    cmd->sendAndRelease();
}

int DragonBuildingController::getUpRoCrtNum()
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).itemId > 0 && ((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_DESTROY))
        {
            auto curTime = GlobalData::shared()->getWorldTime();
            if ( (it->second).updateTime > curTime)
            {
                ret = (it->second).itemId;
                break;
            }
        }
    }
    
    return ret;
}

void DragonBuildingController::initOpenTile(string tiles)
{
    vector<string> openTiles;
    CCCommonUtils::splitString(tiles, ",", openTiles);
    for (int i=0; i<openTiles.size(); i++) {
        int id = atoi(openTiles[i].c_str());
        m_bigTileMap[id].state = FUN_BUILD_ULOCK;
        m_bigTileMap[id].unlock = 1;
    }
    
    map<int, BigTileInfo>::iterator it;
    for(it=m_bigTileMap.begin(); it != m_bigTileMap.end(); ++it)
    {
        if (it->second.state == FUN_BUILD_ULOCK) {
            vector<string> vecPosIds;
            CCCommonUtils::splitString(it->second.positions, ";", vecPosIds);
            for (int i=0; i<vecPosIds.size(); i++) {
                int id = atoi(vecPosIds[i].c_str());
                m_tileMap[id].state = FUN_BUILD_ULOCK;
                m_tileMap[id].unlock = 1;
            }
        }
    }
}

void DragonBuildingController::refreshResTime(CCDictionary* dict)//城市被攻击后，资源田设置被掠夺后的资源
{
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("refreshBuildings"));
    if( arr==NULL ){
        return;
    }
    
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int id = item->valueForKey("itemId")->intValue();
        int pos = item->valueForKey("pos")->intValue();
        int time = item->valueForKey("refreshTime")->doubleValue()/1000;
        if (time>0) {
            time = GlobalData::shared()->changeTime(time);
        }
        int key = getBuildKey(id,pos);
        
        if(curDragonBuildsInfo->find(key) != curDragonBuildsInfo->end())
        {
            auto& tmpInfo = (*curDragonBuildsInfo)[key];
            tmpInfo.refreshTime = time;
        }
    }
//    updateHavestPush();
}

void DragonBuildingController::retDataFromServer(CCDictionary* dict)
{
    isGetAllFunBuilds = true;
    
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("dragonBuilding"));
    if( arr==NULL ){
        GameController::getInstance()->removeWaitInterface();
        return;
    }
    
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int id = item->valueForKey("itemId")->intValue();
        int pos = item->valueForKey("pos")->intValue();
        int level = item->valueForKey("level")->intValue();
        int key = getBuildKey(id,pos);
        
        CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(id));
        FunBuildInfo info;// = FunBuildInfo(xmlDictInfo);
        if (GlobalData::shared()->dragonBuildingInfo.find(key) != GlobalData::shared()->dragonBuildingInfo.end()) {
            info = (GlobalData::shared()->dragonBuildingInfo)[key];
            info.SetInfoFromXml(xmlDictInfo);
        }else {
            info = FunBuildInfo(xmlDictInfo);
        }
        
        info.SetInfoFromServer(item);
        info.itemId = key;
        
        //分析提取引导数据
//        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
//        string GuideBuildKey = "";
//        if (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0)
//        {
//            if(gFake=="" || gFake=="start_1")//提取4个建筑数据
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_1");
//                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_WOOD && pos==26 && level==1) {//伐木场
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_BARRACK2 && pos==11 && level==1) {//骑兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_BARRACK1 && pos==10 && level==1) {//步兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//            }
//            else if(gFake=="end_1" || gFake=="start_2")//提取3个建筑数据
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_2");
//                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_BARRACK2 && pos==11 && level==1) {//骑兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_BARRACK1 && pos==10 && level==1) {//步兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//            }
//            else if(gFake=="end_2" || gFake=="start_3")//提取2个建筑数据
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_3");
//                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_BARRACK1 && pos==10 && level==1) {//步兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//            }
//            else if(gFake=="end_3" || gFake=="start_4")//提取1个建筑数据
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_4");
//                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_BARRACK1 && pos==10 && level==1) {//步兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//            }
//        }
        //分析提取引导数据结束
        
        info.updateTime = QueueController::getInstance()->getFinishTimeByKey(CC_ITOA(key)); //队列功能后台返回后 执行
        
        if (info.updateTime != 0) {
            if (info.level==0) {
                info.state = FUN_BUILD_CREATE;
                //                UIComponent::getInstance()->buildQueueView->SetInfo(key);
            }
            else
            {
                if(info.severST == 2){
                    info.state = FUN_BUILD_DESTROY;
                }
                else {
                    info.state = FUN_BUILD_UPING;
                }
                //                UIComponent::getInstance()->buildQueueView->SetInfo(key);
            }
        }
        else
        {
            info.state = FUN_BUILD_NORMAL;
        }
        
        (GlobalData::shared()->dragonBuildingInfo)[key] = info;
    }
    
    updateAllOutPut();
//    updateHavestPush();
//    updateLimitNum();//更新农场，栖息地建筑上限
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRANSITION_FINISHED);
}

void DragonBuildingController::retSetData(CCDictionary* dict)
{
    isError = false;
    //特殊情况， 主城升级错误，导致 开启建筑 会 发生错误。(目前，主城升级错误，不会触发开启新建筑)
    GlobalData::shared()->playerInfo.gold = dict->valueForKey("gold")->intValue();
    auto cityInfo = _dict(dict->objectForKey("resource"));
    GlobalData::shared()->resourceInfo.lWood = cityInfo->valueForKey("wood")->intValue();
    GlobalData::shared()->resourceInfo.lIron = cityInfo->valueForKey("iron")->intValue();
    GlobalData::shared()->resourceInfo.lStone = cityInfo->valueForKey("stone")->intValue();
    GlobalData::shared()->resourceInfo.lFood = cityInfo->valueForKey("food")->intValue();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("building"));
    if( arr==NULL ){
        GameController::getInstance()->removeWaitInterface();
        return;
    }
    
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        int tid = item->valueForKey("itemId")->intValue();
        int pos = item->valueForKey("pos")->intValue();
        int id = getBuildKey(tid, pos);
        
        if(curDragonBuildsInfo->find(id) != curDragonBuildsInfo->end())
        {
            auto& tmpInfo = (*curDragonBuildsInfo)[id];
            if(tmpInfo.isError)
            {
                tmpInfo.isError = 0;
                tmpInfo.SetInfoFromServer(item);
                if (tmpInfo.updateTime != 0)
                {//前端 未 显示正在建造或者升级   后端显示在建造或升级
                    if (tmpInfo.state==FUN_BUILD_NORMAL)
                    {
                        if (tmpInfo.level==0) {//此中情况不可能发生
                            tmpInfo.state = FUN_BUILD_CREATE;
                        }
                        else
                        {//此中情况不可能发生
                            tmpInfo.state = FUN_BUILD_UPING;
                        }
                    }
                }
                else
                {//前端显示正在建造或者升级   后端显示 未 在建造或升级
                    if (tmpInfo.state==FUN_BUILD_INIT || tmpInfo.state == FUN_BUILD_CREATE)
                    {//此建筑信息 不会存在于 arr中,这是未开始创建的建筑
                        tmpInfo.state = FUN_BUILD_CREATE_ERROR;
                    }
                    else if (tmpInfo.state==FUN_BUILD_UPING)
                    {
                        tmpInfo.state = FUN_BUILD_UPING_ERROR;
                    }
                    else if (tmpInfo.state==FUN_BUILD_NORMAL)
                    {//更新级别信息
                        tmpInfo.state = FUN_BUILD_NORMAL_ERROR;
                    }
                }
            }
        }
    }
    
    map<int, FunBuildInfo>::iterator it;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).itemId > 0)
        {
            if ((it->second).isError && ((it->second).state==FUN_BUILD_INIT || (it->second).state == FUN_BUILD_CREATE || (it->second).level==1))
            {
                (it->second).level = 0;
                (it->second).state = FUN_BUILD_CREATE_ERROR;
            }
        }
    }
    updateAllOutPut();
}

bool DragonBuildingController::startOpenTile(int itemId, int gold)
{
    auto& info = m_bigTileMap[itemId];
    if (gold<=0) {
        if (GlobalData::shared()->resourceInfo.lWood<info.wood_need
            || GlobalData::shared()->resourceInfo.lFood<info.food_need
            || GlobalData::shared()->resourceInfo.lStone<info.stone_need
            || GlobalData::shared()->resourceInfo.lIron<info.iron_need) {
            CCCommonUtils::flyText(_lang("102197"));
            return false;
        }
    }
    if (gold>0) {
        //判断金币数量是否足够
        if (GlobalData::shared()->playerInfo.gold < gold) {
            YesNoDialog::gotoPayTips();
            return false;
        }
    }
    
    GlobalData::shared()->resourceInfo.lWood -= info.wood_need;//*(1-GlobalData::shared()->scienceEffect["207"]*1.0/100);
    GlobalData::shared()->resourceInfo.lFood -= info.food_need;
    GlobalData::shared()->resourceInfo.lStone -= info.stone_need;
    GlobalData::shared()->resourceInfo.lIron -= info.iron_need;
    GlobalData::shared()->resourceInfo.lWood = GlobalData::shared()->resourceInfo.lWood<0?0:GlobalData::shared()->resourceInfo.lWood;
    GlobalData::shared()->resourceInfo.lFood = GlobalData::shared()->resourceInfo.lFood<0?0:GlobalData::shared()->resourceInfo.lFood;
    GlobalData::shared()->resourceInfo.lStone = GlobalData::shared()->resourceInfo.lStone<0?0:GlobalData::shared()->resourceInfo.lStone;
    GlobalData::shared()->resourceInfo.lIron = GlobalData::shared()->resourceInfo.lIron<0?0:GlobalData::shared()->resourceInfo.lIron;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    
    int paramGold = gold>0?1:0;
    TileOpenCommand * cmd=new TileOpenCommand(itemId, PALACE_OPEN_TILE, paramGold);
    cmd->sendAndRelease();
    curFunBuildId = itemId;
    
    if (isError) {
        getDataFromServer();
    }
    return true;
}

void DragonBuildingController::endOpenTile(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        //        (utf_string) c: tile.open
        //        (sfs_object) p:
        //		(int) food: 35171
        //		(int) silver: 20100
        //		(int) iron: 18547
        //		(int) wood: 16592
        //		(int) stone: 16592
        if (dict->objectForKey("gold")) {
            int tmpInt = dict->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        GlobalData::shared()->resourceInfo.lFood = dict->valueForKey("food")->intValue();
        GlobalData::shared()->resourceInfo.lWood = dict->valueForKey("wood")->intValue();
        GlobalData::shared()->resourceInfo.lStone = dict->valueForKey("stone")->intValue();
        GlobalData::shared()->resourceInfo.lIron = dict->valueForKey("iron")->intValue();
        
        m_bigTileMap[curFunBuildId].state = FUN_BUILD_ULOCK;
        m_bigTileMap[curFunBuildId].unlock = 1;
        vector<string> vecPosIds;
        CCCommonUtils::splitString(m_bigTileMap[curFunBuildId].positions, ";", vecPosIds);
        for (int i=0; i<vecPosIds.size(); i++) {
            int id = atoi(vecPosIds[i].c_str());
            m_tileMap[id].state = FUN_BUILD_ULOCK;
            m_tileMap[id].unlock = 1;
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UNLOCK_TILE, CCInteger::create(id));
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UNLOCK_TILE, CCInteger::create(curFunBuildId));
    }
}

bool DragonBuildingController::startOpenFunBuild(int itemId, int pos, int gold, bool isForce)
{
    if (itemId<=0) {
        return false;
    }
    int key = getBuildKey(itemId,pos);
    CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(itemId));
    auto info = FunBuildInfo(xmlDictInfo);
    int tmpTime = info.time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD, tmpTime);
    if(QID_MAX == qid && gold<=0){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
//    if (gold<=0) {
//        if (GlobalData::shared()->resourceInfo.lWood<info.wood_need
//            || GlobalData::shared()->resourceInfo.lFood<info.food_need
//            || GlobalData::shared()->resourceInfo.lStone<info.stone_need
//            || GlobalData::shared()->resourceInfo.lIron<info.iron_need) {
//            CCCommonUtils::flyText(_lang("102197"));
//            return false;
//        }
//    }
    
//    if(!info.isUnLock()) {
//        CCCommonUtils::flyText(_lang("102196"));
//        return false;
//    }
    
    if (gold>0) {
        //判断金币数量是否足够
        if (GlobalData::shared()->playerInfo.gold < gold) {
            YesNoDialog::gotoPayTips();
            return false;
        }
    }
    
    string tool_need = xmlDictInfo->valueForKey("item")->getCString();
    std::vector<string> toolItems;
    CCCommonUtils::splitString(tool_need, "|", toolItems);
    for (int i=0; i<toolItems.size(); i++) {
        string curTool = toolItems[i];
        std::vector<string> item;
        CCCommonUtils::splitString(curTool, ";", item);
        int id = atoi(item[0].c_str());
        int num = atoi(item[1].c_str());
        auto& tool = ToolController::getInstance()->getToolInfoById(id);
        if(num > tool.getCNT() && gold<=0)
        {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }else {
            if (num > tool.getCNT() && gold>0) {
                ToolController::getInstance()->useTool(tool.itemId, tool.getCNT(), false);
            }
            else {
                ToolController::getInstance()->useTool(tool.itemId, num, false);
            }
        }
    }
    
    info.itemId = key;
    info.pos = pos;
    info.state = FUN_BUILD_INIT;
    auto curTime = GlobalData::shared()->getWorldTime();
    int lt = tmpTime;
    info.updateTime = curTime+lt;
    (GlobalData::shared()->dragonBuildingInfo)[key] = info;
//
//    GlobalData::shared()->resourceInfo.lWood -= info.wood_need;//*(1-GlobalData::shared()->scienceEffect["207"]*1.0/100);
//    GlobalData::shared()->resourceInfo.lFood -= info.food_need;
//    GlobalData::shared()->resourceInfo.lStone -= info.stone_need;
//    GlobalData::shared()->resourceInfo.lIron -= info.iron_need;
//    GlobalData::shared()->resourceInfo.lWood = GlobalData::shared()->resourceInfo.lWood<0?0:GlobalData::shared()->resourceInfo.lWood;
//    GlobalData::shared()->resourceInfo.lFood = GlobalData::shared()->resourceInfo.lFood<0?0:GlobalData::shared()->resourceInfo.lFood;
//    GlobalData::shared()->resourceInfo.lStone = GlobalData::shared()->resourceInfo.lStone<0?0:GlobalData::shared()->resourceInfo.lStone;
//    GlobalData::shared()->resourceInfo.lIron = GlobalData::shared()->resourceInfo.lIron<0?0:GlobalData::shared()->resourceInfo.lIron;
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//    
    int paramGold = gold>0?1:0;
    if (isForce) {
        paramGold = 2;
    }
    FunBuildCreate * cmd=new FunBuildCreate(itemId, pos, DRAGON_CT_COMMAND, paramGold);
    cmd->sendAndRelease();
    QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
    curFunBuildId = key;
    
//    info.is_Dirc = false;
    if(gold<=0 || isForce) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_TIME_BAR, CCInteger::create(curFunBuildId));
    }
    else {
//        info.is_Dirc = true;
    }
    
    if (isError) {
        getDataFromServer();
    }
    
    return true;
}

void DragonBuildingController::endOpenFunBuild(CCDictionary* dict, int type)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
        
        int itemId = curFunBuildId;
        if (curUpDragonBuildsInfo->find(itemId)==curUpDragonBuildsInfo->end()) {
            auto& info = (*curDragonBuildsInfo)[itemId];
            info.isError = 1;
//            if(itemId/1000 != FUN_BUILD_MAIN)
//            {
//                info.level += 1;
//            }
            info.state = FUN_BUILD_NORMAL;
            onCreateOrUpError();
        }
    }
    else
    {
        if (type==1 && dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
            
            int itemId = dict->valueForKey("itemId")->intValue();
            int pos = dict->valueForKey("pos")->intValue();
            int tk = DragonBuildingController::getInstance()->getBuildKey(itemId, pos);
            DragonBuildingController::getInstance()->pushBuildQueue(dict);
            (*curDragonBuildsInfo)[tk].state = FUN_BUILD_CHANGE;
            (*curDragonBuildsInfo)[tk].updateTime = GlobalData::shared()->getWorldTime();
            (*curDragonBuildsInfo)[tk].uuid = dict->valueForKey("uuid")->getCString();
            (*curDragonBuildsInfo)[tk].is_Dirc = false;
        }
        else {
            if(dict->objectForKey("remainGold")) {
                int tmpInt = dict->valueForKey("remainGold")->intValue();
                UIComponent::getInstance()->updateGold(tmpInt);
            }
            
            auto tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
            if (tmp > 0) {
                tmp = GlobalData::shared()->changeTime(tmp);
            }
            
            string key = QueueController::getInstance()->addQueueInfo(dict);
            int id = atoi(key.c_str());
            if (curDragonBuildsInfo->find(id) != curDragonBuildsInfo->end()) {
                auto &resInfo = curDragonBuildsInfo->at(id);
                resInfo.updateTime = tmp;
                resInfo.uuid = (*curUpDragonBuildsInfo)[id].uuid;
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("BC_%d_build", id/1000));
        }
    }
    curFunBuildId = 0;
}

bool DragonBuildingController::startUpFunBuild(int itemId, int gold, bool isForce)
{
    if(curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        CCLOG("not exist %d", itemId);
        return false;
    }
    auto &info = (*curDragonBuildsInfo)[itemId];
    if (info.uuid == "") {
        getDataFromServer();
        return false;
    }
    
    //不能升级的情况
//    if(info.type == FUN_BUILD_HOSPITAL) {
//        if (ArmyController::getInstance()->getTotalTreat()>0) {
//            CCCommonUtils::flyText(_lang("102158"));
//            return false;
//        }
//    }
//    else if(info.type == FUN_BUILD_SCIENE) {
//        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
//        if (qid != QID_MAX) {
//            string key = GlobalData::shared()->allQueuesInfo[qid].key;
//            int scienceId = QueueController::getInstance()->getItemId(key);
//            string scienceName = CCCommonUtils::getNameById(CC_ITOA(scienceId));
//            CCCommonUtils::flyText(_lang_1("102157", scienceName.c_str()));
//            return false;
//        }
//    }
//    else if(info.type == FUN_BUILD_FORGE) {
//        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
//        if (qid != QID_MAX) {
//            string key = GlobalData::shared()->allQueuesInfo[qid].key;
//            int equpId = QueueController::getInstance()->getItemId(key);
//            string equpName = CCCommonUtils::getNameById(CC_ITOA(equpId));
//            CCCommonUtils::flyText(_lang_1("102157", equpName.c_str()));
//            return false;
//        }
//    }
//    else if(info.type == FUN_BUILD_WORKSHOP) {
//        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
//        if (qid != QID_MAX) {
//            string key = GlobalData::shared()->allQueuesInfo[qid].key;
//            int mateId = QueueController::getInstance()->getItemId(key);
//            string mateName = CCCommonUtils::getNameById(CC_ITOA(mateId));
//            CCCommonUtils::flyText(_lang_1("102157", mateName.c_str()));
//            return false;
//        }
//    }
//    else if(info.type == FUN_BUILD_BARRACK1 || info.type == FUN_BUILD_BARRACK2 || info.type == FUN_BUILD_BARRACK3 || info.type == FUN_BUILD_BARRACK4) {
//        int type = TYPE_FORCE;
//        if (info.type == FUN_BUILD_BARRACK2) {
//            type = TYPE_RIDE_SOLDIER;
//        }
//        else if (info.type == FUN_BUILD_BARRACK3) {
//            type = TYPE_BOW_SOLDIER;
//        }
//        else if (info.type == FUN_BUILD_BARRACK4) {
//            type = TYPE_CAR_SOLDIER;
//        }
//        int qid = QueueController::getInstance()->getMinTimeQidByType(type);
//        if (qid != QID_MAX) {
//            string key = GlobalData::shared()->allQueuesInfo[qid].key;
//            int armyId = QueueController::getInstance()->getItemId(key);
//            string armyName = CCCommonUtils::getNameById(CC_ITOA(armyId));
//            CCCommonUtils::flyText(_lang_2("102156", _lang(info.name).c_str(), armyName.c_str()));
//            return false;
//        }
//    }
    
    int tmpTime = info.time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD, tmpTime);
    if(QID_MAX == qid && gold==0){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return false;
    }
    
//    if(gold<=0){
//        if (GlobalData::shared()->resourceInfo.lWood<info.wood_need
//            || GlobalData::shared()->resourceInfo.lFood<info.food_need
//            || GlobalData::shared()->resourceInfo.lStone<info.stone_need
//            || GlobalData::shared()->resourceInfo.lIron<info.iron_need) {
//            CCCommonUtils::flyText(_lang("102197"));
//            return false;
//        }
//    }
    
//    if(!info.isUnLock()) {
//        CCCommonUtils::flyText(_lang("102196"));
//        return false;
//    }
    
    if (gold>0) {
        //判断金币数量是否足够
        if (GlobalData::shared()->playerInfo.gold < gold) {
            YesNoDialog::gotoPayTips();
            return false;
        }
    }
    
    std::vector<string> toolItems;
    CCCommonUtils::splitString(info.tool_need, "|", toolItems);
    for (int i=0; i<toolItems.size(); i++) {
        string curTool = toolItems[i];
        std::vector<string> item;
        CCCommonUtils::splitString(curTool, ";", item);
        int id = atoi(item[0].c_str());
        int num = atoi(item[1].c_str());
        auto& tool = ToolController::getInstance()->getToolInfoById(id);
        if(num > tool.getCNT() && gold<=0)
        {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }else {
            if (num > tool.getCNT() && gold>0) {
                ToolController::getInstance()->useTool(tool.itemId, tool.getCNT(), false);
            }
            else {
                ToolController::getInstance()->useTool(tool.itemId, num, false);
            }
        }
    }
    
    info.state = FUN_BUILD_UPING;
    
    auto curTime = GlobalData::shared()->getWorldTime();
    int lt = tmpTime;
    info.updateTime = curTime+lt;
    
//    GlobalData::shared()->resourceInfo.lWood -= info.wood_need;//*(1-GlobalData::shared()->scienceEffect["207"]*1.0/100);
//    GlobalData::shared()->resourceInfo.lFood -= info.food_need;
//    GlobalData::shared()->resourceInfo.lStone -= info.stone_need;
//    GlobalData::shared()->resourceInfo.lIron -= info.iron_need;
//    GlobalData::shared()->resourceInfo.lWood = GlobalData::shared()->resourceInfo.lWood<0?0:GlobalData::shared()->resourceInfo.lWood;
//    GlobalData::shared()->resourceInfo.lFood = GlobalData::shared()->resourceInfo.lFood<0?0:GlobalData::shared()->resourceInfo.lFood;
//    GlobalData::shared()->resourceInfo.lStone = GlobalData::shared()->resourceInfo.lStone<0?0:GlobalData::shared()->resourceInfo.lStone;
//    GlobalData::shared()->resourceInfo.lIron = GlobalData::shared()->resourceInfo.lIron<0?0:GlobalData::shared()->resourceInfo.lIron;
//    
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    
    int paramGold = gold>0?1:0;
    if (isForce) {
        paramGold = 2;
    }
    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, DRAGON_UP_COMMAND, paramGold);
    cmd->sendAndRelease();
    QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
    curFunBuildId = itemId;
    
//    if(curFunBuildId == DBUILDING_HATCHER_ItemId){
//        AppLibHelper::triggerEventMainBuildingLevel(info.level+1);
//    }
    
    info.is_Dirc = false;
    if (gold<=0 || isForce) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_TIME_BAR, CCInteger::create(curFunBuildId));
    }
    else {
        info.is_Dirc = true;
    }
    
    if (isError) {
        getDataFromServer();
    }
    return true;
}

void DragonBuildingController::endUpFunBuild(CCDictionary* dict, int type)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
        
        int itemId = curFunBuildId;
        if (curUpDragonBuildsInfo->find(itemId)==curUpDragonBuildsInfo->end()) {
            auto& info = (*curDragonBuildsInfo)[itemId];
            info.isError = 1;
//            if(itemId/1000 != FUN_BUILD_MAIN)
//            {
//                info.level += 1;
//            }
            info.state = FUN_BUILD_NORMAL;
            onCreateOrUpError();
        }
    }
    else
    {
        if (type==1 && dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
            
            int itemId = dict->valueForKey("itemId")->intValue();
            int pos = dict->valueForKey("pos")->intValue();
            int tk = DragonBuildingController::getInstance()->getBuildKey(itemId, pos);
            DragonBuildingController::getInstance()->pushBuildQueue(dict);
            (*curDragonBuildsInfo)[tk].state = FUN_BUILD_UPING_END;
            (*curDragonBuildsInfo)[tk].is_Dirc = false;
            (*curDragonBuildsInfo)[tk].updateTime = GlobalData::shared()->getWorldTime();
        }
        else {
            if(dict->objectForKey("remainGold")) {
                int tmpInt = dict->valueForKey("remainGold")->intValue();
                UIComponent::getInstance()->updateGold(tmpInt);
            }
            
            auto tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
            if (tmp > 0) {
                tmp = GlobalData::shared()->changeTime(tmp);
            }
            string key = QueueController::getInstance()->addQueueInfo(dict);
            int id = atoi(key.c_str());
            (*curDragonBuildsInfo)[id].updateTime = tmp;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE,
                                                                                   CCString::createWithFormat("BC_%d_up", id / 1000));
        }
    }
    curFunBuildId = 0;
}

bool DragonBuildingController::startDestroyBuild(int itemId)
{
    auto &info = (*curDragonBuildsInfo)[itemId];
    if (info.uuid=="") {
        getDataFromServer();
        return false;
    }
    
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD,info.des_time);
    if(QID_MAX == qid){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return false;
    }
    
    if (info.limitNum <= 1) {
        return false;
    }
    
    info.state = FUN_BUILD_DESTROY;
    
    auto curTime = GlobalData::shared()->getWorldTime();
    int lt = info.des_time;
    info.updateTime = curTime+lt;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    
    curFunBuildId = info.itemId;
    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, PALACE_DES_COMMAND);
    cmd->sendAndRelease();
    QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_TIME_BAR, CCInteger::create(curFunBuildId));
    
    if (isError) {
        getDataFromServer();
    }
    return true;
}

void DragonBuildingController::endDestroyBuild(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        auto tmp = dict->valueForKey("updateTime")->doubleValue()/1000;
        if (tmp > 0) {
            tmp = GlobalData::shared()->changeTime(tmp);
        }
        string key = QueueController::getInstance()->addQueueInfo(dict,curFunBuildId);
        int id = curFunBuildId;
        (*curUpDragonBuildsInfo)[id] = FunBuildInfo();
        (*curDragonBuildsInfo)[id].updateTime = tmp;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
    }
    curFunBuildId = 0;
}

void DragonBuildingController::cancelBuildState(int itemId)
{
    clearUpBuildingInfo(itemId);
    auto& info = getFunbuildById(itemId);
    if (info.state == FUN_BUILD_CREATE) {
        info.state = FUN_BUILD_CANCEL_CREATE;
        info.updateTime = 0;
    }
    else {
        info.state = FUN_BUILD_CANCEL_OTH;
        info.updateTime = 0;
    }
}

void DragonBuildingController::pushBuildQueue(CCDictionary* dict)
{
    curUpDragonBuildsInfo = &(GlobalData::shared()->UpDragonBuildsInfo);
    int tid = dict->valueForKey("itemId")->intValue();
    int pos = dict->valueForKey("pos")->intValue();
    int id = getBuildKey(tid, pos);
    (*curUpDragonBuildsInfo)[id].SetInfoFromServer(dict);
}

void DragonBuildingController::synTimeQueueToBuild(int itemId)
{
}

int DragonBuildingController::getBuildNumByType(int type)
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type){
            ret++;
        }
    }
    return ret;
}

int DragonBuildingController::getCurTotalNum(int type)
{
    int ret = 0;
    if (type == DBUILDING_FARM) {
        return getBuildNumByType(DBUILDING_FARM);
    } else if (type == DBUILDING_HABITAT_1 || type == DBUILDING_HABITAT_2 || type == DBUILDING_HABITAT_3 || type == DBUILDING_HABITAT_4 || type == DBUILDING_HABITAT_5 || type == DBUILDING_HABITAT_6 || type == DBUILDING_HABITAT_7) {
        ret += getBuildNumByType(DBUILDING_HABITAT_1) + getBuildNumByType(DBUILDING_HABITAT_2) + getBuildNumByType(DBUILDING_HABITAT_3) + getBuildNumByType(DBUILDING_HABITAT_4) + getBuildNumByType(DBUILDING_HABITAT_5) + getBuildNumByType(DBUILDING_HABITAT_6) + getBuildNumByType(DBUILDING_HABITAT_7);
    }
    return ret;
}

int DragonBuildingController::getMaxLvBuildByType(int type)
{
    int retBuildID = 0;
    int lv = -1;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type && lv<(it->second).level){
            lv = (it->second).level;
            retBuildID = it->first;
        }
    }
    return retBuildID;
}

bool DragonBuildingController::isExistBuildByTypeLv(int type, int lv)
{
    bool ret = false;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type && (it->second).level>=lv && ((it->second).state==FUN_BUILD_NORMAL || (it->second).state==FUN_BUILD_UPING)){
            ret=true;
            break;
        }
    }
    return ret;
}

void DragonBuildingController::initSetUpBuildingInfo(CCDictionary* dict)
{
    curUpDragonBuildsInfo = &(GlobalData::shared()->UpDragonBuildsInfo);
    int tid = dict->valueForKey("itemId")->intValue();
    int pos = dict->valueForKey("pos")->intValue();
    int id = getBuildKey(tid, pos);
    (*curUpDragonBuildsInfo)[id].SetInfoFromServer(dict);
    (*curUpDragonBuildsInfo)[id].updateTime = 0;
}

bool DragonBuildingController::canHavest(int itemId)
{
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return false;
    }
    auto &info = (*curDragonBuildsInfo)[itemId];
    
    int output = getOutPut(info.itemId);
    if(info.type == 1)
    {
        //        if(GlobalData::shared()->resourceInfo.lMoney + output > GlobalData::shared()->resourceInfo.lStorageMoney)
        //        {
        //            return false;
        //        }
    }
    else if(info.type == 2)
    {
        if(GlobalData::shared()->resourceInfo.lWood + output > GlobalData::shared()->resourceInfo.lStorageWood)
        {
            return false;
        }
    }
    else if(info.type == 3)
    {
        if(GlobalData::shared()->resourceInfo.lIron + output > GlobalData::shared()->resourceInfo.lStorageIron)
        {
            return false;
        }
    }
    return true;
}

bool DragonBuildingController::startGetFunBuildProduct(int itemId,int resType)//收获龙资源 type=0表dragonFood，type=1表dragonGold
{
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return false;
    }
    auto &info = (*curDragonBuildsInfo)[itemId];
    
    //    if (!canHavest(info.itemId)) {
    //        return false;
    //    }
    //收获特效？
//    int output = getOutPut(info.itemId);
//    SceneController::getInstance()->onFlyOutPut(info.itemId, output);
    
    info.refreshTime = GlobalData::shared()->getWorldTime();
    
    if (resType==0) {
        FunBuildCommand * cmd=new FunBuildCommand(info.uuid, DRAGON_DFOOD_COMMAND);
        cmd->sendAndRelease();
    }else if (resType==1){
        FunBuildCommand * cmd=new FunBuildCommand(info.uuid, DRAGON_DGOLD_COMMAND);
        cmd->sendAndRelease();
    }
    
    if (isError) {
        getDataFromServer();
    }
    
    return true;
}

void DragonBuildingController::endGetFunBuildProduct(CCDictionary* dict)
{
    if (dict) {
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(dict);
        
//        updateHavestPush();
    }
}

//种植龙食物
bool DragonBuildingController::startPlantFood(int buildItemId,int foodItemId){
    if (curDragonBuildsInfo->find(buildItemId) == curDragonBuildsInfo->end()) {
        return false;
    }
    auto &info = (*curDragonBuildsInfo)[buildItemId];
    
    PlantDragonFood * cmd=new PlantDragonFood(info.uuid,foodItemId, DRAGON_PLANT_COMMAND);
    cmd->sendAndRelease();
    
    return true;
}
void DragonBuildingController::endPlantFood(CCDictionary* dict){
    //(long) refreshTime: 1433992000000
}

bool DragonBuildingController::costCD(int itemId, string toolUUID, int gold)
{
    auto& info = (*curDragonBuildsInfo)[itemId];
    if (info.uuid == "") {
        getDataFromServer();
        return false;
    }
    
    if (gold > 0) {
        if (GlobalData::shared()->playerInfo.gold < gold) {
            YesNoDialog::gotoPayTips();
            return false;
        }
    }
    
    int spdTime = 0;
    if(toolUUID != "") {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolUUID.c_str()));
        if (toolInfo.getCNT() <= 0) {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }
        int tCnt = toolInfo.getCNT() - 1;
        toolInfo.setCNT(tCnt);
        spdTime = atoi(toolInfo.paras[3].c_str());
    }
    
    info.updateTime -= spdTime;
    bool isFinish = false;
    if (info.updateTime <= GlobalData::shared()->getWorldTime() || toolUUID=="") {
        if (info.state == FUN_BUILD_CREATE) {
            info.state = FUN_BUILD_CHANGE;
            isFinish = true;
        }
        else if (info.state == FUN_BUILD_UPING) {
            info.state = FUN_BUILD_UPING_END;
            isFinish = true;
        }
        else if (info.state == FUN_BUILD_DESTROY) {
            info.state = FUN_BUILD_DESTROY_END;
            isFinish = true;
        }
        else {
            return false;
        }
    }
    
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(itemId));
    if (qid > 0) {
        string markstr = CC_ITOA(info.itemId);
        markstr.append(":").append(CC_ITOA(info.level));
        QueueController::getInstance()->startCCDQueue(qid, toolUUID, isFinish, gold,markstr,1);
    }
    
    if (isError) {
        getDataFromServer();
    }
    
    return true;
}

void DragonBuildingController::retCostCD(CCDictionary* dict)
{
    //    (utf_string) c: queue.ccd
    //	(sfs_object) p:
    //    (long) endTime: 0
    //    (sfs_object) itemObj:
    //
    //
    //    (utf_string) uuid: f57a85ecc6954c238fbe3e1ba170adeb
    //    (int) qid: 1
    //    (int) type: 0
    //    (long) updateTime: 1394084555405
    
    if (dict->objectForKey("errorCode")) {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else {
        QueueController::getInstance()->endCCDQueue(dict);
    }
}

bool DragonBuildingController::startFinish(int itemId)
{
    clearUpBuildingInfo(itemId);
    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(itemId));
    if (qid > 0) {
        bool isSend = true;
        if (m_finishGuideBuildMap.find(itemId) != m_finishGuideBuildMap.end()) {
            isSend = false;
            m_finishGuideBuildMap.erase(itemId);
        }
        QueueController::getInstance()->startFinishQueue(qid,isSend);
    }
    
    //    auto& info = (*curDragonBuildsInfo)[itemId];
    //    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, PALACE_FINISH_COMMAND);
    //    cmd->sendAndRelease();
    return true;
}

void DragonBuildingController::onCreateOrUpError()
{
    isError = true;
    getDataFromServer();
}

bool DragonBuildingController::completeDestroy(int itemId)
{
    if (curUpDragonBuildsInfo->find(itemId)==curUpDragonBuildsInfo->end()) {
        auto& info = (*curDragonBuildsInfo)[itemId];
        info.isError = 1;
        info.state = FUN_BUILD_NORMAL;
        onCreateOrUpError();
        return false;
    }
    return true;
}

bool DragonBuildingController::completeUpOrCreate(int itemId, bool isCreate)
{
    if (curUpDragonBuildsInfo->find(itemId)==curUpDragonBuildsInfo->end()) {
        auto& info = (*curDragonBuildsInfo)[itemId];
        info.isError = 1;
//        if(itemId/1000 != FUN_BUILD_MAIN)
//        {
//            info.level += 1;
//        }
        info.state = FUN_BUILD_NORMAL;
        onCreateOrUpError();
    }
    else {
        updateGeneralData(itemId);
        auto& info = (*curDragonBuildsInfo)[itemId];
        auto& nextInfo = (*curUpDragonBuildsInfo)[itemId];
        info.level = nextInfo.level;
        info.state = FUN_BUILD_NORMAL;
        
        info.time_need = nextInfo.time_need;
        info.oldExp = info.exp;
        info.exp = nextInfo.exp;
        //info.power = nextInfo.power;
        info.silver_need = nextInfo.silver_need;
        info.wood_need = nextInfo.wood_need;
        info.food_need = nextInfo.food_need;
        info.iron_need = nextInfo.iron_need;
        info.stone_need = nextInfo.stone_need;
        info.tool_need = nextInfo.tool_need;
        info.building = nextInfo.building;
        info.plevel = nextInfo.plevel;
        info.updateTime = 0;
        info.des_time = nextInfo.des_time;
        
//        if(info.type == FUN_BUILD_WALL) {
//            GlobalData::shared()->cityDefenseVal += (nextInfo.para2 - info.para2);
//            if(info.level == GlobalData::shared()->shield_base){
//                GlobalData::shared()->playerInfo.protectTimeStamp = 0;
//            }
//        }
        
        info.para1 = nextInfo.para1;
        info.para2 = nextInfo.para2;
        info.para3 = nextInfo.para3;
        info.para4 = nextInfo.para4;
        info.para5 = nextInfo.para5;
        info.nextLevelParas = nextInfo.nextLevelParas;
        
        for (int i=1; ; i++) {
            if (nextInfo.para.find(i) != nextInfo.para.end()) {
                info.para[i] = nextInfo.para[i];
            }
            else{
                break;
            }
        }
        
//        if(info.type == FUN_BUILD_FOOD || info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_STONE || info.type == FUN_BUILD_IRON) {
//            if (isCreate && info.refreshTime>0 && info.refreshTime<GlobalData::shared()->getWorldTime()) {
//            }
//            else {
//                info.refreshTime = GlobalData::shared()->getWorldTime();
//                updateHavestPush();
//            }
//        }
        
        updateDataAboutBuild(itemId);
//        //缓存一下大本数据
//        if(info.type == FUN_BUILD_MAIN) {
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, info.level);
//            CCUserDefault::sharedUserDefault()->flush();
//        }
    }
    return true;
}

void DragonBuildingController::updateGeneralData(int itemId){
}

void DragonBuildingController::updateDataAboutBuild(int itemId)
{
//    GuideController::share()->buildingUpdate(itemId);
    
//    auto &info = (*curDragonBuildsInfo)[itemId];
//    if (info.type == 4) {
//        GlobalData::shared()->resourceInfo.lStorageMoney = getStrory(itemId);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//    }
//    else if (info.type == 5) {
//        GlobalData::shared()->resourceInfo.lStorageWood = getStrory(itemId);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//    }
//    else if (info.type == 6) {
//        GlobalData::shared()->resourceInfo.lStorageIron = getStrory(itemId);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//    }
    
//    if (itemId/1000 == FUN_BUILD_TEMPLE) {//神庙（祭祀）
//        if (info.level == 1) {
//            //            UIComponent::getInstance()->setBuyMoneyBtnState();
//        }
//    }
//    else if(itemId/1000 == FUN_BUILD_STABLE){
//        if(info.level == 1){
//            GlobalData::shared()->lastAcceptEnergyTime = GlobalData::shared()->getWorldTime();
//        }
//    }
//    else if(itemId/1000 == FUN_BUILD_MAIN) {//主城升级完成后，更新可开启建筑
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FUNBUILD_OPEN);
//        UIComponent::getInstance()->showResBar();
//        UIComponent::getInstance()->onOpenShowBuildQueue2();
//        if (info.level == 5) {
//            UIComponent::getInstance()->checkShowQuestPrc();
//        }
//        if (info.level == 3) {
//            UIComponent::getInstance()->showFlygold();
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_TALK);
//        }
//        if (info.level >= 6 && GlobalData::shared()->isOpenEvaluate)
//        {
//            PopupViewController::getInstance()->addPopupView(EvaluationView::create());
//        }
//        DailyActiveController::shared()->setHasUnlockItem(true);
//        
//    }
    updateAllOutPut();
//    updateLimitNum();//更新农场，栖息地建筑上限
}

void DragonBuildingController::clearUpBuildingInfo(int itemid)
{
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *tmpMap = (&GlobalData::shared()->UpDragonBuildsInfo);
    
    for (it = tmpMap->begin(); it != tmpMap->end(); it++) {
        if ((it->second).itemId == itemid)
        {
            tmpMap->erase(it);
            break;
        }
    }
}

int DragonBuildingController::getMainCityLv()
{
    if(curDragonBuildsInfo->find(FUN_BUILD_MAIN_CITY_ID) == curDragonBuildsInfo->end()) {
        return 1;
    }
    auto info = getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    return info.level;
}

int DragonBuildingController::getMaxOutPut(int itemId)
{
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return 0;
    }
    auto &info = (*curDragonBuildsInfo)[itemId];
//    if (!(info.type == DBUILDING_HABITAT))
//    {
//        return 0;
//    }
    
    float pro = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    int retMax = info.para2 * pro;
    return retMax;
}

int DragonBuildingController::getOutPut(int itemId)
{
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return 0;
    }
    float pro1 = 0;
    float pro2 = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    auto &info = (*curDragonBuildsInfo)[itemId];

//    if (info.type == DBUILDING_HABITAT)
//    {
//        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0);
//    }
//    else {
//        return 0;
//    }
    
    auto longTime = GlobalData::shared()->getWorldTime() - info.refreshTime;
    int ret = longTime*1.0/3600 * info.para1 * pro1;
    if (true) {
        int tmpT = 0;
        if (info.effectTime >= GlobalData::shared()->getWorldTime()) {
            tmpT = longTime;
        }
        else if (info.effectTime > info.refreshTime){
            tmpT = info.effectTime - info.refreshTime;
        }
        ret += int((info.para1*1.0/3600)*tmpT*building_base_k7*1.0/100);
    }
    
    int retMax = info.para2 * pro2;
    return ret<retMax?ret:retMax;
}

float DragonBuildingController::getOutPutPerSecond(int itemId){
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return 0;
    }
    float pro1 = 0;
    float pro2 = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    auto &info = (*curDragonBuildsInfo)[itemId];

//    if (info.type == DBUILDING_HABITAT)
//    {
//        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0);
//    }
//    else {
//        return 0;
//    }
    
    auto longTime = 1;
    float ret = longTime*1.0/3600 * info.para1 * pro1; //只要基础和作用的 加倍的不要
    
    float retMax = info.para2 * pro2;
    return ret<retMax?ret:retMax;
}

bool DragonBuildingController::canShowOutPut(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    int limit = building_base_k1*1.0*stroge/100;
    if (curOutPut>=limit) {
        return true;
    }
    return false;
}

int DragonBuildingController::getFullCostTime(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    if (curOutPut<stroge) {
        float speed = getOutPutPerSecond(itemId);
        return (stroge-curOutPut)/speed;
    }
    return 0;
}

bool DragonBuildingController::isFullOutPut(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    if (curOutPut>=stroge) {
        return true;
    }
    return false;
}

int DragonBuildingController::getStrory(int itemId)
{
    return 0;
}

int DragonBuildingController::getOutPutPreH(int itemId)
{
    if (curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        return 0;
    }
    auto &info = (*curDragonBuildsInfo)[itemId];
    if (info.type!=1 && info.type!=2 && info.type!=3) {
        return 0;
    }
    
    float pro1 = 0;
    if (info.type==1) {
        pro1 = GlobalData::shared()->scienceEffect["200"]*1.0;//GlobalData::shared()->userLvAboutInfo.effect_200;
    }
    else if (info.type==2) {
        pro1 = GlobalData::shared()->scienceEffect["201"]*1.0;//GlobalData::shared()->userLvAboutInfo.effect_201;
    }
    
    int ret = info.para1 * (1+pro1/100);
    return ret;
}

FunBuildInfo& DragonBuildingController::getFunbuildById(int itemId)
{
    if(curDragonBuildsInfo->find(itemId) == curDragonBuildsInfo->end()) {
        CCLOG("error error: this build error : %d", itemId);
    }
    FunBuildInfo& ret = (curDragonBuildsInfo->find(itemId))->second;
    return ret;
}

void DragonBuildingController::updateAllOutPut()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
//    curFoodOutPreSed = -ArmyController::getInstance()->getTotalUpKeep()*1.0/3600;
    
    int sumSliver = 0;
    int sumStone = 0;
    int sumIron = 0;
    int sumFood = 0;
    int sumWood = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL)
        {
//            if ((it->second).type == DBUILDING_HABITAT) {
//                sumSliver += atoi( (it->second).para[1].c_str() );
//            }
        }
    }
    
    oriWoodPreH = sumWood;
    oriStonePreH = sumStone;
    oriIronPreH = sumIron;
    oriFoodPreH = sumFood;
    oriSliverPreH = sumSliver;
    
//    SliverOutPreSed = sumSliver*(MAX((100+CCCommonUtils::getEffectValueByNum(54))*1.0/100, 0))*1.0/3600;
//    StoneOutPreSed = sumStone*(MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0))*1.0/3600;
//    WoodOutPreSed = sumWood*(MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0))*1.0/3600;
//    FoodOutPreSed = sumFood*(MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0))*1.0/3600;
//    IronOutPreSed = sumIron*(MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0))*1.0/3600;
}

void DragonBuildingController::addOutPut(int sec){
    float sumSliver = 0;
    float sumStone = 0;
    float sumIron = 0;
    float sumFood = 0;
    float sumWood = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL)
        {
//            if ((it->second).type == DBUILDING_HABITAT) {
//                sumSliver += atoi( (it->second).para[1].c_str() );
//            }
        }
    }
    float sliverSed = sumSliver*(MAX((100+CCCommonUtils::getEffectValueByNum(54))*1.0/100, 0))*1.0/3600;
    int addSliver = sliverSed*sec;
    float stoneSed = sumStone*(MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0))*1.0/3600;
    int addStone = stoneSed*sec;
    float woodSed = sumWood*(MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0))*1.0/3600;
    int addWood = woodSed*sec;
    float foodSed = sumFood*(MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0))*1.0/3600;
    int addFood = foodSed*sec;
    float ironSed = sumIron*(MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0))*1.0/3600;
    int addIron = ironSed*sec;
    
    GlobalData::shared()->resourceInfo.lWood += addWood;
    GlobalData::shared()->resourceInfo.lIron += addIron;
    GlobalData::shared()->resourceInfo.lStone += addStone;
    GlobalData::shared()->resourceInfo.lFood += addFood;
    //GlobalData::shared()->resourceInfo.lMoney += addSliver;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
}


string DragonBuildingController::orderBuild(string buildings)
{
    string ret = "";
    std::vector<std::string> buildIds;
    CCCommonUtils::splitString(buildings, ";", buildIds);
    std::vector<int> tmpBuildings;
    for (int i=0; i<buildIds.size(); i++) {
        tmpBuildings.push_back(atoi(buildIds[i].c_str()));
    }
    
    //    std::sort(tmpBuildings.begin(), tmpBuildings.end());
    
    string canBuild = "";
    string notCanBuild = "";
    string notCanBuildForMainLv = "";
    for (int j=0; j<tmpBuildings.size(); j++) {
        CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(tmpBuildings[j]));
        auto info = FunBuildInfo(xmlDictInfo);
        if (info.isUnLock() && !bReachLimit(info.type)) {
            canBuild += ";" ;
            canBuild += CC_ITOA(tmpBuildings[j]);
        }
        else if (bReachLimit(info.type)) {
            notCanBuildForMainLv += ";";
            notCanBuildForMainLv += CC_ITOA(tmpBuildings[j]);
        }
        else {
            notCanBuild += ";" ;
            notCanBuild += CC_ITOA(tmpBuildings[j]);
        }
    }
    
    ret = canBuild+notCanBuildForMainLv+notCanBuild;
    if (ret.length()>0) {
        ret = ret.substr(1,ret.length()-1);
    }
    return ret;
}

void DragonBuildingController::checkFreeTime(bool stat)
{
    if (GlobalData::shared()->playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        if (GlobalData::shared()->freeSpdT==dataConfig_freeSpdT || stat) {
            int freeTime = VipUtil::getCurVipValueByKey("speedup")*60;
            if (freeTime>0) {
                GlobalData::shared()->freeSpdT = freeTime;
            }
        }
    }
    else{
        if (GlobalData::shared()->freeSpdT!=dataConfig_freeSpdT) {
            GlobalData::shared()->freeSpdT = dataConfig_freeSpdT;
        }
    }
}

int DragonBuildingController::getCanBoostBuild(int buildType)
{
    int retBuildID = 0;
    int lv = -1;
    int curTime = GlobalData::shared()->getWorldTime();
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).state == FUN_BUILD_NORMAL && (it->second).type == buildType && lv<(it->second).level){
            if ((it->second).effectTime < curTime) {
                lv = (it->second).level;
                retBuildID = it->first;
            }
        }
    }
    return retBuildID;
}

void DragonBuildingController::initAnimalData(CCDictionary* dict)
{
    if (dict) {
        int time = dict->valueForKey("nextUpdateTime")->doubleValue()/1000;
        if (time>0) {
            m_refreashAnimalTime = GlobalData::shared()->changeTime(time);
        }
        else {
            m_refreashAnimalTime = GlobalData::shared()->getWorldTime()+10*60;
        }
        m_bigAnimalNum = dict->valueForKey("bigMonster")->intValue();
        m_smallAnimalNum = dict->valueForKey("smallMonster")->intValue();
    }
    
    //    m_bigAnimalNum = 7;
    //    m_smallAnimalNum = 7;
    for (int i=0; i<24; i++) {
        idleAnimalPos.push_back(i);
    }
    
    for (int i=0; i<m_bigAnimalNum; i++) {
        srand(i);
        int rp = rand()%idleAnimalPos.size();
        bigAnimalPos.push_back(idleAnimalPos[rp]);
        idleAnimalPos.erase(idleAnimalPos.begin()+rp);
    }
    
    for (int i=0; i<m_smallAnimalNum; i++) {
        srand(i);
        int rp = rand()%idleAnimalPos.size();
        smallAnimalPos.push_back(idleAnimalPos[rp]);
        idleAnimalPos.erase(idleAnimalPos.begin()+rp);
    }
}

void DragonBuildingController::killAnimalByPos(int pos)
{
    int aType = -1;
    for (int i=0; i<bigAnimalPos.size(); i++) {
        if (bigAnimalPos[i] == pos) {
            idleAnimalPos.push_back(pos);
            bigAnimalPos.erase(bigAnimalPos.begin()+i);
            m_bigAnimalNum--;
            aType = 1;
            break;
        }
    }
    
    if (aType == -1) {
        for (int i=0; i<smallAnimalPos.size(); i++) {
            if (smallAnimalPos[i] == pos) {
                idleAnimalPos.push_back(pos);
                smallAnimalPos.erase(smallAnimalPos.begin()+i);
                m_smallAnimalNum--;
                aType = 0;
                break;
            }
        }
    }
    
    if (aType != -1) {
        //发送领奖
    }
    
}

void DragonBuildingController::retKillAnimal(CCDictionary* dict, int pos)
{
    
}

void DragonBuildingController::postRefreashAnimal()
{
    if (m_refreashAnimalTime <= GlobalData::shared()->getWorldTime()) {
        if (m_bigAnimalNum+m_smallAnimalNum < 15) {
            m_refreashAnimalTime+=60;//假如60秒后不返回继续发送
            //发送刷新怪兽的信息
        }
    }
}

void DragonBuildingController::addAnimalData(CCDictionary* dict)
{
    //刷新怪兽，可能造成 大怪 或者 小怪的增加， 也可能不增加。 （按文档设计可能会出现 大怪增加，小怪减少的情况，此情况和后台确认了，当怪物满的时候不再刷新）
    if (dict) {
        int time = dict->valueForKey("nextUpdateTime")->doubleValue()/1000;
        if (time>0) {
            m_refreashAnimalTime = GlobalData::shared()->changeTime(time);
        }
        else {
            m_refreashAnimalTime += 10*60;
        }
        int newBigNum = dict->valueForKey("bigMonster")->intValue();
        int newSmallnum = dict->valueForKey("smallMonster")->intValue();
        
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        
        if (newBigNum > m_bigAnimalNum) {
            for (int i=0; i<(newBigNum-m_bigAnimalNum); i++) {
                srand(i);
                int rp = rand()%idleAnimalPos.size();
                int pos = idleAnimalPos[rp];
                bigAnimalPos.push_back(idleAnimalPos[rp]);
                idleAnimalPos.erase(idleAnimalPos.begin()+rp);
                //主城添加一只大怪
            }
            m_bigAnimalNum = newBigNum;
        }
        
        if (newSmallnum > m_smallAnimalNum) {
            for (int i=0; i<(newSmallnum-m_smallAnimalNum); i++) {
                srand(i);
                int rp = rand()%idleAnimalPos.size();
                int pos = idleAnimalPos[rp];
                smallAnimalPos.push_back(idleAnimalPos[rp]);
                idleAnimalPos.erase(idleAnimalPos.begin()+rp);
                //主城添加一只小怪
            }
            m_smallAnimalNum = newSmallnum;
        }
    }
}

int DragonBuildingController::getBuildIdByPos(int pos)
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).pos == pos){
            ret = it->first;
            break;
        }
    }
    return ret;
}

bool DragonBuildingController::checkIsGuideBuild()
{
    if(GlobalData::shared()->playerInfo.level>1 || GlobalData::shared()->playerInfo.exp==0) {
        return false;
    }
    return true;
}

void DragonBuildingController::makeGuideBuildData(int itemId, int pos, int level)
{
    string GuideBuildKey = "";
    GuideBuildKey = GuideBuildKey + CC_ITOA(itemId)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
    
    auto pDict = CCDictionary::create();
    auto dict = CCDictionary::create();
    auto& guideBd = m_guideBuildMap[GuideBuildKey];
    dict->setObject(CCString::create(CC_ITOA(guideBd.level)), "level");
    dict->setObject(CCString::create(guideBd.nextLevelParas), "nextLevelParas");
    dict->setObject(CCString::create(CC_ITOA(guideBd.severST)), "opType");
    dict->setObject(CCString::create(CC_ITOA(guideBd.des_time)), "destroy_time");
    dict->setObject(CCString::create(CC_ITOA(guideBd.para2)), "para2");
    dict->setObject(CCString::create(guideBd.uuid), "uuid");
    dict->setObject(CCString::create(CC_ITOA(guideBd.food_need)), "food");
    dict->setObject(CCString::create(CC_ITOA(guideBd.type)), "itemId");
    dict->setObject(CCString::create(CC_ITOA(GlobalData::shared()->getTimeStamp()*1000)), "refreshTime");
    dict->setObject(CCString::create(CC_ITOA(guideBd.para1)), "para1");
    dict->setObject(CCString::create(CC_ITOA(guideBd.wood_need)), "wood");
    dict->setObject(CCString::create(CC_ITOA(guideBd.stone_need)), "stone");
    dict->setObject(CCString::create(CC_ITOA(guideBd.exp)), "exp");
    dict->setObject(CCString::create(guideBd.building), "building");
    dict->setObject(CCString::create(CC_ITOA(guideBd.pos)), "pos");
    dict->setObject(CCString::create(CC_ITOA(guideBd.iron_need)), "iron");
    dict->setObject(CCString::create(CC_ITOA(guideBd.time_need)), "time");
    dict->setObject(CCString::create(CC_ITOA(guideBd.silver_need)), "silver");
    dict->setObject(CCString::create(CC_ITOA(guideBd.para3)), "para3");
    
    pDict->setObject(dict, "itemObj");
    pDict->setObject(CCString::create("1"), "qid");
    pDict->setObject(CCString::create("0"), "type");
    pDict->setObject(CCString::create(CC_ITOA(GlobalData::shared()->getTimeStamp()*1000)), "startTime");
    pDict->setObject(CCString::create(CC_ITOA((GlobalData::shared()->getTimeStamp()+2)*1000)), "updateTime");
    pDict->setObject(CCString::create("0"), "endTime");
    
    m_guideBuildMap.erase(GuideBuildKey);
    int tmpKey = getBuildKey(itemId, pos);
    m_finishGuideBuildMap[tmpKey] = 1;
    
    endOpenFunBuild(pDict, 0);
}

void DragonBuildingController::OpenMoveBuild()
{
    OpenMoveBuildStat = true;
    //发出事件，开始置换，
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_BUILD, CCString::create("-1"));
}

void DragonBuildingController::SetMoveBuildPos(int pos)
{
    WillMovePos.push_back(pos);
    if (WillMovePos.size()==2) {
        //弹出确认交换
        PopupViewController::getInstance()->addPopupView(MoveBuildView::create());
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_BUILD, CCString::create(CC_ITOA(pos)));
}

void DragonBuildingController::CancelLastSetPos()
{
    if (WillMovePos.size() == 2) {
        int lastPos = WillMovePos[1];
        WillMovePos.pop_back();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CANCEL_LAST_MOVE_BUILD, CCString::create(CC_ITOA(lastPos)));
    }
}

void DragonBuildingController::CancelMoveBuild()
{
    OpenMoveBuildStat = false;
    WillMovePos.clear();
    //发出事件，取消置换
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CANCEL_BUILD);
}

void DragonBuildingController::StartMoveBuild(int gold)
{
    FunBuildMove * cmd=new FunBuildMove(WillMovePos[0], WillMovePos[1], gold);
    cmd->sendAndRelease();
}

void DragonBuildingController::EndMoveBuild(CCDictionary* dict)
{
    if (dict->objectForKey("errorCode")) {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else {
        
        if (dict->objectForKey("count")) {
            int cnt = dict->valueForKey("count")->intValue();
            auto& tool = ToolController::getInstance()->getToolInfoById(ITEM_MOVE_BUILD);
            tool.setCNT(cnt);
        }
        
        if (dict->objectForKey("remainGold")) {
            int gold = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(gold);
        }
        
        int oldBuild1 = getBuildIdByPos(WillMovePos[0]);
        int oldBuild2 = getBuildIdByPos(WillMovePos[1]);
        int newBuild1 = 0;
        int newBuild2 = 0;
        
        FunBuildInfo* build1 = NULL;
        FunBuildInfo* build2 = NULL;
        if (oldBuild1 > 0) {
            build1 = &(GlobalData::shared()->dragonBuildingInfo[oldBuild1]);
            build1->pos = WillMovePos[1];
            newBuild1 = getBuildKey(build1->type,build1->pos);
            build1->itemId = newBuild1;
        }
        
        if (oldBuild2 > 0) {
            build2 = &(GlobalData::shared()->dragonBuildingInfo[oldBuild2]);
            build2->pos = WillMovePos[0];
            newBuild2 = getBuildKey(build2->type,build2->pos);
            build2->itemId = newBuild2;
        }
        
        //交换建筑map数据
        if (oldBuild1/1000 == oldBuild2/1000)//相同类型的两个建筑
        {
            if (build1 && build2) {
                FunBuildInfo info1 = FunBuildInfo(build1);
                FunBuildInfo info2 = FunBuildInfo(build2);
                
                GlobalData::shared()->dragonBuildingInfo[oldBuild1] = info2;
                GlobalData::shared()->dragonBuildingInfo[oldBuild2] = info1;
            }
        }
        else
        {
            if (oldBuild1>0 && build1) {
                FunBuildInfo info = FunBuildInfo(build1);
                GlobalData::shared()->dragonBuildingInfo[newBuild1] = info;
                GlobalData::shared()->dragonBuildingInfo.erase(oldBuild1);
            }
            if (oldBuild2>0 && build2) {
                FunBuildInfo info = FunBuildInfo(build2);
                GlobalData::shared()->dragonBuildingInfo[newBuild2] = info;
                GlobalData::shared()->dragonBuildingInfo.erase(oldBuild2);
            }
        }
        
        //更新队列key
        if (oldBuild1>0) {
            int qId = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(oldBuild1));
            if (qId>0) {
                GlobalData::shared()->allQueuesInfo[qId].key = CC_ITOA(newBuild1);
            }
        }
        if (oldBuild2>0) {
            int qId = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(oldBuild2));
            if (qId>0) {
                GlobalData::shared()->allQueuesInfo[qId].key = CC_ITOA(newBuild2);
            }
        }
        
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(oldBuild1)), "oldBuild1");
        dict->setObject(CCString::create(CC_ITOA(oldBuild2)), "oldBuild2");
        dict->setObject(CCString::create(CC_ITOA(newBuild1)), "newBuild1");
        dict->setObject(CCString::create(CC_ITOA(newBuild2)), "newBuild2");
        dict->setObject(CCString::create(CC_ITOA(WillMovePos[0])), "pos1");
        dict->setObject(CCString::create(CC_ITOA(WillMovePos[1])), "pos2");
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FINISH_MOVE_BUILD, dict);
    }
    
    UIComponent::getInstance()->onCancelMoveBuild();
}

void DragonBuildingController::updatePreSed(float dt){
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        bool tmpSt = false;
        if ((it->second).itemId > 0 && ((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_DESTROY || (it->second).state==FUN_BUILD_INIT))
        {
            auto curTime = GlobalData::shared()->getWorldTime();
            if ( (it->second).updateTime <= curTime)
            {
                if((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_INIT)
                {
                    if((it->second).state==FUN_BUILD_INIT)
                    {
//                        onCreateBuild((it->second).itemId);
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_INIT_DIRECT_CHANGE, CCInteger::create(it->first));
                    }
                    (it->second).state = FUN_BUILD_CHANGE;
                }
                else if((it->second).state==FUN_BUILD_DESTROY)
                {
                    (it->second).state = FUN_BUILD_DESTROY_END;
                }
                else
                {
                    (it->second).state = FUN_BUILD_UPING_END;
                }
                tmpSt = true;
            }
            else
            {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_BUILD_SETTIME, CCInteger::create(it->first));
//                if (m_buildItems.find(it->first) != m_buildItems.end()) {
//                    int langTime = (it->second).updateTime - curTime;
//                    m_buildItems[it->first]->setTime(langTime);
//                }
            }
        }
        
        if((it->second).state==FUN_BUILD_NORMAL) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_NORMAL_ING, CCInteger::create(it->first));
//            if (m_buildItems.find(it->first) != m_buildItems.end()) {
//                m_buildItems[it->first]->onUpdateUpIcon(0);
//            }
        }
        
        if((it->second).state==FUN_BUILD_INIT)//可建造状态 转变为 建造中状态
        {
            (it->second).state = FUN_BUILD_CREATE;
//            onCreateBuild((it->second).itemId);
//            showBuildBtns((it->second).itemId);
//            if ((it->second).pos<=16) {
//                checkTileGlow(NULL);
//            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_INIT_END, CCInteger::create(it->first));
        }
        
        if((it->second).state==FUN_BUILD_CHANGE)//建造中状态 转变为 正常状态
        {
            if(completeUpOrCreate((it->second).itemId, true) )
            {
//                onOpenBuildEnd((it->second).itemId);
//                onUpgradeNewBuild((it->second).itemId);
//                onResetBuild((it->second).itemId);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_CHANGE_END, CCInteger::create(it->first));
                if (tmpSt) {
                    startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    clearUpBuildingInfo((it->second).itemId);
                }
                
            }
        }
        
        if((it->second).state==FUN_BUILD_UPING_END)//升级中状态 转变为 正常状态
        {
            if (DragonBuildingController::getInstance()->completeUpOrCreate((it->second).itemId))
            {
//                onUpgradeNewBuild((it->second).itemId);
//                onResetBuild((it->second).itemId);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_UPING_END_END, CCInteger::create(it->first));
                if (tmpSt) {
                    startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    clearUpBuildingInfo((it->second).itemId);
                }
            }
        }
////////////////
        
        if (it->second.type == DBUILDING_HATCHER || (it->second).type== DBUILDING_CAVE || (it->second).type==DBUILDING_HABITAT_1 || (it->second).type==DBUILDING_HABITAT_2 || (it->second).type==DBUILDING_HABITAT_3 || (it->second).type==DBUILDING_HABITAT_4 || (it->second).type==DBUILDING_HABITAT_5 || (it->second).type==DBUILDING_HABITAT_6 || (it->second).type==DBUILDING_HABITAT_7)
        {
            if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_CANSHOWSTATE, CCInteger::create(it->first));
//                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
//                    m_buildItems[(it->second).itemId]->canShowState();
//                }
            }
        }
        else if ((it->second).type==DBUILDING_FARM) {
            if ((it->second).state==FUN_BUILD_NORMAL) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_CANSHOWSTATE, CCInteger::create(it->first));
//                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
//                    m_buildItems[(it->second).itemId]->canShowState();
//                }
            }
        }
        
        if((it->second).type==DBUILDING_FARM){
//            if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
//                m_buildItems[(it->second).itemId]->checkIncreasedStatus();
//            }
        }
        
////////////////////
        
        if ((it->second).state==FUN_BUILD_CREATE_ERROR || (it->second).state == FUN_BUILD_DESTROY_END || (it->second).state == FUN_BUILD_CANCEL_CREATE)
        {
            bool bOK = true;
            if ((it->second).state == FUN_BUILD_DESTROY_END) {
                bOK = completeDestroy(it->first);
                if (bOK) {
                    startFinish((it->second).itemId);
                }
                else {
                    clearUpBuildingInfo((it->second).itemId);
                }
            }
            //建造出错
            if (bOK) {
//                int key = it->first;
//                int pos = it->first%1000;
//                if (m_buildItems.find(key)!=m_buildItems.end()) {
//                    m_buildItems.erase(m_buildItems.find(key));
//                }
//                DragonBuild* delbuild = dynamic_cast<DragonBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
//                delbuild->onBuildDelete();
//                m_nodeBuildings[pos]->removeChild(delbuild);
//                
//                DragonBuild* build = DragonBuild::create( pos , m_nameLayer);
//                build->setTag(pos);
//                m_nodeBuildings[pos]->addChild(build);
//                m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
//                int od = m_nodeBuildings[pos]->getZOrder();
//                build->setTileBatch(m_nodeBuildings[pos]->getPositionX(), m_nodeBuildings[pos]->getPositionY(), m_resbatchNode, od);
//                build->m_key = 1000-od;
//                
//                curDragonBuildsInfo->erase(it);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_CREATE_BUILD_HAS_ERROR, CCInteger::create(it->first));
                break;
            }
        }
        else if((it->second).state==FUN_BUILD_UPING_ERROR)
        {
            //升级出错
            (it->second).state = FUN_BUILD_NORMAL;
//            m_buildItems[(it->second).itemId]->setTime(0);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_UPING_ERROR_END, CCInteger::create(it->first));
        }
        else if((it->second).state==FUN_BUILD_NORMAL_ERROR)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
//            m_buildItems[(it->second).itemId]->setTime(0,true);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_NORMAL_ERROR_END, CCInteger::create(it->first));
        }
        else if((it->second).state==FUN_BUILD_CANCEL_OTH)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
//            m_buildItems[(it->second).itemId]->setTime(0,true);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DS_FUN_BUILD_CANCEL_OTH_END, CCInteger::create(it->first));
        }
    }

}

void DragonBuildingController::updateLimitNum(){//农场，栖息地建筑上限取决于龙巢的para1
    int limitNumFarm=0;
    int limitNumHabitat=0;
    
    auto& info = getFunbuildById(DBUILDING_HATCHER_ItemId);
    map<int, FunBuildInfo>::iterator it;
    vector<string> tmpVec;
    CCCommonUtils::splitString(info.para[1], "|", tmpVec);
    if (tmpVec.size() >= 2) {
        limitNumFarm = atoi(tmpVec[0].c_str());
        limitNumHabitat = atoi(tmpVec[1].c_str());
    }
    
    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
    {
        if ((it->second).type==DBUILDING_FARM||(it->second).type==DBUILDING_HABITAT_1||(it->second).type==DBUILDING_HABITAT_2||(it->second).type==DBUILDING_HABITAT_3||(it->second).type==DBUILDING_HABITAT_4||(it->second).type==DBUILDING_HABITAT_5||(it->second).type==DBUILDING_HABITAT_6||(it->second).type==DBUILDING_HABITAT_7)
        {
            if ((it->second).type==DBUILDING_FARM) {
                (it->second).limitNum=limitNumFarm;
            }else {
                (it->second).limitNum=limitNumHabitat;
            }
        }
    }
    
}

//得到农场，栖息地建筑上限
int DragonBuildingController::getLimitNum(int type){
    int limitNumFarm=0;
    int limitNumHabitat=0;
    
    auto& info = getFunbuildById(DBUILDING_HATCHER_ItemId);
    map<int, FunBuildInfo>::iterator it;
    vector<string> tmpVec;
    CCCommonUtils::splitString(info.para[1], "|", tmpVec);
    if (tmpVec.size() >= 2) {
        limitNumFarm = atoi(tmpVec[0].c_str());
        limitNumHabitat = atoi(tmpVec[1].c_str());
    }
    
    if (type==DBUILDING_FARM) {
        return limitNumFarm;
    }else if(type==DBUILDING_HABITAT_1||type==DBUILDING_HABITAT_2||type==DBUILDING_HABITAT_3||type==DBUILDING_HABITAT_4||type==DBUILDING_HABITAT_5||type==DBUILDING_HABITAT_6||type==DBUILDING_HABITAT_7){
        return limitNumHabitat;
    }

}

bool DragonBuildingController::bReachLimit(int type)
{
    bool ret = false;
    if (type != DBUILDING_HATCHER && type != DBUILDING_CAVE) {
        int limit = DragonBuildingController::getInstance()->getLimitNum(type);
        int nowCnt = DragonBuildingController::getInstance()->getCurTotalNum(type);
        if (nowCnt >= limit && limit > 0) {
            ret = true;
        }
    }
    return ret;
}

//void DragonBuildingController::updateHavestPush(int itemId)
//{
//    int maxTime = 0;
//    int worldTime = GlobalData::shared()->getWorldTime();
//    bool canPush = false;
//    
//    if (itemId == 0)
//    {
//        allFullTime = 0;
//        map<int, FunBuildInfo>::iterator it;
//        //        map<int, FunBuildInfo> *testInfo = &(GlobalData::shared()->dragonBuildingInfo);
//        for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
//        {
//            auto& bInfo = it->second;
//            if (bInfo.state == FUN_BUILD_NORMAL)
//            {
//                //                if (bInfo.type==DBUILDING_HABITAT_1) {
//                //                    int costTime = getFullCostTime(bInfo.itemId);
//                //                    if (costTime > maxTime) {
//                //                        maxTime = costTime;
//                //                    }
//                //                }
//            }
//        }
//        
//        if (maxTime > 60) {
//            canPush = true;
//            allFullTime = worldTime+maxTime;
//        }
//    }
//    else
//    {
//        int costTime = getFullCostTime(itemId);
//        if ( (worldTime+costTime) > allFullTime) {
//            allFullTime = (worldTime+costTime);
//            canPush = true;
//        }
//    }
//    
//    
//    if (canPush)
//    {
//        PushSettingInfo* pushInfo = NULL;
//        for (int i=0; i<GlobalData::shared()->pushSettingVector.size(); i++) {
//            pushInfo = GlobalData::shared()->pushSettingVector[i];
//            if (pushInfo->type == 7) {
//                break;
//            }
//        }
//        string sound = "1";
//        if (pushInfo) {
//            sound = CC_ITOA(pushInfo->sound);
//            if (pushInfo->status == 1) {
//                cocos2d::extension::CCDevice::cancelNotice(99);
//                cocos2d::extension::CCDevice::pushNotice(99, (allFullTime - worldTime),  _lang("105661"), sound, "14");
//            }
//        }
//    }
//}

//void DragonBuildingController::testOpenTile(int itemId)
//{
//    curFunBuildId = itemId;
//    m_bigTileMap[curFunBuildId].state = FUN_BUILD_ULOCK;
//    m_bigTileMap[curFunBuildId].unlock = 1;
//    vector<string> vecPosIds;
//    CCCommonUtils::splitString(m_bigTileMap[curFunBuildId].positions, ";", vecPosIds);
//    for (int i=0; i<vecPosIds.size(); i++) {
//        int id = atoi(vecPosIds[i].c_str());
//        m_tileMap[id].state = FUN_BUILD_ULOCK;
//        m_tileMap[id].unlock = 1;
//        
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UNLOCK_TILE, CCInteger::create(id));
//    }
//    
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UNLOCK_TILE, CCInteger::create(curFunBuildId));
//}

//void DragonBuildingController::updateResPreSed(float t)
//{
//    checkFreeTime();
//    updateAllOutPut();
//    
//    FoodOdd += curFoodOutPreSed;
//    int addFood = FoodOdd;
//    FoodOdd -= addFood;
//    lastAddFood += addFood;
//    
//    if (addFood >0 && GlobalData::shared()->resourceInfo.lFood >= GlobalData::shared()->resourceInfo.lStorageFood) {
//        FoodOdd = addFood = 0;
//    }
//    //    if (addWood>0 && GlobalData::shared()->resourceInfo.lWood >= GlobalData::shared()->resourceInfo.lStorageWood) {
//    //        WoodOdd = addWood = 0;
//    //    }
//    //    if (addIron>0 && GlobalData::shared()->resourceInfo.lIron >= GlobalData::shared()->resourceInfo.lStorageIron) {
//    //        IronOdd = addIron = 0;
//    //    }
//    //    if (addStone>0 && GlobalData::shared()->resourceInfo.lStone >= GlobalData::shared()->resourceInfo.lStorageStone) {
//    //        StoneOdd = addStone = 0;
//    //    }
//    //    if (addSliver>0 && GlobalData::shared()->resourceInfo.lMoney >= GlobalData::shared()->resourceInfo.lStorageMoney) {
//    //        SliverOdd = addSliver = 0;
//    //    }
//    
//    GlobalData::shared()->resourceInfo.lFood += addFood;
//    
//    if(GlobalData::shared()->resourceInfo.lFood<0) {
//        GlobalData::shared()->resourceInfo.lFood = 0;
//    }
//    
//    if(fabs(lastAddFood)>=1) //fabs(lastAddWood)>=1 || fabs(lastAddStone)>=1 || fabs(lastAddIron)>=1 || fabs(lastAddFood)>=1
//    {
//        if(addTime >=60) {
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
//            addTime = 0;
//            //            lastAddWood = 0;
//            //            lastAddStone = 0;
//            //            lastAddIron = 0;
//            lastAddFood = 0;
//        }
//    }
//    addTime++;
//}

//int DragonBuildingController::findBdRoPos()
//{
//    int ret = 0;
//
//    int tmpBid = 0;
//    vector<int> vecBids;
//    vecBids.push_back(FUN_BUILD_BARRACK1);
//    vecBids.push_back(FUN_BUILD_BARRACK2);
//    vecBids.push_back(FUN_BUILD_BARRACK3);
//    vecBids.push_back(FUN_BUILD_BARRACK4);
//    for (int i=0; i<vecBids.size(); i++) {
//        tmpBid = getMaxLvBuildByType(vecBids[i]);
//        if (tmpBid>0) {
//            auto& info = getFunbuildById(tmpBid);
//            if (info.state == FUN_BUILD_CREATE && info.level<=1) {
//                ret = tmpBid;
//            }
//            else {
//                return 0;
//            }
//        }
//    }
//
//    if (ret>0) {
//        return ret;
//    }
//
//    string vecHavePos = "";
//    map<int, FunBuildInfo>::iterator it;
//    map<int, FunBuildInfo> *curDragonBuildsInfo = DragonBuildingController::getInstance()->curDragonBuildsInfo;
//    for(it=curDragonBuildsInfo->begin(); it != curDragonBuildsInfo->end(); ++it)
//    {
//        if ((it->second).pos >= 5 && (it->second).pos <= 16){
//            vecHavePos = vecHavePos+CC_ITOA((it->second).pos)+",";
//        }
//    }
//
//    if (vecHavePos.find("11") == -1 ) {
//        ret = 11;
//        return ret;
//    }
//
//    for (int i=6; i<=16; i++) {
//        string ti = CC_ITOA(i);
//        if (vecHavePos.find(ti.c_str()) == -1 ) {
//            ret = i;
//            break;
//        }
//    }
//
//    return ret;
//}

//string DragonBuildingController::getTalkTips()
//{
//    string ret;
//    vector<string> tmpVec;
//
//    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, 1);
//    if (qid != QID_MAX) {//有建筑队列空闲
//        //        tmpVec.insert(tmpVec.begin(), talkTipsVec4.begin(), talkTipsVec4.end());
//        for (int m=0; m<talkTipsVec4.size(); m++) {
//            string tid = talkTipsVec4[m];
//            string buildType = CCCommonUtils::getPropById(tid, "accept1");
//            int num = getBuildNumByType(atoi(buildType.c_str()));
//            if (num > 0) {
//                tmpVec.push_back(tid);
//            }
//        }
//    }
//
//    qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, 1);
//    if (qid != QID_MAX) {//有科技队列空闲
//        tmpVec.insert(tmpVec.begin(), talkTipsVec1.begin(), talkTipsVec1.end());
//    }
//
//    for (int i=0; i<talkTipsVec3.size(); i++) {//造兵队列
//        string tid = talkTipsVec3[i];
//        string buildType = CCCommonUtils::getPropById(tid, "accept1");
//        int qType = CCCommonUtils::getQueueTypeByBuildType(atoi(buildType.c_str()));
//        if (qType != -1) {
//            qid = QueueController::getInstance()->canMakeItemByType(qType, 1);
//            if (qid != QID_MAX) {
//                tmpVec.push_back(tid);
//            }
//        }
//    }
//
//    if ( WorldController::getInstance()->getCurrentMarchCount() >= WorldController::getInstance()->getMaxMarchCount() )
//    {
//        double armyPower = GlobalData::shared()->playerInfo.armyPower;
//        for (int j=0; j<talkTipsVec2.size(); j++) {//出征队列
//            string tid = talkTipsVec2[j];
//            string sPower = CCCommonUtils::getPropById(tid, "accept1");
//            string ePower = CCCommonUtils::getPropById(tid, "accept2");
//            int startPower = atoi(sPower.c_str());
//            int endPower = atoi(ePower.c_str());
//            if (startPower<=0) {
//                tmpVec.push_back(tid);
//            }
//            else if (startPower <= armyPower) {
//                if (endPower>armyPower || endPower==0) {
//                    tmpVec.push_back(tid);
//                }
//            }
//        }
//    }
//
//    if(ArmyController::getInstance()->getTotalDead()>0) {//有伤兵
//        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_HOSPITAL, 1);
//        if (qid != QID_MAX) {//有治疗队列空闲
//            tmpVec.insert(tmpVec.begin(), talkTipsVec5.begin(), talkTipsVec5.end());
//        }
//    }
//
//    if (tmpVec.size()>0) {
//        int rIdx = CCMathUtils::getRandomInt(0, tmpVec.size()-1);
//        string dialog = CCCommonUtils::getPropById(tmpVec[rIdx], "content");
//        //        string Ttype = CCCommonUtils::getPropById(tmpVec[rIdx], "type");
//        ret = _lang(dialog);
//    }
//    else if (talkTipsVec0.size()>0) {
//        int rIdx = CCMathUtils::getRandomInt(0, talkTipsVec0.size()-1);
//        string dialog = CCCommonUtils::getPropById(talkTipsVec0[rIdx], "content");
//        ret = _lang(dialog);
//    }
//    return ret;
//}

//void DragonBuildingController::initPosPath()
//{
//    if (1) {//pos == 8
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(440,970));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(900);
//        posToPath[8] = path;
//        posToZorders[8] = zorders;
//    }
//    if (1) {//pos == 10
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(700,900));
//        path.push_back(ccp(800,850));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(250,800));
//
//        vector<int> zorders;
//        zorders.push_back(1000);
//        posToPath[10] = path;
//        posToZorders[10] = zorders;
//    }
//    if (1) {//pos == 7
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(450,1290));
//        path.push_back(ccp(490,1210));
//        path.push_back(ccp(670,1120));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(600);
//        zorders.push_back(600);
//        zorders.push_back(600);
//        zorders.push_back(900);
//        posToPath[7] = path;
//        posToZorders[7] = zorders;
//    }
//    if (1) {//pos == 6
//        //        vector<cocos2d::CCPoint> path;
//        //        path.push_back(ccp(350,1170));
//        //        path.push_back(ccp(490,1210));
//        //        path.push_back(ccp(670,1120));
//        //
//        //        path.push_back(ccp(700,1020));
//        //        path.push_back(ccp(540,920));
//        //        path.push_back(ccp(320,900));
//        //        path.push_back(ccp(220,790));
//        //
//        //        vector<int> zorders;
//        //        zorders.push_back(700);
//        //        zorders.push_back(700);
//        //        zorders.push_back(700);
//        //        zorders.push_back(900);
//        //        posToPath[6] = path;
//        //        posToZorders[6] = zorders;
//
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(2010,1130));
//        path.push_back(ccp(1020,615));
//        path.push_back(ccp(720,570));
//
//        vector<int> zorders;
//        zorders.push_back(1010);
//        posToPath[6] = path;
//        posToZorders[6] = zorders;
//    }
//    if (1) {//pos == 9
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(680,1190));
//        path.push_back(ccp(720,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(700);
//        zorders.push_back(700);
//        zorders.push_back(900);
//        posToPath[9] = path;
//        posToZorders[9] = zorders;
//    }
//    if (1) {//pos == 11
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(940,1020));
//        path.push_back(ccp(1000,990));
//        path.push_back(ccp(920,930));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(800);
//        zorders.push_back(800);
//        zorders.push_back(800);
//        zorders.push_back(900);
//        posToPath[11] = path;
//        posToZorders[11] = zorders;
//    }
//    if (1) {//pos == 15
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1630,1350));
//        path.push_back(ccp(1690,1320));
//        path.push_back(ccp(1630,1290));
//        path.push_back(ccp(1450,1380));
//        path.push_back(ccp(1310,1380));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(900);
//        posToPath[15] = path;
//        posToZorders[15] = zorders;
//    }
//    if (1) {//pos == 16
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1870,1470));
//        path.push_back(ccp(1900,1410));
//        path.push_back(ccp(1810,1380));
//        path.push_back(ccp(1630,1470));
//        path.push_back(ccp(1450,1380));
//        path.push_back(ccp(1310,1380));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(900);
//        posToPath[16] = path;
//        posToZorders[16] = zorders;
//    }
//    if (1) {//pos == 14
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1670,1550));
//        path.push_back(ccp(1730,1520));
//        path.push_back(ccp(1450,1380));
//        path.push_back(ccp(1310,1380));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(400);
//        zorders.push_back(900);
//        posToPath[14] = path;
//        posToZorders[14] = zorders;
//    }
//    if (1) {//pos == 13
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1370,1480));
//        path.push_back(ccp(1510,1410));
//        path.push_back(ccp(1450,1380));
//        path.push_back(ccp(1310,1380));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(900);
//        posToPath[13] = path;
//        posToZorders[13] = zorders;
//    }
//    if (1) {//pos == 12
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1210,1640));
//        path.push_back(ccp(1280,1600));
//        path.push_back(ccp(1080,1480));
//        path.push_back(ccp(1090,1270));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
//
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
//
//        vector<int> zorders;
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(900);
//        posToPath[12] = path;
//        posToZorders[12] = zorders;
//    }
//    if (1) {//pos == 52
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1020,650));
//        path.push_back(ccp(720,570));
//
//        vector<int> zorders;
//        zorders.push_back(1010);
//        posToPath[52] = path;
//        posToZorders[52] = zorders;
//    }
//    if (1) {//pos == 53
//        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1300,805));
//        path.push_back(ccp(1020,615));
//        path.push_back(ccp(720,570));
//
//        vector<int> zorders;
//        zorders.push_back(1010);
//        posToPath[53] = path;
//        posToZorders[53] = zorders;
//    }
//
//    sqerPoints.push_back(ccp(290,560));
//    sqerPoints.push_back(ccp(380,515));
//    sqerPoints.push_back(ccp(470,470));
//    sqerPoints.push_back(ccp(560,425));
//    sqerPoints.push_back(ccp(650,380));
//    sqerPoints.push_back(ccp(740,335));
//
//    sqerPoints.push_back(ccp(190,515));
//    sqerPoints.push_back(ccp(280,470));
//    sqerPoints.push_back(ccp(370,425));
//    sqerPoints.push_back(ccp(460,380));
//    sqerPoints.push_back(ccp(550,335));
//    sqerPoints.push_back(ccp(640,290));
//    sqerPoints.push_back(ccp(730,245));
//
//    sqerPoints.push_back(ccp(90,470));
//    sqerPoints.push_back(ccp(180,425));
//    sqerPoints.push_back(ccp(270,380));
//    sqerPoints.push_back(ccp(360,335));
//    sqerPoints.push_back(ccp(450,290));
//    sqerPoints.push_back(ccp(540,245));
//    sqerPoints.push_back(ccp(630,200));
//
//    sqerPoints.push_back(ccp(80,380));
//    sqerPoints.push_back(ccp(170,335));
//    sqerPoints.push_back(ccp(260,290));
//    sqerPoints.push_back(ccp(350,245));
//    sqerPoints.push_back(ccp(440,200));
//    sqerPoints.push_back(ccp(530,155));
//
//    sqerPoints.push_back(ccp(70,290));
//    sqerPoints.push_back(ccp(160,245));
//    sqerPoints.push_back(ccp(250,200));
//    sqerPoints.push_back(ccp(340,155));
//
//    for (int i=0; i<sqerPoints.size(); i++) {
//        sqerPoints[i].x += -10;
//        sqerPoints[i].y += 180;
//    }
//
//}
