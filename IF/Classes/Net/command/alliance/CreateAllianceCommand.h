//
//  CreateAllianceCommand.h
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#ifndef __IF__CreateAllianceCommand__
#define __IF__CreateAllianceCommand__

#include "CommandBase.h"

class CreateAllianceCommand: public CommandBase {
public:
    CreateAllianceCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__CreateAllianceCommand__) */
