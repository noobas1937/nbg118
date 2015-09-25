//
//  CancelInviteAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#ifndef __IF__CancelInviteAllianceCommand__
#define __IF__CancelInviteAllianceCommand__

#include "CommandBase.h"

#define CANCEL_INVITE "al.cancelinvite"

class CancelInviteAllianceCommand: public CommandBase {
public:
    CancelInviteAllianceCommand(std::string Id);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_playerId;
};
#endif /* defined(__IF__CancelInviteAllianceCommand__) */
