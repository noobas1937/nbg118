//
//  FunBuildController.cpp
//  IF
//
//  Created by fubin on 13-11-1.
//
//

#include "FunBuildController.h"
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
#include "LuaController.h"
#include "AllianceManager.h"

static FunBuildController *_instance = NULL;

FunBuildController::FunBuildController()
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
    
    WoodOutPreSed = 0;
    WoodOdd = 0;
    FoodOutPreSed = 0;
    curFoodOutPreSed = 0;
    FoodOdd = 0;
    StoneOutPreSed = 0;
    StoneOdd = 0;
    IronOutPreSed = 0;
    IronOdd = 0;
    SliverOutPreSed = 0;
    SliverOdd = 0;
    
    lastAddWood = 0;
    lastAddStone = 0;
    lastAddIron = 0;
    lastAddFood = 0;
    
    dataConfig_freeSpdT = 0;
    
    building_base_k1 = 0;
    
    OpenMoveBuildStat = false;
    willMoveToBuildType = 0;
    willMoveToBuildItemID = 0;
    //初始化FunBuildController
    curBuildsInfo = &(GlobalData::shared()->imperialInfo);
    curUpBuildsInfo = &(GlobalData::shared()->UpgradeBuildsInfo);
    
    auto posictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("position");
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
    
    auto bigTileDict = LocalController::shared()->DBXMLManager()->getGroupByKey("position_unlock");
    if(bigTileDict)
    {
        CCDictElement* element;
        CCDICT_FOREACH(bigTileDict, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            int id = dictInfo->valueForKey("id")->intValue();
            auto bigTileInfo = BigTileInfo(dictInfo);
            m_bigTileMap[id] = bigTileInfo;
            
            //begin a by ljf
            auto tilePositionUnlockInfo = TilePositionUnlockInfo(dictInfo);
            vector<string> vecPosIds;
            CCCommonUtils::splitString(tilePositionUnlockInfo.positions, ";", vecPosIds);
            for (int i=0; i<vecPosIds.size(); ++i) {
                int id = atoi(vecPosIds[i].c_str());
                m_tilePositionUnlockMap[id] = tilePositionUnlockInfo; //以position为key建立map
            }
            //end a by ljf
        }
    }
    // tao.yu 暂时不需要talkTips表
//    auto talkTipsDict = LocalController::shared()->DBXMLManager()->getGroupByKey("talkTips");
//    if(talkTipsDict)
//    {
//        CCDictElement* element;
//        CCDICT_FOREACH(talkTipsDict, element)
//        {
//            CCDictionary* dictInfo = _dict(element->getObject());
//            string TipsId = dictInfo->valueForKey("id")->getCString();
//            int TipsType = dictInfo->valueForKey("type")->intValue();
//            if (TipsType == 0) {
//                talkTipsVec0.push_back(TipsId);
//            }else if (TipsType == 1) {
//                talkTipsVec1.push_back(TipsId);
//            }else if (TipsType == 2) {
//                talkTipsVec2.push_back(TipsId);
//            }else if (TipsType == 3) {
//                talkTipsVec3.push_back(TipsId);
//            }else if (TipsType == 4) {
//                talkTipsVec4.push_back(TipsId);
//            }else if (TipsType == 5) {
//                talkTipsVec5.push_back(TipsId);
//            }
//        }
//    }
    
    initPosPath();
}

FunBuildController::~FunBuildController() {
}

FunBuildController* FunBuildController::getInstance() {
    if (!_instance) {
        _instance = new FunBuildController();
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FunBuildController::updateResPreSed), _instance, 1.0,kCCRepeatForever, 0.0f, false);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(_instance, callfuncO_selector(FunBuildController::onShowMarkLineNotice), MSG_MARK_LINE_AL, NULL);
    }
    return _instance;
}

void FunBuildController::purgeData() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(FunBuildController::updateResPreSed), _instance);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(_instance, MSG_MARK_LINE_AL);
    delete _instance;
    _instance = NULL;
}

string FunBuildController::getBuildByPos(int pos)
{
    string ret = "";
    if (posToTypeMap.find(pos) != posToTypeMap.end()) {
        ret = posToTypeMap[pos];
    }
    return ret;
}

int FunBuildController::getBuildKey(int type, int pos)
{
    return type*1000+pos;
}

void FunBuildController::getDataFromServer()
{
    FunBuildAllCommand * cmd=new FunBuildAllCommand(PALACE_SHOW_COMMAND);
    cmd->sendAndRelease();
}

int FunBuildController::getUpRoCrtNum()
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
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
//ljf, 从服务器中返回的从信息中，更新已经解锁的tile
void FunBuildController::initOpenTile(string tiles)
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

void FunBuildController::refreshResTime(CCDictionary* dict)//城市被攻击后，资源田设置被掠夺后的资源
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
        
        if(curBuildsInfo->find(key) != curBuildsInfo->end())
        {
            auto& tmpInfo = (*curBuildsInfo)[key];
            tmpInfo.refreshTime = time;
        }
    }
    
    updateHavestPush();
}

void FunBuildController::retDataFromServer(CCDictionary* dict)
{
    isGetAllFunBuilds = true;
    
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("building"));
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
        if (GlobalData::shared()->imperialInfo.find(key) != GlobalData::shared()->imperialInfo.end()) {
            info = (GlobalData::shared()->imperialInfo)[key];
            info.SetInfoFromXml(xmlDictInfo);
        }else {
            info = FunBuildInfo(xmlDictInfo);
        }
        
        info.SetInfoFromServer(item);
        info.itemId = key;
        //分析提取引导数据
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        string GuideBuildKey = "";
        if (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0) //fusheng 新手引导提取数据
        {
//fusheng d 没有骑兵营  建造步兵营后不用再建造
//            int tmpQid1 = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORCE);//骑兵队列 待秒cd
//            int tmpQid2 = QueueController::getInstance()->getCanRecQidByType(TYPE_FORCE);//骑兵队列 待收兵
//            int totoalArmy = ArmyController::getInstance()->getTotalArmy();
//            if (totoalArmy > 1 || tmpQid2 != QID_MAX || tmpQid1 != QID_MAX) {//数据修复,造兵了,但是清数据了
//                gFake = "start_4";
//                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_4");
//                CCUserDefault::sharedUserDefault()->flush();
//            }
            
            if(gFake=="" || gFake=="start_1")//提取4个建筑数据
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_1");
                CCUserDefault::sharedUserDefault()->flush();
                if (id==FUN_BUILD_WOOD && pos==17 && level==1) {//伐木场
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
//                else if (id==FUN_BUILD_BARRACK2 && pos==11 && level==1) {//骑兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
                else if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
                else if (id==FUN_BUILD_BARRACK2 && pos==10 && level==1) {//步兵营
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
            }
            else if(gFake=="end_1" || gFake=="start_2")//提取3个建筑数据
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_2");
                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_BARRACK2 && pos==11 && level==1) {//骑兵营
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_FOOD && pos==17 && level==1) {//农田
                if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
                else if (id==FUN_BUILD_BARRACK2 && pos==10 && level==1) {//步兵营
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
            }
            else if(gFake=="end_2" || gFake=="start_3")//提取2个建筑数据
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","start_3");
                CCUserDefault::sharedUserDefault()->flush();
//                if (id==FUN_BUILD_FOOD && pos==21 && level==1) {//农田
//                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
//                    m_guideBuildMap[GuideBuildKey] = info;
//                    continue;
//                }
//                else if (id==FUN_BUILD_BARRACK1 && pos==10 && level==1) {//步兵营
                if (id==FUN_BUILD_BARRACK2 && pos==10 && level==1) {//步兵营
                    GuideBuildKey = GuideBuildKey + CC_ITOA(id)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(level);
                    m_guideBuildMap[GuideBuildKey] = info;
                    continue;
                }
            }
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
        }
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
        
        (GlobalData::shared()->imperialInfo)[key] = info;
    }
    
    updateAllOutPut();
    updateHavestPush();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRANSITION_FINISHED);
}

