//
//  PushRecordCommand.c
//  IF
//
//  Created by 王智圆 on 15/4/27.
//
//

#include "PushRecordCommand.h"
#include "../../Ext/CCDevice.h"


bool PushRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(PUSH_RECORD_COMMAND) == 0)
    {
        //清除相关缓存
        cocos2d::extension::CCDevice::clearPushCache();
        return true;
    }
    return false;
}