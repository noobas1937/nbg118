//
//  CCArmSingalCallStackWalker.h
//  GameCore
//
//  Created by Chenfei on 13-11-5.
//
//

#ifndef __GameCore__CCArmSingalCallStackWalker__
#define __GameCore__CCArmSingalCallStackWalker__


#include "cocos2d.h"
#include <signal.h>
#include <sys/wait.h>
#include <unwind.h>

NS_CC_BEGIN

typedef struct _unwind_state_t
{
    uint32_t gregs[16];
} unwind_state_t;

class CCArmSingalCallStackWalker
{
public:
    CCArmSingalCallStackWalker(uintptr_t usercontext);
    void dump();
    uintptr_t getPC();
    uintptr_t getLR();
protected:
    unwind_state_t mUnwindState;
    uintptr_t mUContext;
};

NS_CC_END

#endif /* defined(__GameCore__CCArmSingalCallStackWalker__) */
