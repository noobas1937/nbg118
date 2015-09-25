//
//  AllianceGiftInfo.h
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#ifndef __IF__AllianceGiftInfo__
#define __IF__AllianceGiftInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;


class AllianceGiftInfo:public cocos2d::CCObject{
    
public:
    AllianceGiftInfo():id(string())
    ,icon(string())
    ,desc(string())
    ,name(string())
    ,giftId(string())
    ,gifttype(0)
    ,giftlevel(0)
    ,timeleft(0.0)
    ,valid(0)
    ,item(0)
    ,exp(0)
    ,para(0)
    {}
    
    void updateInfo(CCDictionary* dict);
    
    std::string id;
    std::string icon;
    std::string desc;
    std::string name;
    std::string giftId;
    int gifttype;
    int giftlevel;
    double timeleft;
    int valid;
    int item;
    int exp;
    int para;
};
#endif /* defined(__IF__AllianceGiftInfo__) */
