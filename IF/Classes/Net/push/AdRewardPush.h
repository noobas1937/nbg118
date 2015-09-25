//
//  AdRewardPush.h
//  IF
//
//  Created by chenliang on 14/12/31.
//
//

#ifndef __IF__AdRewardPush__
#define __IF__AdRewardPush__

#include "cocos2d.h"
class AdRewardPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif