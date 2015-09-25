//
//  KingdomKingChooseCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#include "KingdomKingChooseCommand.h"
#include "GameController.h"

bool KingdomKingChooseCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(KINGDOM_KING_CHOOSE) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText(pStr->getCString());
        CCLOG("errorCode = %s",pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}