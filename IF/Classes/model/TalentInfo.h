//
//  TalentInfo.h
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#ifndef __IF__TalentInfo__
#define __IF__TalentInfo__

#include "cocos2d.h"

class TalentInfo : public cocos2d::CCObject{
public:
    TalentInfo(cocos2d::CCDictionary* dict);
    
    TalentInfo() :
    talentId(0)
    , level(0)
    , maxLevel(0)
    , effectNum(0)
    , open("")
    , relation("")
    , posX(0)
    , posY(0)
    {};
    bool isLock();
    
    int talentId;
    int level;
    int maxLevel;
    int effectNum;
    std::string open;
    std::string relation;
    int posX;
    int posY;
    
    std::vector<int> effectValue;
};

#endif /* defined(__IF__TalentInfo__) */