void FunBuildController::retSetData(CCDictionary* dict)
{
    isError = false;
    //特殊情况， 主城升级错误，导致 开启建筑 会 发生错误。(目前，主城升级错误，不会触发开启新建筑)
    GlobalData::shared()->playerInfo.gold = dict->valueForKey("gold")->intValue();
    auto cityInfo = _dict(dict->objectForKey("resource"));
    GlobalData::shared()->resourceInfo.setResourceData(cityInfo);
    
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
        
        if(curBuildsInfo->find(id) != curBuildsInfo->end())
        {
            auto& tmpInfo = (*curBuildsInfo)[id];
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
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
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
//begin a by ljf
//从spebuild的onclickthis传过来的itemId为ItemSpec id="9990"这个字段
//从funbuild的onclickthis传过来的itemId为ItemSpec id="9990"这个字段
//end a by ljf
bool FunBuildController::startOpenTile(int itemId, int gold)
{
    auto& info = m_bigTileMap[itemId];
    //auto& info = m_tilePositionUnlockMap[itemId]; //a by ljf
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

void FunBuildController::endOpenTile(CCDictionary* dict)
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
        GlobalData::shared()->resourceInfo.setResourceData(dict);
        int a = curFunBuildId; //a by ljf
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

void FunBuildController::testOpenTile(int itemId)
{
    curFunBuildId = itemId;
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

bool FunBuildController::startOpenFunBuild(int itemId, int pos, int gold, bool isForce)
{
    if (itemId<=0) {
        return false;
    }
    int key = getBuildKey(itemId,pos);
    CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(itemId));
    auto info = FunBuildInfo(xmlDictInfo);
    int tmpTime = info.time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, tmpTime);
    if(QID_MAX == qid && gold<=0){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
    if (gold<=0) {
        if (GlobalData::shared()->resourceInfo.lWood<info.wood_need
            || GlobalData::shared()->resourceInfo.lFood<info.food_need
            || GlobalData::shared()->resourceInfo.lStone<info.stone_need
            || GlobalData::shared()->resourceInfo.lIron<info.iron_need) {
            CCCommonUtils::flyText(_lang("102197"));
            return false;
        }
    }
    
    if(!info.isUnLock()) {
        CCCommonUtils::flyText(_lang("102196"));
        return false;
    }
    
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
    (GlobalData::shared()->imperialInfo)[key] = info;
    
    //引导过程中， 假建造
    string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
    string GuideBuildKey = "";
    GuideBuildKey = GuideBuildKey + CC_ITOA(itemId)+"_"+CC_ITOA(pos)+"_"+CC_ITOA(1);
    if (m_guideBuildMap.find(GuideBuildKey) != m_guideBuildMap.end())
    {
        bool gFakeState = false;
        //fushen d
//        if ((gFake==""||gFake=="start_1") && itemId==FUN_BUILD_WOOD && pos==26) {
//            gFakeState = true;
//            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_1");
//        }
//        else if ((gFake=="end_1"||gFake=="start_2") && itemId==FUN_BUILD_BARRACK2 && pos==11) {
//            gFakeState = true;
//            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_2");
//        }
//        else if ((gFake=="end_2"||gFake=="start_3") && itemId==FUN_BUILD_FOOD && pos==21) {
//            gFakeState = true;
//            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_3");
//        }
//        else if ((gFake=="end_3"||gFake=="start_4") && itemId==FUN_BUILD_BARRACK1 && pos==10) {
//            gFakeState = true;
//            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_4");
//        }
        
//        fusheng begin
        if ((gFake==""||gFake=="start_1") && itemId==FUN_BUILD_WOOD && pos==17) {
            gFakeState = true;
            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_1");
        }
        else if ((gFake=="end_1"||gFake=="start_2") && itemId==FUN_BUILD_FOOD && pos==21) {
            gFakeState = true;
            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_2");
        }
        else if ((gFake=="end_2"||gFake=="start_3") && itemId==FUN_BUILD_BARRACK2 && pos==10) {//fusheng 引导切记修改这里
            gFakeState = true;
            CCUserDefault::sharedUserDefault()->setStringForKey("Guide_Fake","end_3");
        }

//       fusheng end 
        if (gFakeState) {
            CCUserDefault::sharedUserDefault()->flush();
            
            QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
            curFunBuildId = key;
            info.is_Dirc = false;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_TIME_BAR, CCInteger::create(curFunBuildId));
            makeGuideBuildData(itemId, pos, 1);
            return true;
        }
    }
    //假建造结束
    
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
    if (isForce) {
        paramGold = 2;
    }
    FunBuildCreate * cmd=new FunBuildCreate(itemId, pos, PALACE_CT_COMMAND, paramGold);
    cmd->sendAndRelease();
    QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
    curFunBuildId = key;
    
    info.is_Dirc = false;
    if(gold<=0 || isForce) {
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

void FunBuildController::endOpenFunBuild(CCDictionary* dict, int type)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
        
        int itemId = curFunBuildId;
        if (curUpBuildsInfo->find(itemId)==curUpBuildsInfo->end()) {
            auto& info = (*curBuildsInfo)[itemId];
            info.isError = 1;
            if(itemId/1000 != FUN_BUILD_MAIN)
            {
                info.level += 1;
            }
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
            int tk = FunBuildController::getInstance()->getBuildKey(itemId, pos);
            FunBuildController::getInstance()->pushBuildQueue(dict);
            (*curBuildsInfo)[tk].state = FUN_BUILD_CHANGE;
            (*curBuildsInfo)[tk].updateTime = GlobalData::shared()->getWorldTime();
            (*curBuildsInfo)[tk].uuid = dict->valueForKey("uuid")->getCString();
            (*curBuildsInfo)[tk].is_Dirc = false;
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
            if (curBuildsInfo->find(id) != curBuildsInfo->end()) {
                auto &resInfo = curBuildsInfo->at(id);
                resInfo.updateTime = tmp;
                resInfo.uuid = (*curUpBuildsInfo)[id].uuid;
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("BC_%d_build", id/1000));
        }
    }
    curFunBuildId = 0;
}

bool FunBuildController::startUpFunBuild(int itemId, int gold, bool isForce, int isUpStar)
{
    if(curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        CCLOG("not exist %d", itemId);
        return false;
    }
    auto &info = (*curBuildsInfo)[itemId];
    if (info.uuid == "") {
        getDataFromServer();
        return false;
    }
    
    if(info.type == FUN_BUILD_HOSPITAL) {
        if (ArmyController::getInstance()->getTotalTreat()>0) {
            CCCommonUtils::flyText(_lang("102158"));
            return false;
        }
    }
    else if(info.type == FUN_BUILD_SCIENE) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
        if (qid != QID_MAX) {
            string key = GlobalData::shared()->allQueuesInfo[qid].key;
            int scienceId = QueueController::getInstance()->getItemId(key);
            string scienceName = CCCommonUtils::getNameById(CC_ITOA(scienceId));
            CCCommonUtils::flyText(_lang_1("102157", scienceName.c_str()));
            return false;
        }
    }
    else if(info.type == FUN_BUILD_FORGE) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
        if (qid != QID_MAX) {
            string key = GlobalData::shared()->allQueuesInfo[qid].key;
            int equpId = QueueController::getInstance()->getItemId(key);
            string equpName = CCCommonUtils::getNameById(CC_ITOA(equpId));
            CCCommonUtils::flyText(_lang_1("102157", equpName.c_str()));
            return false;
        }
    }
    else if(info.type == FUN_BUILD_WORKSHOP) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
        if (qid != QID_MAX) {
            string key = GlobalData::shared()->allQueuesInfo[qid].key;
            int mateId = QueueController::getInstance()->getItemId(key);
            string mateName = CCCommonUtils::getNameById(CC_ITOA(mateId));
            CCCommonUtils::flyText(_lang_1("102157", mateName.c_str()));
            return false;
        }
    }
    else if(info.type == FUN_BUILD_BARRACK1 || info.type == FUN_BUILD_BARRACK2 || info.type == FUN_BUILD_BARRACK3 || info.type == FUN_BUILD_BARRACK4) {
        int type = TYPE_FORCE;
        if (info.type == FUN_BUILD_BARRACK2) {
            type = TYPE_RIDE_SOLDIER;
        }
        else if (info.type == FUN_BUILD_BARRACK3) {
            type = TYPE_BOW_SOLDIER;
        }
        else if (info.type == FUN_BUILD_BARRACK4) {
            type = TYPE_CAR_SOLDIER;
        }
        int qid = QueueController::getInstance()->getMinTimeQidByType(type);
        if (qid != QID_MAX) {
            string key = GlobalData::shared()->allQueuesInfo[qid].key;
            int armyId = QueueController::getInstance()->getItemId(key);
            string armyName = CCCommonUtils::getNameById(CC_ITOA(armyId));
            CCCommonUtils::flyText(_lang_2("102156", _lang(info.name).c_str(), armyName.c_str()));
            return false;
        }
    }
    
    int tmpTime = info.time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, tmpTime);
    if(QID_MAX == qid && gold==0){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return false;
    }
    
    if(gold<=0){
        if (GlobalData::shared()->resourceInfo.lWood<info.wood_need
            || GlobalData::shared()->resourceInfo.lFood<info.food_need
            || GlobalData::shared()->resourceInfo.lStone<info.stone_need
            || GlobalData::shared()->resourceInfo.lIron<info.iron_need) {
            CCCommonUtils::flyText(_lang("102197"));
            return false;
        }
    }
    
    if(!info.isUnLock()) {
        CCCommonUtils::flyText(_lang("102196"));
        return false;
    }
    
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
    if (isForce) {
        paramGold = 2;
    }
    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, PALACE_UP_COMMAND, paramGold, isUpStar);
    cmd->sendAndRelease();
    QueueController::getInstance()->updateQueueInfo(qid, info.updateTime, info.itemId);
    curFunBuildId = itemId;
    
    if(info.type == FUN_BUILD_MAIN && info.level==5) {//5级大本升级时记录，用来触发引导使用
        string key = GlobalData::shared()->playerInfo.uid + "_" +CC_ITOA(info.level);
        CCUserDefault::sharedUserDefault()->setStringForKey(GUIDE_MAINCITY_LEVEL_UP, key);
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    if(curFunBuildId == FUN_BUILD_MAIN_CITY_ID){
        AppLibHelper::triggerEventMainBuildingLevel(info.level+1);
    }
    
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

void FunBuildController::endUpFunBuild(CCDictionary* dict, int type)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
        
        int itemId = curFunBuildId;
        if (curUpBuildsInfo->find(itemId)==curUpBuildsInfo->end()) {
            auto& info = (*curBuildsInfo)[itemId];
            info.isError = 1;
            if(itemId/1000 != FUN_BUILD_MAIN)
            {
                info.level += 1;
            }
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
            int tk = FunBuildController::getInstance()->getBuildKey(itemId, pos);
            FunBuildController::getInstance()->pushBuildQueue(dict);
            (*curBuildsInfo)[tk].state = FUN_BUILD_UPING_END;
            (*curBuildsInfo)[tk].is_Dirc = false;
            (*curBuildsInfo)[tk].updateTime = GlobalData::shared()->getWorldTime();
            
            if(tk == 400000000)
            {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_SPEED_UP_COMPLETE,CCString::create("upgrade"));//fusheng 用金币直接升级
                
                if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
                {
                    
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA(400000000));
                 
                    QueueController::getInstance()->startFinishQueue(qid,false);//fusheng 在世界里升级  取消升级队列占用
                }
                    
            }
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
            (*curBuildsInfo)[id].updateTime = tmp;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE,
                                                                                   CCString::createWithFormat("BC_%d_up", id / 1000));
        }
    }
    curFunBuildId = 0;
}

