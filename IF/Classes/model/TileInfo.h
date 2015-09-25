//
//  TileInfo.h
//  IF
//
//  Created by fubin on 14-5-12.
//
//

#ifndef IF_TileInfo_h
#define IF_TileInfo_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class TileInfo : public cocos2d::CCObject {
public:
    TileInfo(CCDictionary* dict);
    TileInfo():tileId(0)
    ,unlock(0)
    ,state(1)
    ,xmlOpen(0){};
    
    int tileId;
    int unlock;
    int state;
    int xmlOpen;
    
    void SetInfoFromXml(CCDictionary* dict);
    
protected:
    
};

class BigTileInfo : public cocos2d::CCObject {
public:
    BigTileInfo(CCDictionary* dict);
    BigTileInfo():tileId(0)
    ,unlock(0)
    ,silver_need(0)
    ,wood_need(0)
    ,stone_need(0)
    ,food_need(0)
    ,iron_need(0)
    ,level(0)
    ,positions("")
    ,state(1)
    ,pos(0){};
    
    int tileId;
    int unlock;
    int silver_need;
    int wood_need;
    int stone_need;
    int food_need;
    int iron_need;
    int level;
    string positions;
    int state;
    int pos;
    
    void SetInfoFromXml(CCDictionary* dict);
    
protected:
    
};

#endif
