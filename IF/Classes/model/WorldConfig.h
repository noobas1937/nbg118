//
//  WorldConfig.h
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#ifndef IF_WorldConfig_h
#define IF_WorldConfig_h

using namespace std;

struct WorldConfig {
    double distance[6];
    unsigned int fire[3];
    int fire1[4];
    unsigned int pveHitMax;
    unsigned int pveBuyMax;
    int stamineMax;
    int stamineCostPerTime;
    int boss_decr;
    int bossLimitNum;
    int stamineIntervalTime;
    int stamineIntervalNum;
    int reskey;
    map<unsigned int,unsigned int> pveBuyPrice;
    vector<int> monsterAttackTime;
    vector<int> relicConfig;
    vector<double> monsterAttackPower;
    int ckf_world_row;
};

#endif
