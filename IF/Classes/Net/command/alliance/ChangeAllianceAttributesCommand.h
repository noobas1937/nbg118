//
//  ChangeAllianceAttributesCommand.h
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#ifndef __IF__ChangeAllianceAttributesCommand__
#define __IF__ChangeAllianceAttributesCommand__

#include "CommandBase.h"

class ChangeAllianceAttributesCommand: public CommandBase {
public:
    ChangeAllianceAttributesCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};




#endif /* defined(__IF__ChangeAllianceAttributesCommand__) */
