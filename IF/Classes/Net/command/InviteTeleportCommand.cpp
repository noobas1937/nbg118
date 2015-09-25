//
//  InviteTeleportCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-2-28.
//
//

#include "InviteTeleportCommand.h"
#include "AllianceManager.h"

bool InviteTeleportCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("cmd"))
    {
        if (dict->valueForKey("cmd")->compare(AL_INVITE_TELEPORT) != 0) {
            return false;
        }
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115288"));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITE_TELEPORT_SEND_SUCCESS);
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool RefuseTeleportCommand::handleRecieve(cocos2d::CCDictionary* dict) {
    if (dict->objectForKey("cmd")) {
        if (dict->valueForKey("cmd")->compare(AL_REFUSE_TELEPORT) != 0) {
            return false;
        }
    }
    
    if (!dict->objectForKey("params")) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;

}

bool GetNearestPointCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (!dict->objectForKey("cmd")) {
        return false;
    }
    if (dict->valueForKey("cmd")->compare(AL_NEAREST_POING_GET) != 0) {
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(params, "params");
    dic->setObject(CCString::create(m_mailUid), "mailUid");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GET_NEAREST_POINT_END, dic);
    return true;
}