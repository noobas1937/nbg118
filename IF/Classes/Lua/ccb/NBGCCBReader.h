//
//  NBGCCBReader.h
//  nbg
//
//  Created by guojiang on 4/29/15.
//
//

#ifndef __nbg__NBGCCBReader__
#define __nbg__NBGCCBReader__

#include "INBGCCBDelegate.h"

class NBGCCBReader
: public Ref
, public INBGCCBDelegate
{
public:
    NBGCCBReader();
    virtual ~NBGCCBReader();
    
    void readCCB(const char * ccbiName);
    
    virtual bool onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * node);
    virtual void completedAnimationSequenceNamed(const char *name);
    
    Node* getNode() { return m_pCCBNode; }
    ccbAnimMgr* getAnimationManager() { return m_pAnimationManager; }
    
    ccbReaderAssignMemberVariableCallback assignMemVarCallback;
    ccbAnimationCompletedCallback animCompletedCallback;
    
    // lua
    int luaHandler_memberVarSetter; // (Node* mem, const char* name)
    int luaHandler_animationCallback; // const char* animationName
    
protected:
    void releaseMembers();
    
    Node* m_pCCBNode;
    ccbAnimMgr* m_pAnimationManager;
    
    friend class NBGNode;
};

#endif /* defined(__nbg__NBGCCBReader__) */
