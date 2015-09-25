//
//  FlyCBuildView.h
//  IF
//
//  Created by fubin on 14-9-22.
//
//

#ifndef IF_FlyCBuildView_h
#define IF_FlyCBuildView_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"

class FlyCBuildView: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    bool initFlyCBuildView();
    static FlyCBuildView* create();
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void onShowInfo(int exp, int power);
    void onShowPowerInfo(int power);
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCNode> m_expItemNode;
    CCSafeObject<CCNode> m_powerItemNode;
    
    CCSafeObject<CCNode> m_expNode;
    CCSafeObject<CCNode> m_powerNode;
    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCLabelIF> m_expName;
    CCSafeObject<CCLabelIF> m_expValue;
    CCSafeObject<CCLabelIF> m_powerName;
    CCSafeObject<CCLabelIF> m_powerValue;
    
    void onPlayBoom(float _time);
    
};

#endif
