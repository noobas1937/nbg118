//
//  JoinAllianceTeamCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-18.
//
//

#include "JoinAllianceTeamCommand.h"

bool JoinAllianceTeamCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_TEAM_JOIN) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
    }else{
        
        callSuccess(NetResult::create());
    }
    return true;
}