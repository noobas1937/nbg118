//
//  ResourceInfo.h
//  IF
//
//  Created by zhaohf on 13-9-16.
//
//

#ifndef __IF__ResourceInfo__
#define __IF__ResourceInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class ResourceInfo {
public:
    ResourceInfo(CCDictionary* dict);
    ResourceInfo():
    lStorageMoney(0)
    ,lStorageFood(0)
    ,lStorageWood(0)
    ,lStorageIron(0)
    ,lStorageStone(0)
    ,lMoney(0)
    ,lFood(0)
    ,lWood(0)
    ,lIron(0)
    ,lStone(0)
    ,lChip(0)
    ,lDiamond(0)
    ,warehouseFood(0)
    ,warehouseWood(0)
    ,warehouseStone(0)
    ,warehouseIron(0)
    {}
    //资源刷新接口
    void setResourceData(CCDictionary* dict);
    //仓库上限
    long lStorageMoney;
    long lStorageFood;
    long lStorageWood;
    long lStorageIron;
    long lStorageStone;

    //当前资源数量
    long lMoney;
    long lFood;
    long lWood;
    long lStone;
    long lIron;
    
    //联盟仓库资源数量
    long warehouseFood;
    long warehouseWood;
    long warehouseStone;
    long warehouseIron;
    //抽奖需要用到的资源，lChip:筹码，lDiamond:钻石
    long lChip;
    long lDiamond;
    
    //龙粮食，龙金币
    long lDragonFood;
    long lDragonGold;
};


#endif /* defined(__IF__ResourceInfo__) */
