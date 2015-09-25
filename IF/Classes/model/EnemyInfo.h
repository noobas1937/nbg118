//
//  EnemyInfo.h
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#ifndef IF_EnemyInfo_h
#define IF_EnemyInfo_h

#include "CommonInclude.h"
#include "GeneralInfo.h"

class EnemyInfo : public cocos2d::CCObject {
public:
    EnemyInfo(CCDictionary* dict);
    EnemyInfo():
    uuid("")
    ,uid("")
    ,name("")
    ,xy("")
    ,type(0)
    ,level(0)
    ,arrivalTime(0)
    ,soldiers(0)
    ,power(0)
    ,marchTime(0)
    ,stampTime(0)
    ,abbr("")
    ,pic("")
    ,allianceId("")
    ,picVer(0)
    ,teamId("")
    ,createTime(0)
    ,monsterCount(0)
    ,xyFrom("")
    ,xyTo("")
    ,defType(999)
    ,monsterArmyInfo(std::string(""))
    {};
    
    string uuid;
    string uid;//表里的id
    string name;
    string xy;
    int index;
    int type;
    int level;
    double arrivalTime;
    int soldiers;
    int power;
    int marchTime;
    double createTime;
    time_t stampTime;
    std::string abbr;
    std::string pic;
    std::string teamId;
    std::string allianceId;
    CCSafeObject<CCDictionary> genDic;
    map<int,int> army;
    map<string,int> hero;
    map<int,int>talent;
    map<int,int>science;
    vector<int> ablityTypes;
    int monsterCount;
    std::string monsterArmyInfo;
    int picVer;
    
    
    int indexFrom;
    string xyFrom;
    int defType;
    int indexTo;
    string xyTo;
};

#endif
