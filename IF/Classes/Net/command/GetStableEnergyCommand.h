//
//  GetStableEnergyCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-27.
//
//

#ifndef __IF__GetStableEnergyCommand__
#define __IF__GetStableEnergyCommand__

#include "CommandBase.h"
#include "CommonInclude.h"

#define GET_STABLE_ENERGY_COMMAND "lord.accept.energy"

class GetStableEnergyCommand: public CommandBase {
public:
    GetStableEnergyCommand():
    CommandBase(GET_STABLE_ENERGY_COMMAND)
    {
    
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__GetStableEnergyCommand__) */
