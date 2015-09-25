//
//  ChatLockInfoCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#include "ChatLockInfoCommand.h"
#include "ShieldUnlockPopUpView.h"

bool ChatLockInfoCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_LOCK_INFO_COMMAND) != 0){
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("chatShield"));
        if(arr){
            GlobalData::shared()->chatShieldInfo.parse(arr);
        }
        GlobalData::shared()->chatShieldInfo.isGetInfo = true;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_SHIELD_PANEL);
    return true;
}