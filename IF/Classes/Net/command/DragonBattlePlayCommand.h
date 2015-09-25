//
//  DragonBattlePlayCommand.h
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#ifndef __IF__DragonBattlePlayCommand__
#define __IF__DragonBattlePlayCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_PLAY "dragon.battle.play"

class DragonBattlePlayCommand: public CommandBase {
public:
    DragonBattlePlayCommand() : CommandBase(DRAGON_BATTLE_PLAY){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattlePlayCommand__) */
