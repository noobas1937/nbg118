//
//  WorldMapView.h
//  IF
//
//  Created by 邹 程 on 13-11-19.
//
//

#ifndef __IF__WorldMapView__
#define __IF__WorldMapView__

#include "CommonInclude.h"
#include "WorldController.h"
#include "BaseTileInfo.h"
#include "HFViewport.h"
#include "WorldCommand.h"
#include "CCLabelBatch.h"
#include <deque>
#include "CCLineBatchedSprite.h"
#include "NewWorldMarchArmy.h"
#include "NewBaseTileInfo.h"
#include "IFSkeletonBatchLayer.h"
#include "IFHeiqishiNode.h"
#define  MSG_SHAKETIME              "shakeTime"
#define  MSG_SHAKECMDTIME              "shakeCmdTime"
#define  MSG_SHAKEGUIDETIME              "shakeGuideTime"


#define WM_BG_TAG 10010
#define WM_MAP_TAG (WM_BG_TAG + 1)
#define WM_BETWEEN_SERVER_MAP_TAG 100000
#define WM_MASK_TAG 200000
#define WM_COVER_TAG 10000

enum
{
WM_BG                 = 0,
WM_BETWEEN_SERVER_MAP,
WM_CITY              ,
WM_ROAD              ,
WM_SKY               ,
WM_TILE              ,
WM_GUI               ,
WM_POPUP             ,
WM_COVER             ,
    LAYER_COUNT
};

#define WM_GUI_TAG 453567
#define WM_POPUP_TAG 453568

#define TILE_LAST_TIME 3000.0   // millisecond
#define TILE_FADE_TIME 1000.0   // millisecond
#define miniMapTag 99999999
#define KING_TIME_TAG 88888888
#define MinimapLoadingTag 66666666
#define DEFAULT_THRONE_INDEX 302

#define WORLD_HD_MAX 1.8
#define WORLD_HD_MIN 1.2
#define WORLD_DEFAULT_HD_SCALE 1.4


enum BatchTagType {
    defaultTag
    ,LevelTag
    ,FireTag
    ,ProtectTag
    ,ResourceProtectTag
    ,SmokeTag
    ,CityMoveInTag
    ,CityMoveOutTag
    ,NameTag
    ,NameTag1
    ,NameTag2
    ,EdgyTag
    ,EdgyTag1
    ,WorldBorder
    ,WorldBorder1
    ,CityTag
    ,CityMoveInViewTag
    ,MonsterAttack
    ,MonsterAttack1
    ,MonsterDead
    ,MonsterDead1
    ,MonsterBreath
    ,Rock
    ,Rock1
    ,Rock2
    ,Rock3
    ,WarFire
    ,MonsterBreath1
    ,MonsterProBG1
    ,AllianceFlag
    ,Shadow
    ,MonsterTalkText
    ,MonsterTalkLine
    ,AllianceParticle
    ,MonsterAttackParticle
    ,MonsterParticle
    ,TrebuchetWait
    ,TrebuchetAttack
    ,OfficerTagBG
    ,OfficerTag
    ,OfficerTagParticle
    ,WarBuildTextTag1
    ,WarBuildTextTag2
    ,WarBuildTextTag3
    ,WarBuildStateBG
    ,ThroneProtectParticle
    ,Partical_fieldMonster
    ,Partical_AAreaBuild
    ,Partical_mapMask
    ,AllianceTerritoryParticle
    ,CSProtectTag
//    ,TreasureMapCDIcon
    ,TreasureMapCDBarBG
//    ,TreasureMapCDBar
};

enum MarchAniType {
    // single frame
    AniCityProtect
    ,AniCityResourceProtect
    // double frame
    ,AniCollect
    ,AniCollectCloth
    ,AniCollectTitan
    ,AniScout
    ,AniScoutCloth
    ,AniDeal
    ,AniDealCloth
    ,AniEdgy
    ,AniMonsterAttack
    ,AniMonsterDead
    ,AniMonsterBreath
    ,AniTrebuchetWait
    ,AniTrebuchetAttack
    ,AniAllianceArea
    
};

struct CellMap{
    int level;
    std::string leagueUid;
    bool isCity;
    double locateTime;
    int cityIndex;
};

class DynamicTiledMap;

