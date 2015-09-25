//
//  DefencePush.h
//  IF
//
//  Created by fubin on 14-4-15.
//
//

#ifndef IF_DefencePush_h
#define IF_DefencePush_h

#include "cocos2d.h"
#include "CommonInclude.h"
/// 自动防御数据下发
class DefencePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif
