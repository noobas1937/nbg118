//
//  HospitalReceivePush.h
//  IF
//
//  Created by 李锐奇 on 14-4-15.
//
//

#ifndef __IF__HospitalReceivePush__
#define __IF__HospitalReceivePush__

#include "cocos2d.h"

class HospitalReceivePush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__HospitalReceivePush__) */
