//
//  GCMRewardInfo.cpp
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#include "GCMRewardInfo.h"

GCMRewardInfo::GCMRewardInfo(CCDictionary* dict)
{
    actId = dict->valueForKey("actId")->getCString();
    title = dict->valueForKey("title")->getCString();
    contents = dict->valueForKey("contents")->getCString();
    reward = dict->valueForKey("reward")->getCString();
}