//
//  EmailConfirmPush.cpp
//  IF
//
//  Created by zhaohf on 14-4-11.
//
//

#include "EmailConfirmPush.h"

void EmailConfirmPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(Email_Confirm, 1);
    CCUserDefault::sharedUserDefault()->flush();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_OK);
}