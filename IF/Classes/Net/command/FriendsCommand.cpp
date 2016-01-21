//
//  FriendsCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#include "FriendsCommand.h"
#include "FriendsController.h"

bool ContactApplyCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_APPLY)) {
        return false;
    }
    if (dict->objectForKey("params")) {
        auto params = _dict(dict->objectForKey("params"));
        FriendsController::getInstance()->endApplyFriend(params);
    }
    return true;
}

bool ContactDeleteCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_DELETE) != 0) {
        return false;
    }
    if (dict->objectForKey("params")) {
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        if (m_type == 0) {
            FriendsController::getInstance()->endDeleteFriend(params);
        } else {
            FriendsController::getInstance()->endRejectApply(params);
        }
        
    }
    return true;
}

bool ContactGetCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_GET) != 0) {
        return false;
    }
    
    if (dict->objectForKey("params"))
    {
        auto params = _dict(dict->objectForKey("params"));
        FriendsController::getInstance()->endGetFriendsData(params);
    }
    
    return true;
}

bool ContactAuditCmd::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_AUDIT) != 0) {
        return false;
    }
    if (dict->objectForKey("params")) {
        auto params = _dict(dict->objectForKey("params"));
//        FriendsController::getInstance()->endGetApplyingIds(params);
    }
    return true;
}

bool ContactAcceptCmd::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_ACCEPT) != 0) {
        return false;
    }
    if (dict->objectForKey("params")) {
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        FriendsController::getInstance()->endAcceptApply(params);
    }
    return true;
}

bool ContactDescCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_DESC) != 0) {
        return false;
    }
    if (dict->objectForKey("params")){
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        FriendsController::getInstance()->endChangeDescription(params);
    }
    return true;
}

bool ContactFavoCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_FAVO) != 0) {
        return false;
    }
    if (dict->objectForKey("params")){
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        FriendsController::getInstance()->endUpToStarFriend(params);
    }
    return true;
}

bool ContactFBFavoCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_FB_FAVO) != 0) {
        return false;
    }
    if (dict->objectForKey("params")){
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        params->setObject(CCString::create(m_fbUid), "m_fbUid");
        params->setObject(CCString::create(m_fbName), "m_fbName");
        FriendsController::getInstance()->endFBUpToStarFriend(params);
    }
    return true;
}

bool ContactFavoCancelCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_FAVO_CANCEL) != 0) {
        return false;
    }
    if (dict->objectForKey("params")){
        auto params = _dict(dict->objectForKey("params"));
        params->setObject(CCString::create(m_friendUid), "m_friendUid");
        FriendsController::getInstance()->endDownToNormalFriend(params);
    }
    return true;
}

bool ContactGetFbCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CONTACT_GET_FB) != 0) {
        return false;
    }
    if (dict->objectForKey("params")){
        auto params = _dict(dict->objectForKey("params"));
        FriendsController::getInstance()->endGetFbFriends(params);
    }
    return true;
}





