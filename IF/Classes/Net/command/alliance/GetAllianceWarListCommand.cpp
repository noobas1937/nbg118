//
//  GetAllianceWarListCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-16.
//
//

#include "GetAllianceWarListCommand.h"
#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"

GetAllianceWarListCommand::GetAllianceWarListCommand()
: CommandBase(ALLIANCE_WAR_TEAM_LIST)
{
    
}

bool GetAllianceWarListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_WAR_TEAM_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create(Error_OK, params));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}