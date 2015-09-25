//
//  DropRdCCB.h
//  IF
//
//  Created by fubin on 14-10-14.
//
//

#ifndef IF_DropRdCCB_h
#define IF_DropRdCCB_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"

class DropRdCCB: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    bool initDropRdCCB(int type);
    static DropRdCCB* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void onPlayEnd();
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    
    CCSafeObject<CCArray> m_dropNodes;
    CCSafeObject<CCArray> m_dropSprs;
    
    vector<CCPoint> m_ptArray;
    
    void onPlayDrop();
    void onDropOneCell(CCObject* obj);
    void onPlayMusic(float _time);
    void onDelete(float _time);
    string musicName;
    string cellName;
};

#endif
