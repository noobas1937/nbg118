//
//  GetUserInfoCommand.h
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#ifndef IF_GetMultiUserInfoCommand_h
#define IF_GetMultiUserInfoCommand_h

#include "CommandBase.h"
#define GET_MULTI_USER_INFO_CMD "get.user.info.multi"

class GetMultiUserInfoCommand: public CommandBase {
public:
    GetMultiUserInfoCommand(vector<std::string> *uids) : CommandBase(GET_MULTI_USER_INFO_CMD){
        CCArray *arr = CCArray::create();
        for(vector<std::string>::iterator it = uids->begin(); it != uids->end(); ++it){
            arr->addObject(CCString::create(*it));
//            CCLog("In GetMultiUserInfoCommand");
            std::string str = *it;
            CCLog("para: %s", str.c_str());
        }
        putParam("uids", arr);
        putParam("allservers", CCString::create("1"));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif
