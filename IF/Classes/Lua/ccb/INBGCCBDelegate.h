//
//  INBGCCBDelegate.h
//  nbg
//
//  Created by guojiang on 4/29/15.
//
//

#ifndef nbg_INBGCCBDelegate_h
#define nbg_INBGCCBDelegate_h

#include "cocos2d.h"
#include "cocosbuilder/CocosBuilder.h"

using namespace cocos2d;

typedef cocosbuilder::CCBAnimationManager ccbAnimMgr;

#pragma mark * ccbi

template<typename code_memberVariableTypePoint>
bool __BA_CCB_MEM_VAR_ASSIGNER_GLUE(
    // NBGCCBReader 读取 ccbi 时回调函数 onAssignCCBMemberVariable 的参数
    Ref * ccb_target,  const char * ccb_memberVariableName,  Node * ccb_node,
    // 跟 ccbi 绑定对应的代码 (比如 NBGNode 的 m_pNBGCCBReader)
    Ref * code_target, const char * code_memberVariableName, code_memberVariableTypePoint * code_memberVariable)
{
    if (ccb_target == code_target && nullptr != code_memberVariableName && 0 == strcmp(ccb_memberVariableName, code_memberVariableName))
    {
        code_memberVariableTypePoint pOldVar = *code_memberVariable;
        *code_memberVariable = dynamic_cast<code_memberVariableTypePoint>(ccb_node);
        CC_ASSERT(*code_memberVariable);
        if (pOldVar != (*code_memberVariable))
        {
            CC_SAFE_RELEASE(pOldVar);
            (*code_memberVariable)->retain();
        }
        return true;
    }
    return false;
}

#define BA_CCB_MEM_VAR_ASSIGNER_GLUE(TYPE, VAR) \
if (__BA_CCB_MEM_VAR_ASSIGNER_GLUE<TYPE*>(pTarget, pMemberVariableName, node, pTarget, myNri->VAR, &VAR)) { \
    return true; \
}

#pragma mark * INBGCCBDelegate

class INBGCCBDelegate
: public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::CCBAnimationManagerDelegate
{
};

#pragma mark * IBA2CCBAnimationDelegate

class IBA2CCBAnimationDelegate
{
public:
    virtual void onAnimationCompleted(Node* animationPlayer, const char *timelineName) = 0;
};

#pragma mark * 

typedef std::function<bool(Ref * pTarget, const char * pMemberVariableName, Node * node)> ccbReaderAssignMemberVariableCallback;
typedef std::function<void(const char *animationSequenceName)> ccbAnimationCompletedCallback;

#endif
