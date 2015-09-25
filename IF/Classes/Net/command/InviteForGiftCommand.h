//
//  InviteForGiftCommand.h
//  IF
//
//  Created by xxrdsg on 14-11-25.
//
//

#ifndef __IF__InviteForGiftCommand__
#define __IF__InviteForGiftCommand__

#include "CommandBase.h"

#define INVITE_ACCEPT "invite.accept"
#define INVITE_INFO "invite.info"
#define INVITE_REWARD "invite.reward"

class InviteAcceptCommand: public CommandBase{
public:
    InviteAcceptCommand(std::string code): CommandBase(INVITE_ACCEPT){
        putParam("code", CCString::create(code));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class InviteInfoCommand: public CommandBase{
public:
    InviteInfoCommand(): CommandBase(INVITE_INFO){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class InviteRewardCommand: public CommandBase{
public:
    InviteRewardCommand(int count): CommandBase(INVITE_REWARD){
        putParam("count", CCInteger::create(count));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__InviteForGiftCommand__) */
