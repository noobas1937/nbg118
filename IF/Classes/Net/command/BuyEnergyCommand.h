//
//  BuyEnergyCommand.h
//  IF
//
//  Created by fubin on 13-12-5.
//
//

#ifndef IF_BuyEnergyCommand_h
#define IF_BuyEnergyCommand_h

#include "CommandBase.h"
#define BUY_ENERGY_COMMAND "lord.buy.energy"

class BuyEnergyCommand: public CommandBase {
public:
    BuyEnergyCommand(string cmd):CommandBase(cmd){}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
