//
//  PushSettingInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-27.
//
//

#include "PushSettingInfo.h"
void PushSettingInfo::parse(CCDictionary *dict){
    type = dict->valueForKey("type")->intValue();
    status = dict->valueForKey("status")->intValue();
    sound = dict->valueForKey("audio")->intValue();
}