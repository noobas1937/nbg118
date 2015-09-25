//
//  AllianceShopPush.h
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

#ifndef __IF__AllianceShopPush__
#define __IF__AllianceShopPush__

#include "CommonInclude.h"

class AllianceShopPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
//
class AlliancePointPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__AllianceShopPush__) */
