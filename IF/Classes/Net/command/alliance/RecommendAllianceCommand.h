//
//  CreateAllianceCommand.h
//  IF
//
//  Created by chenliang on 14-4-2.
//
//

#ifndef __IF__RecommendAllianceCommand__
#define __IF__RecommendAllianceCommand__

#include "CommandBase.h"

class RecommendAllianceCommand: public CommandBase {
public:
    RecommendAllianceCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__CreateAllianceCommand__) */
