//
//  UserInviteListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__UserInviteListCommand__
#define __IF__UserInviteListCommand__

#include "CommandBase.h"
#define USER_INVITES_LIST "al.userinvitelist"

class UserInviteListCommand: public CommandBase {
public:
    UserInviteListCommand(int page);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_page;
};

#endif /* defined(__IF__UserInviteListCommand__) */
