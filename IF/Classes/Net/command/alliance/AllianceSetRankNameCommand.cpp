//
//  AllianceSetRankNameCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#include "AllianceSetRankNameCommand.h"

bool AllianceSetRankNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_RANK_NAME_SET) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        callSuccess(NetResult::create(Error_OP_FAILURE, (CCObject*)(params)));
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));
    }
    return true;
}

bool AllianceSetTerritoryNameCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_TERRITORY_NAME_SET) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        callSuccess(NetResult::create(Error_OP_FAILURE, (CCObject*)(params)));
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));
    }
    return true;
}