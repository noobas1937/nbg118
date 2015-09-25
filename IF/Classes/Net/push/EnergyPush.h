//
//  EnergyPush.h
//  IF
//
//  Created by fubin on 13-12-5.
//
//

#ifndef IF_EnergyPush_h
#define IF_EnergyPush_h

#include "cocos2d.h"
#include "CommonInclude.h"
/// 体力值增长数据下发
class EnergyPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif
