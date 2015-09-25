//
//  InviteForGiftCommand.cpp
//  IF
//
//  Created by xxrdsg on 14-11-25.
//
//

#include "InviteForGiftCommand.h"

bool InviteAcceptCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if(dict->valueForKey("cmd")->compare(INVITE_ACCEPT) != 0){
        return false;
    }
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }

    callSuccess(NetResult::createWithSuccess(params));
    
    return true;
}

bool InviteInfoCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if(dict->valueForKey("cmd")->compare(INVITE_INFO) != 0) {
        return false;
    }
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    
    return true;
}

bool InviteRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if(dict->valueForKey("cmd")->compare(INVITE_REWARD) != 0) {
        return false;
    }
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
    } else {
        callSuccess(NetResult::createWithSuccess(params));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
    return true;
}