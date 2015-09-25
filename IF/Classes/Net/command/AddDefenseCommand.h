//
//  AddDefenseCommand.h
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#ifndef __IF__AddDefenseCommand__
#define __IF__AddDefenseCommand__

#include "CommandBase.h"


#define ADD_DEFENSE_COMMAND "world.user.rcd"
class AddDefenseCommand: public CommandBase {
public:
    AddDefenseCommand():
    CommandBase(ADD_DEFENSE_COMMAND)
    {}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__AddDefenseCommand__) */