class WorldMapView : public CCLayer,public ITouchDelegate
{
public:
    static WorldMapView* instance();
    static void setInstance(WorldMapView* view);
    static void unsetInstance();
    
    static WorldMapView* create(CCPoint& tilePoint, MapType mapType,bool isInGuide = false);
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    
    CCNode *getNodeByType(std::string typeStr);
    void leaveWorld();
    void clearPopupView();
    void addPopupView(BaseTileInfo* view);
    void addPopupView2(CCObject* view);
    void addPopupView1(CCObject* view);
    void addPopupView3(NewBaseTileInfo* view);
    void addPopupView4(NewBaseTileInfo* view);
    //    void autoAddResourceView(MarchInfo &info);
    void resetClickState(CCObject* obj);
    void updateSelfName();
    bool isMiniMapModel();
    void showTroopInfo(const string& marchId);
    void addCover();
    void removeCover();
    void addCityMoveParticle(CCObject* obj);
    void onCloseTilePopup();
    WorldMapView():
    m_cityInfo(WorldController::getInstance()->getCityInfos())
    ,m_updateTimeStamp(0)
    ,m_isIdle(true)
    {
        WorldMapView::setInstance(this);
        pthread_mutex_init(&m_addCityLock, NULL);
    }
    void onShowShakeGuide(float time);
    virtual bool init(CCPoint& tilePoint, MapType mapType , bool isInGuide);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    
    void hideGUI(CCObject* obj);
    void showGUI(CCObject* obj);
    void showFirework(cocos2d::CCObject *obj);
    void startShowFirework(int firePosistion,int useItemId);
//    int firePosistion;
//    int m_useItemId;
    void updateGUI(bool setVisible);
    void openTilePanel(unsigned int index);
    void changeServer(int serverId);
    void afterMarchDeploy(CCDictionary* dict,int bType=-1,std::string other="", int targetType=-1);
    void afterMarchSpeedUp(CCDictionary* dict);
    void afterMarchCancel(CCDictionary* dict);
    void afterCityMove(CCDictionary* dict);
    void afterCrossCityMove(CCDictionary* dict);
    void refreshMonster(CCObject *obj);
    void addAttackParticle(CCObject *dict);
    void onMarchCallback(CCObject* obj);
    
    bool updateMarchTarget(MarchInfo &info,double now,float delta);
    
    void gotoTilePoint(const CCPoint &point,bool isForceUpdate = false,int forceServerId = -1);
    void gotoTilePoint(const CCPoint &point, int serverId, bool isForceUpdate = false);
    
    void addFavorite(const std::string &name , unsigned int index, unsigned int type);
    void onAddFavoriteCallback(CCDictionary *info);
    
    void onCityMoveCallback(CCObject* obj);
    void doTeleport(unsigned int type,unsigned int index,const string& name);
    void pushAddTower(unsigned int type, unsigned int index ,int dialog,string aaid,bool isBanner = false,std::string banner = "");
    void finishTeleport(CCObject* obj);
    void onFinishAddTower(CCObject* obj);
    
    void doAllianceArea(unsigned int type ,unsigned int index,bool isSuperMine = false,bool isWarehouse = false,WorldResourceType resType = Wood,string aaid = "");
    void finishAllianceArea(CCObject* pObj);
    
    void finishBattleAni(CCObject* obj);
    void onFinishSpinBattle(CCObject* obj);
    
