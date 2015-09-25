//
//  AllianceAni.h
//  IF
//
//  Created by liweiyu on 14-8-14.
//
//

#ifndef __IF__AllianceAni__
#define __IF__AllianceAni__

#include "CommonInclude.h"
#include "CCAniNode.h"

class AllianceAni:public CCAniNode

, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    AllianceAni(){};
    ~AllianceAni(){};
    bool init();
    static AllianceAni* create();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCSprite> m_alliance;

private:

};
#endif /* defined(__IF__AllianceAni__) */
