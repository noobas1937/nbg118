//
//  FeedRewardCommand.h
//  IF
//
//  Created by ganxiaohua on 15/2/5.
//
//

#ifndef __IF__FeedRewardCommand__
#define __IF__FeedRewardCommand__

#include "CommandBase.h"
#define FEED_REWARD_COMMAND "feedReward"

class FeedRewardCommand: public CommandBase {
public:
    FeedRewardCommand(string key,int type) : CommandBase(FEED_REWARD_COMMAND){
        putParam("key", CCString::create(key.c_str()));
        putParam("type", CCInteger::create(type));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__FeedRewardCommand__) */