bool FunBuildController::startDestroyBuild(int itemId)
{
    auto &info = (*curBuildsInfo)[itemId];
    if (info.uuid=="") {
        getDataFromServer();
        return false;
    }
    
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING,info.des_time);
    if(QID_MAX == qid){
        CCCommonUtils::flyText(_lang("102152"));
        return false;
    }
    
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
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

void FunBuildController::endDestroyBuild(CCDictionary* dict)
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
        (*curUpBuildsInfo)[id] = FunBuildInfo();
        (*curBuildsInfo)[id].updateTime = tmp;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME, CCInteger::create(id));
    }
    curFunBuildId = 0;
}

void FunBuildController::cancelBuildState(int itemId)
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

void FunBuildController::pushBuildQueue(CCDictionary* dict)
{
    curUpBuildsInfo = &(GlobalData::shared()->UpgradeBuildsInfo);
    int tid = dict->valueForKey("itemId")->intValue();
    int pos = dict->valueForKey("pos")->intValue();
    int id = getBuildKey(tid, pos);
    (*curUpBuildsInfo)[id].SetInfoFromServer(dict);
}

void FunBuildController::synTimeQueueToBuild(int itemId)
{
}

int FunBuildController::getBuildNumByType(int type)
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type){
            ret++;
        }
    }
    return ret;
}

int FunBuildController::getMaxLvBuildByType(int type)
{
    int retBuildID = 0;
    int lv = -1;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type && lv<(it->second).level){
            lv = (it->second).level;
            retBuildID = it->first;
        }
    }
    return retBuildID;
}

bool FunBuildController::isExistBuildByTypeLv(int type, int lv)
{
    bool ret = false;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).type == type && (it->second).level>=lv && ((it->second).state==FUN_BUILD_NORMAL || (it->second).state==FUN_BUILD_UPING)){
            ret=true;
            break;
        }
    }
    return ret;
}

void FunBuildController::initSetUpBuildingInfo(CCDictionary* dict)
{
    curUpBuildsInfo = &(GlobalData::shared()->UpgradeBuildsInfo);
    int tid = dict->valueForKey("itemId")->intValue();
    int pos = dict->valueForKey("pos")->intValue();
    int id = getBuildKey(tid, pos);
    (*curUpBuildsInfo)[id].SetInfoFromServer(dict);
    (*curUpBuildsInfo)[id].updateTime = 0;
}

bool FunBuildController::canHavest(int itemId)
{
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return false;
    }
    auto &info = (*curBuildsInfo)[itemId];
    
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

bool FunBuildController::startGetFunBuildProduct(int itemId)
{
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return false;
    }
    auto &info = (*curBuildsInfo)[itemId];
    
//    if (!canHavest(info.itemId)) {
//        return false;
//    }
    curHarvestBuildsInfo = info;
    
//    int output = getOutPut(info.itemId);
//    SceneController::getInstance()->onFlyOutPut(info.itemId, output);
    
    info.refreshTime = GlobalData::shared()->getWorldTime();
    
    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, PALACE_GET_COMMAND);
    cmd->sendAndRelease();
    AllianceManager::getInstance()->showRecAlliance();
    
    if (isError) {
        getDataFromServer();
    }
    
    return true;
}

void FunBuildController::endGetFunBuildProduct(CCDictionary* dict)
{
    if (dict) {
        int output=0;
        if (curHarvestBuildsInfo.type == FUN_BUILD_WOOD)
        {
            output=dict->valueForKey("wood")->intValue()-GlobalData::shared()->resourceInfo.lWood;
        }
        else if (curHarvestBuildsInfo.type == FUN_BUILD_FOOD)
        {
            output=dict->valueForKey("food")->intValue()-GlobalData::shared()->resourceInfo.lFood;
        }
        else if (curHarvestBuildsInfo.type == FUN_BUILD_IRON)
        {
            output=dict->valueForKey("iron")->intValue()-GlobalData::shared()->resourceInfo.lIron;
        }
        else if (curHarvestBuildsInfo.type == FUN_BUILD_STONE)
        {
            output=dict->valueForKey("stone")->intValue()-GlobalData::shared()->resourceInfo.lStone;
        }

        SceneController::getInstance()->onFlyOutPut(curHarvestBuildsInfo.itemId, output,-2);
        
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(dict);
        
        updateHavestPush();
    }
}

