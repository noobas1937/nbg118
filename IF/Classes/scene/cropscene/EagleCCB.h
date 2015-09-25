//
//  EagleCCB.h
//  IF
//
//  Created by fubin on 14-10-8.
//
//

#ifndef IF_EagleCCB_h
#define IF_EagleCCB_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class EagleCCB: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    EagleCCB(){};
    ~EagleCCB(){};
    bool initEagleCCB();
    static EagleCCB* create();
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    virtual void onEnter();
    virtual void onExit();
    void onPlayNext();
    void onPlay(float _time);
    
protected:
private:
    
};

#endif
