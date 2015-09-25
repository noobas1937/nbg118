//
//  GetUserInfoCommand.h
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#ifndef IF_GetUserInfoCommand_h
#define IF_GetUserInfoCommand_h

#include "CommandBase.h"
#define GET_USER_INFO_CMD "get.user.info"

class GetUserInfoCommand: public CommandBase {
public:
    GetUserInfoCommand(std::string uid) : CommandBase(GET_USER_INFO_CMD){
        putParam("uid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define USER_EQUIP_SHOW_CMD "equip.show"

class SetUserEquipShowCommand: public CommandBase {
public:
    SetUserEquipShowCommand(int isShow) : CommandBase(USER_EQUIP_SHOW_CMD){
        putParam("show", CCInteger::create(isShow));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif
