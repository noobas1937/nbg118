//
//  ActivityInfo.h
//  IF
//
//  Created by 李锐奇 on 14-4-3.
//
//

#ifndef __IF__ActivityInfo__
#define __IF__ActivityInfo__

#include "CommonInclude.h"

class ActivityInfo : public CCObject{
public:
    CREATE_FUNC(ActivityInfo);
    void parse(CCDictionary *dict);
    ~ActivityInfo();
    
    int currentScore;
    time_t startTime;
    time_t endTime;
    string activityId;
    string eventId;
    int type;
    int sumScore;
    string getScoreMeth;
    
    map<int, CCSafeObject<CCArray>> m_scoreRwdMap;
    map<int, int> m_rwdPriceMap;
    vector<int> m_tagVec;
    CCArray* getScroeRweMapByKey(int key);
private:
    virtual bool init(){return true;};
};

#endif /* defined(__IF__ActivityInfo__) */
