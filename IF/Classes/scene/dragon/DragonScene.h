//
//  DragonScene.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonScene__
#define __IF__DragonScene__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "DragonBuild.h"
#include "HFViewport.h"
#include "CCLabelBatch.h"
#include "ArcPopupBaseView.h"
#include "SpeBuild.h"
#include "DragonBuildBtnsView.h"
#include "SpeBuildBtnsView.h"
#include "WallBuild.h"
#include "FlyCell.h"
#include "Animal.h"
#include "TalkNoticeCell.h"
#include "IFSkeletonBatchLayer.h"

#define BINDING_TIME "bindingTime"
#define ANNOUCE_TIME "Time"
#define FIRST_OPEN_AFTER_UPDATE "firstOpenAfterUpdate1"
#define MSG_TALK_OPEN "msg.talk.open"
#define BUILD_COUNT 59
#define MSG_SHOW_EXP_PARTICLE "msg.show.exp.particle"
#define MSG_SHOW_POWER_PARTICLE "msg.show.power.particle"

class DragonScene:public CCLayer,public ITouchDelegate,public CCBMemberVariableAssigner
{
public:
    static DragonScene* instance();
    
    DragonScene():lastTouchBuildId(-1),curTouchBuildId(-1),m_count(0),m_singleTouchState(false),m_waterType(0)
    ,m_curBuildId(0),m_curBuildPosx(0),m_curBuildPosy(0),m_tmpMoveX(0),m_tmpMoveY(0),m_canClick(true),m_buildingInitState(false)
    ,m_oldScale(1.0),m_oldPosX(0),m_oldPosY(0),m_isOnlyPower(0),m_isSave(false),m_tmpBuildPos(0),m_removeSpeBId(0),m_curGuideEnd(false),m_isLogin(false),m_forceMove(false),m_exit(false),m_isDay(false),m_isRain(false),m_sysTime(0),m_talkTime(0){};
    
    virtual ~DragonScene(){};
    CREATE_FUNC(DragonScene);
    bool init();
    CC_SYNTHESIZE(CCLayer*, buildingLayer, BuildingLayer);
    CC_SYNTHESIZE(CCLayer*, fixLayer, FixLayer);
    CC_SYNTHESIZE(CCSpriteBatchNode*, mbatchNode, BatchNode);
    
    virtual void onEnter();
    virtual void onExit();
    
    void leaveDragonScene();
    
    //收到消息后相应的方法
    void onGetInitDirChange(CCObject* obj);
    void onGetBuildInitEnd(CCObject* obj);
    void onGetBuildChangeEnd(CCObject* obj);
    void onGetBuildUpingEndEnd(CCObject* obj);
    void onGetBuildCreateErr(CCObject* obj);
    void onGetBuildUpingErrEnd(CCObject* obj);
    void onGetBuildNormalErrEnd(CCObject* obj);
    void onGetBuildCancelOthEnd(CCObject* obj);
    
    void onGetBuildCanShowState(CCObject* obj);
    void onGetBuildSetTime(CCObject* obj);
    void onGetBuildNormalIng(CCObject* obj);
    
    void onUpdateInfo();
    void onCreateBuild(int itemId);
    void onOpenBuild(int itemId);
    void onResetBuild(int itemId);
    void onEnterFrame(float dt);
    void onOpenBuildEnd(int itemId);
    
    void onMoveToBuild(int itemId, bool st=false);
    void onMoveToBuildAndPlay(int itemId, bool st=false);
    void onMoveToBuildAndOpen(int itemId, int type=TYPE_POS_DEF, float dt=0.25, bool bound=false);
    void onMoveToPos(float x, float y, int type=TYPE_POS_DEF, float dt=0.25, float scale=1.3, bool bound=false);
    void onMoveToPosDir(float sx, float sy, float ex, float ey, float dt=0.25, bool bound=false);
    CCPoint onGetSceenPt(float x, float y);
    float onGetTouchLayerScale();
    void onSaveCurPos(CCObject* obj);
    void onGoBackOldPos(CCObject* obj);
    void onFlyOutPut(int itemId, int output, int forceResType=-1);
    
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
    void onResetLastBuildId();
    
