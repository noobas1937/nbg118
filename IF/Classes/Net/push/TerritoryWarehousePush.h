//
//  TerritoryWarehousePush.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/8.
//
//

#ifndef __IF__TerritoryWarehousePush__
#define __IF__TerritoryWarehousePush__

#include "CommonInclude.h"

class TerritoryWarehousePush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class TerritoryWarehouseTakebackPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class TerritoryWarehouseStoragePush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__TerritoryWarehousePush__) */
