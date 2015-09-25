//
//  PushBase.h
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __IF__PushBase__
#define __IF__PushBase__

#include "cocos2d.h"

/// 用于处理服务器主动推送的命令字
class PushBase {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__PushBase__) */
