//
//  GetAllianceRankCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-8.
//
//

#ifndef __IF__GetAllianceRankCommand__
#define __IF__GetAllianceRankCommand__

#include "CommandBase.h"
#define RANK_LIST "al.rank"

class GetAllianceRankCommand: public CommandBase {
public:
    GetAllianceRankCommand(std::string allianceId);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_allianceId;
};
#endif /* defined(__IF__GetAllianceRankCommand__) */
