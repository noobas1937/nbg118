//
//  ChangeNickNameCommand.h
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#ifndef __IF__ChangeNickNameCommand__
#define __IF__ChangeNickNameCommand__

#include "CommandBase.h"
#define USER_MODIFY_NICKNAME_COMMAND "user.modify.nickName"
#define MSG_CHANGE_NAME_BACK "changeNameBack"

class ChangeNickNameCommand: public CommandBase {
public:
    ChangeNickNameCommand(string nickName, string goodsUid=""):
    CommandBase(USER_MODIFY_NICKNAME_COMMAND){
        m_newNickName = nickName;
        putParam("nickName",CCString::create(nickName));
        putParam("goodsUid", CCString::create(goodsUid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
    string m_newNickName;
    
};

#endif /* defined(__IF__ChangeNickNameCommand__) */
