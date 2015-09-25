//
//  CCCallStack.h
//  GameCore
//
//  Created by Chenfei on 13-11-4.
//
//

#ifndef __GameCore__CCCallStack__
#define __GameCore__CCCallStack__

#include "cocos2d.h"
#include <unwind.h>


NS_CC_BEGIN

typedef struct _CallStackFrame
{
    uintptr_t ip;
//    uintptr_t ebp;
//    uintptr_t esp;
//    uintptr_t region_start;
    char* name[49];
} CallStackFrame;

typedef std::list<CallStackFrame> CallStack;

class CCCallStack
{

public:
    CCCallStack(unsigned int stackDepth);
    virtual ~CCCallStack();
    void dump();
    CC_SYNTHESIZE_READONLY(unsigned int, mStackDepth, StackDepth);
    CC_SYNTHESIZE_READONLY(CallStack, mCallStack, CallStack);
protected:
    int mCurrentFrameIndex;
    static _Unwind_Reason_Code cbUnwindTrace(struct _Unwind_Context*, void*);
};

NS_CC_END

#endif /* defined(__GameCore__CCCallStack__) */
