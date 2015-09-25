//
//  MailDeleteCommand.h
//  IF
//
//  Created by 李锐奇 on 13-10-12.
//
//

#ifndef IF_MailDeleteCommand_h
#define IF_MailDeleteCommand_h

#include "CommandBase.h"
#include "cocos2d.h"
#include "MailController.h"

#define MAIL_DELETE_COMMAND "mail.delete"
#define MAIL_DIALOG_READ_COMMAND "chatMail.read"
#define MAIL_DIALOG_SAVE_COMMAND "chatMail.save"
#define MAIL_DIALOG_DELETE_COMMAND "chatMail.delete"

class MailDeleteCommand : public CommandBase{
public:
    MailDeleteCommand(std::string uids, int count) : CommandBase(MAIL_DELETE_COMMAND){
        this->putParam("uids",  CCString::create(uids));
        this->putParam("num",  CCInteger::create(count));
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);

};

class MailDialogReadCommand : public CommandBase{
public:
    MailDialogReadCommand(std::string fromUser,int type=-1, std::string uid = "",bool isModMail=false) : CommandBase(MAIL_DIALOG_READ_COMMAND){
        if(type==-1){
            this->putParam("fromUser",  CCString::create(fromUser));
        }else{
            this->putParam("type",  CCInteger::create(type));
        }
        
        if(uid != ""){
            int mod = 0;
            auto mail = GlobalData::shared()->mailList.find(uid);
            if(mail != GlobalData::shared()->mailList.end()){
                if(mail->second->type == MAIL_MOD_SEND || mail->second->type == MAIL_MOD_PERSONAL){
                    mod = 1;
                }
            }
            if(mod != 0){
                putParam("mod", CCInteger::create(mod));
            }
        }
        
        if(isModMail)
        {
            putParam("mod", CCInteger::create(1));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

class MailDialogSaveCommand : public CommandBase{
public:
    MailDialogSaveCommand(std::string fromUser="",int flag=0,int type=-1, std::string uid = "") : CommandBase(MAIL_DIALOG_SAVE_COMMAND){
        if(type==-1){
            this->putParam("fromUser",  CCString::create(fromUser));
        }else{
            this->putParam("type",  CCInteger::create(type));
        }
        this->putParam("flag",  CCInteger::create(flag));
        if(uid != ""){
            int mod = 0;
            auto mail = GlobalData::shared()->mailList.find(uid);
            if(mail != GlobalData::shared()->mailList.end()){
                if(mail->second->type == MAIL_MOD_SEND || mail->second->type == MAIL_MOD_PERSONAL){
                    mod = 1;
                }
            }
            if(mod != 0){
                putParam("mod", CCInteger::create(mod));
            }
        }

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

class MailDialogDeleteCommand : public CommandBase{
public:
    MailDialogDeleteCommand(std::string fromUser,int type=-1, std::string uid = "") : m_uid(uid),m_type(type),CommandBase(MAIL_DIALOG_DELETE_COMMAND){
        if(type==-1){
            this->putParam("fromUser",  CCString::create(fromUser));
        }else{
            this->putParam("type",  CCInteger::create(type));
        }
        if(uid != ""){
            int mod = 0;
            auto mail = GlobalData::shared()->mailList.find(uid);
            if(mail != GlobalData::shared()->mailList.end()){
                if(mail->second->type == MAIL_MOD_SEND || mail->second->type == MAIL_MOD_PERSONAL){
                    mod = 1;
                }
            }
            if(mod != 0){
                putParam("mod", CCInteger::create(mod));
            }
        }

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_type;
    std::string m_uid;
    
};

#endif
