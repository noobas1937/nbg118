//
//  InvitesAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-2.
//
//

#ifndef __IF__InvitesAllianceCommand__
#define __IF__InvitesAllianceCommand__

#include "CommandBase.h"

#define INVITES_PLAYER "al.invite"

class InvitesAllianceCommand: public CommandBase {
public:
    InvitesAllianceCommand(std::string playerId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_playerId;
};
#endif /* defined(__IF__InvitesAllianceCommand__) */
