//
//  ChangeSignSetCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#ifndef __IF__ChangeSignSetCommand__
#define __IF__ChangeSignSetCommand__

#include "CommandBase.h"

#define SIGN_SET "sign.set"

class ChangeSignSetCommand: public CommandBase {
public:
    ChangeSignSetCommand(std::string sign) : CommandBase(SIGN_SET),m_sign(sign){
        putParam("sign", CCString::create(m_sign.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_sign;
};

#endif /* defined(__IF__ChangeSignSetCommand__) */
