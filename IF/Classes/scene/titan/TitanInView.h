//
//  TitanInView.h
//  IF
//
//  Created by fusheng on 15-9-18.
//
//

#ifndef __IF__TitanInView__
#define __IF__TitanInView__

#include <stdio.h>
#include "cocos2d.h"
#include "Titan.h"
#include "CCTypesExt.h"
#include "CCIFTouchNode.h"
using namespace cocos2d;
class TitanInView: public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{

    
public:
    TitanInView():
    m_Titan(nullptr)
    {};
    virtual ~TitanInView(){};
    static TitanInView* create();
    
    
    void resetDisplay(int tid);
    CCSafeObject<Titan> m_Titan;
private:
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    CCSafeObject<CCNode> m_titanPos;
    CCSafeObject<CCLayerColor> m_titanTouchLayer;
    Vec2 m_TouchPoint;
    bool m_TouchMoved;
};

#endif /* defined(__IF__TitanInView__) */
