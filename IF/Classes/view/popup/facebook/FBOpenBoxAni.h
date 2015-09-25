//
//  FBOpenBoxAni.h
//  IF
//
//  Created by ganxiaohua on 14-11-20.
//
//

#ifndef __IF__FBOpenBoxAni__
#define __IF__FBOpenBoxAni__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"

class FBOpenBoxAni: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    bool initFBOpenBoxAni(int type);
    static FBOpenBoxAni* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void onPlayEnd();
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    CCSafeObject<CCNode> m_oneNode;
    CCSafeObject<CCNode> m_twoNode;
    CCSafeObject<CCNode> m_fiveNode;
    CCSafeObject<CCNode> m_tenNode;
    CCSafeObject<CCNode> m_mutiNode;
    
    void onPlayMusic(float _time);
    void onDelete();
    string musicName;
    int m_type;
};
#endif /* defined(__IF__FBOpenBoxAni__) */
