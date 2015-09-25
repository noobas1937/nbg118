//
//  JoinAllianceTeamCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-18.
//
//

#ifndef __IF__JoinAllianceTeamCommand__
#define __IF__JoinAllianceTeamCommand__

#include "CommandBase.h"

#define ALLIANCE_TEAM_JOIN "alliance.team.join"

class JoinAllianceTeamCommand: public CommandBase {
public:
    JoinAllianceTeamCommand(std::string uid) : CommandBase(ALLIANCE_TEAM_JOIN),m_uid(uid){
        putParam("uuid", CCString::create(m_uid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
#endif /* defined(__IF__JoinAllianceTeamCommand__) */
