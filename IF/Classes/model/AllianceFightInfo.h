//
//  AllianceFightInfo.h
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#ifndef __IF__AllianceFightInfo__
#define __IF__AllianceFightInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "GlobalData.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class AllianceFightInfo:public cocos2d::CCObject
{
public:
    CC_SYNTHESIZE(double, m_time, Time);
    CC_SYNTHESIZE(int, m_win, Win);//进攻方胜为0,失为1
    CC_SYNTHESIZE(int, m_type, Type);
    CC_SYNTHESIZE(int, m_role, Role);//0,攻击方；1，防御方
    CC_SYNTHESIZE(string, m_attackName, AttackName);
    CC_SYNTHESIZE(string, m_attackAID, AttackAID);
    CC_SYNTHESIZE(string, m_attackAlliance, AttackAlliance);
    CC_SYNTHESIZE(int, m_attackKill, AttackKill);
    CC_SYNTHESIZE(string, m_defName, DefName);
    CC_SYNTHESIZE(string, m_defAlliance, defAlliance);
    CC_SYNTHESIZE(string, m_defAID, DefAID);
    CC_SYNTHESIZE(string, m_reward, Reward);
    CC_SYNTHESIZE(string, m_attAsn, AttAsn);
    CC_SYNTHESIZE(string, m_defAsn, DefAsn);
    CC_SYNTHESIZE(int, m_defkill, DefKill);
    CC_SYNTHESIZE(bool, m_selfIsAtt, SelfIsAtt);//自己联盟是进攻方
    CC_SYNTHESIZE(string, m_monsterId, MonsterId);
    
    void parseInfo(string info);
};

#endif /* defined(__IF__AllianceFightInfo__) */
