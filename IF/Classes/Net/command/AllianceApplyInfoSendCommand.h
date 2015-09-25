//
//  AllianceApplyInfoSendCommand.h
//  IF
//
//  Created by 〜TIAN~ on 15/8/5.
//
//

#ifndef __IF__AllianceApplyInfoSendCommand__
#define __IF__AllianceApplyInfoSendCommand__

#include "CommandBase.h"

#define ALLIANCE_APPLY_INFO_SEND "al.apply.message"
#define ALLIANCE_APPLY_INFO_GET "al.apply.message.get"

class AllianceApplyInfoSendCommand: public CommandBase {
public:
    AllianceApplyInfoSendCommand(std::string allianceId, std::string message) : CommandBase(ALLIANCE_APPLY_INFO_SEND){
        m_allianceId = allianceId;
        m_message = message;
        putParam("allianceId", CCString::create(allianceId));
        putParam("message", CCString::create(message));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_allianceId;
    std::string m_message;
};

class AllianceApplyInfoGetCommand: public CommandBase {
public:
    AllianceApplyInfoGetCommand(std::string allianceId, std::string uid): CommandBase(ALLIANCE_APPLY_INFO_GET){
        m_allianceId = allianceId;
        m_uid = uid;
        putParam("allianceId", CCString::create(allianceId));
        putParam("uid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_allianceId;
    std::string m_uid;
};
#endif /* defined(__IF__AllianceApplyInfoSendCommand__) */
