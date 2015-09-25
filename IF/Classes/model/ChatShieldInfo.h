//
//  ChatShieldInfo.h
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#ifndef __IF__ChatShieldInfo__
#define __IF__ChatShieldInfo__
#include "cocos2d.h"

class ShieldInfo : public cocos2d::CCObject{
public:
    ShieldInfo():
    name(""),
    uuid(""),
    uid(""),
    power(0),
    pic(""),
    rank(0),
    abbr(""),
    picVer(0)
    {};
    std::string name;
    std::string uuid;
    std::string uid;
    int power;
    std::string pic;
    std::string abbr;
    int rank;
    int picVer;
};

class ChatShieldInfo{
public:
    ChatShieldInfo() : isGetInfo(false), shieldList(cocos2d::CCArray::create()), maxNum(0){
        shieldList->retain();
        CCLOG("%s", "1");
    };
    void parse(cocos2d::CCArray* arr);
    bool isGetInfo;
    cocos2d::CCArray* shieldList;
    int maxNum;
    bool isShield(std::string uid);
    std::string getUuid(std::string uid);
    ShieldInfo* getShieldInfo(std::string uuid);
    void removeFromList(std::string uid);
    void addToList(cocos2d::CCDictionary *dict);
    bool isReachLimit();
};



#endif /* defined(__IF__ChatShieldInfo__) */
