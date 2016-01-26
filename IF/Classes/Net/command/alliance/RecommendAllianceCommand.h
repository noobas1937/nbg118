//
//  ApplyAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#ifndef __IF__RecommendAllianceCommand__
#define __IF__RecommendAllianceCommand__
#include "CommandBase.h"

//#define FETCH_ALLIANCE "al.fetch"

#define RECOMMEND_ALLIANCE  "al.recommend"

class RecommendAllianceCommand: public CommandBase {
public:
    RecommendAllianceCommand(/*std::string allianceId*/);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_allianceId;
};
#endif /* defined(__IF__RecommendAllianceCommand__) */
