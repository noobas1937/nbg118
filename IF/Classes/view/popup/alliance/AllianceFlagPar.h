//
//  AllianceFlagPar.h
//  IF
//
//  Created by ganxiaohua on 14-8-20.
//
//

#ifndef __IF__AllianceFlagPar__
#define __IF__AllianceFlagPar__

#include "CommonInclude.h"
#include "CCAniNode.h"

class AllianceFlagPar : public cocos2d::Layer
 
{
public:
    AllianceFlagPar(std::string icon):m_icon(icon){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    virtual ~AllianceFlagPar(){}
    
    static AllianceFlagPar* create(std::string icon);
    
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void createPar(std::string icon);
    void bloodPar();
    void snowPar();
    void warhammerPar();
    void treePar();
    void curePar();
    void deathPar();
    void firePar();
    void oceanPar();
    void sunPar();
    void wolfPar();
    
    CCSafeObject<CCNode> m_partileNode;
    CCSafeObject<CCNode> m_clickParNode;
    CCSafeObject<CCScale9Sprite> m_clickNode;
    std::string m_icon;
    unsigned long m_touchTime;
};

class FlagAniCCB:public CCAniNode
//, public cocos2d::CCTargetedTouchDelegate
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    FlagAniCCB(){};
    ~FlagAniCCB(){};
    bool init(std::string flag);
    static FlagAniCCB* create(std::string flag);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
private:
    std::string m_flag;
};
#endif /* defined(__IF__AllianceFlagPar__) */
