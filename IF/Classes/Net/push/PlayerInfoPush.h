//
//  PlayerInfoPush.h
//  IF
//
//  Created by ganxiaohua on 14-4-8.
//
//

#ifndef __IF__PlayerInfoPush__
#define __IF__PlayerInfoPush__

#include "cocos2d.h"
#include "CommonInclude.h"

class PlayerInfoPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__PlayerInfoPush__) */
