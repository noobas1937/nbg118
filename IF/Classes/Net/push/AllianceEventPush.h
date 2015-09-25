//
//  AllianceEventPush.h
//  IF
//
//  Created by ganxiaohua on 14-10-8.
//
//

#ifndef __IF__AllianceEventPush__
#define __IF__AllianceEventPush__

#include "CommonInclude.h"

class AllianceEventPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__AllianceEventPush__) */
