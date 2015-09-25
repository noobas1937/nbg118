//
//  YuanJunInfo.h
//  IF
//
//  Created by ganxiaohua on 14-8-14.
//
//

#ifndef __IF__YuanJunInfo__
#define __IF__YuanJunInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "GlobalData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class YuanJunInfo:public cocos2d::CCObject
{
public:
    CC_SYNTHESIZE(int, m_rank, Rank);
    CC_SYNTHESIZE(std::string, m_name, Name);
    CC_SYNTHESIZE(string, m_uid, Uid);
    CC_SYNTHESIZE(string, m_uuid, Uuid);
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CC_SYNTHESIZE(string, m_pic, Pic);
    CC_SYNTHESIZE(int, m_picVer, picVer);
    CC_SYNTHESIZE(int, m_kills, Kills);
    CC_SYNTHESIZE(int, m_points, Points);
    CC_SYNTHESIZE(int, m_power, Power);
    CC_SYNTHESIZE(int, m_num, Num);
    CC_SYNTHESIZE(bool, m_open, Open);
    CC_SYNTHESIZE(int, m_index, Index);
    CC_SYNTHESIZE(string, m_teamID, TeamId);
    CC_SYNTHESIZE(double, m_arrivalTime, ArrivalTime);
    CC_SYNTHESIZE(double, m_startTime, StartTime);
    CC_SYNTHESIZE(int, m_state, State);
    CC_SYNTHESIZE(int, m_teamSoldier, TeamSoldier);
    CC_SYNTHESIZE(bool, m_isLeader, IsLeader);
    CC_SYNTHESIZE(bool, m_canUse, CanUse);
    
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_soldiers, Soldiers);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_generals, Generals);

    string getTeamPic();
    void parseInfo(CCDictionary* dict);
};
#endif /* defined(__IF__YuanJunInfo__) */
