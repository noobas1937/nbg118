//
//  DragonBattleInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#ifndef __IF__DragonBattleInfoCommand__
#define __IF__DragonBattleInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_INFO "dragon.battle.info"

class DragonBattleInfoCommand: public CommandBase {
public:
    DragonBattleInfoCommand() : CommandBase(DRAGON_BATTLE_INFO){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleInfoCommand__) */
