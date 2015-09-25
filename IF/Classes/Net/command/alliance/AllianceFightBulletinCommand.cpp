//
//  AllianceFightBulletinCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#include "AllianceFightBulletinCommand.h"

bool AllianceFightBulletinCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_FIGHT_BULLETIN) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}