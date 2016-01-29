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

#define NB_WORLD_MONSTER_START_TAG (1000000 * 100)
#define NB_WORLD_MONSTER_ANIM_TAG 10010

class NBWorldUtils
{
public:
    static std::string getMonsterBustImage(std::string monsterId);
    static cocos2d::Node* createSeaMonsterAndWaitingAnimation(std::string monsterId);
    static cocos2d::Node* createSeaMonsterAndAttackAnimation(std::string monsterId);
};

#endif /* NBWorldUtils_hpp */
