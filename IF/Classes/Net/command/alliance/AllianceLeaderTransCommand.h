//
//  AllianceLeaderTransCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#ifndef __IF__AllianceLeaderTransCommand__
#define __IF__AllianceLeaderTransCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_LEADER_TRANS "al.leader.trans"
#define ALLIANCE_AL_ORDER_CMD "leader.command.send"

class AllianceLeaderTransCommand: public CommandBase {
public:
    AllianceLeaderTransCommand(std::string playerId) : CommandBase(ALLIANCE_AL_LEADER_TRANS),m_playerId(playerId){
        putParam("playerId", CCString::create(m_playerId.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_playerId;
};


class AllianceOrderCommand: public CommandBase {
public:
    AllianceOrderCommand(std::string playerId, int cmd) : CommandBase(ALLIANCE_AL_ORDER_CMD){
        putParam("uid", CCString::create(playerId));
        putParam("command", CCInteger::create(cmd));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__AllianceLeaderTransCommand__) */
