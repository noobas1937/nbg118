//
//  NBWorldNPC.hpp
//  IF
//
//  Created by gj on 10/16/15.
//
//

#ifndef NBWorldNPC_hpp
#define NBWorldNPC_hpp

#include "cocos2d.h"

USING_NS_CC;

class NBWorldNPC
{
public:
    static RepeatForever* createAnimation(const char * plist_filename, const char* animation_filename, int animation_start_id, int animation_end_id);
    static void updateOctopus(const Vec2& currentTilePoint, Node* animation_layer);
    static void updateBirds(const Vec2& currentTilePoint, Node* animation_layer);
};

#endif /* NBWorldNPC_hpp */
