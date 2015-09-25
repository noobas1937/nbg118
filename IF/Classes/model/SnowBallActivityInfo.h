//
//  SnowBallActivityInfo.h
//  IF
//
//  Created by lifangkai on 14-12-3.
//
//

#ifndef __IF__SnowBallActivityInfo__
#define __IF__SnowBallActivityInfo__

#include "cocos2d.h"
USING_NS_CC;

class SnowBallActivityInfo : public CCObject{
public:
    CREATE_FUNC(SnowBallActivityInfo);
    void parse(CCDictionary *dict);
    SnowBallActivityInfo():refreshTime(0),ballCount(0),maxThrowCount(0),todayThrowCount(0){};
    ~SnowBallActivityInfo(){};
    time_t refreshTime;
    int ballCount;
    int maxThrowCount;
    int todayThrowCount;
    int goldArr[10];
    int expArr[10];

    
private:
    virtual bool init(){return true;};
};

#endif /* defined(__IF__SnowBallActivityInfo__) */
