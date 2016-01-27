//
//  AchievementController.h
//  IF
//
//  Created by 李锐奇 on 15/3/18.
//
//

#ifndef __IF__RecommendAllianceController__
#define __IF__RecommendAllianceController__

#include "CommonInclude.h"

#include "AllianceInfo.h"

class RecommendAllianceController :
public CCObject
{
public:
    static RecommendAllianceController *getInstance();
    void init();
    void purgeData();
    void updateRecommendAllianceInfo(CCDictionary * pDict);
    void popRecommendAlliance(int createFlag);
    void onPlayerLevelUp(CCObject* obj);
    void checkToPopRecommendAlliance();
    void sendRecommendRequest();
    void checkToSendRecommendRequest();
    void onApplyRecommendAllianceFail();
    void resetData();
    string allianceId;
    string allianceName;
    string shortName;
    string language;
    string leaderName;
    int maxMember;
    int curMember;
    string allianceIcon;
    double fightPower;
    int isCreate;
    int applyRecommendAllianceFailTimes;
    bool isShowed;
    
    bool hasSendRequest;
    AllianceInfo *allInfo;
    bool toShowCreateAfterFailThreeTimes;
};

#endif /* defined(__IF__RecommendAllianceController__) */
