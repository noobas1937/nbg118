//
//  AAreaBuildCCB.h
//  IF
//
//  Created by wangdianzhen on 15/4/27.
//
//

#ifndef __IF__AAreaBuildCCB__
#define __IF__AAreaBuildCCB__

#include "CommonInclude.h"
#include "CCAniNode.h"
class AAreaBuildCCB :public CCAniNode,
public CCBMemberVariableAssigner,
public CCBSelectorResolver
{
public:
    CREATE_FUNC(AAreaBuildCCB);
    bool  init();
    void OnPlayOver();
    AAreaBuildCCB* setCCBName(string ccbName);
protected:
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    virtual void onEnter();
    virtual void onExit();
private:
    void OnRemoveSelf();
};

#endif /* defined(__IF__AAreaBuildCCB__) */
