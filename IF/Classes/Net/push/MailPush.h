//
//  MailPush.h
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#ifndef IF_MailPush_h
#define IF_MailPush_h

#include "cocos2d.h"
/// mail数据下发
class MailPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif
