//
//  AllianceReinforceSoldier.h
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#ifndef __IF__AllianceReinforceSoldier__
#define __IF__AllianceReinforceSoldier__

#include "CommandBase.h"

#define ALLIANCE_AL_REINFORCE_SOLDIER "al.ref.cnt"

class AllianceReinforceSoldier: public CommandBase {
public:
    AllianceReinforceSoldier() : CommandBase(ALLIANCE_AL_REINFORCE_SOLDIER){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceReinforceSoldier__) */
