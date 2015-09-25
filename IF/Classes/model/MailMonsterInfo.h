//
//  MailMonsterInfo.h
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#ifndef __IF__MailMonsterInfo__
#define __IF__MailMonsterInfo__

#include "cocos2d.h"
USING_NS_CC;

class MailMonsterInfo : public CCNode{
public:
    static MailMonsterInfo* create();
    void parse(CCDictionary* dic);
    MailMonsterInfo():uid(""),type(18),createTime(0),normalReward(NULL){};
    ~MailMonsterInfo();
    
    std::string uid;        //
    int type;
    time_t createTime;
    std::string reportUid;
    CCArray* normalReward;    //
    int monsterCurrentHp;        //
    int monsterTotalHp;
    std::string monsterId;
    bool firstKill;
    int exp;
    int hurt;
    int survived;
    int dead;
    int total;
    int monsterDead;
    int monsterX;
    int monsterY;
    int selfX;
    int selfY;
    int selfPowerLost;
    int monsterPowerLost;
    int pointId;
    int monsterResult;//1.shoucha2.shibai3.shengli 4.wuxiao
    
};

#endif /* defined(__IF__MailMonsterInfo__) */
