//
//  NBWorldUtils.hpp
//  DragonClans
//
//  Created by gj on 12/23/15.
//
//

#ifndef NBWorldUtils_hpp
#define NBWorldUtils_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <string>

class NBWorldUtils
{
public:
    static std::string getMonsterBustImage(std::string monsterId);
    static cocos2d::Sprite* createSeaMonsterAndWaitingAnimation(std::string monsterId);
    static cocos2d::Sprite* createSeaMonsterAndAttackAnimation(std::string monsterId);
};

#endif /* NBWorldUtils_hpp */
