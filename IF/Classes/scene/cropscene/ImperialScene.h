//
//  ImperialScene.h
//  IF
//
//  Created by fubin on 13-10-31.
//
//

#ifndef IF_ImperialScene_h
#define IF_ImperialScene_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "FunBuild.h"
#include "HFViewport.h"
#include "CCLabelBatch.h"
#include "ArcPopupBaseView.h"
#include "SpeBuild.h"
#include "FunBuildBtnsView.h"
#include "SpeBuildBtnsView.h"
#include "WallBuild.h"
#include "FlyCell.h"
#include "Animal.h"
#include "TalkNoticeCell.h"
#include "IFSkeletonBatchLayer.h"
#include "CCAniNode.h"

// tao.yu
#include "Titan.h"

#define BINDING_TIME "bindingTime"
#define ANNOUCE_TIME "Time"
#define FIRST_OPEN_AFTER_UPDATE "firstOpenAfterUpdate1"
#define MSG_TALK_OPEN "msg.talk.open"
// tao.yu buildCount从59修改为54
#define BUILD_COUNT 54
#define MSG_SHOW_EXP_PARTICLE "msg.show.exp.particle"
#define MSG_SHOW_POWER_PARTICLE "msg.show.power.particle"
#define ACTIVITY_BOTTLE_TAG 1000

#define JUST3D_NODE_TAG 89757

//begin a by ljf
#define IMPERIAL_SCENE_TOUCH_LAYER_TAG 89217
//end a by ljf

class ImperialScene:public CCLayer,public ITouchDelegate,public CCBMemberVariableAssigner
{
public:
    ImperialScene():lastTouchBuildId(-1),curTouchBuildId(-1),m_count(0),m_singleTouchState(false),m_mainPatPlayTime(0),m_waterType(0)
    ,m_curBuildId(0),m_curBuildPosx(0),m_curBuildPosy(0),m_tmpMoveX(0),m_tmpMoveY(0),m_canClick(true),m_buildingInitState(false)
    ,m_oldScale(1.0),m_oldPosX(0),m_oldPosY(0),m_isOnlyPower(0),m_isSave(false),m_tmpBuildPos(0),m_removeSpeBId(0),m_curGuideEnd(false),m_isLogin(false),m_beginTouchType(0),m_forceMove(false),m_exit(false),m_isDay(false),m_isRain(false),m_sysTime(0),m_talkTime(0), m_lotteryBuild(NULL),mActionManager(NULL),m_nightLights(NULL),m_rescustombatchNode(NULL),m_Titan(NULL){};
    
    virtual ~ImperialScene(){};
    CREATE_FUNC(ImperialScene);
    bool init();
    CC_SYNTHESIZE(CCLayer*, buildingLayer, BuildingLayer);
    CC_SYNTHESIZE(CCLayer*, fixLayer, FixLayer);
    CC_SYNTHESIZE(CCSpriteBatchNode*, mbatchNode, BatchNode);
    
    virtual void onEnter();
    virtual void onExit();
    
    // tao.yu titan
    void onCreateTitan();
    // tao.yu vikings
    void onCreateVikingsShip();
    //begin a by ljf
    void onVikingsShipMove(NBSprite3D * pSprite3d);
    void onVikingsShipIdle(NBSprite3D * pSprite3d);
    bool onVikingsShipTouched(CCTouch* pTouch);
    //end a by ljf
    
    void onUpdateInfo();
    void onCreateBuild(int itemId);
    void onOpenBuild(int itemId);
    void onResetBuild(int itemId);
    void onEnterFrame(float dt);
    void onShowFeedbackAnim(float dt);
    void onShowFeedbackAgainAnim(float dt);
    void onShowFeedbackAgainLaterAnim(float dt);
    void onShowHTPAnim(float dt);
    void onShowHTPAgainAnim(float dt);
    void onShowHTPAgainLaterAnim(float dt);
    void onOpenBuildEnd(int itemId);
    
    void update(float dt); // tao.yu 每帧调用
    
