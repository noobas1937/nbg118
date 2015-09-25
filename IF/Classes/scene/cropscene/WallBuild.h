//
//  WallBuild.h
//  IF
//
//  Created by fubin on 14-7-25.
//
//

#ifndef IF_WallBuild_h
#define IF_WallBuild_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class WallBuild: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    WallBuild():m_key(0),mainWidth(0),mainHeight(0),m_isOpen(false),parentX(0),parentY(0),isCanClick(true),m_signLayer(NULL),m_popLayer(NULL),m_st(0){};
    ~WallBuild(){};
    bool initWallBuild();
    static WallBuild* create();
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickThis(float _time);
    
    virtual void onEnter();
    virtual void onExit();
    void setGary(CCObject* obj);
    void setWhite(CCObject* obj);
    void playFadeOut();
    void playShadow(CCObject* obj);
    void stopShadow(CCObject* obj);
    void onBuildDelete();
    void onPlayUnlock();
    
    void canShowState();
    void setNamePos(int x, int y, CCLayer* sginLayer, std::map<int, CCSpriteBatchNode*> *batches, int zOrder);
    int m_key;
    int mainWidth;
    int mainHeight;
    bool m_isOpen;
    
protected:
    int parentX;
    int parentY;
    bool isCanClick;
    
    CCLayer* m_signLayer;
    CCLayer* m_popLayer;
//    CCSpriteBatchNode* m_backBatchNode;
//    CCSpriteBatchNode* m_frontBatchNode;
    std::map<int,CCSpriteBatchNode*> m_arrBatchNodes;
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_upEffectNode;
    CCSafeObject<CCNode> m_fTouchNode;
    
    CCSafeObject<CCSprite> m_effectSpr;
    CCSafeObject<CCPointArray> m_ptArray;
    
//    CCSafeObject<CCArray> m_walls_0;
//    CCSafeObject<CCArray> m_walls_1;
//    CCSafeObject<CCArray> m_walls_2;
//    CCSafeObject<CCArray> m_walls_3;
//    CCSafeObject<CCArray> m_walls_4;
    std::map<int, CCSafeObject<CCArray>> m_arrWalls;
    CCSafeObject<CCArray> m_touchNodes;
    
    void drowEffectSpr(int zOrder, int tmpOrd);
    void drowPersonSpr(int zOrder, int tmpOrd);
    
    void onCanClick(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    
    void onShowParticle();
    void onHideParticle();
    void onEnterFrame(float dt);
    int m_st;
    
private:
    
};

#endif
