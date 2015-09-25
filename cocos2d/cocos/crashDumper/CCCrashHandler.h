//
//  CCCrashHandler.h
//  GameCore
//
//  Created by ChenFei on 13-10-29.
//
//

#ifndef __GameCore__CCCrashHandler__
#define __GameCore__CCCrashHandler__


#include <signal.h>
#include <sys/wait.h>
#include <unwind.h>

#include "cocos2d.h"

NS_CC_BEGIN

class CCCrashHandler
{
public:
    static CCCrashHandler* sharedHandler();
    void init();
    static void cbCrashHandler(int, siginfo_t *,void *);
    static const char* getSignalDesc(int signal);
    void raiseSignal();
protected:
    CCCrashHandler();
    virtual ~CCCrashHandler();
    
    void dumpProcInfo();
    void dumpLogcat();
    struct sigaction mSigaAction;
    struct sigaction mOldSigaAction;
    stack_t mSignalStack;
    static CCCrashHandler* sCrashHandler;
    
    CC_SYNTHESIZE_READONLY(pid_t, mPID, PID)
    CC_SYNTHESIZE_READONLY(int, mTID, TID)
    CC_SYNTHESIZE_READONLY(uintptr_t, mLibBase, LibBase);
};

NS_CC_END

#endif /* defined(__GameCore__CCCrashHandler__) */
