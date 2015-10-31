//
//  NBWorldMapMainCity.cpp
//  IF
//
//  Created by gj on 10/22/15.
//
//

#include "NBWorldMapMainCity.hpp"
#include "WorldController.h"

static const char* ISLANDS_0[] = {"island_001.png", "island_002.png", "island_003.png", "island_004.png",};
static const char* ISLANDS_1[] = {"island_005.png", "island_006.png", "island_007.png", "island_008.png",};
static const char* ISLANDS_2[] = {"island_009.png", "island_010.png", "island_011.png", "island_012.png",};
static const char* ISLANDS_3[] = {"island_013.png", "island_014.png", "island_015.png", "island_016.png",};

int NBWorldMapMainCity::getMainCityIslandImageIndex(const WorldCityInfo* info, int level, int nSpecialId)
{
    int addIndex = 0;
    if (info->cityIndex == info->parentCityIndex - 1)
    {
        addIndex = 1;
    }
    else if(info->cityIndex == info->parentCityIndex - 1 - WorldController::getInstance()->_current_tile_count_x)
    {
        addIndex = 2;
    }
    else if(info->cityIndex == info->parentCityIndex - WorldController::getInstance()->_current_tile_count_x)
    {
        addIndex = 3;
    }
    
    bool isKing = false;
    auto playerInfo = WorldController::getInstance()->m_playerInfo.find(info->playerName);
    if (playerInfo != WorldController::getInstance()->m_playerInfo.end() && playerInfo->second.officer == KINGDOM_KING_ID)
    {
        isKing = true;
    }
    
    int startIndex = getMainCityId(level, isKing, nSpecialId);
    if (startIndex > -1 && startIndex < 100) // no worldcastle.xls/addPic
    {
        return addIndex;
    }
    return -1;
}

const char * NBWorldMapMainCity::getMainCityIslandImage(int island_index, int x, int y)
{
    // 根据坐标和城市开始索引计算岛的外观，不用存储数据到服务器
    int random_variable = (x + y) % 4;
    
    const char** ISLANDS = ISLANDS_0;
    if (random_variable == 1) ISLANDS = ISLANDS_1;
    else if (random_variable == 2) ISLANDS = ISLANDS_2;
    else if (random_variable == 3) ISLANDS = ISLANDS_3;

    return ISLANDS[island_index];
}

Node * NBWorldMapMainCity::getMainCity(int island_index, int level, int nSpecialId)
{
    Vec2 house_pos(0, 0);
    string picStr = "tile_place_holder.png";
    if (island_index == 0)
    {
        picStr = "lv1.png";
        // 美术给出的坐标为 x：-111，y：-51
        house_pos.x = 256 / 2 - 111;
        house_pos.y = 81;
    }
    
    auto house = CCLoadSprite::createSprite(picStr.c_str());
    house->setAnchorPoint(Vec2(0, 0));
    house->setPosition(house_pos);
    if (island_index != 0)
    {
        house->setOpacity(0);
    }
    return house;
}

int NBWorldMapMainCity::getMainCityId(int level, bool isKing, int nSpecialId)
{
    int id = 44100 - 1 + level;
    if (isKing)
    {
        id = 44999;
    }
    if (nSpecialId != -1)
    {
        id = nSpecialId;
    }
    int startIndex = -1;
    int mainCityIndex = atoi(CCCommonUtils::getPropById(CC_ITOA(id), "Basics").c_str()); // [0, 6]
    int startBaseIndex = 41;
    int defaultStartBaseIndex = 41;
    int baseNumPerCity = 4;
    if (mainCityIndex != -1)
    {
        startIndex = startBaseIndex + mainCityIndex * baseNumPerCity;
    }
    return startIndex;
}
