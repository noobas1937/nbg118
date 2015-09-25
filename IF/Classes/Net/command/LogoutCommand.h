//
//  LogoutCommand.h
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __IF__LogoutCommand__
#define __IF__LogoutCommand__

#include "CommandBase.h"
#define LOGOUT_COMMAND "logout"
#define NEWACCOUNT_COMMAND "new_account"
#define MSG_RET_WARING_ACC "msg_ret_waring_acc"

class LogoutCommand: public CommandBase {
public:
    LogoutCommand();
    
//protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    CCSafeObject<CCDictionary> m_dic;
    void doReload(float f);
};

class NewACCCommand: public CommandBase {
public:
    NewACCCommand(int isOld, int isConfirm):
    CommandBase(NEWACCOUNT_COMMAND){
        putParam("old", CCInteger::create(isOld));
        if(isConfirm == 2) {
            putParam("confirm", CCInteger::create(1));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__LogoutCommand__) */
