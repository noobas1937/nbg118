//
//  ServerFightMapGenerater.cpp
//  IF
//
//  Created by liruiqi on 15/6/5.
//
//

#include "ServerFightMapGenerater.h"
#include "TiledFourCell.h"
#include "tinyxml2/tinyxml2.h"
static ServerFightMapGenerater *_instance = NULL;

ServerFightMapGenerater *ServerFightMapGenerater::create(){
    if(_instance == NULL){
        _instance = new ServerFightMapGenerater;
        if(_instance && _instance->init()){
            
        }else{
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

bool ServerFightMapGenerater::init(){
    getMapConfig();
    generateMap();
    return true;
}

void ServerFightMapGenerater::getMapConfig(){
    mapH = 600;
    mapW = 600;
    totalCell = mapW * mapH;
}

void ServerFightMapGenerater::generateMap(){
    initMap();
    addTileFourLand(0, CCArray::create());
    outputFile();
}

void ServerFightMapGenerater::addTileFourLand(int type, CCArray *arr){
    map<int, int> allLakes;
    if(type == 3){
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            int index = dynamic_cast<CCInteger*>(obj)->getValue();
            allLakes[index] = 1;
        }
    }else{
        int startX = 1;
        int gapX = FightBlockSize;
        int startY = 1;
        int gapY = FightBlockSize;
        int numPerArea = 0;
        while(startY < mapH){
            startX = 1;
            while(startX < mapW){
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
    auto setCell = [&](int index, map<int, TiledFourCell*> &map, int cellPos){
        if(index < 0 || index > totalCell){
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
        int index3 = index1 + mapW;
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
            start = StartPicIndex;
        }
        
        int resIndex = total + start - 1;
        m_worldMap[cellIt->first] = resIndex;
    }
    
    cellMap.clear();
}

CCPoint ServerFightMapGenerater::getPointByIndex(unsigned int index) {
    if (index == 0) {
        return ccp(1, 1);
    }
    if(index < 1 || index > mapW * mapH){
        return ccp(1, 1);
    }
    unsigned int z = index - 1;
    return ccp((int)(z % mapW), (int)(z / mapW));
}

unsigned int ServerFightMapGenerater::getIndexByPoint(const cocos2d::CCPoint &point) {
    if (point.x < 0 || point.y < 0 || point.x > mapW - 1 || point.y > mapH - 1) {
        return 0;
    }
    return (unsigned int)(point.x) + (unsigned int)(point.y * mapW) + 1;
}

void ServerFightMapGenerater::addTileFourCells(int startX, int startY, int area, map<int, int> &indexMap, int type){
    auto addEmptyIndex = [&](int index, std::map<int, int> &checkMap1, std::map<int, int> &checkMap2, std::map<int, int> &checkMap3, CCArray *arr){
        if(index < 0 || index > totalCell){
            return;
        }
        
        auto pt = getPointByIndex(index);
        if(pt.x == 0 || pt.x == mapW - 1 || pt.y == 0 || pt.y == mapH - 1){
            return ;
        }
        
        map<int, int>::iterator it2 = checkMap2.find(index);
        if(it2 == checkMap2.end() || it2->second >= StartPicIndex){
            return;
        }
        
        map<int, int>::iterator it3 = checkMap3.find(index);
        if(it3 != checkMap3.end()){
            return;
        }
        auto check = [](int index, std::map<int, int> &checkMap1, std::map<int, int> &checkMap2){
            map<int, int>::iterator it1 = checkMap1.find(index);
            map<int, int>::iterator it2 = checkMap2.find(index);
            
            if(it1 != checkMap1.end() || it2->second >= StartPicIndex){
                return true;
            }
            return false;
        };
        
        if(check(index, checkMap1, checkMap2)
           || check(index + 1, checkMap1, checkMap2)
           || check(index - 1, checkMap1, checkMap2)
           || check(index - mapW, checkMap1, checkMap2)
           || check(index - mapW + 1, checkMap1, checkMap2)
           || check(index - mapW - 1, checkMap1, checkMap2)
           || check(index + mapW, checkMap1, checkMap2)
           || check(index + mapW + 1, checkMap1, checkMap2)
           || check(index + mapW - 1, checkMap1, checkMap2)
           ){
            return;
        }
        
        arr->addObject(CCInteger::create(index));
    };
    
    int startIndex = getIndexByPoint(ccp(startX, startY));
    map<int, int>::iterator it = m_worldMap.find(startIndex);
    if(it->second >= StartPicIndex){
        return;
    }
    
    if(startIndex < 0 || startIndex > totalCell){
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
        int up = mapIndex - mapW;
        int dp = mapIndex + mapW;
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

void ServerFightMapGenerater::initMap(){
    int i = 1;
    int max = StartPicIndex - 1;
    int decoration = 10;
    while(i <= totalCell){
        m_worldMap[i] = rand() % max + 1;
        if(rand() % decoration == 0){
            m_worldMap1[i] = StartDecoration + rand() % DecorationNum;
        }else{
            m_worldMap1[i] = BlankCellIndex;
        }
        i++;
    }
}

void ServerFightMapGenerater::outputFile(){
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    pDoc->LoadFile("/Users/liruiqi/Desktop/orginal.tmx");
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
    pDoc->SaveFile("/Users/liruiqi/Desktop/ServerFight.tmx");

    delete pDoc;
}
