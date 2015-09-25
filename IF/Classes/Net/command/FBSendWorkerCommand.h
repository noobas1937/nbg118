//
//  FBSendWorkerCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-21.
//
//

#ifndef __IF__FBSendWorkerCommand__
#define __IF__FBSendWorkerCommand__

#include "CommandBase.h"

#define FB_SEND_WORKER_COMMAND "mine.self"

class FBSendWorkerCommand: public CommandBase {
public:
    FBSendWorkerCommand() : CommandBase(FB_SEND_WORKER_COMMAND){
        //putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__FBSendWorkerCommand__) */