    unsigned int getBatchTag(BatchTagType type,unsigned int index);
    void addBatchItem(BatchTagType type,unsigned int index,CCObject* obj = NULL);
    void delBatchItem(BatchTagType type,unsigned int index);
    void removeWalkParticle(int tag);
    CCAnimate* createMarchAni(MarchAniType type,int direction = 0, float deltaTime = 1.0, std::string para = "");
    CCSprite* createMarchSprite(MarchInfo& info);
    IFHeiqishiNode* createHeiqishiSoldier(MarchInfo& info);
    void trebuchetAttack(CCDictionary *params);
    void resetTrebuchet(CCObject *obj);
    CCPoint getViewCenterTilePoint();
    // ITouchDelegate
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta);
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocation);
    virtual void onBeginZoom(const CCPoint& point1,const CCPoint& point2);
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    virtual void onEndZoom(float targetZoom);
    virtual void autoMoveEnd();
    void playFreshResAni(int index);
    int getBestLocIndex(int index);
    bool isFourTileCanRelocate(int index);
    bool isCanRelocationTile(int index);
    void updateWinNum(int index);
    void createWinNum(int index);
    //放置资源点
    void gotoAddWorldItems(int itemid);
    void afterAddWorldItems(cocos2d::CCDictionary *dict);
    
    HFViewport* m_touchDelegateView;
    
    vector<unsigned int> m_cacheToAddCity;
    bool m_inCityUpdate;
    
    vector<unsigned int> m_untouchableTiles;
    
    CCPoint m_directionLBPoint;
    CCPoint m_directionRTPoint;
    CCSprite* m_directionIcon;
    CCLabelIF* m_directionLabel;
    void updateDirection();
    CCSprite* m_loadingIcon;
    
    CCPoint tmpTouchPoint;
    CCPoint selfViewPoint;
    CCSize  halfWinSize;
    
    double m_lastScrollTime;
    
    bool inFollowView;
    string followMarchId;
    
    CCLineBatchedLayer* m_drawRoadNode;
    CCNode* m_touchTroopNode;
    CCNode* m_simNameUnbatchNode;
    CCNode* m_unBatchLabelNode;//非batch的文本，主要用于名字的显示
    CCNode* m_aAreaUnbachLbNode;
    CCSpriteBatchNode* m_towerRangeMaskNode;
    CCNode* m_mapMaskNode;
    CCNode* m_mapMaskCloudNode;
    
    
    CCNode* m_flagBatch;//旗帜
    CCSpriteBatchNode* m_mapMarchNode;//侦查，交易
    IFSkeletonBatchLayer* m_mapMonsterNode;
    Node*                 m_mapMonstersNode; // guo.jiang
    //    IFSkeletonBatchLayer* m_mapMonsterBossNode;//世界boss
    IFSkeletonBatchLayer* m_mapHeiqishiMarchNode;//黑骑士
    CCNode* m_spinLabelNode;//黑骑士 lb
    CCSpriteBatchNode* m_heiqishiLvBgBachNode;//黑骑士lvBg
    CCSpriteBatchNode* m_heiqishiShadowBachNode;
    CCLabelBatchNode* m_heiqishiLvLbNode;
    CCSpriteBatchNode* m_csNode;
    CCNode* m_lianNode;//连胜名字的显示
    
    CCSpriteBatchNode* m_mapArrowSpNode;
    CCSpriteBatchNode* m_mapMonsterShadowNode;
    CCSpriteBatchNode* m_mapMarchNode1;// 部队行军
    
    CCSpriteBatchNode* m_mapTowerBatchNode;
    
    //    CCSpriteBatchNode* m_mapMarchNameBG;// 部队名字底板
    //    CCNode *m_armyNameNode;
    CCSpriteBatchNode* m_occupyPointerNode;
    CCSpriteBatchNode* m_line;
    CCSpriteBatchNode* m_conflictBatchNode;
    CCNode* m_cityBatchNode;
    CCNode* m_aAreaAniNode;
    CCNode* m_bannerNode;
    CCSpriteBatchNode* m_cityAttackNode;
    CCNode* m_throneNode;
    CCNode* m_towerNode;
    CCSpriteBatchNode* m_throneTowerNode;
    CCNode* m_batchNode;
    
    CCNode *m_armyDustParticleNode;
    CCNode *m_flagParticleNode;
    
    CCLabelBatchNode* m_labelNode;
    CCLabelBatchNode* m_labelNode1;
    CCNode *tilePopNode;
    vector<CCParticleBatchNode*> m_parBatchVec;
    vector<CCParticleBatchNode*> m_parWalkBatchVec;
    vector<CCNode*> m_parFlagBatchVec;
    
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_particleVec;
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_walkparticleVec;
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_flagParticleVec;
    
    map<unsigned int , vector<CCSafeObject<CCNode> > > m_cityItem;
    std::list<unsigned int> m_displayedCity;
    map<unsigned int, MarchArmy*> m_marchArmy;
    DynamicTiledMap* m_map;
    CCTMXLayer* m_tileLayer;
    
    CCControlButton* m_btnGoto;
    
    // for city info update
    int m_updateTimeStamp;
    void callBackMapInfo(CCDictionary *param);
    
    void asyncCityInfoParse(CCObject* obj);
    void addCityInMainThread(CCObject* obj);
    void asyncReleaseInMainThread(CCObject* obj);
    void clearAllMarch();
    ~WorldMapView() {
        pthread_mutex_destroy(&m_addCityLock);
        WorldMapView::unsetInstance();
    }
    
    void createCity(WorldCityInfo& info);
    void releaseCity(WorldCityInfo& info);
    
    void addParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    void addWalkParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    void addFlagParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    
    std::string getSpriteName(WorldCityInfo &info);
    void removeTouchEffect();
    void showAndHideUnBatchLabel();
    void showAndHideFieldMonster();
    void onShowAndHideSpinLb();
    void onShowAndHideCloud();
    std::map<int, unsigned int> getCurrentTile(CCPoint &pt, int rowNum, int colNum, int tileX);
    CCPoint getMarchPoint(MarchInfo &info);
    bool needClearAll;
    
    // pos : 根据坐标和城市开始索引计算岛的外观，不用存储数据到服务器
    static CCArray *getCityPicArr(int addIndex, int level, bool isKing ,int nSpecialId = -1, const Vec2& pos = Vec2::ZERO);
    // pos : 根据坐标和城市开始索引计算岛的外观，不用存储数据到服务器
    static CCArray *getCityPicArr(WorldCityInfo &info, int level,int nSpecialId = -1, const Vec2& pos = Vec2::ZERO);
    
    bool isCrossServer;
    void addAllianceArea(int cityIndex,bool isMyAlliance);
    bool isDownloadingMinimap;
    //
    void monsterDeathCB(CCObject* obj);
    void resetAAreaInfo(int tileIndex,int range);
    void initAAreaBlankInfo(int tileIndex,int range,bool _isMyalliance);
    void onRefreshAAreaBuilding(int tileIndex, AAreaState state,bool isSuperMine = false ,bool isTower = false,bool isWarehouse = false,bool isBanner = false);
    void initTowerRange(int tileIndex,bool isInRange);
    CC_SYNTHESIZE(bool, m_needOpenInfoView, NeedOpenInfoView);
    int getPicIdByCityInfo(WorldCityInfo &info);
    string getResNameByType(WorldResourceType type);
    bool isInWinsize(CCPoint viewPoint,int modelNum = 0);
    void onShowTowerRange(bool visable);
    void onSendHelpCmd();
    void onShake(CCObject* pObj);
    void testCastle(int level);
