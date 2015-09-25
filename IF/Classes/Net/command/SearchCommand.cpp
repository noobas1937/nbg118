//
//  SearchCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#include "SearchCommand.h"

bool SearchPlayerCommand::handleRecieve(CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SEARCH_PLAYER_COMMAND) != 0) {
        return false;
    }
    CCDictionary *params = _dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    callSuccess(NetResult::createWithSuccess(params));
    return true;
}

bool SearchAllianceCommand::handleRecieve(CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SEARCH_ALLIANCE_COMMAND) != 0) {
        return false;
    }
    CCDictionary *params = _dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    callSuccess(NetResult::createWithSuccess(params));
    return true;
}