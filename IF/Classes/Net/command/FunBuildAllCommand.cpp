//
//  FunBuildAllCommand.cpp
//  IF
//
//  Created by fubin on 13-11-5.
//
//

#include "FunBuildAllCommand.h"
#include "FunBuildController.h"

bool FunBuildAllCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PALACE_SHOW_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->retSetData(params1);
        return true;
    }
    return false;
}