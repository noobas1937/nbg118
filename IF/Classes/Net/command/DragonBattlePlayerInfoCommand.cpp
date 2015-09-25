//
//  DragonBattlePlayerInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#include "DragonBattlePlayerInfoCommand.h"

bool DragonBattlePlayerInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_PLAYER_INFO) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}