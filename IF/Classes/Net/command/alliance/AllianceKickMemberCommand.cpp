//
//  AllianceKickMemberCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-9.
//
//

#include "AllianceKickMemberCommand.h"
#include "NetController.h"

static string COMMAND_NAME = "al.kick";

AllianceKickMemberCommand::AllianceKickMemberCommand()
: CommandBase(COMMAND_NAME)
{
    
}

bool AllianceKickMemberCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        callSuccess(NetResult::create(Error_OK));
    }
    return true;
}