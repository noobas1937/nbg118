//
//  ActivityEventInfo.h
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#ifndef __IF__ActivityEventInfo__
#define __IF__ActivityEventInfo__

#include "CommonInclude.h"

class ActivityEventInfo : public CCObject{
public:
    CREATE_FUNC(ActivityEventInfo);
    void parse(CCDictionary *dict);
    ActivityEventInfo():id(string()),events(string()),curPhase(0),maxPhase(0),readyTime(0),beginTime(0),endTime(0),currentName(string()){};
    ~ActivityEventInfo(){};
    time_t readyTime;
    time_t beginTime;
    time_t endTime;
    std::string id;
    std::string events;
    std::string target;
    std::string reward;
    std::string currentName;
    int curPhase;
    int maxPhase;
    void reset();
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_rankReward, RankReward);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_totalRankReward, TotalRankReward);
private:
    virtual bool init(){return true;};
};
#endif /* defined(__IF__ActivityEventInfo__) */
