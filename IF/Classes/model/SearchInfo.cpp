//
//  PlayerSearchInfo.cpp
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#include "SearchInfo.h"
#include "UIComponent.h"
#include "ChatController.h"
#include "ScienceController.h"

PlayerSearchInfo::PlayerSearchInfo(CCDictionary* dict)
{
    if (dict->objectForKey("lang")) {
        lang = dict->valueForKey(lang)->getCString();
    }
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
    if (dict->objectForKey("pic")) {
        pic = dict->valueForKey("pic")->getCString();
    }
    if (dict->objectForKey("power")) {
        power = dict->valueForKey("power")->intValue();
    }
    if (dict->objectForKey("uid")) {
        uid = dict->valueForKey("uid")->getCString();
    }
    if (dict->objectForKey("alliancename")) {
        alliancename = dict->valueForKey("alliancename")->getCString();
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }
    if (dict->objectForKey("serverId")) {
        serverId = dict->valueForKey("serverId")->intValue();
    } else {
        serverId = -1;
    }
    if (dict->objectForKey("abbr")) {
        abbr = dict->valueForKey("abbr")->getCString();
    } else {
        abbr = "";
    }
}