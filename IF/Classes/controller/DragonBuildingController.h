//
//  DragonBuildingController.h
//  IF
//
//  Created by 张军 on 15/6/5.
//
//

#ifndef __IF__DragonBuildingController__
#define __IF__DragonBuildingController__

#include "CommonInclude.h"
#include "DragonBuildingInfo.h"
#include "TileInfo.h"

#include "CommonInclude.h"
#include "FunBuildInfo.h"
#include "TileInfo.h"
#include "FunBuildController.h"

#define DBUILDING_HATCHER       450000 //龙巢（孵化池）
#define DBUILDING_CAVE          451000 //繁殖洞穴
#define DBUILDING_FARM          452000 //龙农场
#define DBUILDING_HABITAT_1     453000 //地系栖息地
#define DBUILDING_HABITAT_2     454000 //水系栖息地
#define DBUILDING_HABITAT_3     455000 //火系栖息地
#define DBUILDING_HABITAT_4     456000 //风系栖息地
#define DBUILDING_HABITAT_5     457000 //光系栖息地
#define DBUILDING_HABITAT_6     458000 //暗系栖息地
#define DBUILDING_HABITAT_7     459000 //特殊栖息地

#define DBUILDING_HATCHER_ItemId       450000000 //龙巢（孵化池）

#define DS_INIT_DIRECT_CHANGE "DS_INIT_DIRECT_CHANGE"
#define DS_FUN_BUILD_INIT_END "DS_FUN_BUILD_INIT_END"
#define DS_FUN_BUILD_CHANGE_END "DS_FUN_BUILD_CHANGE_END"
#define DS_FUN_BUILD_UPING_END_END "DS_FUN_BUILD_UPING_END_END"
#define DS_CREATE_BUILD_HAS_ERROR "DS_CREATE_BUILD_HAS_ERROR"
#define DS_FUN_BUILD_UPING_ERROR_END "DS_FUN_BUILD_UPING_ERROR_END"
#define DS_FUN_BUILD_NORMAL_ERROR_END "DS_FUN_BUILD_NORMAL_ERROR_END"
#define DS_FUN_BUILD_CANCEL_OTH_END "DS_FUN_BUILD_CANCEL_OTH_END"

#define DS_FUN_BUILD_CANSHOWSTATE "DS_FUN_BUILD_CANSHOWSTATE"
#define DS_BUILD_SETTIME "DS_BUILD_SETTIME"
#define DS_FUN_BUILD_NORMAL_ING "DS_FUN_BUILD_NORMAL_ING"


class DragonBuildingController : public CCObject{
public:
    static DragonBuildingController* getInstance();
    static void purgeData();
    DragonBuildingController();
    ~DragonBuildingController();
    
    void initOpenTile(string tiles);
    void getDataFromServer();
    void retSetData(CCDictionary* dict);
    void retDataFromServer(CCDictionary* dict);
    
    bool startOpenTile(int itemId, int gold=0);
    void endOpenTile(CCDictionary* dict);
    
    bool startOpenFunBuild(int itemId, int pos, int gold=0, bool isForce=false);
    void endOpenFunBuild(CCDictionary* dict, int type);
    
    bool startUpFunBuild(int itemId, int gold=0, bool isForce=false);
    void endUpFunBuild(CCDictionary* dict, int type);
    //收获龙资源 type=0表dragonFood，type=1表dragonGold
    bool startGetFunBuildProduct(int itemId,int resType=0);
    void endGetFunBuildProduct(CCDictionary* dict);
    //种植龙食物
    bool startPlantFood(int buildItemId,int foodItemId);
    void endPlantFood(CCDictionary* dict);
    
    bool startDestroyBuild(int itemId);
    void endDestroyBuild(CCDictionary* dict);
    
    bool costCD(int itemId, string toolUUID, int gold=0);
    void retCostCD(CCDictionary* dict);
    
    void cancelBuildState(int itemId);
    
    bool completeDestroy(int itemId);
    bool completeUpOrCreate(int itemId, bool isCreate=false);
    void updateDataAboutBuild(int itemId);
    
    void initAnimalData(CCDictionary* dict);
    void killAnimalByPos(int pos);
    void retKillAnimal(CCDictionary* dict, int pos);
    void postRefreashAnimal();
    void addAnimalData(CCDictionary* dict);
    
    bool startFinish(int itemId);
    
    int getUpRoCrtNum();
    bool canHavest(int itemId);
    
    bool canShowOutPut(int itemId);
    bool isFullOutPut(int itemId);
    int getFullCostTime(int itemId);
    
    void refreshResTime(CCDictionary* dict);
    
