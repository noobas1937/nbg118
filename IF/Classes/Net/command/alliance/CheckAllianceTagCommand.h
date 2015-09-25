//
//  CheckAllianceTagCommand.h
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#ifndef __IF__CheckAllianceTagCommand__
#define __IF__CheckAllianceTagCommand__

#include "CommandBase.h"

class CheckAllianceTagCommand: public CommandBase {
public:
    CheckAllianceTagCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__CheckAllianceTagCommand__) */
