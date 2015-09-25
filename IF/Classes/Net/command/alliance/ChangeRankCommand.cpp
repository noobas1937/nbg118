//
//  ChangeRankCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#include "ChangeRankCommand.h"
#include "NetController.h"

static string COMMAND_NAME = "al.setrank";

ChangeRankCommand::ChangeRankCommand()
: CommandBase(COMMAND_NAME)
{
    
}

bool ChangeRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    
    //    callSuccess(NetResult::create(Error_OK, (CCObject*)(params->objectForKey("list"))));
    return true;
}