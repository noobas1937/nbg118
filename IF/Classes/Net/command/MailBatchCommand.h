
#ifndef __IF__MailBatchCommand__
#define __IF__MailBatchCommand__

#include "CommandBase.h"
#include "cocos2d.h"
#include "MailController.h"

#define MAIL_DELETE_BATCH_COMMAND "mail.delete.batch"
#define MAIL_REWARD_BATCH_COMMAND "mail.reward.batch"
#define MAIL_SAVE_BATCH_COMMAND "mail.save.batch"
#define MAIL_READ_BATCH_COMMAND "mail.read.status.betch"
class MailBatchDelCommand : public CommandBase{
public:
    MailBatchDelCommand(std::string uids, std::string types) : CommandBase(MAIL_DELETE_BATCH_COMMAND),m_uids(uids){
        if(!uids.empty()){
            this->putParam("uids",  CCString::create(uids));
        }
        if(!types.empty()){
            this->putParam("types",  CCString::create(types));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uids;
};

class MailBatchRewardCommand : public CommandBase{
public:
    MailBatchRewardCommand(std::string uids, std::string types) : CommandBase(MAIL_REWARD_BATCH_COMMAND),m_uids(uids){
        if(!uids.empty()){
            this->putParam("uids",  CCString::create(uids));
        }
        if(!types.empty()){
            this->putParam("types",  CCString::create(types));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uids;
};

class MailBatchSaveCommand : public CommandBase{
public:
    MailBatchSaveCommand(std::string uids, int flag) : CommandBase(MAIL_SAVE_BATCH_COMMAND),m_uids(uids),m_flag(flag){
        if(!uids.empty()){
            this->putParam("uids",  CCString::create(uids));
        }
        this->putParam("flag",  CCInteger::create(flag));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uids;
    int m_flag;
};

class MailReadStatusBatchCommand : public CommandBase{
public:
    MailReadStatusBatchCommand(std::string uids) : m_uids(uids), CommandBase(MAIL_READ_BATCH_COMMAND){
        putParam("uids", CCString::create(uids));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uids;
};

#endif /* defined(__IF__MailBatchCommand__) */
