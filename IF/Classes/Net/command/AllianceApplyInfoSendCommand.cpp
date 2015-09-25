//
//  AllianceApplyInfoSendCommand.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/5.
//
//

#include "AllianceApplyInfoSendCommand.h"

bool AllianceApplyInfoSendCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_APPLY_INFO_SEND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    
    return true;
}

bool AllianceApplyInfoGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_APPLY_INFO_GET) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}
