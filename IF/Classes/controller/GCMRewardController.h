//
//  GCMRewardController.h
//  IF
//
//  Created by fubin on 13-12-25.
//
//

#ifndef IF_GCMRewardController_h
#define IF_GCMRewardController_h

#include "CommonInclude.h"
#include "GCMRewardInfo.h"

class GCMRewardController : public CCObject{
public:
    static GCMRewardController* getInstance();
    static void purgeData();
    GCMRewardController();
    ~GCMRewardController();
    
    void initGCMRewardData(CCDictionary* dict);
    bool sendReward(string actId);
    CCArray* retReward(CCDictionary* dict);
    CCArray* retReward2(CCArray* arr, bool flyAni = false);
    void flyReward(CCArray* arr);
    void flyToolReward(CCArray* arr);
    CCArray* getFlyArr(CCArray* arr);
    std::string getPicByType(std::string type, int value);
    std::vector<GCMRewardInfo> m_rewardPool;
    bool isFirstOpen;
    
private:
    void aniComplete(CCObject* node);
};
#endif
