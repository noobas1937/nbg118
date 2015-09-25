//
//  PushLeaveCrossServiceActivity.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/11.
//
//

#include "PushLeaveCrossServiceActivity.h"
#include "ActivityController.h"

void PushLeaveCrossServiceActivity::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LEAVE_CKF_SERVER,params);
    }
}