bool FunBuildController::costCD(int itemId, string toolUUID, int gold,int toolCnt)
{
    auto& info = (*curBuildsInfo)[itemId];
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
        toolCnt = (toolCnt==0)?1:toolCnt;
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(atoi(toolUUID.c_str()));
        if (toolInfo.getCNT() < toolCnt) {
            CCCommonUtils::flyText(_lang("102198"));
            return false;
        }
        int tCnt = toolInfo.getCNT() - toolCnt;
        toolInfo.setCNT(tCnt);
        spdTime = atoi(toolInfo.paras[3].c_str()) * toolCnt;
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
        QueueController::getInstance()->startCCDQueue(qid, toolUUID, isFinish, gold,markstr,1,"",toolCnt);
    }
    
    if (isError) {
        getDataFromServer();
    }
    
    return true;
}

void FunBuildController::retCostCD(CCDictionary* dict)
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

bool FunBuildController::startFinish(int itemId)
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
    
//    auto& info = (*curBuildsInfo)[itemId];
//    FunBuildCommand * cmd=new FunBuildCommand(info.uuid, PALACE_FINISH_COMMAND);
//    cmd->sendAndRelease();
    return true;
}

void FunBuildController::onCreateOrUpError()
{
    isError = true;
    getDataFromServer();
}

bool FunBuildController::completeDestroy(int itemId)
{
    if (curUpBuildsInfo->find(itemId)==curUpBuildsInfo->end()) {
        auto& info = (*curBuildsInfo)[itemId];
        info.isError = 1;
        info.state = FUN_BUILD_NORMAL;
        onCreateOrUpError();
        return false;
    }
    return true;
}

bool FunBuildController::completeUpOrCreate(int itemId, bool isCreate)
{
    if (curUpBuildsInfo->find(itemId)==curUpBuildsInfo->end()) {
        auto& info = (*curBuildsInfo)[itemId];
        info.isError = 1;
        if(itemId/1000 != FUN_BUILD_MAIN)
        {
            info.level += 1;
        }
        info.state = FUN_BUILD_NORMAL;
        onCreateOrUpError();
    }
    else {
        updateGeneralData(itemId);
        auto& info = (*curBuildsInfo)[itemId];
        auto& nextInfo = (*curUpBuildsInfo)[itemId];
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
        
        if(info.type == FUN_BUILD_WALL) {
            GlobalData::shared()->cityDefenseVal += (nextInfo.para2 - info.para2);
            if(info.level == GlobalData::shared()->shield_base){
                GlobalData::shared()->playerInfo.protectTimeStamp = 0;
            }
        }
        
        info.starNum = nextInfo.starNum;
        info.starRate = nextInfo.starRate;
        
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
        
//        if(info.level==1 && (info.type==1 || info.type==2 || info.type==2))
//        {//刚建筑完的 资源建筑，默认有4%的产量
//            int tmpLs = getMaxOutPut(info.itemId)*0.04*3600/getOutPutPreH(info.itemId);
//            info.refreshTime = GlobalData::shared()->getWorldTime()-tmpLs;
//            info.refreshTime = 0;
//        }
        
        if(info.type == FUN_BUILD_FOOD || info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_STONE || info.type == FUN_BUILD_IRON) {
            if (isCreate && info.refreshTime>0 && info.refreshTime<GlobalData::shared()->getWorldTime()) {
            }
            else {
                info.refreshTime = GlobalData::shared()->getWorldTime();
                updateHavestPush();
            }
        }
        
        updateDataAboutBuild(itemId);
        //缓存一下大本数据
        if(info.type == FUN_BUILD_MAIN) {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, info.level);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
    return true;
}

void FunBuildController::updateGeneralData(int itemId){
}

void FunBuildController::updateDataAboutBuild(int itemId)
{
    GuideController::share()->buildingUpdate(itemId);

    auto &info = (*curBuildsInfo)[itemId];
    if (info.type == 4) {
        GlobalData::shared()->resourceInfo.lStorageMoney = getStrory(itemId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    else if (info.type == 5) {
        GlobalData::shared()->resourceInfo.lStorageWood = getStrory(itemId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    else if (info.type == 6) {
        GlobalData::shared()->resourceInfo.lStorageIron = getStrory(itemId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    if (itemId/1000 == FUN_BUILD_TEMPLE) {//神庙（祭祀）
        if (info.level == 1) {
//            UIComponent::getInstance()->setBuyMoneyBtnState();
        }
    }
    else if(itemId/1000 == FUN_BUILD_STABLE){
        if(info.level == 1){
            GlobalData::shared()->lastAcceptEnergyTime = GlobalData::shared()->getWorldTime();
        }
    }
    else if(itemId/1000 == FUN_BUILD_MAIN) {//主城升级完成后，更新可开启建筑
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FUNBUILD_OPEN);
        UIComponent::getInstance()->showResBar();
        UIComponent::getInstance()->onOpenShowBuildQueue2();
        if (info.level == 5) {
            UIComponent::getInstance()->checkShowQuestPrc();
        }
        if (info.level == 3) {
            UIComponent::getInstance()->showFlygold();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_TALK);
        }
        if (info.level >= 6 && GlobalData::shared()->isOpenEvaluate && !GlobalData::shared()->isXiaoMiPlatForm())
        {
            auto view = EvaluationView::create();
            if (GuideController::share()->isInTutorial()) {
                PopupViewController::getInstance()->pushPop(view, true);
            }else {
                PopupViewController::getInstance()->addPopupView(view);
            }
        }
        DailyActiveController::shared()->setHasUnlockItem(true);
        
    }
    updateAllOutPut();
}

void FunBuildController::clearUpBuildingInfo(int itemid)
{
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *tmpMap = (&GlobalData::shared()->UpgradeBuildsInfo);
    
    for (it = tmpMap->begin(); it != tmpMap->end(); it++) {
        if ((it->second).itemId == itemid)
        {
            tmpMap->erase(it);
            break;
        }
    }
}

int FunBuildController::getMainCityLv()
{
    if(curBuildsInfo->find(FUN_BUILD_MAIN_CITY_ID) == curBuildsInfo->end()) {
        return 1;
    }
    auto info = getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    return info.level;
}

int FunBuildController::getMaxOutPut(int itemId)
{
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return 0;
    }
    auto &info = (*curBuildsInfo)[itemId];
    if (!(info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_FOOD || info.type == FUN_BUILD_IRON || info.type == FUN_BUILD_STONE))
    {
        return 0;
    }
    
    float pro = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    int retMax = info.para2 * pro;
    return retMax;
}

int FunBuildController::getOutPut(int itemId)
{
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return 0;
    }
    float pro1 = 0;
    float pro2 = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    auto &info = (*curBuildsInfo)[itemId];
    if (info.type == FUN_BUILD_WOOD)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_FOOD)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_IRON)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_STONE)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0);
    }
    else {
        return 0;
    }
    double longTime;
    if(info.refreshTime>0)
    {
        longTime = GlobalData::shared()->getWorldTime() - info.refreshTime;
    }
    else
    {
        longTime = 5;
    }
    int ret = longTime*1.0/3600 * info.para1 * pro1;
    if (true) {
        int tmpT = 0;
        if (info.effectTime >= GlobalData::shared()->getWorldTime()) {
            tmpT = longTime;
        }
        else if (info.effectTime > info.refreshTime){
            tmpT = info.effectTime - info.refreshTime;
        }
        if(info.refreshTime!=0){
            ret += int((info.para1*1.0/3600)*tmpT*building_base_k7*1.0/100);
        }
    }
    
    int retMax = info.para2 * pro2;
    return ret<retMax?ret:retMax;
}

float FunBuildController::getOutPutPerSecond(int itemId){
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return 0;
    }
    float pro1 = 0;
    float pro2 = 1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100;
    auto &info = (*curBuildsInfo)[itemId];
    if (info.type == FUN_BUILD_WOOD)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_FOOD)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_IRON)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0);
    }
    else if (info.type == FUN_BUILD_STONE)
    {
        pro1 = MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0);
    }
    else {
        return 0;
    }
    
    auto longTime = 1;
    float ret = longTime*1.0/3600 * info.para1 * pro1; //只要基础和作用的 加倍的不要
    
    float retMax = info.para2 * pro2;
    return ret<retMax?ret:retMax;
}