private:
    void setPraticesEffect( bool isEnalbe );
    void addUnderNode(unsigned int index);
    void delUnderNode(unsigned int index);
    void addFlagPar(std::string type, const CCPoint &pt, int index);
    int getThroneIndex(int picId);
    void loadThroneResource(int index);
    MapType m_mapType;
    
    map<unsigned int, WorldCityInfo>& m_cityInfo;
    map<unsigned int, bool> mp;
    
    pthread_mutex_t m_addCityLock;
    
    CCNode* m_layers[LAYER_COUNT];
    CCArray *tilePops;
    int roadIndex;
    WorldActivityState m_currentState;
    int currentThroneIndex;
    CCRect m_viewPort;
    CCSize m_winSize;
    CCRect m_monsterRect;
    void onPurgeSpinNode(CCObject* obj);
    void onShowAAreaLb(bool visiable);
    void onSAHaAreaParcital(CCObject* pObj);
    void hideSameBoard(int cityIndex);
    void onTowerAttack(CCNode * marchNode ,MarchInfo& info);
    void onTowerAttackAnimation(int startCityIndex,CCPoint endViewPos,int marchTag);
    void addTowerRange(int tileIndex);
    void onShowMarkLine(CCObject* obj);
    bool canShowShakeGuide();
    bool m_isIdle;
    
    struct FlagParInfo {
        std::string type;
        CCPoint pt;
        int index;
    };
    
    bool m_isInGuide;//fusheng 是否是新手引导中
    
    vector<FlagParInfo> m_flagParDatas;
    
    void update_water_shader(const Vec2& position);
    Texture2D* m_water_wave2;
    Texture2D* m_water_wave1;
    
    friend class DynamicTiledMap;
    friend class NBWorldMonster;
};

#endif /* defined(__IF__WorldMapView__) */
