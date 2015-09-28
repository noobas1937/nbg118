//
//  FunBuildController.h
//  IF
//
//  Created by fubin on 13-11-1.
//
//

#ifndef IF_FunBuildController_h
#define IF_FunBuildController_h

#include "CommonInclude.h"
#include "FunBuildInfo.h"
#include "TileInfo.h"

#define FUN_BUILD_NORMAL 0
#define FUN_BUILD_LOCK 1
#define FUN_BUILD_ULOCK 2
#define FUN_BUILD_CREATE 3
#define FUN_BUILD_UPING 4
#define FUN_BUILD_CHANGE 5
#define FUN_BUILD_UPING_END 6
#define FUN_BUILD_NOT_OPEN 7
#define FUN_BUILD_INIT 8
#define FUN_BUILD_CREATE_ERROR 9
#define FUN_BUILD_UPING_ERROR 10
#define FUN_BUILD_NORMAL_ERROR 11
#define FUN_BUILD_DESTROY 12
#define FUN_BUILD_DESTROY_END 13
#define FUN_BUILD_CANCEL_CREATE 14
#define FUN_BUILD_CANCEL_OTH 15
#define FUN_BUILD_CANOPEN 16

#define FUN_BUILD_MAIN 400000
#define FUN_BUILD_TAVERN 401000
#define FUN_BUILD_STABLE 402000
#define FUN_BUILD_SCIENE 403000
#define FUN_BUILD_CELLAR 404000
#define FUN_BUILD_WAR 405000
#define FUN_BUILD_GOLD 406000
#define FUN_BUILD_SMITHY 407000
#define FUN_BUILD_PRISON 408000
#define FUN_BUILD_SILVER 409000
#define FUN_BUILD_BARRACK 410000
#define FUN_BUILD_HOSPITAL 411000
#define FUN_BUILD_STONE 412000
#define FUN_BUILD_WOOD 413000
#define FUN_BUILD_IRON 414000
#define FUN_BUILD_FOOD 415000
#define FUN_BUILD_FORT 416000
#define FUN_BUILD_WATCH_TOWER 417000
#define FUN_BUILD_ARROW_TOWER 418000
#define FUN_BUILD_WALL 419000
#define FUN_BUILD_AFFAIR_CENTER 420000
#define FUN_BUILD_TEMPLE 421000
#define FUN_BUILD_WONDER 422000
#define FUN_BUILD_MONASTERY 424000
#define FUN_BUILD_CITY_OFFICE 425000 //不用了
#define FUN_BUILD_BARRACK1 423000
#define FUN_BUILD_BARRACK2 424000
#define FUN_BUILD_BARRACK3 425000
#define FUN_BUILD_BARRACK4 426000
#define FUN_BUILD_TRAINFIELD 427000
#define FUN_BUILD_SACRIFICE 428000
#define FUN_BUILD_FORGE 429000
#define FUN_BUILD_WORKSHOP 430000

#define FUN_BUILD_WOOD_STORGE 0
#define FUN_BUILD_IRON_STORGE 0

#define FUN_BUILD_MAIN_CITY_ID 400000000
#define FUN_BUILD_WALL_ID 419000001

#define BIG_TILE_1 9992
#define BIG_TILE_2 9993
#define BIG_TILE_3 9994
#define BIG_TILE_4 9995
#define BIG_TILE_5 9996
#define SPE_BUILD_FLAG 9990001
#define SPE_BUILD_HD 9990002
#define SPE_BUILD_GOLD 9990003
#define SPE_BUILD_MAIL 9990004
#define SPE_BUILD_GOLD_BOX 9990005
#define SPE_BUILD_CHR_TREE 9990006
#define SPE_BUILD_WILL_OPEN 9990007
#define SPE_BUILD_MERCHANTE 9990008
#define SPE_BUILD_SHIP 9990009
#define SPE_BUILD_LOTTERY 9990010
#define SPE_BUILD_CARGO 9990011
#define SPE_BUILD_XIONGDI 9990012
#define SPE_BUILD_NEWED 9990013
#define SPE_BUILD_DRAGON 9990014

