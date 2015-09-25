//
//  RebuildCommand.h
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#ifndef __IF__RebuildCommand__
#define __IF__RebuildCommand__

#include "CommandBase.h"


#define REBUILD_COMMAND "world.user.rcr"
class RebuildCommand: public CommandBase {
public:
    RebuildCommand():
    CommandBase(REBUILD_COMMAND)
    {}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#endif /* defined(__IF__RebuildCommand__) */
