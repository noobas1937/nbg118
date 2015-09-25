//
//  MonsterSiegeStartCommand.h
//  IF
//
//  Created by ganxiaohua on 15/3/16.
//
//

#ifndef __IF__MonsterSiegeStartCommand__
#define __IF__MonsterSiegeStartCommand__

#include "CommandBase.h"
#define MONSTER_SIEGE_START "monster.siege.start"

class MonsterSiegeStartCommand: public CommandBase {
public:
    MonsterSiegeStartCommand() : CommandBase(MONSTER_SIEGE_START){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__MonsterSiegeStartCommand__) */