bool FunBuildController::canShowOutPut(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    int limit = building_base_k1*1.0*stroge/100;
    if (curOutPut>=limit) {
        return true;
    }
    return false;
}

int FunBuildController::getFullCostTime(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    if (curOutPut<stroge) {
        float speed = getOutPutPerSecond(itemId);
        return (stroge-curOutPut)/speed;
    }
    return 0;
}

bool FunBuildController::isFullOutPut(int itemId)
{
    int curOutPut = getOutPut(itemId);
    int stroge = getMaxOutPut(itemId);
    if (curOutPut>=stroge) {
        return true;
    }
    return false;
}

int FunBuildController::getStrory(int itemId)
{
    return 0;
}

int FunBuildController::getOutPutPreH(int itemId)
{
    if (curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        return 0;
    }
    auto &info = (*curBuildsInfo)[itemId];
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

FunBuildInfo& FunBuildController::getFunbuildById(int itemId)
{
    if(curBuildsInfo->find(itemId) == curBuildsInfo->end()) {
        CCLOG("error error: this build error : %d", itemId);
    }
    FunBuildInfo& ret = (curBuildsInfo->find(itemId))->second;
    return ret;
}

void FunBuildController::updateAllOutPut()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    curFoodOutPreSed = -ArmyController::getInstance()->getTotalUpKeep()*1.0/3600;
    
    int sumSliver = 0;
    int sumStone = 0;
    int sumIron = 0;
    int sumFood = 0;
    int sumWood = 0;
//
//    int strogeSliver = 0;
//    int strogeStone = 0;
//    int strogeIron = 0;
//    int strogeFood = 0;
//    int strogeWood = 0;
//    
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL)
        {
            if ((it->second).type == FUN_BUILD_SILVER) {
                sumSliver += atoi( (it->second).para[1].c_str() );
//                strogeSliver += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_STONE) {
                sumStone += atoi( (it->second).para[1].c_str() );
//                strogeStone += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_IRON) {
                sumIron += atoi( (it->second).para[1].c_str() );
//                strogeIron += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_FOOD) {
                sumFood += atoi( (it->second).para[1].c_str() );
//                strogeFood += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_WOOD) {
                sumWood += atoi( (it->second).para[1].c_str() );
//                strogeWood += atoi( (it->second).para[2].c_str() );
            }
        }
    }

    oriWoodPreH = sumWood;
    oriStonePreH = sumStone;
    oriIronPreH = sumIron;
    oriFoodPreH = sumFood;
    oriSliverPreH = sumSliver;
    
    SliverOutPreSed = sumSliver*(MAX((100+CCCommonUtils::getEffectValueByNum(54))*1.0/100, 0))*1.0/3600;
    StoneOutPreSed = sumStone*(MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0))*1.0/3600;
    WoodOutPreSed = sumWood*(MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0))*1.0/3600;
    FoodOutPreSed = sumFood*(MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0))*1.0/3600;
    IronOutPreSed = sumIron*(MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0))*1.0/3600;
//
//    curFoodOutPreSed = (sumFood*(MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0))-ArmyController::getInstance()->getTotalUpKeep())*1.0/3600;
//    
//    GlobalData::shared()->resourceInfo.lStorageFood = strogeFood*(1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100);
//    GlobalData::shared()->resourceInfo.lStorageWood = strogeWood*(1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100);
//    GlobalData::shared()->resourceInfo.lStorageIron = strogeIron*(1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100);
//    GlobalData::shared()->resourceInfo.lStorageStone = strogeStone*(1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100);
//    GlobalData::shared()->resourceInfo.lStorageMoney = strogeSliver*(1.0+CCCommonUtils::getEffectValueByNum(132)*1.0/100);
}

void FunBuildController::addOutPut(int sec){
    float sumSliver = 0;
    float sumStone = 0;
    float sumIron = 0;
    float sumFood = 0;
    float sumWood = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL)
        {
            if ((it->second).type == FUN_BUILD_SILVER) {
                sumSliver += atoi( (it->second).para[1].c_str() );
                //                strogeSliver += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_STONE) {
                sumStone += atoi( (it->second).para[1].c_str() );
                //                strogeStone += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_IRON) {
                sumIron += atoi( (it->second).para[1].c_str() );
                //                strogeIron += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_FOOD) {
                sumFood += atoi( (it->second).para[1].c_str() );
                //                strogeFood += atoi( (it->second).para[2].c_str() );
            }
            else if ((it->second).type == FUN_BUILD_WOOD) {
                sumWood += atoi( (it->second).para[1].c_str() );
                //                strogeWood += atoi( (it->second).para[2].c_str() );
            }
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

void FunBuildController::updateResPreSed(float t)
{
    checkFreeTime();
    updateAllOutPut();
    
//    WoodOdd += WoodOutPreSed;
    FoodOdd += curFoodOutPreSed;
//    StoneOdd += StoneOutPreSed;
//    IronOdd += IronOutPreSed;
//    SliverOdd += SliverOutPreSed;
    
//    int addWood = WoodOdd;
//    WoodOdd -= addWood;
    int addFood = FoodOdd;
    FoodOdd -= addFood;
//    int addStone = StoneOdd;
//    StoneOdd -= addStone;
//    int addIron = IronOdd;
//    IronOdd -= addIron;
//    int addSliver = SliverOdd;
//    SliverOdd -= addSliver;
    
//    lastAddWood += addWood;
//    lastAddStone += addStone;
//    lastAddIron += addIron;
    lastAddFood += addFood;
    
    if (addFood >0 && GlobalData::shared()->resourceInfo.lFood >= GlobalData::shared()->resourceInfo.lStorageFood) {
        FoodOdd = addFood = 0;
    }
//    if (addWood>0 && GlobalData::shared()->resourceInfo.lWood >= GlobalData::shared()->resourceInfo.lStorageWood) {
//        WoodOdd = addWood = 0;
//    }
//    if (addIron>0 && GlobalData::shared()->resourceInfo.lIron >= GlobalData::shared()->resourceInfo.lStorageIron) {
//        IronOdd = addIron = 0;
//    }
//    if (addStone>0 && GlobalData::shared()->resourceInfo.lStone >= GlobalData::shared()->resourceInfo.lStorageStone) {
//        StoneOdd = addStone = 0;
//    }
//    if (addSliver>0 && GlobalData::shared()->resourceInfo.lMoney >= GlobalData::shared()->resourceInfo.lStorageMoney) {
//        SliverOdd = addSliver = 0;
//    }
    
    GlobalData::shared()->resourceInfo.lFood += addFood;
//    GlobalData::shared()->resourceInfo.lWood += addWood;
//    GlobalData::shared()->resourceInfo.lIron += addIron;
//    GlobalData::shared()->resourceInfo.lStone += addStone;
//    GlobalData::shared()->resourceInfo.lMoney += addSliver;
    
    if(GlobalData::shared()->resourceInfo.lFood<0) {
        GlobalData::shared()->resourceInfo.lFood = 0;
    }
//    if(GlobalData::shared()->resourceInfo.lWood<0) {
//        GlobalData::shared()->resourceInfo.lWood = 0;
//    }
//    if(GlobalData::shared()->resourceInfo.lIron<0) {
//        GlobalData::shared()->resourceInfo.lIron = 0;
//    }
//    if(GlobalData::shared()->resourceInfo.lStone<0) {
//        GlobalData::shared()->resourceInfo.lStone = 0;
//    }
//    if(GlobalData::shared()->resourceInfo.lMoney<0) {
//        GlobalData::shared()->resourceInfo.lMoney = 0;
//    }
    
    if(fabs(lastAddFood)>=1) //fabs(lastAddWood)>=1 || fabs(lastAddStone)>=1 || fabs(lastAddIron)>=1 || fabs(lastAddFood)>=1
    {
        if(addTime >=60) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            addTime = 0;
//            lastAddWood = 0;
//            lastAddStone = 0;
//            lastAddIron = 0;
            lastAddFood = 0;
        }
    }
    addTime++;
}

