//
//  PushAllianceUpdate.h
//  IF
//
//  Created by ganxiaohua on 14-7-14.
//
//

#ifndef __IF__PushAllianceUpdate__
#define __IF__PushAllianceUpdate__

#include "CommonInclude.h"

class PushAllianceUpdate{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class PushAllianceScience{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__PushAllianceUpdate__) */
