//
//  GetAllianceMembersCommand.h
//  IF
//
//  Created by chenliang on 14-4-4.
//
//

#ifndef __IF__GetAllianceMembersCommand__
#define __IF__GetAllianceMembersCommand__

#include "CommandBase.h"

class GetAllianceMembersCommand: public CommandBase {
public:
    GetAllianceMembersCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#endif /* defined(__IF__GetAllianceMembersCommand__) */
