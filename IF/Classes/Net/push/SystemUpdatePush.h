//
//  SystemUpdatePush.h
//  IF
//
//  Created by liweiyu on 14-9-16.
//
//

#ifndef __IF__SystemUpdatePush__
#define __IF__SystemUpdatePush__

#include "cocos2d.h"

class SystemUpdatePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__SystemUpdatePush__) */
