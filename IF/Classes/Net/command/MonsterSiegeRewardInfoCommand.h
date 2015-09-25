//
//  MonsterSiegeRewardInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/3/30.
//
//

#ifndef __IF__MonsterSiegeRewardInfoCommand__
#define __IF__MonsterSiegeRewardInfoCommand__

#include "CommandBase.h"
#define MONSTER_SIEGE_REWARD_INFO "monster.siege.reward.info"

class MonsterSiegeRewardInfoCommand: public CommandBase {
public:
    MonsterSiegeRewardInfoCommand() : CommandBase(MONSTER_SIEGE_REWARD_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__MonsterSiegeRewardInfoCommand__) */
