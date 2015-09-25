//
//  CancelAllianceTeamCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-18.
//
//

#ifndef __IF__CancelAllianceTeamCommand__
#define __IF__CancelAllianceTeamCommand__

#include "CommandBase.h"

#define ALLIANCE_TEAM_CANCEL "alliance.team.cancel"

class CancelAllianceTeamCommand: public CommandBase {
public:
    CancelAllianceTeamCommand(std::string uid) : CommandBase(ALLIANCE_TEAM_CANCEL),m_uid(uid){
        putParam("teamUid", CCString::create(m_uid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};
#endif /* defined(__IF__CancelAllianceTeamCommand__) */
