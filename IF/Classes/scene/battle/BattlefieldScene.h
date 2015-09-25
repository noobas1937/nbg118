//
//  BattlefieldScene.h
//  IF
//
//  Created by ganxiaohua on 14-6-18.
//
//

#ifndef __IF__BattlefieldScene__
#define __IF__BattlefieldScene__

#include "CommonInclude.h"
#include "HFViewport.h"
#include "BattleArmy.h"
#include "IFSkeletonAnimation.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class BattlefieldScene :public CCLayer
,public ITouchDelegate
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    BattlefieldScene():m_bmIdx(0){};
    ~BattlefieldScene(){
        CCLOG("BattleScene");
    };
    CREATE_FUNC(BattlefieldScene);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void destory();
protected:
    bool init();
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void goBack(float _time);
    void goBack(CCObject* obj);
    int m_sIndex;
    float m_countTime;
    void moveScreen(float _time);
    void noticeToSoldier(float _time);
    void guidSoldier(float _time);
    
    void noticeToGen(float _time);
    void noticeToGen(CCObject* obj);
    
    void startAttack();
    void initBattle();
    void restart();
    void moveToTheGate(float _time);
    void atBattleAttack(float _time);
private:
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void onShowParticle();
    void onHideParticle();
    
    void addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void onShowWarParticle();
    void onHideWarParticle();
    
    void startPlayBoom(float _time);
    void playBoombParticle(float _time);
    
    void playWallDestroy(float _time);
    void playWallFire(float _time);
    
    void removeSelf(float _time);
    void removeSelf(CCObject* obj);
    
    void startPlayChe(float _time);
    void playBigFireBall(float _time);
    void playEndBigBall(CCObject* obj);
    void playEndBigBall1(CCObject* obj);
    
    CCSafeObject<CCControlButton> m_goBackBtn;
//    CCSafeObject<CCNode> m_centerNode;
//    CCSafeObject<CCNode> m_battleNode;
//    CCSafeObject<CCNode> m_bgNode;
//    CCSafeObject<CCNode> m_forceNode;
//    CCSafeObject<CCLabelIF> m_sLabel;
    CCSafeObject<BattleArmy> m_battleGen;
    CCSafeObject<BattleArmy> m_battleZhuan;
    CCSafeObject<CCSpriteBatchNode> m_soldierNode;
    
//    CCSafeObject<CCNode> m_mainNode;
    
    HFViewport* m_viewPort;
    int m_startIndex;
    CCSafeObject<CCArray> m_attackArmys;
    CCSafeObject<CCArray> m_defArmys;
    CCSafeObject<CCArray> m_attackChe;
    
    CCSafeObject<CCArray> m_atkQiNodes;
    CCSafeObject<CCArray> m_atkGongNodes;
    CCSafeObject<CCArray> m_atkCheNodes;
    CCSafeObject<CCArray> m_atkBuNodes;
    CCSafeObject<CCArray> m_defGongNodes;
    CCSafeObject<CCArray> m_defCheNodes;
    
    CCSafeObject<CCArray> m_solParNodes;
    CCSafeObject<CCArray> m_warParNodes;
    CCSafeObject<CCArray> m_warSFiresNodes;
    CCSafeObject<CCArray> m_warBmNodes;
    CCSafeObject<CCArray> m_wallSmokeNodes;
    CCSafeObject<CCArray> m_wolfSmokeNodes;
    
    CCSafeObject<CCNode> m_atk_gen1;
    CCSafeObject<CCNode> m_def_gen1;
    CCSafeObject<CCNode> m_wallNode;
    CCSafeObject<CCNode> m_desWallParNode;
    
    CCSafeObject<CCLayer> m_warParLayer;
    CCSafeObject<CCLayer> m_solParLayer;
    
    CCSafeObject<CCNode> m_wallDesAni;
    
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrAttackers, ArrAttackers);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_attGenerals, AttGenerals);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrDefenders, ArrDefenders);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_defGenerals, DefGenerals);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_sequenceList, SequenceList);
    CC_SYNTHESIZE_SAFEOBJECT(CCLayer, m_spainLayer, SpainLayer);
    CC_SYNTHESIZE_SAFEOBJECT(CCSpriteBatchNode, m_jianNode, JianNode);
    CC_SYNTHESIZE_SAFEOBJECT(CCSpriteBatchNode, m_bloodNode, BloodNode);
    
    vector<CCParticleBatchNode*> m_parVec;
    vector<CCParticleBatchNode*> m_warParVec;
    CCSafeObject<CCParticleBatchNode> m_guideParticleNode;
    
    int m_bmIdx;
};
#endif /* defined(__IF__BattlefieldScene__) */
