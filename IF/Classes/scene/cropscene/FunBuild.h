//
//  FunBuild.h
//  IF
//
//  Created by fubin on 13-10-31.
//
//

#ifndef IF_FunBuild_h
#define IF_FunBuild_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"
#include "BuildCCB.h"
#include "IFSkeletonAnimation.h"

#define MSG_REMOVE_STATE "msg_remove_state"
#define MSG_PLAY_SHADOW "msg_play_shadow"
#define MSG_STOP_SHADOW "msg_stop_shadow"
#define MSG_SET_GARY "msg_set_gary"
#define MSG_SET_WHITE "msg_set_white"

#define MSG_PLAY_BUILD_SHADOW "msg_play_build_shadow"
#define MSG_STOP_BUILD_SHADOW "msg_stop_build_shadow"

class FunBuildState;

class FunBuild: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    FunBuild():m_key(0),m_type(0),isShowUping(false),mainWidth(0),mainHeight(0),m_buildingKey(0),m_info(NULL),isChange(true)
    ,parentX(0),parentY(0),arrY(0),isUnLock(false),isCanClick(true),isEffectRunning(false),m_zOrder(0),m_clickType(0),m_output(0),m_buildAnimatSt("")
    ,m_nameLayer(NULL),m_signLayer(NULL),m_popLayer(NULL),m_batchNode(NULL),m_popBatchNode(NULL),m_blentBatchNode(NULL),m_spineLayer(NULL),m_tipNode(NULL),m_spr1(NULL),m_customSkinPar(NULL){};
    ~FunBuild(){};
    bool initFunBuild(int itemId, CCLabelBatchNode *nameLayer);
    static FunBuild* create(int itemId, CCLabelBatchNode *nameLayer);
    
    CCNode* getGuideNode(string _key);
    void showTmpBuild(int itemId);
    void hideTmpBuild();
    static FunBuild* createTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch=NULL);
    bool initTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch=NULL);
    
    void setTime(int times, bool updateLv=false);
    void changeBg();
    void showUping();
    void hideUping();
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void onCCBClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    bool myTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void retTouch(CCTouch *pTouch, CCEvent *pEvent);
    
    void onSetClickType();
    void onClickThis(float _time);
    
    virtual void onEnter();
    virtual void onExit();
    void onBuildDelete();
    void onOpenLock();
    void setGary();
    void setWhite();
    void onPlayUpEnd(bool isOnly=false);
    void playShadow();
    void stopShadow();
    void onPlayUpIcon();
    void stopUpIcon();
    
    void playBuildShadow(CCObject* obj);
    void stopBuildShadow(CCObject* obj);
    
    void onShowBuildState(CCObject* params);
    void onRemoveBuildState(CCObject* params);
    void unLockTile(CCObject* params);
    
    void onFlyOutPut(float _time);
    void onFlyOutPutByValue(int Value);
    
    void onUpdateUpIcon(float dt);
    void checkIncreasedStatus();
    void showHarvestAni(CCObject* params);
    void playFortSkillEffect(CCObject* params);
    void playFortSkillEffect(float _time);
    void showFlyOut(float _time);
    
    void canShowState();
    void setNamePos(int x, int y, CCLayer* sginLayer, CCLayer* popLayer, CCSpriteBatchNode* popBatchNode, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch=NULL);
    void setSpineLayer(CCLayer* spineLayer);
    void addTips();
    void removeTips();
    void setTileBatch(int x, int y, CCSpriteBatchNode* batchNode, int zOrder);
    int m_key;
    int m_type;
    bool isShowUping;
    int mainWidth;
    int mainHeight;
    int m_buildingKey;
    int addResTime;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> m_tile;
    CCSafeObject<CCSprite> m_moveFrame;
    CCSafeObject<CCSprite> m_spr;
    
    //custom skin
    CCSafeObject<CCSprite> m_spr1;
protected:
    FunBuildInfo* m_info;
    bool isChange;
    int parentX;
    int parentY;
    float arrY;
    bool isUnLock;
    bool isCanClick;
    bool isEffectRunning;
    //beign a by ljf
    int m_effectState;
    //end a by ljf
    int m_zOrder;
    int m_clickType;
    int m_output;
    string m_buildAnimatSt;
    
    CCLabelBatchNode* m_nameLayer;
    CCLayer* m_signLayer;
    CCLayer* m_popLayer;
    CCSpriteBatchNode* m_batchNode;
    CCSpriteBatchNode* m_popBatchNode;
    CCSpriteBatchNode* m_blentBatchNode;
    
    CCLayer* m_spineLayer;
    
    CCSafeObject<CCNode> m_lvNode;
    CCSafeObject<CCNode> m_arrNode;
