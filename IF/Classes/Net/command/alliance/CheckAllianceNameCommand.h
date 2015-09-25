//
//  CheckAllianceNameCommand.h
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#ifndef __IF__CheckAllianceNameCommand__
#define __IF__CheckAllianceNameCommand__

#include "CommandBase.h"

class CheckAllianceNameCommand: public CommandBase {
public:
    CheckAllianceNameCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__CheckAllianceNameCommand__) */
