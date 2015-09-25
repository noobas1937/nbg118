//
//  WorldMapGenerater.cpp
//  IF
//
//  Created by 李锐奇 on 14-6-16.
//
//

#include "WorldMapGenerater.h"
#include "WorldController.h"
#include "TiledFourCell.h"
#include "tinyxml2/tinyxml2.h"
static WorldMapGenerater *_instance = NULL;

WorldMapGenerater *WorldMapGenerater::create(){
    if(_instance == NULL){
        _instance = new WorldMapGenerater;
        if(_instance && _instance->init()){
            
        }else{
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

bool WorldMapGenerater::init(){
    generateMap();
    return true;
}

void WorldMapGenerater::generateMap(){
    //修改地图
    removeAll1201();
//    intMapByFile();
//    addLinkArea();
//    outPut();
    //以下为初建地图
//    initMap();
//    addRelic();
//    addTileFourLand(0, CCArray::create());
//    addTileFourLand(1, CCArray::create());
//    addTileFourLand(2, CCArray::create());
//    addMoutains();
//    addLandDecoration();
//    outputFile();
}

void WorldMapGenerater::addTileFourLand(int type, CCArray *arr){
    map<int, int> allLakes;
    if(type == 3){
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            int index = dynamic_cast<CCInteger*>(obj)->getValue();
            allLakes[index] = 1;
        }
    }else{
        int startX = 1;
        int gapX = BlockSize;
        int startY = 1;
        int gapY = BlockSize;
        int numPerArea = 0;
        while(startY < _tile_count_y){
            startX = 1;
            while(startX < _tile_count_x){
                numPerArea = 5 + rand() % 6;
                int count = 0;
                while(count < numPerArea){
                    int x = startX + rand() % gapX;
                    int y = startY + rand() % gapY;
                    int num = 3 + rand() % 47;
                    addTileFourCells(x, y, num, allLakes, type);
                    count++;
                }
                startX += gapX;
            }
            startY += gapY;
        }
    }
    auto setCell = [](int index, map<int, TiledFourCell*> &map, int cellPos){
        if(index < 0 || index > _indexLimit){
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
        int index3 = index1 + _tile_count_x;
        int index4 = index3 + 1;
        setCell(index1, cellMap, 4);
        setCell(index2, cellMap, 3);
        setCell(index3, cellMap, 1);
        setCell(index4, cellMap, 2);
    }
    
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
        int start = 0;
        if(type == 0){
            start = StartWaterPicIndex;
            if(total == 15){
                if(rand() % 20 == 0){
                    m_worldMap1[cellIt->first] = WaterDecorationIndex + 98;
                }
            }
        }else if(type == 1){
            start = StartLandPicIndex;
        }else if(type == 2){
            start = StartHillsPicIndex;
        }else if(type == 3){
            start = StartRelicIndex;
        }
        
        int resIndex = total + start - 1;
        m_worldMap[cellIt->first] = resIndex;
    }
    
    cellMap.clear();
}

void WorldMapGenerater::addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap, int type){
    auto addEmptyIndex = [](int index, std::map<int, int> &checkMap1, std::map<int, int> &checkMap2, std::map<int, int> &checkMap3, CCArray *arr){
        if(index < 0 || index > _indexLimit){
            return;
        }
        
        auto pt = WorldController::getPointByIndex(index);
        if(pt.x == 0 || pt.x == _tile_count_x - 1 || pt.y == 0 || pt.y == _tile_count_y - 1){
            return ;
        }

        map<int, int>::iterator it2 = checkMap2.find(index);
        if(it2 == checkMap2.end() || it2->second >= StartWaterPicIndex){
            return;
        }
        
        map<int, int>::iterator it3 = checkMap3.find(index);
        if(it3 != checkMap3.end()){
            return;
        }
        auto check = [](int index, std::map<int, int> &checkMap1, std::map<int, int> &checkMap2){
            map<int, int>::iterator it1 = checkMap1.find(index);
            map<int, int>::iterator it2 = checkMap2.find(index);

            if(it1 != checkMap1.end() || it2->second >= StartWaterPicIndex){
                return true;
            }
            return false;
        };
        
        if(check(index, checkMap1, checkMap2)
           || check(index + 1, checkMap1, checkMap2)
           || check(index - 1, checkMap1, checkMap2)
           || check(index - _tile_count_x, checkMap1, checkMap2)
           || check(index - _tile_count_x + 1, checkMap1, checkMap2)
           || check(index - _tile_count_x - 1, checkMap1, checkMap2)
           || check(index + _tile_count_x, checkMap1, checkMap2)
           || check(index + _tile_count_x + 1, checkMap1, checkMap2)
           || check(index + _tile_count_x - 1, checkMap1, checkMap2)
           ){
            return;
        }
        
        arr->addObject(CCInteger::create(index));
    };
    
    int startIndex = WorldController::getIndexByPoint(ccp(startX, startY));
    map<int, int>::iterator it = m_worldMap.find(startIndex);
    if(it->second >= StartWaterPicIndex){
        return;
    }

    if(startIndex < 0 || startIndex > _indexLimit){
        return;
    }
    map<int, int> tmpMap;
    int counter = 0;
    CCArray *arr = CCArray::create();//用于存储可以放置的点
    addEmptyIndex(startIndex, indexMap, m_worldMap, tmpMap, arr);
    
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
        int up = mapIndex - _tile_count_x;
        int dp = mapIndex + _tile_count_x;
        addEmptyIndex(lp, indexMap, m_worldMap, tmpMap, arr);
        addEmptyIndex(rp, indexMap, m_worldMap, tmpMap, arr);
        addEmptyIndex(up, indexMap, m_worldMap, tmpMap, arr);
        addEmptyIndex(dp, indexMap, m_worldMap, tmpMap, arr);
        counter++;
    }
    
    map<int, int>::iterator idxIt;
    for(idxIt = tmpMap.begin(); idxIt != tmpMap.end(); idxIt++){
        indexMap[idxIt->first] = 1;
    }
    tmpMap.clear();
    arr->removeAllObjects();
}