//    CCSafeObject<CCSprite> m_spr;
    CCSafeObject<CCSprite> m_lvBG;
    
    //begin a by ljf
    CCSafeObject<CCNode> m_spineNode;
    //end a by ljf
    
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCSprite> m_bg;
    CCSafeObject<CCSprite> m_lockIcon;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_upEffectNode;
    CCSafeObject<CCNode> m_forgeEffectNode;
    CCSafeObject<CCNode> m_tipNode;
    
    CCSafeObject<CCArray> m_sprArray;
    CCSafeObject<CCArray> m_blentSprArray;
    CCSafeObject<CCArray> m_personArray;
    
    CCSafeObject<CCLabelBatch> m_lvLabel;
    CCSafeObject<CCSprite> m_arrSpr;
    CCSafeObject<CCSprite> m_gainSpr;
    
    CCSafeObject<CCSprite> m_effectSpr;
    CCSafeObject<CCPointArray> m_ptArray;
    
    CCSafeObject<FunBuildState> m_buildState;
    CCSafeObject<BuildCCB> m_buildCCB;
    
    CCSafeObject<IFSkeletonAnimation> m_spineAni;
    
    bool isTouchInBtns(CCTouch *pTouch, CCEvent *pEvent);
    void onStopUpEffect(float _time);
    void drowEffectSpr(int zOrder, int tmpOrd);
    void drowPersonSpr(int zOrder, int tmpOrd);
    void playCompleteSound(float _time);
    
    void addWorkShopBuildState(CCObject* param = NULL);
    void addFunBuildState();
    void removeFunBuildState();
    void quickProduceTroop(CCObject* param);
    void buildFunctionEffect(CCObject* param);
    
    void onMoveBuild(CCObject* param);
    void onCancelLastMoveBuild(CCObject* param);
    void onCancelMoveBuild(CCObject* param);
    int getCurPos();
    
    void updateLvInfo();
    void playLockJump();
    void playOpenLock();
    void onCanClick(float _time);
    void onPlaySpeEffect(bool show);
    void onShowSpeEffect(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void addForgeFireParToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    vector<CCParticleBatchNode*> m_forgeFireParVec;
    //换皮的特效
    CCParticleBatchNode* m_customSkinPar;
    void addCustomSkinParticle();
    
    void addSpeParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void onHideSpeParticle();
    void onHideForgeFirePar();
    vector<CCParticleBatchNode*> m_speVec;
    
    vector<CCSprite*> m_speEffectVec;
    
    void onShowParticleByType(int type);
    void onShowParticle();
    void onHideParticle();
    CCPoint IsNeedMove(float x, float y, float scale);
    bool isCanRecState();
    void onStartFlyOutPut();
    void onPlayParticle();
    void onCheckOutPut();
    void onResetDirc(float _time);
    void onPlayBall(float _time);
private:
    void showForgeFire(float dt);
    
    string STClickName;
    
    void initEffectState(); //a by ljf
};

class FunBuildState: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
 
{
public:
    FunBuildState():CanDel(true),m_info(NULL),m_buildingKey(0),isChange(true),m_qType(0),m_qid(QID_MAX),m_sumTime(1),m_curTime(0){};
    ~FunBuildState(){};
    bool init(int itemId);
    static FunBuildState* create(int itemId);
    
    CCNode* getGuideNode(string _key);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    bool isCanClick();
    void ClickState();
    void onShowRecState();
    void showUpNode(bool isShow);
    void onCheckAllianceHelp();
    void onHideBtn(int type);
    CCSafeObject<CCNode> m_upNode;
    bool CanDel;
    void autoClickRecBtn();
protected:
    FunBuildInfo* m_info;
    int m_buildingKey;
    bool isChange;
    
    int m_qType;
    int m_qid;
    int m_sumTime;
    int m_curTime;
    
    void onFreeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRecBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onGetBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEnterFrame(float dt);
    void onCheckTime(CCObject* params);
    void onPlayCanRec(int type);
    void onGetResAction(CCObject* params);
    
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCScale9Sprite> m_bar2;
    CCSafeObject<CCNode> m_iconNode;
    
    CCSafeObject<CCNode> m_freeNode;
    CCSafeObject<CCNode> m_freeTouchNode;
    CCSafeObject<CCControlButton> m_freeBtn;
    CCSafeObject<CCLabelIF> m_freeLabel;
    
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCNode> m_helpIconNode;
    
    CCSafeObject<CCNode> m_recNode;
    CCSafeObject<CCControlButton> m_recBtn;
    CCSafeObject<CCNode> m_recIcon;
    
    CCSafeObject<CCNode> m_getNode;
    CCSafeObject<CCControlButton> m_getBtn;
    CCSafeObject<CCNode> m_getIconNode;
private:
    
};

#endif /* defined(__IF__FunBuild__) */
