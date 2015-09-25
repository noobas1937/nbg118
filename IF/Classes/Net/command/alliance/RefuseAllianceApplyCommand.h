//
//  RefuseAllianceApplyCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#ifndef __IF__RefuseAllianceApplyCommand__
#define __IF__RefuseAllianceApplyCommand__

#include "CommandBase.h"

#define REFUSE_APPLY "al.refuseapply"

class RefuseAllianceApplyCommand: public CommandBase {
public:
    RefuseAllianceApplyCommand(std::string playerId,std::string mailId="");
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_playerId;
    std::string m_mailId;
};
#endif /* defined(__IF__RefuseAllianceApplyCommand__) */
