//
//  ChangeRankCommand.h
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#ifndef __IF__ChangeRankCommand__
#define __IF__ChangeRankCommand__

#include "CommandBase.h"

class ChangeRankCommand: public CommandBase {
public:
    ChangeRankCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#endif /* defined(__IF__ChangeRankCommand__) */
