//
//  CCArmSingalCallStackWalker.cpp
//  GameCore
//
//  Created by Chenfei on 13-11-5.
//
//

#include "CCArmSingalCallStackWalker.h"
#include "CCRegionMap.h"
#include "CCCrashDump.h"


// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

// typedef struct ucontext
// {
//     unsigned long     uc_flags;
//     struct ucontext  *uc_link;
//     stack_t           uc_stack;
    
//     struct sigcontext uc_mcontext;
//     sigset_t          uc_sigmask;
    /* Allow for uc_sigmask growth.  Glibc uses a 1024-bit sigset_t.  */
//     int               _sigbuffer[32 - (sizeof (sigset_t) / sizeof (int))];
    /* Last for extensibility.  Eight byte aligned because some
     coprocessors require eight byte alignment.  */
//     unsigned long     uc_regspace[128] __attribute__((__aligned__(8)));
// } ucontext_t;

// #endif

#define ARM_FP mUnwindState.gregs[11]
#define ARM_IP mUnwindState.gregs[12]
#define ARM_SP mUnwindState.gregs[13]
#define ARM_LR mUnwindState.gregs[14]
#define ARM_PC mUnwindState.gregs[15]


#define ADDR_CONTENT_AS_ADDR(addr) (*((unsigned long*)(addr)))

NS_CC_BEGIN

CCArmSingalCallStackWalker::CCArmSingalCallStackWalker(uintptr_t usercontext)
{
    this->mUContext = usercontext;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    if(!mUContext)
    {
        return;
    }
    ucontext* uc = (ucontext*)mUContext;
    mUnwindState.gregs[0] = uc->uc_mcontext.arm_r0;
    mUnwindState.gregs[1] = uc->uc_mcontext.arm_r1;
    mUnwindState.gregs[2] = uc->uc_mcontext.arm_r2;
    mUnwindState.gregs[3] = uc->uc_mcontext.arm_r3;
    mUnwindState.gregs[4] = uc->uc_mcontext.arm_r4;
    mUnwindState.gregs[5] = uc->uc_mcontext.arm_r5;
    mUnwindState.gregs[6] = uc->uc_mcontext.arm_r6;
    mUnwindState.gregs[7] = uc->uc_mcontext.arm_r7;
    mUnwindState.gregs[8] = uc->uc_mcontext.arm_r8;
    mUnwindState.gregs[9] = uc->uc_mcontext.arm_r9;
    mUnwindState.gregs[10] = uc->uc_mcontext.arm_r10;
    mUnwindState.gregs[11] = uc->uc_mcontext.arm_fp;
    mUnwindState.gregs[12] = uc->uc_mcontext.arm_ip;
    mUnwindState.gregs[13] = uc->uc_mcontext.arm_sp;
    mUnwindState.gregs[14] = uc->uc_mcontext.arm_lr;
    mUnwindState.gregs[15] = uc->uc_mcontext.arm_pc;
#endif
}

uintptr_t CCArmSingalCallStackWalker::getPC()
{
    return ARM_PC;
}

uintptr_t CCArmSingalCallStackWalker::getLR()
{
    return ARM_LR;
}

void CCArmSingalCallStackWalker::dump()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    ucontext* uc = (ucontext*)mUContext;
    if(!mUContext)
    {
        return;
    }
    CCRegionMap* regionMap = CCRegionMap::sharedRegionMap();
    
    DUMP("    arm_r0:0x%08lX    arm_r1:0x%08lX    arm_r2:0x%08lX    arm_r3:0x%08lX", uc->uc_mcontext.arm_r0, uc->uc_mcontext.arm_r1, uc->uc_mcontext.arm_r2, uc->uc_mcontext.arm_r3);
    DUMP("    arm_r4:0x%08lX    arm_r5:0x%08lX    arm_r6:0x%08lX    arm_r7:0x%08lX", uc->uc_mcontext.arm_r4, uc->uc_mcontext.arm_r5, uc->uc_mcontext.arm_r6, uc->uc_mcontext.arm_r7);
    
    DUMP("    arm_r8:0x%08lX    arm_r9:0x%08lX    arm_r10:0x%08lX", uc->uc_mcontext.arm_r8, uc->uc_mcontext.arm_r9, uc->uc_mcontext.arm_r10);
    
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr(ARM_FP);
        if(regionInfo)
        {
            DUMP("    arm_fp:0x%08lX               %s", ARM_FP, regionInfo->name);
        }
        else
        {
            DUMP("    arm_fp:0x%08lX", ARM_FP);
        }
    }
    
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr(ARM_IP);
        if(regionInfo)
        {
            DUMP("    arm_ip:0x%08lX               %s", ARM_IP - regionInfo->start, regionInfo->name);
        }
        else
        {
            DUMP("    arm_ip:0x%08lX", ARM_IP);
        }
    }
    
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr(ARM_SP);
        if(regionInfo)
        {
            DUMP("    arm_sp:0x%08lX               %s", ARM_SP, regionInfo->name);
        }
        else
        {
            DUMP("    arm_sp:0x%08lX", ARM_SP);
        }
    }
    
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr(ARM_LR);
        if(regionInfo)
        {
            DUMP("    arm_lr:0x%08lX               %s", ARM_LR - regionInfo->start, regionInfo->name);
        }
        else
        {
            DUMP("    arm_lr:0x%08lX", ARM_LR);
        }
    }
    
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr(ARM_PC);
        if(regionInfo)
        {
            DUMP("    arm_pc:0x%08lX               %s", ARM_PC - regionInfo->start, regionInfo->name);
        }
        else
        {
            DUMP("    arm_pc:0x%08lX", ARM_PC);
        }
    }
    
    int index = 0;
    return;
#endif
}

NS_CC_END