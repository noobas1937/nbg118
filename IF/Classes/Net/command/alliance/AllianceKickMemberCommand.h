//
//  AllianceKickMemberCommand.h
//  IF
//
//  Created by chenliang on 14-4-9.
//
//

#ifndef __IF__AllianceKickMemberCommand__
#define __IF__AllianceKickMemberCommand__

#include "CommandBase.h"

class AllianceKickMemberCommand: public CommandBase {
public:
    AllianceKickMemberCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#endif /* defined(__IF__AllianceKickMemberCommand__) */
