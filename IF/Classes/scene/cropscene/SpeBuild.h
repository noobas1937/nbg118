//
//  SpeBuild.h
//  IF
//
//  Created by fubin on 14-5-15.
//
//

#ifndef IF_SpeBuild_h
#define IF_SpeBuild_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class SpeBuild: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    SpeBuild():m_key(0),mainWidth(0),mainHeight(0),m_isOpen(false),m_buildingKey(0),parentX(0),parentY(0),isCanClick(true),m_signLayer(NULL),m_popLayer(NULL),m_batchNode(NULL),m_popBatchNode(NULL),m_blentBatchNode(NULL),m_shipNode(NULL),m_shipToAnimation(""),m_shipCurAnimation(""){
        
    };
    ~SpeBuild(){};
    bool initSpeBuild(int itemId);
    static SpeBuild* create(int itemId);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickThis(float _time);
    void onShowTextAni(float _time);
    void onShowTextAni2(float _time);
    void onShowTextAniShip(float _time);
    void digMineSuccess(CCObject* param);
    
    virtual void onEnter();
    virtual void onExit();
    void setGary();
    void setWhite();
    void playFadeOut();
    void dtPlayFadeOut(float dt) {playFadeOut();} //a by ljf
    void playShadow();
    void stopShadow();
    void onBuildDelete();
    void onPlayUnlock();
    void checkLeftTime(float _time);
    void checkMine(float _time);
    void canShowState();
    void setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* popBatchNode, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch=NULL);
    void onClickRecBtn(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void addSpeBuildState();
    void removeSpeBuildState(CCObject* obj);
    int m_key;
    int mainWidth;
    int mainHeight;
    bool m_isOpen;
    
    void updateShipState(string state ="");
    void shipLeave(float _time);
    CCSafeObject<CCNode> m_guideNode;
protected:
    int m_buildingKey;
    int parentX;
    int parentY;
    bool isCanClick;
    string m_timeStr;
    double m_countTime;
    int m_gap;
    
    CCLayer* m_signLayer;
    CCLayer* m_popLayer;
    CCSpriteBatchNode* m_batchNode;
    CCSpriteBatchNode* m_popBatchNode;
    CCSpriteBatchNode* m_blentBatchNode;
    
    CCSafeObject<CCNode> m_signCCBNode;
    CCSafeObject<CCSprite> m_spr;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_spineNode;
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCSprite> m_bg;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_upEffectNode;
    CCSafeObject<CCNode> m_textNode;
    CCSafeObject<CCNode> m_textTouchNode;
    CCSafeObject<CCNode> m_spTextNode;
    
    CCSafeObject<CCArray> m_sprArray;
    CCSafeObject<CCArray> m_blentSprArray;
    CCSafeObject<CCArray> m_personArray;
    
    CCSafeObject<CCLabelBatch> m_lvLabel;
    CCSafeObject<CCLabelIF> m_HTPlabel;
    CCSafeObject<CCLabelIF> m_timelabel;
    CCSafeObject<CCLabelIFTTF> m_timeEventlabel;
    CCSafeObject<CCSprite> m_arrSpr;
    CCSafeObject<CCSprite> m_goldMinLock;
    CCSafeObject<CCNode> m_goldAniNode;
    CCSafeObject<CCNode> m_btnNode;
    
    CCSafeObject<CCSprite> m_effectSpr;
    CCSafeObject<CCPointArray> m_ptArray;
    
    void drowEffectSpr(int zOrder, int tmpOrd);
    void drowPersonSpr(int zOrder, int tmpOrd);
    
    void onCanClick(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    
    void onShowParticle();
    void onHideParticle();
    void dailyQuestCallBack(CCObject* pObj);
    string changeShowIcon();
    string randomShowIcon();
    
private:
    CCNode* m_shipNode;
    CCRect m_shipParticalRect[2];
    void changeShipAnimation(string animation,string toAnimation = "",bool partical=true);
    void resetShipAnimation();
    bool canClickShip();
    void clickShip(float _time);
    void clickShipCallBack(CCObject* obj);
    void addShipParticle(string state);
    void addShipPop(CCObject *ccObj);
    void hideNewRDTip(CCObject *ccObj);
    void playShakeState();
    string m_shipToAnimation;
    string m_shipCurAnimation;
    
    string m_touchName;
    string m_clickNameKey;
};

#endif
