//
//  AppLibHelper.h
//  IF
//
//  Created by chenliang on 14/12/2.
//
//

#ifndef IF_AppLibHelper_h
#define IF_AppLibHelper_h

#include "CommonInclude.h"


class AppLibHelper
{
public:
    
    static void sendPayLog(const string &cost,const string &itemId);
    static void sendAdjustTrack(const string &track);
    static void relogin();
    static void updatePlatformScore(int score);

    static void triggerEventLoginComplete(const string &userId,const string &userName,const string &userLevel);
    static void triggerEventCompletedRegistration();
    static void triggerEventCompletedTutorial();
    static void triggerEventMainBuildingLevel(int level);
    static void triggerEventAchievedLevel(int level);
    static void triggerEventPurchaseInit(const string &cost,const string &itemId);
    static void triggerEventPurchase(const string &cost,const string &itemId);
    
    static vector<std::string> getServiceTerms(int type);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static void sendToBranch(std::string cmd, std::string value);
    static void initLibs();
    static string getLocalCountry();

    static void triggerEventActivate();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
};

#endif
