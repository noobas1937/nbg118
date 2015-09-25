//
//  LoginRewardCommand.h
//  IF
//
//  Created by 邹 程 on 13-12-2.
//
//

#ifndef __IF__LoginRewardCommand__
#define __IF__LoginRewardCommand__

#include "CommandBase.h"
#define LOGIN_REWARD_COMMAND "loginAward.receive"

class LoginRewardCommand : public CommandBase {
public:
    LoginRewardCommand():
    CommandBase(LOGIN_REWARD_COMMAND)
    {}
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#endif /* defined(__IF__LoginRewardCommand__) */