string FunBuildController::orderBuild(string buildings)
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
    for (int j=0; j<tmpBuildings.size(); j++) {
        CCDictionary* xmlDictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(tmpBuildings[j]));
        auto info = FunBuildInfo(xmlDictInfo);
        if (info.isUnLock()) {
            canBuild += ";" ;
            canBuild += CC_ITOA(tmpBuildings[j]);
        }
        else {
            notCanBuild += ";" ;
            notCanBuild += CC_ITOA(tmpBuildings[j]);
        }
    }
    
    ret = canBuild+notCanBuild;
    if (ret.length()>0) {
        ret = ret.substr(1,ret.length()-1);
    }
    return ret;
}


void FunBuildController::initPosPath()
{
    if (1) {//pos == 6
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1065,1461));
//        path.push_back(ccp(1020,615));
//        path.push_back(ccp(720,570));
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(1010);
        posToPath[6] = path;
        posToZorders[6] = zorders;
    }
    
    if (1) {//pos == 7
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1453,1484));
//        path.push_back(ccp(490,1210));
//        path.push_back(ccp(670,1120));
//        
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
        
        path.push_back(ccp(1620, 1453)); // p7_0
        
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(600);
//        zorders.push_back(600);
//        zorders.push_back(600);
//        zorders.push_back(900);
        posToPath[7] = path;
        posToZorders[7] = zorders;
    }
    
    if (1) {//pos == 8
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1994,1861));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
        
        path.push_back(ccp(2274, 1913)); // p8_0
        path.push_back(ccp(2467, 2038)); // p8_1
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(900);
        posToPath[8] = path;
        posToZorders[8] = zorders;
    }
    if (1) {//pos == 9
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(2210,2033));
//        path.push_back(ccp(720,1150));
//        
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
        
        path.push_back(ccp(2467, 2038)); // p8_1
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(700);
//        zorders.push_back(700);
//        zorders.push_back(900);
        posToPath[9] = path;
        posToZorders[9] = zorders;
    }
    if (1) {//pos == 10
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(2339,1827));
//        path.push_back(ccp(800,850));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(250,800));
        
        path.push_back(ccp(2467, 2038)); // p8_1
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(1000);
        posToPath[10] = path;
        posToZorders[10] = zorders;
    }
    if (1) {//pos == 11
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(2006,2254));
//        path.push_back(ccp(1000,990));
//        path.push_back(ccp(920,930));
//        
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
        
        path.push_back(ccp(2310, 2213)); // p13_2
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(800);
//        zorders.push_back(800);
//        zorders.push_back(800);
//        zorders.push_back(900);
        posToPath[11] = path;
        posToZorders[11] = zorders;
    }
    if (1) {//pos == 12
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1804,2120));
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
        
        path.push_back(ccp(2310, 2213)); // p13_2
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(900);
        posToPath[12] = path;
        posToZorders[12] = zorders;
    }
    if (1) {//pos == 13
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1536,2273));
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
        
        path.push_back(ccp(1744, 2252)); // p13_0
        path.push_back(ccp(1940, 2050)); // p13_1
        path.push_back(ccp(2310, 2213)); // p13_2
        
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(400);
//        zorders.push_back(900);
        posToPath[13] = path;
        posToZorders[13] = zorders;
    }
    if (1) {//pos == 14
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(805,2116));
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
        
        path.push_back(ccp(1111, 2154)); // p15_2
        path.push_back(ccp(1259, 1976)); // p15_3
        path.push_back(ccp(1103, 1885)); // p15_4
        path.push_back(ccp(1205, 1808)); // p15_5
        path.push_back(ccp(1015, 1582)); // p15_6
        path.push_back(ccp(1234, 1438)); // p15_7
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(300);
//        zorders.push_back(400);
//        zorders.push_back(900);
        posToPath[14] = path;
        posToZorders[14] = zorders;
    }
    if (1) {//pos == 15
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(479,2152));
//        path.push_back(ccp(1690,1320));
//        path.push_back(ccp(1630,1290));
//        path.push_back(ccp(1450,1380));
//        path.push_back(ccp(1310,1380));
//        path.push_back(ccp(970,1210));
//        path.push_back(ccp(970,1150));
        
//        path.push_back(ccp(700,1020));
//        path.push_back(ccp(540,920));
//        path.push_back(ccp(320,900));
//        path.push_back(ccp(220,790));
        
        path.push_back(ccp(763, 2174)); // p15_0
        path.push_back(ccp(919, 2075)); // p15_1
        path.push_back(ccp(1111, 2154)); // p15_2
        path.push_back(ccp(1259, 1976)); // p15_3
        path.push_back(ccp(1103, 1885)); // p15_4
        path.push_back(ccp(1205, 1808)); // p15_5
        path.push_back(ccp(1015, 1582)); // p15_6
        path.push_back(ccp(1234, 1438)); // p15_7
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(500);
//        zorders.push_back(900);
        posToPath[15] = path;
        posToZorders[15] = zorders;
    }
    if (1) {//pos == 16
        vector<cocos2d::CCPoint> path;
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
        
        path.push_back(ccp(1205, 1808)); // p15_5
        path.push_back(ccp(1015, 1582)); // p15_6
        path.push_back(ccp(1234, 1438)); // p15_7
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(400);
        zorders.push_back(900);
        posToPath[16] = path;
        posToZorders[16] = zorders;
    }
    
    

    if (1) {//pos == 52
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1020,650));
//        path.push_back(ccp(720,570));
        
        path.push_back(ccp(1015, 1582)); // p15_6
        path.push_back(ccp(1234, 1438)); // p15_7
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(1010);
        posToPath[52] = path;
        posToZorders[52] = zorders;
    }
    if (1) {//pos == 53
        vector<cocos2d::CCPoint> path;
//        path.push_back(ccp(1300,805));
//        path.push_back(ccp(1020,615));
//        path.push_back(ccp(720,570));
        
        path.push_back(ccp(513, 1773)); // p53_0
        path.push_back(ccp(775, 1668)); // p_53_1
        
        path.push_back(ccp(1015, 1582)); // p15_6
        path.push_back(ccp(1234, 1438)); // p15_7
        
        path.push_back(ccp(1319, 1499)); // p6_0
        path.push_back(ccp(1498, 1404)); // p6_1
        path.push_back(ccp(1734, 1506)); // p6_2
        path.push_back(ccp(1870, 1346)); // p6_3
        path.push_back(ccp(2822, 1860)); // p6_4
        path.push_back(ccp(2784, 2075)); // p6_5
        path.push_back(ccp(2492, 2179)); // p6_6
        path.push_back(ccp(2797, 2353)); // p6_7
        
        vector<int> zorders;
        zorders.push_back(1010);
        posToPath[53] = path;
        posToZorders[53] = zorders;
    }
    
