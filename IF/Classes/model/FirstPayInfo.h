//
//  firstPayInfo.h
//  IF
//
//  Created by lifangkai on 14-10-17.
//
//

#ifndef __IF__FirstPayInfo__
#define __IF__FirstPayInfo__
#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;


class FirstPayInfo : public CCObject{
public:
    CREATE_FUNC(FirstPayInfo);
    void parse(CCDictionary *dict);
    FirstPayInfo():rewardStatus(0),firstPayReward(NULL){};
    ~FirstPayInfo(){
        if(firstPayReward){
            firstPayReward->release();
            firstPayReward = NULL;
        };
    };

    int rewardStatus;
    CCArray* firstPayReward;

private:
    virtual bool init(){return true;};
};

#endif /* defined(__IF__firstPayInfo__) */
