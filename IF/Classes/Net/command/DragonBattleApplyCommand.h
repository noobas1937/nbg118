//
//  DragonBattleApplyCommand.h
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#ifndef __IF__DragonBattleApplyCommand__
#define __IF__DragonBattleApplyCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_APPLY "dragon.battle.apply"

class DragonBattleApplyCommand: public CommandBase {
public:
    DragonBattleApplyCommand() : CommandBase(DRAGON_BATTLE_APPLY){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleApplyCommand__) */
