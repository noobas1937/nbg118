//
//  Animal.h
//  IF
//
//  Created by fubin on 14-7-28.
//
//

#ifndef IF_Animal_h
#define IF_Animal_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class Animal: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    Animal():mainWidth(0),mainHeight(0),m_buildingKey(0),parentX(0),parentY(0),isCanClick(true),m_signLayer(NULL),m_batchNode(NULL){};
    ~Animal(){};
    bool initAnimal(int itemId, int type);
    static Animal* create(int itemId, int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickThis(float _time);
    
    virtual void onEnter();
    virtual void onExit();
    void onAnimalDelete();
    
    void setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* batchNode, int zOrder);
    int mainWidth;
    int mainHeight;
    
protected:
    int m_buildingKey;
    int parentX;
    int parentY;
    bool isCanClick;
    
    CCLayer* m_signLayer;
    CCSpriteBatchNode* m_batchNode;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_upEffectNode;
    
    CCSafeObject<CCArray> m_sprArray;
    
    CCSafeObject<CCSprite> m_effectSpr;
    CCSafeObject<CCPointArray> m_ptArray;
    
    void drowEffectSpr(int zOrder, int tmpOrd);
    
    void onCanClick(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    
    void onShowParticle();
    void onHideParticle();
    
private:
    
};

#endif
