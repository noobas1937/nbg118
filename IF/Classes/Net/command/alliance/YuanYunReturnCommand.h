//
//  YuanYunReturnCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-14.
//
//

#ifndef __IF__YuanYunReturnCommand__
#define __IF__YuanYunReturnCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_REF_RETURN "al.ref.return"

class YuanYunReturnCommand: public CommandBase {
public:
    YuanYunReturnCommand(std::string uid) : CommandBase(ALLIANCE_AL_REF_RETURN),m_uid(uid){
        putParam("uuid", CCString::create(m_uid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

#endif /* defined(__IF__YuanYunReturnCommand__) */
