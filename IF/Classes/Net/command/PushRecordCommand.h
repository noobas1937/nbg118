//
//  PushRecordCommand.h
//  IF
//
//  Created by 王智圆 on 15/4/27.
//
//

#ifndef __IF__PushRecordCommand__
#define __IF__PushRecordCommand__

#include "CommandBase.h"

#define PUSH_RECORD_COMMAND "push.record"

class PushRecordCommand : public CommandBase{
public:
    PushRecordCommand(std::string pushRecordData,std::string pushClickData) : CommandBase(PUSH_RECORD_COMMAND){
        putParam("pushRecordData", CCString::create(pushRecordData));
        putParam("pushClickData", CCString::create(pushClickData));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__PushRecordCommand__) */
