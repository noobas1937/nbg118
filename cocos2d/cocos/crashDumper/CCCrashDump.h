//
//  CCCrashDump.h
//  GameCore
//
//  Created by Chenfei on 13-11-21.
//
//

#ifndef __GameCore__CCCrashDump__
#define __GameCore__CCCrashDump__

#include "cocos2d.h"

#define DUMP(format, ...) CCCrashDump::sharedDump()->appendFormat(format, ##__VA_ARGS__);

NS_CC_BEGIN

class CCCrashDump
{
public:
    virtual ~CCCrashDump() {}
    static CCCrashDump* sharedDump();
    void appendFormat(const char * pszFormat, ...);
    void clear();
    const std::string& getDumpContent()
    {
        return mContent;
    }
protected:
    static CCCrashDump* sCrashDumpInstance;
    std::string mContent;
    CCCrashDump(){}
};

NS_CC_END

#endif /* defined(__GameCore__CCCrashDump__) */