    void onMoveToBuild(int itemId, bool st=false);
    void onMoveToBuildAndPlay(int itemId, bool st=false);
    void onMoveToSpeBuildAndPlay(int itemId, bool st=false);
    void onMoveToBuildAndOpen(int itemId, int type=TYPE_POS_DEF, float dt=0.25, bool bound=false);
    void onMoveToPos(float x, float y, int type=TYPE_POS_DEF, float dt=0.25, float scale=1.3, bool bound=false);
    void onMoveToPosDir(float sx, float sy, float ex, float ey, float dt=0.25, bool bound=false);
    CCPoint onGetSceenPt(float x, float y);
    float onGetTouchLayerScale();
    void onSaveCurPos(CCObject* obj);
    void setPxPy(float x,float y);
    void onGoBackOldPos(CCObject* obj);
    void onFlyOutPut(int itemId, int output, int forceResType=-1);
    void onFlyCargoOutPut(CCObject* obj);
    
    virtual void onTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    void stopShadow(int buildId, CCTouch *pTouch, CCEvent *pEvent);
    
    void showTmpBuild(int pos, int itemId);
    void hideTmpBuild(int pos);
    
    virtual void onSingleTouchBegin(CCTouch* pTouch);
    virtual void onSingleTouchEnd(CCTouch* pTouch);
    
    CCNode* getBuildById(int itemId);
    CCNode* getBuildNameById(int itemId, string _key);
    void addToMovingLayer(CCNode* node);
    float getTouchLayerScale();
    void onRemoveSpeBuild();
    void setUnMoveScence(bool st);
    void showBuildBtns(int itemId);
    void clickWorkshop(int itemId);//点材料工坊
    void onResetLastBuildId();
    
    void onShowBtnsView(int x, int y, int buildId);
    void onShowSpeBtnsView(int x, int y, int buildId);
    void onFlyTroopParticle(int x, int y, int onlyPower=0);
    void onFlyTroopEnd(CCObject *obj);
    void onFlyTroopEndToUI(CCObject *obj);
    void onPlayMoveTroops(int buildId);
    
    int findCanBuildTile(int pos);
    void onPlayBattle();
    void flyBall(int buildId, int type, bool toScene);
    
    void makeCanPlayArrowPoint();//计算哪些建筑播放 手指点击
    void reSetTalkTime();
    void hideFlyArrow(float _time);
    
    void updateShipState(string state);
    CCBAnimationManager* getAnimationManager();
    void setAnimationManager(CCBAnimationManager *pAnimationManager);
    CCSpriteBatchNode* createCustomBatchLayer(string image);
    void removeCustomBatchLayer();
private:
    int lastTouchBuildId;
    int curTouchBuildId;
    void onResourcesUpdate(cocos2d::CCObject* params);
    void onBattleUpdate(CCObject* params);
    void unLockTile(CCObject* params);
    void refreashTile();
    void initBgTree();
    
    void onOpenNewBuild(CCObject* obj);
    void onUpgradeNewBuild(int buildId);
    void flyBallEnd(CCNode* pSender);
    void makeFlyMc(int type, CCNode* flNode);
    void flyOutPut(int buildId, CCNode* flNode, int idx);
    void flyOutPutEnd(CCNode* pSender);
    void showAttention();
    void showAttention1();
    
