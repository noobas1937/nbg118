//
//  AllianceAcceptInviteCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-16.
//
//

#ifndef __IF__AllianceAcceptInviteCommand__
#define __IF__AllianceAcceptInviteCommand__

#include "CommandBase.h"

#define ACCEPT_INVITE "al.acceptinvite"

class AllianceAcceptInviteCommand: public CommandBase {
public:
    AllianceAcceptInviteCommand(std::string Id);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_Id;
};
#endif /* defined(__IF__AllianceAcceptInviteCommand__) */
