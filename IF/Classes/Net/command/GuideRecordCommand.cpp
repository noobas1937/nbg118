//
//  GuideRecordCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-11-13.
//
//

#include "GuideRecordCommand.h"

bool GuideRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GUIDE_RECORD_COMMAND) == 0)
    {
        return true;
    }
    return false;
}