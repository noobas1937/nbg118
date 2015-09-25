//
//  MonsterSiegeActivityInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/3/31.
//
//

#ifndef __IF__MonsterSiegeActivityInfoCommand__
#define __IF__MonsterSiegeActivityInfoCommand__

#include "CommandBase.h"
#define MONSTER_SIEGE_ACTIVITY_INFO "monster.siege.activity.info"

class MonsterSiegeActivityInfoCommand: public CommandBase {
public:
    MonsterSiegeActivityInfoCommand() : CommandBase(MONSTER_SIEGE_ACTIVITY_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__MonsterSiegeActivityInfoCommand__) */