#define MSG_OPEN_TILE "msg_open_tile"
#define MSG_UNLOCK_TILE "msg_unlock_tile"
#define MSG_CHECK_TIME "msg_check_time"
#define MSG_RES_GET "msg_res_get_action"
#define MSG_SHOW_TIME_BAR "msg_show_time_bar"

#define MSG_PERSON_TALK "msg_person_talk"
#define MSG_PERSON_STOP_TALK "msg_person_stop_talk"
#define MSG_FINISH_MOVE_BUILD "msg_finish_move_build"
#define FUN_BUILD_MAIN_LEVEL "fun_build_main_level"

#define GUIDE_MAINCITY_LEVEL_UP "guide_maincity_lv_up"

#define MSG_STOP_CITY_MOVE "msg_stop_city_move"

#define MSG_MARK_LINE_AL "msg_mark_line_al"

const char * const MSG_TITAN_UPGRADE_COMPLETE = "MSG_TITAN_UPGRADE_COMPLETE";//泰坦状态改变


class FunBuildController : public CCObject{
public:
    static FunBuildController* getInstance();
    static void purgeData();
    FunBuildController();
    ~FunBuildController();
    
    void initOpenTile(string tiles);
    void getDataFromServer();
    void retSetData(CCDictionary* dict);
    void retDataFromServer(CCDictionary* dict);
    
    bool startOpenTile(int itemId, int gold=0);
    void endOpenTile(CCDictionary* dict);
    
    bool startOpenFunBuild(int itemId, int pos, int gold=0, bool isForce=false);
    void endOpenFunBuild(CCDictionary* dict, int type);
    
    bool startUpFunBuild(int itemId, int gold=0, bool isForce=false, int isUpStar=0);
    void endUpFunBuild(CCDictionary* dict, int type);
    
    bool startGetFunBuildProduct(int itemId);
    void endGetFunBuildProduct(CCDictionary* dict);
    
    bool startDestroyBuild(int itemId);
    void endDestroyBuild(CCDictionary* dict);
    
    bool costCD(int itemId, string toolUUID, int gold=0,int toolCnt = 0);
    void retCostCD(CCDictionary* dict);
    
    void cancelBuildState(int itemId);
    
    bool completeDestroy(int itemId);
    bool completeUpOrCreate(int itemId, bool isCreate=false);
    void updateDataAboutBuild(int itemId);
    
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
    void updateResPreSed(float t);
    void addOutPut(int sec);
    int getMainCityLv();
    
    void testOpenTile(int itemId);
    void initPosPath();
    
    void checkFreeTime(bool stat=false);
    int  getCanBoostBuild(int buildType);
    
    int findBdRoPos();
    int getBuildIdByPos(int pos);
    string getTalkTips();
    
    bool checkIsGuideBuild();
    void makeGuideBuildData(int itemId, int pos, int level);
    
    void updateHavestPush(int itemId=0);
    
    int curFunBuildId;
    FunBuildInfo curHarvestBuildsInfo;
    map<int, FunBuildInfo> *curBuildsInfo;
    map<int, FunBuildInfo> *curUpBuildsInfo;
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
    
    double WoodOutPreSed;
    double WoodOdd;
    double FoodOutPreSed;
    double curFoodOutPreSed;
    double FoodOdd;
    double StoneOutPreSed;
    double StoneOdd;
    double IronOutPreSed;
    double IronOdd;
    double SliverOutPreSed;
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

    map<int, vector<cocos2d::CCPoint> > posToPath;
    map<int, vector<int> > posToZorders;
    vector<cocos2d::CCPoint> sqerPoints;
    map<int, int> lastSqerPos;
    map<int, int> colTroopsNum;
    
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
    
    vector<string> talkTipsVec0;
    vector<string> talkTipsVec1;
    vector<string> talkTipsVec2;
    vector<string> talkTipsVec3;
    vector<string> talkTipsVec4;
    vector<string> talkTipsVec5;
    
    int allFullTime;
    bool OpenMoveBuildStat;
    vector<int> WillMovePos;
    int willMoveToBuildType;
    int willMoveToBuildItemID;//fusheng 直接跳转的ItemID
private:
    map<int, string> posToTypeMap;
    
    void onShowMarkLineNotice(CCObject* obj);
};
#endif /* defined(__IF__UpBuildingController__) */