void WorldMapGenerater::addMoutains(){
    auto addMoutainCell = [](int startX, int startY, std::map<int, int> &map1, std::map<int, int> &map2){
        int index = WorldController::getIndexByPoint(ccp(startX, startY));
        if(startX < 1 || startX >= _tile_count_x - 1 || startY < 1 || startY >= _tile_count_y - 1){
            return;
        }
        if(map2[index] != 0
           || map2[index - 1] != 0
           || map2[index - _tile_count_x] != 0
           || map2[index - _tile_count_x - 1] != 0
           ){
            return;
        }
        if((map1[index] >= StartLandPicIndex && map1[index] < StartRelicIndex)
           || map1[index] < StartWaterPicIndex
           || (map1[index - 1] >= StartLandPicIndex && map1[index - 1] < StartRelicIndex)
           || map1[index - 1] < StartWaterPicIndex
           || (map1[index - _tile_count_x] >= StartLandPicIndex && map1[index - _tile_count_x] < StartRelicIndex)
           || map1[index - _tile_count_x] < StartWaterPicIndex
           || (map1[index - _tile_count_x - 1] >= StartLandPicIndex && map1[index - _tile_count_x - 1] < StartRelicIndex)
           || map1[index - _tile_count_x - 1] < StartWaterPicIndex
           ){
            
        }else{
            return;
        }
        
        int picIndex = rand() % 3 * 4 + 98;
        map2[index] = picIndex + 1;
        map2[index - 1] = picIndex + 2;
        map2[index - _tile_count_x] = picIndex + 4;
        map2[index - _tile_count_x - 1] = picIndex + 3;
    };
    
    int startX = 1;
    int gapX = BlockSize;
    int startY = 1;
    int gapY = BlockSize;
    int numPerArea = 0;
    while(startY < _tile_count_y){
        startX = 1;
        while(startX < _tile_count_x){
            numPerArea = 5 + rand() % 6;
            int count = 0;
            while(count < numPerArea){
                int x = startX + rand() % gapX;
                int y = startY + rand() % gapY;
                addMoutainCell(x, y, m_worldMap, m_worldMap1);
                if(rand() % 2 == 0){
                    addMoutainCell(x + 2, y, m_worldMap, m_worldMap1);
                }
                if(rand() % 2 == 0){
                    addMoutainCell(x + 2, y + 2, m_worldMap, m_worldMap1);
                }
                if(rand() % 2 == 0){
                    addMoutainCell(x, y + 2, m_worldMap, m_worldMap1);
                }

                count++;
            }
            startX += gapX;
        }
        startY += gapY;
    }

}

void WorldMapGenerater::addRelicToMap(int centerX, int centerY, int xNum, int yNum, map<int, int> &map){
    int countX = 0;
    int countY = 0;
    CCArray *arr = CCArray::create();
    CCArray *arr1 = CCArray::create();
    while(countX < xNum){
        countY = 0;
        while(countY < yNum){
            int x = centerX + countX - int(xNum / 2);
            int y = centerY + countY - int(yNum / 2);
            int index = WorldController::getIndexByPoint(ccp(x, y));
            int dx = abs(centerX - x);
            int dy = abs(centerY - y);
            if(dx <= 3 && dy <= 3){
                arr1->addObject(CCInteger::create(index));
            }
            arr->addObject(CCInteger::create(index));

            countY++;
        }
        countX++;
    }
    addTileFourLand(3, arr);
    
    int idx = 0;
    while(idx < arr1->count()){
        int index = dynamic_cast<CCInteger*>(arr1->objectAtIndex(idx))->getValue();
        m_worldMap[index] = RelicWhite;
        idx++;
    }
    arr->removeAllObjects();
    arr1->removeAllObjects();
}

