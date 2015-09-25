//
//  GetUserInfoCommand.cpp
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#include "GetMultiUserInfoCommand.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "PlayerInfo.h"
#include "ChatController.h"

bool GetMultiUserInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
//    CCLOGFUNC("1");
    if (dict->valueForKey("cmd")->compare(GET_MULTI_USER_INFO_CMD) != 0)
        return false;
//    CCLOGFUNC("2");
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        CCLOGFUNC("3");
        return false;
    }
//    CCLOGFUNC("4");
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::createWithFail(params));
//        ChatController::getInstance()->receiveNoMessage();
//        ChatServiceCocos2dx::postNoMoreMessage(m_tmpType);
    }else{
//        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));
        ChatController::getInstance()->getMultiUserInfoCallback(params);
    }
    return true;
}
