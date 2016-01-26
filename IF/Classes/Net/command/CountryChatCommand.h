//
//  CountryChatCommand.h
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#include "../../Ext/CCDevice.h"
#ifndef IF_CountryChatCommand_h
#define IF_CountryChatCommand_h

#include "CommandBase.h"
#define CHAT_STATE_COUNTRY_COMMAND "chat.country"
#define CHAT_STATE_ALLIANCE_COMMAND "al.msg"
#define CHAT_STATE_GET_COMMAND "chat.get"
#define CHAT_STATE_NOTICE_COMMAND "chat.notice"
class CountryChatCommand: public CommandBase {
public:
    CountryChatCommand(string cmd,const string& msg,int post=0,string sendLocalTime="",const string& dialog ="",CCArray* msgArr = NULL,string thxuuid=""):
    CommandBase(cmd){
        m_post = post;
        putParam("msg", CCString::create(msg));
        putParam("lang",CCString::create(CCCommonUtils::getLanguage()));
        putParam("post",CCInteger::create(post));
        putParam("sendLocalTime",CCString::create(sendLocalTime));
        if (dialog!="") {
            putParam("dialog",CCString::create(dialog));
        }
        if (msgArr!=NULL) {
            putParam("msgarr",msgArr);
        }
        if(thxuuid!=""){
            putParam("thanksUid",CCString::create(thxuuid));
        }
    }

    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_post;
private:
};

class NoticeChatCommand: public CommandBase {
public:
    NoticeChatCommand(std::string msg,bool usePoint,int itemid,std::string sendLocalTime=""):
    CommandBase(CHAT_STATE_NOTICE_COMMAND),mUsePoint(usePoint),mItemId(itemid){
        putParam("msg", CCString::create(msg));
        if(mUsePoint){
            putParam("usePoint", CCInteger::create(1));
        }else{
            putParam("usePoint", CCInteger::create(0));
        }
        putParam("itemId", CCString::create(CC_ITOA(mItemId)));
        putParam("sendLocalTime",CCString::create(sendLocalTime));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    bool mUsePoint;
    int mItemId;
};

class GetChatCommand: public CommandBase {
public:
    GetChatCommand(int type, int start, int end):
    CommandBase(CHAT_STATE_GET_COMMAND){
        putParam("type", CCInteger::create(type));
        putParam("start", CCInteger::create(start));
        putParam("end", CCInteger::create(end));
        putParam("lang",CCString::create(cocos2d::extension::CCDevice::getLanguage()));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__CountryChatCommand__) */