//    int offset_x = 2900; // guo.jiang
//    int offset_y = 2600; // guo.jiang
//    
//    sqerPoints.push_back(ccp(290 + 125 + offset_x, 560 - 320 + offset_y));
//    sqerPoints.push_back(ccp(380 + 175 + offset_x, 515 - 370 + offset_y));
//    sqerPoints.push_back(ccp(470 + 255 + offset_x, 470 - 450 + offset_y));
//    sqerPoints.push_back(ccp(560 + 365 + offset_x, 425 - 560 + offset_y));
//    sqerPoints.push_back(ccp(650 + 365 + offset_x, 380 - 560 + offset_y));
//    sqerPoints.push_back(ccp(740 + 365 + offset_x, 335 - 560 + offset_y));
//    
//    sqerPoints.push_back(ccp(190 + offset_x, 515 + offset_y));
//    sqerPoints.push_back(ccp(280 + offset_x, 470 + offset_y));
//    sqerPoints.push_back(ccp(370 + offset_x, 425 + offset_y));
//    sqerPoints.push_back(ccp(460 + offset_x, 380 + offset_y));
//    sqerPoints.push_back(ccp(550 + offset_x, 335 + offset_y));
//    sqerPoints.push_back(ccp(640 + offset_x, 290 + offset_y));
//    sqerPoints.push_back(ccp(730 + offset_x, 245 + offset_y));
//    
//    sqerPoints.push_back(ccp(90 + offset_x, 470 + offset_y));
//    sqerPoints.push_back(ccp(180 + offset_x, 425 + offset_y));
//    sqerPoints.push_back(ccp(270 + offset_x, 380 + offset_y));
//    sqerPoints.push_back(ccp(360 + offset_x, 335 + offset_y));
//    sqerPoints.push_back(ccp(450 + offset_x, 290 + offset_y));
//    sqerPoints.push_back(ccp(540 + offset_x, 245 + offset_y));
//    sqerPoints.push_back(ccp(630 + offset_x, 200 + offset_y));
//    
//    sqerPoints.push_back(ccp(80 + offset_x, 380 + offset_y));
//    sqerPoints.push_back(ccp(170 + offset_x, 335 + offset_y));
//    sqerPoints.push_back(ccp(260 + offset_x, 290 + offset_y));
//    sqerPoints.push_back(ccp(350 + offset_x, 245 + offset_y));
//    sqerPoints.push_back(ccp(440 + offset_x, 200 + offset_y));
//    sqerPoints.push_back(ccp(530 + offset_x, 155 + offset_y));
//    
//    sqerPoints.push_back(ccp(70 + offset_x, 290 + offset_y));
//    sqerPoints.push_back(ccp(160 + offset_x, 245 + offset_y));
//    sqerPoints.push_back(ccp(250 + offset_x, 200 + offset_y));
//    sqerPoints.push_back(ccp(340 + offset_x, 155 + offset_y));
    
    sqerPoints.push_back(Vec2(3171, 2657)); // troop0
    sqerPoints.push_back(Vec2(3386, 2515)); // troop1
    sqerPoints.push_back(Vec2(3330, 2755)); // troop2
    sqerPoints.push_back(Vec2(3537, 2618)); // troop3
    sqerPoints.push_back(Vec2(3470, 2846)); // troop4
    sqerPoints.push_back(Vec2(3701, 2695)); // troop5
    sqerPoints.push_back(Vec2(3624, 2917)); // troop6
    sqerPoints.push_back(Vec2(3856, 2760)); // troop7
    sqerPoints.push_back(Vec2(3985, 2852)); // troop8
    sqerPoints.push_back(Vec2(3617, 2335)); // troop9
    sqerPoints.push_back(Vec2(3832, 2193)); // troop10
    sqerPoints.push_back(Vec2(3776, 2432)); // troop11
    sqerPoints.push_back(Vec2(3983, 2296)); // troop12
    sqerPoints.push_back(Vec2(3917, 2524)); // troop13
    sqerPoints.push_back(Vec2(4130, 2382)); // troop14
    sqerPoints.push_back(Vec2(4082, 2601)); // troop15
    sqerPoints.push_back(Vec2(4293, 2465)); // troop16
    sqerPoints.push_back(Vec2(4209, 2695)); // troop17
    sqerPoints.push_back(Vec2(4421, 2555)); // troop18
    sqerPoints.push_back(Vec2(4068, 2029)); // troop19
    sqerPoints.push_back(Vec2(4283, 1888)); // troop20
    sqerPoints.push_back(Vec2(4227, 2127)); // troop21
    sqerPoints.push_back(Vec2(4434, 1990)); // troop22
    sqerPoints.push_back(Vec2(4368, 2218)); // troop23
    sqerPoints.push_back(Vec2(4599, 2067)); // troop24
    sqerPoints.push_back(Vec2(4528, 2306)); // troop25
    sqerPoints.push_back(Vec2(4744, 2160)); // troop26
    sqerPoints.push_back(Vec2(4566, 2626)); // troop27
    sqerPoints.push_back(Vec2(5124, 2001)); // troop28
    sqerPoints.push_back(Vec2(5331, 1864)); // troop29
    
    // guo.jiang
//    for (auto it = posToPath.begin(); it != posToPath.end(); ++it) {
//        CCLOG("\n    var itemIndex_Path_%d = library.findItemIndex(\"Path_%d\");", it->first, it->first);
//        CCLOG("    fl.drawingLayer.beginDraw(true);fl.drawingLayer.beginFrame();fl.drawingLayer.setColor( \"#ff0000\" );");
//        Point* last = nullptr;
//        for (auto i = it->second.begin(); i != it->second.end(); ++i) {
//            if (last == nullptr) {
//                last = &(*i);
//            } else {
//                CCLOG("    fl.drawingLayer.moveTo(%d, %d); // (%d, 3510-%d)", (int)last->x, 3510-(int)last->y, (int)last->x, (int)last->y);
//                CCLOG("    fl.drawingLayer.lineTo(%d, %d); // (%d, 3510-%d)", (int)(*i).x, 3510-(int)(*i).y, (int)(*i).x, (int)(*i).y);
//                last = &(*i);
//            }
//            CCLOG("    document.addItem({x:%d, y:%d}, library.items[itemIndex_Path_%d]); // (%d, 3510-%d)", (int)(*i).x, 3510-(int)(*i).y, it->first, (int)(*i).x, (int)(*i).y);
//        }
//        CCLOG("    fl.drawingLayer.endFrame();fl.drawingLayer.endDraw();");
//    }
//    CCLOG("\n");
//    CCLOG("    var itemIndex_Mark = library.findItemIndex(\"Mark\");");
//    for (int i = 0; i < sqerPoints.size(); i++) {
//        CCLOG("    document.addItem({x:%d, y:%d}, library.items[itemIndex_Mark]); // (%d, 3510-%d)", (int)sqerPoints[i].x, 3510-(int)sqerPoints[i].y, (int)sqerPoints[i].x, (int)sqerPoints[i].y);
//        CCLOG("    document.addItem({x:%d, y:%d}, library.items[itemIndex_Mark]); // (%d, %d)", (int)sqerPoints[i].x, (int)sqerPoints[i].y, (int)sqerPoints[i].x, (int)sqerPoints[i].y);
//    }
//    CCLOG("");
}

void FunBuildController::checkFreeTime(bool stat)
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

int FunBuildController::getCanBoostBuild(int buildType)
{
    int retBuildID = 0;
    int lv = -1;
    int curTime = GlobalData::shared()->getWorldTime();
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
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

int FunBuildController::findBdRoPos()
{
    int ret = 0;
    
    int tmpBid = 0;
    vector<int> vecBids;
    vecBids.push_back(FUN_BUILD_BARRACK1);
    vecBids.push_back(FUN_BUILD_BARRACK2);
    vecBids.push_back(FUN_BUILD_BARRACK3);
    vecBids.push_back(FUN_BUILD_BARRACK4);
    for (int i=0; i<vecBids.size(); i++) {
        tmpBid = getMaxLvBuildByType(vecBids[i]);
        if (tmpBid>0) {
            auto& info = getFunbuildById(tmpBid);
            if (info.state == FUN_BUILD_CREATE && info.level<=1) {
                ret = tmpBid;
            }
            else {
                return 0;
            }
        }
    }
    
    if (ret>0) {
        return ret;
    }
    
    string vecHavePos = "";
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).pos >= 5 && (it->second).pos <= 16){
            vecHavePos = vecHavePos+CC_ITOA((it->second).pos)+",";
        }
    }
    
    if (vecHavePos.find("11") == -1 ) {
        ret = 11;
        return ret;
    }
    
    for (int i=6; i<=16; i++) {
        string ti = CC_ITOA(i);
        if (vecHavePos.find(ti.c_str()) == -1 ) {
            ret = i;
            break;
        }
    }
    
    return ret;
}

int FunBuildController::getBuildIdByPos(int pos)
{
    int ret = 0;
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        if ((it->second).pos == pos){
            ret = it->first;
            break;
        }
    }
    return ret;
}

