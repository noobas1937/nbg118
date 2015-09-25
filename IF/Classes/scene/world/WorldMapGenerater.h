//
//  WorldMapGenerater.h
//  IF
//
//  Created by 李锐奇 on 14-6-16.
//
//

#ifndef __IF__WorldMapGenerater__
#define __IF__WorldMapGenerater__

#include "CommonInclude.h"

#define StartWaterPicIndex 5
#define StartLandPicIndex 24
#define StartHillsPicIndex 43
#define StartRelicIndex 62
#define RelicWhite 81
#define RockPicIndex 1
#define DecorationPicIndex 27
#define WaterDecorationIndex 39
#define BlockSize 60

class WorldMapGenerater{
public:
    static WorldMapGenerater *create();
private:
    bool init();
    
    void generateMap();
    void initMap();
    void addRelic();
    void addRelicToMap(int centerX, int centerY, int xNum, int yNum, map<int, int> &map);
    void addTileFourLand(int type, CCArray *arr);//0.water 1.land 2.hills 3.relic
    void addMoutains();
    void addLandDecoration();
    void addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap, int type);
    
    void outputFile();
    map<int, int> m_worldMap;
    map<int, int> m_worldMap1;
    
    void intMapByFile();
    void addLinkArea();
    void outPut();
    
    void removeAll1201();
};
#endif /* defined(__IF__WorldMapGenerater__) */
