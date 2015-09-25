
#ifndef __IF__MailBatchCommand__
#define __IF__MailBatchCommand__

#include "CommandBase.h"
#include "cocos2d.h"
#include "MailController.h"

#define MAIL_DELETE_BATCH_COMMAND "mail.delete.batch"
#define MAIL_REWARD_BATCH_COMMAND "mail.reward.batch"

class MailBatchDelCommand : public CommandBase{
public:
    MailBatchDelCommand(std::string uids, std::string types) : CommandBase(MAIL_DELETE_BATCH_COMMAND){
        if(!uids.empty()){
            this->putParam("uids",  CCString::create(uids));
        }
        if(!types.empty()){
            this->putParam("types",  CCString::create(types));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class MailBatchRewardCommand : public CommandBase{
public:
    MailBatchRewardCommand(std::string uids, std::string types) : CommandBase(MAIL_REWARD_BATCH_COMMAND){
        if(!uids.empty()){
            this->putParam("uids",  CCString::create(uids));
        }
        if(!types.empty()){
            this->putParam("types",  CCString::create(types));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__MailBatchCommand__) */