string FunBuildController::getTalkTips()
{
    string ret;
    vector<string> tmpVec;
    
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, 1);
    if (qid != QID_MAX) {//有建筑队列空闲
//        tmpVec.insert(tmpVec.begin(), talkTipsVec4.begin(), talkTipsVec4.end());
        for (int m=0; m<talkTipsVec4.size(); m++) {
            string tid = talkTipsVec4[m];
            string buildType = CCCommonUtils::getPropById(tid, "accept1");
            int num = getBuildNumByType(atoi(buildType.c_str()));
            if (num > 0) {
                tmpVec.push_back(tid);
            }
        }
    }
    
    qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, 1);
    if (qid != QID_MAX) {//有科技队列空闲
        tmpVec.insert(tmpVec.begin(), talkTipsVec1.begin(), talkTipsVec1.end());
    }
    
    for (int i=0; i<talkTipsVec3.size(); i++) {//造兵队列
        string tid = talkTipsVec3[i];
        string buildType = CCCommonUtils::getPropById(tid, "accept1");
        int qType = CCCommonUtils::getQueueTypeByBuildType(atoi(buildType.c_str()));
        if (qType != -1) {
            qid = QueueController::getInstance()->canMakeItemByType(qType, 1);
            if (qid != QID_MAX) {
                tmpVec.push_back(tid);
            }
        }
    }
    
    if ( WorldController::getInstance()->getCurrentMarchCount() >= WorldController::getInstance()->getMaxMarchCount() )
    {
        double armyPower = GlobalData::shared()->playerInfo.armyPower;
        for (int j=0; j<talkTipsVec2.size(); j++) {//出征队列
            string tid = talkTipsVec2[j];
            string sPower = CCCommonUtils::getPropById(tid, "accept1");
            string ePower = CCCommonUtils::getPropById(tid, "accept2");
            int startPower = atoi(sPower.c_str());
            int endPower = atoi(ePower.c_str());
            if (startPower<=0) {
                tmpVec.push_back(tid);
            }
            else if (startPower <= armyPower) {
                if (endPower>armyPower || endPower==0) {
                    tmpVec.push_back(tid);
                }
            }
        }
    }
    
    if(ArmyController::getInstance()->getTotalDead()>0) {//有伤兵
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_HOSPITAL, 1);
        if (qid != QID_MAX) {//有治疗队列空闲
            tmpVec.insert(tmpVec.begin(), talkTipsVec5.begin(), talkTipsVec5.end());
        }
    }
    
    if (tmpVec.size()>0) {
        int rIdx = CCMathUtils::getRandomInt(0, tmpVec.size()-1);
        string dialog = CCCommonUtils::getPropById(tmpVec[rIdx], "content");
//        string Ttype = CCCommonUtils::getPropById(tmpVec[rIdx], "type");
        ret = _lang(dialog);
    }
    else if (talkTipsVec0.size()>0) {
        int rIdx = CCMathUtils::getRandomInt(0, talkTipsVec0.size()-1);
        string dialog = CCCommonUtils::getPropById(talkTipsVec0[rIdx], "content");
        ret = _lang(dialog);
    }
    return ret;
}

bool FunBuildController::checkIsGuideBuild()
{
    if(GlobalData::shared()->playerInfo.level>1 || GlobalData::shared()->playerInfo.exp==0) {
        return false;
    }
    return true;
}

void FunBuildController::makeGuideBuildData(int itemId, int pos, int level)
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

void FunBuildController::updateHavestPush(int itemId)
{
    int maxTime = 0;
    int worldTime = GlobalData::shared()->getWorldTime();
    bool canPush = false;
    
    if (itemId == 0)
    {
        allFullTime = 0;
        map<int, FunBuildInfo>::iterator it;
        for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
        {
            auto& bInfo = it->second;
            if (bInfo.state == FUN_BUILD_NORMAL)
            {
                if (bInfo.type==FUN_BUILD_WOOD || bInfo.type==FUN_BUILD_FOOD || bInfo.type==FUN_BUILD_IRON || bInfo.type==FUN_BUILD_STONE) {
                    int costTime = getFullCostTime(bInfo.itemId);
                    if (costTime > maxTime) {
                        maxTime = costTime;
                    }
                }
            }
        }
        
        if (maxTime > 60) {
            canPush = true;
            allFullTime = worldTime+maxTime;
        }
    }
    else
    {
        int costTime = getFullCostTime(itemId);
        if ( (worldTime+costTime) > allFullTime) {
            allFullTime = (worldTime+costTime);
            canPush = true;
        }
    }
    
    
    if (canPush)
    {
        PushSettingInfo* pushInfo = NULL;
        for (int i=0; i<GlobalData::shared()->pushSettingVector.size(); i++) {
            pushInfo = GlobalData::shared()->pushSettingVector[i];
            if (pushInfo && pushInfo->type == 7) {
                break;
            }
        }
        string sound = "1";
        if (pushInfo) {
            sound = CC_ITOA(pushInfo->sound);
            if (pushInfo->status == 1) {
                cocos2d::extension::CCDevice::cancelNotice(99);
                cocos2d::extension::CCDevice::pushNotice(99, (allFullTime - worldTime),  _lang("105661"), sound, "14");
            }
        }
    }
}

void FunBuildController::OpenMoveBuild()
{
    OpenMoveBuildStat = true;
    //发出事件，开始置换，
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_BUILD, CCString::create("-1"));
}

void FunBuildController::SetMoveBuildPos(int pos)
{
    WillMovePos.push_back(pos);
    if (WillMovePos.size()==2) {
        //弹出确认交换
        PopupViewController::getInstance()->addPopupView(MoveBuildView::create());
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_BUILD, CCString::create(CC_ITOA(pos)));
}

void FunBuildController::CancelLastSetPos()
{
    if (WillMovePos.size() == 2) {
        int lastPos = WillMovePos[1];
        WillMovePos.pop_back();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CANCEL_LAST_MOVE_BUILD, CCString::create(CC_ITOA(lastPos)));
    }
}

void FunBuildController::CancelMoveBuild()
{
    OpenMoveBuildStat = false;
    WillMovePos.clear();
    //发出事件，取消置换
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CANCEL_BUILD);
}

void FunBuildController::StartMoveBuild(int gold)
{
    FunBuildMove * cmd=new FunBuildMove(WillMovePos[0], WillMovePos[1], gold);
    cmd->sendAndRelease();
}

void FunBuildController::EndMoveBuild(CCDictionary* dict)
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
            build1 = &(GlobalData::shared()->imperialInfo[oldBuild1]);
            build1->pos = WillMovePos[1];
            newBuild1 = getBuildKey(build1->type,build1->pos);
            build1->itemId = newBuild1;
        }
        
        if (oldBuild2 > 0) {
            build2 = &(GlobalData::shared()->imperialInfo[oldBuild2]);
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
                
                GlobalData::shared()->imperialInfo[oldBuild1] = info2;
                GlobalData::shared()->imperialInfo[oldBuild2] = info1;
            }
        }
        else
        {
            if (oldBuild1>0 && build1) {
                FunBuildInfo info = FunBuildInfo(build1);
                GlobalData::shared()->imperialInfo[newBuild1] = info;
                GlobalData::shared()->imperialInfo.erase(oldBuild1);
            }
            if (oldBuild2>0 && build2) {
                FunBuildInfo info = FunBuildInfo(build2);
                GlobalData::shared()->imperialInfo[newBuild2] = info;
                GlobalData::shared()->imperialInfo.erase(oldBuild2);
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

void FunBuildController::onShowMarkLineNotice(CCObject* obj)
{//联盟标记 通知
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint != 0) {
            int leaderPoint = GlobalData::shared()->playerInfo.allianceInfo.leaderPoint;
            auto endPos = WorldController::getPointByIndex(rallyPoint);
            auto startPos = WorldController::getPointByIndex(leaderPoint);
            
            string msg = _lang_2("115514", CC_ITOA(endPos.x), CC_ITOA(endPos.y));
            auto dict = CCDictionary::create();
            dict->setObject(CCString::create("3000002"), "senderUid");
            dict->setObject(CCString::create(msg.c_str()), "msg");
            dict->setObject(CCString::create("alliance_mark"), "tag");
            LuaController::getInstance()->addChatNotice(dict);
        }
    }
}