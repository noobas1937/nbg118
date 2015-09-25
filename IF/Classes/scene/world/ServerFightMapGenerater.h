//
//  ServerFightMapGenerater.h
//  IF
//
//  Created by liruiqi on 15/6/5.
//
//

#ifndef __IF__ServerFightMapGenerater__
#define __IF__ServerFightMapGenerater__

#include "CommonInclude.h"

#define StartPicIndex 5
#define StartDecoration 24
#define DecorationNum 18
#define BlankCellIndex 42
#define FightBlockSize 60

class ServerFightMapGenerater{
public:
    static ServerFightMapGenerater *create();
private:
    bool init();
    void generateMap();
    void initMap();
    void addTileFourLand(int type, CCArray *arr);
    void addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap, int type);
    void getMapConfig();
    void outputFile();
    
    CCPoint getPointByIndex(unsigned int index);
    unsigned int getIndexByPoint(const cocos2d::CCPoint &point);

    map<int, int> m_worldMap;
    map<int, int> m_worldMap1;
    int mapW;
    int mapH;
    int totalCell;
};

#endif /* defined(__IF__ServerFightMapGenerater__) */
