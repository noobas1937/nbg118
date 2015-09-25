//
//  GCMRewardCommand.h
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#ifndef IF_GCMRewardCommand_h
#define IF_GCMRewardCommand_h

#include "CommandBase.h"
#define GCM_REWARD_COMMAND "activity.accept.reward"
#define FEED_REWARD_COMMAND "feed.reward"

class GCMRewardCommand: public CommandBase {
public:
    GCMRewardCommand(string actId, string cmd):CommandBase(cmd)
    {
        putParam("actId", CCString::create(actId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
