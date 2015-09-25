//
//  KingsGiftCommand.h
//  IF
//
//  Created by 李锐奇 on 15/1/26.
//
//

#ifndef __IF__KingsGiftCommand__
#define __IF__KingsGiftCommand__

#include "CommandBase.h"

#define GET_GIFTS_INFO "Get.King.Present.Info"
#define POST_GIFTS "Send.King.Present"
#define GET_HISTORY_INFO "Get.King.Present.Record"

class KingsGiftGetInfoCommand: public CommandBase {
public:
    KingsGiftGetInfoCommand() : CommandBase(GET_GIFTS_INFO){
        putParam("uid", CCString::create(GlobalData::shared()->playerInfo.uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class KingsGiftPostCommand: public CommandBase {
public:
    KingsGiftPostCommand(std::string uid, std::string rewardId, std::string name, std::string rewardName) : CommandBase(POST_GIFTS){
        putParam("toUserId", CCString::create(uid));
        putParam("presentId", CCInteger::create(atoi(rewardId.c_str())));
        m_name = name;
        m_rewardName = rewardName;
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_name;
    std::string m_rewardName;
};

class GetKingsGiftHistoryCommand: public CommandBase {
public:
    GetKingsGiftHistoryCommand() : CommandBase(GET_HISTORY_INFO){
        putParam("page", CCInteger::create(1));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__KingsGiftCommand__) */
