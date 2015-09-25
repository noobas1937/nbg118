//
//  UserDataUpdatePush.h
//  IF
//
//  Created by wangzhenlei on 13-10-15.
//
//

#ifndef __IF__UserDataUpdatePush__
#define __IF__UserDataUpdatePush__

#include "CommonInclude.h"
/// 初始化数据下发
class UserDataUpdatePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class ServerStop {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__UserDataUpdatePush__) */
