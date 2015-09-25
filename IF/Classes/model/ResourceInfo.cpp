//
//  ResourceInfo.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-30.
//
//

#include "CommonInclude.h"
#include "ResourceInfo.h"

void ResourceInfo::setResourceData(CCDictionary *dict){
    const CCString *pStr = dict->valueForKey("silver");
    if(pStr->compare("")!=0){
        lMoney = pStr->intValue();
    }
    pStr = dict->valueForKey("food");
    if(pStr->compare("")!=0){
        lFood = pStr->intValue();
    }
    pStr = dict->valueForKey("wood");
    if(pStr->compare("")!=0){
        lWood = pStr->intValue();
    }
    pStr = dict->valueForKey("iron");
    if(pStr->compare("")!=0){
        lIron = pStr->intValue();
    }
    pStr = dict->valueForKey("stone");
    if(pStr->compare("")!=0){
        lStone = pStr->intValue();
    }
    pStr = dict->valueForKey("chip");
    if(pStr->compare("")!=0){
        lChip = pStr->intValue();
    }
    pStr = dict->valueForKey("diamond");
    if(pStr->compare("")!=0){
        lDiamond = pStr->intValue();
    }
    pStr = dict->valueForKey("dragonFood");
    if(pStr->compare("")!=0){
        lDragonFood = pStr->intValue();
    }
    pStr = dict->valueForKey("dragonGold");
    if(pStr->compare("")!=0){
        lDragonGold = pStr->intValue();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);

}