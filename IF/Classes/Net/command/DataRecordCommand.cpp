//
//  DataRecordCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-1-10.
//
//

#include "DataRecordCommand.h"

bool DataRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(OPEN_PANEL_COMMAND) != 0)
        return false;
    return true;
}
bool PayRecordCommand::isSendRecord = false;
bool PayRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(PAY_RECORD_COMMAND) != 0)
        return false;
    isSendRecord=false;
    return true;
}