    void initMc2();
    void initBigTile();
    void startGuide(float _time);
    
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    bool isGeneralOnFormationBuilding(int buildingId);
    void canSingleTouch(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void addCloudParticleToBatch(cocos2d::CCParticleSystemQuad *particle, bool isTorch = false);
    void makeWater(int type);
    
    void onRemoveTmpBuild(CCObject* obj);
    void onEnableMoveScence(float _time);
    void trackDBM();
    void trackTencent();
    std::string UrlEncode(const std::string& str);
    bool simple_xor(const std::string& info,const std::string& key,unsigned char* result);
    void onTrackResponse(CCHttpClient* client, CCHttpResponse* response);

    
    void buildingCallBack(CCObject* params);
    void wallCallBack(CCObject* params);
    void showHarvestEffect(CCObject* obj);
    void scheduleHarvestEffect(float _time);
    void guideEnd(CCObject* obj);
    void retGuideEnd(float _time);
    void clearGuideState(float _time);
    void playFlyTroopParticle(float _time);
    void playPowerAni(float _time);
    void downloadXML(float _time);
    
    void onFinishMoveBuild(CCObject* obj);
    
    void onPowerADD(CCObject* obj);
    
    void onStopAllActions(CCObject* params);
    
    map<int, FunBuild*> m_buildItems;
    map<int, SpeBuild*> m_bigTiles;
    map<int, CCNode*> m_bigTileNodes;
    void moveMapToPosition(CCObject* obj);
    
    void refreshSoldiers(CCObject* obj);
    void addSoldierToMap(int stype, int num, int ptx, int pty);
    
    void playFountain();
    void removeFountain(CCObject *obj);
    
    void unLockTouch(float _time);
    
    void playWatchGlow();
    
    void clearDrop(float _time);
    
    void checkTileGlow(CCObject* obj);
    
    void onRemoveGuideParticle(CCObject* obj);

    void playArrowClickEnd();//播放连续点击 位置切换
    void playArrowClick();//播放连续点击
    void stopArrowClick(float _time);//停止播放
    
    void getScienceData();
    void recordGaid();
    void createActiveBd(CCObject* obj);
    
    void showSunshine();//雨过天晴
    void setSunAngle(float _time);//设定阳光角度
    void checkDayTime(bool isInit=false);//判断是否是白天
    void showRain();//下雨
    void showWaterfall();// 瀑布
    void showFirework(CCObject* obj);//烟花
    void startShowFirework(float t);//烟花序列特效
    void titanChangeStatus(CCObject* obj);//泰坦状态改变
    void handleTitanUpgrade(CCObject* obj);//泰坦升级完成
    void titanUpgradeComplete(CCObject* obj);//泰坦升级完成
    
    void onShowTalkView(CCObject* obj);
    void addDefenseFailEffect();//城防着火时主城死兵的效果
    //主城夜晚灯光
    void addNightLight();
    void removeAllNightLight();
    void changeDayOrNight(bool isNight, bool isAni);
    void addTorchToNode(CCNode *node, int index);
    CCSafeObject<CCNode> m_nodeBuildings[BUILD_COUNT];
    CCSafeObject<CCNode> titanRootNode;
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<SpeBuild> m_flagBuild;
    CCSafeObject<CCNode> m_hdNode;
    CCSafeObject<SpeBuild> m_hdBuild;
    CCSafeObject<CCNode> m_goldMineNode;
    CCSafeObject<SpeBuild> m_goldMineBuild;
    CCSafeObject<CCNode> m_goldBoxNode;
    CCSafeObject<SpeBuild> m_goldBoxBuild;
    CCSafeObject<CCNode> m_shipNode;
    CCSafeObject<SpeBuild> m_shipBuild;
    //商人
    CCSafeObject<CCNode> m_merchanteNode;
    CCSafeObject<SpeBuild> m_merchanteBuild;
    //转盘
//    CCSafeObject<CCNode> m_lotteryNode;
    CCSafeObject<SpeBuild> m_lotteryBuild;
    //货物
    CCSafeObject<CCNode> m_cargoNode;
    CCSafeObject<SpeBuild> m_cargoBuild;
    //兄弟会
    CCSafeObject<CCNode> m_xiongdiNode;
    CCSafeObject<SpeBuild> m_xiongdiBuild;
    //新手物资
    CCSafeObject<CCNode> m_newRDNode;
    CCSafeObject<SpeBuild> m_newRDBuild;
    //巨龙战入口
    CCSafeObject<CCNode> m_dragonNode;
    CCSafeObject<SpeBuild> m_dragonBuild;
    //火把
    CCSafeObject<CCNode> m_torchNode;
    CCSafeObject<CCNode> m_torchBatchNode;
    CCSafeObject<CCNode> m_torchNode1;
    CCSafeObject<CCNode> m_torchNode2;
    CCSafeObject<CCNode> m_torchNode3;
    CCSafeObject<CCNode> m_torchNode4;
    CCSafeObject<CCNode> m_torchNode5;
    CCSafeObject<CCNode> m_torchNode6;
    CCSafeObject<CCNode> m_torchNode7;
    CCSafeObject<CCNode> m_torchNode8;
    
    CCSafeObject<SpeBuild> m_mailBuild;
    
    CCSafeObject<CCNode> m_mcNode1;
    CCSafeObject<CCNode> m_mcNode2;
    CCSafeObject<CCNode> m_mcNode3;
    CCSafeObject<CCNode> m_mcNode4;
    CCSafeObject<CCNode> m_mcNode5;
    CCSafeObject<CCNode> m_desNode1;
    CCSafeObject<CCNode> m_desNode2;
    CCSafeObject<CCNode> m_desNode3;
    CCSafeObject<CCLayer> m_touchOldLayer;
    CCSafeObject<CCLayer> m_waterLayer;
    CCSafeObject<CCLayer> m_cloudLayer;
    CCSafeObject<CCLayer> m_bgParticleLayer;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCLayer> m_signLayer;
    CCSafeObject<CCLayer> m_spineLayer;
    CCSafeObject<CCLayer> m_nightLayer;
    CCSafeObject<CCLayer> m_funLayer;
    CCSafeObject<CCLayer> m_battleLayer;
    CCSafeObject<CCLayer> m_soldierLayer;
    
    CCSafeObject<CCLabelBatchNode> m_nameLayer;
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCParticleSystemQuad>  m_praticle;
    
    CCSafeObject<CCNode> m_flyNodes[10];
    
    CCSafeObject<CCNode> m_collectNode;
    CCSafeObject<CCSpriteBatchNode> m_collectBatch;
    
    CCSafeObject<CCNode> m_sunNode;
    CCSafeObject<CCSprite> m_sunshine[4];
    CCSafeObject<CCSprite> m_flyArrow;

    CCSafeObject<CCSpriteBatchNode> m_tilebatchNode;
    CCSafeObject<CCSpriteBatchNode> m_arrbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_resbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_rescustombatchNode;
    CCSafeObject<CCSpriteBatchNode> m_resBlentBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_soldierBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_chrTreeBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_chrTreeBlentBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_cludeBatchNode;
    CCSafeObject<CCNode> m_soldierFlagNode;
    map<int,CCSpriteBatchNode*> m_buildBatchMap;
    
    map<int,CCLayer*> m_buildSpineMap;
    
    CCSafeObject<CCArray> m_personArray;
    CCSafeObject<CCPointArray> m_ptArray;
    CCSafeObject<CCPointArray> m_pt1Array;
    CCSafeObject<CCPointArray> m_ptArrowClick;
    CCSafeObject<CCPointArray> m_pt2Array;
    CCSafeObject<CCArray> m_soldierArray;
    
    vector<CCParticleBatchNode*> m_parVec;
    vector<CCParticleBatchNode*> m_cloudVec;
    vector<CCParticleSystemQuad*> m_waveVec;
    
    CCSafeObject<CCNode> m_bigTileNode1;
    CCSafeObject<CCNode> m_bigTileNode2;
    CCSafeObject<CCNode> m_bigTileNode3;
    CCSafeObject<CCNode> m_bigTileNode4;
    CCSafeObject<CCNode> m_bigTileNode5;
    CCSafeObject<CCNode> m_FountainTouchNode;
    CCSafeObject<CCNode> m_fountainNode;
    CCSafeObject<CCNode> m_forestNode;
//    CCSafeObject<CCNode> m_couldNode;
    CCSafeObject<CCNode> m_crossNode;
    CCSafeObject<CCNode> m_crossClickNode;

    
    CCSafeObject<FunBuildBtnsView> m_buildBtnsView;
    CCSafeObject<SpeBuildBtnsView> m_speBuildBtnsView;
    CCLabelIF* m_feedlabel;
    CCSafeObject<CCSprite> m_feedBackHead;
    
    CCSafeObject<TalkNoticeCell> m_talkACTCell;
    CCSafeObject<TalkNoticeCell> m_talkACTCell2;
    
    CCSafeObject<CCNode> m_dropNode;
    CCSafeObject<CCNode> m_tileGlowNode;
    
    CCSafeObject<CCSprite> m_HTPHead;
    CCSafeObject<CCSprite> m_HTPHead1;
    CCLabelIF* m_HTPlabel;
    CCSafeObject<CCDictionary> m_ripple;
    CCSafeObject<CCNode> m_failEffectNode;
    CCSafeObject<CCNode> m_icon_op_mail;
    CCSafeObject<CCArray> m_torchPar;
    CCSafeObject<CCNode> m_nightLights;
    CCSafeObject<Layer> m_touchLayer;
    CCSafeObject<Sprite3D> m_Layer3d;
    CCSafeObject<Sprite3D> m_Layer2d;
    
    CCSafeObject<Titan> m_Titan;
    CCSafeObject<CCNode> m_node3d;
    // tao.yu titan node
    CCSafeObject<CCNode> m_titanNode;
    // tao.yu 维京船
    CCSafeObject<CCNode> m_vikingNode;
    bool m_isVikingShipMove;
    //begin a by ljf
    CCSafeObject<CCNode> m_Node0;
    CCSafeObject<CCNode> m_vikingPath1;
    CCSafeObject<CCNode> m_vikingPath2;
    CCSafeObject<CCNode> m_vikingPath3;
    CCSafeObject<CCNode> m_vikingPath4;
    CCSafeObject<NBSprite3D> m_vikings3D;
    CCSafeObject<CCNode> m_vikingTouchNode;
    //end a by ljf
    // tao.yu titan move path
    CCSafeObject<CCNode> m_tpath_1;
    CCSafeObject<CCNode> m_tpath_2;
    CCSafeObject<CCNode> m_tpath_3;
    CCSafeObject<CCNode> m_tpath_4;
    CCSafeObject<CCNode> m_wallNode;
    CCSafeObject<WallBuild> m_wallBuild;
    CCSafeObject<CCNode> m_wallZOrder_0;
    CCSafeObject<CCNode> m_wallZOrder_1;
    CCSafeObject<CCNode> m_wallZOrder_2;
    CCSafeObject<CCNode> m_wallZOrder_3;
    CCSafeObject<CCNode> m_wallZOrder_4;
    
    CCSafeObject<CCNode> m_waterfallNode;
    CCSafeObject<CCNode> m_waterfall_prt;
    
    std::map<int, CCSpriteBatchNode*> m_wallBatchs;
    
    void addGuideParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    CCSafeObject<CCNode> m_guideParticleNode;
    vector<CCParticleBatchNode*> m_guideParticleVec;
    
    int m_count;
    bool m_singleTouchState;
    bool m_buildingInitState;
    int m_mainPatPlayTime;
    int m_waterType;
    
    int m_curBuildId;
    int m_curBuildPosx;
    int m_curBuildPosy;
    int m_tmpMoveX;
    int m_tmpMoveY;
    bool m_canClick;
    
    float m_oldScale;
    float m_oldPosX;
    float m_oldPosY;
    float m_flyX;
    float m_flyY;
    int flyCount;
    int m_isOnlyPower;
    bool m_isSave;
    int m_tmpBuildPos;
    int m_removeSpeBId;
    bool m_curGuideEnd;
    bool m_isLogin;
    int m_beginTouchType;
    
    int _arrowPtIdx;
    bool m_forceMove;
    bool m_exit;
    
    bool m_isDay;
    bool m_isRain;
    int m_sysTime;
    int m_talkTime;
    int m_noticeCount;
    int m_helpCount;
    int startDarkTime;
    int completeDarkTime;
    int startLightTime;
    int completeLightTime;
    int m_useItemId;
    CCBAnimationManager* mActionManager;
};
#endif /* defined(__IF__ImperialScene__) */
