//
//  FriendsCommand.h
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#ifndef __IF__FriendsCommand__
#define __IF__FriendsCommand__

#include "CommandBase.h"
#define CONTACT_APPLY "contact.apply"
#define CONTACT_DELETE "contact.delete"
#define CONTACT_GET "contact.get"
#define CONTACT_AUDIT "contact.audit"
#define CONTACT_ACCEPT "contact.accept"
#define CONTACT_DESC "contact.desc"
#define CONTACT_FAVO "contact.favo"
#define CONTACT_FAVO_CANCEL "contact.favo.cancel"
#define CONTACT_GET_FB "fb.friend.get"//"search.player"
#define CONTACT_FB_FAVO "fb.friend.favo"

class ContactApplyCmd: public CommandBase
{
public:
    ContactApplyCmd(string& frienduid): CommandBase(CONTACT_APPLY), m_friendUid(frienduid)
    {
        putParam("friendUid", CCString::create(frienduid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
};

class ContactDeleteCmd: public CommandBase
{
public:
    ContactDeleteCmd(string& frienduid, int type): CommandBase(CONTACT_DELETE), m_friendUid(frienduid), m_type(type)
    {
        putParam("friendUid", CCString::create(frienduid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
    int m_type;//0 删除好友 ； 1 拒绝陌生人的好友申请
};

class ContactGetCmd: public CommandBase
{
public:
    ContactGetCmd():CommandBase(CONTACT_GET){}
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ContactAuditCmd: public CommandBase
{
public:
    ContactAuditCmd():CommandBase(CONTACT_AUDIT){}
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ContactAcceptCmd: public CommandBase
{
public:
    ContactAcceptCmd(string friendUid): CommandBase(CONTACT_ACCEPT), m_friendUid(friendUid)
    {
        putParam("friendUid", CCString::create(friendUid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
};


class ContactDescCmd: public CommandBase
{
public:
    ContactDescCmd(string uid, string desc): CommandBase(CONTACT_DESC), m_friendUid(uid), m_desc(desc)
    {
        putParam("friendUid", CCString::create(uid));
        putParam("description", CCString::create(desc));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
    string m_desc;
};

class ContactFavoCmd: public CommandBase
{
public:
    ContactFavoCmd(string uid): CommandBase(CONTACT_FAVO), m_friendUid(uid)
    {
        putParam("friendUid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
};

class ContactFBFavoCmd: public CommandBase
{
public:
    ContactFBFavoCmd(string uid,string fbUid,string fbName): CommandBase(CONTACT_FB_FAVO), m_friendUid(uid), m_fbUid(fbUid), m_fbName(fbName)
    {
        putParam("friendUid", CCString::create(uid));
        putParam("fbUid", CCString::create(fbUid));
        putParam("fbName", CCString::create(fbName));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
    string m_fbUid;
    string m_fbName;
};

class ContactFavoCancelCmd: public CommandBase
{
public:
    ContactFavoCancelCmd(string uid): CommandBase(CONTACT_FAVO_CANCEL), m_friendUid(uid)
    {
        putParam("friendUid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_friendUid;
};

class ContactGetFbCmd: public CommandBase
{
public:
    ContactGetFbCmd(string uids): CommandBase(CONTACT_GET_FB){
        putParam("uid", CCString::create(uids));
//        putParam("key", CCString::create(uids));
//        putParam("page", CCInteger::create(1));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__FriendsCommand__) */
