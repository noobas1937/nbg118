//
//  HarvestSkillAni.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__HarvestSkillAni__
#define __IF__HarvestSkillAni__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"

class HarvestSkillAni: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    bool initHarvestSkillAni(int type);
    static HarvestSkillAni* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void onPlayEnd(float _time);
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    
    CCSafeObject<CCArray> m_dropNodes;
    CCSafeObject<CCArray> m_dropSprs;
    
    void onPlayMusic(float _time);
    void onDelete();
    string musicName;
};
#endif /* defined(__IF__HarvestSkillAni__) */
