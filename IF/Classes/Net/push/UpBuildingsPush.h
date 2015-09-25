//
//  UpBuildingsPush.h
//  IF
//
//  Created by fubin on 13-9-16.
//
//

#ifndef IF_UpBuildingsPush_h
#define IF_UpBuildingsPush_h

#include "cocos2d.h"
/// 建造队列数据下发
class UpBuildingsPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class ResBuildingsPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__UpBuildingsPush__) */
