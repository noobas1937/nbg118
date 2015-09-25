//
//  InviteFBIdAcceptCommand.h
//  IF
//
//  Created by ganxiaohua on 15/3/30.
//
//

#ifndef __IF__InviteFBIdAcceptCommand__
#define __IF__InviteFBIdAcceptCommand__

#include "CommandBase.h"
#define INVITE_FBID_ACCEPT "invite.fbId.accept"

class InviteFBIdAcceptCommand: public CommandBase {
public:
    InviteFBIdAcceptCommand(string fbId) : CommandBase(INVITE_FBID_ACCEPT){
        putParam("fbId", CCString::create(fbId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__InviteFBIdAcceptCommand__) */
