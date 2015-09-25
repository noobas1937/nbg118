//
//  SetAllianceRankCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-5.
//
//

#ifndef __IF__SetAllianceRankCommand__
#define __IF__SetAllianceRankCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_SET_RANK "al.setrank"

class SetAllianceRankCommand: public CommandBase {
public:
    SetAllianceRankCommand(std::string playerId,int rank) : CommandBase(ALLIANCE_AL_SET_RANK),m_playerId(playerId),m_rank(rank){
        putParam("playerId", CCString::create(m_playerId.c_str()));
        putParam("rank", CCInteger::create(m_rank));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_playerId;
    int m_rank;
};

#endif /* defined(__IF__SetAllianceRankCommand__) */
