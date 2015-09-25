//
//  MazeController.cpp
//  IF
//
//  Created by 李锐奇 on 14-6-24.
//
//

#include "MazeController.h"
#include "WorldController.h"
#include "TiledFourCell.h"

static MazeController *_instance = NULL;

MazeController *MazeController::getInstance(){
    if(_instance == NULL){
        _instance = new MazeController();
    }
    return _instance;
}

void MazeController::generateNewMaze(){
    int num = WorldController::getInstance()->m_monsterAttackInfo->count();
    int level = num / num_per_level + 1;
    if(level == mazeLevel){
        return;
    }
    mazeLevel = level;
    resetOpenIdx();
    allPoints.clear();
    resetMap();
    addTileFourLand();
}

void MazeController::resetOpenIdx(){
    int num = WorldController::getInstance()->m_monsterAttackInfo->count();
    openedIdx = num % num_per_level + 1;
}

int MazeController::getIndexByPos(const CCPoint &pt){
    return pt.x + pt.y * num_x + 1;
}

CCPoint MazeController::getPosByIndex(int index) {
    if (index == 0) {
        return ccp(-1, -1);
    }
    unsigned int z = index - 1;
    return ccp((int)(z%num_x), (int)(z/num_y));
}

CCPoint MazeController::getRealPosByIndex(int index){
    CCPoint pt = getPosByIndex(index);
    auto disPoint = ccpSub(pt, ccp(50, 50));
    return ccp(0 + (disPoint.x-disPoint.y) * _half_tile_w, 0 - (disPoint.x+disPoint.y) * _half_tile_h);
}

void MazeController::addTileFourLand(){
    map<int, int> allLakes;

    addTileFourCells(50, 50, 30, allLakes);
    
    auto setCell = [](int index, map<int, TiledFourCell*> &map, int cellPos){
        if(index < 0 || index > WorldController::getInstance()->_current_index_limit){
            return;
        }
        std::map<int, TiledFourCell*>::iterator it = map.find(index);
        TiledFourCell *cell;
        if(it == map.end()){
            cell = TiledFourCell::create();
            map[index] = cell;
        }else{
            cell = it->second;
        }
        if(cellPos == 1){
            cell->setiRightTop(1);
        }else if(cellPos == 2){
            cell->setiLeftTop(2);
        }else if(cellPos == 3){
            cell->setiLeftBottom(8);
        }else if(cellPos == 4){
            cell->setiRightBottom(4);
        }
    };
    
    map<int, TiledFourCell*> cellMap;
    map<int, int>::iterator it;
    for(it = allLakes.begin(); it != allLakes.end(); it++){
        int index1 = it->first;
        int index2 = index1 + 1;
        int index3 = index1 + num_x;
        int index4 = index3 + 1;
        setCell(index1, cellMap, 4);
        setCell(index2, cellMap, 3);
        setCell(index3, cellMap, 1);
        setCell(index4, cellMap, 2);
    }
    CCArray *arr = CCArray::create();
    CCArray *arr1 = CCArray::create();
    map<int, TiledFourCell*>::iterator cellIt;
    for(cellIt = cellMap.begin(); cellIt != cellMap.end(); cellIt++){
        int total = cellIt->second->getiTotalNum();
        if(total == 3){
            if(rand() % 2 == 0){
                total = 16;
            }
        }
        if(total == 5){
            if(rand() % 2 == 0){
                total = 17;
            }
        }
        if(total == 10){
            if(rand() % 2 == 0){
                total = 18;
            }
        }
        if(total == 12){
            if(rand() % 2 == 0){
                total = 19;
            }
        }
        
        int resIndex = total;
        m_map[cellIt->first] = resIndex;
        if(resIndex == 15){
            arr->addObject(CCInteger::create(cellIt->first));
        }else{
            arr1->addObject(CCInteger::create(cellIt->first));
        }
    }
    
    while(arr->count() > num_per_level){
        arr->removeObjectAtIndex(rand() % arr->count());
    }
    while(arr1->count() > num_per_level){
        arr1->removeObjectAtIndex(rand() % arr1->count());
    }
    
    int i = 0;
    while(i < arr->count()){
        int index = dynamic_cast<CCInteger*>(arr->objectAtIndex(i))->getValue();
        allPoints[i + 1] = index;
        i++;
    }
    while(i < num_per_level){
        int index = dynamic_cast<CCInteger*>(arr1->objectAtIndex(i))->getValue();
        allPoints[i + 1] = index;
        i++;
    }
    arr->removeAllObjects();
    cellMap.clear();
}

void MazeController::addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap){
    auto addEmptyIndex = [](int index, std::map<int, int> &checkMap, std::map<int, int> &checkMap1, CCArray *arr, const CCPoint &pt){
        std::map<int, int>::iterator it = checkMap.find(index);
        if(it != checkMap.end()){
            return;
        }
        it = checkMap1.find(index);
        if(it == checkMap1.end()){
            return;
        }
        
        arr->addObject(CCInteger::create(index));
    };
    
    int startIndex = getIndexByPos(ccp(startX, startY));
    
    map<int, int> tmpMap;
    int counter = 0;
    CCArray *arr = CCArray::create();//用于存储可以放置的点
    addEmptyIndex(startIndex, tmpMap, m_map, arr, ccp(-1, -1));
    
    while(counter < area && arr->count() > 0){
        //随机一个可以放置的点 放到已放置的点里
        int tmpTotal = arr->count();
        int tmpIndex = rand() % tmpTotal;
        int mapIndex = dynamic_cast<CCInteger*>(arr->objectAtIndex(tmpIndex))->getValue();
        tmpMap[mapIndex] = 1;
        arr->removeObjectAtIndex(tmpIndex);
        //寻找刚放置点的四周可以放置点的点
        int lp = mapIndex - 1;
        int rp = mapIndex + 1;
        int up = mapIndex - num_x;
        int dp = mapIndex + num_x;
        addEmptyIndex(lp, tmpMap, m_map, arr, getPosByIndex(lp));
        addEmptyIndex(rp, tmpMap, m_map, arr, getPosByIndex(rp));
        addEmptyIndex(up, tmpMap, m_map, arr, getPosByIndex(up));
        addEmptyIndex(dp, tmpMap, m_map, arr, getPosByIndex(dp));
        counter++;
    }
    
    map<int, int>::iterator idxIt;
    for(idxIt = tmpMap.begin(); idxIt != tmpMap.end(); idxIt++){
        indexMap[idxIt->first] = 1;
    }
    tmpMap.clear();
    arr->removeAllObjects();
}

void MazeController::resetMap(){
    int designWidthCol = 3;
    int designHeightRow = 9;
    int col = 50;
    int row = 50;
    int value1 = designWidthCol/2 + designHeightRow/2;
    int value2 = designHeightRow/2 - designWidthCol/2;
    for (int i=col-value1; i< col + value1; i++) {
        int startY = 0;
        int countY = 0;
        int thisRow = i - col + value1;
        if (thisRow < designWidthCol) {
            startY = row - value2 - 1 - thisRow;
            countY = (thisRow + 1)*2;
        } else if (thisRow < designHeightRow) {
            startY = row - value1 + thisRow - designWidthCol;
            countY = 2*designWidthCol;
        } else {
            startY = row - value1 + thisRow - designWidthCol;
            countY = (designWidthCol + designHeightRow - thisRow)*2;
        }
        for (int j=startY;j<startY+countY;j++) {
            CCLOG("%d, %d", i, j);
            m_map[getIndexByPos(ccp(i, j))] = 0;
        }
    }
}
