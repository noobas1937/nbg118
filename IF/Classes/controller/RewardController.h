//
//  RewardController.h
//  IF
//
//  Created by 李锐奇 on 14-2-20.
//
//

#ifndef __IF__RewardController__
#define __IF__RewardController__

#include "CommonInclude.h"
#include "GCMRewardInfo.h"

class RewardController : public CCObject{
public:
    static RewardController *getInstance();
    std::string retReward(CCArray *arr, bool flyAni = true);
    std::string getPicByType(int type, int value);
    std::string getNameByType(int type, int value = 0);
    int getTypeByName(std::string name);
    std::string flyReward(CCArray *arr);
    CCArray *getRewardArr(CCDictionary *dict);
    CCArray *getBossReward(std::string itemId);
    
    CCArray *retRewardArr(CCArray *arr);
    std::string getRewardNameInArrayByIndex(CCArray *arr, int idx );
    int getRewardCountInArrayByIndex(CCArray *arr, int idx );
private:
    void aniComplete(CCNode *node);
};
#endif /* defined(__IF__RewardController__) */