    int getOutPut(int itemId);
    float getOutPutPerSecond(int itemId);
    int getMaxOutPut(int itemId);
    int getStrory(int itemId);
    int getOutPutPreH(int itemId);
    string getBuildByPos(int pos);
    string orderBuild(string buildings);
    
    int getMaxLvBuildByType(int type);
    FunBuildInfo& getFunbuildById(int itemId);
    int getBuildKey(int type, int pos);
    
    void pushBuildQueue(CCDictionary* dict);
    void synTimeQueueToBuild(int itemId);
    int getBuildNumByType(int type);
    bool isExistBuildByTypeLv(int type, int lv);
    void updateAllOutPut();
//    void updateResPreSed(float t);
    void updatePreSed(float dt);
    void addOutPut(int sec);
    int getMainCityLv();
    
//    void testOpenTile(int itemId);
//    void initPosPath();
    
    void checkFreeTime(bool stat=false);
    int  getCanBoostBuild(int buildType);
    
    int findBdRoPos();
    int getBuildIdByPos(int pos);
//    string getTalkTips();
    
    bool checkIsGuideBuild();
    void makeGuideBuildData(int itemId, int pos, int level);
    
//    void updateHavestPush(int itemId=0);
    
    int curFunBuildId;
    map<int, FunBuildInfo> *curDragonBuildsInfo;
    map<int, FunBuildInfo> *curUpDragonBuildsInfo;
    map<int, TileInfo> m_tileMap;
    map<int, BigTileInfo> m_bigTileMap;
    void clearUpBuildingInfo(int itemid);
    void initSetUpBuildingInfo(CCDictionary* dict);
    void onCreateOrUpError();
    void updateGeneralData(int itemId);
    bool isGetAllFunBuilds;
    
    void OpenMoveBuild();
    void SetMoveBuildPos(int pos);
    void CancelLastSetPos();
    void CancelMoveBuild();
    void StartMoveBuild(int gold);
    void EndMoveBuild(CCDictionary* dict);
    
    float oldScale;
    int oldx;
    int oldy;
    int willOpenOrUpId;
    bool isFirst;
    bool isError;
    int addTime;
    
//    double WoodOutPreSed;
    double WoodOdd;
//    double FoodOutPreSed;
//    double curFoodOutPreSed;
    double FoodOdd;
//    double StoneOutPreSed;
    double StoneOdd;
//    double IronOutPreSed;
    double IronOdd;
//    double SliverOutPreSed;
    double SliverOdd;
    
    double oriWoodPreH;
    double oriStonePreH;
    double oriIronPreH;
    double oriFoodPreH;
    double oriSliverPreH;
    
    int building_base_k1;
    int building_base_k2;
    int building_base_k3;
    int building_base_k4;
    int building_base_k5;
    int building_base_k6;//资源建筑加速持续时间，dataconfig中building_base{k6}秒
    int building_base_k7;//2、资源建筑加速效果公式，产量=int(原产量*（1+building_base{k7}/100))
    int building_base_k8;//队列租用的最短时间
    int building_base_k9;//队列租用最短时间所消耗的金币
    
//    map<int, vector<cocos2d::CCPoint> > posToPath;
//    map<int, vector<int> > posToZorders;
//    vector<cocos2d::CCPoint> sqerPoints;
//    map<int, int> lastSqerPos;
//    map<int, int> colTroopsNum;
    
    map<string, FunBuildInfo> m_guideBuildMap;
    map<int, int> m_finishGuideBuildMap;
    
    int lastAddWood;
    int lastAddStone;
    int lastAddIron;
    int lastAddFood;
    
    int dataConfig_freeSpdT;
    
    int m_refreashAnimalTime;
    int m_bigAnimalNum;
    int m_smallAnimalNum;
    vector<int> bigAnimalPos;
    vector<int> smallAnimalPos;
    vector<int> idleAnimalPos;
    
//    vector<string> talkTipsVec0;
//    vector<string> talkTipsVec1;
//    vector<string> talkTipsVec2;
//    vector<string> talkTipsVec3;
//    vector<string> talkTipsVec4;
//    vector<string> talkTipsVec5;
    
    int allFullTime;
    bool OpenMoveBuildStat;
    vector<int> WillMovePos;
    int willMoveToBuildType;
    
    void updateLimitNum();//更新农场，栖息地建筑上限
    int getLimitNum(int type);//得到农场，栖息地建筑上限
    
    bool bReachLimit(int type);//农场、栖息地数量达到上限
    int getCurTotalNum(int type);//得到农场、栖息地的当前数量
    
    vector<int> dragonFoodIds;//可种植的龙食物id
private:
    map<int, string> posToTypeMap;
};
#endif /* defined(__IF__DragonBuildingController__) */
