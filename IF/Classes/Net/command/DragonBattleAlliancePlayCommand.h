//
//  DragonBattleAlliancePlayCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#ifndef __IF__DragonBattleAlliancePlayCommand__
#define __IF__DragonBattleAlliancePlayCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_ALLIANCE_PLAY "dragon.battle.alliance.play"

class DragonBattleAlliancePlayCommand: public CommandBase {
public:
    DragonBattleAlliancePlayCommand(string uid,int state) : CommandBase(DRAGON_BATTLE_ALLIANCE_PLAY){
        putParam("uid", CCString::create(uid));
        putParam("state", CCInteger::create(state));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleAlliancePlayCommand__) */
