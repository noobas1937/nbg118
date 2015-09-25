//
//  CheckAllianceTagCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#include "CheckAllianceTagCommand.h"
#include "NetController.h"

static string COMMAND_NAME = "al.abbr";

CheckAllianceTagCommand::CheckAllianceTagCommand()
: CommandBase(COMMAND_NAME)
{
    
}

bool CheckAllianceTagCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    
    callSuccess(NetResult::create(Error_OK, (CCObject*)(params->valueForKey("result"))));
    return true;
}