//
//  JoinAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#ifndef __IF__JoinAllianceCommand__
#define __IF__JoinAllianceCommand__

#include "CommandBase.h"
#define JOIN_ALLIANCE "al.join"

class JoinAllianceCommand: public CommandBase {
public:
    JoinAllianceCommand(std::string allianceId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_allianceId;
};

#endif /* defined(__IF__JoinAllianceCommand__) */
