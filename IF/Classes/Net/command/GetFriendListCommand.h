//
//  GetFriendListCommand.h
//  IF
//
//  Created by xupengzhan on 15-10-20.
//
//

#ifndef __IF__GetFriendListCommand__
#define __IF__GetFriendListCommand__

#include "CommandBase.h"
#define COMMAND_GET_FRIEND_LIST "contact.get.server.current"

class GetFriendListCommand: public CommandBase {
public:
    GetFriendListCommand():CommandBase(COMMAND_GET_FRIEND_LIST)
    {};
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    void onGetFriendInfoSuccess(CCObject* pObj);
};

#endif /* defined(__IF__GetInviteeCommand__) */
