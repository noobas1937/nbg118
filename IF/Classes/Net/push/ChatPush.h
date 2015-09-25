//
//  ChatPush.h
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#ifndef IF_ChatPush_h
#define IF_ChatPush_h

#include "cocos2d.h"
/// 聊天数据下发
class ChatPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
/// 聊天数据下发
class ChatBanPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ChatPush__) */
