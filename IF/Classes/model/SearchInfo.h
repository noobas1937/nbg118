//
//  PlayerSearchInfo.h
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#ifndef __IF__PlayerSearchInfo__
#define __IF__PlayerSearchInfo__

#include "CommonInclude.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PlayerSearchInfo:
public cocos2d::CCObject
{
public:
    PlayerSearchInfo(CCDictionary* dict);
    PlayerSearchInfo():
    lang("")
    , name("")
    , pic("")
    , power(0)
    , uid("")
    , alliancename("")
    , level(0)
    , serverId(-1)
    , abbr("")
    {};
    
    string lang;
    string name;
    string pic;
    int power;
    string uid;
    string alliancename;
    int level;
    int serverId;
    string abbr;
};


#endif /* defined(__IF__PlayerSearchInfo__) */
