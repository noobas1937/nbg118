//
//  AcceptAllianceAppliyCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#ifndef __IF__AcceptAllianceApplyCommand__
#define __IF__AcceptAllianceApplyCommand__

#include "CommandBase.h"

#define ACCEPT_APPLY "al.acceptapply"

class AcceptAllianceApplyCommand: public CommandBase {
public:
    AcceptAllianceApplyCommand(std::string Id,std::string mailId="");
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string playerId;
    std::string m_mailId;
};
#endif /* defined(__IF__AcceptAllianceAppliyCommand__) */
