//
//  DragonBattlePlayerInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#ifndef __IF__DragonBattlePlayerInfoCommand__
#define __IF__DragonBattlePlayerInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_PLAYER_INFO "dragon.battle.player.info"

class DragonBattlePlayerInfoCommand: public CommandBase {
public:
    DragonBattlePlayerInfoCommand() : CommandBase(DRAGON_BATTLE_PLAYER_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattlePlayerInfoCommand__) */
