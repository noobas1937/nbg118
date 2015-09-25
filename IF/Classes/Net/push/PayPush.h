//
//  PayPush.h
//  COK
//
//  Created by zhaohf on 14-5-4.
//
//

#ifndef __IF__PayPush__
#define __IF__PayPush__

#include "cocos2d.h"

class PayPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__PayPush__) */