void WorldMapGenerater::addRelic(){
    auto &worldConfig = GlobalData::shared()->worldConfig;
    
    int numPerLine = worldConfig.relicConfig[1];
    int bigW = worldConfig.relicConfig[2];
    int bigH = worldConfig.relicConfig[2];
    int smallW = worldConfig.relicConfig[3];
    int smallH = worldConfig.relicConfig[3];
    int gapX = (_tile_count_x - bigW) / (numPerLine * 2);
    int gapY = (_tile_count_y - bigH) / (numPerLine * 2);

    int centerX = 0;
    int centerY = 0;
    int counter = 0;
    //center
    centerX = _tile_count_x / 2;
    centerY = _tile_count_y / 2;
    addRelicToMap(centerX, centerY, 52, 52, m_worldMap);
}

void WorldMapGenerater::addLandDecoration(){
    auto addDeco = [](int startX, int startY, std::map<int, int> &map1, std::map<int, int> &map2){
        int index = WorldController::getIndexByPoint(ccp(startX, startY));
        if(startX < 0 || startX > _tile_count_x - 1 || startY < 0 || startY > _tile_count_y - 1){
            return;
        }
        if(map2[index] != 0){
            return;
        }
        if((map1[index] >= StartLandPicIndex && map1[index] < StartRelicIndex) || map1[index] < StartWaterPicIndex){
            
        }else{
            return;
        }
        int r = rand() % 4 < 3 ? 0 : 1;
        int picIndex = rand() % (5 + 7 * r) + 98;
        map2[index] = picIndex + 27;
    };
    
    int startX = 1;
    int gapX = BlockSize;
    int startY = 1;
    int gapY = BlockSize;
    int numPerArea = 0;
    while(startY < _tile_count_y){
        startX = 1;
        while(startX < _tile_count_x){
            numPerArea = 450 + rand() % 300;
            int count = 0;
            while(count < numPerArea){
                int x = startX + rand() % gapX;
                int y = startY + rand() % gapY;
                addDeco(x, y, m_worldMap, m_worldMap1);
                count++;
            }
            startX += gapX;
        }
        startY += gapY;
    }
}

void WorldMapGenerater::initMap(){
    int i = 1;
    while(i <= _indexLimit){
        m_worldMap[i] = rand() % 4 + 1;
        m_worldMap1[i] = 0;
        i++;
    }
}

void WorldMapGenerater::outputFile(){
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    pDoc->LoadFile("/Users/liruiqi/Desktop/1201xml.tmx");
    auto tile = pDoc->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
    map<int, int>::iterator tileIt = m_worldMap.begin();
    while(tile){
        tile->SetAttribute("gid", CC_ITOA(tileIt->second));
        tile = tile->NextSiblingElement();
        tileIt++;
    }
    
    auto ground = pDoc->FirstChildElement("map")->LastChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
    map<int, int>::iterator groundIt = m_worldMap1.begin();
    while(ground){
        ground->SetAttribute("gid", CC_ITOA(groundIt->second));
        ground = ground->NextSiblingElement();
        groundIt++;
    }

    pDoc->SaveFile("/Users/liruiqi/Desktop/WorldMap1.tmx");
    delete pDoc;
}

void WorldMapGenerater::intMapByFile(){
    int i = 1;
    int total = 1204 * 1204;
    while(i <= total){
        m_worldMap[i] = 1;
        m_worldMap1[i] = 0;
        i++;
    }
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    pDoc->LoadFile("/Users/liruiqi/Desktop/WorldMap.tmx");
    auto tile = pDoc->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
    int index = 1;
    while(tile){
        int gid = tile->IntAttribute("gid");
        CCPoint orgPt = WorldController::getInstance()->getPointByIndex(index, 1201, 1201);
        int nowIndex = WorldController::getInstance()->getIndexByPoint(orgPt, 1204, 1204);
        CCPoint nowPt = WorldController::getInstance()->getPointByIndex(nowIndex, 1204, 1204);
        if(orgPt.x != nowPt.x || orgPt.y != nowPt.y){
            CCLOG("1201");
        }
        m_worldMap[nowIndex] = gid;
        tile = tile->NextSiblingElement();
        index++;
    }
    
    auto ground = pDoc->FirstChildElement("map")->LastChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
    index = 1;
    while(ground){
        int gid = ground->IntAttribute("gid");
        CCPoint orgPt = WorldController::getInstance()->getPointByIndex(index, 1201, 1201);
        int nowIndex = WorldController::getInstance()->getIndexByPoint(orgPt, 1204, 1204);
        m_worldMap1[nowIndex] = gid;
        ground = ground->NextSiblingElement();
        index++;
    }
}

void WorldMapGenerater::addLinkArea(){
    int start = 1201;
    int end = 1204;
    int indexY = start;
    int indexX = 0;

    while (indexY < end) {
        indexX = 0;
        while (indexX < end) {
            CCPoint pt = ccp(indexX, indexY);
            int mapIndex = WorldController::getInstance()->getIndexByPoint(pt, 1204, 1204);
            int gid = 84;
            if(indexY == 1201){
                gid = 92;
            }else if(indexY == 1203){
                gid = 88;
            }
            m_worldMap[mapIndex] = gid;
            m_worldMap1[mapIndex] = 0;
            indexX++;
        }
        indexY++;
    }
    indexX = start;
    indexY = 0;
    while (indexX < end) {
        indexY = 0;
        while (indexY < end) {
            CCPoint pt = ccp(indexX, indexY);
            int mapIndex = WorldController::getInstance()->getIndexByPoint(pt, 1204, 1204);
            int gid = 84;
            if(indexX == 1201){
                gid = 90;
            }else if(indexX == 1203){
                gid = 86;
            }
            m_worldMap[mapIndex] = gid;
            m_worldMap1[mapIndex] = 0;
            indexY++;
        }
        indexX++;
    }
}

void WorldMapGenerater::outPut(){
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    pDoc->LoadFile("/Users/liruiqi/Desktop/WorldMap1.tmx");
    auto tile = pDoc->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data");
    for (auto tileIt = m_worldMap.begin(); tileIt != m_worldMap.end(); tileIt++) {
        tinyxml2::XMLElement *xmlElement = pDoc->NewElement("tile");
        xmlElement->SetAttribute("gid", CC_ITOA(tileIt->second));
        tile->LinkEndChild(xmlElement);
    }
    auto ground = pDoc->FirstChildElement("map")->FirstChildElement("layer");
    ground = ground->NextSiblingElement();
    ground = ground->FirstChildElement("data");
    for (auto groundIt = m_worldMap1.begin(); groundIt != m_worldMap1.end(); groundIt++) {
        tinyxml2::XMLElement *xmlElement = pDoc->NewElement("tile");
        xmlElement->SetAttribute("gid", CC_ITOA(groundIt->second));
        ground->LinkEndChild(xmlElement);
    }
    pDoc->SaveFile("/Users/liruiqi/Desktop/WorldMap2.tmx");
    
    delete pDoc;
}

void WorldMapGenerater::removeAll1201(){
    int i = 1;
    int total = 1204 * 1204;
    while(i <= total){
        auto pt = WorldController::getInstance()->getPointByIndex(i, 1204, 1204);
        if(pt.x < 1201 && pt.y < 1201){
            m_worldMap1[i] = 0;
            i++;
            continue;
        }
        if(rand() % 4 == 0){
            int randNum = 1.1 * rand();
            if(randNum % 4 == 0){
                m_worldMap1[i] = 93;
            }else if (randNum % 4 == 1){
                m_worldMap1[i] = 94;
            }else{
                m_worldMap1[i] = 95 + int(1.1 * rand()) % 4;
            }
            
        }else{
            m_worldMap1[i] = 0;
        }
        
        i++;
    }

    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    pDoc->LoadFile("/Users/liruiqi/Desktop/WorldMap1.tmx");
//    auto tile = pDoc->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->FirstChildElement("tile");
    int index = 1;
//    while(tile){
//        auto pt = WorldController::getInstance()->getPointByIndex(index, 1204, 1204);
//        if(pt.x < 1201 && pt.y < 1201){
//            tile->SetAttribute("gid", 0);
//            m_worldMap1[index] = 0;
//        }else{
//            int gid = tile->IntAttribute("gid");
//            tile->SetAttribute("gid", gid - 2);
//        }
//        tile = tile->NextSiblingElement();
//        index++;
//    }
    
    auto ground = pDoc->FirstChildElement("map")->FirstChildElement("layer");
    ground = ground->NextSiblingElement();
    ground = ground->FirstChildElement("data");
    for (auto groundIt = m_worldMap1.begin(); groundIt != m_worldMap1.end(); groundIt++) {
        tinyxml2::XMLElement *xmlElement = pDoc->NewElement("tile");
        xmlElement->SetAttribute("gid", CC_ITOA(groundIt->second));
        ground->LinkEndChild(xmlElement);
    }

    pDoc->SaveFile("/Users/liruiqi/Desktop/WorldMap2.tmx");
    delete pDoc;
}