//
//  ChatUnLockCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#include "ChatUnLockCommand.h"

bool ChatUnLockCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_UNLOCK_COMMAND) != 0){
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }
    else
    {
        string _uuid = params->valueForKey("uuid")->getCString();
        ShieldInfo* info = GlobalData::shared()->chatShieldInfo.getShieldInfo(_uuid);
        CCCommonUtils::flyHint("quest_icon_1.png", _lang("105315"), _lang_1("105320", info->name.c_str()));
        GlobalData::shared()->chatShieldInfo.removeFromList(info->uid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("refreshShieldPanel");
    }
    
    return true;
}