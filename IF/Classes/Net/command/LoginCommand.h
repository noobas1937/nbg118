//
//  LoginCommand.h
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __IF__LoginCommand__
#define __IF__LoginCommand__

#include "CommandBase.h"
#define LOGIN_COMMAND "login"
class LoginCommand: public CommandBase {
public:
    LoginCommand(int loginType = 5,bool bIsResume = false);
    void setDefaultUserName(){
        std::string uid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
        setUsername(uid);
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    bool m_bIsResume;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_username, Username);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_password, Password);
    void feedBack();
    std::string new_server;
};

#endif /* defined(__IF__LoginCommand__) */
