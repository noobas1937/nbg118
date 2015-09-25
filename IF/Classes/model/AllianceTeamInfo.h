//
//  AllianceTeamInfo.h
//  IF
//
//  Created by ganxiaohua on 14-8-18.
//
//

#ifndef __IF__AllianceTeamInfo__
#define __IF__AllianceTeamInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "YuanJunInfo.h"
#include "GlobalData.h"
#include "WorldController.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class AllianceTeamInfo:public cocos2d::CCObject
{
public:
    CC_SYNTHESIZE(string, m_attackUid, AttackUid);
    CC_SYNTHESIZE(string, m_attackName, AttackName);
    CC_SYNTHESIZE(string, m_attackAName, AttackAName);
    CC_SYNTHESIZE(string, m_attackAAbb, AttackAAbb);
    CC_SYNTHESIZE(string, m_targetPic, TargetPic);
    CC_SYNTHESIZE(int, m_attackPicVer, AttackPicVer);
    CC_SYNTHESIZE(string, m_targetAIcon, TargetAIcon);
    CC_SYNTHESIZE(double, m_waitTime, WaitTime);
    CC_SYNTHESIZE(double, m_createTime, CreateTime);
    CC_SYNTHESIZE(int, m_currSoldiers, CurrSoldiers);
    CC_SYNTHESIZE(int, m_maxSoldiers, MaxSoldiers);
    CC_SYNTHESIZE(string, m_targetUid, TargetUid);
    CC_SYNTHESIZE(int, m_targetPicVer, TargetPicVer);
    CC_SYNTHESIZE(int, m_targetId, TargetId);
    CC_SYNTHESIZE(int, m_targetType, TargetType);
    CC_SYNTHESIZE(int, m_attackId, AttackId);
    CC_SYNTHESIZE(string, m_targetName, TargetName);
    CC_SYNTHESIZE(string, m_targetAName, TargetAName);
    CC_SYNTHESIZE(int, m_targetCityLevel, TargetCityLevel);
    CC_SYNTHESIZE(string, m_targetAAbbr, TargetAAbbr);
    CC_SYNTHESIZE(int, m_type, Type);
    CC_SYNTHESIZE(string, m_teamId, TeamId);
    CC_SYNTHESIZE(string, m_attackAIcon, AttackAIcon);
    CC_SYNTHESIZE(int, m_mySide, MySide); //1 攻 2 守
    CC_SYNTHESIZE(int, m_reinfoceCount, ReinfoceCount);
    CC_SYNTHESIZE(int, m_reinforceMax, reinforceMax);
    
    CC_SYNTHESIZE(string, m_attackPic, AttackPic);
    CC_SYNTHESIZE(double, m_marchTime, MarchTime);
    
    CC_SYNTHESIZE(int, m_battleType, BattleType);//0 个人进攻， 1 个人防守，2 组队进攻 3 组队防守
    CC_SYNTHESIZE(int, m_monsterCount, MonsterCount);//大于1为打怪组队
    
    CC_SYNTHESIZE(string, m_marchId, marchId);
    CC_SYNTHESIZE(int, m_marchType, marchType);
    
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_member, Member);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_reinforce, Reinforce);
    CC_SYNTHESIZE(string, m_territoryFlag, TerritoryFlag);//true：领地战争
    CC_SYNTHESIZE(string, m_territoryType, TerritoryType);//tower堡垒 flag国旗
    CC_SYNTHESIZE(string, m_territoryName, TerritoryName);//领地名字
    CC_SYNTHESIZE(string, m_territoryPic, TerritoryPic);//领地图片
    CC_SYNTHESIZE(int, m_territoryStat, TerritoryStat);//领地状态
    CC_SYNTHESIZE(string, m_territoryAlAbbr, TerritoryAlAbbr);//领地联盟简称
    CC_SYNTHESIZE(double, m_territoryCrashTime, TerritoryCrashTime);//领地被摧毁时间
    CC_SYNTHESIZE(int, m_territoryLevel, TerritoryLevel);//领地等级
    string getAttAlliancePic();
    string getTargetAlliancePic();
    
    void parseInfo(CCDictionary* dict);
    void parseOnePersonInfo(CCDictionary* dict);
};
#endif /* defined(__IF__AllianceTeamInfo__) */
