//
//  WorldRefresh.h
//  IF
//
//  Created by 李锐奇 on 14-9-1.
//
//

#ifndef __IF__WorldRefresh__
#define __IF__WorldRefresh__

#define WORLD_ALLIANCE_REFRESH "worldAllianceRefresh"

#include "CommonInclude.h"

class WorldAllianceRefreshPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__WorldRefresh__) */
