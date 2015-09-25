//
//  PlatformUtil.h
//  IF
//
//  Created by chenliang on 14/12/1.
//
//
#ifndef IF_PlatformUtil_h
#define IF_PlatformUtil_h

#include <string.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
//#include "json.h"

namespace PlatformUtilies{
    //第三方登录：Google+,GameCenter相关,0:手动 1：自动连接
    void login_Platform(int loginType);
    void goto_IOSGameCenter();
    void logout_Platform(int platform);
    bool isAvailable_Platform(int platform);
    bool isConnected_Platform(int platform);
    cocos2d::CCDictionary* getLoginedInfo_Platform(int platform);
    
    void loginPlatform(std::string pf);
    void logoutPlatform(std::string pf);
};


#endif