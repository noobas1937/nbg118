//
//  UserBindCommand.h
//  IF
//
//  Created by wangzhenlei on 13-9-29.
//
//

#ifndef __IF__UserBindCommand__
#define __IF__UserBindCommand__

#include "CommandBase.h"
#define User_Bind_COMMAND "user.bind"
#define user_Bind_Cancel "bind.cancel"
#define User_Clean_Post "user.clean.post"
#define USER_BIND_GAID "bind.gaid"

enum BIND_TYPE
{
    DEVICE,
    FACEBOOK,
    GOOGLEPLAY,
    CUSTOM,
    APPSTORE,
    QQ,//原预计QQ和微信分类绑定，现合并到TENCENT
    WEIXIN,//同上
    WEIBO,//CN
    TENCENT,//CN
    VK,//RU
};

class UserBindCommand: public CommandBase {
public:
    UserBindCommand(string googlePlay,string facebook,string device,string mail,string pass,int optType,string preUid="",string googlePlayName="",string facebookName="",string preName="", std::string bindPf = "", std::string bindId = "", std::string bindName = ""):
    CommandBase(User_Bind_COMMAND){
        m_googlePlay = googlePlay;
        m_facebook = facebook;
        m_device = device;
        m_mail = mail;
        m_pass = pass;
        m_optType =optType;
        m_preUid =preUid;
        m_preName = preName;
        m_facebookAccountName = facebookName;
        m_googleAccountName = googlePlayName;
        m_bindId = bindId;
        m_bindPf = bindPf;
        m_bindName = bindName;
        putParam("googlePlay", CCString::create(googlePlay));
        putParam("facebook", CCString::create(facebook));
        putParam("deviceId", CCString::create(device));
        putParam("mail", CCString::create(mail));
        putParam("pass", CCString::create(pass));
        putParam("optType", CCInteger::create(optType));
        putParam("googleAccountName", CCString::create(googlePlayName));
        putParam("facebookAccountName", CCString::create(facebookName));
        putParam("bindId", CCString::create(bindId));
        putParam("bindPf", CCString::create(bindPf));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        putParam("pf", CCString::create("AppStore"));
#endif
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    void changeAccountOKCallback();
    void changeAccountSecondOKCallback();
    void changeAccountCancelCallback();
    void continueAccountOKCallback();
    void cancelContinueAccountOKCallback();
    void onEnterFrame(float t);
private:
    string m_googlePlay;
    string m_facebook;
    string m_device;
    string m_mail;
    string m_pass;
    string m_ip;
    string m_zone;
    int m_port;
    string m_gameUid;
    string m_preUid;
    string m_preName;
    string m_uuid;
    int m_optType;
    string m_facebookAccountName;
    string m_googleAccountName;
    std::string m_bindPf;
    std::string m_bindId;
    std::string m_bindName;
};

class UserBindCancelCommand: public CommandBase {
public:
    UserBindCancelCommand(string type):
    CommandBase(user_Bind_Cancel){
        m_type = type;
        putParam("type", CCString::create(type));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    string m_type;
};

class UserCleanPostCommand: public CommandBase {
public:
    UserCleanPostCommand():
    CommandBase(User_Clean_Post){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

class UserBindGaidCommand: public CommandBase {
public:
    UserBindGaidCommand(string gaid):
    CommandBase(USER_BIND_GAID){
        putParam("gaid", CCString::create(gaid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

#endif /* defined(__IF__UserBindCommand__) */