    void onShowBtnsView(int x, int y, int buildId);
    void onFlyTroopParticle(int x, int y, int onlyPower=0);
    void onFlyTroopEnd(CCObject *obj);
    void onFlyTroopEndToUI(CCObject *obj);
    
    int findCanBuildTile(int pos);
    void flyBall(int buildId, int type, bool toScene);
    
    void makeCanPlayArrowPoint();//计算哪些建筑播放 手指点击
    void hideFlyArrow(float _time);
private:
    int lastTouchBuildId;
    int curTouchBuildId;
    void onResourcesUpdate(cocos2d::CCObject* params);
    void onBattleUpdate(CCObject* params);
    void unLockTile(CCObject* params);
    void refreashTile();
    
    void onOpenNewBuild(CCObject* obj);
    void onUpgradeNewBuild(int buildId);
    void flyBallEnd(CCNode* pSender);
    void makeFlyMc(int type, CCNode* flNode);
    void flyOutPut(int buildId, CCNode* flNode, int idx);
    void flyOutPutEnd(CCNode* pSender);
    void showAttention();
    void showAttention1();
    
    void startGuide(float _time);
    
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    bool isGeneralOnFormationBuilding(int buildingId);
    void canSingleTouch(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    
    void onRemoveTmpBuild(CCObject* obj);
    void onEnableMoveScence(float _time);
    
    void buildingCallBack(CCObject* params);
    void showHarvestEffect(CCObject* obj);
    void scheduleHarvestEffect(float _time);
    void guideEnd(CCObject* obj);
    void retGuideEnd(float _time);
    void clearGuideState(float _time);
    void playFlyTroopParticle(float _time);
    
    void onFinishMoveBuild(CCObject* obj);
    
//    void onPowerADD(CCObject* obj);
    
    map<int, DragonBuild*> m_buildItems;
    void moveMapToPosition(CCObject* obj);
    
    void unLockTouch(float _time);
    
    void playWatchGlow();
    
    void clearDrop(float _time);
    
    void checkTileGlow(CCObject* obj);
    
    void playArrowClickEnd();//播放连续点击 位置切换
    void playArrowClick();//播放连续点击
    void stopArrowClick(float _time);//停止播放
    
    void showSunshine();//雨过天晴
    void setSunAngle(float _time);//设定阳光角度
    void checkDayTime(bool isInit=false);//判断是否是白天
    void showRain();//下雨
    
    CCSafeObject<CCNode> m_nodeBuildings[BUILD_COUNT];
    
    CCSafeObject<CCLayer> m_touchLayer;
    CCSafeObject<CCLayer> m_waterLayer;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCLayer> m_signLayer;
    CCSafeObject<CCLayer> m_funLayer;
    
    CCSafeObject<CCLabelBatchNode> m_nameLayer;
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCParticleSystemQuad>  m_praticle;
    
    CCSafeObject<CCNode> m_collectNode;
    CCSafeObject<CCSpriteBatchNode> m_collectBatch;
    
    CCSafeObject<CCNode> m_sunNode;
    CCSafeObject<CCSprite> m_flyArrow;
    
    map<int, CCSpriteBatchNode*> m_buildBatchMap;
    CCSafeObject<CCPointArray> m_ptArrowClick;
    vector<CCParticleBatchNode*> m_parVec;
    CCSafeObject<DragonBuildBtnsView> m_buildBtnsView;
    
    CCSafeObject<CCNode> m_dropNode;
    CCSafeObject<CCNode> m_tileGlowNode;
    
    int m_count;
    bool m_singleTouchState;
    bool m_buildingInitState;
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
};
#endif /* defined(__IF__DragonScene__) */
