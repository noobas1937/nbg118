//
//  NBWorldMonster.hpp
//  IF
//
//  Created by gj on 10/15/15.
//
//

#ifndef NBWorldMonster_hpp
#define NBWorldMonster_hpp

#include "cocos2d.h"
#include "WorldMapView.h"

USING_NS_CC;

class NBWorldMonster {
public:
    static void addFieldMonsterUnderNode(const WorldCityInfo& info, const Vec2& pos, unsigned int index);
    static void addActBossTileUnderNode(const WorldCityInfo& info, const Vec2& pos, unsigned int index);
    static void createMonsterBatchItem(BatchTagType type, unsigned int index);
};

#endif /* NBWorldMonster_hpp */
