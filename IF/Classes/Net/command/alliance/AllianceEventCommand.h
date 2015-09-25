//
//  AllianceEventCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-8.
//
//

#ifndef __IF__AllianceEventCommand__
#define __IF__AllianceEventCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_EVENT "al.event"

class AllianceEventCommand: public CommandBase {
public:
    AllianceEventCommand(std::string lastEventTime) : CommandBase(ALLIANCE_AL_EVENT){
        putParam("lastEventTime", CCString::create(lastEventTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceEventCommand__) */
