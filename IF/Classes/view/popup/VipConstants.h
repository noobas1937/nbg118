//
//  VipConstants.h
//  IF
//
//  Created by chenliang on 14-3-19.
//
//

#ifndef IF_VipConstants_h
#define IF_VipConstants_h

#include <stdlib.h>
#include <string>

#define KEY_VIP_POINTS_REQUIRED "point"
#define KEY_SPEED_UP "speedup"
#define KEY_FOOD "food"
#define KEY_WOOD "wood"
#define KEY_STONE "stone"
#define KEY_IRON "iron"
#define KEY_SILVER "silver"
#define KEY_MARCH "march"
#define KEY_ATTACK "attack"
#define KEY_DEFENSE "defense"
#define KEY_HEALTH "health"
#define KEY_AFFAIRS "affairs"
#define KEY_QUEST "alquest"
#define KEY_QUEST_FINISH "alquest_finish"


#define TEXT_VIP_POINTS_REQUIRED "PLACEHOLDER"
#define TEXT_SPEED_UP "103021"
#define TEXT_FOOD "103022"
#define TEXT_WOOD "103023"
#define TEXT_STONE "103024"
#define TEXT_IRON "103025"
#define TEXT_SILVER "103026"
#define TEXT_MARCH "103027"
#define TEXT_ATTACK "103028"
#define TEXT_DEFENSE "103030"
#define TEXT_HEALTH "103029"
#define TEXT_AFFAIRS "103031"
#define TEXT_QUEST "103032"
#define TEXT_QUEST_FINISH "103033"

class VipConstants
{
public:
    static const std::string ATTRIBUTES[];
    static const std::string ATTRIBUTES_TEXT_IDS[];
    
    static const int VIP_DATA_ID_OFFSET;
};

#endif
