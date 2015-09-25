//
//  CCCrashDump.cpp
//  GameCore
//
//  Created by Chenfei on 13-11-21.
//
//

#include "CCCrashDump.h"

#define DUMP_MAX_LENGTH 16*1024

NS_CC_BEGIN

CCCrashDump* CCCrashDump::sCrashDumpInstance = NULL;

void CCCrashDump::clear()
{
    this->mContent.clear();
}

CCCrashDump* CCCrashDump::sharedDump()
{
    if(!sCrashDumpInstance)
    {
        sCrashDumpInstance = new CCCrashDump();
    }
    return sCrashDumpInstance;
}

void CCCrashDump::appendFormat(const char * pszFormat, ...)
{
    char szBuf[DUMP_MAX_LENGTH+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, DUMP_MAX_LENGTH, pszFormat, ap);
    va_end(ap);
    mContent.append(szBuf);
    mContent.append("\n");
}

NS_CC_END


