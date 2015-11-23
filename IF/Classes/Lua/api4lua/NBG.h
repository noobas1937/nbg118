//
//  NBG.h
//  nbg
//
//  Created by gj on 9/7/15.
//
//

#ifndef __nbg__NBG__
#define __nbg__NBG__

#include <stdio.h>
#include <string>

#include "cocos2d.h"

#define KEYCHAIN_ACCOUNT_OPENUDID    "nbg_keychain_account_openudid"
#define KEYCHAIN_ACCOUNT_SERVICENAME "nbg_keychain_account_servicename"

#define ACCOUNT_OPENUDID "ACCOUNT_OPENUDID"

class NBG
{
public:
    static std::string getUDID();
    
    static std::string getOpenUDID(bool autosave = true);
    
    static std::string getCountryCode();
    static std::string getPublishRegion();
    static std::string getPlatformUID();
    static std::string getPlatformToken();
    
    static std::string getHandSetInfo();
    static std::string getVersionName();
    static std::string getVersionCode();
    
    static std::string md5(const char* in, std::string& out);
    
    static std::string getLanguage();
    
    static long long getCurrentMillisecond();   // 获取当前毫秒
    static long getCurrentSecond(); // 获取当前秒

    static int get_COCOS2D_DEBUG();
};

#endif /* defined(__nbg__NBG__) */
