//
//  NBWorldMapMainCity.hpp
//  IF
//
//  Created by gj on 10/22/15.
//
//

#ifndef NBWorldMapMainCity_hpp
#define NBWorldMapMainCity_hpp

#include "cocos2d.h"
USING_NS_CC;

class WorldCityInfo;

// worldcastle.xls
class NBWorldMapMainCity
{
public:
    static int getMainCityIslandImageIndex(const WorldCityInfo* info, int level, int nSpecialId);
    static Node * getMainCityIslandImage(int island_index, int x, int y);
    static Node * getMainCity(int island_index, int level, bool isKing, int nSpecialId);
    static bool isKing(const WorldCityInfo &info);
    static int getMainCityId(int level, bool isKing, int nSpecialId);
    
protected:
    static int getMainCityStartIndex(int level, bool isKing, int nSpecialId);
};

#endif /* NBWorldMapMainCity_hpp */
