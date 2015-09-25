//
//  KingsGiftCommand.cpp
//  IF
//
//  Created by 李锐奇 on 15/1/26.
//
//

#include "KingsGiftCommand.h"

bool KingsGiftGetInfoCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GET_GIFTS_INFO) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}

bool KingsGiftPostCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(POST_GIFTS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        std::string errorStr = pStr->getCString();
        if("110117" == errorStr){
            CCCommonUtils::flyHint("", "", _lang_1(errorStr, m_name.c_str()));
        }else{
            CCCommonUtils::flyHint("", "", _lang(errorStr));
        }
    }else{
        CCCommonUtils::flyHint("", "", _lang("110131"));
        callSuccess(NetResult::create(Error_OK, params));
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}

bool GetKingsGiftHistoryCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GET_HISTORY_INFO) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}
