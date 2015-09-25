//
//  ActivityEventCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-24.
//
//

#ifndef __IF__ActivityEventCommand__
#define __IF__ActivityEventCommand__

#include "CommandBase.h"
#define ACTIVITY_EVENT_COMMAND "score.login"

class ActivityEventCommand: public CommandBase {
public:
    ActivityEventCommand() : CommandBase(ACTIVITY_EVENT_COMMAND){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__ActivityEventCommand__) */
