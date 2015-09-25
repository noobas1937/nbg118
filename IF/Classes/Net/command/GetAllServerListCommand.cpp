//
//  GetAllServerListCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-10-13.
//
//

#include "GetAllServerListCommand.h"
#include "WorldController.h"

bool GetAllServerListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CROSS_SERVER_LS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        WorldController::getInstance()->parseServerList(params);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SERVER_LIST_BACK, params);
    }
    
    return true;
}