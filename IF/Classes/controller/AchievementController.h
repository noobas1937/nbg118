//
//  AchievementController.h
//  IF
//
//  Created by 李锐奇 on 15/3/18.
//
//

#ifndef __IF__AchievementController__
#define __IF__AchievementController__

#include "CommonInclude.h"
#include "AchievementInfo.h"
#include "MedalInfo.h"

#define ACHIEVEMENT_VISIBLE_STATE_CHANGE "achievementVisibleChange"
#define ACHIEVEMENT_STATE_CHAGE "achievementStateChange"
#define ACHIEVEMENT_MEDAL_GET "ACHIEVEMENT_MEDAL_GET"

enum AchievementFunctionState{
    FUNCTION_OFF,
    FUNCTION_ON,
};

class AchievementController :
public CCObject
{
public:
    static AchievementController *getInstance();
    void init();
    void updateAchievement(CCArray *arr);
    void updateAchievement(CCDictionary *dict, bool postNotification = true);
    void initMedalInfo(CCArray *arr);
    void updateMyMedalInfo();
    void refreshAllAchievementVisibleFlag();
    CCArray *getVisbleAchievement();
    std::string getKeyByItemId(std::string itemId);
    std::map<std::string, AchievementInfo> m_infos;
    std::map<string, MedalInfo> m_myMedalInfos;
    std::map<string, MedalInfo> m_otherMedalInfos;
    bool isDataBack;
    bool isNeedPostCompleteAchievementToGoogle;
    void purgeData();
    void getReward(std::string itemId);
    void getDataFromServer();
    void getMedalDataFormServer();
    void doWhenComplete(std::string itemId);
    void doOpenGooglePlay();
    void doOpenGooglePlayAchievement();
    void postCompleteAchievement(CCObject *obj = NULL);
    void postCompleteAchievementToGoogle();
    std::map<std::string, int> allCompelete;
    int openFlag;
    bool firstOpen;
    void setOpenFlag(int i);
    void getOtherMedalInfo(CCArray* a);
    CCArray* getOtherMedalInfo();
    CCArray* getMyMedalInfo();
    float getAchieveProgress();
    int getMedalComCnt();
    string getMedalIconName(string medalId, int type);
    void changePlayerMedalId();
    void firstOpenPopup();
    int getRewardAchieve();
    void getMedalIconColor(string medalId, map<int, vector<float>> &colorMap);
};

#endif /* defined(__IF__AchievementController__) */
