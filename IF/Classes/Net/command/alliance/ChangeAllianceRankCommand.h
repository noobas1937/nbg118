//
//  ChangeAllianceRankCommand.h
//  IF
//
//  Created by chenliang on 14-4-4.
//
//

#ifndef __IF__ChangeAllianceRankCommand__
#define __IF__ChangeAllianceRankCommand__

#include "CommandBase.h"

class ChangeAllianceRankCommand: public CommandBase {
public:
    ChangeAllianceRankCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#endif /* defined(__IF__ChangeAllianceRankCommand__) */
