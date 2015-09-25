//
//  CCCallStack.cpp
//  GameCore
//
//  Created by Chenfei on 13-11-4.
//
//

#include "CCCallStack.h"
#include "CCRegionMap.h"

#define LOG(format, ...)      CCLOG(format, ##__VA_ARGS__)

NS_CC_BEGIN

CCCallStack::CCCallStack(unsigned int stackDepth):mStackDepth(stackDepth),mCurrentFrameIndex(0)
{
    _Unwind_Backtrace(cbUnwindTrace, this);
};

CCCallStack::~CCCallStack()
{
    this->mCallStack.clear();
}

_Unwind_Reason_Code CCCallStack::cbUnwindTrace(struct _Unwind_Context* context, void* arg)
{
    CCCallStack* thiz = (CCCallStack*)arg;
    //    void *fn = _Unwind_FindEnclosingFunction((char* ) addr);
    
    CallStackFrame frame;
    frame.ip = _Unwind_GetIP(context);
    
    thiz->mCallStack.push_back(frame);
    
    if(thiz->mCallStack.size() > thiz->mStackDepth)
    {
        return _URC_END_OF_STACK;
    }
    return _URC_NO_REASON;
}

void CCCallStack::dump()
{
    CCRegionMap* regionMap = CCRegionMap::sharedRegionMap();
    CallStack::iterator iter = mCallStack.begin();
    CCLOG("Begin to dump stack trace:");
    int index = 0;
    while( iter != mCallStack.end() )
    {
        const RegionInfo* regionInfo = regionMap->getRegionInfoByAddr((*iter).ip);
        if(regionInfo)
        {
            CCLOG("    #%d:0x%lX\t%s", index++, (*iter).ip - regionInfo->start, (char*)regionInfo->name);
        }
        else
        {
            CCLOG("    #%d:0x%lX", index++, (*iter).ip);
        }
        iter++;
    }
    CCLOG("End to dump stack trace:");
}


NS_CC_END


