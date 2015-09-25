//
//  AllianceGiftInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-10.
//
//

#include "AllianceGiftInfo.h"

void AllianceGiftInfo::updateInfo(CCDictionary* dict){
    if (dict->valueForKey("id")) {
        id = dict->valueForKey("id")->getCString();
    }
    if (dict->valueForKey("icon")) {
        icon = dict->valueForKey("icon")->getCString();
    }
    if (dict->valueForKey("desc")) {
        icon = dict->valueForKey("desc")->getCString();
    }
    if (dict->valueForKey("name")) {
        icon = dict->valueForKey("name")->getCString();
    }
    if (dict->valueForKey("giftId")) {
        giftId = dict->valueForKey("giftId")->getCString();
    }
    if (dict->valueForKey("gifttype")) {
        gifttype = dict->valueForKey("gifttype")->intValue();
    }
    if (dict->valueForKey("giftlevel")) {
        giftlevel = dict->valueForKey("giftlevel")->intValue();
    }
    if (dict->valueForKey("timeleft")) {
        timeleft = dict->valueForKey("timeleft")->doubleValue();
    }
    if (dict->valueForKey("valid")) {
        valid = dict->valueForKey("valid")->intValue();
    }
    if (dict->valueForKey("item")) {
        item = dict->valueForKey("item")->intValue();
    }
    if (dict->valueForKey("exp")) {
        exp = dict->valueForKey("exp")->intValue();
    }
    if (dict->valueForKey("para")) {
        para = dict->valueForKey("para")->intValue();
    }
}