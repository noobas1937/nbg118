//
//  UserPlatformInfo.h
//  IF
//
//  Created by liruiqi on 15/6/10.
//
//

#ifndef __IF__UserPlatformInfo__
#define __IF__UserPlatformInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define BIND_QQ "qq"
#define BIND_WEIXIN "weixin"
#define BIND_WEIBO "weibo"
#define BIND_VK "vk"

//不包括facebook，gamecenter，googleplay
class UserPlatformInfo{
public:
    UserPlatformInfo() :
    bindPf("")
    , bindId("")
    , bindName(""){};
    std::string bindPf;
    std::string bindId;
    void setBindName(std::string name);
    std::string getBindName();
    void parse(CCDictionary *dict);
private:
    std::string bindName;
};

#endif /* defined(__IF__UserPlatformInfo__) */
