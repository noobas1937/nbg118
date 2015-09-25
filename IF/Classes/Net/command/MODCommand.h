//
//  MODCommand.h
//  IF
//
//  Created by 李锐奇 on 14/11/20.
//
//

#ifndef __IF__MODCommand__
#define __IF__MODCommand__

#include "CommandBase.h"

#define GET_MOD_COMMAND "mod.choose"

class MODCommand: public CommandBase {
public:
    MODCommand(std::string lang) : CommandBase(GET_MOD_COMMAND), m_lang(lang){
        putParam("lang", CCString::create(lang));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_lang;

};
#define GET_MOD_LAN "modlang.list.get"//获取语言列表
class MODLanGetCommand : public CommandBase{
public:
    MODLanGetCommand() : CommandBase(GET_MOD_LAN){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define GET_MOD_REWARD "mod.get.gold"
class MODRewardCommand : public CommandBase{
public:
    MODRewardCommand() : CommandBase(GET_MOD_REWARD){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define GET_MODE_REWARD_STATUS "mod.get.gold.info"
class MODRewardInfoCommand : public CommandBase{
public:
    MODRewardInfoCommand() : CommandBase(GET_MODE_REWARD_STATUS){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define GET_USER_INFO "mod.search.user"
class MODGetUserInfoCommand : public CommandBase{
public:
    MODGetUserInfoCommand(int serverId, std::string name) : CommandBase(GET_USER_INFO){
        putParam("serverId", CCInteger::create(serverId));
        putParam("name", CCString::create(name));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define GET_ALL_SERVER_USER_INFO "mod.search.mod"
class MODGetServerModInfoCommand : public CommandBase{
public:
    MODGetServerModInfoCommand(int serverId) : CommandBase(GET_ALL_SERVER_USER_INFO){
        putParam("serverId", CCInteger::create(serverId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define SET_MOD "mod.set"

class MODSetCommand : public CommandBase{
public:
    MODSetCommand(int serverId, std::string uid) : CommandBase(SET_MOD){
        putParam("serverId", CCInteger::create(serverId));
        putParam("uid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__MODCommand__) */
