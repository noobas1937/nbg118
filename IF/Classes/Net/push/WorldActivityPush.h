//
//  WorldActivityPush.h
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#ifndef __IF__WorldActivityPush__
#define __IF__WorldActivityPush__

#include "CommonInclude.h"

class WorldActivityPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchDeletePush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldMarchAddPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class WorldTrebuchetAttPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class OfficerClearPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class OfficerAddPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__WorldActivityPush__) */
