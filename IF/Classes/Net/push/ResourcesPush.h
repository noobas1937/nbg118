//
//  ResourcesPush.h
//  IF
//
//  Created by zhaohf on 13-9-6.
//
//

#ifndef __IF__ResourcesPush__
#define __IF__ResourcesPush__

#include "cocos2d.h"
/// 资源更新数据下发
class ResourcesPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ResourcesPush__) */
