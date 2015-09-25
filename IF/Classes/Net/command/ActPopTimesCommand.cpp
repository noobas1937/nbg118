//
//  ActPopTimesCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-7-9.
//
//

#include "ActPopTimesCommand.h"

bool ActPopTimesCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ACT_POPUP) != 0)
        return false;
    return true;
}