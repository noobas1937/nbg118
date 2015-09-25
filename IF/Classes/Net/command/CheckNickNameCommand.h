//
//  CheckNickNameCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#ifndef __IF__CheckNickNameCommand__
#define __IF__CheckNickNameCommand__

#include "CommandBase.h"

#define USER_CHECK_NICK_NAME "user.check.nickName"

class CheckNickNameCommand: public CommandBase {
public:
    CheckNickNameCommand(std::string nickName) : CommandBase(USER_CHECK_NICK_NAME),m_nickName(nickName){
        putParam("nickName", CCString::create(m_nickName.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_nickName;
};
#endif /* defined(__IF__CheckNickNameCommand__) */
