//
//  CDkeyRewardCommand.h
//  IF
//
//  Created by ganxiaohua on 14-10-27.
//
//

#ifndef __IF__CDkeyRewardCommand__
#define __IF__CDkeyRewardCommand__

#include "CommandBase.h"
#define CDKEY_REWARD_CMD "activation.code"

class CDkeyRewardCommand: public CommandBase {
public:
    CDkeyRewardCommand(std::string code) : CommandBase(CDKEY_REWARD_CMD),m_code(code){
        putParam("code", CCString::create(m_code.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_code;
};
#endif /* defined(__IF__CDkeyRewardCommand__) */
