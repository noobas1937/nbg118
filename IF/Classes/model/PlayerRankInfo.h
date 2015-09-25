//
//  PlayerRankInfo.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__PlayerRankInfo__
#define __IF__PlayerRankInfo__

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;
class PlayerRankInfo:public CCNode{
public:
    static PlayerRankInfo* create();
    PlayerRankInfo():alliancename(""),abbr(""),uid(""),name(""),pic(""),power(0),armyKill(0),level(0),picVer(0){};
    ~PlayerRankInfo(){};
    void update(CCDictionary* dict,double time);
    
    std::string alliancename;
    std::string abbr;
    std::string uid;
    std::string name;
    std::string pic;
    unsigned long power;
    unsigned long armyKill;
    int level;
    int picVer;
    
};

#endif /* defined(__IF__PlayerRankInfo__) */
