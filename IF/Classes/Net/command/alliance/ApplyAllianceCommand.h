//
//  ApplyAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#ifndef __IF__ApplyAllianceCommand__
#define __IF__ApplyAllianceCommand__
#include "CommandBase.h"

#define APPLY_ALLIANCE "al.apply"

class ApplyAllianceCommand: public CommandBase {
public:
    ApplyAllianceCommand(std::string allianceId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_allianceId;
};
#endif /* defined(__IF__ApplyAllianceCommand__) */
