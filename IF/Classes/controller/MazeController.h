//
//  MazeController.h
//  IF
//
//  Created by 李锐奇 on 14-6-24.
//
//

#ifndef __IF__MazeController__
#define __IF__MazeController__

#include "CommonInclude.h"

static const int num_x = 100;
static const int num_y = 100;
static const int num_per_level = 5;
static const int _tile_w = 136;
static const int _tile_h = 68;
static const int _half_tile_w = _tile_w / 2;
static const int _half_tile_h = _tile_h / 2;

class MazeController{
public:
    static MazeController *getInstance();
    MazeController() : openedIdx(-1), mazeLevel(-1){};
    void generateNewMaze();
    void resetOpenIdx();
    std::map<int, int> m_map;
    std::map<int, int> allPoints;
    int openedIdx;
    int mazeLevel;
    void addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap);
    void addTileFourLand();
    void resetMap();
    int getIndexByPos(const CCPoint &pt);
    CCPoint getPosByIndex(int index);
    CCPoint getRealPosByIndex(int index);
};

#endif /* defined(__IF__MazeController__) */
