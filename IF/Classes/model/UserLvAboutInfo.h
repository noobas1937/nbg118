//
//  UserLvAboutInfo.h
//  IF
//
//  Created by fubin on 13-10-17.
//
//

#ifndef IF_UserLvAboutInfo_h
#define IF_UserLvAboutInfo_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class UserLvAboutInfo {
public:
    UserLvAboutInfo():
    soldier_param(0)
    ,soldier_basics("0")
    ,k1(0)
    ,k2(0)
    ,cd_cost_k1(0)
    ,cd_cost_k2(0)
    ,cd_cost_k3(0)
    ,cd_cost_k4(0)
    ,glodenHammerTime(0)
    ,glod_cd_k2(0)
    ,resource_market_k1(0)
    ,resource_market_k2(0)
    ,resource_market_k3(0)
    ,resource_market_k4(0)
    ,player_max_k1(9999)
    ,player_max_k2(9999)
    ,new_login_k1(0)
    ,ranking_k3(0)
    ,effect_200(0)
    ,effect_201(0)
    ,effect_202(0)
    ,effect_203(0)
    ,effect_204(0)
    ,effect_205(0)
    ,effect_206(0)
    ,effect_207(0)
    {}
    //每小时产出
    float soldier_param;
    string soldier_basics;
    int k1;
    int k2;
    
    float cd_cost_k1;
    float cd_cost_k2;
    float cd_cost_k3;
    float cd_cost_k4;
    int glodenHammerTime;
    int glod_cd_k2;
    float auto_building_k2;
    
    int resource_market_k1;
    int resource_market_k2;
    int resource_market_k3;
    int resource_market_k4;
    
    int player_max_k1;
    int player_max_k2;
    int train_k2;
    int train_k3;
    int new_login_k1;
    int ranking_k3;
    
    float effect_200;
    float effect_201;
    float effect_202;
    float effect_203;
    float effect_204;
    float effect_205;
    float effect_206;
    float effect_207;
};


#endif /* defined(__IF__UserLvAboutInfo__) */
