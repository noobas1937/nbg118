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
    int currentLevel;
    time_t startTime;
    time_t endTime;
    std::string activityId;
    int type;
    std::string lv;
    std::string score;
    std::string action;
    std::string prize;
    std::string rewardFlag;
    std::string board;
    std::string name;
    
    map<int, int> actionMap;
    CCSafeObject<CCArray> scoreArr;
    CCSafeObject<CCArray> prizeArr;
    CCSafeObject<CCArray> boardArr;
    int currentScore;
private:
    virtual bool init(){return true;};
};

#endif /* defined(__IF__ActivityInfo__) */
