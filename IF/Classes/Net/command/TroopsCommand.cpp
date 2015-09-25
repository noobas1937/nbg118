//
//  TroopsCommand.cpp
//  IF
//
//  Created by fubin on 14-3-6.
//
//

#include "TroopsCommand.h"
#include "TroopsController.h"

bool TroopsCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TROOPS_DEFENCE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        TroopsController::getInstance()->retSaveDefence(params1);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(TROOPS_PREPARE_COMMAND) == 0)
    {
        return true;
    }
    return false;
}

bool TroopsAutoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TROOPS_AUTO_DEF_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        TroopsController::getInstance()->retSaveAutoDefence(params1);
        return true;
    }
    return false;
}