//
//  AllianceRankInfo.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__AllianceRankInfo__
#define __IF__AllianceRankInfo__

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;
class AllianceRankInfo:public CCNode{
public:
    static AllianceRankInfo* create();
    AllianceRankInfo():alliancename(""),uid(""),icon(""),fightpower(0),leader(""),nums(0),armyKill(0){};
    ~AllianceRankInfo(){};
    void update(CCDictionary* dict);
    std::string alliancename;
    std::string abbr;
    std::string uid;
    std::string icon;
    unsigned long fightpower;
    std::string leader;
    int nums;
    unsigned long armyKill;

    
};

#endif /* defined(__IF__AllianceRankInfo__) */
